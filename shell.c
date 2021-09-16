#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>

#include "list.h"

#define MAX_LINE 1024
#define MAX_TOKENS 10

int trocearCadena(char * str, char * tokens[])
{
    int i=1;

    if ((tokens[0]=strtok(str," \n\t"))==NULL)
        return 0;
    while ((tokens[i]=strtok(NULL," \n\t"))!=NULL)
        i++;

    return i;
}

int prueba(char *tokens[], int ntokens) {

    if(tokens[0] != NULL){
        if (strcmp(tokens[0], "-l") == 0){
            printf("m.llopez\n");
            printf("xoel.diaz\n");

        }else if (strcmp(tokens[0], "-n") == 0){
            printf("Miguel López López\n");
            printf("Xoel Díaz Préstamo\n");

        }
    }else {
        printf("Miguel López López: m.llopez\n");
        printf("Xoel Díaz Préstamo: xoel.diaz\n");

    }
    printf("%d\n", ntokens);
    for (int i=0;i<ntokens;i++){
        printf("%s\n",tokens[i]);

    }

    return 0;
}

int autores(char *tokens[], int ntokens) {
    if(tokens[0] != NULL){
        if (strcmp(tokens[0], "-l") == 0){
            printf("m.llopez\n");
            printf("xoel.diaz\n");

        }else if (strcmp(tokens[0], "-n") == 0){
            printf("Miguel López López\n");
            printf("Xoel Díaz Préstamo\n");

        }
    }else {
        printf("Miguel López López: m.llopez\n");
        printf("Xoel Díaz Préstamo: xoel.diaz\n");

    }

    return 0;
}

int pid(char *tokens[], int ntokens) {
    int pid, p_pid;

    pid = getpid();
    p_pid = getppid();
    
    if(tokens[0] != NULL){
        if (strcmp(tokens[0], "-p") == 0){
            printf("Pid del padre del shell: %d\n", p_pid);
        }
    }else {
        printf("Pid de shell: %d\n", pid);

    }
    return 0;
}

int carpeta(char *tokens[], int ntokens) {

    if(tokens[0] != NULL){

        chdir(tokens[0]);

    }else {
        char dir [MAX_LINE];
        getcwd(dir, sizeof(dir));
        printf("%s\n", dir);

    }
    return 0;
}

int salir(char *tokens[], int ntokens) {
    return 1;
}

struct cmd {
    char *cmd_name;
    int (*cmd_fun)(char *tokens[], int ntokens);
};

struct cmd cmds[] ={
    {"prueba", prueba},
    {"autores", autores},
    {"pid", pid},
    {"carpeta", carpeta},
    {"fin", salir},
    {"salir", salir},
    {"bye", salir},
    {NULL,  NULL}
};

int process(char *tokens[], int ntokens) {
    for(int i=0; cmds[i].cmd_name != NULL; i++) {
        if(strcmp(tokens[0], cmds[i].cmd_name) ==0) {
            return cmds[i].cmd_fun(tokens+1, ntokens-1);
        }
    }

    printf("comando no encontrado\n");
    return 0;
}

int main() {
    char *line;
    char *tokens[MAX_TOKENS];
    int ntokens;
    int end=0;

    while(!end) {
        line = readline("✦»> ");
        ntokens = trocearCadena(line, tokens);
        end = process(tokens, ntokens);
        free(line);
    }

}
