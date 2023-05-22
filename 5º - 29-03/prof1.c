#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
    int pid_o = getpid();
    int retval;
    int proc_clone[5];

    for(int i = 0; i < 5; i++){
        if(pid_o == getpid()){
            retval = fork();
            if(retval == 0){
                printf("[c] Processo clonado %d\n", i);
                sleep(1);
            }else{
                proc_clone[i] = retval;
            }
        }
    }

    if(getpid() == pid_o){
        for(int i = 0; i < 5; i++){
            waitpid(proc_clone[i], NULL, 0);
        }
    }

    return 0;
}