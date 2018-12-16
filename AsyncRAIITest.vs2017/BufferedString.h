// MIT License
// 
// Copyright(c) 2018 Alex Tversky
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#pragma once

#include "IBuffer.h"
#include <string>

namespace Util
{
	typedef std::basic_string<unsigned char, std::char_traits<unsigned char>, std::allocator<unsigned char>> UString;

	/**
	 * @brief BufferedString class - implements synchronous IBuffer API for std::string
	 *
	 */
	class BufferedString : public IBuffer
	{
	public:
		BufferedString()
		{}
		virtual ~ BufferedString()
		{}

		unsigned long read(unsigned char*& bufferPtr, unsigned long& size) override
		{
			size = static_cast<unsigned long>(resource_.size());
			bufferPtr = const_cast<unsigned char*>(resource_.c_str());
			return size;
		}

		void write(const unsigned char* bufferPtr, const unsigned long size) override
		{
			resource_.assign(bufferPtr, size);
		}

	private:
		BufferedString(const BufferedString &other) = delete;
		BufferedString(const BufferedString &&other) = delete;
		BufferedString &operator=(const BufferedString &other) = delete;
		BufferedString &operator=(const BufferedString &&other) = delete;

	private:
		UString resource_;
	};
};

