/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

#pragma once

namespace lemons
{

template <typename T = int>
void multiThreadedFor (std::function<void (T idx)>&& callback,
					   T start, T end, T interval = 1,
					   juce::ThreadPool* threadPool = nullptr)
{
	if (threadPool == nullptr)
	{
		for (auto i = start; i < end; i += interval)
			callback (i);

		return;
	}

	const auto num = threadPool->getNumThreads();

	juce::WaitableEvent wait;
	std::atomic<int>	threadsRunning { num };

	for (auto i = 0; i < num; i++)
	{
		threadPool->addJob ([i, &callback, &wait, &threadsRunning, start, end, interval, num]
							{
			for (auto j = start + interval * i; j < end; j += interval * num)
				callback (j);

			if (--threadsRunning == 0)
				wait.signal(); });
	}

	wait.wait();
}

}  // namespace lemons
