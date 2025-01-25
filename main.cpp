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
