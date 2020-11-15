#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<pthread.h>
#define SIZE 1024

void write_file(int sockfd){
  int byteCount;
  FILE *fp;
  char buffer[SIZE];
  int fileNameLen;
  byteCount = recv(sockfd, buffer, SIZE, 0);
    if (byteCount <= 0){
      return;
    }
    fileNameLen=(int)(buffer[0]);
    char fileName[fileNameLen];
    fileName[fileNameLen]='\0';
    strcpy(fileName,buffer+1);
    fp = fopen(fileName, "w");
  while (1) {
    byteCount = recv(sockfd, buffer, SIZE, 0);
    if (byteCount <= 0){
      break;
      return;
    }
    fprintf(fp, "%s", buffer);
    //fwrite(buffer,1,n,fp);
    bzero(buffer, SIZE);
  }
  printf("[<]File recived: %s, length: %d\n",fileName,fileNameLen);
  printf("[+]Data written in the file successfully.\n");
  printf("-----------------------------------------\n");
  fclose(fp);
  close(sockfd);
  return;
}

int main(){

  int port = 8080;
  int err;
  pid_t childPID;
  int sockfd, new_sock;
  struct sockaddr_in server_addr, new_addr;
  socklen_t addr_size;
  char buffer[SIZE];
int childProcCount;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    perror("[-]Error in socket");
    exit(1);
  }
  printf("[+]Server socket created successfully.\n");

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  err = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(err < 0) {
    perror("[-]Error in bind");
    exit(1);
  }
  printf("[+]Binding successfull.\n");

  if(listen(sockfd, 10) == 0){
    printf("[+]Listening....\n");
 }else{
    perror("[-]Error in listening");
    exit(1);
 }
  addr_size = sizeof(new_addr);
  while(1){

    new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);

   if ( (childPID = fork()) == 0 ) {

      close(sockfd); /* child closes listening socket */

      write_file(new_sock);

      close(new_sock);
      exit(0);  /* child terminates
    }
    close(connfd);  /*parent closes connected socket*/
}
    
  }
  return 0;
}