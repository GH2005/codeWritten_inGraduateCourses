#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <queue>
#include <chrono>
#include <Windows.h>
#include <Psapi.h>
#include "../common.h"

int main() {

	std::queue<int> q;

	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	for (int i = 0; i < THREAD_COUNT; i++) {
		for (int j = 0; j < UNIT_COUNT; j++) {
			q.push(j);
			for (int k = 0; k < SPIN_COUNT; k++);
			int v = q.front();  q.pop();
			for (int k = 0; k < SPIN_COUNT; k++);
		}
	}
	std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
	std::chrono::duration<int, std::milli> duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	HANDLE handle = GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));
	printf("%d KB, %d ms\n", pmc.PeakWorkingSetSize / 1024, duration.count());

	system("pause");
	return 0;
}