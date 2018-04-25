#ifndef IAL_H
#define IAL_H

/*
 *	Projekt : Implementace interpretu imperativního jazyka IFJ16
 *	Resitele : xcocul00,xdrdak01,xfaige00,xjaros37, xkucer91
 *	Subor : ial.h
 *	Datum : zari - prosinec 2016
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <malloc.h>

#define zabo unsigned int

/* HASH TABLE */
typedef struct hashItem tabItem;

typedef struct {
	int size;
	tabItem *array[];
}hash_table;


struct hashItem {

	struct hashItem *next;
	hash_table *table;
	bool isfunc;
	char *name;
	int typeData;
	bool defined;
	int param;
	int paramPos;
	int intVal;
	double doubleVal;
	char *stringVal;
	char *blok;
        char *next_param;
};




hash_table *init_HashTable(int size);
void free_HashTable(hash_table *tmp);
tabItem *create_tabItem();
void insert_into_HashTable(hash_table *table, tabItem *item);
void copy_HashTable_Item(tabItem *src, tabItem *dest);
tabItem *search_HashTable(const hash_table *table, const char *item);

/* PROTOTYPY */
void Xprint(const char *s);
int Xlength (const char *s);
char * substr(const char *s, int i, int n);
int compare(const char *s1,const char *s2);
int find (const char *s, const char *search);
char * sort(const char *s);
int readInt();
double readDouble();
char * readString();

#endif
