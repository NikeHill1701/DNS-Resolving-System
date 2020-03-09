#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#define PORT 8080
#define MAX_LIMIT 50

int stoi(char *st){
    int x=0, n=strlen(st);
    for(int i=0;i<n;i++){
        x *= 10;
        x += st[i]-'0';
    }
    return x;
}

void getmessage(char st[], int flag, char *dst){
    int n=strlen(st);
    if(n>4 && st[0]=='w' && st[1]=='w' && st[2]=='w' && st[3]=='.'){
        dst = st+4;
    }else{
        dst=st;
    }
    char dst1[strlen(dst)+1];
    dst1[0] = '0'+flag;
    for (int i=1;i<strlen(dst1);i++)
        dst1[i] = dst[i-1];
    dst = dst1;
    return;
}

int main(int argc, char** argv){

    // socket file descriptor
    int sockfd = 0;
    int server_port;
    char *server_ipaddr;
    server_ipaddr = argv[1];
    server_port = stoi(argv[2]);
    printf("%s\t", server_ipaddr);
    printf("%d\n", server_port);
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
    }else{
        printf("Socket: 'Hello World'\n");
    }

    // establishing connection
    if(connect(sockfd,(struct sockaddr*) &server_addr,sizeof(server_addr))<0){
        printf("Error establishing connection with the server\n");
        return -1;
    }else{
        printf("Server:'Hello'\n");
    }
    char *requestmsg, st[50]; 
    //TODO:: Input dns request
    printf("Requesting input\n");
    scanf("%s", st);
    requestmsg = st;
    //getmessage(st,0,requestmsg);
    printf("Request msg from client: %s\n", requestmsg);
    send(sockfd,requestmsg,strlen(requestmsg)*sizeof(char),0);
    char *response;
    recv(sockfd,response,512,0);
    printf("Response from server: %s\n", response);
    return 0;
}
