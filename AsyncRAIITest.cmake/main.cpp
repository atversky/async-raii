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
#include "BufferedString.h"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>
#include <vector>

using namespace Util;
using namespace std;

/**
 * @brief main() - application entry point function for unit testing.
 *
 * @note Running single writter thread (writes buffer ever 35mSec),
 *       and 3 reader threads (each reading buffer every 2mSec)
 */
int main()
{
	BufferedString resource;
	AsyncRAII resourceWrapper(resource);
	std::mutex coutMutex;

	cout << endl << "Async RAII test output:" << endl;

	auto writeFunction = [&]()
	{
		static const unsigned char dummyStr[] = "STRING-";
		const int writterLoops(5);
		for (int i = 0; i < writterLoops; i++)
		{
			UString str(dummyStr);
			str += UString(1, static_cast<unsigned char>('0' + i + 1));
			resourceWrapper.write(str.c_str(), static_cast<unsigned long>(str.size()));
			this_thread::sleep_for(chrono::milliseconds(35));
		}
	};
	thread writterThread(writeFunction);

	auto readerFunction = [&]()
	{
		const int readerLoops(100);
		for (int i = 0; i < readerLoops; i++)
		{
			unsigned char* bufferPtr(nullptr);
			unsigned long size(0);
			(void)resourceWrapper.read(bufferPtr, size);
			UString buffer(bufferPtr, size);
			stringstream str;
			str << "READER: ";
			for_each(buffer.begin(), buffer.end(), [&](unsigned char c) { str << c;  });
			{
				std::lock_guard<std::mutex> guard(coutMutex);
				cout << str.str() << endl;
			}
			this_thread::sleep_for(chrono::milliseconds(2));
		}
	};
	thread readerThread1(readerFunction);
	thread readerThread2(readerFunction);
	thread readerThread3(readerFunction);

	writterThread.join();
	readerThread1.join();
	readerThread2.join();
	readerThread3.join();

	cout << endl;

	return 0;
}