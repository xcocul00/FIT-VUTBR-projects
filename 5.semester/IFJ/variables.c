#include "variables.h"

hash_table *TbSmHash;


int counterVar = 1;
/* 
*	@function generateVariable
*	@param string *var
*	@description Generuje identifikator promenne ve tvaru /^\$[0-9]+$/ 
*/
int generateVariable(string *var) {
	strClear(var);
	if(strAddChar(var, '$'))
		return ERR_INSIDE;
  
	int i = counterVar;
	
	while (i) {
	    if (strAddChar(var, (char)(i % 10 + '0')))
	        return ERR_INSIDE;
	    i /= 10;
  	}
  	
	counterVar ++;
	return 0;
}


/* 
*	@function insertVarInt
*	@param int var
*	@param tabItem ** newVariableInfo
*	@description
*/
int insertVarInt(int var, tabItem ** newVariableInfo) {
	
    string newVar;
    if (strInit(&newVar))
        return ERR_INSIDE;
 	// printf("1x??\n");
	if (generateVariable(&newVar))
        return ERR_INSIDE;
 // printf("2x??\n");
 	tabItem *item = create_tabItem();
 	
 	item->name 		= strGetStr(&newVar);
 	item->typeData	= TYPE_INT_DATA;
 	item->intVal	= var;
 	// printf("%s\n",item->name);


    insert_into_HashTable(TbSmHash, item);
		// printf("xxxx??\n");
	strFree(&newVar);
	printf("4x??\n"); 	
	return 0;
}


/* 
*	@function insertVarDbl
*	@param int var
*	@param tData ** newVariableInfo
*	@description
*/
int insertVarDbl(double var, tabItem ** newVariableInfo) {
	
    string newVar;
    if (strInit(&newVar))
        return ERR_INSIDE;
 	
	if (generateVariable(&newVar))
        return ERR_INSIDE;
 
 	tabItem *item = create_tabItem();;
 	
 	item->name 		= strGetStr(&newVar);
 	item->typeData	= TYPE_DOUBLE_DATA;
 	item->doubleVal	= var;
 	
    insert_into_HashTable(TbSmHash, item);

	strFree(&newVar);
	 	
	return 0;
}
 
 
/* 
*	@function insertVarStr
*	@param int var
*	@param tData ** newVariableInfo
*	@description
*/
int insertVarStr(char * str, tabItem ** newVariableInfo) {
	
	string newVar;
    if (strInit(&newVar))
        return ERR_INSIDE;
 	
	if (generateVariable(&newVar))
        return ERR_INSIDE;
 
 	tabItem *item = create_tabItem();;
 	
 	item->name 		= strGetStr(&newVar);
 	item->typeData	= TYPE_STRING_DATA;
 	
 	strcpy(item->stringVal, str);
 	
    insert_into_HashTable(TbSmHash, item);

	strFree(&newVar);
	 	
	return 0;
}
