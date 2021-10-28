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
        MAGENTA"[-f] [name]"RESET"	Crea un fichero o directorio"},
    {"borrar", borrar,
        MAGENTA"[name1 name2 ..]"RESET"	Borra ficheros o directorios vacios"},
    {"borrarrec", borrarrec,
        MAGENTA"[name1 name2 ..]"RESET"	Borra ficheros o directorios no vacios"},
    {"listfich", listfich,
        MAGENTA"[-long][-link][-acc] [n1 n2 ..]"RESET"	lista ficheros"},
    {"listdir", listdir,
        MAGENTA"[-reca] [-recb] [-hid][-long][-link][-acc] [n1 n2 ..]"RESET"	lista ficheros contenidos de directorios"},
    {"fin", salir,
        "Termina la ejecucion del shell"},
    {"salir", salir,
        "Termina la ejecucion del shell"},
    {"bye", salir,
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
        if (strcmp(tokens[0], "-c") == 0){ //borrar historial
            clean(ctx->historial);
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

int isDirEmpty(char *dirname) {   //Check if a directory is empty
    int n = 0;
    struct dirent *p;
    DIR *dir = opendir(dirname);
    if (dir == NULL) //Not a directory or doesn't exist
        return 1;
    while ((p = readdir(dir)) != NULL) {
        if(++n > 2)break;
    }
    closedir(dir);
    if (n <= 2) //Directory Empty
        return 1;
    else
        return 0;
}

int borrarDir(char *dir){  //Deletes a directory
    DIR *dirp;
    struct dirent *flist;
    char aux[MAX_LINE];

    if((dirp=opendir(dir)) ==NULL)return -1;

    while ((flist=readdir(dirp))!=NULL) { //recorre los archivos del directorio
        strcpy(aux, dir);
        strcat(strcat(aux, "/"),flist->d_name);

        if(strcmp(flist->d_name, "..") == 0 ||
                strcmp(flist->d_name, ".") == 0)continue;

        if(isDir(flist->d_name)){ //si es un directorio repetir recursivamente
            borrarDir(aux);
        }
        if(remove(aux))return -1; //borra el directorio
    }
    closedir(dirp);

    return 0;
}

int borrarrec(char *tokens[], int ntokens, context *ctx) {
    char out [MAX_LINE] = RED"Imposible borrar"RESET;

    if(ntokens != 0){
        for(int i=0; i< ntokens; i++){
            if(isDir(tokens[i]) && borrarDir(tokens[i])==-1){
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

char letraTF (mode_t m){
    switch (m&S_IFMT) { /*and bit a bit con los bits de formato,0170000 */
        case S_IFSOCK: return 's'; /*socket */
        case S_IFLNK:
            return 'l'; /*symbolic link*/
        case S_IFREG:
            return '-'; /* fichero normal*/
        case S_IFBLK:
            return 'b'; /*block device*/
        case S_IFDIR:
            return 'd'; /*directorio */
        case S_IFCHR:
            return 'c'; /*char device*/
        case S_IFIFO:
            return 'p'; /*pipe*/
        default: return '?'; /*desconocido, no deberia aparecer*/
    }
}

char * convierteModo (mode_t m){
    static char permisos[12];
    strcpy (permisos,"---------- ");
    permisos[0]=letraTF(m);
    if (m&S_IRUSR) permisos[1]='r';
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='r';
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r';
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s';
    if (m&S_ISGID) permisos[6]='s';
    if (m&S_ISVTX) permisos[9]='t';
    return permisos;
}

off_t sizeFich(char *file){     //Returns size of one file
    struct stat s;

    if(stat(file,&s)==-1) return -1;
    return s.st_size;
}

int printFileInfo(char *path, struct listOptions *opts){   //Shows one file's info
    struct stat s;
    struct group *grp;
    struct passwd *pwd;
    char fechaOut [MAX_LINE];
    char *permisos = "---------- ";
    struct tm lt;
    char symlink[MAX_LINE] = "";
    char *file = basename(path);
    char *fileColor;

    if(lstat(path,&s)==-1) return -1;

    permisos = convierteModo(s.st_mode);

    //set a different color for kinds of files
    if(permisos[0] == 'd')fileColor=BLUE; //is a directory
    else if (permisos[0] == 'l')fileColor=CYAN; //is a symlink
    else{
        if(permisos[3] == 'x')fileColor=GREEN; //is executable
        else fileColor=RESET;
    }

    if(!opts->lng){ //Basic listing
        long size;
        if((size=sizeFich(path))==-1)return -1;
        else printf("%ld\t%s%s\n"RESET,size,fileColor,file);
    }else{ //Long listing
        if((pwd = getpwuid(s.st_uid)) == NULL)return -1;
        if((grp = getgrgid(s.st_gid)) == NULL)return -1;

        if(opts->acc) localtime_r(&s.st_atime, &lt);
        else localtime_r(&s.st_mtime, &lt);

        // YY/MM/DD-hh:mm
        strftime(fechaOut, MAX_LINE, "%Y/%m/%d-%H:%M", &lt);

        printf("%s%4ld ( %ld)\t%s\t%s\t%s%9ld %s%s"RESET, fechaOut, s.st_nlink, s.st_ino,
                pwd->pw_name, grp->gr_name, permisos, s.st_size, fileColor, file) ;
        if(opts->link && (readlink(path, symlink, MAX_LINE)!=-1))
            printf(CYAN" -> %s\n"RESET, symlink);
        else printf("\n");
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

int isDir(const char *path){
    struct stat s;
    stat(path, &s);
    int out = S_ISDIR(s.st_mode);
    return out;
}

int listSubDir(char *dir, struct listOptions *opts){
    DIR *dirp;
    struct dirent *flist;
    char aux[MAX_LINE];

    if((dirp=opendir(dir)) ==NULL)return -1;
    while ((flist=readdir(dirp))!=NULL) { //recorre los archivos en el directorio

        if(!opts->hid && flist->d_name[0] == '.')continue;   //If "hid" option is off, we ignore
        if(strcmp(flist->d_name, "..") == 0 ||              // files that start with ".." or "."
                strcmp(flist->d_name, ".") == 0)continue;

        strcpy(aux, dir);
        strcat(strcat(aux, "/"),flist->d_name);
        if(isDir(aux)){
            printDirInfo(aux, opts);
        }
    }
    closedir(dirp);
    return 0;
}

int printDirInfo(char *dir, struct listOptions *opts){  //Shows one directory's information
    DIR *dirp;
    struct dirent *flist;
    char aux[MAX_LINE];

    if(opts->recb){ 
        if(listSubDir(dir, opts))return -1;
    } if((dirp=opendir(dir)) ==NULL)return -1;

    printf(YELLOW"✦****** %s ******✦\n"RESET,dir);
    while ((flist=readdir(dirp))!=NULL) { //recorre los archivos en el directorio
        strcpy(aux, dir);
        strcat(strcat(aux, "/"),flist->d_name);

        if(!opts->hid && flist->d_name[0] == '.')continue;

        if(printFileInfo(aux, opts))return -1;
    }
    closedir(dirp);

    if(!opts->recb && opts->reca){//recb has priority over reca
        if(listSubDir(dir, opts))return -1;
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
            }
    }else { //muestra el directorio actual
        carpeta(0,0,ctx);
    }
    return 0;
}

int salir(char *tokens[], int ntokens, context *ctx) {
    return 1;
}

/* int mklist(char *tokens[], int ntokens, context *ctx) { */
/*     struct memData *info = malloc(sizeof(struct data)); */
/*     info->mem = atoi(tokens[0]); */
/*     insert(&ctx->memory, info); */

/*     return 0; */
/* } */

/* int listTest(char *tokens[], int ntokens, context *ctx) { */
/*     int position = 0; */

/*     for(pos p=first(ctx->memory); !end(ctx->memory, p); p=next(ctx->memory, p)) { */
/*         struct memData *info = get(ctx->memory, p); */
/*         printf("%d-> %d\n", position, info->mem); */
/*         position++; */
/*     } */
/*     return 0; */
/* } */
