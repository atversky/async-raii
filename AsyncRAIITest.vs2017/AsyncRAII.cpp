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

#include "AsyncRAII.h"
#include <chrono>
#include <thread>

namespace Util
{
	AsyncRAII::AsyncRAII(IBuffer& resource)
		: resourceRef_(resource)
		, readersCounter_(0)
		, terminate_(false)
	{

	}

	AsyncRAII::~AsyncRAII()
	{
		terminate_ = true;

		// Until until all witters and readers completed
		std::lock_guard<std::recursive_mutex> guard(writtersMutex_);
		const long timeSliceMilliseconds(25);
		while (0 < readersCounter_.load(std::memory_order_acquire))
		{
			std::this_thread::sleep_for(
				std::chrono::milliseconds(timeSliceMilliseconds));
		}
	}

	unsigned long AsyncRAII::read(unsigned char*& bufferPtr, unsigned long& size)
	{
		unsigned long resourceSize(0);
		if (!terminate_)
		{
			std::lock_guard<std::recursive_mutex> guard(writtersMutex_);
			readersCounter_++;
			resourceSize = resourceRef_.read(bufferPtr, size);
			readersCounter_--;
		}

		return resourceSize;
	}

	void AsyncRAII::write(const unsigned char* bufferPtr, const unsigned long size)
	{
		if (!terminate_)
		{
			std::lock_guard<std::recursive_mutex> guard(writtersMutex_);
			const long timeSliceMilliseconds(25);
			while (0 < readersCounter_.load(std::memory_order_acquire))
			{
				std::this_thread::sleep_for(
					std::chrono::milliseconds(timeSliceMilliseconds));
			}
			resourceRef_.write(bufferPtr, size);
		}
	}
};
