/*
** server.h for ftp server in /home/nguye_1/rendu/PSU_2013_myftp
**
** Made by Alexandre NGUYEN
** Login   <nguye_1@epitech.net>
**
** Started on  Wed Apr  9 10:23:43 2014 Alexandre NGUYEN
** Last update Sun Apr 13 21:44:54 2014 Alexandre NGUYEN
*/

#ifndef SERVER_H_
# define SERVER_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <netdb.h>
#include <stdio.h>

typedef struct		s_server
{
  struct sockaddr_in	sin;
  struct sockaddr_in	sin_client;
  int			client_len;
  int			s;
  int			port;
  int			error;
  char			*str;
  int			cs;
  char			*func_name[6];
  char			buffer[4096];
  int			deep;
}			t_server;

void			throw_error(char *str);

void			ls(t_server *server);

void			pwd(t_server *server);

void			quit(t_server *server);

char			check_cd(t_server *server);
void			cd(t_server *server);

void			get_next(t_server *server, int size, int file);
void			get(t_server *server);

void			ssend(t_server *server, char *size, int fd);
void			put_next(t_server *server, int fd);
void			put(t_server *server);

void			do_client(t_server *server);
void			assign_func(t_server *server);
void			init(t_server *server);
int			main(int ac, char **av);


#endif /* SERVER_H_ */
