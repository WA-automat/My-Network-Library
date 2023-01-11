// µ÷ÊÔ×´Ì¬
#define DEBUG

#include<iostream>
#include<future>
#include<Windows.h>
#include"../utils/container.h"

using namespace std;
int main(void) {

	BlockQueue<int> q;

	cout << sizeof BlockQueue<int>() << endl;

	auto task_1 = async([&]() {
		int data = q.pull();
		cout << data << endl;
		});
	auto task_2 = async([&]() {
		Sleep(1000);
		q.push(1);
		});

	task_1.get();
	task_2.get();

	system("pause");
	return 0;
}