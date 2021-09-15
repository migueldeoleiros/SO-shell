#ifndef __LIST_H__
#define __LIST_H__

typedef struct node *list;
typedef struct node *pos;

list init_list();
void insert(list *, void *);
pos first(list l);
pos next(list l, pos p);
int end(list l, pos p); // estoy en el final
void *get(list l, pos p);

#endif
