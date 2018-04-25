
/*
 *	Projekt : Implementace interpretu imperativního jazyka IFJ16
 *	Resitele : xcocul00,xdrdak01,xfaige00,xjaros37, xkucer91
 *	Subor : ial.c
 *	Datum : zari - prosinec 2016
 *
 */
#include "ial.h"

/* VSTAVANE FUNKCIE */
/*
 * funkcia vrati dlzku stringu
 */
int Xlength (const char *s){
	return (int)strlen(s);
}

/*
 * vrati substring od pozicie i dlzky n
 */
char * substr(const char *s, int i, int n){
	char *s2;
	int length=Xlength(s);
	int length_result;
	if(i<0 || i>=n || i>=length){
		return "";
	}
	if(n>length){
		n=length;
	}
	if(n < (length-i)){
		length_result=length-i;
	}
	else{
		length_result=(size_t)n;
	}
	s2=malloc((length_result +1) * sizeof(char));
	if (s2==NULL){
		return "";
	}
	if(i<length){
		memcpy(s2,s+i,length_result);
	}
	
	if(n > length){
		s2[n-1]='\0';
	}
	else{
		s2[n]='\0';
	}

	return s2;
}
/*
 * porovna lexikograficky dva stringy ak sa rovnaju vrati 0 ak je prvy vacsi vrati 1 inac -1
 */
int compare(const char *s1,const char *s2){
	int result = 0;
	bool diff=false;

	while(!diff && *s2){
		result=*(unsigned char *) s1 - *(unsigned char *) s2;
		s1++;s2++;
		if(result!=0){
			diff=true;
		}
	}
	if(result<0){
		return -1;
	}
	else if(result>0){
		return +1;
	}
	else
		return 0;
}
/*
 * vyhlada substring v stringu a vrati poziciu jeho zaciatku, ak nenajde vrati -1
 */
int find (const char *s, const char *search){
	int length_s=Xlength(s);
	int length_search=Xlength(search);
	int tmp[256];
	int x,y;

	if(length_search==0){
		return 0;
	}
	for(int i=0; i< 256; i++){
		tmp[i]=length_search;
	}
	for(int i=0; i<length_search ; i++){
		tmp[(int)search[i]] = length_search - (i+1);
	}
	x=length_search-1;
	while(x<length_s){
		y=length_search-1;
		while(y>=0 && s[x] == search[y]){
			x--;y--;
		}
		if(y<0){
			return x+1;
		}
		x=x+tmp[(int)s[x]];
	}
	return -1;

}
/*
 * vykona shell sort
 */
char * sort(const char *s){
	int length=Xlength(s);
	char * result=malloc((length+1)*(sizeof(char)));
	strcpy(result,s);
	int gap=length/2;
	char c;

	while(gap>0){
		for(int i=0; i<length-gap;i++){
			int x=i+gap;
			c=result[x];
			while(result[x-gap] > c && x >= gap){
				result[x]=result[x-gap];
				x=x-gap;
			}
			result[x]=c;
		}
		if(gap == 2){
			gap=1;
		}
		else
			gap=(int)(gap/2.2);
	}
	return result;
}

/*
*	nacitanie stringu z stdin
*/
char * readString(){
	char * string=(char *)malloc(10);
	int i=0;
	do
	{
		string [i]=getc(stdin);
		if(string[i]=='\n' || string[i]==EOF){
			string[i]='\0';
			break;
		}
		i++;
	}while(1);
	return string;
}
/*
*	nacitanie stringu z stdin, prevod na integer
*/
int readInt(){
	char * string = readString();
	if(Xlength(string)==1){
		if(string[0]=='0'){
			return 0;
		}
	}
	int result = atoi(string);
	if(result==0){
		fprintf(stderr,"ERR:Int\n");
		exit (7);
	}
	return result;
}
/*
*	nacitanie stringu z stdin, prevod na double
*/
double readDouble(){
	char * string = readString();
	double result = atof(string);
	if(Xlength(string)==1){
		if(string[0]=='0'){
			return 0;
		}
	}
	if (result==0){
		fprintf(stderr,"ERR:Double\n");
		exit (7);
	}
	return result;
}

/*
*	vypis hodnoty na standardny vystup
*/
void Xprint(const char *s){
	double num=0;
	char * string=(char *)malloc(100);
	strcpy(string,s);
	if(find(s,"+")==-1){
		num=atof(s);
		if (num!=0 || s[0]=='0'){
				printf("%g\n", num);
		}
		else
			printf("%s\n", s);

	}
	else
		{
			int i=0;
			while(s[i]!='\0'){
			if(s[i]=='+'){
				string[i]=15;
			}
			i++;
			}
			printf("%s\n", string);
		}

}

/* HASH TABLE */
/*
* 	inicializacia hash tabulky
*/
hash_table *init_HashTable(int size){
	hash_table *tmp;
	if((tmp=malloc(sizeof(hash_table)+sizeof(struct hashItem) * size)) == NULL){
		return NULL;
	}
	tmp->size = size;
	return tmp;
}
/*
* 	uvolnenie hash tabulky
*/

void free_HashTable(hash_table *tmp){
	if(tmp == NULL){
		return;
	}
	for(int i=0; i< tmp->size;++i){
		while(tmp->array[i] != NULL){
			tabItem *next = tmp->array[i]->next;
			if(next != NULL){
				free(tmp->array[i]->name);
				free(tmp->array[i]);
				tmp->array[i]=next;
				continue;
			}
			else if (tmp->array != NULL){
				free(tmp->array[i]->name);
				free(tmp->array[i]);
			}
			break;
		}
	}
	free(tmp);
}
/*
*  vytvorenie hashu
*  preco 65599? Magic konstanta, pochadza z Berkeley
*/
static inline zabo get_hash(const char *string, int size){
	unsigned int value = 0;
	const unsigned char *tmp;
	for(tmp = (const unsigned char *) string; *tmp != '\0'; tmp++){
		value =value *65599 + *tmp;
	}
	return value % size;
}
/*
* 	inicializacia noveho itemu do tabulky
*/
tabItem *create_tabItem(){
	tabItem *tmp;
	if((tmp=malloc(sizeof(tabItem)))==NULL){
		return NULL;
	}
	tmp->name=NULL;
	tmp->typeData=-1;
	tmp->param=-1;
	tmp->paramPos=-1;
	tmp->stringVal=NULL;
	return tmp;
}
/*
* 	vlozenie itemu do tabulky
*/
 void insert_into_HashTable(hash_table *table, tabItem *item){
 	int hash = get_hash(item->name, table->size);
 	if(table->array[hash]==NULL){
 		table->array[hash]=item;
 	}else{
	 		tabItem *tmp;
	 		tmp=table->array[hash];
	 		while(true){
	 			if(!strcmp(tmp->name,item->name)){
	 				table->array[hash]=item;
	 				break;
	 			}
	 			if(tmp->next == NULL){
	 				tmp->next=item;
	 				break;
	 			}else{
	 				tmp=tmp->next;
	 				}
	 		}
 	}
 }
/* Kopiruje data zo src do dest, oba ukazatele musia byt inicializovane (create_tabItem)*/
void copy_HashTable_Item(tabItem *src, tabItem *dest) {
    dest->isfunc = src->isfunc;
    strcpy(dest->name, src->name);
    dest->typeData = src->typeData;
    dest->defined = src->defined;
    dest->param = src->param;
    dest->paramPos = src->paramPos;
    dest->intVal = src->intVal;
    dest->doubleVal = src->doubleVal;
    strcpy(dest->stringVal, src->stringVal);
    strcpy(dest->blok, src->blok);
}

/*
* 	vyhladanie itemu v tabulke podla hash
*/
tabItem *search_HashTable(const hash_table *table, const char *item){
 	int hash = get_hash(item, table->size);
 	if(table->array[hash]==NULL){
 		return NULL;
 	}
 	else if(!strcmp(table->array[hash]->name,item)){
 			return table->array[hash];
 		}
 		else{
 			tabItem *tmp;
 			tmp=table->array[hash];
 			while(tmp != NULL){
 				if(!strcmp(tmp->name,item)){
 						return tmp;
 				}
 				else{
 					tmp=tmp->next;
 				}
 			}
 		}
 	return NULL;
 }
