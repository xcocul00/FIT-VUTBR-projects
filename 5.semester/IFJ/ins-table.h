#ifndef INS_TABLE
#define INS_TABLE

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//instructions - enum in scaner.h

typedef struct {
  Tok_type instType;  // typ instrukce
  tabItem *addr1; // adresa 1
  tabItem *addr2; // adresa 2
  tabItem *addr3; // adresa 3
} tInstr;

typedef struct listItem {
  tInstr *Instruction;
  struct listItem *nextItem;
} tListItem;

typedef struct {
  struct listItem *first;  // ukazatel na prvni prvek
  struct listItem *last;   // ukazatel na posledni prvek
  struct listItem *active; // ukazatel na aktivni prvek
} tListOfInstr;

void getInstruction(Tok_type *instType, tabItem **addr1, tabItem **addr2, tabItem **addr3);

void generateInstruction(Tok_type instType, tabItem *addr1, tabItem *addr2, tabItem *addr3);

void insertInstruction(tInstr *I);

void initList();

void disposeList();

void resetActive();

#endif
