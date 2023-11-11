#include "local.hpp"
#include <cstdio>

using namespace std;

int main(){
    key_t key;
    pid_t cli_pid;
    MESSAGE msg;
    int mid, n;
    static char m_key[10];
    cli_pid = getpid( );
    
    if ((key = ftok(".", SEED)) == -1) {
        perror("Client: key generation");
        return 1;
    }

    if ((mid=msgget(key, 0 )) == -1 ) {
        mid = msgget(key,IPC_CREAT | 0660);
        switch (fork()) {
            case -1:
                perror("Client: fork");
                return 2;
            case 0:
                sprintf(m_key, "%d", mid);
                execlp("./server", "server", m_key, "&", 0);
                perror("Client: exec");
                return 3;
        }
    }

    while (true) {
        msg.msg_to = SERVER;
        msg.msg_fm = cli_pid;
        write(fileno(stdout), "cmd> ", 6);
        memset(msg.buffer, 0x0, BUFSIZ);
        if ( (n=read(fileno(stdin), msg.buffer, BUFSIZ)) == 0 ) break;
        n += sizeof(msg.msg_fm);
        
        if (msgsnd(mid, &msg, n, 0) == -1 ) {
            perror("Client: msgsend");
            return 4;
        }

        if( (n=msgrcv(mid, &msg, BUFSIZ, cli_pid, 0)) != -1 ){
            if(msg.msg_to != cli_pid){
                if (msgsnd(mid, &msg, n, 0) == -1 ) {
                    perror("Client: msgsend");
                    return 4;
                }
            }else{
                write(fileno(stdout), msg.buffer, n);
            }
        } 

    }

    msgsnd(mid, &msg, 0, 0);

    return 0;

}