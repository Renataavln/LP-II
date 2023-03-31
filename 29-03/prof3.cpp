#include <iostream>
#include <thread>

int foo(){
    printf("oi\n");

    return 0;
}

int main(){
    std::thread t(foo);

    t.join();

    return 0;
}