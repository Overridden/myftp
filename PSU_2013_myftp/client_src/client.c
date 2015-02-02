/*
** server.c for ftp in /home/nguye_1/rendu/PSU_2013_myftp
**
** Made by Alexandre NGUYEN
** Login   <nguye_1@epitech.net>
**
** Started on  Tue Apr  8 15:58:58 2014 Alexandre NGUYEN
** Last update Sun Apr 13 23:16:46 2014 Alexandre NGUYEN
*/

#include "client.h"

void			throw_error(char *str)
{
  perror(str);
  exit(EXIT_FAILURE);
}

void			do_client(t_client *client)
{
  while (1)
    {
      write(1, "> ", 2);
      bzero(client->buffer, 4096);
      if (read(1, client->buffer, 4096) > 1)
	{
	  if (put_or_get(client))
	    {
	      write(client->s, client->buffer, strlen(client->buffer) - 1);
	      if (strncmp("quit\n", client->buffer, 5) == 0)
		{
		  close(client->s);
		  return ;
		}
	      read(client->s, client->server, 4096);
	      write(1, client->server, strlen(client->server));
	      if (strncmp(client->server, "ERROR\n", 6))
		printf("SUCCESS\n");
	      bzero(client->server, 4096);
	    }
	}
    }
  close(client->s);
}

int			main(int ac, char **av)
{
  t_client		client;

  if (ac < 3)
    throw_error("Usage: ./client [IP_ADRESS] [PORT]\n");
  client.port = atoi(av[2]);
  client.s = socket(AF_INET, SOCK_STREAM, 0);
  if (client.s == -1)
    throw_error("Trouble to open the socket\n");
  client.sin.sin_family = AF_INET;
  client.sin.sin_port = htons(client.port);
  client.sin.sin_addr.s_addr = inet_addr(av[1]);
  write(1, "connecting...\n", 14);
  client.error = connect(client.s, (const struct sockaddr *)&client.sin, sizeof(client.sin));
  if (client.error == -1)
    {
      throw_error("Can not connect to the server.\nClosing the socket...\n");
      close(client.s);
      exit(EXIT_FAILURE);
    }
  do_client(&client);
  return (EXIT_SUCCESS);
}
