#pragma once
#include "shared.h"
#include "PixelFormat.h"

namespace gfx_tools {
	GFX_TOOLS_RD_API void GFX_TOOLS_RD_CALL Init();
	GFX_TOOLS_RD_API void GFX_TOOLS_RD_CALL Shutdown();

	GFX_TOOLS_RD_API unsigned char* GFX_TOOLS_RD_CALL ConvertHeightMapIntoDuDvBumpMap(
		unsigned long width,
		unsigned long height,
		unsigned char* inputPointer,
		EnumPixelFormat inputEnumPixelFormat,
		unsigned long inputStride,
		unsigned char* outputPointer,
		EnumPixelFormat outputEnumPixelFormat,
		unsigned long outputStride
	);

	GFX_TOOLS_RD_API unsigned char* GFX_TOOLS_RD_CALL ConvertHeightMapIntoNormalMap(
		unsigned long width,
		unsigned long height,
		unsigned char* inputPointer,
		EnumPixelFormat inputEnumPixelFormat,
		unsigned long inputStride,
		unsigned char* outputPointer,
		EnumPixelFormat outputEnumPixelFormat,
		unsigned long outputStride,
		float strength
	);
};