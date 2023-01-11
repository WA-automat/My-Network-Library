// µ÷ÊÔ×´Ì¬
#define DEBUG

#include<iostream>
#include"../utils/threadPool/ThreadPool.hpp"
using namespace std;
int main(void) {

	ThreadPool pool;

	pool.createThreadsWorker();

	auto task1 = pool.execute([]() {cout << 1 << endl; });
	auto task2 = pool.execute([]() {cout << 2 << endl; });
	auto task3 = pool.execute([]() {cout << 3 << endl; });

	pool.finishThreadsWorker();

	pool.createThreadsWorker();

	auto task4 = pool.execute([]() {cout << 4 << endl; });
	auto task5 = pool.execute([]() {cout << 5 << endl; });
	auto task6 = pool.execute([]() {cout << 6 << endl; });

	pool.finishThreadsWorker();

	system("pause");
	return 0;
}