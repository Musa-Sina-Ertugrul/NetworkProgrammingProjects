#include "local.hpp"
#include <iostream>
#include <cstdio>
#include <ctype.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[ ]) {
    int mid, n;
    MESSAGE msg;

    mid = atoi(argv[1]);

    unordered_map<string,long int> users = {{"USSEnterprise",-1L},{"USSVoyager",-1L},{"USSColumbia",-1L}};

    while (1) {
        memset( msg.buffer, 0x0, BUFSIZ );
        if ((n=msgrcv(mid, &msg, BUFSIZ, SERVER, 0)) == -1 ) {
            perror("Server: msgrcv");
            return 2;
        }
        else if (n == 0) break;
        string&& name = process_msg(&msg,&users);

        if( name == ""){
            msg.msg_to = msg.msg_fm;
            msg.msg_fm = SERVER;
            memset(msg.buffer,0x0,BUFSIZ);
            unsigned int i = 0;

            for( auto letter : "Something is wrong in your message"){
                msg.buffer[i] = letter;
                i++;
            }
            n += sizeof(msg.msg_fm);
            if (msgsnd(mid, &msg, n, 0) == -1 ) {
                perror("Server: msgsnd");
                return 3;
            }

        }else{
            msg.msg_to = users[name];

            n += sizeof(msg.msg_fm);
            n += sizeof(msg.msg_to);
            if (msgsnd(mid, &msg, n, 0) == -1 ) {
                perror("Server: msgsnd");
                return 3;
            }
        }

    }

    msgctl(mid, IPC_RMID, (struct msqid_ds *) 0 );
    exit(0);
}