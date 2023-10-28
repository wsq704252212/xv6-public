#include "types.h"
#include "stat.h"
#include "user.h"


void heavyTask() {
    int n, i, j, k, l, m, o;
    float task=1.23;
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
    }
}

int
main(int argc, char **argv)
{
  
  int firstTic, secondTic;
  firstTic = atoi(argv[1]);
  secondTic = atoi(argv[2]);
  
  //printf(1, "%d, %d\n", firstTic, secondTic);

  int pid1, pid2;

  pid1 = fork();
  if (pid1 == 0) {
    sleep(1);
    int cnt = 0;
    for(;;) {
        heavyTask();
        cnt++;
        printf(1, "first child finishes %d tasks\n", cnt);
    }
    exit();
  } 

  pid2 = fork();
  if (pid2 == 0) {
    sleep(1);
    int cnt = 0;
    for(;;) {
        heavyTask();
        cnt++;
        printf(1, "second child finishes %d tasks\n", cnt);
    }
    exit();
  } 

  setTickets(pid1, firstTic);
  setTickets(pid2, secondTic);

  // children never stop
  // exit to return shell

  exit();
}