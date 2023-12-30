#Created Simple optimization of a high-frequency trading (HFT) system using C++, achieving a significant reduction in latency and enhancing execution efficiency.
##Stock Data collected from yahoo Finanace in formof csv format. Due to complexity of (HFT) system in this project only m


1. Profiling and Analysis:

* Used Profiling Tools: Employ profiling tools like Valgrind and gprof to pinpoint functions or code blocks consuming significant time or memory.

2. Memory Optimization:
* Optimize Data Structures: Restructure data structures for cache efficiency and reduced memory access times.
* Memory Pools: Consider memory pools for frequent allocations/deallocations of similar-sized objects.
3. Algorithm Optimization:
* Parallelization: Utilize multi-core processors by parallelizing tasks using std::execution::par or OpenMP.
* Vectorization: Employ SIMD instructions AVX to accelerate computations on multiple data elements simultaneously.
4. Network Optimization:

* Asynchronous I/O: Use libuv or Boost.Asio for non-blocking network operations, preventing threads from idling while waiting for network responses.

5. Compiler Optimization:

* Enable Optimization Flags: Compile code with optimization flags (-O2 or -O3) to enable compiler-level optimizations.

6. Hardware Considerations:

* CPU Architecture: Choose hardware with a low-latency network interface and a CPU architecture suited for HFT workloads (e.g., high clock speed, large caches).
