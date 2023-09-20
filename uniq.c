#include "types.h"
#include "stat.h"
#include "user.h"

//tode
int maxNum = 1024;
char buf[1024];

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

int splitLines(char* buf, char** lines) {
    int i = 0, lineCnt = 0, letterCnt = 0;

    lines[0] = malloc(maxNum * sizeof(char));
    for(i=0; i<strlen(buf); i++) {
        if (buf[i] == '\n' || buf[i] == '\r') {
            lines[lineCnt][letterCnt] = '\0';
            lineCnt++;
            letterCnt = 0;
            lines[lineCnt] = malloc(maxNum* sizeof(char));
            continue;
        }
        lines[lineCnt][letterCnt] = buf[i];
        letterCnt++;
    }
    lines[lineCnt][letterCnt] = '\0';
    return lineCnt+1;
}

void uniq(int fd, struct Options o) {
    int i, n, cnt = 0;
    char *uniqBuf = malloc(maxNum * sizeof(char));

    while((n = read(fd, buf, sizeof(buf))) > 0) {
        char **lines = malloc(maxNum * sizeof(char*));
        int lineCnt = splitLines(buf, lines);
        if (lineCnt == 0){
            exit();
        }
        
        strcpy(uniqBuf, lines[0]);
        cnt = 1;
        for (i = 1; i < lineCnt; i++) {
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
    printfByOption(o.printOption, uniqBuf, cnt);
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
