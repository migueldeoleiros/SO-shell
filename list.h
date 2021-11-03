#ifndef __LIST_H__
#define __LIST_H__

typedef struct node *list;
typedef struct node *pos;

list init_list();
void insert(list *, void *d);
pos first(list l);
pos next(list l, pos p);
void *get(list l, pos p);
int end(list l, pos p); // estoy en el final
int numPos(list l); //cuenta el numero de posiciones
pos findItem(list L, void *d); 
void deleteAtPosition(list* list, pos p, void (*free_data)(void *)); 
void freeList(list *l, void (*free_data)(void *));

#endif
