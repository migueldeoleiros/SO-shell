// TITLE: Sistemas Operativos Práctica 1 
// AUTHOR 1: Miguel López López        LOGIN 1: m.llopez 
// AUTHOR 2: Xoel Díaz Préstamo        LOGIN 2: xoel.diaz                  
// GROUP: 2.1.4 
// DATE: 24/09/2021 

#include "headers.h"
#include "list.h"


list lista; //crear lista
int countHist = 0; //crear contador para historial


int autores(char *tokens[], int ntokens) {
    if(tokens[0] != NULL){
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

int pid(char *tokens[], int ntokens) {
    int pid, p_pid;

    pid = getpid();
    p_pid = getppid();
    
    if(tokens[0] != NULL){
        if (strcmp(tokens[0], "-p") == 0){
            printf("Pid del padre del shell: %d\n", p_pid);
        }
    }else {
        printf("Pid de shell: %d\n", pid);

    }
    return 0;
}

int carpeta(char *tokens[], int ntokens) {
    if(tokens[0] != NULL){
        chdir(tokens[0]);

    }else {
        char dir [MAX_LINE];
        getcwd(dir, sizeof(dir));
        printf("%s\n", dir);

    }
    return 0;
}

int fecha(char *tokens[], int ntokens) {
    struct tm* fecha;
    time_t t;
    char fechaOut [MAX_LINE];
    char timeOut [MAX_LINE];
    t = time(NULL);
    fecha = localtime(&t);

    if(tokens[0] != NULL){
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

int infosis(char *tokens[], int ntokens) {
    struct utsname sys;

    if (!uname(&sys)){
       printf("%s (%s), OS: %s-%s-%s \n",
               sys.nodename, sys.machine, sys.sysname, sys.release, sys.version);
    }

    return 0;
}

int ayuda(char *tokens[], int ntokens) {
    if(tokens[0] != NULL){
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

int hist(char *tokens[], int ntokens) {

    if(tokens[0] != NULL){
        if (strcmp(tokens[0], "-c") == 0){
            borrar(lista);
            lista = init_list();
            countHist = 0;
        }else if (isNumber(tokens[0])){
            int num = abs(atoi(tokens[0]));

            pos pos=first(lista);
            struct data *info = get(lista, pos);

            while(num != info->num && !end(lista, pos)) {
                printf("%d-> %s", info->num, info->cmd);
                pos=next(lista, pos);
                info = get(lista, pos);
            }
        }
    }else {
        for(pos p=first(lista); !end(lista, p); p=next(lista, p)) {
            struct data *d = get(lista, p);
            printf("%d-> %s", d->num, d->cmd);
        }
    }
    return 0;
}

int comando(char *tokens[], int ntokens) {
    if(tokens[0] != NULL){
        if (isNumber(tokens[0])){
            int num = atoi(tokens[0]);

            pos pos=first(lista);
            struct data *info = get(lista, pos);
            int final= numPos(lista);

            if (num > final){
                printf("No existe comando en esa posición\n");

            }else{
                while(num != info->num) {
                    pos=next(lista, pos);
                    info = get(lista, pos);
                }
                printf("Ejecutando hist (%d): %s ", info->num, info->cmd);
                leerEntrada( 0, info->cmd);
            }
        }

    }else {
        hist(tokens, ntokens);
    }
    return 0;
}

int salir(char *tokens[], int ntokens) {
    return 1;
}

int empiezaPor(const char *pre, const char *str){
    if(strncmp(pre, str, strlen(pre)) == 0) return 0;
    return 1;
}

int trocearCadena(char * str, char * tokens[]){ int i=1;

    if ((tokens[0]=strtok(str," \n\t"))==NULL)
        return 0;
    while ((tokens[i]=strtok(NULL," \n\t"))!=NULL)
        i++;

    return i;
}

int process(char *tokens[], int ntokens) {
    for(int i=0; cmds[i].cmd_name != NULL; i++) {
        if(strcmp(tokens[0], cmds[i].cmd_name) ==0) {
            return cmds[i].cmd_fun(tokens+1, ntokens-1);
        }
    }

    printf("comando no encontrado\n");
    return 0;
}

int imprimirPrompt(char *line){
    printf("✦»> ");
    fgets(line, MAX_LINE, stdin);

    return 0;
}

int leerEntrada(int end, char *line){
    char *tokens[MAX_TOKENS];
    int ntokens;

    if(empiezaPor("comando", line) != 0){
        struct data *info = malloc(sizeof(struct data));
        info->num = countHist;
        strcpy(info->cmd, line);
        insert(&lista , info);
        countHist++;
    }

    ntokens = trocearCadena(line, tokens);
    end = process(tokens, ntokens);
    
    return end;
}

int main() {
    char line [MAX_LINE];
    int end=0;

    lista = init_list();

    while(!end) {
        imprimirPrompt(line);
        if (line[0]!='\n'){
            end = leerEntrada(end, line);
        }
    }
}
