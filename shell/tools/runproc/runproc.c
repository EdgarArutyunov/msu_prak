#include "../list/list.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>

/*void parse_list(char *** argv, plist p, int * fread, int * fwrite, int * bmode) {
    while (p) {
        if (strcmp(p->key, ">") || strcmp(p->key, ">>")) {
            *fwrite = open("")
        }
    }
}*/

plist parse_list(plist p, int * fr, int * fw, int * bmode) {
    *fr = *fw = -1;
    *bmode = 0;
    plist res = NULL;
    while (p) {
        if (p->type == BASH_MODE) {
            if (!strcmp(p->key, "&")) {
                *bmode = 1;
            } else if (!strcmp(p->key, ">>")) {
                if (*fw != -1) {
                    close(*fw);
                }
                *fw = open(p->next->key, O_WRONLY | O_APPEND | O_CREAT, S_IWRITE | S_IREAD);
                p = p->next;
            } else if (!strcmp(p->key, ">")) {
                if (*fw != -1) {
                    close(*fw);
                }
                *fw = open(p->next->key, O_WRONLY | O_CREAT | O_TRUNC, S_IWRITE | S_IREAD);
                p = p->next;
            } else if (!strcmp(p->key, "<")) {
                if (*fr != -1) {
                    close(*fr);
                }
                *fr = open(p->next->key, O_RDONLY);
                p = p->next;
            }
            
        } else {
            res = add_word(res, p->key, p->type);
        }
        p = p->next;
    }
    return res;
}

void runproc(plist p) { //plist ./m >> 2 << 2 > 2 < 3 > 4 &
    if (!p) {
        return ;
    }
    int fr = -1, fw = -1, bm = 0;
    plist res = parse_list(p, &fr, &fw, &bm);
    char ** argv = ltoa(res);
    if (!strcmp(argv[0], "cd")) {
        if (argv[1]) {
            if (argv[2]) {
                fprintf(stderr, "Djarvis Error:: Too many arguments for cd\n");
                return ;
            }
            chdir(argv[1]);
        }
        delete_list(res);
        free(argv);
        return ;
    }

    int pid = fork();
    if (pid < 0) {
        fprintf(stderr, "can't fork\n");
        delete_list(res);
        free(argv);
        if (fr != -1) {
            close(fr);
        }
        if (fw != -1) {
            close(fw);
        }
        return ;
    } else if (pid == 0) {
        if (fr != -1) {
            dup2(fr, 0);
            close(fr);
        }
        if (fw != -1) {
            dup2(fw, 1);
            close(fw);
        }
        if (bm) {
            signal(SIGINT,SIG_IGN);
            if (fr != -1) {
                fprintf(stderr, "WAAT\n");
                exit(1);
            }
            fr = open("/dev/null", O_RDONLY);
            dup2(fr, 0);
        }
        execvp(argv[0], argv);
        fprintf(stderr, "%s:: Error\n", argv[0]);
        exit(1);
    } else {
        if (!bm) {
            wait(NULL);
        }
        if (fr != -1) {
            close(fr);
        }
        if (fw != -1) {
            close(fw);
        }
        free(argv);
        delete_list(res);
    }
}