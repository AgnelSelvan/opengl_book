
#### 1.2.3 Multi-threading and parallelism in CPP

##### What is multi-threading?
Multi-threading refers to the ability of running multiple threads parallel in a single processor.

#### Key components in C++ Multi-threading
1. `std::thread`
    - `std::thread` allows to create and manage thread in C++.
    - A thread can be created by passing a callable object to the `std::thread` constructor.
    ```cpp
        #include <iostream>
        #include <thread>

        void printMessage(){
            std::cout << "Hello from Multi-thread" << std::endl;
        }

        int main(){
            std::thread t(printMessage);
            t.join();
            return 0;
        }
    ```
2. Thread Life-cycle management
    - `join()`: Block the current thread until the thread execution finishes
    - `detach()`: Detach the thread allowing it to run independently.
3. Mutex (`std::mutex`)
    - Used to prevent race conditions by ensuring only one thread can access a critical section at a time.
    ```cpp
        #include <iostream>
        #include <thread>
        #include <mutex>

        std::mutex mtx;

        void printMessage(int i){
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << "Hello from Multi-thread " << i << std::endl;
        }

        int main(){
            std::thread t1(printMessage, 1);
            std::thread t2(printMessage, 2);
            std::thread t3(printMessage, 3);
            t1.join();
            t2.join();
            t3.join();
            return 0;
        }
    ```
4. Condition Variable (`std::condition_variable`)
    - Used for thread synchronization, allowing thread to wait for specific conditions to be met.
    ```cpp
        #include <iostream>
        #include <queue>
        #include <thread>
        #include <mutex>
        #include <condition_variable>
        #include <chrono>

        // Shared data structures
        std::queue<int> dataQueue;
        std::mutex mtx;
        std::condition_variable cv;
        bool done = false;

        // Producer function
        void producer() {
            for (int i = 1; i <= 10; ++i) {
                std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Simulate work
                {
                    std::lock_guard<std::mutex> lock(mtx);
                    dataQueue.push(i);
                    std::cout << "Produced: " << i << std::endl;
                }
                cv.notify_one(); // Notify the consumer that data is available
            }

            // Notify the consumer that production is done
            {
                std::lock_guard<std::mutex> lock(mtx);
                done = true;
            }
            cv.notify_all();
        }

        // Consumer function
        void consumer() {
            while (true) {
                std::unique_lock<std::mutex> lock(mtx);
                cv.wait(lock, [] { return !dataQueue.empty() || done; }); // Wait for data or end of production

                // Process all available data
                while (!dataQueue.empty()) {
                    int value = dataQueue.front();
                    dataQueue.pop();
                    std::cout << "Consumed: " << value << std::endl;
                }

                // Break if production is done and queue is empty
                if (done && dataQueue.empty()) {
                    break;
                }
            }
        }

        int main() {
            std::thread producerThread(producer);
            std::thread consumerThread(consumer);

            producerThread.join();
            consumerThread.join();

            return 0;
        }
    ```
5. Thread-Safe Data Structures
    - C++ provides tools like `std::atomic` for lock-free thread-safe operations.
    ```cpp

    #include <iostream>
    #include <thread>
    #include <atomic>
    #include <vector>

    // Shared atomic counter
    std::atomic<int> counter(0);

    // Function to increment the counter
    void incrementCounter(int iterations) {
        for (int i = 0; i < iterations; ++i) {
            ++counter; // Atomic increment
        }
    }

    int main() {
        const int numThreads = 4;
        const int incrementsPerThread = 10000;

        // Create a group of threads
        std::vector<std::thread> threads;
        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back(incrementCounter, incrementsPerThread);
        }

        // Join all threads
        for (auto& t : threads) {
            t.join();
        }

        // Print the final value of the counter
        std::cout << "Final counter value: " << counter.load() << std::endl;

        return 0;
    }

    ```

### What is parallelism?
Parallelism refers to executing multiple tasks on multi-core processor. Unlike Multithreading, Parallelism focus on distributing computations to maximize CPU utilization.

#### Key Techniques in C++ Parallelism
1. Thread Pool
    - Instead of creating and destroying a threads repeatedly, a thread pool manages a fixed number of reusable threads.
    ```cpp
    #include <iostream>
    #include <vector>
    #include <thread>
    #include <queue>
    #include <mutex>
    #include <condition_variable>
    #include <functional>

    class ThreadPool {
    public:
        ThreadPool(size_t numThreads) {
            for (size_t i = 0; i < numThreads; ++i) {
                threads.emplace_back([this] {
                    while (true) {
                        std::function<void()> task;
                        {
                            std::unique_lock<std::mutex> lock(mtx);
                            cv.wait(lock, [this] { return !tasks.empty() || terminate; });
                            if (terminate && tasks.empty()) return;
                            task = std::move(tasks.front());
                            tasks.pop();
                        }
                        task();
                    }
                });
            }
        }

        void enqueue(std::function<void()> task) {
            {
                std::unique_lock<std::mutex> lock(mtx);
                tasks.push(std::move(task));
            }
            cv.notify_one();
        }

        ~ThreadPool() {
            {
                std::unique_lock<std::mutex> lock(mtx);
                terminate = true;
            }
            cv.notify_all();
            for (std::thread& thread : threads) {
                if (thread.joinable()) thread.join();
            }
        }

    private:
        std::vector<std::thread> threads;
        std::queue<std::function<void()>> tasks;
        std::mutex mtx;
        std::condition_variable cv;
        bool terminate = false;
    };

    int main() {
        ThreadPool pool(4);

        for (int i = 0; i < 10; ++i) {
            pool.enqueue([i] {
                std::cout << "Task " << i << " is running on thread "
                        << std::this_thread::get_id() << '\n';
            });
        }

        return 0;
    }
    ```
2. Standard Template Library (STL) Algorithms with Parallelism
    - Since C++17, many STL algorithms support parallel execution using the <execution> header.
    ```cpp
    #include <iostream>
    #include <vector>
    #include <execution>
    #include <numeric>

    int main() {
        std::vector<int> vec(100, 1);
        int sum = std::reduce(std::execution::par, vec.begin(), vec.end());
        std::cout << "Sum: " << sum << std::endl;
        return 0;
    }
    ```
3. Task-based Parallelism with `std::async`
    - `std::async` runs tasks asynchronously and returns a future.
    ```cpp
    #include <iostream>
    #include <future>

    int calculate(int x) {
        return x * x;
    }

    int main() {
        std::future<int> result = std::async(std::launch::async, calculate, 10);
        std::cout << "Result: " << result.get() << std::endl;
        return 0;
    }

    ```

