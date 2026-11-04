#include "osi_common.h"
#ifdef _WIN32
#define IO_SEND(s,b,n) send((s),(b),(int)(n),0)
#define IO_RECV(s,b,n) recv((s),(b),(int)(n),0)
#else
#include <sys/socket.h>
#define IO_SEND(s,b,n) send((s),(b),(n),0)
#define IO_RECV(s,b,n) recv((s),(b),(n),0)
#endif
int send_all(socket_t fd,const void *buf,size_t len){
 const char *p=(const char*)buf; size_t done=0;
 while(done<len){int n=IO_SEND(fd,p+done,len-done);if(n<=0)return -1;done+=(size_t)n;}return 0;
}
int recv_all(socket_t fd,void *buf,size_t len){
 char *p=(char*)buf;size_t done=0;
 while(done<len){int n=IO_RECV(fd,p+done,len-done);if(n<=0)return -1;done+=(size_t)n;}return 0;
}
