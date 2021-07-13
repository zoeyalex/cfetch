#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/utsname.h>
#include <sys/statvfs.h>
#include <stdbool.h>
#include "config.h"

int pkg_count()
{
  int files = 0;
  if (!strcmp(os_name, "Gentoo"))
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
  else if (!strcmp(os_name, "OpenBSD"))
    {
      DIR *dir = opendir("/var/db/pkg");
      struct dirent *file;
      while ((file = readdir(dir)) != NULL)
	if (file->d_type == DT_DIR && strcmp(file->d_name, ".") && strcmp(file->d_name, ".."))
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
  int esc_iter;
  char id[16] = "0123456789ABCDEF";
  printf("\x1b[0m");
  for(esc_iter = 0; esc_iter < 8; esc_iter++)
    printf("\x1b[30m\x1b[4%dm %c ", esc_iter, id[esc_iter]);
  printf("\x1b[0m\n");
  for(esc_iter = 0; esc_iter < 8; esc_iter++)
    printf("\x1b[30m\x1b[10%dm %c ", esc_iter, id[esc_iter+8]);
  printf("\x1b[0m\n");
}

void disk()
{
  char *path = "/"; /* path */
  struct statvfs buf; /* struct pointer */
  if (!statvfs(path, &buf)) /* return 0 on success -1 otherwise */
    {
      unsigned long bavail, blk_size;
      bavail = buf.f_bavail; /* unprivileged user avaialble */
      blk_size = buf.f_bsize; /* filesystem block size (4096)*/
      printf("\t/: %lu\n", bavail * blk_size >> 30);
    }
  else
    printf("disk usage n/a\n");
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

  if(show_term)
    {
      char *term = getenv("TERM");
      if(!term)
	term = "term n/a";
      printf("\t%s\n", term);
    }
  if(show_editor)
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
  if(show_disk)
    disk();
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
