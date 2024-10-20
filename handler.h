// handler.h
#ifndef HANDLER_H
#define HANDLER_H

void *handle_client(void *socket_desc);
//char whoami[20];


#define MAX_USERS 100    // Maximum number of concurrent users
#define MAX_USERNAME_LEN 50

#endif // HANDLER_H
