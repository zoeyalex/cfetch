#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/utsname.h>
#include <stdbool.h>
#include "config.h"

int pkg_count()
{
  int files = 0;
  if (!strcmp(os, "Gentoo"))
    {
      DIR *dir = opendir("/var/db/pkg");
      struct dirent *file, *file2;
      while ((file = readdir(dir)) != NULL)
	if (file->d_type == DT_DIR && strcmp(file->d_name, ".") && strcmp(file->d_name, ".."))
	  {
	    char dp[1024];
	    DIR *ddir;
	    strcpy(dp, "/var/db/pkg/");
	    strcat(dp, file->d_name);
	    ddir = opendir(dp);
	    while ((file2 = readdir(ddir)) != NULL)
	      if (file2->d_type == DT_DIR && strcmp(file2->d_name, ".") && strcmp(file2->d_name, ".."))
		files++;
	    closedir(ddir);
	  }
      closedir(dir);
    }
  else if (!strcmp(os, "OpenBSD"))
    {
      DIR *dir = opendir("/var/db/pkg");
      struct dirent *file;
      while ((file = readdir(dir)) != NULL)
	if (file->d_type == DT_REG && strcmp(file->d_name, ".") && strcmp(file->d_name, ".."))
	  files ++;
      closedir(dir);
    }
  return files;
}

void help()
{
  printf("usage: cfetch [-p]\n");
}

void palette()
{
  int esc_i;
  char id[16] = "0123456789ABCDEF";
  printf("\x1b[0m");
  for(esc_i = 0; esc_i < 8; esc_i++)
    printf("\x1b[30m\x1b[4%dm %c ",esc_i, id[esc_i]);
  printf("\x1b[0m\n");
  for(esc_i = 0; esc_i < 8; esc_i++)
    printf("\x1b[30m\x1b[10%dm %c ", esc_i, id[esc_i+8]);
  printf("\x1b[0m\n");
}

void fetch()
{

  char*shell = getenv("SHELL");
  if(!shell)
    shell = "shell n/a";
  struct utsname uname_data;
  uname(&uname_data);
  printf("(\\ /)\t%s\n", uname_data.release);
  printf("( . .)\tpkg %d\n", pkg_count());
  printf("c(\")(\")\t%s\n", shell);

  if(tm)
    {
      char *term = getenv("TERM");
      if(!term)
	term = "term n/a";
      printf("\t%s\n", term);
    }
  if(ed)
    {
      char *editor = getenv("EDITOR");
      if(!editor)
	{
	  editor = getenv("VISUAL");
	  if(!editor)
	    editor = "editor n/a";
	}
      printf("\t%s\n", editor);
    }
}

int main(int argc, char *argv[])
{
  if(argc == 1)
    fetch();
  else if(argc == 2 && !strcmp(argv[1], "-p"))
    {
      fetch();
      palette();
    }
  else
    help();
}
