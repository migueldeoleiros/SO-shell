#include "headers.h"
#include <stdio.h>
#include <stdlib.h>

int empiezaPor(const char *pre, const char *str){
    if(strncmp(pre, str, strlen(pre)) == 0) return 0;
    return 1;
}

void freeMem(void *ptr) {
    struct memMalloc *mem = ptr;

    free(mem->direccion_bloque);
    free(mem);
}

void freeMmap(void *ptr) {
    struct memMmap *mem = ptr;

    munmap(mem->direccion_bloque, mem->tamano_bloque);
    free(mem);
}

int min(int a, int b){
    if(a<b) return a;
    else return b;
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

        if(isDir(aux)){ //si es un directorio repetir recursivamente
            borrarDir(aux);
        }
        if(remove(aux))return -1; //borra el directorio
    }
    closedir(dirp);

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

void printMem(context ctx, int malloc, int mmap, int shared){
    char time [MAX_LINE];

    if(malloc == 1)
        for(pos p=first(ctx.malloc); !end(ctx.malloc, p); p=next(ctx.malloc, p)) {
            struct memMalloc *info = get(ctx.malloc, p);
            strftime(time, MAX_LINE, "%b %d %H:%M ",info->time);
            printf("\t%p%12d %s ", info->direccion_bloque, info->tamano_bloque, time);
            printf("malloc\n");
        }
    if(mmap == 1)
        for(pos p=first(ctx.mmap); !end(ctx.mmap, p); p=next(ctx.mmap, p)) {
            struct memMmap *info = get(ctx.mmap, p);
            strftime(time, MAX_LINE, "%b %d %H:%M ",info->time);
            printf("\t%p%12d %s ", info->direccion_bloque, info->tamano_bloque, time);
            printf("mmap %s (fd:%d)\n",info->file_name, info->fd);
        }
    if(shared == 1)
        for(pos p=first(ctx.shared); !end(ctx.shared, p); p=next(ctx.shared, p)) {
            struct memShared *info = get(ctx.shared, p);
            strftime(time, MAX_LINE, "%b %d %H:%M ",info->time);
            printf("\t%p%12d %s ", info->direccion_bloque, info->tamano_bloque, time);
            printf("shared memory (key:%d)\n", info->key);
        }
}

void deleteMemAddress(char* address,context* ctx){ //delete from address
    char *ptr;
    long addr = strtoul(address,&ptr,16);

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

void * MmapFichero (char * fichero, int protection, context *ctx){
    int fd, map=MAP_PRIVATE,modo=O_RDONLY;
    struct stat s;
    void *p;
    if (protection&PROT_WRITE)
        modo=O_RDWR;
    if (stat(fichero,&s)==-1 || (fd=open(fichero, modo))==-1)
        return NULL;
    if ((p=mmap(NULL,s.st_size, protection,map,fd,0))==MAP_FAILED)
        return NULL;

    //Guardar Direccion de Mmap (p, s.st_size,fichero,df......)
    time_t t = time(NULL);
    struct memMmap *info = malloc(sizeof(struct memMmap));

    info->time = localtime(&t);
    info->tamano_bloque = sizeFich(fichero);
    info->direccion_bloque = p;
    info->fd = fd;
    strcpy(info->file_name, fichero);

    insert(&ctx->mmap, info);
    return p;
}

void * ObtenerMemoriaShmget (key_t clave, size_t tam){
/*Obtienen un puntero a una zaona de memoria compartida*/
/*si tam >0 intenta crearla y si tam==0 asume que existe*/
    void * p;
    int aux,id,flags=0777;
    struct shmid_ds s;
    if (tam)
    /*si tam no es 0 la crea en modo exclusivo
    esta funcion vale para shared y shared -create*/
        flags=flags | IPC_CREAT | IPC_EXCL;
        /*si tam es 0 intenta acceder a una ya creada*/
    if (clave==IPC_PRIVATE)
    /*no nos vale*/
        {errno=EINVAL; return NULL;}
    if ((id=shmget(clave, tam, flags))==-1)
        return (NULL);
    if ((p=shmat(id,NULL,0))==(void*) -1){
        aux=errno;
    /*si se ha creado y no se puede mapear*/
        if (tam)
/*se borra */
            shmctl(id,IPC_RMID,NULL);
        errno=aux;
        return (NULL);
    }
    shmctl (id,IPC_STAT,&s);
    /* Guardar En Direcciones de Memoria Shared (p, s.shm_segsz, clave.....);*/
    return (p);
}

void SharedCreate (char *arg[], context *ctx){ /*arg[0] is the key and arg[1] is the size*/
    key_t k;
    size_t tam=0;
    void *p;
    if (arg[0]==NULL || arg[1]==NULL){/*Listar Direcciones de Memoria Shared */
        printf(YELLOW"******Lista de bloques asignadoa shared para el proceso %d\n"RESET, getpid());
        printMem(*ctx, 0,0,1);
        return;
    }
    k=(key_t) atoi(arg[1]);
    if (arg[1]!=NULL)
        tam=(size_t) atoll(arg[2]);
    if ((p=ObtenerMemoriaShmget(k,tam))==NULL)
        perror (RED"Imposible obtener memoria shmget"RESET);
    else{
        time_t t = time(NULL);
        struct memShared *info = malloc(sizeof(struct memShared));

        info->time = localtime(&t);
        info->tamano_bloque = tam;
        info->key = k;
        info->direccion_bloque = p;

        insert(&ctx->shared, info);
        printf ("Memoria de shmget de clave %d asignada en %p\n",k,p);
    }
}

void SharedDelkey (char *args[]){ /*arg[0] points to a str containing the key*/
    key_t clave;
    int id;
    char *key=args[0];
    if (key==NULL || (clave=(key_t) strtoul(key,NULL,10))==IPC_PRIVATE){
        printf ("shared -delkey clave_valida\n");
        return;
    }
    if ((id=shmget(clave,0,0666))==-1){
        perror (RED"shmget: imposible obtener memoria compartida"RESET);
        return;
    }
    if (shmctl(id,IPC_RMID,NULL)==-1)
        perror (RED"shmctl: imposible eliminar memoria compartida\n"RESET);
}

void dopmap (void){ /*no arguments necessary*/
    pid_t pid;
    /*ejecuta el comando externo pmap para */
    char elpid[32];
    /*pasandole el pid del proceso actual */
    char *argv[3]={"pmap",elpid,NULL};
        sprintf (elpid,"%d", (int) getpid());
    if ((pid=fork())==-1){
        perror (RED"Imposible crear proceso"RESET);
        return;
    }
    if (pid==0){
        if (execvp(argv[0],argv)==-1)
            perror(RED"cannot execute pmap"RESET);
        exit(1);
    }
    waitpid (pid,NULL,0);
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

ssize_t EscribirFichero (char *fich, void *p, ssize_t n){
    ssize_t nescritos,tam=n;
    int df, aux;
    df=open(fich,O_RDWR);
    if((nescritos=write(df,p,tam))==-1){
        aux=errno;
        close(df);
        errno=aux;
        return -1;
    }
    close (df);
    return (nescritos);
}

void doRecursiva(int n){
  char automatico[MAX_LINE];
  static char estatico[MAX_LINE];

  printf("parametro n:%d en %p\n",n,&n);
  printf("array estatico en:%p \n",estatico);
  printf("array automatico en %p\n",automatico);
  printf(YELLOW"**************\n"RESET);

  n--;
  if(n>0)
    doRecursiva(n);
}

void redirectStderr(char *file){
  fflush(stderr);
  int fd=open(file,O_WRONLY | O_CREAT | O_TRUNC, 0644);
  dup2(fd,STDERR_FILENO);
  close(fd);
}

void restoreStderr(){
  fflush(stderr);
  dup2(STDOUT_FILENO,STDERR_FILENO);
}

void MostrarEntorno (char **entorno, char * nombre_entorno){
    int i=0;
    while (entorno[i]!=NULL) {
          printf ("%p->%s[%d]=(%p) %s\n", &entorno[i],nombre_entorno, i,entorno[i],entorno[i]);
          i++;
        }
      }

int BuscarVariable (char * var, char *e[]){
  int pos=0;
  char aux[MAX_LINE];
  strcpy (aux,var);
  strcat (aux,"=");
  while (e[pos]!=NULL)
    if (!strncmp(e[pos],aux,strlen(aux)))
      return (pos);
    else
      pos++;
  errno=ENOENT;
  return(-1);
}

int CambiarVariable(char * var, char * valor, char *e[]){
  int pos;
  char *aux;
  if ((pos=BuscarVariable(var,e))==-1)
      return(-1);
  if ((aux=(char *)malloc(strlen(var)+strlen(valor)+2))==NULL)
      return -1;
  strcpy(aux,var);
  strcat(aux,"=");
  strcat(aux,valor);
  e[pos]=aux;
  return (pos);
}

char * NombreUsuario (uid_t uid){
    struct passwd *p;
    if ((p=getpwuid(uid))==NULL)
        return (" ??????");
    return p->pw_name;
}

uid_t UidUsuario (char * nombre){
    struct passwd *p;
    if ((p=getpwnam (nombre))==NULL)
        return (uid_t) -1;
    return p->pw_uid;
}

void MostrarUidsProceso (void){
    uid_t real=getuid(), efec=geteuid();
    printf ("Credencial real: %d, (%s)\n", real, NombreUsuario (real));
    printf ("Credencial efectiva: %d, (%s)\n", efec, NombreUsuario (efec));
}

void CambiarUidLogin (char * login){
    uid_t uid;
    if ((uid=UidUsuario(login))==(uid_t) -1){
        printf("loin no valido: %s\n", login);
        return;
    }
    if (setuid(uid)==-1)
        printf ("Imposible cambiar credencial: %s\n", strerror(errno));
}

int execute(char* parameters[],int replace, int pri, int wait){
    int pid, pid2;
    char** p = parameters;
    pid2= getpid();
    if(replace){
        if(pri)
            setpriority(PRIO_PROCESS,pid2,atoi(parameters[0]));
        if(execvp(parameters[0], &p[0]) == -1){
            perror(RED"No ejecutado"RESET);
            return -1;
        }
    }else if((pid=fork())==0){
        if(pri){
            pid2=getpid();
            setpriority(PRIO_PROCESS,pid2,atoi(parameters[0]));
        }
        if(execvp(parameters[pri], &p[pri])==-1){
            perror(RED"No ejecutado"RESET);
            exit(0);
            return -1;
        }
    }
    if(wait)
        waitpid (pid,NULL,0);
    return pid;
}

int executeVar(char* var[], char* parameters[],int replace, int pri, int wait){
    int pid, pid2;
    pid2= getpid();
    if(replace){
        if(pri)
            if(setpriority(PRIO_PROCESS,pid2,atoi(var[0]))==-1){
                perror(RED"error de prioridad"RESET);
                exit(0); return -1;
            }
        if(OurExecvpe(parameters[0], parameters, &var[pri]) == -1){
            perror(RED"No ejecutado"RESET);
            return -1;
        }
    }else if((pid=fork())==0){
        if(pri){
            pid2=getpid();
            if(setpriority(PRIO_PROCESS,pid2,atoi(var[0]))==-1){
                perror(RED"error de prioridad"RESET);
                exit(0); return -1;
            }
        }
        if(OurExecvpe(parameters[0], parameters, &var[pri])==-1){
            perror(RED"No ejecutado"RESET);
            exit(0); return -1;
        }
    }
    if(wait)
        waitpid (pid,NULL,0);
    return pid;
}

int executeAs(char* parameters[], int wait){
    int pid;
    char** p = parameters;
    if((pid=fork())==0){
        CambiarUidLogin(parameters[0]);
        if(execvp(parameters[1], &p[1]) == -1){
            perror(RED"No ejecutado"RESET);
            exit(0);
            return -1;
        }
    }
    if(wait)
        waitpid (pid,NULL,0);
    return pid;
}

int executeVarAs(char* var[], char* parameters[], int wait){
    int pid;
    if((pid=fork())==0){
        CambiarUidLogin(var[0]);
        printf("%s\n",parameters[0]);
        printf("%s\n",var[0]);
        printf("%s\n",var[1]);
        if(OurExecvpe(parameters[0], parameters,&var[1]) == -1){
            perror(RED"No ejecutado"RESET);
            exit(0);
            return -1;
        }
    }
    if(wait)
        waitpid (pid,NULL,0);
    return pid;
}

int executeAll(char *tokens[],int ntokens, int replace, int pri, int wait){
        char *var[MAX_TOKENS] = {};
        char **tokensAux = tokens;
        int i=0;
        if(pri){
            var[0]=tokens[0]; i=1;
        }
        if(strcmp(tokens[0],"NULLENV")!=0){
            for(;i<ntokens;i++){
                if(BuscarVariable(tokens[i],__environ)==-1)
                    break;
                var[i] = tokens[i];
            }
        }else i=1;
        if(i==0 || (pri && i==1))
            execute(&tokensAux[0],replace,pri,wait);
        else
            executeVar(var,&tokensAux[i],replace,pri,wait);
    return 0; 
}

int executeAllAs(char *tokens[],int ntokens,int wait){
        char *var[MAX_TOKENS] = {};
        char **tokensAux = tokens;
        int i;
        var[0]=tokens[0];
        if(strcmp(tokens[0],"NULLENV")!=0){
            for(i=1;i<ntokens;i++){
                if(BuscarVariable(tokens[i],__environ)==-1)
                    break;
                var[i] = tokens[i];
            }
        }else i=1;
        if(i==1)
            executeAs(&tokensAux[0],wait);
        else
            executeVarAs(var,&tokensAux[i],wait);
    return 0; 
}

int backlist(char *tokens[], int ntokens, int pri, context *ctx){
    int i=0;
    if(ntokens!=0){
        char aux[MAX_LINE] = "";
        time_t t = time(NULL);
        struct job *info = malloc(sizeof(struct job));
        if(pri){
          if(!isNumber(tokens[0])){
            printf("Uso: backpri "RED"priority"RESET" program parameters...\n");
            return -1;
          }
        }
        for(i=pri; i<ntokens; i++){
          strcat(aux, " ");
          strcat(aux, tokens[i]);
        }
        strcpy(info->process, aux);
        info->time = localtime(&t);
        info->uid = getuid();
        strcpy(info->state, "ACTIVO");
        info->out = 0;
        info->pid = execute(tokens,0,pri,0);
        insert(&ctx->jobs, info);
    }
    return 0;
}

int backlistVar(char *var[],char *tokens[], int ntokens, int pri, context *ctx){
    int i=0;
    if(ntokens!=0){
        char aux[MAX_LINE] = "";
        time_t t = time(NULL);
        struct job *info = malloc(sizeof(struct job));
        if(pri){
          if(!isNumber(tokens[0])){
            printf("Uso: backpri "RED"priority"RESET" program parameters...\n");
            return -1;
          }
        }
        for(i=pri; i<ntokens; i++){
          strcat(aux, " ");
          strcat(aux, tokens[i]);
        }
        strcpy(info->process, aux);
        info->time = localtime(&t);
        info->uid = getuid();
        strcpy(info->state, "ACTIVO");
        info->out = 0;
        info->pid = executeVar(var,tokens,0,pri,0);
        insert(&ctx->jobs, info);
    }
    return 0;
}

struct SEN{
    char *nombre;
    int senal;
};

static struct SEN sigstrnum[]={
      "HUP", SIGHUP,
      "INT", SIGINT,
      "QUIT", SIGQUIT,
      "ILL", SIGILL,
      "TRAP", SIGTRAP,
      "ABRT", SIGABRT,
      "IOT", SIGIOT,
      "BUS", SIGBUS,
      "FPE", SIGFPE,
      "KILL", SIGKILL,
      "USR1", SIGUSR1,
      "SEGV", SIGSEGV,
      "USR2", SIGUSR2,
      "PIPE", SIGPIPE,
      "ALRM", SIGALRM,
      "TERM", SIGTERM,
      "CHLD", SIGCHLD,
      "CONT", SIGCONT,
      "STOP", SIGSTOP,
      "TSTP", SIGTSTP,
      "TTIN", SIGTTIN,
      "TTOU", SIGTTOU,
      "URG", SIGURG,
      "XCPU", SIGXCPU,
      "XFSZ", SIGXFSZ,
      "VTALRM", SIGVTALRM,
      "PROF", SIGPROF,
      "WINCH", SIGWINCH,
      "IO", SIGIO,
      "SYS", SIGSYS,
#ifdef SIGPOLL
      "POLL", SIGPOLL,
#endif
#ifdef SIGPWR
      "PWR", SIGPWR,
#endif
#ifdef SIGEMT
      "EMT", SIGEMT,
#endif
#ifdef SIGINFO
      "INFO", SIGINFO,
#endif
#ifdef SIGSTKFLT
      "STKFLT", SIGSTKFLT,
#endif
#ifdef SIGCLD
      "CLD", SIGCLD,
#endif
#ifdef SIGLOST
      "LOST", SIGLOST,
#endif
#ifdef SIGCANCEL
      "CANCEL", SIGCANCEL,
#endif
#ifdef SIGTHAW
      "THAW", SIGTHAW,
#endif
#ifdef SIGFREEZE
      "FREEZE", SIGFREEZE,
#endif
#ifdef SIGLWP
      "LWP", SIGLWP,
#endif
#ifdef SIGWAITING
      "WAITING", SIGWAITING,
#endif
      NULL,-1,
    };

int Senal(char * sen){
    int i;
    for (i=0; sigstrnum[i].nombre!=NULL; i++)
        if (!strcmp(sen, sigstrnum[i].nombre))
            return sigstrnum[i].senal;
    return -1;
}
char *NombreSenal(int sen){
  int i;
  for (i=0; sigstrnum[i].nombre!=NULL; i++)
        if (sen==sigstrnum[i].senal)
            return sigstrnum[i].nombre;
  return ("SIGUNKNOWN");
}

char * Ejecutable (char *s){
    char path[MAX_LINE];
    static char aux2[MAX_LINE];
    struct stat st;
    char *p;
    if (s==NULL || (p=getenv("PATH"))==NULL)
        return s;
    if (s[0]=='/' || !strncmp (s,"./",2) || !strncmp (s,"../",3))
        return s;
    /*is an absolute pathname*/
    strncpy (path, p, MAX_LINE);
    for (p=strtok(path,":"); p!=NULL; p=strtok(NULL,":")){
        sprintf (aux2,"%s/%s",p,s);
        if (lstat(aux2,&st)!=-1)
            return aux2;
    }
    return s;
}
int OurExecvpe(char *file, char *argv[], char *envp[]){
    return (execve(Ejecutable(file),argv, envp));
}

