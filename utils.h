// utils.h
#ifndef UTILS_H
#define UTILS_H

int read_line(int fd, char *buffer);
int lock_file(int fd, int lock_type);
void unlock_file(int fd);
int authenticate_user(char*,char*,int);
int change_password_user(char*,char*,int);
#endif
