:author: Maro� Cocu�a < xcocul00@stud.fit.vutbr.cz >
:updated: 2016/12/14

    

==============================================================
IMP - Riadenie v��ahu
==============================================================


.. contents:: Obsah

Zadanie projektu
==================

V jazyce C naimplementujte pro MCU �ady MSP430x na FITkitu aplikaci (tj. programovou realizaci �adi�e) pro ��zen� provozu v�tahu ve v�cepodla�n� budov� - bli��� specifikace viz n�e (pou�it� vestavn�ho OS p�i implementaci je nepovinn�). J�drem Va�� realizace nech� je ud�lostmi ��zen� mechanismus (nap�. kone�n� stavov� automat) schopn� na z�klad� p��slu�n� ud�losti a (vnit�n�ho) stavu aplikace vyvolat p��slu�nou reakci, tj. zm�nu dosavadn�ho chov�n� a stavu aplikace. V r�mci realizace tedy, mj., pe�liv� vymezte a zdokumentujte vstupy a v�stupy tohoto mechanismu, mno�inu vnit�n�ch stav� aplikace a podm�nek p�echod� mezi nimi. 

**Detaily k zad�n�**

    Budova m� celkem 4 podla��: sklep (ID = -1), 1. nadzemn� podla�� (p��zem�, ID = 1),  2. nadzemn� podla�� (pokoje, ID = 2) a 3. nadzemn� podla�� (p�da, ID = 3).
    V ka�d�m podla�� budovy jsou v�tahov� dve�e, u nich� je um�st�no tla��tko pro p�ivol�n� v�tahu a indik�tor aktu�ln� �innosti v�tahu (pohyb nahoru, dol�, st�n�, apod.).
    V�tah se sm� pohypovat pouze tehdy, jsou-li v�echny dve�e uzav�eny. 
    Po stisku tla��tka pro p�ivol�n� v�tahu v dan�m pat�e v�tah tento po�adavek zaregistruje a po dob� nezbytn� nutn� k vy��zen� ji� p�ijat�ch po�adavk� v tomto pat�e zastav�. 
    Po zastaven� v�tahu v dan�m pat�e se v tomto pat�e po kr�tk�m �ek�n� otev�ou dve�e umo��uj�c� p��stup k v�tahov� kabin�. Dve�e z�stanou ur�itou dobu otev�en� a pot� se za�nou pomalu zav�rat. Existuj�-li nevy��zen� po�adavky na pohyb v�tahu, v�tah se po kr�tk� dob� �ek�n� za�ne pohybovat ve sm�ru zaji��uj�c�m vy��zen� po�adavk�. Pokud se v�tah je�t� neza�al pohybovat a bylo stisknuto tla��tko aktu�ln�ho podla��, v�tah se chov� jako by v dan�m pat�e zastavil znovu.
    V kabin� je mo�n� pomoc� tla��tek volit podla��, ve kter�ch m� v�tah p�i sv�m pohybu �achtou zastavit. Dosud nevy��zen� po�adavky na zastaven� v dan�ch podla��ch, volen� tla��tky v kabin� nebo tla��tky pro p�ivol�n� v�tahu v jednotliv�ch podla��ch, jsou signalizov�ny u p��slu�n�ch tla��tek. Tla��tko Stop, um�st�n� v kabin�, slou�� k neprodlen�mu zastaven� pohybuj�c�ho se v�tahu.
    Informace o aktu�ln�m podla�� (my�lena ID uveden� v��e), na kter�m se v�tah nach�z�, je vyobrazena na speci�ln�m displeji, p�i�em� hodnota na displeji se aktualizuje v�dy p�edt�m, ne� se za�nou otev�rat dve�e dan�ho podla�� nebo tehdy, zm�n�-li v�tah p�i pohybu �achtou podla�� (tj., dostane-li se podlaha v�tahu do jedn� roviny s podlahou p��slu�n�ho podla��).  
    V�tah, zastaven� tla��tkem Stop, se za�ne pohybovat a� pot�, co je stisknuto tla��tko servisn�ho technika (Opraveno): po stisku tla��tka Opraveno jsou ve�ker� nevy��zen� po�adavky na p�ivol�n� v�tahu smaz�ny, pot� je vnucen po�adavek na pohyb v�tahu do p��zem� a je spu�t�no ��zen� pohybu v�tahu.  

Implement�cia projektu za pomoci FreeRTOS
======================================================

K implement�ci� riadenia v��ahu boli pou�it� 3 task-y.

1. *VytahTask* : je task, ktor� sa star� o to kam p�jde v��ah. Pracuje s dvoma poliami, v ktor�ch s� stla�en� tla�idla po porad�. V prvom poli sa nach�dzaj� stla�enia tla�idiel z kab�ny v��ahu. V druhom tla�idl� stla�en� na chodb�ch.Po stla�en� tla�idla sa v��ah presunie na dan� poschodie. Pod�a toho ko�ko poschod� potrebuje prejs�, tak dlho mu to trv�. Po dorazen�, sa otvoria dvere. V��ah sa pohybuje a� po zatvoren� dver�. 

2. *KeyboardHandler* : sl��i pre spracov�vanie stla�en� kl�vesnice. Spracov�van� tla�idla s� A-D pre jednotliv� poschodia z kab�ny. Tla�idla 2,5,8,0 pre tla�idla chodby. Tla�idlo # zablokuje v��ah, vtedy sa �ak� na stla�enie tla�idla * pre uvo�nenie v��ahu. Pri stla�en� in�ho tla�idla, sa vyp�e n�poveda.

3. *Print_pos* : sl��i pre zobrazenie aktu�lneho stavu na LCD display. Vypisuje aktu�lne poschodie a �i sa v��ah h�be alebo nie.  

::

    xTaskCreate(vytahTask, "Vytah", 50, NULL, 1, NULL);
    xTaskCreate(keyboardHandler, "KeybordHandler", 32, NULL, 1, NULL);
    xTaskCreate(print_pos, "Printer", 80, NULL, 1, NULL);


Pou�it� glob�lne premenn�
====================================================== 
1. **stop_button** - premenn� pre zaseknutie v��ahu
2. **pressed** - stla�enie nejak�ho tla�idla, �alej sa porovn�va o ak� i�lo
3. **kab_pos** - ukazovate� na �al�� prvok v poli tla�idiel kab�ny
4. **schody_pos** - ukazovate� na �al�� prvok v poli tla�idiel schodov
5. **kabina [4]** - pole stla�en�ch tla�idiel kab�ny
6. **schody [4]** - pole stla�en�ch tla�idiel schodov
7. **pos1** - ukazovate� na prvok, ktor� sa vykon� v poli tla�idiel kab�ny
8. **pos2** - ukazovate� na prvok, ktor� sa vykon� v poli tla�idiel schodov
9. **poschodie** - aktu�lne poschodie
10. **pohyb** - oznamuje �i sa v��ah h�be
11. **smer** - smer pohybu v��ahu
12. **dvere** - zobrazuje �i s� dvere otvoren�, alebo nie

�peci�lne znaky 
====================================================== 
1. **unsigned char sipka_hore[8]** = {0x04, 0x0e, 0x15, 0x04, 0x04, 0x04, 0x04, 0x04} - ��pka hore
2. **unsigned char sipka_dole[8]** = {0x04, 0x04, 0x04, 0x04, 0x04, 0x15, 0x0e, 0x04} - ��pka dole

Zobrazovanie aktu�lneho stavu
======================================================

Inform�cie sa zobrazuj� v okne termin�lu, ako aj na display Fitkitu. 

**Display:**

Na display Fitkitu sa zobrazi postupne : 

1. p�smeno **P**, ktor� zna�� poschodie a ��selna hodnota poschodia
2. p�smeno **S**, ktor� zna�� smer pohybu v��ahu. Hodnota - je, �e v��ah stoj�, ��pka hore a ��pka dole zna�ia pohyb v  smere ��pky
3. p�smeno **D** a symbol |x| alebo || zna�iaci otvoren�, alebo zatvoren� dvere

**Termin�l:**

Na obrazovku termin�lu sa zobrazuj� inform�cie o behu programu. Zobrazuj� sa sem napr�klad inform�cie o stla�en� tla�idla, �i o otv�ran� dver� a n�slednom zatv�ran�. �alej sa zobrazuje ak sa v��ah pohybuje.

Implement�cia v��ahu
======================================================
Po spusten� aplik�cie sa v��ah nach�dza na 1 poschod�. Po stla�en� tla�idla sa v��ah za�ne pres�va� na dan� poschodie, na display sa vypisuje postupne smer pohybu a poschodie kde sa pr�ve v��ah nach�dza. Znak dver� je zatvoren�. 

Po dojazde na poschodie sa smer zmen� na znak - (statie), po chv�ke sa otvoria dvere aby umo�nili n�stup os�b. N�sledne sa dvere zatvoria. Ak nie je stla�en� �iadne tla�idlo v��ah stoji na poslednom poschod�. Pri stla�en� poschodia , kde sa pr�ve nach�dza, sa otvoria dvere. In�� sa v��ah presunie na poschodie z ktor�ho bol privolan�.

Ak s� naraz stla�en� viacer� poschodia, v��ah vybavuje po�iadavky postupne ako pri�li.

Ak je po�as jazdy stla�en� tla�idlo **STOP** (#). V��ah sa zasekne a �ak� na opravu v podobe tla�idla (*). N�sledne sa v��ah zresetuje a presunie sa na 1 poschodie. Ak sa v��ah presunie na poschodie a vtedy niekto stla�� tla�idlo STOP, v��ah e�te otvor� dvere. Potom �ak� na opravu.   

Spustenie aplik�cie
========================
1. Aplik�ciu je nutn� skompilova�

2. N�sledne ju nahra� do Fitkitu

3. Aplik�cia sa ovl�da stl��an�m pr�slu�n�ch tla�idiel. Pri stla�en� tla�idla, ktor� sa nepou��va, sa vyp�e n�poveda.

Mo�n� vylep�enia do bud�cna
=============================
1. Algoritmus pre efekt�vnej�ie vybavovanie po�iadaviek
2. Vylep�enie v�pisu na display

