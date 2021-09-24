#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <time.h>

#define MAX_LINE 1024
#define MAX_TOKENS 10

//funciones de lectura y salida
int trocearCadena(char * str, char * tokens[]);
int process(char *tokens[], int ntokens);
int imprimirPrompt(char *line);
int leerEntrada(int end, char *line);

//comandos
int autores(char *tokens[], int ntokens);
int pid(char *tokens[], int ntokens);
int carpeta(char *tokens[], int ntokens);
int fecha(char *tokens[], int ntokens);
int infosis(char *tokens[], int ntokens);
int ayuda(char *tokens[], int ntokens);
int isNumber(char * string);
int hist(char *tokens[], int ntokens);
int comando(char *tokens[], int ntokens);
int salir(char *tokens[], int ntokens);

struct data{
    int num;
    char cmd [MAX_LINE];
};

struct cmd {
    char *cmd_name;
    int (*cmd_fun)(char *tokens[], int ntokens);
    char *cmd_help;
};

struct cmd cmds[] ={
    {"autores", autores, "[-n|-l]	Muestra los nombres y logins de los autores"},
    {"pid", pid, "[-p]	Muestra el pid del shell o de su proceso padre"},
    {"carpeta", carpeta, "[dir]	Cambia (o muestra) el directorio actual del shell"},
    {"fecha", fecha, "[-d|-h]	Muestra la fecha y o la hora actual"},
    {"infosis", infosis, "Muestra informacion de la maquina donde corre el shell"},
    {"hist", hist, "[-c|-N]	Muestra el historico de comandos, con -c lo borra"},
    {"ayuda", ayuda, "[cmd]	Muestra ayuda sobre los comandos"},
    {"comando", comando, "[N]	Repite el comando N (del historico)"}, {"fin", salir, "Termina la ejecucion del shell"},
    {"salir", salir, "Termina la ejecucion del shell"},
    {"bye", salir, "Termina la ejecucion del shell"},
    {NULL,  NULL, NULL}
};

