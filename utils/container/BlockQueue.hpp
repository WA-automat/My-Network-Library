#pragma once

#include<list>
#include<ctime>
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
	BlockQueue() {

#ifdef DEBUG
		cout << "[Debug]:Create BlockQueue!" << endl;
#endif // DEBUG

	}

	~BlockQueue() {

#ifdef DEBUG
		cout << "[Debug]:Destory BlockQueue!" << endl;
#endif // DEBUG

	}

	// 在阻塞队列队尾插入一个元素
	void emplace(const T& x) {

		// 加锁
		unique_lock<mutex> ul(mx);
		_q.emplace_back(x);
		cv.notify_all();

#ifdef DEBUG
		cout << "[Debug]:Push success!" << endl;
#endif // DEBUG

	}

	// 在阻塞队列队尾插入一个元素
	void emplace(T&& x) {

		// 加锁
		unique_lock<mutex> ul(mx);
		_q.emplace_back(std::move(x));
		cv.notify_all();

#ifdef DEBUG
		cout << "[Debug]:Push success!" << endl;
#endif // DEBUG

	}

	// 从阻塞队列队头删除一个元素（拉取）
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

#ifdef DEBUG
		cout << "[Debug]:Pull success!" << endl;
#endif // DEBUG

		return front;
	}

	// 获得阻塞队列的长度
	int size(void) {

		// 加锁
		unique_lock<mutex> ul(mx);

#ifdef DEBUG
		cout << "[Debug]:Get size success!" << endl;
#endif // DEBUG

		return _q.size();

	}

	// 清空阻塞队列
	void clear(void) {
		
		// 加锁
		unique_lock<mutex> ul(mx);

		// 清空队列
		_q.clear();

	}

	bool empty(void) {

		// 加锁
		unique_lock<mutex> ul(mx);

		// 返回队列是否为空
		return _q.empty();

	}

	// 私有成员
private:

	// 队列使用双链表实现
	list<T> _q;
	
	// 锁与条件变量
	mutex mx;
	condition_variable cv;
};