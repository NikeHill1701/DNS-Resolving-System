#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>

// Function for converting a string to integer
int stoi(char *st){
    int x=0, n=strlen(st);
    for(int i=0;i<n;i++){
        x *= 10;
        x += st[i]-'0';
    }
    return x;
}

//Function for connecting with the DNS Server and updating the Proxy Cache
int proxy2dns(char buff[], char *p2d_buff)
{
    int sockfd, server_port;
    char server_ip[256];
    struct sockaddr_in servaddr, client;
    printf("Please input the DNS server IP and Port:\n");
    scanf("%s", server_ip);
    scanf("%d", &server_port);
    printf("Server_ip: %s\tserver_port: %d\n", server_ip, server_port);

    //Creating socket
    if((sockfd=socket(AF_INET, SOCK_STREAM, 0))<0)
    {
        printf("Error in creating client socket for proxy\n");
        return -1;
    }
    else
    {
        printf("Client socket for proxy created\n");
    }
    
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=inet_addr(server_ip);
    servaddr.sin_port=htons(server_port);

    // Connecting to the server
    if(connect(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr))!=0)
    {
        printf("Connection with DNS server failed\n");
        return -1;
    }
    else
    {
        printf("Connection with DNS server established\n");
    }
    
    // Forward the cache miss request to the DNS server
    send(sockfd, buff, 256, 0);

    char recv_buff[256];
    int len_recv = recv(sockfd, recv_buff, sizeof(recv_buff), 0);   // Recieve data form the DNS server
    
    if(recv_buff == NULL){
        printf("DNS server did not respond\n");
        return -1;
    }

    if(recv_buff[0]=='0')   // If the DNS server contains the requested data
    {
        FILE *fp;
        fp=fopen("proxy_cache.txt", "r");   // Proxy cache
        int c=0;
        char line_buf[256]; // Buffer for reading the proxy cache
        memset(line_buf, 0, 256);
        while(fgets(line_buf, 256, fp))
            c++;    // Counting the number of entries in cache
        fclose(fp);

        if(c<3) // If the cache has less than 3 entries, just append the new entry to the bottom
        {
            fp=fopen("proxy_cache.txt", "a");
            char str;
            if(buff[0]=='0')    // If the request is for IP address
            {
                for(int i=1; i<256 && buff[i]!='\0'; i++)
                    fputc(buff[i], fp);
                fputc(' ', fp);
                for(int i=1; i<256 && recv_buff[i]!='\0'; i++)
                    fputc(recv_buff[i], fp);
                fputc('\n', fp);
            }
            else    // If the request is for Domain name
            {
                for (int i = 1; i < 256 && recv_buff[i] != '\0'; i++)
                    fputc(recv_buff[i], fp);
                fputc(' ', fp);
                for (int i = 1; i < 256 && buff[i] != '\0'; i++)
                    fputc(buff[i], fp);
                fputc('\n', fp);
            }
        }
        else    // If the cache has 3 entries, delete the 1st entry and append the new entry at the bottom
        {
            FILE *fp1, *fpt;
            fp1=fopen("proxy_cache.txt", "r");
            fpt=fopen("temp.txt", "w");
            char copy_buff[256];
            memset(copy_buff, 0, 256);
            fgets(copy_buff, 256, fp1);

            // Copy the 2nd and 3rd entry into the temp file
            while(c-- > 1)
            {
                fgets(copy_buff, 256, fp1);
                fprintf(fpt, "%s", copy_buff);
            }
            fclose(fp1);

            // Add the new entry to the bottom
            if (buff[0] == '0')
            {
                for (int i = 1; i < 256 && buff[i] != '\0'; i++)
                    fputc(buff[i], fpt);
                fputc(' ', fpt);
                for (int i = 1; i < 256 && recv_buff[i] != '\0'; i++)
                    fputc(recv_buff[i], fpt);
                fputc('\n', fpt);
            }
            else
            {
                for (int i = 1; i < 256 && recv_buff[i] != '\0'; i++)
                    fputc(recv_buff[i], fpt);
                fputc(' ', fpt);
                for (int i = 1; i < 256 && buff[i] != '\0'; i++)
                    fputc(buff[i], fpt);
                fputc('\n', fpt);
            }
            fclose(fpt);
            remove("proxy_cache.txt");
            rename("temp.txt", "proxy_cache.txt");
        }
    }
    
    printf("Response from DNS server: %s\n", recv_buff);
    close(sockfd);
    strcpy(p2d_buff, recv_buff);
    return 0;
}

// Function for checking the proxy cache for the required query
int communicate(int sockfd)
{
    char buff[256]; //Buffer for the message from the client    
    memset(buff, 0, 256);
    int len_message;
    len_message = recv(sockfd, buff, sizeof(buff), 0); //Recieving the message from the client
    printf("Request msg from client: %s\n", buff);

    FILE *fp;
    if(fp=fopen("proxy_cache.txt", "r"))
    {
        printf("File open successful\n");
    }
    else
    {
        printf("File open failed\n");
    }

    char line_buf[256]; //Buffer for reading each line of the proxy cache file
    memset(line_buf, 0, 256);
    int cache_flag=0;

    while(fgets(line_buf, 256, fp))
    {
        if(buff[0]=='0') //If the request is for the IP address
        {

            int i;
            //Domain 
            for(i=0; i<sizeof(line_buf)/sizeof(char); i++)
                if(line_buf[i]==' ')
                    break;
            
            //IP
            int j=i+1, k;
            for(k=j; k<sizeof(line_buf)/sizeof(char); k++)
                if(line_buf[k]=='\n')
                    break;

            if(strncmp(buff+1, line_buf, len_message-1) != 0) 
                continue;   //String comparision failed for this line

            cache_flag=1;   //Cache hit

            char send_buf[k-j+2];
            printf("Buffer created successfully\n");

            send_buf[0]='0';
            for(int itr=j+1; itr<=k; itr++)
                send_buf[itr-j]=line_buf[itr-1];    //The required IP address
            send_buf[k-j+1] = '\0';

            printf("Response to client: %s\n", send_buf);
            send(sockfd, send_buf, sizeof(send_buf), 0);
            break;
        }
        else if(buff[0]=='1') // If the request is for the Domain name
        {
            int i;
            //Domain 
            for(i=0; i<sizeof(line_buf)/sizeof(char); i++)
                if(line_buf[i]==' ')
                    break;
            int j=i+1;

            if(strncmp(buff+1, line_buf+j, len_message-1) != 0)
                continue;
            
            cache_flag=1;
            char send_buf[i+2];

            send_buf[0]='0';
            for(int itr=1; itr<=i; itr++)
                send_buf[itr]=line_buf[itr-1];
            send_buf[i+1]='\0';

            send(sockfd, send_buf, sizeof(send_buf), 0);
            break;
        }
    }
    fclose(fp);

    if(cache_flag)
        return 0;   //If cache hit
    printf("Proxy cache miss\n");

    char p2d_buff[256]; //Buffer to recieve data from DNS server
    proxy2dns(buff, p2d_buff);
    printf("p2d_buff: %s\n", p2d_buff);
    send(sockfd, p2d_buff, 256, 0);
    return 0;
}

// Main function for enabling the client to connect with the proxy server
int main(int argc, char *argv[])
{
    int port, sockfd, new_socket;
    //Creating socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Error in creating server socket for proxy\n");
        return -1;
    }
    else
    {
        printf("Server socket for proxy created\n");
    }
    

    port=stoi(argv[1]);
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;
    //Binding the socket
    if ((bind(sockfd, (struct sockaddr *)&address, sizeof(address))) < 0)
    {
        printf("Socket bind failed\n");
        return -1;
    }
    else
    {
        printf("Socket bind successful for proxy\n");
    }
    
    //Listening for the client
    if ((listen(sockfd, 5)) < 0)
    {
        printf("Listening failed\n");
        return -1;
    }
    else
    {
        printf("Proxy server listening\n");
    }
    
    //Accepting the connection
    int len = sizeof(address);
    if ((new_socket = accept(sockfd, (struct sockaddr *)&address, &len)) < 0)
    {
        printf("Server accept failed\n");
        return -1;
    }
    else
    {
        printf("Connection accpeted from client\n");
    }
    
    communicate(new_socket);
    //Closing the connection
    close(sockfd);
}