#include "headers.h"

struct cmd cmds[] ={
    {"autores", autores,
        MAGENTA"[-n|-l]"RESET"	Muestra los nombres y logins de los autores"},
    {"pid", pid,
        MAGENTA"[-p]"RESET"	Muestra el pid del shell o de su proceso padre"},
    {"carpeta", carpeta,
        MAGENTA"[dir]"RESET"	Cambia (o muestra) el directorio actual del shell"},
    {"fecha", fecha,
        MAGENTA"[-d|-h]"RESET"	Muestra la fecha y o la hora actual"},
    {"infosis", infosis,
        "Muestra informacion de la maquina donde corre el shell"},
    {"hist", hist,
        MAGENTA"[-c|-N]"RESET"	Muestra el historico de comandos, con -c lo borra"},
    {"ayuda", ayuda,
        MAGENTA"[cmd]"RESET"	Muestra ayuda sobre los comandos"},
    {"comando", comando,
        MAGENTA"[N]"RESET"	Repite el comando N (del historico)"},
    {"crear", crear,
        MAGENTA"[-f][name]"RESET"	Crea un fichero o directorio"},
    {"borrar", borrar,
        MAGENTA"[name1 name2 ..]"RESET"	Borra ficheros o directorios vacios"},
    {"borrarrec", borrarrec,
        MAGENTA"[name1 name2 ..]"RESET"	Borra ficheros o directorios no vacios"},
    {"listfich", listfich,
        MAGENTA"[-long][-link][-acc][n1 n2 ..]"RESET"	lista ficheros"},
    {"listdir", listdir,
        MAGENTA"[-reca][-recb][-hid][-long][-link][-acc] [n1 n2 ..]"RESET"	lista ficheros contenidos de directorios"},
    {"malloc", mallocUs,
        MAGENTA"[-free][tam]"RESET"	asigna(o desasigna) memoria en el programa"},
    {"mmap", mmapUs,
        MAGENTA"[-free][fich][perm]"RESET"	mapea(o desmapea) ficheros en el espacio de direcciones del proceso"},
    {"shared", shared,
        MAGENTA"[-free|-create|-delkey][cl][tam]"RESET"	asigna(o desasigna) memoria compartida en el programa"},
    {"dealloc", dealloc,
        MAGENTA"[-malloc|-shared|-mmap]..."RESET"	Desasigna un bloque de memoria asignado con malloc, shared o mmap"},
    {"memoria", memoria,
        MAGENTA"[-blocks|-funcs|-vars|-all|-pmap]..."RESET"	Muestra muestra detalles de la memoria del proceso"},
    {"volcarmem", volcarmem,
        MAGENTA"[addr][cont]"RESET"	Vuelca en pantallas los contenidos (cont bytes) de la posicion de memoria addr"},
    {"llenarmem", llenarmem,
        MAGENTA"[addr][cont][byte]"RESET"	Llena la memoria a partir de addr con byte"},
    {"recursiva", recursiva,
        MAGENTA"[n]"RESET"	Invoca a la funcion recursiva n veces"},
    {"fin", salir,
        "Termina la ejecucion del shell"},
    {"salir", salir,
        "Termina la ejecucion del shell"},
    {"bye", salir,
        "Termina la ejecucion del shell"},
    {"exit", salir,
        "Termina la ejecucion del shell"},
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
            printf(YELLOW"Pid del padre del shell:"RESET" %d\n", p_pid);
        }
    }else {
        printf(YELLOW"Pid de shell:"RESET" %d\n", pid);

    }
    return 0;
}

int carpeta(char *tokens[], int ntokens, context *ctx) {
    char dir [MAX_LINE];
    char out [MAX_LINE] = RED"Imposible cambiar directorio"RESET;

    if(ntokens != 0){ //cambia al directorio dir
        char preDir [MAX_LINE];

        getcwd(preDir, sizeof(preDir));
        chdir(tokens[0]);
        getcwd(dir, sizeof(dir));

        if(strcmp(dir,preDir)==0){
            perror(out);
        }

    }else { //muestra el directorio actual
        getcwd(dir, sizeof(dir));
        printf("%s\n", dir);

    }
    return 0;
}

int fecha(char *tokens[], int ntokens, context *ctx) {
    time_t t= time(NULL);
    struct tm* fecha= localtime(&t);
    char fechaOut [MAX_LINE];
    char timeOut [MAX_LINE];

    if(ntokens != 0){
        if (strcmp(tokens[0], "-d") == 0){ //fecha
            // DD/MM/YYYY
            strftime(fechaOut, MAX_LINE, "%d/%m/%Y ",fecha);
            printf("%s\n",fechaOut);

        }else if (strcmp(tokens[0], "-h") == 0){ //hora
            // hh:mm:ss
            strftime(timeOut, MAX_LINE, "%H:%M:%S ",fecha);
            printf("%s\n",timeOut);

        }
    }else { //fecha y hora
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
    if(ntokens != 0){ //muestra ayuda sobre el comando tokens[0]
        for(int i=0; cmds[i].cmd_name != NULL; i++) {
            if(strcmp(tokens[0], cmds[i].cmd_name) ==0) {
                printf(GREEN"%s"RESET" %s\n", cmds[i].cmd_name, cmds[i].cmd_help);
            }
        }
    }else { //ayuda general (lista de comandos)
        printf(YELLOW"'"GREEN"ayuda cmd"YELLOW"' donde "GREEN"cmd"YELLOW" es uno de los siguientes comandos:\n"RESET);
        for(int i=0; cmds[i].cmd_name != NULL; i++) {
            printf("%s ",cmds[i].cmd_name);
        }
        printf("\n");
    }
    return 0;
}

int hist(char *tokens[], int ntokens, context *ctx) {
    int position = 0;

    if(ntokens != 0){
        if (strcmp(tokens[0], "-c") == 0){ //borrar historial
            freeList(&ctx->historial,free);
            ctx->historial = init_list();
        }else if (isNumber(tokens[0])){ //mostrar historial hasta num
            int num = abs(atoi(tokens[0]));

            pos posData=first(ctx->historial);
            struct histData *info = get(ctx->historial, posData);

            while(num != position && !end(ctx->historial, posData)) {
                printf("%d-> %s\n", position,info->cmd);
                posData = next(ctx->historial, posData);
                position++;
                info = get(ctx->historial, posData);
            }
        }
    }else { //mostrar historial
        for(pos p=first(ctx->historial); !end(ctx->historial, p); p=next(ctx->historial, p)) {
            struct histData *info = get(ctx->historial, p);
            printf("%d-> %s\n", position, info->cmd);
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
            char *cmd = get(ctx->historial, pos);
            int final= numPos(ctx->historial);

            if (num > final){ //si se pasa de las posiciones que existen
                printf(RED"No existe comando en esa posición\n"RESET);

            }else{
                int position=0;
                while(num != position) { //recorre el historial hasta llegar al deseado
                    pos=next(ctx->historial, pos);
                    cmd = get(ctx->historial, pos);
                    position++;
                }
                printf("Ejecutando hist (%d): "GREEN"%s\n"RESET, position, cmd);
                leerEntrada( 0, cmd, ctx); //ejecuta el comando
            }
        }else printf(RED"Error: "RESET"%s"RED" no es un número\n"RESET,tokens[0]);

    }else {
        hist(tokens, ntokens, ctx);
    }
    return 0;
}

int crear(char *tokens[], int ntokens, context *ctx) {

    if(ntokens != 0){
        char path[MAX_LINE];
        char out [MAX_LINE] = RED"Imposible crear"RESET;

        getcwd(path, sizeof(path));
        strcat(path, "/");

        if (strcmp(tokens[0], "-f") == 0){ //para crear archivo
            char* name = tokens[1];
            if(creat(strcat(path, name), 0666) ==-1){
                perror(out);
            }

        }else{ //para crear un directorio
            char* name = tokens[0];
            if(mkdir(strcat(path, name), 0755) ==-1){
                perror(out);
            }
        }
    }else {
        carpeta(0,0,ctx);
    }
    return 0;
}

int borrar(char *tokens[], int ntokens, context *ctx) {
    char out [MAX_LINE] = RED"Imposible borrar"RESET;

    if(ntokens != 0){ //borra archivo
        for(int i=0; i< ntokens; i++){
            if(remove(tokens[i]) !=0){
                perror(out);
            }
        }
    }else { //muestra el directorio actual
        carpeta(0,0,ctx);
    }
    return 0;
}


int borrarrec(char *tokens[], int ntokens, context *ctx) {
    char out [MAX_LINE] = RED"Imposible borrar"RESET;

    if(ntokens != 0){
        for(int i=0; i< ntokens; i++){
            if(isDir(tokens[i]) ){
                if(borrarDir(tokens[i])==-1 || remove(tokens[i]))
                    perror(out);
            }else if(remove(tokens[i])){
                perror(out);
            }
        }
    }else { //muestra el directorio actual
        carpeta(0,0,ctx);
    }
    return 0;
}

int listfich(char *tokens[], int ntokens, context *ctx) {
    char path[MAX_LINE];
    char out [MAX_LINE] = RED"error de lectura"RESET;

    getcwd(path, sizeof(path));
    strcat(path, "/");

    if(ntokens != 0){
        struct listOptions opts = {0,0,0,0,0,0};

        for(int i=0;i<ntokens;i++){
            if(strcmp(tokens[i], "-long") == 0) opts.lng=1;
            else if(strcmp(tokens[i], "-acc") == 0) opts.acc=1;
            else if(strcmp(tokens[i], "-link") == 0) opts.link=1;
        }
        int i=(opts.lng+opts.acc+opts.link);
        if(i==ntokens)carpeta(0,0,ctx);//si no existe parametro a listar
        while(i< ntokens){
            if(printFileInfo(tokens[i], &opts)==-1){
                perror(out);
            }
            i++;
        }
    }else { //muestra el directorio actual
        carpeta(0,0,ctx);
    }
    return 0;
}

int listdir(char *tokens[], int ntokens, context *ctx) {
    char out [MAX_LINE] = RED"Error de lectura"RESET;

    if(ntokens != 0){
            struct listOptions opts = {0,0,0,0,0,0};

            for(int i=0;i<ntokens;i++){
                if(strcmp(tokens[i], "-long") == 0) opts.lng=1;
                else if(strcmp(tokens[i], "-acc") == 0) opts.acc=1;
                else if(strcmp(tokens[i], "-link") == 0) opts.link=1;
                else if(strcmp(tokens[i], "-hid") == 0) opts.hid=1;
                else if(strcmp(tokens[i], "-reca") == 0) opts.reca=1;
                else if(strcmp(tokens[i], "-recb") == 0) opts.recb=1;
            }
            int i=(opts.reca+opts.recb+opts.acc+opts.link+opts.hid+opts.lng);
            if(i==ntokens)carpeta(0,0,ctx);//si no existe parametro a listar
            while(i< ntokens){
                if(printDirInfo(tokens[i], &opts)==-1){
                    perror(out);
                }
                i++;
            } }else { //muestra el directorio actual
        carpeta(0,0,ctx);
    }
    return 0;
}

void freeMem(void *ptr) {
    struct memData *mem = ptr;

    free(mem->direccion_bloque);
    free(mem->time);
}

int mallocUs(char *tokens[], int ntokens, context *ctx){
    if(ntokens != 0 && strcmp(tokens[0], "-free") == 0 && ntokens ==2 && isNumber(tokens[1])){ //free memory
        for(pos p=first(ctx->memory); !end(ctx->memory, p); p=next(ctx->memory, p)) {
            struct memData *info = get(ctx->memory, p);
            if(info->tamano_bloque == atoi(tokens[1])){
              deleteAtPosition(&ctx->memory,p,free);
              break;
            }
        }
    }else if(ntokens != 0 && isNumber(tokens[0])){
        int num = atoi(tokens[0]);
        time_t t = time(NULL);
        struct memData *info = malloc(sizeof(struct memData));

        info->time = localtime(&t);
        info->tipo_reserva = 0;
        info->tamano_bloque = num;
        info->direccion_bloque = malloc(num);

        insert(&ctx->memory, info);
        printf("Asignados %d bytes en %p\n", num, &info->direccion_bloque);
    }else { //mostrar list
        char time [MAX_LINE];
        printf("******Lista de bloques asignados malloc para el proceso %d\n", getpid());

        for(pos p=first(ctx->memory); !end(ctx->memory, p); p=next(ctx->memory, p)) {
            struct memData *info = get(ctx->memory, p);
            strftime(time, MAX_LINE, "%b %d %H:%M ",info->time);
            printf("\t%p%12d %s %d\n", &info->direccion_bloque, info->tamano_bloque, time, info->tipo_reserva);
        }
    }
    return 0;
}
void * MmapFichero (char * fichero, int protection, context *ctx){
    int df, map=MAP_PRIVATE,modo=O_RDONLY;
    struct stat s;
    void *p;
    if (protection&PROT_WRITE)
        modo=O_RDWR;
    if (stat(fichero,&s)==-1 || (df=open(fichero, modo))==-1)
        return NULL;
    if ((p=mmap(NULL,s.st_size, protection,map,df,0))==MAP_FAILED)
        return NULL;
    /*Guardar Direccion de Mmap (p, s.st_size,fichero,df......);*/
    time_t t = time(NULL);
    struct memData *info = malloc(sizeof(struct memData));

    info->time = localtime(&t);
    info->tipo_reserva = 1;
    info->tamano_bloque = sizeFich(fichero);
    info->direccion_bloque = p;

    insert(&ctx->memory, info);
    return p;
}
int mmapUs(char *tokens[], int ntokens, context *ctx){ /*arg[0] is the file name and arg[1] is the permissions*/
    char *perm;
    void *p;
    int protection=0;
    if (tokens[0]==NULL){/*Listar Direcciones de Memoria mmap;*/ 
        return 0;}
    if ((perm=tokens[1])!=NULL && strlen(perm)<4) {
        if (strchr(perm,'r')!=NULL) protection|=PROT_READ;
        if (strchr(perm,'w')!=NULL) protection|=PROT_WRITE;
        if (strchr(perm,'x')!=NULL) protection|=PROT_EXEC;
    }
    if ((p=MmapFichero(tokens[0],protection,ctx))==NULL)
        perror ("Imposible mapear fichero");
    else
        printf ("fichero %s mapeado en %p\n", tokens[0], p);
    return 0;
}

#define LEERCOMPLETO ((ssize_t)-1)
ssize_t LeerFichero (char *fich, void *p, ssize_t n){ /* le n bytes del fichero fich en p */
    ssize_t nleidos,tam=n; /*si n==-1 lee el fichero completo*/
    int df, aux;
    struct stat s;
    if (stat (fich,&s)==-1 || (df=open(fich,O_RDONLY))==-1)
        return ((ssize_t)-1);
    if (n==LEERCOMPLETO)
        tam=(ssize_t) s.st_size;
    if ((nleidos=read(df,p, tam))==-1){
        aux=errno;
        close(df);
        errno=aux;
        return ((ssize_t)-1);
    }
    close (df);
    return (nleidos);
}
//mapea(o desmapea) ficheros en el espacio de direcciones del proceso
/* int mmapUs(char *tokens[], int ntokens, context *ctx){ */
/*     return 0; */
/* } */

//asigna(o desasigna) memoria compartida en el programa
int shared(char *tokens[], int ntokens, context *ctx){
    return 0;
}
//Desasigna un bloque de memoria asignado con malloc, shared o mmap
int dealloc(char *tokens[], int ntokens, context *ctx){
    return 0;
}
//Muestra muestra detalles de la memoria del proceso
int memoria(char *tokens[], int ntokens, context *ctx){
    return 0;
}
//Vuelca en pantallas los contenidos (cont bytes) de la posicion de memoria addr
int volcarmem(char *tokens[], int ntokens, context *ctx){
    return 0;
}
//Llena la memoria a partir de addr con byte
int llenarmem(char *tokens[], int ntokens, context *ctx){
    return 0;
}
//Invoca a la funcion recursiva n veces
int recursiva(char *tokens[], int ntokens, context *ctx){
    return 0;
}

int salir(char *tokens[], int ntokens, context *ctx) {
    return 1;
}
