#include <stdio.h> 
#include <string.h> 
#include <unistd.h>
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#define MAX 80 
#define PORT 8080 
#define SA struct sockaddr 

void whattodo(int sockfd){
	char buffer[MAX];
	int n;
	char 
	for ( ; ; )
	{
		memset(buffer,0,sizeof(buffer));
		recv(sockfd,buffer,sizeof(buffer));
		printf("proxy server sent %s to server\n",buffer);
		
		FILE *fptr;
  		fptr=fopen("dns.txt","r")
  		if(fptr==NULL)
  		{
  			printf("error in opening file\n");
  			exit(1);
  		}
  		while(fgets())
        n = 0; 
        // copy server message in the buffer 
        while ((buff[n++] = getchar()) != '\n') 
            ;


  		char file_line[100];
  		
  		//if(straight ) limit by space
  		
  		while(fscanf(fptr,"%[^\n]",file_line)){
  			char err_msg[1024]="1entry not found in the database\0"
  			char sip[20]="";
  			char dname[30]="";
  			char delimiter="\0"
  			int i,j;
  			for (i = 0; i < 18; i++)
  			{
  				if(file_line[i]==' ')break;
  			}
  			strncat(sip,file_line,i);
  			strncat(sip,*delimiter,1);

  			for (j = i+1; j <50; j++)
  			{
  				if (file_line[j]=='\n')break;
  			}
			strncat(dname,file_line+i+1,j-i-1);
  			strncat(dname,*delimiter,1);  			
  			//ip f[0->i-1] size i
  			//dom f[i+1 -> j-1] size j-i-1

  			if (buff[0]=='0')//requesting IP
  			{
  				if(strncmp(file_line+i+1,buff+1,j-i-1)==0)
  					write(sockfd, sip,100);	
  			}
  			else if (buff[0]=='1')//requesting dname
  			{
  				if(strncmp(file_line,buff+1,i)==0)
  					write(sockfd, dname,100);	
  			}
  			else continue;
  		}
  		write(sockfd, err_msg,30);
  		fprintf( "%s\n",file_line );


  		fclose(fptr);
        // and send that buffer to client 
        write(sockfd, buff, sizeof(buff));

	}
}

int main(){
	int sockfd, connfd, length,binded;
    struct sockaddr_in address,client;
    // socket create and verification 
	sockfd= socket(AF_INET,SOCK_STREAM,0);

    if (sockfd < 0)
    {
    	printf("hold up!socket creation failed bruh!\n");
    	exit(0);
    }
    else
    	printf("yayy! socket creation successful!\n");

    bzero(&address,sizeof(address));

    address.sin_family=AF_INET;
    address.sin_addr.s_addr=htonl(KP_IP);
    address.sin_port=htons(K_P_ORT);

    binded=bind(sockfd,(SA*)&address,sizeof(address));

    if (binded) != 0) { 
        printf("hold up!socket binding failed bruh!\n"); 
        exit(0); 
    } 
    else
        printf("I renounce socket binded\n");

    //ready to listen
    if (lister(sockfd,5) <0)
    {
    	printf("Socket ain't listening bruh!\n");
    	exit(0);
    }
    else
    	printf("Socket's all ears\n");

    connfd=accept(sockfd,(SA*)&client,&length);
    if (connfd<0)
    {
    	printf("Ah rejection!\n");
    	exit(0);
    }
    else
    	printf("YAY! server accepted client!\n");

    whattodo(connfd);

    close(sockfd);

}