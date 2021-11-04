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

pos findItem(list L, void *d){
    pos p;
    for(p=L;((p!=NULL)&&(p->data != d));p=p->next);
    if(p!=NULL&&(p->data == d))
        return p;
    else
        return NULL;
}

void deleteAtPosition(list* list, pos p, void (*free_data)(void *)) {
    pos i;

    if(p == *list) {
        *list = (*list)->next;
    }else if(p->next == NULL) {
        for (i = *list; i->next != p; i = i->next);
        i->next = NULL;
    }else {
        i = p->next;
        p->data = i->data;
        p->next = i->next;
        p = i;
    }
    free_data(p);
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
