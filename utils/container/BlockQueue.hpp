#pragma once

#include<list>
#include<ctime>
#include<mutex>
#include<condition_variable>

using namespace std;

// �������У��̰߳�ȫ�Ķ���
// �ڶ��̱߳��ʱ���ж���̷߳���һ������ʱ�������Ϊ��
// ���ȡ������Ԫ�صķ����ͻ�������ֱ����������Ԫ�ؿ��Ի�ȡ
template<typename T>
class BlockQueue {

	// ���г�Ա
public:

	// ���캯������������ʹ��Ĭ��ֵ
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

	// ���������ж�β����һ��Ԫ��
	void emplace(const T& x) {

		// ����
		unique_lock<mutex> ul(mx);
		_q.emplace_back(x);
		cv.notify_all();

#ifdef DEBUG
		cout << "[Debug]:Push success!" << endl;
#endif // DEBUG

	}

	// ���������ж�β����һ��Ԫ��
	void emplace(T&& x) {

		// ����
		unique_lock<mutex> ul(mx);
		_q.emplace_back(std::move(x));
		cv.notify_all();

#ifdef DEBUG
		cout << "[Debug]:Push success!" << endl;
#endif // DEBUG

	}

	// ���������ж�ͷɾ��һ��Ԫ�أ���ȡ��
	// �ڶ��̱߳��ʱ���ж���̷߳���һ������ʱ�������Ϊ��
	// ���ȡ������Ԫ�صķ����ͻ�������ֱ����������Ԫ�ؿ��Ի�ȡ
	T pull(void) {

		// ����
		unique_lock<mutex> ul(mx);

		while (_q.size() == 0) {
			cv.wait(ul);
		}

		T front = _q.front();

		// ɾ����ͷԪ��
		_q.pop_front();

#ifdef DEBUG
		cout << "[Debug]:Pull success!" << endl;
#endif // DEBUG

		return front;
	}

	// ����������еĳ���
	int size(void) {

		// ����
		unique_lock<mutex> ul(mx);

#ifdef DEBUG
		cout << "[Debug]:Get size success!" << endl;
#endif // DEBUG

		return _q.size();

	}

	// �����������
	void clear(void) {
		
		// ����
		unique_lock<mutex> ul(mx);

		// ��ն���
		_q.clear();

	}

	bool empty(void) {

		// ����
		unique_lock<mutex> ul(mx);

		// ���ض����Ƿ�Ϊ��
		return _q.empty();

	}

	// ˽�г�Ա
private:

	// ����ʹ��˫����ʵ��
	list<T> _q;
	
	// ������������
	mutex mx;
	condition_variable cv;
};