#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#define SIZE 1024

void send_file(FILE *fp, int sockfd,char* fileName){
  int n;
  char data[SIZE] = {0};
  
  int fileNameLen = (int)strlen(fileName);
  char header[SIZE];
  header[0]=fileNameLen;
  
  strcpy(header+1,fileName);
  //sprintf(header,"%d%s",fileNameLen,fileName);
   if (send(sockfd, header, sizeof(header), 0) == -1) {
      perror("[-]Error in sending file header.");
      exit(1);
    }
  while(fgets(data, SIZE, fp) != NULL) {
    if (send(sockfd, data, sizeof(data), 0) == -1) {
      perror("[-]Error in sending file data.");
      exit(1);
    }
    printf("----------Sent data-----------\n");
    printf("File name: %s, length: %d\n",fileName,fileNameLen);
    printf("Header: %s, Data : %s\n",header,data);
    printf("------------------------------\n");
    
    bzero(data, SIZE);
  }
}

int main(int argc, char *argv[]){
  int port = 8080;
  int sockfd;
  struct sockaddr_in server_addr;
  FILE *fp;
  

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    perror("[-]Error in socket creation");
    exit(1);
  }
  printf("[+]Server socket created successfully.\n");

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  int err = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(err == -1) {
    perror("[-]Error in connection to server");
    exit(1);
  }
 printf("[+]Connected to Server.\n");
 
  char* fileName = argv[1];

  fp = fopen(fileName, "r");
  if (fp == NULL) {
    perror("[-]Error in reading file.");
    exit(1);
  }

  send_file(fp, sockfd,fileName);
  printf("[+]File data sent successfully.\n");

  printf("[+]Closing the connection.\n");
  close(sockfd);

  return 0;
}
