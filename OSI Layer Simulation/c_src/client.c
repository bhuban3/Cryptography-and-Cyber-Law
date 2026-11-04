#include "osi_common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif
static const char *labels[7]={"L7 Application","L6 Presentation","L5 Session","L4 Transport (simulated)","L3 Network (simulated)","L2 Data Link (simulated)","L1 Physical (simulated)"};
static const char *tags[6]={"APP","PRS","SES","TRN","NET","DLK"};
static int wrap(char *out,size_t cap,const char *tag,const char *input){
 int n=snprintf(out,cap,"%s[%s]",tag,input);return n<0||(size_t)n>=cap?-1:n;
}
int main(int argc,char **argv){
#ifdef _WIN32
 WSADATA wd;if(WSAStartup(MAKEWORD(2,2),&wd)!=0){fprintf(stderr,"WSAStartup failed\n");return 1;}
#endif
 const char *host=argc>1?argv[1]:"127.0.0.1";
 char message[MAX_MSG],current[MAX_FRAME],next[MAX_FRAME];
 if(argc>2)snprintf(message,sizeof message,"%s",argv[2]);
 else {printf("Message to send: ");if(!fgets(message,sizeof message,stdin))return 1;message[strcspn(message,"\r\n")]=0;}
 if(strlen(message)==0){fprintf(stderr,"Empty message not allowed.\n");return 1;}
 snprintf(current,sizeof current,"%s",message);
 puts("\n=== CLIENT: ENCAPSULATION (top-down) ===");
 for(int i=0;i<7;i++){
   printf("%s: %s\n",labels[i],i==0?current:"encapsulating");
   if(i<6){int n=wrap(next,sizeof next,tags[i],current);if(n<0){fprintf(stderr,"Payload too large.\n");return 1;}strcpy(current,next);}
   if(i==1||i==3||i==5)printf("  [progress] completed %d/7 layers\n",i+1);
 }
 uint32_t len=(uint32_t)strlen(current),netlen=htonl(len);
 socket_t fd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
#ifdef _WIN32
 if(fd==INVALID_SOCKET){fprintf(stderr,"socket failed: %d\n",WSAGetLastError());WSACleanup();return 1;}
#else
 if(fd<0){perror("socket");return 1;}
#endif
 struct sockaddr_in addr;memset(&addr,0,sizeof addr);addr.sin_family=AF_INET;addr.sin_port=htons(PORT);
 addr.sin_addr.s_addr=inet_addr(host);
 if(addr.sin_addr.s_addr==INADDR_NONE && strcmp(host,"255.255.255.255")!=0){fprintf(stderr,"Use a numeric IPv4 address, e.g. 127.0.0.1.\n");CLOSESOCK(fd);return 1;}
 if(connect(fd,(struct sockaddr*)&addr,sizeof addr)<0){fprintf(stderr,"connect failed; start server first (Windows error %d)\n",
#ifdef _WIN32
 WSAGetLastError()
#else
 0
#endif
 );CLOSESOCK(fd);return 1;}
 if(send_all(fd,&netlen,sizeof netlen)||send_all(fd,current,len)){fprintf(stderr,"send failed\n");CLOSESOCK(fd);return 1;}
 uint32_t ack=0;if(recv_all(fd,&ack,sizeof ack)){fprintf(stderr,"No server acknowledgement.\n");CLOSESOCK(fd);return 1;}
 CLOSESOCK(fd);
#ifdef _WIN32
 WSACleanup();
#endif
 puts("\n[network] payload sent through a real TCP socket.");
 puts("[client] server acknowledged receipt.");return 0;
}
