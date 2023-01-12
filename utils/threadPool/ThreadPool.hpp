#pragma once

#include<vector>
#include<functional>
#include<ctime>
#include<thread>
#include<atomic>
#include<future>
#include<mutex>
#include<condition_variable>
#include"../container/BlockQueue.hpp"

using namespace std;

// 一个可以重复使用的线程池
// 创建的一组工作线程都至少需要捕获一个线程
class ThreadPool {

	// 公有成员
public:

	// 构造函数
	ThreadPool() :state(1) {}


	// 创建一组工作线程
	// 将状态改为工作状态
	// 参数为工作线程的数量
	void createThreadsWorker(size_t threads, int times);


	// 将函数包装成线程入队
	template<typename F, class ...Args>
	auto execute(F&& f, Args&& ... args)
		->future<typename std::result_of<F(Args...)>::type>;


	// 销毁当前的这组工作线程
	// 需要状态为工作状态
	// 需要等待当前所有工作线程结束
	void finishThreadsWorker(int times);

	// 清空阻塞队列
	void clearQueue(void);


	// 析构函数
	// 析构前应当调用停止函数使得工作线程全部执行完毕
	~ThreadPool();


	// 私有成员
private:

	// 任务队列中的元素类型
	typedef function<void()> taskType;

	// 判断线程池状态
	// 0表示线程池已经销毁
	// 1表示线程池存在，但未进行工作
	// 2表示线程池存在，并且正在工作
	atomic_int state;

	// 工作线程池
	vector<thread> workers;

	// 线程阻塞队列
	BlockQueue<taskType> tasks;

};

// 创建一组工作线程
// 将状态改为工作状态
// 参数为工作线程数与工作线程最多保持多久
// 工作线程至少也会捕获threads个线程
void ThreadPool::createThreadsWorker(size_t threads = 8, int times = 1000) {

	// try...catch语句块
	try {

		// 判断是否为可用线程池
		if (!this->state) {
			cout << "[error]:The pool has been deleted!" << endl;
			throw runtime_error("The pool has been deleted!");
		}
		else if (this->state == 2) {
			cout << "[error]:The pool is working!" << endl;
			throw runtime_error("The pool is working!");
		}

		// 更改状态
		state = 2;

		// 一共有threads个工作线程
		for (int i = 0; i < threads; ++i) {

			// 每一次都从线程阻塞队列中取出一个线程
			// 当队列为空时，阻塞队列会阻塞该进程，直至队列被填充
			// 当队列不为空时，从阻塞队列中拉取出来并emplace_back
			workers.emplace_back(
				[this, times](void) {

					auto start_time = clock();
					auto now_time = clock();

					// 死循环
					// TODO 不应该是死循环，在使用后应当暂停它
					for (; now_time - start_time <= times; now_time = clock()) {

						// 当已经退出工作
						if (state != 2) break;

						// 从阻塞队列中拉取任务并执行
						auto task = tasks.pull();
						task();

						// 超时或当前阻塞队列已经空了，则退出当前工作线程
						if (now_time - start_time > times || tasks.empty() || state != 2) break;

					}
				});

		}

#ifdef DEBUG
		cout << "[Debug]:Create success!" << endl;
#endif // DEBUG



	}
	// 捕获所有异常
	// 更改状态后重新抛出异常
	catch (...) {

		// 状态修改为销毁
		state = 0;
		throw;

	}

}


// 完成并销毁当前的这组工作线程
// 需要状态为工作状态
// 需要等待当前所有工作线程结束
// 调用这个函数不代表所有的工作线程都结束使用
// 因为每个工作线程至少要从阻塞队列中取出一次线程
inline void ThreadPool::finishThreadsWorker(int times = 1000) {

	// try...catch语句块
	try {

#ifdef DEBUG
		cout << "[Debug]:Begin finish!" << endl;
#endif // DEBUG

		// 等待时间到达
		Sleep(times);

		// 判断是否为可用线程池
		if (!this->state) {
			cout << "[error]:The pool has been deleted!" << endl;
			throw runtime_error("The pool has been deleted!");
		}

		// 判断是否在工作
		else if (this->state == 1) {
			cout << "[warn]:The pool is not working!" << endl;
		}

		// 条件满足
		else {

			// 等待所有工作线程完成
			for (auto& worker : workers) {

				// 工作线程不应当阻塞
				worker.detach();

#ifdef DEBUG
				cout << "[Debug]:Work finish!" << endl;;
#endif // DEBUG


			}


			// 清空工作线程
			workers.clear();
			
			// 修改状态
			state = 1;
		}

	}
	// 捕获所有异常
	// 更改状态后重新抛出异常
	catch (...) {

		// 状态修改为销毁
		state = 0;
		throw;

	}

}

// 清空阻塞队列
inline void ThreadPool::clearQueue(void) {

	// try...catch语句块
	try {

		// 清空阻塞队列
		this->tasks.clear();

	}
	// 捕获所有异常
	// 更改状态后重新抛出异常
	catch (...) {

		// 状态修改为销毁
		state = 0;
		throw;

	}	
}

// 析构函数
inline ThreadPool::~ThreadPool() {

	// 调用停止函数
	if (state == 2) {
		// 成员函数
		this->finishThreadsWorker();
	}

	// 更改状态
	state = 0;
}

// 将函数包装成线程入队
template<typename F, class ...Args>
inline auto ThreadPool::execute(F&& f, Args && ...args) -> future<typename std::result_of<F(Args ...)>::type> {

	// try...catch语句块
	try {
		
		// 设定返回值的类型，用result_type代替
		using result_type = typename std::result_of<F(Args...)>::type;

		// 判断是否为可用线程池
		if (!this->state) {
			cout << "[error]:The pool has been deleted!" << endl;
			throw runtime_error("The pool has been deleted!");
		}

		// 将传入的函数作为新线程被指向package_task的智能指针存储
		auto task = std::make_shared<std::packaged_task<result_type()>>(
			std::bind(std::forward<F>(f), std::forward<Args>(args)...)
			);

		// 用res存储线程的future
		std::future<result_type> result = task->get_future();

		// 阻塞队列加入新线程
		tasks.emplace([task]() {(*task)(); });

		// 返回future
		return result;

	}
	// 捕获所有异常
	// 更改状态后重新抛出异常
	catch (...) {

		// 状态修改为销毁
		state = 0;
		throw;

	}

}

