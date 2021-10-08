#include <stdio.h>
#include <stdio_ext.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
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

typedef struct context{
    list historial;
}context;

struct cmd{
    char *cmd_name;
    int (*cmd_fun)(char *tokens[], int ntokens, context *ctx);
    char *cmd_help;
};

extern struct cmd cmds[];

//funciones de lectura y salida
int empiezaPor(const char *pre, const char *str);
int trocearCadena(char * str, char * tokens[]);
int process(char *tokens[], int ntokens, context *ctx);
int imprimirPrompt(char *line);
int leerEntrada(int end, char *line, context *ctx);

//comandos
int autores(char *tokens[], int ntokens, context *ctx);
int pid(char *tokens[], int ntokens, context *ctx);
int carpeta(char *tokens[], int ntokens, context *ctx);
int fecha(char *tokens[], int ntokens, context *ctx);
int infosis(char *tokens[], int ntokens, context *ctx);
int ayuda(char *tokens[], int ntokens, context *ctx);
int hist(char *tokens[], int ntokens, context *ctx);
int comando(char *tokens[], int ntokens, context *ctx);
int crear(char *tokens[], int ntokens, context *ctx);
int borrar(char *tokens[], int ntokens, context *ctx);
int borrarrec(char *tokens[], int ntokens, context *ctx);
int listfich(char *tokens[], int ntokens, context *ctx);
int listdir(char *tokens[], int ntokens, context *ctx);
int salir(char *tokens[], int ntokens, context *ctx);

//auxiliares
int isNumber(char * string);
