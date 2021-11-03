#include "headers.h"

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