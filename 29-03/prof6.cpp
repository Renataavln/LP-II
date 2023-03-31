#include <iostream>
#include <thread>
#include <string>
#include <mutex>

#define QTD_THREADS 5

std::mutex m;

int print_msg(std::string msg, int qtd){
    for(int i = 0; i < qtd; i++){
        std::cout << "[" << std::this_thread::get_id << "]" << msg << std::endl;
        m.unlock();
    }

    return 0;
}

int main(){
    std::thread *t[QTD_THREADS];

    for(int i = 0; i < QTD_THREADS; i++){
        t[i] = new std::thread(print_msg, "oi brasil", i);
    }

    for(int i = 0; i < QTD_THREADS; i++){
        t[i]->join();
    }

    return 0;
}