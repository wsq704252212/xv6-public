#include "types.h"
#include "stat.h"
#include "user.h"

int maxNum = 4;
char buf[3];

struct Options {
    // 0 means standard output, 
    // 1 means standard output with count, 
    // 2 means only output duplicate lines
    int printOption;
    // 0 means standard cmp
    // 1 means ignore lower and upper case when cmp
    int cmpOption;  
};

void printfByOption(int po, char* str, int cnt) {
    if (po == 0) {
        printf(1, "%s\n", str);
    } else if (po == 1) {
        printf(1, "%d %s\n", cnt, str);
    } else {
        if (cnt > 1) {
            printf(1, "%s\n", str);
        }
    }
}

int cmpLetterIgnoreCase(char a, char b) {
    if(a <='z' && a >= 'a') a = a + 'A' - 'a';
    if(b <='z' && b >= 'a') b = b + 'A' - 'a';

    return a == b;
}

int strcmpIgnoreCase(const char *p, const char *q)
{
    while(*p && cmpLetterIgnoreCase(*p, *q))
        p++, q++;
    return (uchar)*p - (uchar)*q;
}

int cmpByOption(int co, char* a, char* b) {
    if (co == 0) {
        return strcmp(a, b);
    } else {
        return strcmpIgnoreCase(a, b);
    }
}

// return number of the lines in buf.
// the last line is not counted, because we don't know if it is end.
// Instead, the last line will be save in the char* rest
int splitLines(char* buf, char** lines, char** rest) {
    int i = 0, lineCnt = 0, letterCnt = 0;

    // line[0] = rest + line[0] in buf
    lines[0] = malloc((maxNum + strlen(*rest) + 1) * sizeof(char));
    strcpy(lines[0], *rest);
    letterCnt = strlen(*rest);

    // split lines in bufs
    for(i=0; i<strlen(buf); i++) {
        if (buf[i] == '\n' || buf[i] == '\r') {
            lines[lineCnt][letterCnt] = '\0';
            lineCnt++;
            letterCnt = 0;
            lines[lineCnt] = malloc(maxNum * sizeof(char));
            continue;
        }
        lines[lineCnt][letterCnt] = buf[i];
        letterCnt++;
    }
    lines[lineCnt][letterCnt] = '\0';

    // set last line to rest
    free(*rest);
    *rest = malloc((strlen(lines[lineCnt]) + 1) * sizeof(char));
    strcpy(*rest, lines[lineCnt]);

    return lineCnt;
}

void uniq(int fd, struct Options o) {
    int i, n, cnt = 0;
    // uniqBuf records the first uniq line
    char *uniqBuf = "";
    int uniqBufInUse = 0;
    // rest records the last line in the buf
    char *rest = malloc(1 * sizeof(char));
    char **lines = malloc(maxNum * sizeof(char*));

    while((n = read(fd, buf, sizeof(buf) - 1)) > 0) {
        buf[n] = '\0';

        int lineCnt = splitLines(buf, lines, &rest);

        // if uniqBuf is not used, we set line[0] to uniqBuf, we compare from line[1].ßß
        // if uniqBuf is used, it means it saves the uniqLine in the last iteration, we compare from line[0] this iteration.
        int lineIndexToCmp = 1 - uniqBufInUse;
        if (uniqBufInUse == 0 && lineCnt > 0) {
            uniqBuf = malloc((strlen(lines[0]) + 1) * sizeof(char));
            strcpy(uniqBuf, lines[0]);
            cnt = 1;
            uniqBufInUse = 1;
        }

        // compare and count unique lines
        for (i = lineIndexToCmp; i < lineCnt; i++) {
            if(cmpByOption(o.cmpOption, lines[i], uniqBuf) == 0) {
                cnt++;
                continue;
            }

            printfByOption(o.printOption, uniqBuf, cnt);
            strcpy(uniqBuf, lines[i]);
            cnt = 1;
        }
    }

    // Print the last line
    if(cmpByOption(o.cmpOption, rest, uniqBuf) == 0) {
        cnt++;
        printfByOption(o.printOption, uniqBuf, cnt);
    } else {
        printfByOption(o.printOption, uniqBuf, cnt);
        printfByOption(o.printOption, rest, 1);
    }
}



int main(int argc, char *argv[]){
    int i, j, fd;  
    char *path = "";
    
    struct Options o = {
        printOption: 0,
        cmpOption:0
    };

    // Parse command line
    for(i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            path = argv[i];
            continue;
        }

        for(j = 1; j < strlen(argv[i]); j++) {
            switch(argv[i][j]) {
                case 'c':
                    o.printOption = 1; 
                    break;
                case 'd':
                    o.printOption = 2;
                    break;
                case 'i':
                    o.cmpOption = 1;
                    break;
                default:
                    printf(1, "Option is unknown.\n");
                    exit();
            }
        }
    }

    // Do uniq
    if(strlen(path) == 0){
        uniq(0, o);
        exit();
    }

    if((fd = open(path, 0)) < 0){
        printf(1, "uniq: cannot open %s\n", path);
        exit();
    }  
    uniq(fd, o);
    close(fd);
    exit();
}
