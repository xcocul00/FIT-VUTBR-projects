#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Soubor:  proj1.c
 * Datum:   2014/10/10
 * Autor:   Maros Cocula, xcocul00@stud.fit.vutbr.cz
 */

//--------------------------------------------------------------------------------------------------------------//

void Help ()    //void Help zobrazi po zadani argumentu --help napovedu k programu
{
  fprintf (stdout,
 "--------------------------------------------------------------------------------------------------------------------------------------------\n"  
 " Argumenty programu:\n"
 "\n"
 "\n"
 " operace reprezentuje jednu z následujících operací:\n"
 "\n"
 "select značí operaci, která z dané tabulky pouze vybere a následně vytiskne hodnoty daných buněk,\n"
 "min značí vyhledání a následný tisk minimální hodnoty z daného rozsahu buněk,\n"
 "max značí vyhledání a následný tisk maximální hodnoty z daného rozsahu buněk,\n"
 "sum značí výpočet a následný tisk sumy hodnot všech vybraných buněk,\n"
 "avg značí výpočet a následný tisk aritmetického průměru vybraných buněk.\n"
 "\n" 
 "výběr_buněk reprezentuje rozsah tabulky, nad kterým má být provedena daná operace. Rozsah je definován jedním z následujících argumentů:\n"
 "\n"
 "row X značí výběr všech buněk na řádku X (X > 0),\n"
 "col X značí výběr všech buněk ve sloupci X (X > 0),\n"
 "rows X Y značí výběr všech buněk od řádku X (včetně) až po Y (včetně). 0 < X <= Y.\n"
 "cols X Y značí výběr všech buněk od sloupce X (včetně) až po Y (včetně). 0 < X <= Y.\n"
 "range A B X Y značí výběr buněk od řádku A po řádek B a od sloupce X po sloupec Y (včetně daných řádků a sloupců). 0 < A <= B, 0 < X <= Y.\n"
"---------------------------------------------------------------------------------------------------------------------------------------------\n"
	  );
}

//--------------------------------------------------------------------------------------------------------------//
int zistiArgumenty(int argc, char *argv[])      //Funkcia zistiArgumenty zisti ci boli argumenty spravne zadane
{  
  if (strcmp(argv[2],"row")==0 || strcmp(argv[2],"col")==0 )   // zistovanie spravnosti pre argumenty row a col
    {
      if (argc < 5 && argc > 3)
	{ if (atoi(argv[3])>0)
             return 1;
	}
    }
  else  if (strcmp(argv[2],"rows")==0 || strcmp(argv[2],"cols")==0 )    //zistovanie spravnosti pre argumenty rows a cols
    {
      if (argc < 6 && argc > 4)
	{ if (atoi(argv[3])>0 && atoi(argv[4])>0)
	      return 1;
	}
    }
  else  if (strcmp(argv[2],"range")==0 )                                //zistovanie spravnosti pre argument range
    {
      if (argc < 8 && argc > 6)
	{ if (atoi(argv[3])>0 && atoi(argv[4])>0)
	      return 1;
	}
    }
  fprintf(stderr,"Zadali ste nespravne argumenty, pre zobrazenie napovedy pouzite argument --help \n");                                                                                                                              // ak boli argumenty zadane nespravne zobrazi sa chybova hlaska
    return 0;
}
//--------------------------------------------------------------------------------------------------------------//
int zistiArgumentyHelp(int argc)                                                //zistovanie spravnsoti pre argument --help
{
  if ((argc) > 2)
    {
      fprintf(stderr,"Pre zobrazenie napovedy zadajte argument --help" );       //ak bol argument zadany zle vypise sa chybova hlaska
      return 1;
    }
  else
      return 0;
}


//--------------------------------------------------------------------------------------------------------------//
void funkcia(char *argv[])          //procedura pre vypocet vsetkych pozadovanych operacii
{
  char *slovo;                       //definovanie premennych
  char riadok[1024];
  int i,pomocna,pocet=0,pomoc,priznak=0;        
  int a,b,c,d;
  double priznakCisla,min=0,max,suma=0;
  
  if (strcmp(argv[1],"select")==0) priznak=1;           //nastavenie pomocnych premennych, potrebnych pre vypocty
  a=atoi(argv[3]);
  b=a;
    if (strcmp(argv[2],"rows")==0 || strcmp(argv[2],"cols")==0)
    {
      b=atoi(argv[4]);
         if (a>=b)
      	fprintf(stderr,"Chybne zadane argumenty\n");
    }
  if (strcmp(argv[2],"range")==0)
    {
      b=atoi(argv[4]);
      c=atoi(argv[5]);
      d=atoi(argv[6]);
      if (a>=b || c>=d)
	fprintf(stderr,"Chybne zadane argumenty\n");
    }  
//---------------------------------------------------------------------//
  if ((strcmp(argv[2],"row")==0) || (strcmp(argv[2],"rows")==0))       //vypocty pre argumenty row a rows
	{
       	  for(i=1; i<=b; i++)
	  {
	    if (fgets(riadok,1023,stdin)==NULL)                       //zistovanie ci riadok nie je prazdny
	   {      
	      fprintf(stderr,"Zadany riadok neexistuje\n");
	      break;
	   }
	    pomoc=0;
	    slovo=strtok (riadok," \t");                              //rozkuskovanie riadka na slova
	     if (i>=a && i<=b)
	      while (slovo != NULL)
	    {
	      priznakCisla = strtod(slovo,&slovo);                    //rozkuskovanie slova na cislo a slovo
	        if(*slovo == '\0' || *slovo == 10)          
		  { if (strcmp(argv[1],"select")==0)
		      { 
			fprintf(stdout,"%.10g\n",priznakCisla);       //vypis slova ak je cislo
		      }
		    if (pocet==0)        { max=min=priznakCisla;}     //vypocty potrebne pre min,max,avg a sum
		    suma+=priznakCisla;
		    if(priznakCisla<min) {min=priznakCisla;}
		    if(priznakCisla>max) {max=priznakCisla;}
		    pocet++;
		    pomoc++;
		  } else if (priznak==1)                              //vypis slova ak nie je cislom
                    fprintf(stdout,"%s\n",slovo);
		slovo = strtok( NULL ," \t");                         //preskocenie medzery medzi slovami
	    }
	  }
	  if (pomoc==0){ fprintf(stderr, "V riadku nie je ziadne cislo\n");} //vypis chybovej hlasky ak pri pocitani v riadku nie je ziadne cislo
	} 
//--------------------------------------------------------------------//
  if ((strcmp(argv[2],"col")==0) || (strcmp(argv[2],"cols")==0))      //vypocty pre argumenty col a cols
    {
      
      while(1){	
	 if (fgets(riadok,1023,stdin)==NULL)                          //zistovanie ci riadok nie je prazdny
	   {      
	     break;
	   } 
	pomocna=1;
	pomoc=0;
	slovo=strtok (riadok, " \t");                                 //rozkuskovanie riadka na slova
	while(slovo!=NULL)
	  {
	  if (pomocna>=a && pomocna<=b)
	    { 
	        priznakCisla = strtod(slovo,&slovo);                  //rozkuskovanie slova na cislo a slovo
                if(*slovo == '\0' || *slovo == 10)
                  {if (strcmp(argv[1],"select")==0)
		      {
		      fprintf(stdout,"%.10g\n",priznakCisla);         //vypis slova ak je cislo
		      }
		    if (pocet==0)             {max=min=priznakCisla;} //vypocty potrebne pre min,max,avg a sum
		    suma+=priznakCisla;
		    if(priznakCisla<min)      {min=priznakCisla;}
		    if(priznakCisla>max)      {max=priznakCisla;}
		    pocet++;
		    pomoc++;
		  } else if(priznak==1)
                  {  fprintf(stdout,"%s\n",slovo);                     //vypis slova ak nie je cislom
		    }
	    }
	  slovo=strtok(NULL," \t");                                    //preskocenie medzery medzi slovami
	  pomocna++;
	  }
	if(pomocna-1<b)
	  {
	    fprintf(stderr,"Zadany stlpec neexistuje\n");              //chybova hlaska ak zadany stlpec sa nenachadza v subore
	  break;
	  }
	if (strcmp(argv[1],"select")!=0 && pomoc==0){fprintf(stderr, "V stlpci nie je ziadne cislo\n");break;} 
	                                                               //chybova hlaska ak v pocitanom stlpci sa nenachadza ziadne cislo
              }
    }
//---------------------------------------------------------------------//
 if ((strcmp(argv[2],"range")==0))                                     //vypocty pre argument range
   { 
      for(i=1; i<=b; i++)
	{
	  pomocna=1;
	  pomoc=0;
	  if (fgets(riadok,1023,stdin)==NULL)                          //zistovanie ci riadok nie je prazdny
	   {  
	     fprintf(stderr,"Zadany riadok neexistuje\n");
	     break;	     
	    } 
	  slovo= strtok (riadok," \t");                                //rozkuskovanie riadka na slova
	  if (i>=a && i<=b)
	    {
	      while(slovo!=NULL)
		{
		  if (pomocna>=c && pomocna<=d)
		    {
		      priznakCisla= strtod(slovo,&slovo);              //rozkuskovanie slova na cislo a slovo
		      if(*slovo == '\0' || *slovo == 10)
			{ if (strcmp(argv[1],"select")==0)
			    {
			  fprintf(stdout,"%.10g\n",priznakCisla);      //vypis slova ak je cislo
			    }
			  if (pocet==0)            {max=min=priznakCisla;} //vypocty potrebne pre min,max,avg a sum
			  suma+=priznakCisla;
			  if(priznakCisla<min)     {min=priznakCisla;}
			  if(priznakCisla>max)     {max=priznakCisla;}
			  pocet++;
			  pomoc++;
			} else if(priznak==1)
			{fprintf(stdout,"%s\n",slovo);}                 //vypis slova ak nie je cislom
		    }
		  slovo=strtok(NULL," \t");                             //preskocenie medzery medzi slovami
		  pomocna++;
		}
	      if(pomocna-1<d)
		{
		  fprintf(stderr,"Zadany stlpec neexistuje\n");        //chybova hlaska ak sa pozadovany stlpec nenachadza v subore 
		break;
		}
	      if (strcmp(argv[1],"select")!=0 && pomoc==0){fprintf(stderr, "Vas vyber neobsahuje ziadne cislo\n");break;}
	    }                                                         //chybova hlaska ak sa v pozadovanom vybere nenachadza ziadne cislo
	}
   }
 //-------------------------------------------------------------------------------//
 if(strcmp(argv[1],"min")==0) fprintf(stdout,"Minimum je %.10g\n",min);        //vypis pozadovanej funkcie na obrazovku
else if(strcmp(argv[1],"max")==0) fprintf(stdout,"Maximum je %.10g\n",max);
else if(strcmp(argv[1],"sum")==0) fprintf(stdout,"Sucet je %.10g\n",suma);
else if(strcmp(argv[1],"avg")==0) 
  {
    if(suma!=0)
     fprintf(stdout,"Priemer je %.10g\n",suma/pocet);
 else fprintf(stdout,"Priemer je %.10g\n",suma); 
  }
}

//--------------------------------------------------------------------------------------------------------------//

int main(int argc, char *argv[])
{
  if(argc==1)                                                      //chybova hlaska ak nebol zadany ziaden argument
    {
       fprintf(stderr,"Nezadali ste ziaden argument, pre zobrazenie napovedy pouzite argument --help \n");
    }
else  
  if (strcmp("--help", argv[1]) == 0)                               //zistenie zadaneho argumentu --help
   {
     if((zistiArgumentyHelp(argc))== 0)                             //vyvolanie funkcie void Help()
        Help();
   }
else
  if (zistiArgumenty(argc,argv)==0)                                //vyvolanie funkcie pre zistiArgumenty pre spravnost argumentov 
       return 0;
     else
       funkcia(argv);                                              //vyvolanie procedury pre pocitanie pozadovanej funkcie
return 0;
}
