#include "headers.h"

int empiezaPor(const char *pre, const char *str){
    if(strncmp(pre, str, strlen(pre)) == 0) return 0;
    return 1;
}

void freeMem(void *ptr) {
    struct memData *mem = ptr;

    free(mem->direccion_bloque);
    free(mem->time);
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

    for(pos p=first(ctx.memory); !end(ctx.memory, p); p=next(ctx.memory, p)) {
        struct memData *info = get(ctx.memory, p);
        strftime(time, MAX_LINE, "%b %d %H:%M ",info->time);
        if (malloc && info->tipo_reserva==0){
            printf("\t%p%12d %s ", info->direccion_bloque, info->tamano_bloque, time);
            printf("malloc\n");
        }else if (mmap && info->tipo_reserva==1){
            printf("\t%p%12d %s ", info->direccion_bloque, info->tamano_bloque, time);
            printf("mmap %s (fd:%d)\n",info->file_name, info->aux);
        }else if (shared && info->tipo_reserva==2){
            printf("\t%p%12d %s ", info->direccion_bloque, info->tamano_bloque, time);
            printf("shared memory (key:%d)\n", info->aux);
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
    struct memData *info = malloc(sizeof(struct memData));

    info->tipo_reserva = 1;
    info->time = localtime(&t);
    info->tamano_bloque = sizeFich(fichero);
    info->direccion_bloque = p;
    info->aux = fd;
    strcpy(info->file_name, fichero);

    insert(&ctx->memory, info);
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
        perror ("Imposible obtener memoria shmget");
    else{
        time_t t = time(NULL);
        struct memData *info = malloc(sizeof(struct memData));

        info->time = localtime(&t);
        info->tipo_reserva = 2;
        info->tamano_bloque = tam;
        info->aux = k;
        info->direccion_bloque = p;

        insert(&ctx->memory, info);
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
        perror ("shmget: imposible obtener memoria compartida");
        return;
    }
    if (shmctl(id,IPC_RMID,NULL)==-1)
        perror ("shmctl: imposible eliminar memoria compartida\n");
}

void dopmap (void){ /*no arguments necessary*/
    pid_t pid;
    /*ejecuta el comando externo pmap para */
    char elpid[32];
    /*pasandole el pid del proceso actual */
    char *argv[3]={"pmap",elpid,NULL};
        sprintf (elpid,"%d", (int) getpid());
    if ((pid=fork())==-1){
        perror ("Imposible crear proceso");
        return;
    }
    if (pid==0){
        if (execvp(argv[0],argv)==-1)
            perror("cannot execute pmap");
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
