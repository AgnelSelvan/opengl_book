#include <iostream>

int main(){
    int* a = new int;
    *a = 5;
    delete a;
    return 0;
}