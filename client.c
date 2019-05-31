#include <sys/socket.h>  
 #include <sys/types.h>  
 #include <resolv.h>  
 #include <string.h>  
 // main entry point  
 int main(int argc, char* argv[])  
 {  
      //socket variables  
      char IP[200];  
      char port[200];  
      char buffer[65535];  
      int sd;  
      struct sockaddr_in client_sd;  
      printf("\nEnter proxy address:");  
      fgets(IP,sizeof("127.0.01\n")+1,stdin);  
      fputs(IP,stdout);  
      printf("\nEnter a port:");  
      fgets(port,sizeof("5000\n")+1,stdin);  
      fputs(port,stdout);  
      if((strcmp(IP,"127.0.0.1\n"))!=0 || (strcmp(port,"5000\n"))!=0)  
      {  
           printf("Invalida proxy settings. Try again...");  
      }  
      else  
      {  
           // create a socket  
           if((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)  
           {  
                printf("socket not created\n");  
           }  
           memset(&client_sd, 0, sizeof(client_sd));  
           // set socket variables  
           client_sd.sin_family = AF_INET;  
           client_sd.sin_port = htons(5000);  
           // assign any IP address to the client's socket  
           client_sd.sin_addr.s_addr = INADDR_ANY;   
           // connect to proxy server at mentioned port number  
           connect(sd, (struct sockaddr *)&client_sd, sizeof(client_sd));  
           //send and receive data contunuously  
           while(1)  
            {  
                printf("Type here:");  
                fgets(buffer, sizeof(buffer), stdin);  
                write(sd, buffer, sizeof(buffer));  
                printf("\nServer response:\n\n");  
                read(sd, buffer, sizeof(buffer));  
                fputs(buffer, stdout);  
                  //printf("\n");       
           };  
           //close(sd);  
      }  
      return 0;  
 }  
