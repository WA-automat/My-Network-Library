#pragma once

#include<list>
#include<mutex>
#include<condition_variable>

using namespace std;

// 阻塞队列：线程安全的队列
// 在多线程编程时当有多个线程访问一个队列时如果队列为空
// 则获取队列中元素的方法就会阻塞，直到队列中有元素可以获取
template<typename T>
class BlockQueue {

	// 公有成员
public:

	// 构造函数与析构函数使用默认值
	BlockQueue() = default;
	~BlockQueue() = default;

	// 在阻塞队列队尾插入一个元素
	void push(const T& x) {

		// 加锁
		unique_lock<mutex> ul(mx);
		_q.push_back(x);
		cv.notify_all();

	}

	// 在阻塞队列队尾插入一个元素
	void push(T&& x) {

		// 加锁
		unique_lock<mutex> ul(mx);
		_q.push_back(std::move(x));
		cv.notify_all();
	
	}

	// 从阻塞队列队头删除一个元素
	// 在多线程编程时当有多个线程访问一个队列时如果队列为空
	// 则获取队列中元素的方法就会阻塞，直到队列中有元素可以获取
	T pull(void) {

		// 加锁
		unique_lock<mutex> ul(mx);

		while (_q.size() == 0) {
			cv.wait(ul);
		}

		T front = _q.front();

		// 删除队头元素
		_q.pop_front();

		return front;
	}

	// 获得阻塞队列的长度
	int size(void) {

		// 加锁
		unique_lock<mutex> ul(mx);
		return _q.size();

	}

	// 私有成员
private:

	// 队列使用双链表实现
	list<T> _q;
	
	// 锁与条件变量
	mutex mx;
	condition_variable cv;
};