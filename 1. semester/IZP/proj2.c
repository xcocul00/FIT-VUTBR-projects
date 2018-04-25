#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
 * Soubor:  proj1.c
 * Datum:   2014/11/21
 * Autor:   Maros Cocula, xcocul00@stud.fit.vutbr.cz
 * Projekt: Projekt 2 - Iteracne vypocty
 * Popis:   Program vypocitava vzdialenost predmetu a pocita tangens
 */

void help(){                                             //funkcia pre vypis pomocnika
  fprintf(stdout,
"Argumenty programu:\n"

"    --help zpĹŻsobĂ­, Ĺže program vytiskne nĂĄpovÄdu pouĹžĂ­vĂĄnĂ­ programu a skonÄĂ­.\n"
"    --tan srovnĂĄ pĹesnosti vĂ˝poÄtu tangens Ăşhlu A (v radiĂĄnech) mezi volĂĄnĂ­m tan z matematickĂŠ knihovny, a vĂ˝poÄtu tangens pomocĂ­ Taylorova\n"
" polyno mu a zĹetÄzenĂŠho zlomku. Argumenty N a M udĂĄvajĂ­, ve kterĂ˝ch iteracĂ­ch iteraÄnĂ­ho vĂ˝poÄtu mĂĄ srovnĂĄnĂ­ probĂ­hat. 0 < N <= M < 14\n"
"    -m vypoÄĂ­tĂĄ a zmÄĹĂ­ vzdĂĄlenosti.\n"
"        Ăhel Îą (viz obrĂĄzek) je dĂĄn argumentem A v radiĂĄnech. Program vypoÄĂ­tĂĄ a vypĂ­ĹĄe vzdĂĄlenost mÄĹenĂŠho objektu. 0 < A <= 1.4 < Ď/2.\n"
"        Pokud je zadĂĄn, Ăşhel Î˛ udĂĄvĂĄ argument B v radiĂĄnech. Program vypoÄĂ­tĂĄ a vypĂ­ĹĄe i vĂ˝ĹĄku mÄĹenĂŠho objektu. 0 < B <= 1.4 < Ď/2\n"
"        Argument -c nastavuje vĂ˝ĹĄku mÄĹicĂ­ho pĹĂ­stroje c pro vĂ˝poÄet. VĂ˝ĹĄka c je dĂĄna argumentem X (0 < X <= 100). Argument je volitelnĂ˝ -\n"
" implici tnĂ­ vĂ˝ĹĄka je 1.5 metrĹŻ.\n");
}
//-------------------------------------------------------------------//
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
//-------------------------------------------------------------------//
/*Funkcia ktora vrati hodnotu v absolutnej hodnote */
double Myabs(double x)
{
  if(x<0)
    return -x;
  else return x;
}
//-------------------------------------------------------------------//
/* Imlementacia tangensu pomocou taylorovho polynomu */
double taylor_tan(double x, unsigned int n)
{
    double a[13] = {1, 1, 2, 17, 62, 1382, 21844, 929569, 6404582, 443861162, 18888466084, 113927491862, 58870668456604};
    double b[13] = {1, 3, 15, 315, 2835, 155925, 6081075, 638512875, 10854718875, 1856156927625, 194896477400625, 49308808782358125, 3698160658676859375};
    unsigned int i;
    double s = 0;
    double t = x;
    for (i = 0; i < n; i++){ 
      s = s + (a[i]*t)/(b[i]);
      t = t*x*x;                                                          // vypocet aktualnej iteracie
    }   
    return s;
}
//-------------------------------------------------------------------//
/* Implementacia tangensu pomocou zretazenych zlomkov */
double cfrac_tan(double x, unsigned int n)
{
  double s = 0.0;
  double a;
  double b;
  int i=1 ;
  while(n>0)
{
    if(n==1){ a = x;
      b = n;
    }
    else {
      a=-x*x;
      b = n*2-1;
    }
    s = a / (b+s);                                                         // vypocet aktualnej iteracie
    i++;
    n--;
  }
  return s;
}
//-------------------------------------------------------------------//
/* funkcia pre zistenie spravnosti zadanych argumentov */
int zistiArgumenty(int argc, char *argv[])
{
  if ((strcmp(argv[1],"--tan")==0) && argc==5)                             // spravnost argumentov pre funkciu --tan
    {
      if((atoi(argv[3])>0) && (atoi(argv[3])<=atoi(argv[4])) && (atoi(argv[4]) < 14) && (atof(argv[2])>0) && ((atof(argv[2])<=1.4)))
	return 1;
    } 
  else if ((strcmp(argv[1],"-m")==0) && argc<=4)                           // spravnost argumentov pre funkciu -m
         {
	   double a,b=1.0;
	   a=atof(argv[2]);
	   (argv[3]!=NULL) ? b=atof(argv[3]) : a;
	   if(a>0 && a<=1.4 && b>0 && b<=1.4)
	     {
	   return 1;
	     }                                                             // spravnost aegumentov ak je zadana aj vyska c
	 }else if((strcmp(argv[1],"-c")==0) && argc<7 && argc>4 && (strcmp(argv[3],"-m")==0) ) 
                 {
		   double a,b,x=1.0;
		   x=atof(argv[2]);
		   a=atof(argv[4]);
		   (argv[5]!=NULL) ? b=atof(argv[5]) : a;
		   if(a>0 && a<=1.4 && b>0 && b<=1.4 && x>0 && x<=100)
		     {
		       return 1;
		     }
		 }
  return 0;
      }
//-------------------------------------------------------------------//
/* vypis vysledkov pre funkciu --tan*/
void vypis(char * argv[])
{
  double m,t,te,c,ce;
  for (int i=1; i<=atoi(argv[4]); i++)                                  //vypis vysledku pre zadany pocet iteracii
 	{
	  m=tan(atof(argv[2]));
	  t=taylor_tan(atof(argv[2]),i);
	  te=Myabs(m-t);
	  c=cfrac_tan(atof(argv[2]),i);
	  ce=Myabs(m-c);
	  if (i>=atof(argv[3]))
	    fprintf(stdout,"%d %e %e %e %e %e\n",i,m,t,te,c,ce);
	}
}
//-------------------------------------------------------------------//
void vzdialenost(char * argv[])
{ /*funkcia pre vypocet vzdielonosti od predmetu a vysky predmetu */
  //  const double presnost = 1e-10;
  double b,c=1.5,a=atof(argv[2]),vzdialenost;
  (argv[3]!=NULL) ? b=atof(argv[3]) : 0;                                 //nastavenie uhla beta pre parameter -m
  if(strcmp(argv[1],"-c")==0)                                            // nastavenie uhlov alfa a beta pre parameter -c
    {
  (argv[2]!=NULL) ? c=atof(argv[2]) : 1.5;
  a=atof(argv[4]);
  (argv[5]!=NULL) ? b=atof(argv[5]) : 0;
    } 
     
  vzdialenost=c/cfrac_tan(a,13);
  fprintf(stdout,"%.10e\n",vzdialenost);                                 //vypis vzdialenosti od predmetu
  fprintf(stdout,"%.10e\n",(c+(cfrac_tan(b,13)*vzdialenost)));           //vypis vysky predmetu
  
}
//-------------------------------------------------------------------//
/* Hlavna funkcia ktora vyvolava ostatne funkcie */
int main(int argc, char *argv[])
{
  if (argc==1)                                                          //kontrola ak nebol zadany ziaden argument
    {
      fprintf(stderr,"Nezadali ste argumenty\n");
      return 0;
    }
  if(strcmp(argv[1],"--help")==0)
    {
      if(zistiHelp(argc)==0)
	help();  
    }
  if(zistiArgumenty(argc,argv)==1)
    {
      if(strcmp(argv[1],"--tan")==0)
        {
	vypis(argv);
	}
      if(strcmp(argv[1],"-m")==0 || strcmp(argv[1],"-c")==0)
	{
	  vzdialenost(argv);
	}
    } else printf ("Zadali ste nespravne argumenty, pre napovedu pouzite argument --help\n");
return 0;
}
