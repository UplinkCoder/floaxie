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

#ifndef FLOAXIE_TAKE_APART_H
#define FLOAXIE_TAKE_APART_H

#include <cstddef>
#include <cctype>
#include <cassert>

#include <limits>

#include <iostream>

namespace floaxie
{
	namespace detail
	{
		int tiny_atoi(const char* buffer, int len)
		{
			assert(len <= 3);
			int ret(0);
			int multiplier(1);
			for (int pos = len - 1; pos >= 0; --pos)
			{
				ret += multiplier * (buffer[pos] - '0');
				multiplier *= 10;
			}

			return ret;
		}
	}

	constexpr std::size_t max_buffer_length(32);

	void take_apart(const char* str, const char** str_end, char* buffer, bool* sign, int* len, int* K)
	{
		char* mant = buffer;
		char exp[max_buffer_length];

		char* active_buffer(mant);

		bool mantissa_sign(true), exp_sign(true);
		bool* active_sign(&mantissa_sign);

		std::size_t mantissa_len(0), exp_len(0);
		std::size_t* active_len(&mantissa_len);

		std::size_t dot_pos(std::numeric_limits<std::size_t>::max());
		std::size_t dot_left_margin(0);
		std::size_t e_pos(std::numeric_limits<std::size_t>::max());

		std::size_t offset = 2048;
		std::size_t leading_zeros_cnt(0);

		for (std::size_t pos = 0; pos < offset; ++pos)
		{
			const char c = str[pos];

			switch (c)
			{
			case '0':
				if (!(*active_len))
				{
					if (dot_pos != std::numeric_limits<std::size_t>::max())
					{
						++leading_zeros_cnt;
					}
					break;
				}
				// fall down

			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if ((*active_len) < max_buffer_length)
				{
					active_buffer[(*active_len)++] = c;
				}
				else
				{
					active_buffer[max_buffer_length - 1] = (!active_buffer[max_buffer_length - 1]) & (c - '0');
				}
				break;

			case '.':
				if (dot_pos == std::numeric_limits<std::size_t>::max())
				{
					dot_pos = (*active_len);
				}
				else
				{
					offset = pos;
				}
				break;

			case 'e':
				if (e_pos == std::numeric_limits<std::size_t>::max())
				{
					e_pos = pos;
					active_buffer[(*active_len)] = 0;
					dot_left_margin = leading_zeros_cnt;
					active_buffer = exp;
					active_sign = &exp_sign;
					active_len = &exp_len;
				}
				else
				{
					offset = pos;
				}
				break;

			case '-':
			case '+':
				if (pos == 0 || pos == e_pos + 1)
				{
					*active_sign = static_cast<bool>('-' - c); // '+' => true, '-' => false
				}
				else
				{
					offset = pos;
				}
				break;

			case 0:
				active_buffer[(*active_len)] = 0;

				offset = pos;

			default:
				offset = pos;
				break;
			}
		}

		if (e_pos == std::numeric_limits<std::size_t>::max() &&
			dot_left_margin != leading_zeros_cnt)
		{
			dot_left_margin = leading_zeros_cnt;
		}

		if (exp_len > 3)
		{
			*str_end = str;
			*len = 0;
			return;
		}

		*str_end = str + offset;

		*K = detail::tiny_atoi(exp, exp_len);

		const bool dot_set(dot_pos != std::numeric_limits<std::size_t>::max());

		while (mant[mantissa_len - 1] == '0')
		{
			--mantissa_len;
			if (mantissa_len + dot_left_margin < dot_pos)
				++(*K);
		}

		mant[mantissa_len] = 0;

		if (!exp_sign)
		{
			*K = -*K;
		}

		if (dot_set)
		{
			*K -= mantissa_len + dot_left_margin - dot_pos;
		}

		*sign = mantissa_sign;
		*len = mantissa_len;
	}
}

#endif // FLOAXIE_TAKE_APART_H
