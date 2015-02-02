/*
** ls_pwd_cd.c for my_ftp in /home/nguye_1/rendu/PSU_2013_myftp
**
** Made by Alexandre NGUYEN
** Login   <nguye_1@epitech.net>
**
** Started on  Sun Apr 13 21:47:22 2014 Alexandre NGUYEN
** Last update Sun Apr 13 21:47:55 2014 Alexandre NGUYEN
*/

#include "server.h"

void			ls(t_server *server)
{
  DIR			*rep;
  struct dirent*	readfile;
  char			buffer[100000];

  rep = NULL;
  readfile = NULL;
  bzero(buffer, 100000);
  if ((rep = opendir(".")) == NULL)
    write(server->cs, "ERROR\n", 6);
  else
    {
      while ((readfile = readdir(rep)))
	{
	  strncat(buffer, readfile->d_name, strlen(readfile->d_name));
	  strncat(buffer, "\n", 1);
	}
      write(server->cs, buffer, strlen(buffer));
      closedir(rep);
    }
}

void			pwd(t_server *server)
{
  write(server->cs, getenv("PWD"), strlen(getenv("PWD")));
  write(server->cs, "\n", 1);
}

void			quit(t_server *server)
{
  server = server;
  exit(EXIT_SUCCESS);
}

char			check_cd(t_server *server)
{
  char			*pwd;

  pwd = getenv("PWD");
  if (!strncmp(server->buffer, "..\0", 3))
    {
      --server->deep;
      if (server->deep < 0)
	{
	  chdir(pwd);
	  ++server->deep;
	  write(server->cs, "ERROR\n", 6);
	  return (EXIT_FAILURE);
	}
    }
  else
    if (chdir(server->buffer) == -1)
      {
	write(server->cs, "No shuch file or directory\n", 27);
	perror("chdir");
      }
    else
      ++server->deep;
  return (EXIT_SUCCESS);
}

void			cd(t_server *server)
{
  char			tmp[1000];

  memmove(server->buffer, server->buffer + 3, strlen(server->buffer) - 3);
  memset(server->buffer + (strlen(server->buffer) - 3), 0,
	 strlen(server->buffer) - (strlen(server->buffer) - 3));
  if (!strlen(server->buffer) || strpbrk(server->buffer, "/"))
    {
      write(1, "ERROR\n", 6);
      write(server->cs, "ERROR\n", 6);
      return ;
    }
  if (check_cd(server) == EXIT_FAILURE)
    return ;
  else
    {
      bzero(tmp, 1000);
      strncat(tmp, "Going in ", 9);
      strncat(tmp, server->buffer, strlen(server->buffer));
      strncat(tmp, "\n", 1);
      write(server->cs, tmp, strlen(tmp));
    }
}
