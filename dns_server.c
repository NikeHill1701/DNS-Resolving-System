#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#define PORT 8080 
#define SA struct sockaddr 

int stoi(char *st){          //function that converts a string to integer,used for port number received
    int x=0, n=strlen(st);
    for(int i=0;i<n;i++){
        x *= 10;
        x += st[i]-'0';
    }
    return x;
}

int whatUgonnado(int sockfd){ //It is called once connection of proxy_server is accepted by DNS server
	char buffer[256];
	int n; 
	memset(buffer,0,sizeof(buffer));
	recv(sockfd,buffer,sizeof(buffer),0);
	printf("proxy server sent %s to server\n",buffer);
	
	FILE *fptr;
	fptr = fopen("dns.txt","r"); //dns.txt is our directory of address mapings 
	if(fptr == NULL)
	{
		printf("error 404: File not found\n");
		return 0;
	}
	else
	{
		printf("File open successful\n");
	}

	char file_line[256];
	int hit = 0;
	char err_msg[256]="1entry not found in the database\0";

	while(EOF != fscanf(fptr,"%[^\n]\n",file_line)){
		char ip_addr[20]="0";
		char dom_name[30]="0";
		char delimiter='\0';
		int i,j;
		for (i = 0; i < 18; i++)
		{
			if(file_line[i]==' ')
				break;
		}
		strncat(ip_addr,file_line,i);
		strncat(ip_addr,&delimiter,1);

		for (j = i+1; j < 256; j++)
		{
			if (file_line[j]=='\n')
				break;
		}
		strncat(dom_name,file_line+i+1,j-i-1);
		strncat(dom_name,&delimiter,1);  			
		
		//ip address is of length i and is f[0->i-1]
		//domain name is of size j-i-1 and is f[i+1 -> j-1] 

		// proxy_server requesting IP addresss
		if (buffer[0]=='0')
		{
			if(strncmp(file_line+i+1,buffer+1,j-i-1)==0)
			{
				printf("Boom Hit! Ip requested is:  %s\n", ip_addr);
				send(sockfd, ip_addr,strlen(ip_addr)*sizeof(char),0);
				hit = 1;
				break;
			}
			
		}
		//proxy_server requesting domain_name
		else if (buffer[0]=='1')
		{
			if(strncmp(file_line,buffer+1,i)==0)
			{
				printf("Boom Hit! Domain name requested is:  %s\n", dom_name);
				send(sockfd, dom_name,strlen(dom_name)*sizeof(char),0);
				hit = 1;
				break;
			}
		}
	}
	if (!hit)
	{
		send(sockfd,err_msg,strlen(err_msg)*sizeof(char),0);
		printf("%s\n", err_msg);
	}

	fclose(fptr);
	close(sockfd);
	return 0;
}

int main(int argc, char* argv[]){

	int sockfd, connfd;
    // socket create and verification 
	sockfd= socket(AF_INET,SOCK_STREAM,0);
	//creating a IPv4 socket for TCP returns Socet file descriptor
    if (sockfd < 0)
    {
    	printf("hold up!socket creation failed bruh:(\n");
    	return 0;
    } 
	else
	{
    	printf("yayy! socket creation successful!\n");
	}
	int port = stoi(argv[1]);
	// Configurinng the socket's address.
	struct sockaddr_in address;
    bzero(&address,sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port=htons(port);

	if(bind(sockfd, (SA*) &address, sizeof(address)) < 0) {
        printf("hold up!socket binding failed!\n"); 
        return 0; 
    } 
    else
	{
        printf("I renounce socket binded\n");
	}
    if ((listen(sockfd,5)) < 0)
    {
    	printf("Arrogant socket!\n");
    	return 0;
    }
    else
	{
    	printf("Socket's all ears\n");
	}
	int length = sizeof(address);
	while (1)
	{
		connfd = accept(sockfd,(SA*)&address,&length);
    	if (connfd<0)
    	{
    		printf("Ah rejection!\n");
    		continue;
    	}
    	else
    		printf("YAY! server accepted client!\n");

    	whatUgonnado(connfd);
	}

    close(sockfd);
	return 0;
}
