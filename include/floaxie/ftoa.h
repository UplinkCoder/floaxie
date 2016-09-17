/*
 * Copyright 2015 Alexey Chernov <4ernov@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef FLOAXIE_FTOA_H
#define FLOAXIE_FTOA_H

#include <cmath>
#include <cstddef>
#include <cassert>

#include <floaxie/grisu.h>
#include <floaxie/prettify.h>

namespace floaxie
{
	constexpr std::size_t max_buffer_size() noexcept
	{
		// digits, '.' (or 'e' plus three-digit power with optional sign) and '\0'
		return max_digits() + 1 + 1 + 3 + 1;
	}

	template<typename FloatType> inline void ftoa(FloatType v, char* buffer) noexcept
	{
		assert(!std::isnan(v));
		assert(!std::isinf(v));

		if (v == 0)
		{
			buffer[0] = '0';
			buffer[1] = '.';
			buffer[2] = '0';
			buffer[3] = '\0';
		}
		else
		{
			*buffer = '-';
			buffer += v < 0;

			constexpr int alpha(-35), gamma(-32);
			constexpr unsigned int decimal_scientific_threshold(16);

			int len, K;

			grisu2<alpha, gamma>(v, buffer, &len, &K);
			prettify<decimal_scientific_threshold>(buffer, len, K);
		}
	}
}

#endif // FLOAXIE_FTOA_H
