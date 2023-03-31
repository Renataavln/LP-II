#include <iostream>
#include <string>
#include <thread>

class pessoa{
    int id;
    std::string nome;
    static int counter;

    public:
    pessoa(int i, std::string n):id(i), nome(n) {counter++;}
    int get_id(){return id;}
    void set_id(int i) {id = i;}
    std::string get_nome() {return nome;}
    void print_info(std::string msg){
        std::cout << "pessoa(";
        std::cout << id << ", ";
        std::cout << nome << ")" << std::endl;
        std::cout << msg << std::endl;
    }
    static void print_counter(){
        std::cout << "pessoas = " << counter << std::endl;
    }
};

int pessoa::counter = 0;
int main(){
    pessoa p1(1001, "Pessoa diferente");
    std::thread t1(&pessoa::print_info, &p1, "mensagem");

    t1.join();

    pessoa p2(1010, "Pessoa Generica");
    std::thread t2(pessoa::print_counter);

    t2.join();

    return 0;
}