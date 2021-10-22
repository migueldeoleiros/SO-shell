#include <stdlib.h>
#include <string.h>

struct node {
    char data[1024];
    struct node *next;
};

#include "list.h"

list init_list() {
    return NULL;
}

void insert(list *l, char *data) {
    if(*l == NULL) { // lista vacia
        *l = malloc(sizeof(struct node));
        (*l)->next = NULL;
        strcpy((*l)->data , data);
    } else {
        struct node *aux = *l;
        while(aux->next != NULL)
            aux = aux->next;
        aux->next = malloc(sizeof(struct node));
        aux->next->next = NULL;
        strcpy(aux->next->data , data);
    }
}

pos first(list l) {
    return l;
}

pos next(list l, pos p) {
    if(p==NULL) return NULL;
    return p->next;
}

int end(list l, pos p) { // estoy en el final
    return (p==NULL);
}

void *get(list l, pos p) {
    if(p==NULL) return NULL;
    return p->data;
}

void clean(list l) { //borrar la lista
    pos pos;

    while (l != NULL) {
        pos = l;
        l = l->next;
        free(pos); //liberar memoria
    }
}

int numPos(list l){ //numero total de posiciones
    int i;
    pos p=l;
    for(i=0; next(l,p)!= NULL; i++ )
        p=next(l,p);
    return i;
}


