#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(){
    int server_fd, new_socket;
    struct sockaddr_in endereco;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    endereco.sin_family = AF_INET;
    endereco.sin_addr.s_addr = INADDR_ANY;
    endereco.sin_port = htons(8000);

    /* o professor vai terminar na próxima aula */

    return 0;
}