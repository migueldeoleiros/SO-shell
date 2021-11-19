// TITLE: Sistemas Operativos Práctica 2
// AUTHOR 1: Miguel López López        LOGIN 1: m.llopez
// AUTHOR 2: Xoel Díaz Préstamo        LOGIN 2: xoel.diaz
// GROUP: 2.1.4
// DATE: 19/11/2021

#include "headers.h"

char* deleteEnter(char *str){
    int length = strlen(str)-1;

    if(str[length] == '\n')
        str[length] = '\0';
    return str;
}

int trocearCadena(char * str, char * tokens[]){ int i=1;

    if ((tokens[0]=strtok(str," \n\t"))==NULL)
        return 0;
    while ((tokens[i]=strtok(NULL," \n\t"))!=NULL)
        i++;

    return i;
}

int process(char *tokens[], int ntokens, context *ctx) {
    for(int i=0; cmds[i].cmd_name != NULL; i++) {
        if(strcmp(tokens[0], cmds[i].cmd_name) ==0) {
            return cmds[i].cmd_fun(tokens+1, ntokens-1, ctx);
        }
    }

    printf(RED"Comando no encontrado\n"RESET);
    return 0;
}

int imprimirPrompt(char *line){
    //uncomment to see working directory in the prompt
    /*
    char path[MAX_LINE];
    getcwd(path, sizeof(path));
    printf(GREEN"%s ",path);
    */

    printf(BLUE"✦"GREEN"»> "RESET);
    fgets(line, MAX_LINE, stdin);

    return 0;
}

int leerEntrada(int end, char *line, context *ctx){
    char *tokens[MAX_TOKENS];
    int ntokens;

    if(empiezaPor("comando", line) != 0){
        struct histData *info = malloc(sizeof(struct histData));
        strcpy(info->cmd, deleteEnter(line));
        insert(&ctx->historial, info);
    }

    ntokens = trocearCadena(line, tokens);
    end = process(tokens, ntokens, ctx);

    return end;
}

int main() {
    char line [MAX_LINE];
    int end=0;

    context ctx;
    ctx.historial = init_list();
    ctx.malloc = init_list();
    ctx.mmap = init_list();
    ctx.shared = init_list();

    while(!end) {
        imprimirPrompt(line);
        if (line[0]!='\n'){
            end = leerEntrada(end, line, &ctx);
        }
    }
    freeList(&ctx.historial,free);
    freeList(&ctx.malloc,freeMem);
    freeList(&ctx.mmap,freeMmap);
    freeList(&ctx.shared,free);
}
