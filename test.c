#include "types.h"
#include "stat.h"
#include "user.h"


int
main(int argc, char **argv)
{
  int parentPty, childPty;
  if (argv[1][0] == '-') {
    parentPty = -atoi(argv[1]+1);
  } else {
    parentPty = atoi(argv[1]);
  }

  if (argv[2][0] == '-') {
    childPty = -atoi(argv[2]+1);
  } else {
    childPty = atoi(argv[2]);
  }

  printf(1, "%d, %d\n", parentPty, childPty);

  if (fork() == 0) {
    nice(childPty);
    int n, i, j, k, l, m, o, cnt = 0;
    float task;

    for(n = 0; n < 5; n++) {
        for (i = -(2<<29); i < 2<<29; i++) {
            for (j = -(2<<29); j < 2<<29; j++) {
                for (k = -(2<<29); k < 2<<29; k++) {
                  for (l = -(2<<29); l < 2<<29; l++) {
                    for (m = -(2<<29); m < 2<<29; m++) {
                      for (o = -(2<<29); o < 2<<29; o++) {
                        task = task + task * 2.34 * 6.899;
                      }
                    }
                  }
                    
                }
            }
        }
        cnt++;
        printf(1, "child finishes %d tasks\n", cnt);
    }
    exit();
  } 

  nice(parentPty);
  
  int n, i, j, k, l, m, o, cnt = 0;
  float task;
  for(n = 0; n < 5; n++){
    for (i = -(2<<29); i < 2<<29; i++) {
        for (j = -(2<<29); j < 2<<29; j++) {
                for (k = -(2<<29); k < 2<<29; k++) {
                  for (l = -(2<<29); l < 2<<29; l++) {
                    for (m = -(2<<29); m < 2<<29; m++) {
                      for (o = -(2<<29); o < 2<<29; o++) {
                        task = task + task * 2.34 * 6.899;
                      }
                    }
                  }
                }
            }
    }
    cnt++;
    printf(1, "parent finishes %d tasks\n", cnt);
  } 
  
  wait();

  exit();
}
