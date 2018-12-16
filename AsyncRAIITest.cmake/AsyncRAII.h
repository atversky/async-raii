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
#include <atomic>
#include <mutex>

namespace Util
{
	/**
	 * @brief AsyncRAII class - asynchronous RAII for reading writting any rsource of type IBuffer.
	 *
	 * @note Any number of callers can simultaneously take the lock for reading,
	 *       as long as nothing currently holds the lock for writing,
	 *       or is blocked waiting to take the lock for writing.
	 *       Multiple callers should not hold the write-lock at the same time,
	 *       that a read-lock never be held at the same time as the write-lock,
	 *       and that an attempt to take the write-lock should block future attempts to take a read lock
	 *       until the write-lock is eventually taken and released.
	 */
	class AsyncRAII : public IBuffer
	{
	public:
		AsyncRAII(IBuffer& resource);
		~AsyncRAII();

		unsigned long read(unsigned char*& bufferPtr, unsigned long& size) override;
		void write(const unsigned char* bufferPtr, const unsigned long size) override;

	private:
		AsyncRAII(const AsyncRAII &other) = delete;
		AsyncRAII(const AsyncRAII &&other) = delete;
		AsyncRAII &operator=(const AsyncRAII &other) = delete;
		AsyncRAII &operator=(const AsyncRAII &&other) = delete;

	private:
		IBuffer& resourceRef_;
		std::atomic<int> readersCounter_;
		std::recursive_mutex writtersMutex_;
		bool terminate_;
	};
};
