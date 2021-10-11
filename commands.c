#include "headers.h"

struct cmd cmds[] ={
    {"autores", autores, "[-n|-l]	Muestra los nombres y logins de los autores"},
    {"pid", pid, "[-p]	Muestra el pid del shell o de su proceso padre"},
    {"carpeta", carpeta, "[dir]	Cambia (o muestra) el directorio actual del shell"},
    {"fecha", fecha, "[-d|-h]	Muestra la fecha y o la hora actual"},
    {"infosis", infosis, "Muestra informacion de la maquina donde corre el shell"},
    {"hist", hist, "[-c|-N]	Muestra el historico de comandos, con -c lo borra"},
    {"ayuda", ayuda, "[cmd]	Muestra ayuda sobre los comandos"},
    {"comando", comando, "[N]	Repite el comando N (del historico)"},
    {"crear", crear, "[-f] [name]	Crea un fichero o directorio"},
    {"borrar", borrar, "[name1 name2 ..]	Borra ficheros o directorios vacios"},
    {"borrarrec", borrarrec, "[name1 name2 ..]	Borra ficheros o directorios no vacios"},
    {"listfich", listfich, "[-long][-link][-acc] n1 n2 ..	lista ficheros"},
    {"listdir", listdir, "[-reca] [-recb] [-hid][-long][-link][-acc] n1 n2 ..	lista ficheros contenidos de directorios"},
    {"fin", salir, "Termina la ejecucion del shell"},
    {"salir", salir, "Termina la ejecucion del shell"},
    {"bye", salir, "Termina la ejecucion del shell"},
    {NULL,  NULL, NULL}
};

int autores(char *tokens[], int ntokens, context *ctx) {
    if(ntokens != 0){
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

int pid(char *tokens[], int ntokens, context *ctx) {
    int pid, p_pid;

    pid = getpid();
    p_pid = getppid();
    
    if(ntokens != 0){
        if (strcmp(tokens[0], "-p") == 0){
            printf("Pid del padre del shell: %d\n", p_pid);
        }
    }else {
        printf("Pid de shell: %d\n", pid);

    }
    return 0;
}

int carpeta(char *tokens[], int ntokens, context *ctx) {
    char dir [MAX_LINE];

    if(ntokens != 0){
        char preDir [MAX_LINE];

        getcwd(preDir, sizeof(preDir));
        chdir(tokens[0]);
        getcwd(dir, sizeof(dir));

        if(strcmp(dir,preDir)==0){
            char out [MAX_LINE] = "Imposible cambiar directorio";
            perror(out);
        }

    }else {
        getcwd(dir, sizeof(dir));
        printf("%s\n", dir);

    }
    return 0;
}

int fecha(char *tokens[], int ntokens, context *ctx) {
    struct tm* fecha;
    time_t t;
    char fechaOut [MAX_LINE];
    char timeOut [MAX_LINE];
    t = time(NULL);
    fecha = localtime(&t);

    if(ntokens != 0){
        if (strcmp(tokens[0], "-d") == 0){
            // DD/MM/YYYY
            strftime(fechaOut, MAX_LINE, "%d/%m/%Y ",fecha);
            printf("%s\n",fechaOut);

        }else if (strcmp(tokens[0], "-h") == 0){
            // hh:mm:ss
            strftime(timeOut, MAX_LINE, "%H:%M:%S ",fecha);
            printf("%s\n",timeOut);

        }
    }else {
        strftime(fechaOut, MAX_LINE, "%d/%m/%Y ",fecha);
        printf("%s\n",fechaOut);
        strftime(timeOut, MAX_LINE, "%H:%M:%S ",fecha);
        printf("%s\n",timeOut);

    }

    return 0;
}

int infosis(char *tokens[], int ntokens, context *ctx) {
    struct utsname sys;

    if (!uname(&sys)){
       printf("%s (%s), OS: %s-%s-%s \n",
               sys.nodename, sys.machine, sys.sysname, sys.release, sys.version);
    }

    return 0;
}

int ayuda(char *tokens[], int ntokens, context *ctx) {
    if(ntokens != 0){
        for(int i=0; cmds[i].cmd_name != NULL; i++) {
            if(strcmp(tokens[0], cmds[i].cmd_name) ==0) {
                printf("%s %s\n", cmds[i].cmd_name, cmds[i].cmd_help);
            }
        }
    }else {
        printf("'ayuda cmd' donde cmd es uno de los siguientes comandos:\n");
        for(int i=0; cmds[i].cmd_name != NULL; i++) {
            printf("%s ",cmds[i].cmd_name);
        }
        printf("\n");
    }
    return 0;
}

int isNumber(char * string){
   for(int i = 0; i < strlen( string ); i ++){
      if (string[i] < 48 || string[i] > 57 ){
          if (string[i] != 45){
            return 0;
          }
      }
   }
   return 1;
}

int hist(char *tokens[], int ntokens, context *ctx) {
    int position = 0;

    if(ntokens != 0){
        if (strcmp(tokens[0], "-c") == 0){
            clean(ctx->historial);
            ctx->historial = init_list();
        }else if (isNumber(tokens[0])){
            int num = abs(atoi(tokens[0]));

            pos posData=first(ctx->historial);
            struct data *info = get(ctx->historial, posData);

            while(num != position && !end(ctx->historial, posData)) {
                printf("%d-> %s\n", position, info->cmd);
                posData = next(ctx->historial, posData);
                position++;
                info = get(ctx->historial, posData);
            }
        }
    }else {
        for(pos p=first(ctx->historial); !end(ctx->historial, p); p=next(ctx->historial, p)) {
            struct data *d = get(ctx->historial, p);
            printf("%d-> %s\n", position, d->cmd);
            position++;
        }
    }
    return 0;
}

int comando(char *tokens[], int ntokens, context *ctx) {
    if(ntokens != 0){
        if (isNumber(tokens[0])){
            int num = atoi(tokens[0]);

            pos pos=first(ctx->historial);
            struct data *info = get(ctx->historial, pos);
            int final= numPos(ctx->historial);

            if (num > final){
                printf("No existe comando en esa posición\n");

            }else{
                int position=0;
                while(num != position) {
                    pos=next(ctx->historial, pos);
                    info = get(ctx->historial, pos);
                    position++;
                }
                printf("Ejecutando hist (%d): %s\n", position, info->cmd);
                leerEntrada( 0, info->cmd, ctx);
                __fpurge(stdout);
            }
        }

    }else {
        hist(tokens, ntokens, ctx);
    }
    return 0;
}

int crear(char *tokens[], int ntokens, context *ctx) {

    if(ntokens != 0){
        char path[MAX_LINE];
        char out [MAX_LINE] = "Imposible crear";

        getcwd(path, sizeof(path));
        strcat(path, "/");

        if (strcmp(tokens[0], "-f") == 0){
            char* name = tokens[1];
            if(creat(strcat(path, name), 0666) !=0){
                perror(out);
            }

        }else{
            char* name = tokens[0];
            if(mkdir(strcat(path, name), 0755) !=0){
                perror(out);
            }
        }
    }else {
        carpeta(0,0,ctx);
    }
    return 0;
}

int borrar(char *tokens[], int ntokens, context *ctx) {
    char path[MAX_LINE];
    char aux[MAX_LINE];
    char out [MAX_LINE] = "Imposible borrar";

    getcwd(path, sizeof(path));
    strcat(path, "/");

    if(ntokens != 0){
        for(int i=0; i< ntokens; i++){
            strcpy(aux, path);
            if(remove(strcat(aux, tokens[i])) !=0){
                perror(out);
            }
        }
    }else {
        carpeta(0,0,ctx);
    }

    return 0;
}

int salir(char *tokens[], int ntokens, context *ctx) {
    return 1;
}
