/*
Implemente um monitor que controle o acesso a um buffer compartilhado. Crie uma thread produtora e
outra thread consumidora que utilizem o monitor
    buffer -> 8 chars
    monitor -> inserir, recuperar
*/

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 8
#define QTD_PROD 10

using std::thread;
using std::mutex;
using std::cout;
using std::endl;
using std::condition_variable;

class monitor_buffer{
    int buffer[BUFFER_SIZE];
    int count;
    int in;
    int out;
    mutex mtx;
    condition_variable not_full;
    condition_variable not_empty;

    public:
        monitor_buffer(){
            count = 0;
            in = 0;
            out = 0;
        }

        void insert(int item){
            std::unique_lock<std::mutex> lock(mtx);

            while(count == BUFFER_SIZE){
                not_full.wait(lock);
            }
            buffer[in] = item;
            in = (in + 1) % BUFFER_SIZE;
            count++;
            not_empty.notify_one();
        }
        
        int retrieve(){
            std::unique_lock<std::mutex> lock(mtx);

            while(count == 0){
                not_empty.wait(lock);
            }

            int item = buffer[out];
            out = (out + 1) % BUFFER_SIZE;
            count--;
            not_full.notify_one();

            return item;
        }

};

monitor_buffer buf;

void producer(){
    for(int i = 0; i < QTD_PROD; i++){
        int item = rand() % 100;
        buf.insert(item);
        cout << "[P] Produziu: " << item << endl;
        sleep(1);
    }
}

void consumer(){
    for(int i = 0; i < QTD_PROD; i++){
        int item = buf.retrieve();
        cout << "[C] Consumi: " << item << endl;
        sleep(1);
    }
}

int main(){
    time_t t;
    srand(time(&t));
    thread prod(producer);
    thread cons(consumer);

    prod.join();
    cons.join();

    return 0;
}