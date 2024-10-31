#pragma once
#include "shared.h"
#include "PixelFormat.h"

namespace gfx_tools {
	union FormatHint {
		static const int HINT_NONE = 0;
		static const int HINT_ALPHA = 1;
		static const int HINT_LUMINANCE = 2;

		int hint : 3;

		EnumPixelFormat GetPixelFormat(uint32_t bits, bool hasAlpha) const;

		private:
		typedef std::map<int, EnumPixelFormat> HINT_PIXELFORMAT_MAP;

		static const HINT_PIXELFORMAT_MAP HINT_PIXELFORMAT_8_MAP;
	};
}