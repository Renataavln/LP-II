#include <iostream>
#include <thread>
#include <unistd.h>

#define LOOP_LIMIT 1000

int foo(int loop_limit, std::string msg){
    for(int i = 0; i < loop_limit; i++){
        std::cout << msg << std::endl;
    }

    std::cout << loop_limit << std::endl;

    return 0;
}

int main(){
    std::thread t(foo, 500, "aaaaaaa");

    t.join();

    return 0;
}