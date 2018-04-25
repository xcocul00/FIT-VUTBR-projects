#ifndef INTERPRET_H_INCLUDED
#define INTERPRET_H_INCLUDED

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ial.h"
#include "ins-table.h"

#define MACATY_STRING(X)  ((X == TYPE_STRING_DATA ) || (X == TYPE_STRING))
#define MACATY_INTEGER(X) ((X == TYPE_INT_DATA    ) || (X == TYPE_INT))
#define MACATY_DOUBLE(X)  ((X == TYPE_DOUBLE_DATA ) || (X == TYPE_DOUBLE))

#define INT_RECAST   4
#define INT2_RECAST  5

#define dT1_STRING   6
#define dT2_STRING   7

#define true   1
#define false  0

#define MAX_RETURN_STACK 30

#define MAX_RAMEC_STACK 30

#define MAX_RETADDR_STACK 30

typedef struct {
   struct listItem *ptr;
} jumpPtr;

extern int jumpCount;


/* Zasobnik pro ukladani return adres
 * t.j. kam sa ma po skoku vratit kdyz najde instrukci ~ I_RETURN
 */
typedef struct {
   struct listItem *ptr[MAX_RETURN_STACK];
   int top;   /* index prvku na vrcholu zásobníku */
} tReturnStack;

typedef struct {
   hash_table *ptr[MAX_RAMEC_STACK];
   int top;   /* index prvku na vrcholu zásobníku */
} tRamecStack;

typedef struct {
   tabItem *ptr[MAX_RETADDR_STACK];
   int top;
} tRetAddrStack;

int dataTypeCtrl(int dT1, int dT2, int iT);
int interpret(tReturnStack RetStack, tRamecStack RamStack, tRetAddrStack RetAddrStack);


void initJumpArray();
void insertJump(struct listItem *label);
void jump(tabItem *label);

void returnSInit ( tReturnStack* s );
void returnSTop ( const tReturnStack* s, struct listItem **ptr );
void returnSPop ( tReturnStack* s );
void returnSPush ( tReturnStack* s, struct listItem *ptr );

void ramecSInit ( tRamecStack* s );
void ramecSTop ( const tRamecStack* s, hash_table **ptr );
void ramecSPop ( tRamecStack* s );
void ramecSPush ( tRamecStack* s, hash_table *ptr );

void retAddrSInit ( tRetAddrStack* s );
void retAddrSTop ( const tRetAddrStack* s, tabItem **ptr );
void retAddrSPop ( tRetAddrStack* s );
void retAddrSPush ( tRetAddrStack* s, tabItem *ptr );

#endif // INTERPRET_H_INCLUDED
