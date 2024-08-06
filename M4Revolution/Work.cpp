#include "Work.h"

// acquire lock to prevent data race on completed
void Work::Event::setDone() {
	std::lock_guard<std::mutex> lock(mutex);
	done = true;
}

// allow one thread to wake up first
Work::Event::Event() {
	setDone();
	conditionVariable.notify_one();
}

// notify any remaining threads
Work::Event::~Event() {
	setDone();
	conditionVariable.notify_all();
}

// prevent spurious wakeup
void Work::Event::wait() {
	std::unique_lock<std::mutex> lock(mutex);

	while (!done) {
		conditionVariable.wait(lock);
	}

	// TODO: reset the event
	//done = false;
}

// wake up the next thread
void Work::Event::set() {
	setDone();
	conditionVariable.notify_one();
}

Work::Data::Data() {
}

Work::Data::Data(size_t size, POINTER pointer)
	: size(size),
	pointer(pointer) {
}

Work::BigFileTask::BigFileTask(std::ifstream &inputFileStream, Ubi::BigFile::File::POINTER_SET_MAP &fileVectorIteratorSetMap)
	: INPUT_POSITION(inputFileStream.tellg()),
	bigFile(Ubi::BigFile(inputFileStream, fileSystemSize, fileVectorIteratorSetMap)) {
}

Ubi::BigFile::File::SIZE Work::BigFileTask::getFileSystemSize() {
	return fileSystemSize;
}

Work::FileTask::FileTask(std::streampos bigFileInputPosition)
	: BIG_FILE_INPUT_POSITION(bigFileInputPosition) {
}

// TODO: this feels sketch, make sure it works
Work::FileTask::FileTask(std::streampos bigFileInputPosition, std::ifstream &inputFileStream, std::streamsize count)
	: BIG_FILE_INPUT_POSITION(bigFileInputPosition) {
	if (!count) {
		return;
	}

	const size_t BUFFER_SIZE = 0x10000;

	Work::Data::POINTER pointer = 0;
	std::streamsize countRead = BUFFER_SIZE;
	std::streamsize gcountRead = 0;

	do {
		countRead = (std::streamsize)min((size_t)count, (size_t)countRead);
		pointer = Work::Data::POINTER(new unsigned char[countRead]);

		readFileStreamPartial(inputFileStream, pointer.get(), countRead, gcountRead);

		if (!gcountRead) {
			break;
		}

		queue.emplace(gcountRead, pointer);

		if (count != -1) {
			count -= gcountRead;

			if (!count) {
				break;
			}
		}
	} while (countRead == gcountRead);

	if (count != -1) {
		if (count) {
			throw std::runtime_error("count must not be greater than file size");
		}
	}
}

// called in order to lock the data queue so we can add new data
// the Lock class ensures the writer thread will automatically wake up to write it after we add the new data
Work::Lock<Work::Data::QUEUE> Work::FileTask::lock() {
	return Lock<Data::QUEUE>(event, queue);
}

// called to signal to the writer thread that we are done adding new data
// the event is set in order to make sure it gets this message
void Work::FileTask::complete() {
	completed = true;
	event.set();
}

bool Work::FileTask::getCompleted() {
	return completed;
}

Work::Lock<Work::BigFileTask::VECTOR> Work::Tasks::bigFileLock() {
	return Lock<BigFileTask::VECTOR>(bigFileEvent, bigFileTaskVector);
}

Work::Lock<Work::FileTask::QUEUE> Work::Tasks::fileLock() {
	return Lock<FileTask::QUEUE>(fileEvent, fileTaskQueue);
}