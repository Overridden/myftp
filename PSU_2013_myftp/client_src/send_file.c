/*
** send_file.c<2> for myftp in /home/nguye_1/rendu/PSU_2013_myftp
**
** Made by Alexandre NGUYEN
** Login   <nguye_1@epitech.net>
**
** Started on  Sun Apr 13 21:57:55 2014 Alexandre NGUYEN
** Last update Sun Apr 13 23:18:46 2014 Alexandre NGUYEN
*/

#include "client.h"

void			csend_next(t_client *client, int size, int fd)
{
  int			i;

  i = -1;
  while (++i < (size / 1500))
    {
      bzero(client->server, 4096);
      read(fd, client->server, 1500);
      write(client->s, client->server, 1500);
    }
  bzero(client->server, 4096);
  read(fd, client->server, size % 1500);
  write(client->s, client->server, size % 1500);
}

void			csend(t_client *client, char *size, int fd)
{
  char			tmp[2000];
  int			size_file;

  bzero(tmp, 2000);
  size_file = atoi(size);
  printf("Sending %s: %d octects\nSUCCESS\n", client->buffer, size_file);
  strncat(tmp, size, strlen(size));
  strncat(tmp, "5KU3gG6i6Wg3huQv7hsS84g9Vc7P5FBe", 32);
  bzero(client->server, 2000);
  strncat(tmp, client->buffer, strlen(client->buffer));
  strncat(tmp, "eqbPv9Ae6NU2886ZJy4N873mVzC6zrHs", 32);
  write(client->s, tmp, strlen(tmp));
  csend_next(client, size_file, fd);
  bzero(client->server, 4096);
}

void			cput_next(t_client *client)
{
  FILE	*file;
  char	tmp[1000];
  int	i;

  write(client->s, "put", 3);
  bzero(tmp, 1000);
  strncat(tmp, "stat ", 5);
  strncat(tmp, client->buffer, strlen(client->buffer));
  file = popen(tmp, "r");
  bzero(client->server, 4096);
  fgets(client->server, 4096, file);
  fgets(client->server, 4096, file);
  pclose(file);
  memmove(client->server, client->server + 8, strlen(client->server) - 8);
  i = -1;
  while (client->server[++i] != ' ')
    ;
  memset(client->server + i, 0, strlen(client->server) - i);
}

void			cput(t_client *client)
{
  int	fd;

  if (strstr(client->buffer, "/"))
    {
      printf("ERROR\n");
      return ;
    }
  memmove(client->buffer, client->buffer + 4, strlen(client->buffer) - 4);
  memset(client->buffer + (strlen(client->buffer) - 4), 0,
	 strlen(client->buffer) - (strlen(client->buffer) - 4));
  client->buffer[strlen(client->buffer) - 1] = 0;
  if ((fd = open(client->buffer, O_RDONLY)) == -1)
    perror(client->buffer);
  else
    {
      cput_next(client);
      csend(client, client->server, fd);
    }
  close(fd);
}

char			put_or_get(t_client *client)
{
  if (!strncmp("put ", client->buffer, 4) || !strncmp("get ", client->buffer, 4))
    {
      if (!strncmp("put ", client->buffer, 4))
	cput(client);
      else
	cget(client);
      return (EXIT_SUCCESS);
    }
  return (1);
}
