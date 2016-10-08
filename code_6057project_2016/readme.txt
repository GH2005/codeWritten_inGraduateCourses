This "Visual Studio Solution" folder contains four "Project"s for experimenting on Lock-free (Non-blocking) queues (data structure). Lock-free data structures are concurrent ones that are not implemented using thread locks (e.g., mutex).

I conducted a comparative study on four queue algorithms, three of which were concurrent ones:
concurrenct_queue_lockfree: a concurrent lock-free queue class in Parallel Patterns Library (PPL) provided by Visual C++ since version 2010;
cpp11_michael_scott: my implementation of Michael and Scott’s lock-free queue  using C++ 11;  
lock_based_queue: a lock-based concurrent queue by Palacios . It uses a mutex to guard an instance of std::queue; 
std_queue: a single-threaded std::queue.

When implementing Michael and Scott's lock-free queue, I introduced std::shared_ptr for memory reclamation.

Michael, M. M., & Scott, M. L. (1996). Simple, fast, and practical non-blocking and blocking concurrent queue algorithms. Proceedings of the Fifteenth Annual ACM Symposium on Principles of Distributed Computing - PODC '96. doi:10.1145/248052.248106.
Palacios, J. (2013). Concurrent queue – C++ 11. Retrieved from https://juanchopanzacpp.wordpress.com/2013/02/26/concurrent-queue-c11/.
