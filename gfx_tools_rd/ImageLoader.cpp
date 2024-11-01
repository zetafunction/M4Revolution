#include "ImageLoader.h"
#include <M4Image/M4Image.h>

namespace gfx_tools {
	RawBuffer::SIZE ImageLoader::GetRawBufferTotalSize() {
		return rawBufferTotalSize;
	}

	bool ImageLoader::GetImageInfo(ImageInfo &imageInfo) {
		MAKE_SCOPE_EXIT(imageInfoScopeExit) {
			imageInfo = ImageInfo();
		};

		if (!validatedImageInfoOptional.has_value()) {
			GetImageInfoImpEx();

			if (!validatedImageInfoOptional.has_value()) {
				return false;
			}
		}

		imageInfo = validatedImageInfoOptional.value().Get();
		imageInfoScopeExit.dismiss();
		return true;
	}

	void ImageLoader::SetPixelFormat(EnumPixelFormat enumPixelFormat) {
		if (validatedImageInfoOptional.has_value()) {
			validatedImageInfoOptional.value().OverwritePixelFormat(enumPixelFormat);
		}
	}

	ImageLoader::~ImageLoader() {
		if (refCountedPointer) {
			refCountedPointer->Release();
		}
	}

	ImageLoaderMultipleBuffer::SIZE ImageLoaderMultipleBuffer::GetNumberOfRawBuffers() {
		return numberOfRawBuffers;
	}

	ImageLoaderMultipleBuffer::~ImageLoaderMultipleBuffer() {
		for (LOD i = 0; i < numberOfRawBuffers; i++) {
			RawBufferEx &rawBuffer = rawBuffers[i];

			if (rawBuffer.owner) {
				M4Image::allocator.freeSafe(rawBuffer.pointer);
			}
		}
	}

	void ImageLoaderMultipleBuffer::SetHint(FormatHint formatHint) {
		this->formatHint = formatHint;
	}

	void ImageLoaderMultipleBuffer::GetLOD(LOD lod, RawBuffer::POINTER pointer, SIZE stride, SIZE rows) {
		const RawBufferEx &RAW_BUFFER = rawBuffers[lod];

		if (!validatedImageInfoOptional.has_value()) {
			GetImageInfoImpEx();
		}

		const ImageInfo &IMAGE_INFO = validatedImageInfoOptional.value().Get();

		if (rows < IMAGE_INFO.textureHeight) {
			throw std::invalid_argument("rows must not be less than Texture Height");
		}

		size_t m4ImageStride = RAW_BUFFER.stride;

		if (m4ImageStride) {
			const M4Image RAW_BUFFER_M4_IMAGE(
				uncompressedImageInfo.textureWidth,
				uncompressedImageInfo.textureHeight,
				m4ImageStride,
				uncompressedImageInfo.GetRequestedColorFormat(),
				RAW_BUFFER.pointer
			);

			m4ImageStride = stride;

			M4Image m4Image(
				IMAGE_INFO.textureWidth,
				IMAGE_INFO.textureHeight,
				m4ImageStride,
				IMAGE_INFO.GetRequestedColorFormat(),
				pointer
			);

			m4Image.blit(RAW_BUFFER_M4_IMAGE);
			return;
		}

		m4ImageStride = stride;

		M4Image m4Image(
			IMAGE_INFO.textureWidth,
			IMAGE_INFO.textureHeight,
			m4ImageStride,
			IMAGE_INFO.GetRequestedColorFormat(),
			pointer
		);

		m4Image.load(RAW_BUFFER.pointer, RAW_BUFFER.size, GetExtension());
	}

	void ImageLoaderMultipleBuffer::ResizeLOD(
		LOD lod,
		RawBuffer::POINTER pointer,
		SIZE stride,
		SIZE rows,
		Q_FACTOR qFactor,
		ImageInfo &imageInfo,
		DIMENSION textureWidth,
		DIMENSION textureHeight,
		ares::RectU32* rectU32Pointer
	) {
		if (rows < imageInfo.textureHeight) {
			throw std::invalid_argument("rows must not be less than Texture Height");
		}

		uncompressedImageInfo = imageInfo;
		uncompressedImageInfo.textureWidth = (ImageInfo::DIMENSION)textureWidth;
		uncompressedImageInfo.textureHeight = (ImageInfo::DIMENSION)textureHeight;

		size_t m4ImageStride = stride;

		const M4Image M4IMAGE(
			imageInfo.textureWidth,
			imageInfo.textureHeight,
			m4ImageStride,
			imageInfo.GetColorFormat(),
			pointer
		);

		RawBuffer::POINTER rawBufferPointer = 0;
		m4ImageStride = 0;

		M4Image uncompressedM4Image(
			uncompressedImageInfo.textureWidth,
			uncompressedImageInfo.textureHeight,
			m4ImageStride,
			uncompressedImageInfo.GetRequestedColorFormat(),
			rawBufferPointer
		);

		uncompressedM4Image.blit(M4IMAGE);
		
		RawBufferEx rawBuffer(
			rawBufferPointer,
			(RawBuffer::SIZE)(m4ImageStride * uncompressedImageInfo.textureHeight),
			true,
			m4ImageStride
		);

		SetLODRawBufferImpEx(lod, rawBuffer, 0);
	}

	void ImageLoaderMultipleBuffer::SetLOD(
		LOD lod,
		RawBuffer::POINTER pointer,
		SIZE stride,
		SIZE rows,
		Q_FACTOR qFactor,
		ImageInfo &imageInfo,
		ares::RectU32* rectU32Pointer
	) {
		ResizeLOD(lod, pointer, stride, rows, qFactor, imageInfo, imageInfo.textureWidth, imageInfo.textureHeight, rectU32Pointer);
	}

	RawBuffer::POINTER ImageLoaderMultipleBuffer::CreateLODRawBuffer(LOD lod, RawBuffer::SIZE size) {
		RawBuffer rawBuffer((RawBuffer::POINTER)M4Image::allocator.mallocSafe(size), size, true);
		SetLODRawBufferImp(lod, rawBuffer, 0);
		return rawBuffer.pointer;
	}

	void ImageLoaderMultipleBuffer::SetLODRawBuffer(LOD lod, RawBuffer::POINTER pointer, RawBuffer::SIZE size, ubi::RefCounted* refCountedPointer) {
		RawBuffer rawBuffer(pointer, size, false);
		SetLODRawBufferImp(lod, rawBuffer, refCountedPointer);
	}

	void ImageLoaderMultipleBuffer::GetLODRawBuffer(LOD lod, RawBuffer::POINTER &pointer, RawBuffer::SIZE &size) {
		// TODO
	}

	RawBuffer::POINTER ImageLoaderMultipleBuffer::GetLODRawBuffer(LOD lod) {
		// TODO
		return 0;
	}

	bool ImageLoaderMultipleBuffer::GetImageInfoImp(ValidatedImageInfo &validatedImageInfo) {
		MAKE_SCOPE_EXIT(validatedImageInfoScopeExit) {
			validatedImageInfo = ValidatedImageInfo();
		};

		GetImageInfoImpEx();

		if (validatedImageInfoOptional.has_value()) {
			validatedImageInfo = validatedImageInfoOptional.value();
			validatedImageInfoScopeExit.dismiss();
			return true;
		}
		return false;
	}

	void ImageLoaderMultipleBuffer::SetLODRawBufferImp(LOD lod, RawBuffer value, ubi::RefCounted* refCountedPointer) {
		SetLODRawBufferImpEx(lod, value, refCountedPointer);
	}

	const L_TCHAR* ImageLoaderMultipleBuffer::GetExtension() {
		return 0;
	}

	L_INT ImageLoaderMultipleBuffer::GetFormat() {
		return 0;
	}

	L_INT ImageLoaderMultipleBuffer::CreateBitmapHandle(LOD lod, HANDLE &bitmapHandlePointer) {
		// in this implementation we have no concept of a bitmap handle
		bitmapHandlePointer = 0;
		return SUCCESS;
	}

	void ImageLoaderMultipleBuffer::GetImageInfoImpEx() {
		MAKE_SCOPE_EXIT(validatedImageInfoOptionalScopeExit) {
			validatedImageInfoOptional = std::nullopt;
		};

		const LOD MAIN_LOD = 0;
		const RawBufferEx &MAIN_RAW_BUFFER = rawBuffers[MAIN_LOD];

		// the main raw buffer's pointer is required
		if (!MAIN_RAW_BUFFER.pointer) {
			return;
		}

		const size_t BYTES = 3;
		const int VOLUME_EXTENT = 1;

		#define LOD_SIZE_IN_BYTES(bits, textureWidth, textureHeight, volumeExtent) (((bits) >> BYTES) * (textureWidth) * (textureHeight) * (volumeExtent))

		const char* extension = GetExtension();
		uint32_t bits = 0;
		int textureWidth = 0;
		int textureHeight = 0;
		ValidatedImageInfo::SIZE_IN_BYTES sizeInBytes = 0;

		if (MAIN_RAW_BUFFER.stride) {
			validatedImageInfoOptional.emplace(uncompressedImageInfo);
			sizeInBytes = uncompressedImageInfo.lodSizesInBytes[MAIN_LOD];
		} else {
			bool isAlpha = false;

			try {
				M4Image::getInfo(MAIN_RAW_BUFFER.pointer, MAIN_RAW_BUFFER.size, extension, &isAlpha, &bits, &textureWidth, &textureHeight);
			} catch (...) {
				return;
			}

			validatedImageInfoOptional.emplace(textureWidth, textureHeight, VOLUME_EXTENT, formatHint.GetEnumPixelFormat(isAlpha, bits), formatHint);
			bits = validatedImageInfoOptional.value().GetBitsPerPixel();
			sizeInBytes = LOD_SIZE_IN_BYTES(bits, textureWidth, textureHeight, VOLUME_EXTENT);
		}

		ValidatedImageInfo &validatedImageInfo = validatedImageInfoOptional.value();
		validatedImageInfo.SetNumberOfLOD((LOD)numberOfRawBuffers);
		validatedImageInfo.SetLodSizeInBytes(MAIN_LOD, sizeInBytes);

		// the following should fail only if we fail to get info
		// we are allowed to have buffers with null pointers, with zero sized images
		// it is only a failure if there is an invalid image
		for (LOD i = MAIN_LOD + 1; i < numberOfRawBuffers; i++) {
			MAKE_SCOPE_EXIT(setLodSizeInBytesScopeExit) {
				validatedImageInfo.SetLodSizeInBytes(i, 0);
			};

			const RawBufferEx &RAW_BUFFER = rawBuffers[i];

			if (!RAW_BUFFER.pointer) {
				continue;
			}

			if (RAW_BUFFER.stride) {
				validatedImageInfo.SetLodSizeInBytes(i, uncompressedImageInfo.lodSizesInBytes[i]);
				setLodSizeInBytesScopeExit.dismiss();
			} else {
				try {
					M4Image::getInfo(RAW_BUFFER.pointer, RAW_BUFFER.size, extension, 0, &bits, &textureWidth, &textureHeight);
				} catch (...) {
					return;
				}

				validatedImageInfo.SetLodSizeInBytes(i, LOD_SIZE_IN_BYTES(bits, textureWidth, textureHeight, VOLUME_EXTENT));
				setLodSizeInBytesScopeExit.dismiss();
			}
		}

		validatedImageInfoOptionalScopeExit.dismiss();
	}

	void ImageLoaderMultipleBuffer::SetLODRawBufferImpEx(LOD lod, const RawBufferEx &value, ubi::RefCounted* refCountedPointer) {
		RawBufferEx &rawBuffer = rawBuffers[lod];

		if (rawBuffer.owner) {
			M4Image::allocator.freeSafe(rawBuffer.pointer);
		}

		rawBufferTotalSize += value.size - rawBuffer.size;
		rawBuffer = value;

		if (this->refCountedPointer != refCountedPointer) {
			if (this->refCountedPointer) {
				this->refCountedPointer->Release();
			}

			this->refCountedPointer = refCountedPointer;

			if (this->refCountedPointer) {
				this->refCountedPointer->AddRef();
			}
		}
	}
}