#pragma once
#include "shared.h"
#include "PixelFormat.h"

namespace gfx_tools {
	// TODO these will be imported from M4Image
	struct Color16 {
		unsigned char channels[2] = {};
	};

	struct Color32 {
		unsigned char channels[4] = {};
	};

	/*
	void GFX_TOOLS_RD_API Init();
	void GFX_TOOLS_RD_API Shutdown();
	*/

	GFX_TOOLS_RD_API unsigned char* GFX_TOOLS_RD_CALL ConvertHeightMapIntoDuDvBumpMap(
		unsigned long width,
		unsigned long height,
		unsigned char* inputBuffer,
		EnumPixelFormat inputEnumPixelFormat,
		unsigned long inputStride,
		unsigned char* outputBuffer,
		EnumPixelFormat outputEnumPixelFormat,
		unsigned long outputStride
	);

	/*
	GFX_TOOLS_RD_API unsigned char* GFX_TOOLS_RD_CALL ConvertHeightMapIntoNormalMap(
		unsigned long width,
		unsigned long height,
		unsigned char* inputBuffer,
		EnumPixelFormat inputEnumPixelFormat,
		unsigned long inputStride,
		unsigned char* outputBuffer,
		EnumPixelFormat outputEnumPixelFormat,
		unsigned long outputStride,
		float
	);
	*/
};