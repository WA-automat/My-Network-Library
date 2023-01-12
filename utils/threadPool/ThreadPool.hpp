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

// һ�������ظ�ʹ�õ��̳߳�
// ������һ�鹤���̶߳�������Ҫ����һ���߳�
class ThreadPool {

	// ���г�Ա
public:

	// ���캯��
	ThreadPool() :state(1) {}


	// ����һ�鹤���߳�
	// ��״̬��Ϊ����״̬
	// ����Ϊ�����̵߳�����
	void createThreadsWorker(size_t threads, int times);


	// ��������װ���߳����
	template<typename F, class ...Args>
	auto execute(F&& f, Args&& ... args)
		->future<typename std::result_of<F(Args...)>::type>;


	// ���ٵ�ǰ�����鹤���߳�
	// ��Ҫ״̬Ϊ����״̬
	// ��Ҫ�ȴ���ǰ���й����߳̽���
	void finishThreadsWorker(int times);

	// �����������
	void clearQueue(void);


	// ��������
	// ����ǰӦ������ֹͣ����ʹ�ù����߳�ȫ��ִ�����
	~ThreadPool();


	// ˽�г�Ա
private:

	// ��������е�Ԫ������
	typedef function<void()> taskType;

	// �ж��̳߳�״̬
	// 0��ʾ�̳߳��Ѿ�����
	// 1��ʾ�̳߳ش��ڣ���δ���й���
	// 2��ʾ�̳߳ش��ڣ��������ڹ���
	atomic_int state;

	// �����̳߳�
	vector<thread> workers;

	// �߳���������
	BlockQueue<taskType> tasks;

};

// ����һ�鹤���߳�
// ��״̬��Ϊ����״̬
// ����Ϊ�����߳����빤���߳���ౣ�ֶ��
// �����߳�����Ҳ�Ჶ��threads���߳�
void ThreadPool::createThreadsWorker(size_t threads = 8, int times = 1000) {

	// try...catch����
	try {

		// �ж��Ƿ�Ϊ�����̳߳�
		if (!this->state) {
			cout << "[error]:The pool has been deleted!" << endl;
			throw runtime_error("The pool has been deleted!");
		}
		else if (this->state == 2) {
			cout << "[error]:The pool is working!" << endl;
			throw runtime_error("The pool is working!");
		}

		// ����״̬
		state = 2;

		// һ����threads�������߳�
		for (int i = 0; i < threads; ++i) {

			// ÿһ�ζ����߳�����������ȡ��һ���߳�
			// ������Ϊ��ʱ���������л������ý��̣�ֱ�����б����
			// �����в�Ϊ��ʱ����������������ȡ������emplace_back
			workers.emplace_back(
				[this, times](void) {

					auto start_time = clock();
					auto now_time = clock();

					// ��ѭ��
					// TODO ��Ӧ������ѭ������ʹ�ú�Ӧ����ͣ��
					for (; now_time - start_time <= times; now_time = clock()) {

						// ���Ѿ��˳�����
						if (state != 2) break;

						// ��������������ȡ����ִ��
						auto task = tasks.pull();
						task();

						// ��ʱ��ǰ���������Ѿ����ˣ����˳���ǰ�����߳�
						if (now_time - start_time > times || tasks.empty() || state != 2) break;

					}
				});

		}

#ifdef DEBUG
		cout << "[Debug]:Create success!" << endl;
#endif // DEBUG



	}
	// ���������쳣
	// ����״̬�������׳��쳣
	catch (...) {

		// ״̬�޸�Ϊ����
		state = 0;
		throw;

	}

}


// ��ɲ����ٵ�ǰ�����鹤���߳�
// ��Ҫ״̬Ϊ����״̬
// ��Ҫ�ȴ���ǰ���й����߳̽���
// ��������������������еĹ����̶߳�����ʹ��
// ��Ϊÿ�������߳�����Ҫ������������ȡ��һ���߳�
inline void ThreadPool::finishThreadsWorker(int times = 1000) {

	// try...catch����
	try {

#ifdef DEBUG
		cout << "[Debug]:Begin finish!" << endl;
#endif // DEBUG

		// �ȴ�ʱ�䵽��
		Sleep(times);

		// �ж��Ƿ�Ϊ�����̳߳�
		if (!this->state) {
			cout << "[error]:The pool has been deleted!" << endl;
			throw runtime_error("The pool has been deleted!");
		}

		// �ж��Ƿ��ڹ���
		else if (this->state == 1) {
			cout << "[warn]:The pool is not working!" << endl;
		}

		// ��������
		else {

			// �ȴ����й����߳����
			for (auto& worker : workers) {

				// �����̲߳�Ӧ������
				worker.detach();

#ifdef DEBUG
				cout << "[Debug]:Work finish!" << endl;;
#endif // DEBUG


			}


			// ��չ����߳�
			workers.clear();
			
			// �޸�״̬
			state = 1;
		}

	}
	// ���������쳣
	// ����״̬�������׳��쳣
	catch (...) {

		// ״̬�޸�Ϊ����
		state = 0;
		throw;

	}

}

// �����������
inline void ThreadPool::clearQueue(void) {

	// try...catch����
	try {

		// �����������
		this->tasks.clear();

	}
	// ���������쳣
	// ����״̬�������׳��쳣
	catch (...) {

		// ״̬�޸�Ϊ����
		state = 0;
		throw;

	}	
}

// ��������
inline ThreadPool::~ThreadPool() {

	// ����ֹͣ����
	if (state == 2) {
		// ��Ա����
		this->finishThreadsWorker();
	}

	// ����״̬
	state = 0;
}

// ��������װ���߳����
template<typename F, class ...Args>
inline auto ThreadPool::execute(F&& f, Args && ...args) -> future<typename std::result_of<F(Args ...)>::type> {

	// try...catch����
	try {
		
		// �趨����ֵ�����ͣ���result_type����
		using result_type = typename std::result_of<F(Args...)>::type;

		// �ж��Ƿ�Ϊ�����̳߳�
		if (!this->state) {
			cout << "[error]:The pool has been deleted!" << endl;
			throw runtime_error("The pool has been deleted!");
		}

		// ������ĺ�����Ϊ���̱߳�ָ��package_task������ָ��洢
		auto task = std::make_shared<std::packaged_task<result_type()>>(
			std::bind(std::forward<F>(f), std::forward<Args>(args)...)
			);

		// ��res�洢�̵߳�future
		std::future<result_type> result = task->get_future();

		// �������м������߳�
		tasks.emplace([task]() {(*task)(); });

		// ����future
		return result;

	}
	// ���������쳣
	// ����״̬�������׳��쳣
	catch (...) {

		// ״̬�޸�Ϊ����
		state = 0;
		throw;

	}

}

