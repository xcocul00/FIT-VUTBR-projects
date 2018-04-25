
/* c206.c **********************************************************}
{* Téma: Dvousmìrnì vázaný lineární seznam
**
**                   Návrh a referenèní implementace: Bohuslav Køena, øíjen 2001
**                            Pøepracované do jazyka C: Martin Tuèek, øíjen 2004
**                                            Úpravy: Bohuslav Køena, øíjen 2015
**
** Implementujte abstraktní datový typ dvousmìrnì vázaný lineární seznam.
** U¾iteèným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován promìnnou
** typu tDLList (DL znamená Double-Linked a slou¾í pro odli¹ení
** jmen konstant, typù a funkcí od jmen u jednosmìrnì vázaného lineárního
** seznamu). Definici konstant a typù naleznete v hlavièkovém souboru c206.h.
**
** Va¹ím úkolem je implementovat následující operace, které spolu
** s vý¹e uvedenou datovou èástí abstrakce tvoøí abstraktní datový typ
** obousmìrnì vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu pøed prvním pou¾itím,
**      DLDisposeList ... zru¹ení v¹ech prvkù seznamu,
**      DLInsertFirst ... vlo¾ení prvku na zaèátek seznamu,
**      DLInsertLast .... vlo¾ení prvku na konec seznamu, 
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek, 
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku, 
**      DLDeleteFirst ... zru¹í první prvek seznamu,
**      DLDeleteLast .... zru¹í poslední prvek seznamu, 
**      DLPostDelete .... ru¹í prvek za aktivním prvkem,
**      DLPreDelete ..... ru¹í prvek pøed aktivním prvkem, 
**      DLPostInsert .... vlo¾í nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vlo¾í nový prvek pøed aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... pøepí¹e obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na dal¹í prvek seznamu,
**      DLPred .......... posune aktivitu na pøedchozí prvek seznamu, 
**      DLActive ........ zji¹»uje aktivitu seznamu.
**
** Pøi implementaci jednotlivých funkcí nevolejte ¾ádnou z funkcí
** implementovaných v rámci tohoto pøíkladu, není-li u funkce
** explicitnì uvedeno nìco jiného.
**
** Nemusíte o¹etøovat situaci, kdy místo legálního ukazatele na seznam 
** pøedá nìkdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodnì komentujte!
**
** Terminologická poznámka: Jazyk C nepou¾ívá pojem procedura.
** Proto zde pou¾íváme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornìní na to, ¾e do¹lo k chybì.
** Tato funkce bude volána z nìkterých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální promìnná -- pøíznak o¹etøení chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L pøed jeho prvním pou¾itím (tzn. ¾ádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádìt nad ji¾ inicializovaným
** seznamem, a proto tuto mo¾nost neo¹etøujte. V¾dy pøedpokládejte,
** ¾e neinicializované promìnné mají nedefinovanou hodnotu.
**/
	    L->Act = NULL;  // vynulovanie prvkov
	    L->First = NULL;
		L->Last = NULL;	
}

void DLDisposeList (tDLList *L) {
/*
** Zru¹í v¹echny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Ru¹ené prvky seznamu budou korektnì
** uvolnìny voláním operace free. 
**/
	while (L->First != NULL)  // pokiaµ nezrú¹í v¹etky prvky
	{
		tDLElemPtr tmp = L->First; // doèasný ukazateµ na prvý prvok
		L->First = L->First->rptr; // prvý prvok nastaví na druhý prvok
		free(tmp);                 // uvolní miesto po prvku v pamati
	}	
	L->Act = NULL;       // vymazanie ostatných zbytkov
	L->Last = NULL;
	
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vlo¾í nový prvek na zaèátek seznamu L.
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/
		tDLElemPtr tmp;    // nový prvok
		if((tmp = malloc(sizeof(struct tDLElem))) == NULL) // alokácia nového prvku
		{
			DLError(); // o¹etrenie chyby
			return;
		}
		tmp->data = val;  // hodnota nového prvku
		tmp->rptr = L->First; // prepojenie medzi novým a starým prvkom
		tmp->lptr = NULL;  // nastavenie prvku ako prvého
		if (L->First != NULL)  
		{
			L->First->lptr = tmp;
		}
		else
		{
			L->Last = tmp; // vlo¾enie do prázdneho zoznamu
		}
		L->First = tmp;

}

void DLInsertLast(tDLList *L, int val) {
/*
** Vlo¾í nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/ 	
	tDLElemPtr tmp; // nový prvok
	if ((tmp = malloc(sizeof(struct tDLElem))) == NULL) // alokácia prvku
		{
			DLError(); // o¹etrenie chyby
			return;
		}	
	tmp->data = val;   // nastavenie hodnoty novému prvku
	tmp->lptr = L->Last; // prepojenie medzi posledným a predposledným prvkom
	tmp->rptr = NULL;  // nastavenie prvku ako posledného
	if (L->Last != NULL) // pre neprázdny zoznam 
	{
		L->Last->rptr = tmp;
	}
	else // pre prázdny zoznam
	{
		L->First = tmp;
	}
	L->Last = tmp;
	
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný pøíkaz (nepoèítáme-li return),
** ani¾ byste testovali, zda je seznam L prázdný.
**/
	L->Act = L->First; // nastaví aktivitu na prvý prvok		
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný pøíkaz (nepoèítáme-li return),
** ani¾ byste testovali, zda je seznam L prázdný.
**/
	L->Act = L->Last;	// nastavenie aktivity na posledný prvok
	
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostøednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
	if (L->Last == NULL && L->First == NULL) // ak je zoznam prázdny o¹etrí chybu
	{
		DLError();
		return;
	}	
	*val = L->First->data; // vráti hodnotu prvého prvku
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostøednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
	if (L->Last == NULL && L->First == NULL) // ak je zoznam prázdny o¹etrí chybu
	{
		DLError();
		return;
	}	
	*val = L-> Last->data; // vráti hodnotu posledného prvku zoznamu
}

void DLDeleteFirst (tDLList *L) {
/*
** Zru¹í první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se nedìje.
**/
	if (L->Last == NULL && L->First == NULL) // ak je zoznam prázdny nerobí niè
		{
			return;	
		}
	if (L->First != NULL) // ak je prvý prvok aktívny deaktivuje ho
	{
		tDLElemPtr tmp = L->First;
		if (L->Act == L->First)
		{
			L->Act = NULL;
		}
		if (L->First == L->Last) // ak je posledný vymazaný, vynuluje ho
		{
			L->First= NULL;
			L->Last = NULL;
		}
		else
		{
			L->First = L->First->rptr;
			L->First->lptr = NULL;
		}
		free(tmp); // uvolní
	}	
}	

void DLDeleteLast (tDLList *L) {
/*
** Zru¹í poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se nedìje.
**/ 
	if (L->Last == NULL && L->First == NULL) // ak je zoznam prázdny nerobí niè
	{
		return;
	}	
	tDLElemPtr tmp;
	if (L->Last != NULL)
	{
		tmp= L->Last;
		if(L->Act == L-> Last)  // ak je prvok aktívny deaktivuje ho
		{
			L->Act = NULL;
		}
		if (L->First == L->Last)
		{
			L->First = NULL;
			L->Last = NULL;
		}
		else  // z posledného spraví predposledný
		{
			L->Last = L->Last->lptr;
			L->Last->rptr = NULL;
		}
		free(tmp); // uvoµní pamä»
	}
	
}

void DLPostDelete (tDLList *L) {
/*
** Zru¹í prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se nedìje.
**/
		if (L->Act != NULL && L->Act->rptr != NULL) // ak je zoznam aktívny a neprazdný
		{
			tDLElemPtr tmp = L->Act->rptr; // zálohuje zmazaný
			L->Act->rptr = tmp->rptr;      // posunie ukazatel na ïal¹í prvok
			if (tmp == L->Last)
			{
				L->Last = L->Act;  
			}
			else
			{
				tmp->rptr->lptr = L->Act;
			}
			free(tmp); // uvolní pamä»
		}
}

void DLPreDelete (tDLList *L) {
/*
** Zru¹í prvek pøed aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se nedìje.
**/
	if (L->Act != NULL && L->Act->lptr != NULL)
	{
		tDLElemPtr tmp = L->Act->lptr; // ak je zoznam aktívny a aktuálny není prvý
		L->Act->lptr = tmp->lptr;  
		if(tmp == L->First)
		{
			L->First = L->Act; // ak je mazaný prvý, bude aj aktívny
		}
		else
		{
			tmp->lptr->rptr = L->Act;
		}
		free(tmp); //uvolní pamä»
	}	
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vlo¾í prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se nedìje.
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/
	if(L->Act != NULL) // ak je zoznam aktívny
	{
		tDLElemPtr tmp;
		if((tmp = malloc(sizeof(struct tDLElem))) == NULL) // alokácia nového prvku
		{
			DLError(); // o¹etrenie chyby
			return;
		}
		tmp->data = val; // naplní dáta
		tmp->rptr = L->Act->rptr;
		tmp->lptr = L->Act;
		L->Act->rptr = tmp;
		if (L->Act == L->Last) // ak je aktívny posledný, bude nový prvok posledný
		{
			L->Last = tmp;
		}
		else
		{
			tmp->rptr->lptr = tmp;
		}
	}	
	
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vlo¾í prvek pøed aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se nedìje.
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/
	if(L->Act != NULL)
	{
		tDLElemPtr tmp;
		if((tmp = malloc(sizeof(struct tDLElem))) == NULL)
		{
			DLError();
			return;
		}
		tmp->data = val;
		tmp->lptr = L->Act->lptr;
		tmp->rptr = L->Act;
		L->Act->lptr = tmp;
		if(L->Act == L->First)
		{
			L->First = tmp; // ak je prvok aktívny, bude nový prvok prvý
		}
		else
		{
			tmp->lptr->rptr = tmp;
		}

	}	
	
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostøednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
	if (L->Act == NULL) // ak nie je zoznam aktívny
	{ 
		DLError(); // o¹etrí chybu
		return;
	}	
	*val = L->Act->data; // vráti hodnotu aktívneho prvku

	
}

void DLActualize (tDLList *L, int val) {
/*
** Pøepí¹e obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedìlá nic.
**/
	if (L->Act == NULL) // ak nie je zoznam aktívny, nerobí niè
	{
		return;
	}	
	L->Act->data = val; // prepí¹e hodnotu aktívneho prvku
	
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedìlá nic.
** V¹imnìte si, ¾e pøi aktivitì na posledním prvku se seznam stane neaktivním.
**/
	if(L->Act == NULL) // ak nie je zoznam aktívny, nerobí niè
	{
		return;
	}
	L->Act = L->Act->rptr; // posunie aktivitu na následujúci prvok
	
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na pøedchozí prvek seznamu L.
** Není-li seznam aktivní, nedìlá nic.
** V¹imnìte si, ¾e pøi aktivitì na prvním prvku se seznam stane neaktivním.
**/
	if(L->Act == NULL) // ak nie je zoznam aktívny, nerobí niè
	{
		return;
	}	
	L->Act = L->Act->lptr; // posunie aktivitu na predchádzajúci prvok
	
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním pøíkazem return.
**/
	
return L->Act != NULL;	 //ak je zoznam aktívny, vráti nenulovú hodnotu
}

/* Konec c206.c*/
