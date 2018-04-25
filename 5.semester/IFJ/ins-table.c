#include "ins-table.h"

tListOfInstr *list;
jumpPtr *jumpArray;


/*
** @function getInstruction()
** @description
**/
void getInstruction (Tok_type *instType, tabItem **addr1, tabItem **addr2, tabItem **addr3) {
    if (list->active != NULL) {
        *instType = list->active->Instruction->instType;
        *addr1 = list->active->Instruction->addr1;
        *addr2 = list->active->Instruction->addr2;
        *addr3 = list->active->Instruction->addr3;
        list->active = list->active->nextItem;
    } else {
        *instType = END_OF_FILE;
    }
}

/*
** @function generateInstruction()
** @description
**/
void generateInstruction(Tok_type instType, tabItem *addr1, tabItem *addr2, tabItem *addr3) {
     //vlozi novou instrukci do seznamu instrukci
    tInstr *I;
    if((I=malloc(sizeof(tInstr)))==NULL){
        //return NULL;
    }
    I->instType = instType;
    I->addr1 = addr1;
    I->addr2 = addr2;
    I->addr3 = addr3;
    // ++ funkce na vlozeni instrukce na zacatek seznamu

    insertInstruction(I);
}

/*
** @function insertInstruction()
** @description
**/
void insertInstruction(tInstr *I) {
    tListItem *Item;
    if((Item=malloc(sizeof(tListItem)))==NULL){
        //return NULL;
    }
    Item->Instruction = I;
    if(list->first == NULL) {
        // Ak je zoznam prazdny, vlozi INST na zacatek a first,last,
        // active ukazuju na tento prvek
        Item->nextItem = NULL;
        list->first = list->last = list->active = Item;
    } else {
        // Ak v nom uz nieco je, vlozi INST na zacatek, INST->nextItem
        // bude ukazovat na predosly prvy prvek a pak budou
        // first a active ukazovat na nvoy INST
        Item->nextItem = NULL;
        list->last->nextItem = Item;
        list->last = Item;
    }
}

/*
** @function initList()
** @description
**/
void initList() {
    if((list=malloc(sizeof(tListOfInstr)))==NULL){
       //return NULL;
    }
    list->first = list->active = list->last = NULL;
}

/*
** @function disposeList()
** @description
**/
void disposeList() {
    if (list->first != NULL) {
        list->active = list->first->nextItem;
        while (list->active != NULL) {
            free(list->first->Instruction);
            free(list->first);
            list->first = list->active;
            list->active = list->first->nextItem;
        }
    }
    free(list->last->Instruction);
    free(list->last);
    free(list);
}

/*
** @function resetActive()
** @description
**/
void resetActive() {
    list->active = list->first;
}
