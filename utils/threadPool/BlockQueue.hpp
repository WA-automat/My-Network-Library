#pragma once

#include<list>
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
	BlockQueue() = default;
	~BlockQueue() = default;

	// ���������ж�β����һ��Ԫ��
	void push(const T& x) {

		// ����
		unique_lock<mutex> ul(mx);
		_q.push_back(x);
		cv.notify_all();

	}

	// ���������ж�β����һ��Ԫ��
	void push(T&& x) {

		// ����
		unique_lock<mutex> ul(mx);
		_q.push_back(std::move(x));
		cv.notify_all();
	
	}

	// ���������ж�ͷɾ��һ��Ԫ��
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

		return front;
	}

	// ����������еĳ���
	int size(void) {

		// ����
		unique_lock<mutex> ul(mx);
		return _q.size();

	}

	// ˽�г�Ա
private:

	// ����ʹ��˫����ʵ��
	list<T> _q;
	
	// ������������
	mutex mx;
	condition_variable cv;
};