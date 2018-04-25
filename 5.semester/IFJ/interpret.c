
#include "interpret.h"

int count= 0;
tListOfInstr *list; // globalni promenna uchovavajici seznam instrukci
int jumpCount = 0;
jumpPtr *jumpArray;

/*TODO         COMMENTS kek
               ADD string v pripade rovnakych VAR - DONE
               */

//fukcia na kontrolu premennych v operandoch
int dataTypeCtrl(int dT1, int dT2, int iT){
   if((MACATY_STRING(dT1) || MACATY_STRING(dT2)) && (iT != TYPE_ADD && iT != TYPE_ASSIG)){
      ERR_PRINT(ERR_SEM_TYPE,"Nepovolena kombinace operace a datovych typu");
   }
   else if((MACATY_STRING(dT1) || MACATY_STRING(dT2)) && iT == TYPE_ADD) {
      if(MACATY_STRING(dT1) && !MACATY_STRING(dT2))
         return dT1_STRING;
      else if(!MACATY_STRING(dT1) && MACATY_STRING(dT2))
         return dT2_STRING;
      return TYPE_STRING_DATA;
   }
   else if((MACATY_STRING(dT1) || MACATY_STRING(dT2)) && iT == TYPE_ASSIG){
      if(MACATY_STRING(dT1) && !MACATY_STRING(dT2))
         ERR_PRINT(ERR_SEM_TYPE,"Nepovolena kombinace datovych typu");

      else if(!MACATY_STRING(dT1) && MACATY_STRING(dT2))
         ERR_PRINT(ERR_SEM_TYPE,"Nepovolena kombinace datovych typu");

      else
         return TYPE_STRING_DATA;

   }
   else if (MACATY_INTEGER(dT1) && MACATY_INTEGER(dT2))
      return TYPE_INT_DATA;
   else if(MACATY_DOUBLE(dT1) && MACATY_INTEGER(dT2))
      return TYPE_DOUBLE_DATA;
   else if(MACATY_INTEGER(dT1) && MACATY_DOUBLE(dT2))
      return INT_RECAST;
   else if(MACATY_DOUBLE(dT1) && MACATY_INTEGER(dT2))
      return INT2_RECAST;
   else
    // printf("%d %d", dT1, dT2);
      ERR_PRINT(ERR_SEM_TYPE,"Nezname kombinace datovych typu");
   return 0;
}

tabItem *searchRamecVar(tRamecStack RamStack, const char *varName){
    for (int i = 0; i < RamStack.top; i++) {
        tabItem *ptr = search_HashTable(RamStack.ptr[RamStack.top - i], varName);
        if (ptr != NULL) {
            return ptr;
        }
    }
    return NULL;
}


/*
** @function interpret()
** @description
*/
int interpret(tReturnStack RetStack, tRamecStack RamStack, tRetAddrStack RetAddrStack) {
    Tok_type instType;
    tabItem *addr1;
    tabItem *addr2;
    tabItem *addr3;

    getInstruction(&instType, &addr1, &addr2, &addr3);

    hash_table *ptr = init_HashTable(100);

    while (instType != END_OF_FILE) {

      hash_table *ramec;
      ramecSTop(&RamStack, &ramec);
      if(ramec != NULL){
         tabItem *var;
         if (addr1 != NULL) {
            if(addr1->name != NULL){
                    var = searchRamecVar(RamStack, addr1->name);
           
                 if(var != NULL ){
                     addr1 = var;
                 }
            }
         }
         if (addr2 != NULL) {
              if(addr2->name != NULL){
                    var = searchRamecVar(RamStack, addr2->name);
           
                 if(var != NULL ){
                     addr2 = var;
                 }
            }
         }
         if (addr3 != NULL) {
             if(addr3->name != NULL){
                    var = searchRamecVar(RamStack, addr3->name);
           
                 if(var != NULL ){
                     addr3 = var;
                 }
            }
         }
      }
        switch (instType) {

            //pretypovanie pre vsetky standardne operatory - setrime riadky
            case TYPE_ADD:
            case TYPE_SUB:
            case TYPE_MUL:
            case TYPE_DIV:
            case TYPE_LESS:
            case TYPE_GREAT:
            case TYPE_LESS_OR_EQ:
            case TYPE_GREAT_OR_EQ:
            case TYPE_EQUAL:
            case TYPE_NOT_EQUAL:
            case TYPE_ASSIG:   ;
                int recast = dataTypeCtrl(addr1->typeData, addr2->typeData, instType);
                switch (recast){
                  case TYPE_INT_DATA:
                     //printf("NO CONVERSION NEEDED INT\n");
                     break;
                  case TYPE_DOUBLE_DATA:
                     //printf("NO CONVERSION NEEDED DBL\n");
                     break;
                  case INT_RECAST:
                     addr1->doubleVal = (double)addr1->intVal;
                     //printf("CONVERSION NEEDED INT1\n");
                     break;
                  case INT2_RECAST:
                     //printf("CONVERSION NEEDED INT2\n");
                     addr2->doubleVal = (double)addr2->intVal;
                     break;
                  default:
                     break;
               }
                //ADD, SUB, MUL a DIV
                switch (instType){
                    case TYPE_ADD:
                        if(recast == TYPE_INT_DATA){
                            addr3->intVal = addr1->intVal + addr2->intVal;
                            // printf("TYPE_ADD = %d\n", addr3->intVal);

                        } else if(recast == TYPE_STRING_DATA){
                            char *temp_str;
                            if((temp_str = malloc(strlen(addr1->stringVal)+strlen(addr2->stringVal)+1)) != NULL){

                                strcat(temp_str,addr1->stringVal);
                                strcat(temp_str,addr2->stringVal);
                                addr3->stringVal = malloc(strlen(addr1->stringVal)+strlen(addr2->stringVal)+1);
                                addr3->stringVal[0] = '\0';

                                strcpy(addr3->stringVal, temp_str);
                                free((temp_str));
                            }

                        } else if(recast == dT1_STRING){
                            addr2->stringVal = malloc(100*sizeof(char));
                            if(addr2->typeData == TYPE_INT_DATA){
                                snprintf(addr2->stringVal, 100, "%d", addr2->intVal);
                            } else {
                                snprintf(addr2->stringVal, 100, "%g", addr2->doubleVal);
                            }

                            char *temp_str;
                            if((temp_str = malloc(strlen(addr1->stringVal)+strlen(addr2->stringVal)+1)) != NULL){

                                strcat(temp_str,addr1->stringVal);
                                strcat(temp_str,addr2->stringVal);                                
                                addr3->stringVal = malloc(strlen(addr1->stringVal)+strlen(addr2->stringVal)+1);
                                addr3->stringVal[0] = '\0';

                                strcpy(addr3->stringVal, temp_str);
                                free((temp_str));
                            }
                            free(addr2->stringVal);
                        } else if(recast == dT2_STRING){
                            addr1->stringVal = malloc(100*sizeof(char));
                            if(addr1->typeData == TYPE_INT_DATA){
                                snprintf(addr1->stringVal, 100, "%d", addr1->intVal);
                            } else {
                                snprintf(addr1->stringVal, 100, "%g", addr1->doubleVal);
                            }

                            char *temp_str;

                            if((temp_str = malloc(strlen(addr1->stringVal)+strlen(addr2->stringVal)+1)) != NULL){

                                strcat(temp_str,addr1->stringVal);
                                strcat(temp_str,addr2->stringVal);
                                addr3->stringVal = malloc(strlen(addr1->stringVal)+strlen(addr2->stringVal)+1);
                                addr3->stringVal[0] = '\0';

                                strcpy(addr3->stringVal, temp_str);
                                free((temp_str));
                            }
                            free(addr1->stringVal);
                        } else if((recast == TYPE_DOUBLE_DATA) || (recast == INT_RECAST) || (recast == INT2_RECAST)){
                            addr3->doubleVal = addr1->doubleVal + addr2->doubleVal;
                            // printf("TYPE_ADD = %f\n", addr3->doubleVal);
                        }
                        break;


                    case TYPE_SUB:
                        if(recast == TYPE_INT_DATA){
                            addr3->intVal = addr1->intVal - addr2->intVal;
                            // printf("TYPE_SUB = %d\n", addr3->intVal);
                        } else {
                            addr3->doubleVal = addr1->doubleVal - addr2->doubleVal;
                            // printf("TYPE_SUB = %f\n", addr3->doubleVal);
                        }
                        break;


                    case TYPE_MUL:
                        if(recast == TYPE_INT_DATA){
                            addr3->intVal = addr1->intVal * addr2->intVal;
                            // printf("TYPE_MUL = %d\n", addr3->intVal);
                        } else {
                            addr3->doubleVal = addr1->doubleVal * addr2->doubleVal;
                            // printf("TYPE_MUL = %f\n", addr3->doubleVal);
                        }
                        break;


                    case TYPE_DIV:
                        if(recast == TYPE_INT_DATA){
                            addr3->intVal = addr1->intVal / addr2->intVal;
                            // printf("TYPE_DIV = %d\n", addr3->intVal);
                        } else {
                            addr3->doubleVal = addr1->doubleVal / addr2->doubleVal;
                            // printf("TYPE_DIV = %f\n", addr3->doubleVal);
                        }
                        break;


                    case TYPE_LESS:
                        if (recast == TYPE_INT_DATA) {
                            if (addr1->intVal < addr2->intVal)
                                addr3->intVal = true;
                            else
                                addr3->intVal = false;
                        } else if (recast == TYPE_STRING_DATA) {
                            if (compare(addr1->stringVal, addr2->stringVal) == -1)
                                addr3->intVal = true;
                            else
                                addr3->intVal = false;
                        } else {
                            if (addr1->doubleVal < addr2->doubleVal)
                                addr3->intVal = true;
                            else
                                addr3->intVal = false;
                        }
                        break;


                    case TYPE_GREAT:
                        if (recast == TYPE_INT_DATA) {
                            if (addr1->intVal > addr2->intVal)
                                addr3->intVal = true;
                            else
                                addr3->intVal = false;
                        } else if (recast == TYPE_STRING_DATA) {
                            if (compare(addr1->stringVal, addr2->stringVal) == 1)
                                addr3->intVal = true;
                            else
                                addr3->intVal = false;
                        } else {
                            if (addr1->doubleVal > addr2->doubleVal)
                                addr3->intVal = true;
                            else
                                addr3->intVal = false;
                        }
                        break;


                    case TYPE_LESS_OR_EQ:
                        if (recast == TYPE_INT_DATA) {
                            if (addr1->intVal <= addr2->intVal)
                                addr3->intVal = true;
                            else
                                addr3->intVal = false;
                        } else if (recast == TYPE_STRING_DATA) {
                            if (compare(addr1->stringVal, addr2->stringVal) < 1)
                                addr3->intVal = true;
                            else
                                addr3->intVal = false;
                        } else {
                            if (addr1->doubleVal <= addr2->doubleVal)
                                addr3->intVal = true;
                            else
                                addr3->intVal = false;
                        }
                        break;


                    case TYPE_GREAT_OR_EQ:
                        if (recast == TYPE_INT_DATA) {
                            if (addr1->intVal >= addr2->intVal)
                                addr3->intVal = true;
                            else
                                addr3->intVal = false;
                        } else if (recast == TYPE_STRING_DATA) {
                            if (compare(addr1->stringVal, addr2->stringVal)  >= 0 )
                                addr3->intVal = true;
                            else
                                addr3->intVal = false;
                        } else {
                            if (addr1->doubleVal <= addr2->doubleVal)
                                addr3->intVal = true;
                            else
                                addr3->intVal = false;
                        }
                        break;


                    case TYPE_EQUAL:
                    if (recast == TYPE_INT_DATA) {
                            if (addr1->intVal == addr2->intVal)
                                addr3->intVal = true;
                            else
                                addr3->intVal = false;
                        } else if (recast == TYPE_STRING_DATA) {
                            if (compare(addr1->stringVal, addr2->stringVal) == 0)
                                addr3->intVal = true;
                            else
                                addr3->intVal = false;
                        } else {
                            if (addr1->doubleVal == addr2->doubleVal)
                                addr3->intVal = true;
                            else
                                addr3->intVal = false;
                        }
                        break;


                    case TYPE_NOT_EQUAL:
                        if (recast == TYPE_INT_DATA) {
                            if (addr1->intVal != addr2->intVal)
                                addr3->intVal = true;
                            else
                                addr3->intVal = false;
                        } else if (recast == TYPE_STRING_DATA) {
                            if (compare(addr1->stringVal, addr2->stringVal) != 0)
                                addr3->intVal = true;
                            else
                                addr3->intVal = false;
                        } else {
                            if (addr1->doubleVal != addr2->doubleVal)
                                addr3->intVal = true;
                            else
                                addr3->intVal = false;
                        }
                        break;

                    case TYPE_ASSIG:

                        if (recast == TYPE_STRING_DATA || recast == TYPE_STRING){
                            addr1->stringVal = addr2->stringVal;
                        } else if (recast == TYPE_INT_DATA || recast == TYPE_INT)
                            addr1->intVal = addr2->intVal;
                        else if (recast == TYPE_DOUBLE_DATA || recast == TYPE_DOUBLE)
                            addr1->doubleVal = addr2->doubleVal;
                        else if (recast == INT_RECAST )
                            addr1->intVal = (int)addr2->doubleVal;
                        else if (recast == INT2_RECAST)
                            addr1->doubleVal = (double)addr2->intVal;
                        break;

                    default:
                        break;
                }
                break;

            case INT_READ_INT: ;
                if( addr1->typeData == TYPE_INT){
                   addr1->intVal = readInt(); 
                }
                if( addr1->typeData == TYPE_DOUBLE){
                   addr1->doubleVal = readInt(); 
                }
                
                break;

            case DOUBLE_READ_DOUBLE: ;
                addr1->doubleVal = readDouble();
                break;

            case STRING_READ_STRING: ;
                 addr1->stringVal = readString();
                break;

            case VOID_PRINT:
                if((addr1->typeData == TYPE_INT)||(addr1->typeData == TYPE_INT_DATA))
                    printf("%d", addr1->intVal);
                else if((addr1->typeData == TYPE_DOUBLE)||(addr1->typeData == TYPE_DOUBLE_DATA))
                    printf("%g", addr1->doubleVal);
                else if((addr1->typeData == TYPE_STRING_DATA)||(addr1->typeData == TYPE_STRING))
                    printf("%s", addr1->stringVal);
                break;

            case INT_LENGTH:
                addr1->intVal = Xlength(addr2->stringVal);
                break;

            case STRING_SUBSTR: ;
                tabItem *argument;
                tabItem *argument2;
                tabItem *argument3;
                argument  = addr1;
                argument2 = addr2;
                argument3 = addr3;
                getInstruction(&instType, &addr1, &addr2, &addr3);
                // printf("%s\n",argument->stringVal );
                // printf("%d\n", argument2->intVal);
                //  printf("%d\n", argument3->intVal);
                addr1->stringVal = substr(argument->stringVal, argument2->intVal, argument3->intVal);
                break;

            case INT_COMPARE:
                    // printf("*********************COMP\n");
                addr1->intVal = compare(addr2->stringVal, addr3->stringVal);
                // printf("%d\n",addr1->intVal );
                break;

            case INT_FIND:
                // printf("1 :%s 2:%s\n",addr2->stringVal,addr3->stringVal );
                addr1->intVal = find(addr2->stringVal, addr3->stringVal);
                // printf("%d\n", addr1->intVal);
                break;

            case STRING_SORT:
            // printf("asdasdasd\n");
                addr1->stringVal = sort(addr2->stringVal);
                // printf("%s\n",addr1->stringVal );
                break;

            case TYPE_JUMP:
                returnSPush(&RetStack, list->active);
                retAddrSPush(&RetAddrStack, addr2);
                jump(addr1);
                // printf("jump to func %s \n", addr1->name);
                break;

            case TYPE_IF:
                if(addr1->intVal == true){ //addr1 - podmienka, ak je rovna 1, true -> JUMP
                    //vyhlada ramec kam chceme skocit v zasobniku
                    jump(addr2);
                }
                //ak je podmienka nerovna 1 => false a addr3 je neprazdna => ELSE
                else if((addr1->intVal != true) && (addr3->stringVal != NULL)) {
                    jump(addr3);
                }
                break;


            case TYPE_RETURN:
                if(!strcmp(addr1->name, "run")) {
                    // printf("konec runu");
                    return 0;
                }

                if (RetStack.top != 0) {
                    returnSTop( &RetStack, &list->active);
                    returnSPop( &RetStack);

                    if (RetAddrStack.top != 0) {
                        tabItem *ptr;
                        retAddrSTop(&RetAddrStack, &ptr);
                        if( ptr != NULL || addr1 != NULL) {
                            if((addr1->typeData == TYPE_INT)||(addr1->typeData == TYPE_INT_DATA))
                                ptr->intVal = addr1->intVal;
                            else if((addr1->typeData == TYPE_DOUBLE)||(addr1->typeData == TYPE_DOUBLE_DATA))
                                ptr->doubleVal = addr1->doubleVal;
                            else if((addr1->typeData == TYPE_STRING_DATA)||(addr1->typeData == TYPE_STRING)){

                                strcpy(ptr->stringVal, addr1->stringVal);
                            }
                        }
                    }
                }
                break;


            case TYPE_CLASS:
                ptr = addr1->table;
                ramecSPush(&RamStack, ptr);
                break;

            case I_RAMEC:
                ptr = addr1->table;
                ramecSPush(&RamStack, ptr);
                int paramCount = addr1->param;
                for (int i = 1; i < (paramCount); i++) {
                    getInstruction(&instType, &addr1, &addr2, &addr3);
                    // spracovani parametru funkce
                }

                break;
            default:
                break;
        }
        getInstruction(&instType, &addr1, &addr2, &addr3);
    }
    return 0;
}



/*
 * @brief      Prechadza sa cela instrukcni paska, a pokud se najde RAMEC, jeho ukazatel se ulozi (volani insertJump())
 *
 * @param      label  The label
 */
void initJumpArray() {
    jumpCount = 0;
    struct listItem *ptr = list->first;
    while (ptr != NULL) {
        if (ptr->Instruction->instType == I_RAMEC) {
            insertJump(ptr);
        }
        ptr = ptr->nextItem;
    }
}

/*
 * @brief      Vlozi ukazatel do jumpArray na index jumpCount, inkrementuje jumpCount
 *
 * @param      label  ukazatel na instrukci typu I_RAMEC na kterou bude mozno skakat
 */
void insertJump(struct listItem *label) {
    jumpArray = realloc(jumpArray, sizeof(jumpPtr)*(jumpCount+1));
    jumpArray[jumpCount++].ptr = label;
}

/*
 * @brief      Prijima odkaz na tabItem, label , potom prehladava jumpArray a porovnava mena
 *             ulozenych instrukci s retazcom premennej label
 * @param      label  The label
 */
void jump(tabItem *label) {
    for (int i = 0; i < jumpCount; i++) {
        if (!strcmp(jumpArray[i].ptr->Instruction->addr1->name, label->name)) {
            list->active = jumpArray[i].ptr; //a nasledne presunie aktivnu instrukciu
            break;
        }
    }
}

/**
 * @brief      Funkcie pre pracu so zasobnikom s return ukazatelmi
 *
 * @param      s     Zasobnik 
 */
void returnSInit ( tReturnStack* s ) {
    s->top = 0;
    s->ptr[0] = NULL;
}

void returnSTop ( const tReturnStack* s, struct listItem **ptr ) {
    (*ptr) = s->ptr[s->top];
}

void returnSPop ( tReturnStack* s ) {
    if(s->top != 0)
        s->top--;
}

void returnSPush ( tReturnStack* s, struct listItem *ptr ) {
    s->ptr[++s->top] = ptr;
}

/*Funkcie pre pracu so zasobnikom s ukazatelmi na tabulky ramcov*/
void ramecSInit ( tRamecStack* s ) {
    s->top = 0;
    s->ptr[0] = NULL;
}

void ramecSTop ( const tRamecStack* s, hash_table **ptr ) {
    (*ptr) = s->ptr[s->top];
}

void ramecSPop ( tRamecStack* s ) {
   if(s->top != 0)
      s->top--;
}

void ramecSPush ( tRamecStack* s, hash_table *ptr ) {
    s->ptr[++s->top] = ptr;
}

void retAddrSInit ( tRetAddrStack* s ) {
    s->top = 0;
    s->ptr[0] = NULL;
}

void retAddrSTop ( const tRetAddrStack* s, tabItem **ptr ) {
    (*ptr) = s->ptr[s->top];
}

void retAddrSPop ( tRetAddrStack* s ) {
    if(s->top != 0)
        s->top--;
}

void retAddrSPush ( tRetAddrStack* s, tabItem *ptr ) {
    s->ptr[++s->top] = ptr;
}
