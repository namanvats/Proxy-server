#include <sys/socket.h>  
 #include <sys/types.h>  
 #include <resolv.h>  
 #include <string.h>  
 #include <stdlib.h>  
 #include <pthread.h>  
 #include<unistd.h>  
 #include<netdb.h> //hostent  
 #include<arpa/inet.h>  
 int hostname_to_ip(char * , char *);  
 // client will connect to server using given IP and port   
 struct serverInfo  
 {  
      int client_fd;  
      char ip[100];  
      char port[100];  
 };  
 // A thread function    
 void *runSocket(void *vargp)  
 {  
   struct serverInfo *info = (struct serverInfo *)vargp;  
   char buffer[65535];  
   int bytes =0;  
      printf("client:%d\n",info->client_fd);  
      fputs(info->ip,stdout);  
      fputs(info->port,stdout);  
      //code to connect to main server via this proxy server  
      int server_fd =0;  
      struct sockaddr_in server_sd;  
      // create a socket  
      server_fd = socket(AF_INET, SOCK_STREAM, 0);  
      if(server_fd < 0)  
      {  
           printf("server socket not created\n");  
      }  
      printf("server socket created\n");       
      memset(&server_sd, 0, sizeof(server_sd));  
      // set socket variables  
      server_sd.sin_family = AF_INET;  
      server_sd.sin_port = htons(atoi(info->port));  
      server_sd.sin_addr.s_addr = inet_addr(info->ip);  
      //connect to main server from this proxy server  
      if((connect(server_fd, (struct sockaddr *)&server_sd, sizeof(server_sd)))<0)  
      {  
           printf("server connection not established");  
      }  
      printf("server socket connected\n");  
      while(1)  
      {  
           //receive data from client  
           memset(&buffer, '\0', sizeof(buffer));  
           bytes = read(info->client_fd, buffer, sizeof(buffer));  
           if(bytes <= 0)  
           {  
           }  
           else   
           {  
                // send data to main server  
                write(server_fd, buffer, sizeof(buffer));  
                //printf("client fd is : %d\n",c_fd);                    
                printf("From client :\n");                    
                fputs(buffer,stdout);       
                  fflush(stdout);  
           }  
           //recieve response from server  
           memset(&buffer, '\0', sizeof(buffer));  
           bytes = read(server_fd, buffer, sizeof(buffer));  
           if(bytes <= 0)  
           {  
           }            
           else  
           {  
                // send response back to client  
                write(info->client_fd, buffer, sizeof(buffer));  
                printf("From server :\n");                    
                fputs(buffer,stdout);            
           }  
      };       
   return NULL;  
 }  
 // main entry point  
 int main(int argc,char *argv[])  
 {  
     pthread_t tid;  
     char port[100],ip[100];  
     char *hostname = argv[1];  
     char proxy_port[100];  
        // accept arguments from terminal  
        strcpy(ip,argv[1]); // server ip  
        strcpy(port,argv[2]);  // server port  
        strcpy(proxy_port,argv[3]); // proxy port  
        //hostname_to_ip(hostname , ip);  
        printf("server IP : %s and port %s" , ip,port);   
        printf("proxy port is %s",proxy_port);        
        printf("\n");  
      //socket variables  
      int proxy_fd =0, client_fd=0;  
      struct sockaddr_in proxy_sd;  
 // add this line only if server exits when client exits  
 signal(SIGPIPE,SIG_IGN);  
      // create a socket  
      if((proxy_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)  
      {  
          printf("\nFailed to create socket");  
      }  
      printf("Proxy created\n");  
      memset(&proxy_sd, 0, sizeof(proxy_sd));  
      // set socket variables  
      proxy_sd.sin_family = AF_INET;  
      proxy_sd.sin_port = htons(atoi(proxy_port));  
      proxy_sd.sin_addr.s_addr = INADDR_ANY;  
      // bind the socket  
      if((bind(proxy_fd, (struct sockaddr*)&proxy_sd,sizeof(proxy_sd))) < 0)  
      {  
           printf("Failed to bind a socket");  
      }  
      // start listening to the port for new connections  
      if((listen(proxy_fd, SOMAXCONN)) < 0)  
      {  
           printf("Failed to listen");  
      }  
      printf("waiting for connection..\n");  
      //accept all client connections continuously  
      while(1)  
      {  
           client_fd = accept(proxy_fd, (struct sockaddr*)NULL ,NULL);  
           printf("client no. %d connected\n",client_fd);  
           if(client_fd > 0)  
           {  
                 //multithreading variables      
                 struct serverInfo *item = malloc(sizeof(struct serverInfo));  
                 item->client_fd = client_fd;  
                 strcpy(item->ip,ip);  
                 strcpy(item->port,port);  
                 pthread_create(&tid, NULL, runSocket, (void *)item);  
                 sleep(1);  
           }  
      }  
      return 0;  
 }  
 int hostname_to_ip(char * hostname , char* ip)  
 {  
   struct hostent *he;  
   struct in_addr **addr_list;  
   int i;  
   if ( (he = gethostbyname( hostname ) ) == NULL)   
   {  
     // get the host info  
     herror("gethostbyname");  
     return 1;  
   }  
   addr_list = (struct in_addr **) he->h_addr_list;  
   for(i = 0; addr_list[i] != NULL; i++)   
   {  
     //Return the first one;  
     strcpy(ip , inet_ntoa(*addr_list[i]) );  
     return 0;  
   }  
   return 1;  
 }  
