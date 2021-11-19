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
        MAGENTA" n"RESET"   Invoca a la funcion recursiva n veces"},
    {"recursiva", recursiva,
        MAGENTA"[n]"RESET"	Invoca a la funcion recursiva n veces"},
    {"e-s", e_s,
        MAGENTA"[read|write] [-o] fiche addr cont"RESET"   Lee o escribe bytes de memoria en un fichero"},
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

int mallocUs(char *tokens[], int ntokens, context *ctx){
    if(ntokens == 2 && strcmp(tokens[0], "-free") == 0 && isNumber(tokens[1])){ //free memory
        for(pos p=first(ctx->malloc); !end(ctx->malloc, p); p=next(ctx->malloc, p)) {
            struct memMalloc *info = get(ctx->malloc, p);
            if(info->tamano_bloque == atoi(tokens[1])){
                deleteAtPosition(&ctx->malloc,p,freeMem);
                break;
            }
        }
    }else if(ntokens != 0 && isNumber(tokens[0])){
        int num = atoi(tokens[0]);
        time_t t = time(NULL);
        struct memMalloc *info = malloc(sizeof(struct memMalloc));

        info->time = localtime(&t);
        info->tamano_bloque = num;
        info->direccion_bloque = malloc(num);

        insert(&ctx->malloc, info);
        printf("Asignados %d bytes en %p\n", num, info->direccion_bloque);
    }else { //show list
        printf(YELLOW"******Lista de bloques asignados malloc para el proceso %d\n"RESET, getpid());
        printMem(*ctx, 1,0,0);
    }
    return 0;
}

//mapea(o desmapea) ficheros en el espacio de direcciones del proceso
int mmapUs(char *tokens[], int ntokens, context *ctx){
    char *perm;
    void *p;
    int protection=0;
    if (ntokens==0){/*Listar Direcciones de Memoria mmap;*/
        printf(YELLOW"******Lista de ficheros mapeados por mmap para el proceso %d\n"RESET, getpid());
        printMem(*ctx, 0,1,0);
    }else{
        if(strcmp(tokens[0], "-free") ==0 ){
            for(pos p=first(ctx->mmap); !end(ctx->mmap, p); p=next(ctx->mmap, p)) {
                struct memMmap *info = get(ctx->mmap, p);
                if(strcmp(info->file_name, tokens[1])==0){
                    deleteAtPosition(&ctx->mmap,p,freeMmap);
                    break;
                }
            }
        }else{
            if ((perm=tokens[1])!=NULL && strlen(perm)<4) {
                if (strchr(perm,'r')!=NULL) protection|=PROT_READ;
                if (strchr(perm,'w')!=NULL) protection|=PROT_WRITE;
                if (strchr(perm,'x')!=NULL) protection|=PROT_EXEC;
            }
            if ((p=MmapFichero(tokens[0],protection,ctx))==NULL)
                perror (RED"Imposible mapear fichero"RESET);
            else
                printf ("fichero %s mapeado en %p\n", tokens[0], p);
        }
    }
    return 0;
}

//asigna(o desasigna) memoria compartida en el programa
int shared(char *tokens[], int ntokens, context *ctx){
    if(ntokens != 0){
        if(strcmp(tokens[0], "-free") ==0 && ntokens >= 2){
            for(pos p=first(ctx->shared); !end(ctx->shared, p); p=next(ctx->shared, p)) {
                struct memShared *info = get(ctx->shared, p);
                if(info->key == atoi(tokens[1])){
                    deleteAtPosition(&ctx->shared,p,free);
                    break;
                }
            }
            return 0;
        }else if(strcmp(tokens[0], "-create") ==0 && ntokens >= 3){
            SharedCreate(tokens, ctx);
            return 0;

        }else if(strcmp(tokens[0], "-delkey") ==0 && ntokens >= 2){
            SharedDelkey(&tokens[1]);
            return 0;
        }
    }
    printf(YELLOW"******Lista de bloques asignadoa shared para el proceso %d\n"RESET, getpid());
    printMem(*ctx, 0,0,1);
    return 0;
}


//Desasigna un bloque de memoria asignado con malloc, shared o mmap
int dealloc(char *tokens[], int ntokens, context *ctx){
    if(ntokens !=0){
        if(strcmp(tokens[0], "-malloc")== 0){
            char *input[] = {"-free",tokens[1]};
            mallocUs(input, 2, ctx);

        }else if(strcmp(tokens[0], "-mmap")== 0){
            char *input[] = {"-free",tokens[1]};
            mmapUs(input, 2, ctx);

        }else if(strcmp(tokens[0], "-shared")== 0){
            char *input[] = {"-free",tokens[1]};
            shared(input, 2, ctx);

        }else{ //delete from address
            char *ptr;
            long addr = strtoul(tokens[0],&ptr,16);

            for(pos p=first(ctx->malloc); !end(ctx->malloc, p); p=next(ctx->malloc, p)) {
                struct memMalloc *info = get(ctx->malloc, p);
                if(info->direccion_bloque == (long *)addr){
                    printf("delete at position %p\n", (long *)addr);
                    deleteAtPosition(&ctx->malloc,p,freeMem);
                    break;
                }
            }
            for(pos p=first(ctx->mmap); !end(ctx->mmap, p); p=next(ctx->mmap, p)) {
                struct memMmap *info = get(ctx->mmap, p);
                if(info->direccion_bloque == (long *)addr){
                    printf("delete at position %p\n", (long *)addr);
                    deleteAtPosition(&ctx->mmap,p,freeMmap);
                    break;
                }
            }
            for(pos p=first(ctx->shared); !end(ctx->shared, p); p=next(ctx->shared, p)) {
                struct memShared *info = get(ctx->shared, p);
                if(info->direccion_bloque == (long *)addr){
                    printf("delete at position %p\n", (long *)addr);
                    deleteAtPosition(&ctx->shared,p,free);
                    break;
                }
            }
        }
    }else{
        printf(YELLOW"******Lista de bloques asignados para el proceso %d\n"RESET, getpid());
        printMem(*ctx, 1,1,1);
    }
    return 0;
}

//global variables for function memoria
int global1=0,global2=0,global3=0;
//Muestra muestra detalles de la memoria del proceso
int memoria(char *tokens[], int ntokens, context *ctx){
    if(ntokens !=0){
        for(int i=0;i<ntokens;i++){
            if(strcmp(tokens[i], "-vars")== 0){
                auto int x=0,y=0,z=0;
                static int a=0,b=0,c=0;

                printf("automatic variables:\t%p, %p, %p\n", &x, &y, &z);
                printf("static variables:\t%p, %p, %p\n", &a, &b, &c);
                printf("global variables:\t%p, %p, %p\n", &global1, &global2, &global3);

            }if(strcmp(tokens[i], "-funcs")== 0){
                printf("program functions:\t%p, %p, %p\n", autores, pid, infosis);
                printf("library functions:\t%p, %p, %p\n", malloc, printf, strcmp);

            }if(strcmp(tokens[i], "-blocks")== 0){
                dealloc(NULL, 0, ctx);

            }else if(strcmp(tokens[i], "-all")== 0){
                char *input[] = {"-vars","-funcs", "-blocks"};
                memoria(input, 3, ctx);

            }else if(strcmp(tokens[i], "-pmap")== 0){
                dopmap();
            }
        }
    }else{
        char *input[] = {"-all"};
        memoria(input, 1, ctx);
    }
    return 0;
}

//Vuelca en pantallas los contenidos (cont bytes) de la posicion de memoria addr
int volcarmem(char *tokens[], int ntokens, context *ctx){
    if(ntokens !=0){
        int n = 25;
        if(ntokens == 2 && isNumber(tokens[1])) n = atoi(tokens[1]);

        char *ptr;
        long addr = strtoul(tokens[0],&ptr,16);

        for(int i=0;i<n;i+=min(n-i, 25)){
            long aux = addr;
            for(int j=0;j<min(n-i, 25);j++){
                printf(" %c ", (*(char *)aux == '\n')? ' ' : *(char *)aux);
                aux ++;
            }
            printf("\n");
            for(int j=0;j<min(n-i, 25);j++){
                printf(YELLOW"%02X "RESET, *(char *)addr);
                addr ++;
            }
            printf("\n");
        }

    }else printf("uso: volcarmem"RED" addr [count]\n"RESET);
    return 0;
}

//Llena la memoria a partir de addr con byte
int llenarmem(char *tokens[], int ntokens, context *ctx){
    if(ntokens !=0){
        char *ptr;
        int n = 128;
        int c = 65;
        if(ntokens >= 2 && isNumber(tokens[1])) n = atoi(tokens[1]);
        if(ntokens > 2 && isNumber(tokens[1])){
            if(isNumber(tokens[2])) c = atoi(tokens[2]);
            else c = strtoul(tokens[2],&ptr,16);
        }

        long addr = strtoul(tokens[0],&ptr,16);
        for(int i=0;i<n;i++){
            *(int *)addr = c;
            addr ++;
        }
    }else printf("uso: llenarmem"RED" addr [count] [byte]\n"RESET);
    return 0;
}

//Invoca a la funcion recursiva n veces
int recursiva(char *tokens[], int ntokens, context *ctx){
    if(ntokens !=0){
        if (isNumber(tokens[0])){ //mostrar historial hasta num
            int num = atoi(tokens[0]);
            doRecursiva(num);
        }else printf(RED"parametro no es un numero\n"RESET);
    }else printf("uso: recursiva"RED" n\n"RESET);

    return 0;
}

int e_s(char *tokens[], int ntokens, context *ctx){
    if(ntokens !=0){
        char *ptr;
        if(strcmp(tokens[0], "read")== 0){
            int n = ((ssize_t)-1);
            long addr = strtoul(tokens[2],&ptr,16);
            if(ntokens==4){
                n = atoi(tokens[3]);
            }if(ntokens >= 3){
                if(LeerFichero(tokens[1], (long *)addr, n)==-1)perror(RED"error de lectura"RESET);
            }
        }else if(strcmp(tokens[0], "write")== 0){
            if((strcmp(tokens[1], "-o")==0)){
                creat(tokens[2], 0666);
                long addr = strtoul(tokens[3],&ptr,16);
                if(EscribirFichero(tokens[2], (long *)addr, atoi(tokens[4]))==-1)
                    perror(RED"error de escritura"RESET);
            }else if(open(tokens[1],O_RDWR)==-1){
                creat(tokens[1], 0666);
                long addr = strtoul(tokens[2],&ptr,16);
                if(EscribirFichero(tokens[1], (long *)addr, atoi(tokens[3]))==-1)
                    perror(RED"error de escritura"RESET);
            }
        }
    }
    else printf("uso: e-s "RED"[read|write]"RESET" ......\n");
    return 0;
}

int salir(char *tokens[], int ntokens, context *ctx) {
    return 1;
}
