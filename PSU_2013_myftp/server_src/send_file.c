/*
** send_file.c for myftp in /home/nguye_1/rendu/PSU_2013_myftp
**
** Made by Alexandre NGUYEN
** Login   <nguye_1@epitech.net>
**
** Started on  Sun Apr 13 21:45:44 2014 Alexandre NGUYEN
** Last update Sun Apr 13 23:18:27 2014 Alexandre NGUYEN
*/

#include "server.h"

void			ssend(t_server *server, char *size, int fd)
{
  char			tmp[1000];
  int			size_file;
  int			i;

  i = -1;
  bzero(tmp, 1000);
  strncat(tmp, size, strlen(size));
  write(server->cs, tmp, 1000);
  size_file = atoi(tmp);
  while (++i < (size_file / 1500))
    {
      bzero(server->buffer, 4096);
      read(fd, server->buffer, 1500);
      write(server->cs, server->buffer, 1500);
    }
  bzero(server->buffer, 4096);
  read(fd, server->buffer, size_file % 1500);
  write(server->cs, server->buffer, size_file % 1500);
}

void			put_next(t_server *server, int fd)
{
  FILE			*file;
  char			tmp[1000];
  int			i;

  printf("Sending [%s]\n", server->buffer);
  bzero(tmp, 1000);
  strncat(tmp, "stat ", 5);
  strncat(tmp, server->buffer, strlen(server->buffer));
  file = popen(tmp, "r");
  bzero(server->buffer, 4096);
  fgets(server->buffer, 4096, file);
  fgets(server->buffer, 4096, file);
  pclose(file);
  memmove(server->buffer, server->buffer + 8, strlen(server->buffer) - 8);
  i = -1;
  while (server->buffer[++i] != ' ')
    ;
  memset(server->buffer + i, 0, strlen(server->buffer) - i);
  ssend(server, server->buffer, fd);
}

void			put(t_server *server)
{
  int			fd;

  bzero(server->buffer, 4096);
  read(server->cs, server->buffer, 1000);
  if ((fd = open(server->buffer, O_RDONLY)) == -1)
    {
      write(server->cs, "ERROR\n", 6);
      perror(server->buffer);
    }
  else
    put_next(server, fd);
}
