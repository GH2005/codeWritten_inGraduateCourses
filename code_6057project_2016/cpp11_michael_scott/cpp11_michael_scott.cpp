#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <memory>
#include <thread>
#include <chrono>
#include <Windows.h>
#include <Psapi.h>
#include "../common.h"

template<class Type> class LockfreeQueue {
public:
	struct Node {
		Type VALUE;
		std::shared_ptr<Node> NEXT;
		Node() : VALUE(), NEXT(nullptr) {}
	};
private:
	std::shared_ptr<Node> HEAD;
	std::shared_ptr<Node> TAIL;
public:
	LockfreeQueue() { // HEAD & TAIL point to the first node (dummy)
		this->HEAD = std::make_shared<Node>();
		this->TAIL = this->HEAD;
	}
	~LockfreeQueue() {
		// no need to do anything; shared_ptrs handle it automatically;
		// the destruction of any shared_ptr will trigger the destruction 
		// of its targeted Node who also has a shared_ptr as a member
	}
public:
	void Enqueue(Type &value) {
		// new Node
		std::shared_ptr<Node> node = std::make_shared<Node>();
		node->VALUE = value;

		// CAS loop: take snapshots -> examine snapshots -> publish
		std::shared_ptr<Node> tail, next;
		while (1) {
			// atomically take snapshots
			tail = std::atomic_load(&this->TAIL);
			next = std::atomic_load(&tail->NEXT);

			if (tail == std::atomic_load(&this->TAIL)) {	// check consistency
				// examine snapshots
				if (!next) {	// next.get() == nullptr 
					// if tail->NEXT is remaining the same, the following CAS will succeed,
					// linking the new node to the end
					if (std::atomic_compare_exchange_weak(&tail->NEXT, &next, node))
						break;
				}
				else {	// next.get() != nullptr
					// try to swing this->TAIL one step to the end
					std::atomic_compare_exchange_weak(&this->TAIL, &tail, next);
				}
			}
		}

		// new node has been linked; just try once to swing this->TAIL to the end
		std::atomic_compare_exchange_weak(&this->TAIL, &tail, node);
	}

	bool Dequeue(Type &value) {
		while (1) {
			// atomically take snapshots
			std::shared_ptr<Node> head = std::atomic_load(&this->HEAD);
			std::shared_ptr<Node> tail = std::atomic_load(&this->TAIL);
			std::shared_ptr<Node> next = std::atomic_load(&head->NEXT);

			if (head == std::atomic_load(&this->HEAD)) {	// check consistency
				// examine snapshots
				if (head == tail) {	// including two small conditions:
					// 1. the queue has just one dummy Node
					if (!next) return false;
					// 2. the queue has two Nodes, but this->TAIL is pointing to the
					//     first one (dummy), so swing this->TAIL to the end
					std::atomic_compare_exchange_weak(&this->TAIL, &tail, next);
				}
				else {	// can ignore this->TAIL and situations related to it
					value = next->VALUE;	// give the value to the consumer
					// atomically change this->HEAD
					if (std::atomic_compare_exchange_weak(&this->HEAD, &head, next))
						break;
				}
			}
		}
		return true;
	}
};

void ThreadWork(int idx, LockfreeQueue<int> *q) {
	for (int j = 0; j < UNIT_COUNT; j++) {
		q->Enqueue(j);
		for (int k = 0; k < SPIN_COUNT; k++);
		int v;  q->Dequeue(v);
		for (int k = 0; k < SPIN_COUNT; k++);
	}
}

int main() {
	LockfreeQueue<int> queue;

	std::thread t[THREAD_COUNT];
	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	for (int i = 0; i < THREAD_COUNT; i++) {
		t[i] = std::thread(ThreadWork, i, &queue);
	}
	for (int i = 0; i < THREAD_COUNT; i++) {
		t[i].join();
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