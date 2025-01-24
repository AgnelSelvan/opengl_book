
#### 1.2.2 Memory Management in CPP

##### Types of Memory Allocation

###### Stack Allocation
- Stack Memory is used for storing local variables and function call data.
- Memory allocated on the stack is automatically managed and released when it goes out of the scope.
- Stack allocation is fast but Stack memory is limited. Large objects or arrays should not be allocated on the stack, as they may cause a stack overflow.
```cpp
void function() {
    int x = 5; // Stack allocation for x
} // x is automatically de-allocated here
```

###### Heap Allocation
- Heap memory (or dynamic memory) is allocated manually using new and delete.
- Unlike Stack Memory, Heap memory is managed by the developers allowing for more flexibility for managing memory.
- It is essential to de-allocate memory on the heap to prevent memory leaks.
```cpp
int main(){
    int* a = new int;
    *a = 5;
    delete a;
    return 0;
}
```

##### Dynamic Memory Management
- `new`
    - allocates memory on heap.
    - returns a pointer to the allocated memory.
    - Throws an exception `(std::bad_alloc)` if allocation fails.
    ```cpp
    int* ptr = new int(10); // Allocates and initializes memory
    ```
- `delete`
    - Frees memory allocated with `new`
    - Must be called once for every `new` to avoid memory leaks.
    - Using `delete` on a null pointer is safe
    ```cpp
    delete ptr; // De-allocates memory
    ```
    ```cpp
    Heap:
    | Address |  Value |
    | 0x1000  |   10   |   // ptr points to this memory location

    Stack:
    | ptr     | 0x1000 |   // ptr stores the address 0x1000 pointing to heap

    ```
- `new[]` and `delete[]`
    - Use `new[]` to allocate arrays on the heap.
    - Use `delete[]` to de-allocate arrays.
    ```cpp
    int* arr = new int[5];
    delete[] arr;
    ```

##### Smart Pointers
Smart Pointer helps automate memory management and reduces risk of leaks. Smart pointer releases memory when they automatically go out of the scope.
- `std::unique_ptr`
    - Manages a single object
    - Ensure sole ownership, so that it cannot be copied but can be moved.
    - Automatically deletes the managed object when it goes out of the scope.
    ```cpp
    std::unique_ptr<int> ptr = std::make_unique<int>(10);
    ```
- `std::shared_ptr`
    - Allows shared ownership of a pointer.
    - Maintains a reference count, so the memory is freed only when all owners are done with it.
    ```cpp
    std::shared_ptr<int> ptr1 = std::make_shared<int>(10);
    std::shared_ptr<int> ptr2 = ptr1; // ptr1 and ptr2 share ownership
    ```
- `std::weak_ptr`
    - A weak reference to an object managed by `std::shared_ptr`.
    - Does not affect reference count, so it doesn't prevent the object from being deleted.
    ```cpp
    std::weak_ptr<int> weakPtr = ptr1; // weakPtr doesn't affect reference count
    ```





