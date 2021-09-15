#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>

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

int autores(char *tokens[], int ntokens) {
    printf("Miguel López López: m.llopez\n");
    printf("Xoel Díaz Préstamo: xoel.diaz\n");

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
    {"autores", autores},
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
        line = readline("> ");
        ntokens = trocearCadena(line, tokens);
        end = process(tokens, ntokens);
        free(line);
    }

}
