#include <stdio.h>
#include <stdio_ext.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <time.h>

#include "list.h"

#define MAX_LINE 1024
#define MAX_TOKENS 10

struct data{
    char cmd [MAX_LINE];
};

struct cmd{
    char *cmd_name;
    int (*cmd_fun)(char *tokens[], int ntokens);
    char *cmd_help;
};

typedef struct context{
    list historial;
}context;

//funciones de lectura y salida
int empiezaPor(const char *pre, const char *str);
int trocearCadena(char * str, char * tokens[]);
int process(char *tokens[], int ntokens);
int imprimirPrompt(char *line);
int leerEntrada(int end, char *line, context ctx);

//comandos
int autores(char *tokens[], int ntokens, context ctx);
int pid(char *tokens[], int ntokens, context ctx);
int carpeta(char *tokens[], int ntokens, context ctx);
int fecha(char *tokens[], int ntokens, context ctx);
int infosis(char *tokens[], int ntokens, context ctx);
int ayuda(char *tokens[], int ntokens, context ctx);
int hist(char *tokens[], int ntokens, context ctx);
int comando(char *tokens[], int ntokens, context ctx);
int salir(char *tokens[], int ntokens, context ctx);

//auxiliares
int isNumber(char * string);

