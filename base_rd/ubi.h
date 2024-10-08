#pragma once

#define BASE_RD_CALL __cdecl

#ifdef _WIN32
	#ifdef BASE_RD_LIBRARY
		#define BASE_RD_API __declspec(dllexport)
	#else
		#define BASE_RD_API __declspec(dllimport)
	#endif
#else
	#define BASE_RD_API
#endif

namespace ubi {
	class BASE_RD_API InputStream {
		public:
		unsigned long Read(unsigned char* buffer, unsigned long position, unsigned long size);
	};

	class BASE_RD_API InputFileStream {
		public:
		InputFileStream(char const* path);
		virtual ~InputFileStream();
		unsigned __int64 GetSize();
	};
}