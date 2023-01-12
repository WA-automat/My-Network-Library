//// µ÷ÊÔ×´Ì¬
//#define DEBUG

#include<iostream>
#include<Windows.h>
#include"../utils/threadPool/ThreadPool.hpp"
using namespace std;
int main(void) {

	ThreadPool pool;

	pool.createThreadsWorker(18);

	auto task1 = pool.execute([]() {cout << 1 << endl; });
	auto task2 = pool.execute([]() {cout << 2 << endl; });
	auto task3 = pool.execute([]() {cout << 3 << endl; });

	pool.finishThreadsWorker();

	auto task4 = pool.execute([]() {cout << 4 << endl; });
	auto task5 = pool.execute([]() {cout << 5 << endl; });
	auto task6 = pool.execute([]() {cout << 6 << endl; });

	auto task7 = pool.execute([]() {cout << 7 << endl; });
	auto task8 = pool.execute([]() {cout << 8 << endl; });
	auto task9 = pool.execute([]() {cout << 9 << endl; });
	auto task10 = pool.execute([]() {cout << 10 << endl; });
	auto task11 = pool.execute([]() {cout << 11 << endl; });
	auto task12 = pool.execute([]() {cout << 12 << endl; });
	auto task13 = pool.execute([]() {cout << 13 << endl; });
	auto task14 = pool.execute([]() {cout << 14 << endl; });
	auto task15 = pool.execute([]() {cout << 15 << endl; });
	auto task16 = pool.execute([]() {cout << 16 << endl; });
	auto task17 = pool.execute([]() {cout << 17 << endl; });
	auto task18 = pool.execute([]() {cout << 18 << endl; });

	system("pause");
	return 0;
}