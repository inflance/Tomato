#pragma once

#include "PublicSingeton.h"

namespace Tomato{

	namespace File {

		class FileDiolog : public PublicSingeton<FileDiolog>
		{
		public:
			static std::string SaveFile(const char* filter);
			static std::string OpenFile(const char* filter);

		};

	}
}