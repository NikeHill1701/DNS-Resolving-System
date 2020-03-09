#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#define PORT 8080 
#define SA struct sockaddr 

int stoi(char *st){
    int x=0, n=strlen(st);
    for(int i=0;i<n;i++){
        x *= 10;
        x += st[i]-'0';
    }
    return x;
}

void whatUgonnado(int sockfd){
	char buffer[256];
	int n; 
	memset(buffer,0,sizeof(buffer));
	recv(sockfd,buffer,sizeof(buffer),0);
	printf("proxy server sent %s to server\n",buffer);
	
	FILE *fptr;
	fptr = fopen("dns.txt","r");
	if(fptr == NULL)
	{
		printf("error in opening file\n");
		exit(0);
	}
	else
	{
		printf("File open successful\n");
	}

	char file_line[256];
	
	//if(straight ) limit by space
	int hit = 0;
	char err_msg[256]="1entry not found in the database\0";

	while(EOF != fscanf(fptr,"%[^\n]\n",file_line)){
		char sip[20]="0";
		char dname[30]="0";
		char delimiter='\0';
		int i,j;
		for (i = 0; i < 18; i++)
		{
			if(file_line[i]==' ')
				break;
		}
		strncat(sip,file_line,i);
		strncat(sip,&delimiter,1);
		printf("%s\n", sip);
		for (j = i+1; j < 256; j++)
		{
			if (file_line[j]=='\n')
				break;
		}
		strncat(dname,file_line+i+1,j-i-1);
		strncat(dname,&delimiter,1);  			
		printf("%s\n", dname);
		
		//ip f[0->i-1] size i
		//dom f[i+1 -> j-1] size j-i-1

		// requesting IP
		if (buffer[0]=='0')
		{
			if(strncmp(file_line+i+1,buffer+1,j-i-1)==0)
			{
				printf("Hit! %s\n", sip);
				int bytes_sent = send(sockfd, sip,strlen(sip)*sizeof(char),0);	
				printf("bytes sent: %d\n", bytes_sent);
				hit = 1;
				break;
			}
			
		}
		// requesting dname
		else if (buffer[0]=='1')
		{
			if(strncmp(file_line,buffer+1,i)==0)
			{
				send(sockfd, dname,strlen(dname)*sizeof(char),0);
				hit = 1;
				break;
			}
		}
	}
	if (!hit)
	{
		int bytes_sent = send(sockfd,err_msg,strlen(err_msg)*sizeof(char),0);
		printf("%s\tbytes_sent=%d\n", err_msg, bytes_sent);
	}
	//fprintf(,"%s\n",file_line );

	fclose(fptr);
	// and send that buffer to client 
	// write(sockfd, buffer, sizeof(buffer));

}

int main(int argc, char* argv[]){

	int sockfd, connfd, binded;
    // socket create and verification 
	sockfd= socket(AF_INET,SOCK_STREAM,0);

    if (sockfd < 0)
    {
    	printf("hold up!socket creation failed bruh:(\n");
    	exit(0);
    } 
	else
	{
    	printf("yayy! socket creation successful!\n");
	}
	int port = stoi(argv[1]);
	// Configure the socket's address.
	struct sockaddr_in address;
    bzero(&address,sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port=htons(port);

	if(bind(sockfd, (SA*) &address, sizeof(address)) < 0) {
        printf("hold up!socket binding failed bruh!\n"); 
        exit(0); 
    } 
    else
	{
        printf("I renounce socket binded\n");
	}
    //ready to listen
    if ((listen(sockfd,5)) < 0)
    {
    	printf("Dumbass socket!\n");
    	exit(0);
    }
    else
	{
    	printf("Socket's all ears\n");
	}
	int length = sizeof(address);
    connfd = accept(sockfd,(SA*)&address,&length);
    if (connfd<0)
    {
    	printf("Ah rejection!\n");
    	exit(0);
    }
    else
    	printf("YAY! server accepted client!\n");

    whatUgonnado(connfd);

    close(sockfd);

}
