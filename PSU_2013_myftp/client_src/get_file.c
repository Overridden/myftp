/*
** get_file.c<2> for myftp in /home/nguye_1/rendu/PSU_2013_myftp
**
** Made by Alexandre NGUYEN
** Login   <nguye_1@epitech.net>
**
** Started on  Sun Apr 13 21:58:36 2014 Alexandre NGUYEN
** Last update Sun Apr 13 23:24:50 2014 Alexandre NGUYEN
*/

#include "client.h"

void			cget_next(t_client *client, int size_file, char *name)
{
  int			i;
  int			file;

  i = -1;
  if (strncmp("ERROR\n", client->buffer, 6) == 0)
    {
      printf("ERROR\n");
      return ;
    }
  printf("Receiving %s: %d octets\nSUCCESS\n", name, size_file);
  if ((file = open(name, O_CREAT | O_WRONLY | O_TRUNC,S_IRWXU | S_IRWXG | S_IRWXO)) == -1)
    perror(name);
  while (++i < (size_file / 1500))
    {
      bzero(client->server, 4096);
      read(client->s, client->server, 1500);
      write(file, client->server, 1500);
    }
  bzero(client->server, 4096);
  read(client->s, client->server, size_file % 1500);
  write(file, client->server, size_file % 1500);
  close(file);
}

void			cget(t_client *client)
{
  int			size_file;
  char			name[1000];

  if (strpbrk(client->buffer, "/"))
    {
      printf("ERROR\n");
      return ;
    }
  write(client->s, client->buffer, 4096);
  memmove(client->buffer, client->buffer + 4, strlen(client->buffer) - 4);
  memset(client->buffer + (strlen(client->buffer) - 4), 0,
	 strlen(client->buffer) - (strlen(client->buffer) - 4));
  client->buffer[strlen(client->buffer) - 1] = 0;
  write(client->s, client->buffer, 1000);
  bzero(name, 1000);
  strncat(name, client->buffer, strlen(client->buffer));
  bzero(client->buffer, 4096);
  read(client->s, client->buffer, 1000);
  size_file = atoi(client->buffer);
  cget_next(client, size_file, name);
}
