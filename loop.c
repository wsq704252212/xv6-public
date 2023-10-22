#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char **argv)
{
    int pty;
    if (argv[1][0] == '-') {
        pty = -atoi(argv[1]+1);
    } else {
        pty = atoi(argv[1]);
    }

    nice(pty);
    
    printf(1,"loop start, priotity: %d\n", pty);    
    for(;;);
   
    exit();
}