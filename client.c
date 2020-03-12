#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#define PORT 8080
#define MAX_LIMIT 50

// function to convert string to integer
int stoi(char *st){
    int x=0, n=strlen(st);
    for(int i=0;i<n;i++){
        x *= 10;
        x += st[i]-'0';
    }
    return x;
}

// function that trims "www" infront of a url
void getmessage(char st[], int flag, char *dst){
    int n=strlen(st);
    dst[0] = '0'+flag;
    if(n>4 && st[0]=='w' && st[1]=='w' && st[2]=='w' && st[3]=='.') {
        strcat(dst, st+4);
    }
    else {
        strcat(dst, st);
    }
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
    inet_aton(server_ipaddr,(struct in_addr* )&server_addr.sin_addr.s_addr);
    
    // create socket
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0) {
        printf("Error creating socket\n");
        return 0;
    } else {
        printf("Socket: 'Hello World'\n");
    }

    // establishing connection
    if (connect(sockfd,(struct sockaddr*) &server_addr,sizeof(server_addr)) < 0) {
        printf("Error establishing connection with the server\n");
        return 0;
    } 
    else {
        printf("Server:'Hello'\n");
    }
    char requestmsg[256], st[256]; 
    int flag;

    // Input dns request message
    printf("Requesting input: request and type of request\n");
    scanf("%s %d", st, &flag);

    getmessage(st,flag,requestmsg);
    printf("Request msg to server: %s\n", requestmsg);

    send(sockfd,requestmsg,strlen(requestmsg)*sizeof(char),0);
    char response[256];
    recv(sockfd,response,sizeof(response),0);
    printf("Response from server: %s\n", response);
    printf("The requested data: %s\n", response+1);
    
    // free the socket
    close(sockfd);
    
    return 0;
}
