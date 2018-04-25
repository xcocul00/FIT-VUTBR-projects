
/* c201.c *********************************************************************}
{* T�ma: Jednosm�rn� line�rn� seznam
**
**                     N�vrh a referen�n� implementace: Petr P�ikryl, ��jen 1994
**                                          �pravy: Andrea N�mcov� listopad 1996
**                                                   Petr P�ikryl, listopad 1997
**                                P�epracovan� zad�n�: Petr P�ikryl, b�ezen 1998
**                                  P�epis do jazyka C: Martin Tu�ek, ��jen 2004
**	                                      �pravy: Bohuslav K�ena, ��jen 2015
**
** Implementujte abstraktn� datov� typ jednosm�rn� line�rn� seznam.
** U�ite�n�m obsahem prvku seznamu je cel� ��slo typu int.
** Seznam bude jako datov� abstrakce reprezentov�n prom�nnou typu tList.
** Definici konstant a typ� naleznete v hlavi�kov�m souboru c201.h.
** 
** Va��m �kolem je implementovat n�sleduj�c� operace, kter� spolu s v��e
** uvedenou datovou ��st� abstrakce tvo�� abstraktn� datov� typ tList:
**
**      InitList ...... inicializace seznamu p�ed prvn�m pou�it�m,
**      DisposeList ... zru�en� v�ech prvk� seznamu,
**      InsertFirst ... vlo�en� prvku na za��tek seznamu,
**      First ......... nastaven� aktivity na prvn� prvek,
**      CopyFirst ..... vrac� hodnotu prvn�ho prvku,
**      DeleteFirst ... zru�� prvn� prvek seznamu,
**      PostDelete .... ru�� prvek za aktivn�m prvkem,
**      PostInsert .... vlo�� nov� prvek za aktivn� prvek seznamu,
**      Copy .......... vrac� hodnotu aktivn�ho prvku,
**      Actualize ..... p�ep�e obsah aktivn�ho prvku novou hodnotou,
**      Succ .......... posune aktivitu na dal�� prvek seznamu,
**      Active ........ zji��uje aktivitu seznamu.
**
** P�i implementaci funkc� nevolejte ��dnou z funkc� implementovan�ch v r�mci
** tohoto p��kladu, nen�-li u dan� funkce explicitn� uvedeno n�co jin�ho.
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

#include "c201.h"

int solved;
int errflg;

void Error() {
/*
** Vytiskne upozorn�n� na to, �e do�lo k chyb�.
** Tato funkce bude vol�na z n�kter�ch d�le implementovan�ch operac�.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;                      /* glob�ln� prom�nn� -- p��znak chyby */
}

void InitList (tList *L) {
/*
** Provede inicializaci seznamu L p�ed jeho prvn�m pou�it�m (tzn. ��dn�
** z n�sleduj�c�ch funkc� nebude vol�na nad neinicializovan�m seznamem).
** Tato inicializace se nikdy nebude prov�d�t nad ji� inicializovan�m
** seznamem, a proto tuto mo�nost neo�et�ujte. V�dy p�edpokl�dejte,
** �e neinicializovan� prom�nn� maj� nedefinovanou hodnotu.
**/
	L->Act=NULL;      //pr�zdny aktu�lny
	L->First=NULL;    // pr�zdny prv�
 
}

void DisposeList (tList *L) {
/*
** Zru�� v�echny prvky seznamu L a uvede seznam L do stavu, v jak�m se nach�zel
** po inicializaci. Ve�ker� pam� pou��van� prvky seznamu L bude korektn�
** uvoln�na vol�n�m operace free.
***/

	while (L->First != NULL) {     // pokia� nezru��m v�etky prvky zoznamu
		tElemPtr tmp = L->First;   // do�asn� ukazate� na prv� prvok zoznamu
		L->First = L->First->ptr;  // posun v prvkoch o miesto �alej
		free (tmp);				   // zru�� do�asn� ukazate�	 						
	}
	L->Act = NULL;  // nastav� prvok na neakt�vny


}

void InsertFirst (tList *L, int val) {
/*
** Vlo�� prvek s hodnotou val na za��tek seznamu L.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci Error().
**/
	tElemPtr tmp;     // nov� prvok
	if ((tmp = malloc(sizeof(struct tElem))) == NULL )  // alok�cia prvku
	{
		Error();    // o�etrenie chyby alok�cie
		return;     
	}
		tmp->data = val;       // hodnota nov�ho prvku
		tmp->ptr = L->First;   // prv� prvok ukazuje na b�val� prv�
 		L->First = tmp;        // nastavenie prvku ako prv�ho 
	
}

void First (tList *L) {
/*
** Nastav� aktivitu seznamu L na jeho prvn� prvek.
** Funkci implementujte jako jedin� p��kaz, ani� byste testovali,
** zda je seznam L pr�zdn�.
**/
	L->Act = L->First; // prv� prvok nastav� na akt�vny

}

void CopyFirst (tList *L, int *val) {
/*
** Prost�ednictv�m parametru val vr�t� hodnotu prvn�ho prvku seznamu L.
** Pokud je seznam L pr�zdn�, vol� funkci Error().
**/
	if (L-> First == NULL)   // zist� �i je zoznam pr�zdny
	{
		Error();             // o�etrenie chyby  
		return;
	}
	*val = L->First->data;   // vr�ti hodnotu prv�ho prvku
	
}

void DeleteFirst (tList *L) {
/*
** Zru�� prvn� prvek seznamu L a uvoln� j�m pou��vanou pam�.
** Pokud byl ru�en� prvek aktivn�, aktivita seznamu se ztr�c�.
** Pokud byl seznam L pr�zdn�, nic se ned�je.
**/
	if (L->First == NULL)   // nerob� ni� ak je zoznam pr�zdny
	{
		return;
	}	
	if (L->Act == L->First) // ak je prv� akt�vny, deaktivuje ho
	{
		L->Act=NULL;
	}
	tElemPtr tmp = L->First; // do�asn� ukazate� na n�sleduj�ci prvok
	L->First = L->First->ptr; // prepojenie na �al�� prvok
	free(tmp);               // zru�� star� prv� prvok

}	

void PostDelete (tList *L) {
/* 
** Zru�� prvek seznamu L za aktivn�m prvkem a uvoln� j�m pou��vanou pam�.
** Pokud nen� seznam L aktivn� nebo pokud je aktivn� posledn� prvek seznamu L,
** nic se ned�je.
**/
	if (L->Act == NULL || L->Act->ptr == NULL)  // pokia� je zoznam neakt�vny ni� nerob�
	{
		return;
	}
	tElemPtr tmp = L->Act->ptr;          // do�asn� ukazate� na n�sleduj�ci prvok
	L->Act->ptr = L->Act->ptr->ptr;      // prepojenie na �al�� prvok
	free (tmp);                          // zru�� star� prv� prvok
	
 }

void PostInsert (tList *L, int val) {
/*
** Vlo�� prvek s hodnotou val za aktivn� prvek seznamu L.
** Pokud nebyl seznam L aktivn�, nic se ned�je!
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** zavol� funkci Error().
**/
	if (L->Act == NULL)  // ak nie je zoznam akt�vny nerob� ni�
	{
		return;
	}	
	tElemPtr tmp;        // nov� prvok
	if ((tmp = malloc(sizeof(struct tElem))) == NULL)  // alok�cia nov�ho prvku
	{
		Error();
		return;
	}
	tmp->data = val;           // vlo�enie hodnoty prvku
	tmp->ptr = L->Act->ptr;    // prepojenie na n�sleduj�ci prvok
	L->Act->ptr = tmp;         // nov� prvok bude za n�sleduj�cim akt�vnym  

}

void Copy (tList *L, int *val) {
/*
** Prost�ednictv�m parametru val vr�t� hodnotu aktivn�ho prvku seznamu L.
** Pokud seznam nen� aktivn�, zavol� funkci Error().
**/
	if (L->Act == NULL)     //  pokia� nie je zoznam akt�vny o�etr� chybu
	{
		Error();
		return;
	}
	*val = L->Act->data;    // pre��ta aktivn� d�ta a vr�ti ich

}

void Actualize (tList *L, int val) {
/*
** P�ep�e data aktivn�ho prvku seznamu L hodnotou val.
** Pokud seznam L nen� aktivn�, ned�l� nic!
**/
	if (L->Act == NULL)  // ak nie je zoznam akt�vny nerob� ni�
 	{
		return;
	}	
	L->Act->data = val;  // vlo�� hodnotu val ako d�ta akt�vneho prvku
	
}

void Succ (tList *L) {
/*
** Posune aktivitu na n�sleduj�c� prvek seznamu L.
** V�imn�te si, �e touto operac� se m��e aktivn� seznam st�t neaktivn�m.
** Pokud nen� p�edan� seznam L aktivn�, ned�l� funkce nic.
**/
	if (L-> Act == NULL)  // pokia� nie je zoznam akt�vny nerob� ni�
	{
		return;
	}	
	L->Act = L->Act->ptr; // posunie aktivitu na �al�� prvok zoznamu
	
}

int Active (tList *L) {		
/*
** Je-li seznam L aktivn�, vrac� nenulovou hodnotu, jinak vrac� 0.
** Tuto funkci je vhodn� implementovat jedn�m p��kazem return. 
**/
	
	return L->Act != NULL;   // ak je zoznam akt�vny vracia nenulov� hodnotu, inak vracia 0
}


/* Konec c201.c */
