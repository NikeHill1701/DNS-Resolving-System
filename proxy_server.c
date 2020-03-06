#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>

char *proxy2dns(char buff[])
{
    int sockfd, new_socket;
    struct sockaddr_in servaddr, client;
    
    if((sockfd=socket(AF_INET, SOCK_STREAM, 0))<0)
    {
        printf('Error in creating client socket for proxy\n');
        exit(0);
    }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    servaddr.sin_port=htons(PORT);

    if(connect(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr))!=0)
    {
        printf('Connection with DNS server failed\n');
        exit(0);
    }

    send(sockfd, buff, 1024, 0);

    char recv_buff[1024];
    memset(recv_buff, 0, 1024);
    int len_recv;
    len_recv=recv(sockfd, recv_buff, sizeof(recv_buff), 0);
    if(recv_buff[0]==0)
    {
        FILE *fp;
        fp=fopen('proxy_cache.txt', "r");
        int c=0;
        char line_buf[1024];
        memset(line_buf, 0, 1024);
        while(fgets(line_buf, 1024, fp))
            c++;
        fclose(fp);
        if(c<3)
        {
            fp=fopen('proxy_cache.txt', "a");
            char str;
            if(buff[0]=='0')
            {
                for(int i=1; i<1024 || buff[i]!='\0'; i++)
                    fputc(buff[i], fp);
                fputc(' ', fp);
                for(int i=1; i<1024 || recv_buff[i]!='\0'; i++)
                    fputc(recv_buff[i], fp);
                fputc('\n', fp);
            }
            else
            {
                for (int i = 1; i < 1024 || recv_buff[i] != '\0'; i++)
                    fputc(recv_buff[i], fp);
                fputc(' ', fp);
                for (int i = 1; i < 1024 || buff[i] != '\0'; i++)
                    fputc(buff[i], fp);
                fputc('\n', fp);
            }
        }
        else
        {
            FILE *fp1, *fpt;
            fp1=fopen('proxy_cache.txt', "r");
            fpt=fopen('temp.txt', "w");
            char copy_buff[1024];
            memset(copy_buff, 0, 1024);
            fgets(copy_buff, 1024, fp1);
            while(!feof(fp1))
            {
                fgets(copy_buff, 1024, fp1);
                fprintf(fpt, "%s", copy_buff);
                fprintf(fpt, "\n");
            }
            fclose(fp1);
            if (buff[0] == '0')
            {
                for (int i = 1; i < 1024 || buff[i] != '\0'; i++)
                    fputc(buff[i], fp);
                fputc(' ', fp);
                for (int i = 1; i < 1024 || recv_buff[i] != '\0'; i++)
                    fputc(recv_buff[i], fp);
                fputc('\n', fp);
            }
            else
            {
                for (int i = 1; i < 1024 || recv_buff[i] != '\0'; i++)
                    fputc(recv_buff[i], fp);
                fputc(' ', fp);
                for (int i = 1; i < 1024 || buff[i] != '\0'; i++)
                    fputc(buff[i], fp);
                fputc('\n', fp);
            }
            fclose(fpt);
            remove('proxy_cache.txt');
            rename('temp.txt', 'proxy_cache.txt');
        }
    }
    close(sockfd);
    return recv_buff;
}

void communicate(int sockfd)
{
    char buff[1024];
    int n;
    
    memset(buff, 0, 1024);
    int len_message;
    len_message = recv(sockfd, buff, sizeof(buff), 0);
    FILE *fp;
    fp=fopen('proxy_cache.txt', "r");
    for(int i=0; i<1024; i++)
    {
        if(buff[i]=='\0')
        {
            len_message=i;
            break;
        }
    }
    char line_buf[1024];
    memset(line_buf, 0, 1024);
    int cache_flag=0;
    
    while(fp && fgets(line_buf, 1024, fp))
    {
        if(buff[0]='0')
        {
            int i;
            for(i=0; i<1024; i++)
                if(line_buf[i]=' ')
                    break;

            int j=i+1, k;
            for(k=j; k<1024; k++)
                if(line_buf[k]='\n')
                    break;
            
            if(!strncmp(buff+1, line_buf, len_message-1))
                continue;
            
            cache_flag=1;

            char send_buf[1024];
            memset(send_buf, 0, 1024);
            for(int itr=j; itr<=k; itr++)
                send_buf[itr-j]=line_buf[itr];
            send(sockfd, send_buf, 1024, 0);
            break;
        }
        else if(buff[0]='1')
        {
            int i;
            for(i=0; i<1024l i++)
                if(line_buf[i]=' ')
                    break;
            int j=i+1;

            if(!strncmp(buff+1, line_buf+j, len_message-1))
                continue;
            
            cache_flag=1;
            char send_buf[1024];
            memset(send_buf, 0, 1024);
            for(int itr=0; itr<i; itr++)
                send_buf[itr]=line_buf[itr];
            send(sockfd, send_buf, 1024, 0);
            break;
        }
    }
    fclose(fp);
    if(cache_flag)
        return;
    char p2d_buff[1024];
    p2d_buff=proxy2dns(buff, len_message);
    send(sockfd, p2d_buff, 1024, 0);
}

int main()
{
    int PORT, sockfd, new_socket;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Error in creating server socket for proxy\n");
        exit(0);
    }
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = INADDR_ANY;

    if ((bind(sockfd, (struct sockaddr *)&address, sizeof(address))) < 0)
    {
        printf("Socket bind failed\n");
        exit(0);
    }

    if ((listen(sockfd, 5)) < 0)
    {
        printf("Listening failed\n");
        exit(0);
    }

    if ((new_socket = accept(sockfd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        printf("Server accept failed\n");
        exit(0);
    }
    communicate(new_socket);
    close(sockfd);
}