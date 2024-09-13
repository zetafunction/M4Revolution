#include "PixelFormat.h"

namespace gfx_tools {
	PixelFormat PixelFormat::m_formatDescriptionTable[] = {
		{0, 0, 0, 0, 0, 0, 0, 0},
	};

	char* PixelFormat::ms_formatNames[] = {
		"PIXELFORMAT_UNKNOWN",
		"PIXELFORMAT_RGB_888",
		"PIXELFORMAT_ARGB_8888",
		"PIXELFORMAT_XRGB_8888",
		"PIXELFORMAT_RGB_565",
		"PIXELFORMAT_XRGB_1555",
		"PIXELFORMAT_ARGB_1555",
		"PIXELFORMAT_ARGB_4444",
		"PIXELFORMAT_RGB_332",
		"PIXELFORMAT_A_8",
		"PIXELFORMAT_ARGB_8332",
		"PIXELFORMAT_XRGB_4444",
		"PIXELFORMAT_ABGR_2_10_10_10",
		"PIXELFORMAT_GR_16_16",
		"PIXELFORMAT_AP_88",
		"PIXELFORMAT_P_8",
		"PIXELFORMAT_L_8",
		"PIXELFORMAT_AL_88",
		"PIXELFORMAT_AL_44",
		"PIXELFORMAT_VU_88",
		"PIXELFORMAT_LVU_655",
		"PIXELFORMAT_XLVU_8888",
		"PIXELFORMAT_QWVU_8888",
		"PIXELFORMAT_VU_16_16",
		"PIXELFORMAT_AWVU_2_10_10_10",
		"PIXELFORMAT_UYVY_8888",
		"PIXELFORMAT_YUY2_8888",
		"PIXELFORMAT_DXT1",
		"PIXELFORMAT_DXT2",
		"PIXELFORMAT_DXT3",
		"PIXELFORMAT_DXT4",
		"PIXELFORMAT_DXT5",
		"PIXELFORMAT_D_16_LOCKABLE",
		"PIXELFORMAT_D_32",
		"PIXELFORMAT_DS_15_1",
		"PIXELFORMAT_DS_24_8",
		"PIXELFORMAT_D_16",
		"PIXELFORMAT_DX_24_8",
		"PIXELFORMAT_DXS_24_4_4",
		"PIXELFORMAT_BGR_888",
		"PIXELFORMAT_ABGR_8888",
		"PIXELFORMAT_XBGR_8888",
		"PIXELFORMAT_BGR_565",
		"PIXELFORMAT_XBGR_1555",
		"PIXELFORMAT_ABGR_1555",
		"PIXELFORMAT_ABGR_4444",
		"PIXELFORMAT_BGR_233",
		"PIXELFORMAT_ABGR_8233",
		"PIXELFORMAT_XBGR_4444"
	};

	PixelFormat::PixelFormat(
		unsigned __int64 maskRed,
		unsigned __int64 maskGreen,
		unsigned __int64 maskBlue,
		unsigned __int64 maskAlpha,
		unsigned __int64 maskPalette,
		unsigned char bitsPerPixel,
		unsigned __int64 maskDepth,
		unsigned __int64 maskStencil
	)
	: maskRed(maskRed),
	maskGreen(maskGreen),
	maskBlue(maskBlue),
	maskAlpha(maskAlpha),
	maskPalette(maskPalette),
	bitsPerPixel(bitsPerPixel),
	maskDepth(maskDepth),
	maskStencil(maskStencil) {
		hasColor = maskRed || maskGreen || maskBlue || maskAlpha;
		hasBitsPerPixel = bitsPerPixel;
	}

	bool PixelFormat::HasRed() {
		return maskRed;
	}

	bool PixelFormat::HasGreen() {
		return maskGreen;
	}

	bool PixelFormat::HasBlue() {
		return maskBlue;
	}

	bool PixelFormat::HasAlpha() {
		return maskAlpha;
	}

	bool PixelFormat::HasPalette() {
		return maskPalette;
	}

	bool PixelFormat::HasDepth() {
		return maskDepth;
	}

	bool PixelFormat::HasStencil() {
		return maskStencil;
	}

	bool PixelFormat::HasColor() {
		return hasColor;
	}

	bool PixelFormat::HasBitsPerPixel() {
		return hasBitsPerPixel;
	}

	unsigned __int64 PixelFormat::GetMaskRed() {
		return maskRed;
	}

	unsigned __int64 PixelFormat::GetMaskGreen() {
		return maskGreen;
	}

	unsigned __int64 PixelFormat::GetMaskBlue() {
		return maskBlue;
	}

	unsigned __int64 PixelFormat::GetMaskAlpha() {
		return maskAlpha;
	}

	unsigned __int64 PixelFormat::GetMaskPalette() {
		return maskPalette;
	}

	unsigned char PixelFormat::GetBitsPerPixel() {
		return bitsPerPixel;
	}

	unsigned __int64 PixelFormat::GetMaskDepth() {
		return maskDepth;
	}

	unsigned __int64 PixelFormat::GetMaskStencil() {
		return maskStencil;
	}

	PixelFormat* PixelFormat::GetPixelFormat(EnumPixelFormat enumPixelFormat) {
		return &m_formatDescriptionTable[enumPixelFormat];
	}

	EnumPixelFormat PixelFormat::GetPixelFormatWithAlpha(EnumPixelFormat enumPixelFormat) {
		const ENUM_PIXEL_FORMAT_MAP ENUM_PIXEL_FORMAT_WITH_ALPHA_MAP = {
			{PIXELFORMAT_XRGB_8888, PIXELFORMAT_ARGB_8888},
			{PIXELFORMAT_XRGB_1555, PIXELFORMAT_ARGB_1555},
			{PIXELFORMAT_XRGB_4444, PIXELFORMAT_ARGB_4444},
			{PIXELFORMAT_XBGR_8888, PIXELFORMAT_ABGR_8888},
			{PIXELFORMAT_XBGR_1555, PIXELFORMAT_ABGR_1555},
			{PIXELFORMAT_XBGR_4444, PIXELFORMAT_ABGR_4444}
		};

		ENUM_PIXEL_FORMAT_MAP::const_iterator enumPixelFormatMapIterator = ENUM_PIXEL_FORMAT_WITH_ALPHA_MAP.find(enumPixelFormat);
		return enumPixelFormatMapIterator == ENUM_PIXEL_FORMAT_WITH_ALPHA_MAP.end() ? enumPixelFormat : enumPixelFormatMapIterator->second;
	}

	EnumPixelFormat PixelFormat::GetPixelFormatWithoutAlpha(EnumPixelFormat enumPixelFormat) {
		const ENUM_PIXEL_FORMAT_MAP ENUM_PIXEL_FORMAT_WITHOUT_ALPHA_MAP = {
			{PIXELFORMAT_ARGB_8888, PIXELFORMAT_XRGB_8888},
			{PIXELFORMAT_ARGB_1555, PIXELFORMAT_XRGB_1555},
			{PIXELFORMAT_ARGB_4444, PIXELFORMAT_XRGB_4444},
			{PIXELFORMAT_ABGR_8888, PIXELFORMAT_XBGR_8888},
			{PIXELFORMAT_ABGR_1555, PIXELFORMAT_XBGR_1555},
			{PIXELFORMAT_ABGR_4444, PIXELFORMAT_XBGR_4444}
		};

		ENUM_PIXEL_FORMAT_MAP::const_iterator enumPixelFormatMapIterator = ENUM_PIXEL_FORMAT_WITHOUT_ALPHA_MAP.find(enumPixelFormat);
		return enumPixelFormatMapIterator == ENUM_PIXEL_FORMAT_WITHOUT_ALPHA_MAP.end() ? enumPixelFormat : enumPixelFormatMapIterator->second;
	}

	char* PixelFormat::GetPixelFormatString(EnumPixelFormat enumPixelFormat) {
		return ms_formatNames[enumPixelFormat];
	}
};