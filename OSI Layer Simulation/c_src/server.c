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
static const char *labels[7]={"L1 Physical (handled by OS/hardware)","L2 Data Link (simulated)","L3 Network (simulated)","L4 Transport (simulated)","L5 Session (simulated)","L6 Presentation (simulated)","L7 Application"};
static const char *tags[6]={"DLK","NET","TRN","SES","PRS","APP"};
static int unwrap(char *s,const char *tag){
 size_t k=strlen(tag);if(strncmp(s,tag,k)||s[k]!='[')return -1;
 size_t n=strlen(s);if(n<k+2||s[n-1]!=']')return -1;
 memmove(s,s+k+1,n-k-2);s[n-k-2]=0;return 0;
}
int main(void){
#ifdef _WIN32
 WSADATA wd;if(WSAStartup(MAKEWORD(2,2),&wd)!=0){fprintf(stderr,"WSAStartup failed\n");return 1;}
#endif
 socket_t listenfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
#ifdef _WIN32
 if(listenfd==INVALID_SOCKET){fprintf(stderr,"socket failed\n");WSACleanup();return 1;}
#else
 if(listenfd<0){perror("socket");return 1;}
#endif
 int yes=1;setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,(const char*)&yes,sizeof yes);
 struct sockaddr_in a;memset(&a,0,sizeof a);a.sin_family=AF_INET;a.sin_addr.s_addr=htonl(INADDR_ANY);a.sin_port=htons(PORT);
 if(bind(listenfd,(struct sockaddr*)&a,sizeof a)<0||listen(listenfd,8)<0){fprintf(stderr,"bind/listen failed; port may be busy or blocked\n");CLOSESOCK(listenfd);return 1;}
 printf("OSI receiver listening on TCP port %d. Stop with Ctrl+C.\n",PORT);
 for(;;){
  socket_t fd=accept(listenfd,NULL,NULL);
#ifdef _WIN32
  if(fd==INVALID_SOCKET)continue;
#else
  if(fd<0)continue;
#endif
  uint32_t netlen;if(recv_all(fd,&netlen,sizeof netlen)){CLOSESOCK(fd);continue;}
  uint32_t len=ntohl(netlen);if(len==0||len>=MAX_FRAME){fprintf(stderr,"Invalid frame size.\n");CLOSESOCK(fd);continue;}
  char buf[MAX_FRAME];if(recv_all(fd,buf,len)){fprintf(stderr,"Incomplete payload.\n");CLOSESOCK(fd);continue;}buf[len]=0;
  puts("\n=== SERVER: DECAPSULATION (bottom-up) ===");
  int good=1;
  for(int i=0;i<6;i++){
   printf("%s: envelope received\n",labels[i]);
   if(unwrap(buf,tags[i])){fprintf(stderr,"Malformed envelope; dropping message.\n");good=0;break;}
   if(i==0||i==2||i==4)printf("  [progress] completed %d/7 layers\n",i+2);
  }
  if(good){printf("%s: delivered message: %s\n",labels[6],buf);uint32_t ack=htonl(0x4f534931u);send_all(fd,&ack,sizeof ack);}
  CLOSESOCK(fd);
 }
}
