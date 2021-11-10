#include <stdio.h>
#include <stdio_ext.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <time.h>
#include <grp.h>
#include <pwd.h>
#include <errno.h>

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

struct histData{ //data for history list
    char cmd[MAX_LINE];
};

struct memData{ //data for allocated memory list
    int tamano_bloque;
    long* direccion_bloque;
    struct tm* time;
    int tipo_reserva;
    int aux;
    char file_name[MAX_LINE];
};

typedef struct context{  //variables que se les pasan a las funciones
    list historial;
    list memory;
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

/*Input/Output functions*/
//Deletes last char of a string if it is \n
char* deleteEnter(char* str);
//Checks if a string (str) contains another string (pre)
int empiezaPor(const char *pre, const char *str);
//Splits onen string (by it's blank spaces) and adds it into tokens
int trocearCadena(char * str, char * tokens[]);
//Process introduced command (tokens) and executes appropriated function
int process(char *tokens[], int ntokens, context *ctx);
//Prints shell's prompt
int imprimirPrompt(char *line);
//Reads text input from user, process it and adds it to our list
int leerEntrada(int end, char *line, context *ctx);

/*COMMANDS*/
//Returns 1 to end shell's execution
int salir(char *tokens[], int ntokens, context *ctx);
//Prints authors' logins/names
int autores(char *tokens[], int ntokens, context *ctx);
//Shows shell's pid or parents' pid
int pid(char *tokens[], int ntokens, context *ctx);
//Changes or shows actual directory
int carpeta(char *tokens[], int ntokens, context *ctx);
//Shows date or actual time
int fecha(char *tokens[], int ntokens, context *ctx);
//Shows info about the device where the shell is running
int infosis(char *tokens[], int ntokens, context *ctx);
//Shows help about the other commands
int ayuda(char *tokens[], int ntokens, context *ctx);
//Prints commands' historic or deletes it
int hist(char *tokens[], int ntokens, context *ctx);
//Executes again 'N' command from hist
int comando(char *tokens[], int ntokens, context *ctx);
//Creates one file or directory
int crear(char *tokens[], int ntokens, context *ctx);
//Removes files or empty directories
int borrar(char *tokens[], int ntokens, context *ctx);
//Removes files or non empty directories
int borrarrec(char *tokens[], int ntokens, context *ctx);
//Lists files
int listfich(char *tokens[], int ntokens, context *ctx);
//lists files contained in directories
int listdir(char *tokens[], int ntokens, context *ctx);
//asigna(o desasigna) memoria en el programa
int mallocUs(char *tokens[], int ntokens, context *ctx);
//mapea(o desmapea) ficheros en el espacio de direcciones del proceso
int mmapUs(char *tokens[], int ntokens, context *ctx);
//asigna(o desasigna) memoria compartida en el programa
int shared(char *tokens[], int ntokens, context *ctx);
//Desasigna un bloque de memoria asignado con malloc, shared o mmap
int dealloc(char *tokens[], int ntokens, context *ctx);
//Muestra muestra detalles de la memoria del proceso
int memoria(char *tokens[], int ntokens, context *ctx);
//Vuelca en pantallas los contenidos (cont bytes) de la posicion de memoria addr
int volcarmem(char *tokens[], int ntokens, context *ctx);
//Llena la memoria a partir de addr con byte
int llenarmem(char *tokens[], int ntokens, context *ctx);
//Invoca a la funcion recursiva n veces
int recursiva(char *tokens[], int ntokens, context *ctx);
//leer o escribir un fichero
int e_s(char *tokens[], int ntokens, context *ctx);


/*auxiliary*/
//Checks if a string is a number
int isNumber(char * string);
//Removes one directory
int borrarDir(char *dir);
//returns info from mode_t in a human readable format
char letraTF (mode_t m);
//concatenates info from letraTF
char * convierteModo (mode_t m);
//Shows one file's size
off_t sizeFich(char *file);
//shows info about a file given parameters
int printFileInfo(char *file, struct listOptions *opts);
//return 1 if path is a directory
int isDir(const char *path);
//shows info about files in a Directory
int isDirEmpty(char *dirname);
//checks if a directory is empty
int printDirInfo(char *dir, struct listOptions *opts);
//Shows subdirectories of a given dir
int listSubDir(char *dir, struct listOptions *opts);
//prints info about memoly allocated from the mempry list
void printMem(context ctx, int malloc, int mmap, int shared);
//maps a file into memoery and records it in the memlist
void * MmapFichero (char * fichero, int protection, context *ctx);
void * ObtenerMemoriaShmget (key_t clave, size_t tam);
void SharedCreate (char *arg[], context *ctx); 
void SharedDelkey (char *args[]); 
void dopmap (void); 
ssize_t LeerFichero (char *fich, void *p, ssize_t n);
