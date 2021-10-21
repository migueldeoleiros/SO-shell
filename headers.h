#include <stdio.h>
#include <stdio_ext.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <time.h>
#include <grp.h>
#include <pwd.h>

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

struct listOptions{
    int lng;
    int acc;
    int link;
    int hid;
    int reca;
    int recb;
};

/*funciones para lectura y salida*/
//borra el ultimo caracter de un string si este es un enter (\n)
char* deleteEnter(char* str);
//comprueba si un string (str) contiene al principio otro string (pre)
int empiezaPor(const char *pre, const char *str);
//trocea un string dado por espacios y lo mete en un array (tokens)
int trocearCadena(char * str, char * tokens[]);
//procesa el comando introducido en el array (tokens) y ejecuta la funcion adecuada
int process(char *tokens[], int ntokens, context *ctx);
//imprime el prompt del shell
int imprimirPrompt(char *line);
//lee la entrada de texto del usuario, la procesa y la incorpora a la lista
int leerEntrada(int end, char *line, context *ctx);

/*comandos*/
//muestra nombres de logins de autores
int autores(char *tokens[], int ntokens, context *ctx); 
//muestra el pid del shell o su proceso padre
int pid(char *tokens[], int ntokens, context *ctx);
//cambia o mustra el directorio del actual del shell
int carpeta(char *tokens[], int ntokens, context *ctx); 
//muestra la fecha y o la hora actual
int fecha(char *tokens[], int ntokens, context *ctx);
//muestra informacion de la maquina donde corre el shell
int infosis(char *tokens[], int ntokens, context *ctx);
//muestra el historico de comandos o lo borra
int ayuda(char *tokens[], int ntokens, context *ctx);
//muestra ayuda sobre los comandos
int hist(char *tokens[], int ntokens, context *ctx);
//repite el comando N del historico
int comando(char *tokens[], int ntokens, context *ctx);
//crea un fichero o directorio 
int crear(char *tokens[], int ntokens, context *ctx);
//borra ficheros o directorios vacios
int borrar(char *tokens[], int ntokens, context *ctx);
//borra ficheros o directorios no vacios
int borrarrec(char *tokens[], int ntokens, context *ctx);
//lista ficheros
int listfich(char *tokens[], int ntokens, context *ctx);
//lista ficheros contenidos en directorios
int listdir(char *tokens[], int ntokens, context *ctx);
//retorna 1 para terminar la ejecucion del shell
int salir(char *tokens[], int ntokens, context *ctx);

/*auxiliares*/
int isNumber(char * string); //comprueba si un string es un numero o no
