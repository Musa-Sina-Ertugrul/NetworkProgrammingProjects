#ifndef LOCAL
#define LOCAL

#define _GNU_SOURCE
#include <cstdio>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <unordered_map>

using namespace std;

const char SEED ='M';
const long int SERVER=1L;

typedef struct MESSAGE
{
    long int msg_to;
    long int msg_fm;
    char buffer[BUFSIZ];
};

// Functions
bool inline check_to(char *);
string process_msg(MESSAGE* msg,unordered_map<string,long int>*);
bool inline check_begginning_square_brackets(char square_brackets);
bool inline check_ending_square_brackets(char square_brackets);
bool check_correct_name(char * name,int len,unordered_map<string,long int>*);
void inline initialize_pid(long int* pid, string* name,unordered_map<string,long int>*);
string process_communication(char * msg, int len,unordered_map<string,long int>*);
string process_login(char * msg, int len, long int * pid,unordered_map<string,long int>*);

bool check_correct_name(char * name,int len,unordered_map<string,long int>* users){
    string tmp_name(name);

    for(unsigned int i = 0;i<len;tmp_name += name[i], i++);
    printf("%s",tmp_name);
    if((*users).find(tmp_name) != (*users).end()){
        return true;
    }

    return false;
}

void inline initialize_pid(long int* pid, string* name,unordered_map<string,long int>* users){

    if((*users)[*name] == -1L) (*users)[*name] = *pid;
    else perror("1. Close app \n2. Open app \n3. Enter a!her name \n!E : Possible names USSEnterprise, USSVoyager, USSColumbia");
    printf("%d",(*users)[*name]);

}

bool inline check_login(char * msg){

    return msg[0] == 'l' && msg[1] == 'o' && msg[2] == 'g' && msg[3] == 'i' && msg[4] == 'n';

}

string process_msg(MESSAGE* msg,unordered_map<string,long int>*users){

    try{
        string&& result = process_login(msg->buffer,strlen(msg->buffer),&(msg->msg_fm),users);
        if(result != ""){
            return result;
        }

        result = process_communication(msg->buffer,strlen(msg->buffer),users);
        if(result != ""){
            return result;
        }

    }catch(int error_code){

        printf("error recived %d ( Probally msg has ! enough length )", error_code);
        return "";

    }
}

string process_login(char * msg, int len, long int * pid,unordered_map<string,long int>* users){
    if(check_login(msg)){
        unsigned int index = 5;
        string name = "";
        
        if (check_begginning_square_brackets(msg[index]) && index < len){
            index++;
        }else{
            return "";
        }

        while(! check_ending_square_brackets(msg[index]) && index < len){
            name += msg[index];
            index++;
        }

        if((*users).find(name) == users->end()){
            printf("Wrong name %s",name);
            perror("");
            return "";

        }

        initialize_pid(pid, &name,users);
        return name;
    }

    return "";
}

string process_communication(char * msg, int len,unordered_map<string,long int>* users){
    if( check_to(msg) ){
        unsigned int index = 2;
        string name = "";

        if (check_begginning_square_brackets(msg[index]) && index < len){
            index++;
        }else{
            return "";
        }

        while(! check_ending_square_brackets(msg[index]) && index < len){
            name += msg[index];
            index++;
        }

        if((*users).find(name) == users->end()){
            printf("Wrong name %s",name);
            perror("");
            return "";
        }

        return name;
    }

    return "";
}

bool inline check_begginning_square_brackets(char square_brackets){

    return square_brackets == '[';
}

bool inline check_ending_square_brackets(char square_brackets){

    return square_brackets == ']';
}

bool inline check_to(char * msg){

    return msg[0] == 't' && msg[1] == 'o';

}

#endif
