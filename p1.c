// TITLE: Sistemas Operativos Práctica 1 
// AUTHOR 1: Miguel López López        LOGIN 1: m.llopez 
// AUTHOR 2: Xoel Díaz Préstamo        LOGIN 2: xoel.diaz                  
// GROUP: 2.1.4 
// DATE: 22/10/2021 

#include "headers.h"

int empiezaPor(const char *pre, const char *str){
    if(strncmp(pre, str, strlen(pre)) == 0) return 0;
    return 1;
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

    printf("comando no encontrado\n");
    return 0;
}

int imprimirPrompt(char *line){
    printf("✦»> ");
    fgets(line, MAX_LINE, stdin);

    return 0;
}

int leerEntrada(int end, char *line, context *ctx){
    char *tokens[MAX_TOKENS];
    int ntokens;

    if(empiezaPor("comando", line) != 0){
        struct data *info = malloc(sizeof(struct data));
        strcpy(info->cmd, line);
        insert(&ctx->historial , info);
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

    while(!end) {
        imprimirPrompt(line);
        if (line[0]!='\n'){
            end = leerEntrada(end, line, &ctx);
        }
    }
}
