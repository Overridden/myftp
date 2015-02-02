/*
** client.h for ftp in /home/nguye_1/rendu/PSU_2013_myftp
**
** Made by Alexandre NGUYEN
** Login   <nguye_1@epitech.net>
**
** Started on  Wed Apr  9 10:10:22 2014 Alexandre NGUYEN
** Last update Sun Apr 13 22:00:44 2014 Alexandre NGUYEN
*/

#ifndef CLIENT_H_
# define CLIENT_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <netdb.h>
#include <fcntl.h>

typedef struct		s_client
{
  struct sockaddr_in	sin;
  int			s;
  int			port;
  int			error;
  char			buffer[4096];
  char			server[4096];
}			t_client;

void			throw_error(char *str);

void			cget_next(t_client *client, int size_file, char *name);
void			cget(t_client *client);

void			csend_next(t_client *client, int size, int fd);
void			csend(t_client *client, char *size, int fd);
void			cput_next(t_client *client);
void			cput(t_client *client);
char			put_or_get(t_client *client);

void			do_client(t_client *client);
int			main(int ac, char **av);

#endif /* CLIENT_H_ */
