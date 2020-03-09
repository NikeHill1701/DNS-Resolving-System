#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#define PORT 8080
#define MAX_LIMIT 50

char* getmessage(char st[], int flag){
    int n=strlen(st);
    string dst;
    if(n>4 && st[0]=='w' && st[1]=='w' && st[2]=='w' && st[3]=='.'){
        strcpy(dst,st+4,n-4);
    }
    else{
        dst=st;
    }
    return to_string(flag)+dst;
}

int main(){

    // socket file descriptor
    int sockfd = 0;
    int server_port;
    char server_ipaddr[MAX_LIMIT];

    // Setup server socket addr.
    struct sockaddr_in server_addr;         
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    inet_aton(server_ipaddr,&server_addr.sin_addr.s_addr);
    
    // create socket
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        printf("Error creating socket\n");
        return -1;
    }

    // establishing connection
    if(connect(sockfd,(struct sockaddr*)server_addr,sizeof(server_addr)<0){
        printf("Error establishing connection with the server\n");
        return -1;
    }
    char *requestmsg = getmessage(st,1);
    send(sockfd,requestmsg,sizeof(requestmsg));
    recv(sockfd,reply,1024);
}
