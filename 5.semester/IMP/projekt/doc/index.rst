:author: Maro¹ Cocuµa < xcocul00@stud.fit.vutbr.cz >
:updated: 2016/12/14

    

==============================================================
IMP - Riadenie vý»ahu
==============================================================


.. contents:: Obsah

Zadanie projektu
==================

V jazyce C naimplementujte pro MCU øady MSP430x na FITkitu aplikaci (tj. programovou realizaci øadièe) pro øízení provozu výtahu ve vícepodla¾ní budovì - bli¾¹í specifikace viz ní¾e (pou¾ití vestavného OS pøi implementaci je nepovinné). Jádrem Va¹í realizace nech» je událostmi øízený mechanismus (napø. koneèný stavový automat) schopný na základì pøíslu¹né události a (vnitøního) stavu aplikace vyvolat pøíslu¹nou reakci, tj. zmìnu dosavadního chování a stavu aplikace. V rámci realizace tedy, mj., peèlivì vymezte a zdokumentujte vstupy a výstupy tohoto mechanismu, mno¾inu vnitøních stavù aplikace a podmínek pøechodù mezi nimi. 

**Detaily k zadání**

    Budova má celkem 4 podla¾í: sklep (ID = -1), 1. nadzemní podla¾í (pøízemí, ID = 1),  2. nadzemní podla¾í (pokoje, ID = 2) a 3. nadzemní podla¾í (pùda, ID = 3).
    V ka¾dém podla¾í budovy jsou výtahové dveøe, u nich¾ je umístìno tlaèítko pro pøivolání výtahu a indikátor aktuální èinnosti výtahu (pohyb nahoru, dolù, stání, apod.).
    Výtah se smí pohypovat pouze tehdy, jsou-li v¹echny dveøe uzavøeny. 
    Po stisku tlaèítka pro pøivolání výtahu v daném patøe výtah tento po¾adavek zaregistruje a po dobì nezbytnì nutné k vyøízení ji¾ pøijatých po¾adavkù v tomto patøe zastaví. 
    Po zastavení výtahu v daném patøe se v tomto patøe po krátkém èekání otevøou dveøe umo¾òující pøístup k výtahové kabinì. Dveøe zùstanou urèitou dobu otevøené a poté se zaènou pomalu zavírat. Existují-li nevyøízené po¾adavky na pohyb výtahu, výtah se po krátké dobì èekání zaène pohybovat ve smìru zaji¹»ujícím vyøízení po¾adavkù. Pokud se výtah je¹tì nezaèal pohybovat a bylo stisknuto tlaèítko aktuálního podla¾í, výtah se chová jako by v daném patøe zastavil znovu.
    V kabinì je mo¾né pomocí tlaèítek volit podla¾í, ve kterých má výtah pøi svém pohybu ¹achtou zastavit. Dosud nevyøízené po¾adavky na zastavení v daných podla¾ích, volené tlaèítky v kabinì nebo tlaèítky pro pøivolání výtahu v jednotlivých podla¾ích, jsou signalizovány u pøíslu¹ných tlaèítek. Tlaèítko Stop, umístìné v kabinì, slou¾í k neprodlenému zastavení pohybujícího se výtahu.
    Informace o aktuálním podla¾í (my¹lena ID uvedená vý¹e), na kterém se výtah nachází, je vyobrazena na speciálním displeji, pøièem¾ hodnota na displeji se aktualizuje v¾dy pøedtím, ne¾ se zaènou otevírat dveøe daného podla¾í nebo tehdy, zmìní-li výtah pøi pohybu ¹achtou podla¾í (tj., dostane-li se podlaha výtahu do jedné roviny s podlahou pøíslu¹ného podla¾í).  
    Výtah, zastavený tlaèítkem Stop, se zaène pohybovat a¾ poté, co je stisknuto tlaèítko servisního technika (Opraveno): po stisku tlaèítka Opraveno jsou ve¹keré nevyøízené po¾adavky na pøivolání výtahu smazány, poté je vnucen po¾adavek na pohyb výtahu do pøízemí a je spu¹tìno øízení pohybu výtahu.  

Implementácia projektu za pomoci FreeRTOS
======================================================

K implementácií riadenia vý»ahu boli pou¾ité 3 task-y.

1. *VytahTask* : je task, ktorý sa stará o to kam pôjde vý»ah. Pracuje s dvoma poliami, v ktorých sú stlaèené tlaèidla po poradí. V prvom poli sa nachádzajú stlaèenia tlaèidiel z kabíny vý»ahu. V druhom tlaèidlá stlaèené na chodbách.Po stlaèení tlaèidla sa vý»ah presunie na dané poschodie. Podµa toho koµko poschodí potrebuje prejs», tak dlho mu to trvá. Po dorazení, sa otvoria dvere. Vý»ah sa pohybuje a¾ po zatvorení dverí. 

2. *KeyboardHandler* : slú¾i pre spracovávanie stlaèení klávesnice. Spracovávané tlaèidla sú A-D pre jednotlivé poschodia z kabíny. Tlaèidla 2,5,8,0 pre tlaèidla chodby. Tlaèidlo # zablokuje vý»ah, vtedy sa èaká na stlaèenie tlaèidla * pre uvoµnenie vý»ahu. Pri stlaèení iného tlaèidla, sa vypí¹e nápoveda.

3. *Print_pos* : slú¾i pre zobrazenie aktuálneho stavu na LCD display. Vypisuje aktuálne poschodie a èi sa vý»ah hýbe alebo nie.  

::

    xTaskCreate(vytahTask, "Vytah", 50, NULL, 1, NULL);
    xTaskCreate(keyboardHandler, "KeybordHandler", 32, NULL, 1, NULL);
    xTaskCreate(print_pos, "Printer", 80, NULL, 1, NULL);


Pou¾ité globálne premenné
====================================================== 
1. **stop_button** - premenná pre zaseknutie vý»ahu
2. **pressed** - stlaèenie nejakého tlaèidla, ïalej sa porovnáva o aké i¹lo
3. **kab_pos** - ukazovateµ na ïal¹í prvok v poli tlaèidiel kabíny
4. **schody_pos** - ukazovateµ na ïal¹í prvok v poli tlaèidiel schodov
5. **kabina [4]** - pole stlaèených tlaèidiel kabíny
6. **schody [4]** - pole stlaèených tlaèidiel schodov
7. **pos1** - ukazovateµ na prvok, ktorý sa vykoná v poli tlaèidiel kabíny
8. **pos2** - ukazovateµ na prvok, ktorý sa vykoná v poli tlaèidiel schodov
9. **poschodie** - aktuálne poschodie
10. **pohyb** - oznamuje èi sa vý»ah hýbe
11. **smer** - smer pohybu vý»ahu
12. **dvere** - zobrazuje èi sú dvere otvorené, alebo nie

©peciálne znaky 
====================================================== 
1. **unsigned char sipka_hore[8]** = {0x04, 0x0e, 0x15, 0x04, 0x04, 0x04, 0x04, 0x04} - ¹ípka hore
2. **unsigned char sipka_dole[8]** = {0x04, 0x04, 0x04, 0x04, 0x04, 0x15, 0x0e, 0x04} - ¹ípka dole

Zobrazovanie aktuálneho stavu
======================================================

Informácie sa zobrazujú v okne terminálu, ako aj na display Fitkitu. 

**Display:**

Na display Fitkitu sa zobrazi postupne : 

1. písmeno **P**, ktoré znaèí poschodie a èíselna hodnota poschodia
2. písmeno **S**, ktoré znaèí smer pohybu vý»ahu. Hodnota - je, ¾e vý»ah stojí, ¹ípka hore a ¹ípka dole znaèia pohyb v  smere ¹ípky
3. písmeno **D** a symbol |x| alebo || znaèiaci otvorené, alebo zatvorené dvere

**Terminál:**

Na obrazovku terminálu sa zobrazujú informácie o behu programu. Zobrazujú sa sem napríklad informácie o stlaèení tlaèidla, èi o otváraní dverí a následnom zatváraní. Ïalej sa zobrazuje ak sa vý»ah pohybuje.

Implementácia vý»ahu
======================================================
Po spustení aplikácie sa vý»ah nachádza na 1 poschodí. Po stlaèení tlaèidla sa vý»ah zaène presúva» na dané poschodie, na display sa vypisuje postupne smer pohybu a poschodie kde sa práve vý»ah nachádza. Znak dverí je zatvorený. 

Po dojazde na poschodie sa smer zmení na znak - (statie), po chvíµke sa otvoria dvere aby umo¾nili nástup osôb. Následne sa dvere zatvoria. Ak nie je stlaèené ¾iadne tlaèidlo vý»ah stoji na poslednom poschodí. Pri stlaèení poschodia , kde sa práve nachádza, sa otvoria dvere. Ináè sa vý»ah presunie na poschodie z ktorého bol privolaný.

Ak sú naraz stlaèené viaceré poschodia, vý»ah vybavuje po¾iadavky postupne ako pri¹li.

Ak je poèas jazdy stlaèené tlaèidlo **STOP** (#). Vý»ah sa zasekne a èaká na opravu v podobe tlaèidla (*). Následne sa vý»ah zresetuje a presunie sa na 1 poschodie. Ak sa vý»ah presunie na poschodie a vtedy niekto stlaèí tlaèidlo STOP, vý»ah e¹te otvorí dvere. Potom èaká na opravu.   

Spustenie aplikácie
========================
1. Aplikáciu je nutné skompilova»

2. Následne ju nahra» do Fitkitu

3. Aplikácia sa ovláda stláèaním príslu¹ných tlaèidiel. Pri stlaèení tlaèidla, ktoré sa nepou¾íva, sa vypí¹e nápoveda.

Mo¾né vylep¹enia do budúcna
=============================
1. Algoritmus pre efektívnej¹ie vybavovanie po¾iadaviek
2. Vylep¹enie výpisu na display

