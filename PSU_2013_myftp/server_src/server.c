/*
** server.c for ftp in /home/nguye_1/rendu/PSU_2013_myftp
**
** Made by Alexandre NGUYEN
** Login   <nguye_1@epitech.net>
**
** Started on  Tue Apr  8 15:58:58 2014 Alexandre NGUYEN
** Last update Sun Apr 13 21:54:56 2014 Alexandre NGUYEN
*/

#include "server.h"

void			throw_error(char *str)
{
  perror(str);
  exit(EXIT_FAILURE);
}

void			do_client(t_server *server)
{
  int	i;
  void	(*func[6])(t_server *);

  func[0] = ls;
  func[1] = pwd;
  func[2] = quit;
  func[3] = cd;
  func[4] = get;
  func[5] = put;
  printf("Connection from %s\n", server->str);
  while (1336)
    {
      printf("Waiting for instructions...\n");
      i = -1;
      bzero(server->buffer, 4096);
      read(server->cs, server->buffer, 4096);
      printf("%s: %s\n", server->str, server->buffer);
      while (++i < 6 && strncmp(server->buffer, server->func_name[i],
				strlen(server->func_name[i])))
	;
      if (i < 6)
	func[i](server);
      else
	write(server->cs, "ERROR\n", 7);
    }
}

void			assign_func(t_server *server)
{
  server->func_name[0] = "ls";
  server->func_name[1] = "pwd";
  server->func_name[2] = "quit";
  server->func_name[3] = "cd ";
  server->func_name[4] = "put";
  server->func_name[5] = "get";
}

void			init(t_server *server)
{
  server->deep = 0;
  if ((server->s = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    throw_error("Trouble to open the socket.\n");
  server->sin.sin_family = AF_INET;
  server->sin.sin_port = htons(server->port);
  server->sin.sin_addr.s_addr = INADDR_ANY;
  if ((bind(server->s, (const struct sockaddr *)&server->sin,
	    sizeof(server->sin))) == -1)
    {
      close(server->s);
      throw_error("Error to bind the socket.\nClosing the socket...\n");
    }
  if ((listen(server->s, 100)) == -1)
    {
      close(server->s);
      throw_error("Error to listen to the socket.\nClosing the socket...\n");
    }
  assign_func(server);
}

int			main(int ac, char **av)
{
  t_server		server;

  if (ac < 2)
    throw_error("Usage: ./server [PORT]\n");
  server.port = atoi(av[1]);
  init(&server);
  while (42)
    {
      if ((server.cs = accept(server.s, (struct sockaddr *)&server.sin_client,
				 (socklen_t *)&server.client_len)) == -1)
      	{
	  write(server.cs, "ERROR\n", 6);
	  close(server.s);
	  throw_error("Error: accept connection.\nClosing the socket...\n");
	}
      if (fork() == 0)
	{
	  server.str = inet_ntoa(server.sin_client.sin_addr);
	  do_client(&server);
	  exit(EXIT_FAILURE);
	}
    }
  write(1, "\nClosing socket...\n", 19);
  close(server.s);
  return (EXIT_SUCCESS);
}
