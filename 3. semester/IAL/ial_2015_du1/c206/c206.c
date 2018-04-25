
/* c206.c **********************************************************}
{* T�ma: Dvousm�rn� v�zan� line�rn� seznam
**
**                   N�vrh a referen�n� implementace: Bohuslav K�ena, ��jen 2001
**                            P�epracovan� do jazyka C: Martin Tu�ek, ��jen 2004
**                                            �pravy: Bohuslav K�ena, ��jen 2015
**
** Implementujte abstraktn� datov� typ dvousm�rn� v�zan� line�rn� seznam.
** U�ite�n�m obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datov� abstrakce reprezentov�n prom�nnou
** typu tDLList (DL znamen� Double-Linked a slou�� pro odli�en�
** jmen konstant, typ� a funkc� od jmen u jednosm�rn� v�zan�ho line�rn�ho
** seznamu). Definici konstant a typ� naleznete v hlavi�kov�m souboru c206.h.
**
** Va��m �kolem je implementovat n�sleduj�c� operace, kter� spolu
** s v��e uvedenou datovou ��st� abstrakce tvo�� abstraktn� datov� typ
** obousm�rn� v�zan� line�rn� seznam:
**
**      DLInitList ...... inicializace seznamu p�ed prvn�m pou�it�m,
**      DLDisposeList ... zru�en� v�ech prvk� seznamu,
**      DLInsertFirst ... vlo�en� prvku na za��tek seznamu,
**      DLInsertLast .... vlo�en� prvku na konec seznamu, 
**      DLFirst ......... nastaven� aktivity na prvn� prvek,
**      DLLast .......... nastaven� aktivity na posledn� prvek, 
**      DLCopyFirst ..... vrac� hodnotu prvn�ho prvku,
**      DLCopyLast ...... vrac� hodnotu posledn�ho prvku, 
**      DLDeleteFirst ... zru�� prvn� prvek seznamu,
**      DLDeleteLast .... zru�� posledn� prvek seznamu, 
**      DLPostDelete .... ru�� prvek za aktivn�m prvkem,
**      DLPreDelete ..... ru�� prvek p�ed aktivn�m prvkem, 
**      DLPostInsert .... vlo�� nov� prvek za aktivn� prvek seznamu,
**      DLPreInsert ..... vlo�� nov� prvek p�ed aktivn� prvek seznamu,
**      DLCopy .......... vrac� hodnotu aktivn�ho prvku,
**      DLActualize ..... p�ep�e obsah aktivn�ho prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na dal�� prvek seznamu,
**      DLPred .......... posune aktivitu na p�edchoz� prvek seznamu, 
**      DLActive ........ zji��uje aktivitu seznamu.
**
** P�i implementaci jednotliv�ch funkc� nevolejte ��dnou z funkc�
** implementovan�ch v r�mci tohoto p��kladu, nen�-li u funkce
** explicitn� uvedeno n�co jin�ho.
**
** Nemus�te o�et�ovat situaci, kdy m�sto leg�ln�ho ukazatele na seznam 
** p�ed� n�kdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodn� komentujte!
**
** Terminologick� pozn�mka: Jazyk C nepou��v� pojem procedura.
** Proto zde pou��v�me pojem funkce i pro operace, kter� by byly
** v algoritmick�m jazyce Pascalovsk�ho typu implemenov�ny jako
** procedury (v jazyce C procedur�m odpov�daj� funkce vracej�c� typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozorn�n� na to, �e do�lo k chyb�.
** Tato funkce bude vol�na z n�kter�ch d�le implementovan�ch operac�.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* glob�ln� prom�nn� -- p��znak o�et�en� chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L p�ed jeho prvn�m pou�it�m (tzn. ��dn�
** z n�sleduj�c�ch funkc� nebude vol�na nad neinicializovan�m seznamem).
** Tato inicializace se nikdy nebude prov�d�t nad ji� inicializovan�m
** seznamem, a proto tuto mo�nost neo�et�ujte. V�dy p�edpokl�dejte,
** �e neinicializovan� prom�nn� maj� nedefinovanou hodnotu.
**/
	    L->Act = NULL;  // vynulovanie prvkov
	    L->First = NULL;
		L->Last = NULL;	
}

void DLDisposeList (tDLList *L) {
/*
** Zru�� v�echny prvky seznamu L a uvede seznam do stavu, v jak�m
** se nach�zel po inicializaci. Ru�en� prvky seznamu budou korektn�
** uvoln�ny vol�n�m operace free. 
**/
	while (L->First != NULL)  // pokia� nezr��� v�etky prvky
	{
		tDLElemPtr tmp = L->First; // do�asn� ukazate� na prv� prvok
		L->First = L->First->rptr; // prv� prvok nastav� na druh� prvok
		free(tmp);                 // uvoln� miesto po prvku v pamati
	}	
	L->Act = NULL;       // vymazanie ostatn�ch zbytkov
	L->Last = NULL;
	
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vlo�� nov� prvek na za��tek seznamu L.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
**/
		tDLElemPtr tmp;    // nov� prvok
		if((tmp = malloc(sizeof(struct tDLElem))) == NULL) // alok�cia nov�ho prvku
		{
			DLError(); // o�etrenie chyby
			return;
		}
		tmp->data = val;  // hodnota nov�ho prvku
		tmp->rptr = L->First; // prepojenie medzi nov�m a star�m prvkom
		tmp->lptr = NULL;  // nastavenie prvku ako prv�ho
		if (L->First != NULL)  
		{
			L->First->lptr = tmp;
		}
		else
		{
			L->Last = tmp; // vlo�enie do pr�zdneho zoznamu
		}
		L->First = tmp;

}

void DLInsertLast(tDLList *L, int val) {
/*
** Vlo�� nov� prvek na konec seznamu L (symetrick� operace k DLInsertFirst).
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
**/ 	
	tDLElemPtr tmp; // nov� prvok
	if ((tmp = malloc(sizeof(struct tDLElem))) == NULL) // alok�cia prvku
		{
			DLError(); // o�etrenie chyby
			return;
		}	
	tmp->data = val;   // nastavenie hodnoty nov�mu prvku
	tmp->lptr = L->Last; // prepojenie medzi posledn�m a predposledn�m prvkom
	tmp->rptr = NULL;  // nastavenie prvku ako posledn�ho
	if (L->Last != NULL) // pre nepr�zdny zoznam 
	{
		L->Last->rptr = tmp;
	}
	else // pre pr�zdny zoznam
	{
		L->First = tmp;
	}
	L->Last = tmp;
	
}

void DLFirst (tDLList *L) {
/*
** Nastav� aktivitu na prvn� prvek seznamu L.
** Funkci implementujte jako jedin� p��kaz (nepo��t�me-li return),
** ani� byste testovali, zda je seznam L pr�zdn�.
**/
	L->Act = L->First; // nastav� aktivitu na prv� prvok		
}

void DLLast (tDLList *L) {
/*
** Nastav� aktivitu na posledn� prvek seznamu L.
** Funkci implementujte jako jedin� p��kaz (nepo��t�me-li return),
** ani� byste testovali, zda je seznam L pr�zdn�.
**/
	L->Act = L->Last;	// nastavenie aktivity na posledn� prvok
	
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prost�ednictv�m parametru val vr�t� hodnotu prvn�ho prvku seznamu L.
** Pokud je seznam L pr�zdn�, vol� funkci DLError().
**/
	if (L->Last == NULL && L->First == NULL) // ak je zoznam pr�zdny o�etr� chybu
	{
		DLError();
		return;
	}	
	*val = L->First->data; // vr�ti hodnotu prv�ho prvku
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prost�ednictv�m parametru val vr�t� hodnotu posledn�ho prvku seznamu L.
** Pokud je seznam L pr�zdn�, vol� funkci DLError().
**/
	if (L->Last == NULL && L->First == NULL) // ak je zoznam pr�zdny o�etr� chybu
	{
		DLError();
		return;
	}	
	*val = L-> Last->data; // vr�ti hodnotu posledn�ho prvku zoznamu
}

void DLDeleteFirst (tDLList *L) {
/*
** Zru�� prvn� prvek seznamu L. Pokud byl prvn� prvek aktivn�, aktivita 
** se ztr�c�. Pokud byl seznam L pr�zdn�, nic se ned�je.
**/
	if (L->Last == NULL && L->First == NULL) // ak je zoznam pr�zdny nerob� ni�
		{
			return;	
		}
	if (L->First != NULL) // ak je prv� prvok akt�vny deaktivuje ho
	{
		tDLElemPtr tmp = L->First;
		if (L->Act == L->First)
		{
			L->Act = NULL;
		}
		if (L->First == L->Last) // ak je posledn� vymazan�, vynuluje ho
		{
			L->First= NULL;
			L->Last = NULL;
		}
		else
		{
			L->First = L->First->rptr;
			L->First->lptr = NULL;
		}
		free(tmp); // uvoln�
	}	
}	

void DLDeleteLast (tDLList *L) {
/*
** Zru�� posledn� prvek seznamu L. Pokud byl posledn� prvek aktivn�,
** aktivita seznamu se ztr�c�. Pokud byl seznam L pr�zdn�, nic se ned�je.
**/ 
	if (L->Last == NULL && L->First == NULL) // ak je zoznam pr�zdny nerob� ni�
	{
		return;
	}	
	tDLElemPtr tmp;
	if (L->Last != NULL)
	{
		tmp= L->Last;
		if(L->Act == L-> Last)  // ak je prvok akt�vny deaktivuje ho
		{
			L->Act = NULL;
		}
		if (L->First == L->Last)
		{
			L->First = NULL;
			L->Last = NULL;
		}
		else  // z posledn�ho sprav� predposledn�
		{
			L->Last = L->Last->lptr;
			L->Last->rptr = NULL;
		}
		free(tmp); // uvo�n� pam�
	}
	
}

void DLPostDelete (tDLList *L) {
/*
** Zru�� prvek seznamu L za aktivn�m prvkem.
** Pokud je seznam L neaktivn� nebo pokud je aktivn� prvek
** posledn�m prvkem seznamu, nic se ned�je.
**/
		if (L->Act != NULL && L->Act->rptr != NULL) // ak je zoznam akt�vny a neprazdn�
		{
			tDLElemPtr tmp = L->Act->rptr; // z�lohuje zmazan�
			L->Act->rptr = tmp->rptr;      // posunie ukazatel na �al�� prvok
			if (tmp == L->Last)
			{
				L->Last = L->Act;  
			}
			else
			{
				tmp->rptr->lptr = L->Act;
			}
			free(tmp); // uvoln� pam�
		}
}

void DLPreDelete (tDLList *L) {
/*
** Zru�� prvek p�ed aktivn�m prvkem seznamu L .
** Pokud je seznam L neaktivn� nebo pokud je aktivn� prvek
** prvn�m prvkem seznamu, nic se ned�je.
**/
	if (L->Act != NULL && L->Act->lptr != NULL)
	{
		tDLElemPtr tmp = L->Act->lptr; // ak je zoznam akt�vny a aktu�lny nen� prv�
		L->Act->lptr = tmp->lptr;  
		if(tmp == L->First)
		{
			L->First = L->Act; // ak je mazan� prv�, bude aj akt�vny
		}
		else
		{
			tmp->lptr->rptr = L->Act;
		}
		free(tmp); //uvoln� pam�
	}	
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vlo�� prvek za aktivn� prvek seznamu L.
** Pokud nebyl seznam L aktivn�, nic se ned�je.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
**/
	if(L->Act != NULL) // ak je zoznam akt�vny
	{
		tDLElemPtr tmp;
		if((tmp = malloc(sizeof(struct tDLElem))) == NULL) // alok�cia nov�ho prvku
		{
			DLError(); // o�etrenie chyby
			return;
		}
		tmp->data = val; // napln� d�ta
		tmp->rptr = L->Act->rptr;
		tmp->lptr = L->Act;
		L->Act->rptr = tmp;
		if (L->Act == L->Last) // ak je akt�vny posledn�, bude nov� prvok posledn�
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
** Vlo�� prvek p�ed aktivn� prvek seznamu L.
** Pokud nebyl seznam L aktivn�, nic se ned�je.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
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
			L->First = tmp; // ak je prvok akt�vny, bude nov� prvok prv�
		}
		else
		{
			tmp->lptr->rptr = tmp;
		}

	}	
	
}

void DLCopy (tDLList *L, int *val) {
/*
** Prost�ednictv�m parametru val vr�t� hodnotu aktivn�ho prvku seznamu L.
** Pokud seznam L nen� aktivn�, vol� funkci DLError ().
**/
	if (L->Act == NULL) // ak nie je zoznam akt�vny
	{ 
		DLError(); // o�etr� chybu
		return;
	}	
	*val = L->Act->data; // vr�ti hodnotu akt�vneho prvku

	
}

void DLActualize (tDLList *L, int val) {
/*
** P�ep�e obsah aktivn�ho prvku seznamu L.
** Pokud seznam L nen� aktivn�, ned�l� nic.
**/
	if (L->Act == NULL) // ak nie je zoznam akt�vny, nerob� ni�
	{
		return;
	}	
	L->Act->data = val; // prep�e hodnotu akt�vneho prvku
	
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na n�sleduj�c� prvek seznamu L.
** Nen�-li seznam aktivn�, ned�l� nic.
** V�imn�te si, �e p�i aktivit� na posledn�m prvku se seznam stane neaktivn�m.
**/
	if(L->Act == NULL) // ak nie je zoznam akt�vny, nerob� ni�
	{
		return;
	}
	L->Act = L->Act->rptr; // posunie aktivitu na n�sleduj�ci prvok
	
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na p�edchoz� prvek seznamu L.
** Nen�-li seznam aktivn�, ned�l� nic.
** V�imn�te si, �e p�i aktivit� na prvn�m prvku se seznam stane neaktivn�m.
**/
	if(L->Act == NULL) // ak nie je zoznam akt�vny, nerob� ni�
	{
		return;
	}	
	L->Act = L->Act->lptr; // posunie aktivitu na predch�dzaj�ci prvok
	
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivn�, vrac� nenulovou hodnotu, jinak vrac� 0.
** Funkci je vhodn� implementovat jedn�m p��kazem return.
**/
	
return L->Act != NULL;	 //ak je zoznam akt�vny, vr�ti nenulov� hodnotu
}

/* Konec c206.c*/
