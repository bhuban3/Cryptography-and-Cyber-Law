#ifndef OSI_COMMON_H
#define OSI_COMMON_H
#include <stddef.h>
#define PORT 5055
#define MAX_FRAME 8192
#define MAX_MSG 4096
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
typedef SOCKET socket_t;
#define CLOSESOCK closesocket
#else
#include <sys/types.h>
typedef int socket_t;
#define CLOSESOCK close
#endif
int send_all(socket_t fd,const void *buf,size_t len);
int recv_all(socket_t fd,void *buf,size_t len);
#endif
