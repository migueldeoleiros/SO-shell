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
#include <libgen.h>
#include <sys/utsname.h>
#include <time.h>
#include <grp.h>
#include <pwd.h>

#include "list.h"

#define MAX_LINE 1024
#define MAX_TOKENS 15

//colores
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m" 
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

struct data{ //objetos de la lista
    char cmd [MAX_LINE];
};

typedef struct context{  //variables que se les pasan a las funciones
    list historial;
}context;

struct cmd{  //lista de comandos y descripción
    char *cmd_name;
    int (*cmd_fun)(char *tokens[], int ntokens, context *ctx);
    char *cmd_help;
};
extern struct cmd cmds[];

struct listOptions{ //opciones de listado de archivos
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
//comprueba si un string es un numero o no
int isNumber(char * string); 
//borra un directorio y sus ficheros recursivamente
int borrarDir(char *dir); 
//cambia los mode_t a formato humano
char letraTF (mode_t m);
//concatena la informacion de letraTF 
char * convierteModo (mode_t m);
//retorna el tamaño de un fichero
off_t sizeFich(char *file); 
//muestra información sobre un fichero dados unos parametros
int printFileInfo(char *file, struct listOptions *opts); 
//retorna 1 si path es un directorio
int isDir(const char *path); 
//muestra informacion sobre los archivos en un directorio
int printDirInfo(char *dir, struct listOptions *opts); 
//busca los subdirectorios y usa la funcion prinDirInfo en ellos 
int listSubDir(char *dir, struct listOptions *opts); 
