#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg){
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[]){

  int socketfd, port, n;
  struct sockaddr_in serverAddress;
  struct hostent *server;

  char buffer[256];

  if (argc < 3) {
   fprintf(stderr,"usage %s hostname port\n", argv[0]);
   exit(1);
  }

  port = atoi(argv[2]);
  socketfd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketfd < 0)
          error("ERROR opening socket");
  server = gethostbyname(argv[1]);

  if (server == NULL) {
    fprintf(stderr,"ERROR, no such host\n");
    exit(0);
  }

  bzero((char *) &serverAddress, sizeof(serverAddress));

  serverAddress.sin_family = AF_INET;
  bcopy((char *)server->h_addr,
         (char *)&serverAddress.sin_addr.s_addr,
         server->h_length);
    serverAddress.sin_port = htons(port);

      if (connect(socketfd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) >= 0){
        while(1){
          printf("Please enter the message: ");
          bzero(buffer,256);
          fgets(buffer,255,stdin);
          if(strcmp(buffer, "quit\n") == 0) break;
          n = send(socketfd, buffer, strlen(buffer),0);
          if (n < 0)
               error("ERROR writing to socket");
          bzero(buffer,256);
          printf("%s\n", buffer);
      }
      close(socketfd);
    } else error("ERROR connecting");

  return 0;
}