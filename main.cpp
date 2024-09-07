#include <iostream>
#include <stdio.h>

void getFileName(const char*& fileName){
    std::cout << fileName << std::endl;
    std::cout << &fileName << std::endl;
}
void getFileNameStr(std::string& fileName){
    std::cout << fileName << std::endl;
    std::cout << &fileName << std::endl;
}

int main(){
    std::cout << "Hello World" << std::endl;
    const char* fileName = "some.txt";
    std::string fileName1 = "some_str.txt";
    getFileName(fileName);
    std::cout << " " << std::endl;
    std::cout << &fileName << std::endl;
    std::cout << " " << std::endl;
    getFileNameStr(fileName1);
    std::cout << " " << std::endl;
    std::cout << &fileName1 << std::endl;
    std::cout << " " << std::endl;

    char c = 'D';
    std::cout << c << std::endl;
    std::cout << &c << std::endl;
    char* p = &c;
    *p = 'C';
    std::cout << p << std::endl;
    std::cout << c << std::endl;
    return 0;
}