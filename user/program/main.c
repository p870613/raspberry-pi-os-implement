#include <syscall.h>
#include <printf.h>
#include <types.h>
#include <time.h>

int main(int argc, char **argv) {
// printf("%d\n", argc);
 // printf("Argv Test, pid %d\n", getpid());
 // i
  //for (int i = 0; i < argc; ++i) {
  //  printf("%s\n", argv[i]);
  //}
//  char *fork_argv[] = {"fork_test", 0};
//  delay(2);
//  exec("fork_test", fork_argv);
  return 0;
}
/*
int main(void) {
    printf("Fork Test, pid %d\n", getpid());
    int cnt = 1;
    int ret = 0;
    if ((ret = fork()) == 0) { // child
      printf("pid: %d, cnt: %d, ptr: %p\n", getpid(), cnt, &cnt);
      ++cnt;
      fork();
      while (cnt < 5) {
          printf("pid: %d, cnt: %d, ptr: %p\n", getpid(), cnt, &cnt);
          delay(3);
          ++cnt;
      }
    } else {
        printf("parent here, pid %d, child %d\n", getpid(), ret);
    }
  return 0;
}
*/
