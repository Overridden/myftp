/*
** get_file.c for myftp in /home/nguye_1/rendu/PSU_2013_myftp
**
** Made by Alexandre NGUYEN
** Login   <nguye_1@epitech.net>
**
** Started on  Sun Apr 13 21:46:34 2014 Alexandre NGUYEN
** Last update Sun Apr 13 23:24:04 2014 Alexandre NGUYEN
*/

#include "server.h"

void			get_next(t_server *server, int size, int file)
{
  int			i;

  i = -1;
  while (++i < (size / 1500))
    {
      bzero(server->buffer, 4096);
      read(server->cs, server->buffer, 1500);
      write(file, server->buffer, 1500);
    }
  bzero(server->buffer, 4096);
  read(server->cs, server->buffer, (size % 1500));
  write(file, server->buffer, 1500);
}

void			get(t_server *server)
{
  char			name[1000];
  char			size[1000];
  int			size_file;
  int			file;

  bzero(server->buffer, 4096);
  bzero(name, 1000);
  bzero(size, 1000);
  read(server->cs, server->buffer, 1500);
  strncat(size, server->buffer, strlen(server->buffer) -
	  strlen(strstr(server->buffer, "5KU3gG6i6Wg3huQv7hsS84g9Vc7P5FBe")));
  size_file = atoi(size);
  memmove(server->buffer, &server->buffer[strlen(size) + 32],
	  strlen(server->buffer) - strlen(size) - 32);
  memset(&server->buffer[strlen(server->buffer) - strlen(size) - 32], 0,
	 strlen(server->buffer) - (strlen(server->buffer) - strlen(size) - 32));
  strncat(name, server->buffer, strlen(server->buffer) -
	  strlen(strstr(server->buffer, "eqbPv9Ae6NU2886ZJy4N873mVzC6zrHs")));
  printf("getting [%s]\n", name);
  bzero(server->buffer, 4096);
  if ((file = open(name, O_CREAT | O_WRONLY | O_TRUNC,S_IRWXU | S_IRWXG | S_IRWXO)) == -1)
    perror(name);
  get_next(server, size_file, file);
  close(file);
}
