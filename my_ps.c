#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int check_if_number(char *name) {
  int i;

  for (i = 0; name[i] != '\0'; i++) {
    if (!isdigit(name[i])) {
      return 0;
    }
  }

  return 1;
}

int main() {
  DIR *dir;
  FILE *fp;
  struct dirent *read_dir;
  int pid, fd;
  char *tty;
  char cmd[256], path[1024], s_time[256];
  unsigned long time_x, time_y;
  dir = opendir("/proc");

  if (!dir) {
    perror("The proc direcetory returned NULL");
    exit(1);
  }

  printf("%s \t%8s %s\n", "PID", "TIME", "CMD");

  while ((read_dir = readdir(dir)) != NULL) {

    if (check_if_number(read_dir->d_name)) {
      sprintf(path, "/proc/%s/stat", read_dir->d_name);
      fp = fopen(path, "r");

      if (fp) {
        fscanf(fp, "%d %s", &pid, cmd);
        cmd[strlen(cmd) - 1] = '\0';
      }

      for (pid = 0; pid < 11; pid++) {
        fscanf(fp, "%lu", &time_x);
      }

      fscanf(fp, "%lu", &time_y);
      time_x = (int)((double)(time_x + time_y) / sysconf(_SC_CLK_TCK));
      sprintf(s_time, "%02lu:%02lu:%02lu", (time_x / 3600) % 3600,
              (time_x / 60) % 60, time_x % 60);

      printf("%s \t%8s %s\n", read_dir->d_name, s_time, cmd + 1);
      fclose(fp);
    }
  }

  return 0;
}
