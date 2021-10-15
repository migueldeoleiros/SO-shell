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

int isDirEmpty(char *dirname) {
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

int borrarrec(char *tokens[], int ntokens, context *ctx) { char path[MAX_LINE];
    char aux[MAX_LINE];
    char out [MAX_LINE] = "Imposible borrar";
    DIR *dirp;
    struct dirent *dp;

    getcwd(path, sizeof(path));
    strcat(path, "/");


    if(ntokens != 0){
        for(int i=0; i< ntokens; i++){
            strcpy(aux, path);
            while(!isDirEmpty(strcat(aux, tokens[i]))){

            }
            dirp = opendir(strcat(aux, tokens[i]));
            dp = readdir(dirp);
            for(int j =0;readdir(dirp)!=NULL;j++){
                printf("%s\n",strcat(aux, &dp->d_name[j]));
            }
            /* if(remove(strcat(aux, dp->d_name)) !=0){ */
            /*     perror(out); */
            /* } */
        }
        closedir(dirp);
    }else {
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

off_t sizeFich(char *file){
    struct stat s;

    if(stat(file,&s)==-1) return -1;
    return s.st_size;
}

int printFileInfo(char *file, int acc, int link){
    struct stat s;
    struct group *grp;
    struct passwd *pwd;
    char fechaOut [MAX_LINE];
    char *permisos = "---------- ";
    struct tm lt;
    char symlink[MAX_LINE];

    if(lstat(file,&s)==-1) return -1;

    pwd = getpwuid(s.st_uid);
    grp = getgrgid(s.st_gid);

    permisos = convierteModo(s.st_mode);

    if(acc) localtime_r(&s.st_ctime, &lt);
    else localtime_r(&s.st_mtime, &lt);

    strftime(fechaOut, MAX_LINE, "%Y/%m/%d-%H:%M", &lt);

    printf("%s\t%ld ( %ld)\t%s\t%s\t%s\t%ld %s", fechaOut, s.st_nlink, s.st_ino,
            pwd->pw_name, grp->gr_name, permisos, s.st_size, file) ;
    if(link && (readlink(file, symlink, MAX_LINE)!=-1))
        printf(" -> %s\n", symlink);
    else printf("\n");

    return 0;
}


int listfich(char *tokens[], int ntokens, context *ctx) {
    char path[MAX_LINE];
    char out [MAX_LINE] = "error de lectura";
    off_t size;

    getcwd(path, sizeof(path));
    strcat(path, "/");

    if(ntokens != 0){
        int acc=0,link=0,lng=0;

        for(int i=0;i<ntokens;i++){
            if(strcmp(tokens[i], "-long") == 0) lng=1;
            if(strcmp(tokens[i], "-acc") == 0) acc=1;
            if(strcmp(tokens[i], "-link") == 0) link=1;
        }
        if(lng){
            for(int i=1+(acc+link); i< ntokens; i++){
                if(printFileInfo(tokens[i], acc, link)==-1){
                    perror(out);
                }
            }
        }else{
            for(int i=0; i< ntokens; i++){
                if((size=sizeFich(tokens[i]))==-1){
                    perror(out);
                }else{
                    printf("%ld\t%s\n",size, tokens[i]);
                }
            }
        }
    }else {
        carpeta(0,0,ctx);
    }
    return 0;
}

int printDirInfo(char *dir, int lng, int acc, int link, int hid){
    off_t size;
    DIR *dirp;
    struct dirent *flist;

    if((dirp=opendir(dir)) ==NULL)return -1;
    while ((flist=readdir(dirp))!=NULL) {
        chdir(dir);
        if(!hid && flist->d_name[0] == '.')continue;
        if(lng)printFileInfo(flist->d_name, acc,link);
        else{
            if((size=sizeFich(flist->d_name))==-1){
                return -1;
            }else{
                printf("%ld\t%s\n",size, flist->d_name);
            }
        }
        chdir("..");
    }
    closedir(dirp);
    return 0;
}

int listdir(char *tokens[], int ntokens, context *ctx) {
    char path[MAX_LINE];
    char out [MAX_LINE] = "error de lectura";
    off_t size;

    getcwd(path, sizeof(path));
    strcat(path, "/");

    if(ntokens != 0){
            int acc=0,link=0,hid=0,lng=0;

            for(int i=0;i<ntokens;i++){
                if(strcmp(tokens[i], "-long") == 0) lng=1;
                if(strcmp(tokens[i], "-acc") == 0) acc=1;
                if(strcmp(tokens[i], "-link") == 0) link=1;
                if(strcmp(tokens[i], "-hid") == 0) hid=1;
            }
            for(int i=(acc+link+hid+lng); i< ntokens; i++){
                if(printDirInfo(tokens[i] ,lng , acc, link, hid)==-1){
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
