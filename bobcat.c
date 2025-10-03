#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void stdin_print(char *buf, size_t count) {
  while (1) {
    size_t res = read(0, buf, count);

    if (res < 0) {
      warn("stdin");  // use warn
      exit(1);
    }
    if (res == 0) break;  // <-- when I hit crtl D
    // printf("%ld\n", res);//when does this = 0
    //for (size_t i = 0; i < res; ++i) {
      // printf("%c", buf[i]);
      //void *p = &buf[i];
      //write(1, p, 1);
    //}
    size_t total_written = 0;
    while (total_written < res) {
      ssize_t written = write(1, buf + total_written, res - total_written);
      if (written < 0) {
          warn("stdout");
          exit(1);
      }
      total_written += written;
  }

  }
}

int main(int argc, char *argv[]) {
  // we want to set up the basics which is taking a set of files from ./bobcat
  // and reading it
  //  argc and argv will allow us to do this
  // we just need to check if the files are valid or not and whether we have
  // permission to open them or not(this shouldn't be a problem tbh)

  // stdin to stdout pipeline
  // printf("%d\n",argc);
  char buf[4096];
  size_t count = sizeof(buf);
  int exit_status = 0;
  // case 1 and 2 now Don't mix
  if (argc == 1 || (argc == 2 && strcmp(argv[1], "-") == 0)) {
    stdin_print(buf, count);
    return 0;
  }

  for (int i = 1; i < argc; i++) {  // argc is lets say 3, i = 1,2 --> 2
                                    // iterations = argc -1(files) good
    if (strcmp(argv[i], "-") == 0) {
      // printf("%s", argv[i]);
      stdin_print(buf, count);
      continue;
    }  // printf("yatta"); works for now

    int fd = open(argv[i], O_RDONLY, 0);  // file descriptor

    if (fd < 0) {
      warn("%s", argv[i]);
      exit_status = 1;
      continue;
    }  // works
    // int res = read(fd,buf,count);
    // printf("%d\n",res);//works
    while (1) {
      size_t res = read(fd, buf, count);

      if (res < 0) {
        warn("%s", argv[i]);
        exit(1);
      }
      if (res == 0) break;  // <-- when I hit crtl D
      // printf("%ld\n", res);//when does this = 0
      size_t total_written = 0;
      while (total_written < res) {
        ssize_t written = write(1, buf + total_written, res - total_written);
        if (written < 0) {
            warn("stdout");
            exit(1);
        }
        total_written += written;
      }
    }
  }
  // printf("skibidi\n");//this env works fine time to plan
  return exit_status;
}
