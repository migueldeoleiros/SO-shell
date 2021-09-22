#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <time.h>

#include "list.h"

#define MAX_LINE 1024
#define MAX_TOKENS 10

list lista; //crear lista
int countHist = 0; //crear contador para historial

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

int fecha(char *tokens[], int ntokens) {
    struct tm* fecha;
    time_t t;
    t = time(NULL);
    fecha = localtime(&t);

    if(tokens[0] != NULL){
        if (strcmp(tokens[0], "-d") == 0){
            // DD/MM/YYYY
            printf("%d/%d/%d\n", fecha->tm_mday, fecha->tm_mon+1, 1900+fecha->tm_year);

        }else if (strcmp(tokens[0], "-h") == 0){
            // hh:mm:ss
            printf("%d:%d:%d\n", fecha->tm_hour, fecha->tm_min, fecha->tm_sec);

        }
    }else {
        printf("%d:%d:%d\n", fecha->tm_hour, fecha->tm_min, fecha->tm_sec);
        printf("%d/%d/%d\n", fecha->tm_mday, fecha->tm_mon+1, 1900+fecha->tm_year);

    }

    return 0;
}

int infosis(char *tokens[], int ntokens) {
    struct utsname sys;

    if (!uname(&sys)){
       printf("%s (%s), OS: %s-%s-%s \n",
               sys.nodename, sys.machine, sys.sysname, sys.release, sys.version);
    }

    return 0;
}

int ayuda(char *tokens[], int ntokens) {
    if(tokens[0] != NULL){
        

    }else {

    }

    return 0;
}

struct data{
    int num;
    char cmd [MAX_LINE];
};

int isNumber(char * string)
{
   for(int i = 0; i < strlen( string ); i ++)
   {
      //ASCII value of 0 = 48, 9 = 57. So if value is outside of numeric range then fail
      //Checking for negative sign "-" could be added: ASCII value 45.
      if (string[i] < 48 || string[i] > 57)
         return 0;
   }

   return 1;
}

int hist(char *tokens[], int ntokens) {

    if(tokens[0] != NULL){
        if (strcmp(tokens[0], "-c") == 0){
            borrar(lista);
            lista = init_list();
            countHist = 0;
        }else if (isNumber(tokens[0])){
            int num = abs(atoi(tokens[0]));
            printf("%d\n", num);
            //cambiar esto para que imprima solo el numero 
            for(pos p=first(lista); !end(lista, p); p=next(lista, p)) {
                struct data *d = get(lista, p);
                printf("%d %s\n", d->num, d->cmd);
        }
        }
        

    }else {
        for(pos p=first(lista); !end(lista, p); p=next(lista, p)) {
            struct data *d = get(lista, p);
            printf("%d %s\n", d->num, d->cmd);
        }

    }

    return 0;
}

int salir(char *tokens[], int ntokens) {
    return 1;
}

struct cmd {
    char *cmd_name;
    int (*cmd_fun)(char *tokens[], int ntokens);
    char cmd_help [MAX_LINE];
};

struct cmd cmds[] ={
    {"autores", autores, "[-n|-l]	Muestra los nombres y logins de los autores"},
    {"pid", pid, "[-p]	Muestra el pid del shell o de su proceso padre"},
    {"carpeta", carpeta, "[dir]	Cambia (o muestra) el directorio actual del shell"},
    {"fecha", fecha, "[-d|-h]	Muestra la fecha y o la hora actual"},
    {"infosis", infosis, "Muestra informacion de la maquina donde corre el shell"},
    {"fin", salir, "Termina la ejecucion del shell"},
    {"hist", hist, "Termina la ejecucion del shell"},
    {"salir", salir, "Termina la ejecucion del shell"},
    {"bye", salir, "Termina la ejecucion del shell"},
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

int imprimirPrompt(char *line){
    printf("✦»> ");
    fgets(line, MAX_LINE, stdin);

    return 0;
}

int leerEntrada(int end, char *line){
    char *tokens[MAX_TOKENS];
    int ntokens;

    ntokens = trocearCadena(line, tokens);
    end = process(tokens, ntokens);
    
    struct data *d = malloc(sizeof(struct data));
    d->num = countHist;
    strcpy(d->cmd, line);
    insert(&lista , d);

    return end;
}

int main() {
    char line [MAX_LINE];
    int end=0;

    lista = init_list();

    while(!end) {
        imprimirPrompt(line);
        end = leerEntrada(end, line);
        countHist++;
    }

}
