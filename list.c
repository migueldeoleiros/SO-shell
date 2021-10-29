#include <stdlib.h>
#include <string.h>

struct node {
    void *data;
    struct node *next;
};

#include "list.h"

list init_list() {
    return NULL;
}

int createNode(pos *p) {
  *p = malloc(sizeof(**p));
  return *p != NULL;
}

void insert(list *l, void *data){
  pos item, aux;

  if(createNode(&item)){
  item->data = data;
  item->next = NULL;
  if(*l == NULL){
    *l = item;
  }else{
    for (aux = *l; aux->next != NULL; aux = aux->next); //nos movemos al final de la lista
    aux->next = item;
  }
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

int numPos(list l){ //numero total de posiciones
    int i;
    pos p=l;
    for(i=0; next(l,p)!= NULL; i++ )
        p=next(l,p);
    return i;
}

void freeList(list *l, void (*free_data)(void *)) {
    struct node *n, *aux;
    n = *l;

    while(n != NULL) {
        aux = n;
        free_data(n->data);
        n = n -> next;
        free(aux);
    }
    *l = NULL;
}
