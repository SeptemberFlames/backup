#ifndef _ASYNC_THREAD_POOL_H__
#define _ASYNC_THREAD_POOL_H__

#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <future>
#include <functional>

struct CTask
{
	std::function<bool()> task;
	std::function<void()> callback;
};

class CThreadTask
{
public:
	bool m_bRuning;

public:
	CThreadTask() 
	{
		m_bRuning = true;
	}

	~CThreadTask()
	{
		m_bRuning = false;
		m_condition.notify_all();
		m_thread.join();
	}

	void init()
	{
		m_thread = std::thread([this]()
		{
			while (true)
			{
				std::unique_lock<std::mutex> lock(m_queueMutex);

				m_condition.wait(lock, [this] {
					if (!m_bRuning) {
						return true;
					}
					return !m_taskQueue.empty();
				});

				if (!m_bRuning) {
					break;
				}

				auto task = m_taskQueue.front();
				m_taskQueue.pop();

				auto result = task.task();
				if (result) 
				{
					if (task.callback != nullptr)
					{
						task.callback();
					}
				}
			}
		});
	}

	void add(std::function<bool()> func, std::function<void()> callback = nullptr)
	{
		CTask task;
		task.task = func;
		task.callback = callback;
		m_taskQueue.push(task);
		m_condition.notify_one();
	}

	int getSize()
	{
		return m_taskQueue.size();
	}

	void stop()
	{
		m_bRuning = false;
		m_condition.notify_all();
		m_thread.join();
	}

private:
	std::thread m_thread;
	std::queue<CTask> m_taskQueue;

	std::mutex m_queueMutex;
	std::condition_variable m_condition;
};

class CThreadPool
{
public:
	CThreadPool() {}

	~CThreadPool()
	{
		std::lock_guard<std::mutex> lk(m_workMutex);

		for (auto& t : _workers)
		{
			t->stop();
		}
		_workers.clear();
	}

	void init(int size)
	{
		std::lock_guard<std::mutex> lk(m_workMutex);

		for (int i = 0; i < size; ++i)
		{
			CThreadTask *task = new CThreadTask;
			task->init();
			_workers.emplace_back(task);
		}
	}
	
	void stop()
	{
		std::lock_guard<std::mutex> lk(m_workMutex);

		for (auto& t : _workers)
		{
			t->stop();
		}
		_workers.clear();
	}

	void add(std::function<bool()> task, std::function<void()> callback = nullptr)
	{
		std::lock_guard<std::mutex> lk(m_workMutex);

		int index = 0;
		for (size_t i = 1; i < _workers.size(); ++i)
		{
			if (_workers[i]->getSize() < _workers[index]->getSize())
			{
				index = i;
			}
		}
		_workers[index]->add(task, callback);
	}

private:
	std::vector<CThreadTask*> _workers;

private:
	std::mutex m_workMutex;

};

#endif
