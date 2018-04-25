#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef int bool;
#define true 1
#define false 0
#define lava 1
#define prava 2
#define vrch 4


/*
 * Soubor:  proj3.c
 * Datum:   2014/12/13
 * Autor:   Maros Cocula, xcocul00@stud.fit.vutbr.cz
 * Projekt: Projekt 3 - Prechod bludiskom
 * Popis:   Program vyhladava cestu von z bludiska
 */
//**************************************************************************************************//
/* Funkcia pre zistenie spravnosti argumentov pre help */
int zistiHelp(int argc)
{
  if ((argc) > 2)                                                        //Za argumentom help uz nenasleduje ziaden dalsi argument
    {
   fprintf(stderr,"Pre zobrazenie napovedy zadajte argument --help\n");
     return 1;                                                           // Pri zle zadanom formate funkcie help sa zobrazi chybova hlaska 
    }
  else
     return 0;
}
//**************************************************************************************************//
 //funkcia pre vypis pomocnika
void help(){                                            
  fprintf(stdout,
"Argumenty programu:\n"
"*****************************************************************************\n"
"    --help zpĹŻsobĂ­, Ĺže program vytiskne nĂĄpovÄdu pouĹžĂ­vĂĄnĂ­ programu a skonÄĂ­.\n"
"*****************************************************************************\n"
"    --test pouze zkontroluje, Ĺže soubor danĂ˝ druhĂ˝m argumentem programu obsahuje ĹĂĄdnou definici mapy bludiĹĄtÄ.\n"
"      V pĹĂ­padÄ, Ĺže formĂĄt obrĂĄzku odpovĂ­dĂĄ definici (viz nĂ­Ĺže), vytiskne Valid.\n" 
"      V opaÄnĂŠm pĹĂ­padÄ (napĹ. ilegĂĄlnĂ­ch znakĹŻ, chybÄjĂ­cĂ­ch ĂşdajĹŻ nebo ĹĄpatnĂ˝ch hodnot) program tiskne Invalid.\n"
"*****************************************************************************\n"
"    --rpath hledĂĄ prĹŻchod bludiĹĄtÄm na vstupu na ĹĂĄdku R a sloupci C.\n"
"      PrĹŻchod hledĂĄ pomocĂ­ pravidla pravĂŠ ruky (pravĂĄ ruka vĹždy na zdi).\n"
"*****************************************************************************\n"
"    --lpath hledĂĄ prĹŻchod bludiĹĄtÄm na vstupu na ĹĂĄdku R a sloupci C, ale za pomoci pravidla levĂŠ ruky.\n"
"*****************************************************************************\n"
"    --shortest (prĂŠmiovĂŠ ĹeĹĄenĂ­) hledĂĄ nejkratĹĄĂ­ cestu z bludiĹĄtÄ pĹi vstupu na ĹĂĄdku R a sloupci C.\n");
}
//**************************************************************************************************//
//funkcia pre zistenie spravnosti zadanych argumentov
int zistiArgumenty(int argc, char *argv[])
{
  if(argc != 3 &&  strcmp(argv[1],"--test")==0)
    return 1;

  if(argc != 5 && (strcmp(argv[1],"--rpath")==0 || strcmp(argv[1],"--lpath")==0 || strcmp(argv[1],"--shortest")==0) )
    return 1;
  if ((atoi(argv[2]) == 0 || atoi(argv[3]) == 0) && ((strcmp(argv[1],"--rpath")==0 || strcmp(argv[1],"--lpath")==0)))
    return 1;
  
  return 0;
}
//**************************************************************************************************//
// vyhodnocovanie spravnosti argumentov
int kontrola(int argc, char* argv[])
{
   if (argc==1)                                                          //kontrola ak nebol zadany ziaden argument
    {
      fprintf(stderr,"Nezadali ste argumenty\n");
      return -1;
    }
   if ((strcmp(argv[1],"--test")!=0) && (strcmp(argv[1],"--rpath")!=0) && (strcmp(argv[1],"--lpath")!=0) && (strcmp(argv[1],"--shortest")!=0))
     {
       fprintf(stderr,"Zadali ste nespravne argumenty pre napovedu pouzite argument --help\n");
       return -1;
     }
 if(strcmp(argv[1],"--help")==0)
    {
      if(zistiHelp(argc)==0)
	help();
      return -1;
    }
 if(zistiArgumenty(argc,argv)==1)
   {
     fprintf(stderr,"Zadali ste nespravne argumenty pre napovedu pouzite argument --help\n");
     return -1;
   }
 
 return 0;
}
//**************************************************************************************************//
//definovanie struktury Map
typedef struct{
  int rows;                                                                   //riadky
  int cols;                                                                   //stlpce
  unsigned char *cells;                                                       //ukazovatel na prvok
}Map;
//**************************************************************************************************//
// alokacia pola na zasobnik
int alokacia(Map *map, int rows, int cols)
{
  map -> cells = malloc(rows*cols*sizeof(unsigned char*));              //alokacia potrebnej velkosti
  if (map -> cells == NULL)
    return -1;
  return 0;
}
//**************************************************************************************************//
//uvolnenie alokovanej pamati
void uvolnenie (Map *map)
{
  free(map ->cells);
  map -> cells=NULL;
}
//**************************************************************************************************//
// nacitanie suboru do struktury Map
int nacitanie (FILE *subor, Map *map)
{
  int chybova;
  chybova= fscanf (subor, "%d %d", &map -> rows, &map -> cols);               //nacitanie rows a cols
  if (alokacia(map,map->rows,map->cols) != 0 )
    return chybova;
  if(chybova!=2)
    {
    fprintf(stderr,"Zle parametre suboru\n");
    return chybova;
    }
  
  int i;
  for(i=0;i< map->rows * map->cols; i++)
    {
      fscanf(subor, "%s", &map->cells[i]);                             //nacitanie jednotlivych prvkov
        if(map->cells[i]== '\0')
	  {
	    fprintf(stderr,"Zle parametre suboru\n");
	    return chybova;
	  }
	}


 return 0;
}
//**************************************************************************************************//
// funkcia vrati hodnotu ci je dotazovana stena volna
bool isborder(Map *map, int r, int c,int border)
{
  if((map->cells[r*map->cols+c] & border)==border)
    return true;                                                   //stena sa nachadza na danej pozicii
  else
    return false;                                                  //stena sa nenachadza na pozicii
}
//**************************************************************************************************//
//funkcia zistuje ci je matica v subore validna
int test(Map *map)
{
  int riadok,stlpec;            //ak sa v matici nachadza cislo ktore neni v rozsahu <0,7>, alebo znak
  for(int i=0;i<map->rows * map->cols;i++)
    {
     if((map->cells[i] > '7') || (map->cells[i] < '0'))
       {
       return 1;
       }}

  for(riadok=0;riadok < map->rows; riadok++)
    {
      for(stlpec=0;stlpec < map->cols; stlpec++)  //dve vedla seba leziace bunky musia mat steny na rovnakej strane
	{
   	  if(((stlpec+1)%(map->cols)==0) || ((isborder(map,riadok,stlpec,prava)) == (isborder(map,riadok,stlpec+1,lava))))
	    { 
	       if (((riadok%2==0) && (stlpec%2 !=0)) || ((riadok%2 !=0) && (stlpec%2 == 0)))
		 {
		   if (riadok !=  (map->rows)-1)
		     {
		   if((isborder(map,riadok,stlpec,vrch)) != (isborder(map,riadok+1,stlpec,vrch)))
		     return 1;
		     }
		 }
	    }
	
	  else 
	    return 1;
	}
    }
  
  return 0;
}
//**************************************************************************************************//
//vrati pravidlo podla ktoreho sa riadime
int vratRuku(char *argv[])
{
  if (strcmp(argv[1],"--rpath")==0)
    return 1;
  else 
    return 0;
}
//**************************************************************************************************//
//vrati dotazovanu stenu pri zaciatku pocitania
int start_border(Map *map, int r, int c, int leftright)
{

  if((r==1 && c==1) || (c==1 && r==map->rows))  //lavy horny a lavy dolny roh
    return prava;
  if ((c%2!=0) && (r==1) && (c==map->cols))     //pravy horny roh okrem lpath ak su stlpce parne
    return lava;
  if (r==map->rows && c==map->cols && c%2!=0)   //pravy dolny roh okrem rpath ak su stlpce parne
    return lava;
  
  if(leftright==1)
    { 
      if (r==map->rows && c==map->cols && c%2==0) 
	return vrch;
      if(r==1)                          // zhora
        {
	  if (c%2!=0 || (c==map->cols))
	    {
	      return lava;
	    }
	  else 
	    return 5;
	}
      if(r==map->rows)                  //zdola
	{
	  if(c%2!=0)
	    {
	      return prava;
	    }
	  else
	    return 5;
	}
      if(c==1)                         //zlava
	{
	  if(r%2==0)
	    {
	      return vrch;
	    }
	  else
	    return prava;
	}
      if(c==map->cols)                 //zprava
	{
	  if(r%2==0)
	    {
	      return vrch;
	    }
	  else
	    return lava;
	}
    }
  //-----------------------------------------------------------------------------------//
  if(leftright==0)
    { 
      if (r==1 && c==map->cols && c%2==0)  //pravy horny roh
	return vrch;
      if (r==map->rows && c==map->cols && c%2==0)
	return 1;
      if(r==1)                          // zhora
        {
	  if (c%2!=0)
	    {
	      return prava;
	    }
	  else 
	    return 5;
	}
      if(r==map->rows )                  //zdola
	{
	  if(c%2!=0)
	    {
	      return lava;
	    }
	  else
	    return 5;
	}
      if(c==1)                         //zlava
	{
	  if(r%2==0)
	    {
	      return prava;
	    }
	  else
	    return vrch;
	}
      if(c==map->cols)                 //zprava
	{
	  if(r%2==0)
	    {
	      return lava;
	    }
	  else
	    return vrch;
	}
    }
  return 0;
  }
//**************************************************************************************************//
//zisti ci sa vstup nachadza v mape
int kontrolujVstup(char *argv[],Map *map)
{
  if(map->rows < atoi(argv[2]) || map->cols < atoi(argv[3]) )
    return 1;
  else
  return 0;
}
//**************************************************************************************************//
int rpath(Map *map,char *argv[], int border)
{ 
  int riadok=atoi(argv[2]), stlpec=atoi(argv[3]);
  int predosla=border;

  printf("%d,%d\n",riadok,stlpec);

  if(riadok==1 && stlpec==1)
    {
    if(isborder(map,riadok,stlpec,border)!=1)
      {
      stlpec++;
      }
    }
  if(riadok==map->rows && stlpec==1)
    {
    if(isborder(map,riadok,stlpec,border)!=1)
      stlpec++;
    else
      return 1;
    }
  if(riadok==1 && stlpec==map->cols)
    {
    if(isborder(map,riadok,stlpec,border)==0)
      stlpec--;
    else 
      return 1;
    }
  if(riadok==map->rows && stlpec==map->cols)
    {
    if(isborder(map,riadok,stlpec,border)==0)
      {
      if(stlpec%2==0)
	stlpec--;
      else
	riadok--;
      }
    else
      return 1;
    }
  //****************************************************************//koniec rohovych
  if(riadok%2==0 && stlpec==1)
    {
      if(isborder(map,riadok,stlpec,border)==0)
	riadok ++;
    }
  if(riadok%2!=0 && stlpec==1)
    {
      if(isborder(map,riadok,stlpec,border)==1)
	stlpec++;
    }
  if(stlpec%2!=0)
    {
      if(riadok==1)
	{
	  if(isborder(map,riadok,stlpec,border)==0)
	    stlpec--;
	}
      if(riadok==map->rows)
	{
	  if(isborder(map,riadok,stlpec,border)==0)
	    stlpec++;
	}  
    }
  if(riadok%2==0 && stlpec==map->cols)
    {
    if(map->cols%2==0)
    {
      if(isborder(map,riadok,stlpec,border)==0)
	riadok--;
    }
    if(map->cols%2!=0)
      {
	if(isborder(map,riadok,stlpec,border)==0)
	  stlpec--;
      }
    }
  if(riadok%2!=0 && stlpec==map->cols)
    {
    if(map->cols%2==0)
    {
      if(isborder(map,riadok,stlpec,border)==0)
	stlpec--;
    }
    if(map->cols%2!=0)
      {
	if(isborder(map,riadok,stlpec,border)!=0)
	  riadok--;
      }
    }
    
  //***********************************************************************//koniec zaciatocnych
  printf("%d,%d\n",riadok,stlpec);
  //***********************************************************************//koniec zaciatocnych
  while(riadok>1 && stlpec>1 && riadok <= (map->rows) && stlpec<= (map->cols))
    { 
      if((riadok+stlpec)%2==0)
	{
	  switch (predosla) {
	  case 1 : predosla= prava;
	           if(isborder(map,riadok,stlpec,predosla)==0)
		     stlpec++;
		   else
		     {
		     predosla=vrch;
		     if(isborder(map,riadok,stlpec,predosla)==0)
		       riadok++;
		     else
		       {
			predosla=lava;
			if(isborder(map,riadok,stlpec,predosla)==0)
			  stlpec--;
		       }
		     }
		   printf("%d,%d\n",riadok,stlpec);
		   break;
	  case 2 : predosla= vrch;
	           if(isborder(map,riadok,stlpec,predosla)==0)
		     riadok--;
		    else
		     {
		     predosla=lava;
		     if(isborder(map,riadok,stlpec,predosla)==0)
		       stlpec--;
		     else
		       {
			predosla=prava;
			if(isborder(map,riadok,stlpec,predosla)==0)
			  stlpec++;
		       }
		     }
		   printf("%d,%d\n",riadok,stlpec);
		   break;
	  case 4 : predosla= lava;
	           if(isborder(map,riadok,stlpec,predosla)==0)
		     stlpec--;
		    else
		     {
		     predosla=prava;
		     if(isborder(map,riadok,stlpec,predosla)==0)
		       stlpec++;
		     else
		       {
			predosla=vrch;
			if(isborder(map,riadok,stlpec,predosla)==0)
			  riadok--;
		       }
		     }
		   printf("%d,%d\n",riadok,stlpec);
		   break;
	  default : return 5; break;
	  }
	}
      if((riadok+stlpec)%2!=0)
	{ 
	  switch (predosla) {
	  case 1 : predosla= vrch;
	           if(isborder(map,riadok,stlpec,predosla)==0)
		     riadok++;
		    else
		     {
		     predosla=prava;
		     if(isborder(map,riadok,stlpec,predosla)==0)
		       stlpec++;
		     else
		       {
			predosla=lava;
			if(isborder(map,riadok,stlpec,predosla)==0)
			  stlpec--;
		       }
		     }
		   printf("%d,%d\n",riadok,stlpec);
		   break;
	  case 2 : predosla= lava;
	           if(isborder(map,riadok,stlpec,predosla)==0)
		     stlpec--;
		    else
		     {
		     predosla=vrch;
		     if(isborder(map,riadok,stlpec,predosla)==0)
		       riadok++;
		     else
		       {
			predosla=prava;
			if(isborder(map,riadok,stlpec,predosla)==0)
			  stlpec++;
		       }
		     }
		   printf("%d,%d\n",riadok,stlpec);
		   break;
	  case 4 : predosla= prava;
	           if(isborder(map,riadok,stlpec,predosla)==0)
		     stlpec++;
		    else
		     {
		     predosla=lava;
		     if(isborder(map,riadok,stlpec,predosla)==0)
		       stlpec--;
		     else
		       {
			predosla=vrch;
			if(isborder(map,riadok,stlpec,predosla)==0)
			  riadok++;
		       }
		     }
		   printf("%d,%d\n",riadok,stlpec);
		   break;
	  default : return 5; break;
	  }
	}
    }	 
	  
  return 0;
}
//**************************************************************************************************//
// hlavna funkcia
int main (int argc, char * argv[])
{
  if(kontrola(argc,argv)!=0)
    return -1;
 Map map;

 FILE *subor = fopen(argv[argc-1],"r");                                             //otvorenie suboru
 if (subor == NULL)
   {
     fprintf(stderr,"Subor sa nepodarilo nacitat\n");
     return -1;
   }
 if(nacitanie(subor, &map)!=0)
   return -1;

 fclose(subor);                                                                     //zatvorenie suboru

 if(strcmp(argv[1],"--test")==0)
   {
     if(test(&map)==0)
       fprintf(stdout,"Valid\n");
     else
       fprintf(stdout,"Invalid\n");
      uvolnenie(&map); 
     return 0;
   }
 if (strcmp(argv[1],"--rpath")==0 ||strcmp(argv[1],"--lpath")==0)
   {
     if(kontrolujVstup(argv,&map)==0)
       {
	 if(test(&map)==0)
	   {
	     if(start_border(&map,atoi(argv[2]),atoi(argv[3]),vratRuku(argv))==5)
	       {
		 fprintf(stderr,"Do bludiska sa neda vojst zo zadaneho miesta\n");
		 uvolnenie(&map);
		 return 0;
	       }
	     else
	       {
		 if(strcmp(argv[1],"--rpath")==0)
		   rpath(&map,argv,start_border(&map,atoi(argv[2]),atoi(argv[3]),vratRuku(argv)));
		 if(strcmp(argv[1],"--lpath")==0)
		   printf("Lava\n");
	       }
	   }
	 else
	   {
	   fprintf(stderr,"Mapa nie je validna\n");
	   uvolnenie(&map);
	   return 0;
	   }}
     else
       {
	 fprintf(stderr,"Vstup bludiska sa nenachadza v mape\n");
	 uvolnenie(&map);
	 return 0;
       }
   }
  uvolnenie(&map);                                                         //uvolnenie alokovanej pamate
return 0;
}
