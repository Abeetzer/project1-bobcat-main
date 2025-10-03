#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//
int main(int argc, char *argv[]) {
  // we want to set up the basics which is taking a set of files from ./bobcat
  // and reading it
  //  argc and argv will allow us to do this
  // we just need to check if the files are valid or not and whether we have
  // permission to open them or not(this shouldn't be a problem tbh)

  // stdin to stdout pipeline
  char buf[1024];
  size_t count = sizeof(buf);
  if (argc == 1 || (argc > 1 && strcmp(argv[1], "-") == 0)) {
    printf("here");
    while (1) {
      size_t res = read(0, buf, count);

      if (res < 0) {
        printf("fatal error\n");
        exit(1);
      }
      if (res == 0) break;  // <-- when I hit crtl D
      // printf("%ld\n", res);//when does this = 0
      for (size_t i = 0; i < res; ++i) {
        // printf("%c", buf[i]);
        void *p = &buf[i];
        write(1, p, 1);
      }
    }
  }

  for (int i = 1; i < argc; i++) {  // argc is lets say 3, i = 1,2 --> 2
                                    // iterations = argc -1(files) good
    if (strcmp(argv[i], "-") == 0) {
      continue;
    }  // printf("yatta"); works for now
    int fd = open(argv[i], O_RDONLY, 0);
    if (fd < 0) {
      fprintf(stderr, "bobcat: %s: %s\n", argv[i], strerror(errno));
      continue;
    }  // works
    // int res = read(fd,buf,count);
    // printf("%d\n",res);//works
    while (1) {
      size_t res = read(fd, buf, count);

      if (res < 0) {
        printf("fatal error\n");
        exit(1);
      }
      if (res == 0) break;  // <-- when I hit crtl D
      // printf("%ld\n", res);//when does this = 0
      for (size_t i = 0; i < res; ++i) {
        // printf("%c", buf[i]);
        void *p = &buf[i];
        write(0, p, 1);
      }
    }
  }
  // printf("skibidi\n");//this env works fine time to plan
  return EXIT_SUCCESS;
}
