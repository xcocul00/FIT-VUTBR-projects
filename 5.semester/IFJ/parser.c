
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>


#include "scaner.c"
#include "ial.c"
#include "parser.h"

#include "interpret.c"
#include "ins-table.c"



// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// POMOCNE FUNKCE -----
// void table_check(hash_table *table, char *name){

// 			tabItem *check = search_HashTable(table,name);
// 			// printf("----------TABLE----------\n");
// 			 // printf("next_param %s\nblock: %s\n name: %s\n isFunc: %d\ntypeData: %d\n defined: %d\n param: %d\n paramPos: %d\n intVal: %d\n doubleVal: %lf\n stringval: %s\n",
// 		    	check->next_param,check->blok,check->name,check->isfunc,check->typeData,check->defined,check->param,check->paramPos,check->intVal,check->doubleVal,check->stringVal );

// 		    // printf("--------------------\n");
// }
// void check_item(tabItem *check) {
// // printf("--------ITEM------------\n");
// 			 // printf("next_param %s\nblock: %s\n name: %s\n isFunc: %d\ntypeData: %d\n defined: %d\n param: %d\n paramPos: %d\n intVal: %d\n doubleVal: %lf\n stringval: %s\n",
// 		    	check->next_param,check->blok,check->name,check->isfunc,check->typeData,check->defined,check->param,check->paramPos,check->intVal,check->doubleVal,check->stringVal );

// 		    // printf("--------------------\n");
// }


// void //ptok(){
// 	if(DEBUG){
// 		// printf(">>>>>>>>>>>>>>>>>>>>>>>\n");
// 		// printf("ID %d - %s\n",tok->type, tok->attribute );
// 		// printf(">>>>>>>>>>>>>>>>>>>>>>>\n");
// 	}	
// }

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

//#include "pa.c"





int pa_extend(tabItem *item, tabItem *item2, tabItem *result){
	return 0;
}


void pa(){
	int count = 0;
	tok = get();

	// // printf("\n           -- PA --\n");
	while(tok->type != TYPE_SEMICOLON){
		tok = get();
		
		count++;
		if(count == 20) // pro jistotu
			break;
	}


}

void pa_comma(){
	int count = 0;
	

	// printf("\n           -- PA --\n");
	while(tok->type != TYPE_COMMA){
		tok = get();
		
		count++;
		if(count == 20) // pro jistotu
			break;
	}


}


void to_semicolon(){
	while(tok->type != TYPE_SEMICOLON){
		tok = get();
		
	}


}

void to_zavorka(){
	int zavorky = 1;
	tok = get();
	while(1){
		// //ptok();
		if(tok->type == TYPE_BLOCK_BEGIN){
			// // printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<XXXXXXXXXXXXXX\n");
			zavorky++;
		}
		if(tok->type == TYPE_BLOCK_END){
			// // printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
			zavorky--;
		}
		if(zavorky == 0){
			break;
		}

		tok = get();
		
	}


}

void to_RIGHT_BRACKET(){
	int zavorky = 1;
	tok = get();
	while(1){
		// //ptok();
		if(tok->type == TYPE_LEFT_BRACKET){
			// // printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<XXXXXXXXXXXXXX\n");
			zavorky++;
		}
		if(tok->type == TYPE_RIGHT_BRACKET){
			// // printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
			zavorky--;
		}
		if(zavorky == 0){
			break;
		}

		tok = get();
		
	}

}




int main(int argc, char *argv[]){
	// // printf("---------------------------------------\n");
	
	

	initList();


    table_class = init_HashTable(100);
    if (table_class->size != 100){
    	ERR_PRINT(ERR_INSIDE, "Init tabulky failed");
    	return 1;
    }
    //table_class->table=init_HashTable(10);

    if(sber){
    		read_file(argc,argv);
    	// vycisteni tokenu
    	tok = init_token();
    	tok = NULL;	

    	// Zacatek parsovani
		if (parser() != 0){
			free_HashTable(table_class);
			ERR_PRINT(ERR_OK,"Correctly ended\n\n");	
		}
		close_file();
		sber = false;
		

    }
    sber = false;
   //  if(DEBUG) // printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<< SBER II <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
  	// if(DEBUG) // printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
    
    if(!sber){
    	read_file(argc,argv);
    	// vycisteni tokenu
    	

    	actual_char = '\0';
    	next_char = '\0';
		tok = init_token();
    	tok = NULL;

    	// // printf("t: %d\n",tok2->type );
    	// Zacatek parsovani
		if (parser() == 0){
			// free_HashTable(table_class);
		}
		close_file();
    }

	// // printf("-----------------///-----END PARSER ----------------\n");

    tReturnStack RetStack;
    returnSInit(&RetStack);
    tRamecStack RamStack;
    ramecSInit(&RamStack);
    tRetAddrStack RetAddrStack;
    retAddrSInit(&RetAddrStack);   

    initJumpArray();

    interpret(RetStack, RamStack, RetAddrStack);
    disposeList();

    // printf("\n");
	
	// uvolneni TS - class
	free_HashTable(table_class);
	return 1; // neznama chyba


	
}


/*zpracovani seznamu trid*/
int parser(){

	// nacte se prvni token
	tok = get();
	//ptok();
	// kontrola validniho konce souboru
	if(tok->type == END_OF_FILE){


		if(!is_main){
			ERR_PRINT(ERR_SEM,"Chybi trida main");
		}
		if(!is_run){
			ERR_PRINT(ERR_SEM,"v tride main chybi run");
		}

		// vsechno ok- validni konec
		if (is_main && is_run){
			// if(DEBUG)// printf("Vsechno ok - main true, run true\n");
			return 0;
		}		

	}
	// zpracuj danou tridu
	class_parse();

	// pokracuj na dalsi
	parser();

return 0;
}	

void class_parse(){

	/* -- Nasleduje cteni CLASS -- */

	if(tok->type != TYPE_CLASS){
		// ERR_PRINT(ERR_SYN,"Prvnim znakem musi byt class");
		ERR_PRINT(ERR_SYN,"Nacteny dokument je prazdny");
	}

	//inicializace item
	tabItem *item;	
	item = create_tabItem();

	item->typeData = TYPE_CLASS;

	// class ID
	tok = get();
	if(tok->type != TYPE_NAME){
		ERR_PRINT(ERR_SYN,"za class nenasleduje id");
	}

	// kontrola zdali ID neni rezervovane ifj16
	if (!strcmp(tok->attribute,"ifj16")){
		ERR_PRINT(ERR_SEM,"ID v tride nesmi byt ifj16");
	}
	

	item->name = tok->attribute;
	

	// kontrola zdali class ID uz neexistuje
	if(sber) {
		tabItem *check = create_tabItem(); // pro jistotu
		check = search_HashTable(table_class,tok->attribute);
		// check2 = search_HashTable(check->table,tok->attribute); ----------------------------------------------------------------------
		if (check != NULL){
			ERR_PRINT(ERR_SEM,"trida uz je declarovana");
		}
		// vytvorime tridu v TS
		insert_into_HashTable(table_class,item);

	}
	
	

	// zachovani ukazatele na aktivni tridu 
	class_save = search_HashTable(table_class,tok->attribute);
	// if(DEBUG){
	// 	// printf("-- class_save\n");
	// 	check_item(class_save);
	// }
	if(!sber) generateInstruction(TYPE_CLASS,class_save, NULL, NULL);

	// tabItem *tmp;
	// tmp = create_tabItem();
	// tmp->name = "ahoj";
	// check_item(tmp);

	//inicializuji si tabulku v TS s nazvem tridy
	if(sber) class_save->table = init_HashTable(100);
		// insert_into_HashTable(class_save->table,tmp);
	// table_check(class_save->table,tmp->name);
	// check_item(class_save);





	// vytvorime instance tridy v instrukcnim zasobniku
	// TODO instance(nazev_CLASS, TYPE_CLASS) --------------------

	if (!strcmp(tok->attribute,"Main")){
		is_main = true;
		in_main = true;
		// if (DEBUG) // printf("Main_is - ok \nMain_in - ok \n");
	}

	tok = get();

	// class id {
	if (tok->type != TYPE_BLOCK_BEGIN ){
		ERR_PRINT(ERR_SEM,"chybi { ");
	}

	class_body();


}


void class_body(){

	tok = get();
	// static
	if (tok->type == TYPE_STATIC) {
	
		tok = get();

		// Typ
		switch (tok->type) {
			case TYPE_INT:		//integer				
			case TYPE_DOUBLE:	//double				
			case TYPE_STRING:	//String				
			case TYPE_VOID:		//void
				break;

			default:
				ERR_PRINT(ERR_SYN,"chybi, static X <- TYPE");
				break;
		}

		tmp.st_static = true;
		tmp.type = tok->type;

		tok = get();
		if (tok->type != TYPE_NAME) {
			ERR_PRINT(ERR_SYN,"chybi, static type X <- ID");
		}
		
		// nactu si ID do pomocne
		tmp.name = tok->attribute;

		tok = get();
		// narazil jsem na fce ---------------------FCE DEF
		if (tok->type == TYPE_LEFT_BRACKET){
			func_def();
			

		}
		else{
			// nebo globalni prom
			prom_def(false);
		}
		
		// rekurzivni opakovani
		class_body();

	} else if( tok->type == TYPE_NAME){
	
		tmp.name = tok->attribute;
		tok = get();
		// PA ----------------------------------------------------------
		class_body();
		
	}else if(tok->type == TYPE_BLOCK_END){
		// instance konce tridy v zasobniku

		if(in_main){
			in_main = false;
		}


	} else{
		//ptok();
		ERR_PRINT(ERR_SYN,"chyba v tele tridy");
	}


}



void prom_def(bool is_def){

	tabItem *item = create_tabItem();

	// tabItem *item2 = create_tabItem();

	// if(DEBUG) // printf("tmp: id -%s type-%d static-%d\n",tmp.name, tmp.type,tmp.st_static );

	// // printf("%s\n",func_save->name );
	// // printf("HEREE\n");

	
	if (!is_def){

		item->name = tmp.name;
		item->typeData = tmp.type;
		item->isfunc = false;

		if(!tmp.st_static){
			
			item->blok = func_save->name;
			tabItem *check;
			check = search_HashTable(func_save->table,item->name);
			if (check != NULL){
				ERR_PRINT(ERR_SEM,"prom uz je ve funkci declarovana");
			}
			
			// vytvorime novou prom	
			insert_into_HashTable(func_save->table,item);
			// if(DEBUG)table_check(func_save->table,item->name);
			

		}else{
			
			item->blok = class_save->name;
			if(sber){
				tabItem *check;
				check = search_HashTable(class_save->table,item->name);
				if (check != NULL){
					ERR_PRINT(ERR_SEM,"prom uz je declarovana");
				}

				// vytvorime novou prom	
				insert_into_HashTable(class_save->table,item);
				// if(DEBUG)table_check(class_save->table,item->name);	
			}
		}
	
		// if(DEBUG) {
		// 	// printf("\nlol\n");
		// 	// printf("%d\n",tmp.type );
		// }
		
		
		
		if(tok->type == TYPE_SEMICOLON){
			// int x;
			// tok = get();
			// //ptok();
			// // printf("<<<<<<<<<<<<<<<<<<<<<<<<<<xx\n");
			// generateInstruction(TYPE_ASSIG,item,NULL,NULL); -------------------------------------------INSTR
			return;
		}
		else if(sber){

			to_semicolon();
			// tok = get();
			// //ptok();
		}
		//////////////////////////////
		// ----UZ JE DEFINOVANA ----
		/////////////////////////////
	}else{
		
		// if(DEBUG) //ptok();
		// na vstupu je ID
    	tmp.name = tok->attribute;
        	
    	// jedna se o vestavenou funkci?
    	if (!strcmp(tok->attribute,"ifj16")) {
    		// nacteni tecky
    		tok = get();
       		if (tok->type != TYPE_DOT) {
				ERR_PRINT(ERR_SYN,"missing dot after class ifj16 // ifj16.id");
			}
			//nazev vestav funkce
			tok = get();
			item->name = tok->attribute;
		
			// if(DEBUG) //ptok();

			if (!strcmp(tok->attribute,"print")) {
				
				//item =pa(item);
				tok =get(); // (
				tok =get(); //	string in ------------------------volani PA

				
				// item ->stringVal = tok->attribute;
				// item ->typeData = tok->type;

				tabItem *argument = create_tabItem();
						
						// prvni argument
						if (tok->type == TYPE_NAME){
							tmp.name = tok->attribute;
							tok=get();
							if( tok->type == TYPE_DOT){

								// tabItem *check_class = create_tabItem();
						    	// check_class->blok = tmp.name;

						    	tok = get();
						    	if (tok->type != TYPE_NAME){
									ERR_PRINT(ERR_SYN,"chybi ID // class.id");
								}

								// check_class->name = tok->attribute;

															
								// Vstup = game.x
								// vyhleda nazev tridy podle bloku v item
								tabItem * search_class;
								search_class = search_HashTable(table_class,tmp.name);
								// nalezena trida je v search_class
								if (search_class == NULL){

									ERR_PRINT(ERR_SEM,"Trida nenalezena");
								}
								else{
									// check_item(search_class);
									// V nalezene tride game, vyhleda podle name z itemu
									tabItem *search_prom = search_HashTable(search_class->table,tok->attribute);
									// nalezena promenna je v search_prom a vypada nasledovne ------------------------------>
									if (search_prom == NULL){
										ERR_PRINT(ERR_SEM,"funkce ve tride nenalezena");
									} else if(search_prom->isfunc == 1){
										ERR_PRINT(ERR_SEM,"nejedna se o prom");
									}

									argument = search_HashTable(search_class->table,tok->attribute);
									tok = get();tok = get();				
								}
							// nejedna se o glob prom
							}else{

									// tabItem *search_prom;
									argument = search_HashTable(func_save->table,tmp.name);
									if (argument == NULL){
										ERR_PRINT(ERR_SEM,"funkce ve tride nenalezena");
									} else if(argument->isfunc == 1){
										ERR_PRINT(ERR_SEM,"jedna se o funkci");
									}
									// if(DEBUG) check_item(argument);
								

									// argument = search_HashTable(func_save->table, tmp.name);		



								// if(DEBUG) check_item(argument);
								if (argument->typeData != TYPE_STRING){
									// argument->typeData == TYPE_STRING;
										// char * str;
									// s// printf(str, "%d", argument->intVal);
									// argument->stringVal == inttostr()
									// ERR_PRINT(ERR_SEM,"Fce, length, v argumentu musi byt string");	
								}
								pa();   // PA -- 
							}

							
						}
						else{

							if (tok->type != TYPE_STRING_DATA){
								ERR_PRINT(ERR_SEM,"Fce, length, v argumentu musi byt string");	
							}
							argument->stringVal =tok->attribute;
							argument->typeData =tok->type;

							pa();  // PA -- 
						}		

					


						// // navratovaodnota fce
						// if (item->typeData != TYPE_INT){
						// 	ERR_PRINT(ERR_SEM,"Fce, prom do ktere se uklada sort musi byt string");	
						// }
						
							
								// // printf("asdasdasdasd*******///-----------------///PPPPPPP//\n"); //ptok();
						// if(DEBUG)check_item(item);
						// if(DEBUG)check_item(argument);
						generateInstruction(VOID_PRINT,argument,NULL, NULL); //------------------------------------------------ INSTRUKCE length do item pridat argument
						// tok=get();
						// if(DEBUG) //ptok();










				// // printf("xxxxxxxxxxxxxxxxx98888888888888888888888888888xxxxxx\n");
				// check_item(item);
			
				// generateInstruction(VOID_PRINT, item, NULL, NULL);
				


			}
			// else if (read_func(item) != 0){
			// 	ERR_PRINT(ERR_SYN,"Error ve vestavena_func");
			// } 			
			


    	} else{
    		// id =
    		// id.
    		// id()
    		tok = get();
    		
    		// if(DEBUG) //ptok();
    		if(tok->type == TYPE_DOT){

    			tabItem *check_class = create_tabItem();
    			check_class->blok = tmp.name;

    			tok = get();
    			if (tok->type != TYPE_NAME){
					ERR_PRINT(ERR_SYN,"chybi ID // class.id");
				}

				check_class->name = tok->attribute;

				// class.id =
				// class.id()
				// if(DEBUG) //ptok();
				tok = get();
				if (tok->type == TYPE_LEFT_BRACKET){
										
						// Vstup = game.x(
						// vyhleda nazev tridy podle bloku v item
						
						tabItem * search_class;
						search_class = search_HashTable(table_class,check_class->blok);
						// nalezena trida je v search_class
						if (search_class == NULL){

							ERR_PRINT(ERR_SEM,"Trida nenalezena");

						}
						else{

								// check_item(search_class);
								// V nalezene tride game, vyhleda podle name z itemu
								tabItem *search_prom = search_HashTable(search_class->table,check_class->name);
								// nalezena promenna je v search_prom a vypada nasledovne ------------------------------>
								if (search_prom == NULL){
									ERR_PRINT(ERR_SEM,"funkce ve tride nenalezena");
								} else if(search_prom->isfunc != 1){
									ERR_PRINT(ERR_SEM,"nejedna se o funkci");

								}

								
						}


						// zpracovani parametru funkce






				}else if(tok->type == TYPE_ASSIG){
						// vstup = game.x =
						check_class->isfunc = 0;
						
						// if(DEBUG) check_item(check_class);
						
						// Vstup = game.x
						// vyhleda nazev tridy podle bloku v item
						tabItem * search_class = search_HashTable(table_class,check_class->blok);
						// nalezena trida je v search_class
						if (search_class == NULL){

							ERR_PRINT(ERR_SEM,"Trida nenalezena");

						}
						else{

								// check_item(search_class);
								// V nalezene tride game, vyhleda podle name z itemu
								item = search_HashTable(search_class->table,check_class->name);
								// nalezena promenna je v search_prom a vypada nasledovne ------------------------------>
								if (item == NULL){
									ERR_PRINT(ERR_SEM,"Promena ve tride nenalezena");
								}


						}

						

						



					// PA -----------------------------------------------------
					

				} else{
					
					ERR_PRINT(ERR_SYN,"error class.id -< ");
					
				
				}

				



    		}else if(tok->type == TYPE_ASSIG){
    			// kontrola jestli ID uz je definovane
    			
    			// tabItem *check;
    			item = search_HashTable(func_save->table,tmp.name);
    			if (item == NULL){
    				ERR_PRINT(ERR_SEM,"prom neni definovane ve funkci");
    			}





    		}else if(tok->type == TYPE_LEFT_BRACKET){
    			// ID je funkce func()

    			// kontrola jestli funkce uz je definovana
    			
    			tabItem *check_funkce;
    			check_funkce = search_HashTable(class_save->table,tmp.name);
    			if (check_funkce == NULL){
    				ERR_PRINT(ERR_SEM,"funkce neni definovane ve tride");
    			}



    		}else{
    			// za id nenadleduje . ani =

    		}

    		
    		
    	}




	}
	
	if((tok->type == TYPE_ASSIG)&&(!sber)){
			tok = get();

			// if(DEBUG) {
			// 	// printf("**********************PROM_ASSIGN************************\n");
			// 	//ptok();
			// }
			// // printf("%d %s\n",tmp.type,tmp.name );
			if(tok->type == TYPE_NAME){

				// vstup = int x = id
								// class.id
								// func()
								// class.func()

				// ulozim si nazev prniho id
				tmp.name = tok->attribute;
    			tmp.type = tok->type;



    			//**************************************************************************
    				//	Zpracovani ifj16 funkci   				vstup: <type> <ID> = ifj16
    			//**************************************************************************
   			

    			if (!strcmp(tok->attribute,"ifj16")) {

		    		// nacteni tecky
		    		tok = get();
		       		if (tok->type != TYPE_DOT) {
						ERR_PRINT(ERR_SYN,"missing dot after class ifj16 // ifj16.id");
					}
					//nazev vestav funkce
					tok = get();
					// tabItem *item = create_tabItem();
					// item->name = tok->attribute;
					// // printf("SSSSSSSSSSSSSSSSSSSS\n" );
					// //ptok();
					tmp.name = tok->attribute;
					// if(DEBUG) check_item(item);

					tok = get();
					if (tok->type != TYPE_LEFT_BRACKET){
						ERR_PRINT(ERR_SYN,"U vestavene funkce chybi zavorka (");
					}
					tok = get();

		    		if (read_func(item,tmp.name) != 0){
						ERR_PRINT(ERR_SYN,"Error in vestavena_func");
					}	
					
					// if(DEBUG) // printf("\nkonec FCE ---\n");
					//ptok();
					tok = get();
					if(tok->type != TYPE_SEMICOLON){
						ERR_PRINT(ERR_SYN,"Chybi strednik");
					}
					
					


		    	} else{

			    	//*****************************

			    	tok = get();
		    		// if(DEBUG) //ptok();
		    		if(tok->type == TYPE_SEMICOLON){

		    			// int x = id;
		    			tabItem *search_item;
		    			search_item = search_HashTable(func_save->table,tmp.name);
		    			if (search_item == NULL){
							ERR_PRINT(ERR_SEM,"prom neni ve funkci definovana");
						}
						
						// if(DEBUG){ 
						// 	check_item(item);
						// 	check_item(search_item);
						// }

						// generateInstruction(TYPE_ASSIG, item, check_item, NULL) --------------------------------------- INSTRUKCE


		    		}


		    		else if(tok->type == TYPE_DOT){

		    			// tabItem *check_class = create_tabItem();
		    			// check_class->blok = tmp.name;

		    			tok = get();
		    			char *tmp_id = tok->attribute;
		    			if (tok->type != TYPE_NAME){
							ERR_PRINT(ERR_SYN,"chybi ID // class.id");
						}

						// check_class->name = tok->attribute;

						// int x = class.id()
						tok = get();
						if (tok->type == TYPE_LEFT_BRACKET){
												
								// Vstup = game.x(
								// vyhleda nazev tridy podle bloku v item
								tabItem * search_class;
								search_class = search_HashTable(table_class,tmp.name);
								// nalezena trida je v search_class
								if (search_class == NULL){

									ERR_PRINT(ERR_SEM,"Trida nenalezena");

								}
								

								// check_item(search_class);
								// V nalezene tride game, vyhleda podle name z itemu
								tabItem *search_func = search_HashTable(search_class->table,tmp_id);
								// nalezena promenna je v search_prom a vypada nasledovne ------------------------------>
								if (search_func == NULL){
									ERR_PRINT(ERR_SEM,"funkce ve tride nenalezena");
								} else if(search_func->isfunc != 1){
									ERR_PRINT(ERR_SEM,"nejedna se o funkci");
								}
								// if(DEBUG) {
								// 	check_item(item);
								// 	check_item(search_func);
								// }


								//***************************************
								// Zpracovani agrumentu funkce
								
								char* prvni_param = search_func->next_param;
								
								
								// if(DEBUG) // printf("PARAMETRY FUNKCE\n");

								tabItem *param_dest;
								tabItem *parametr = create_tabItem();

								generateInstruction(TYPE_JUMP,search_func,item,NULL);
								tok = get();
								

								for (int x=0; x < search_func->param;x++){


									if (tok->type == TYPE_NAME){
									
										parametr = get_argument();
										// if (parametr->typeData != TYPE_STRING){
										// 	ERR_PRINT(ERR_SEM,"Fce, sort, v argumentu musi byt string");	
										// }
									}
									else{

										switch(tok->type){

											case TYPE_STRING_DATA:
												parametr->stringVal = tok->attribute;
												break;
											case TYPE_INT_DATA:
												parametr->intVal = atoi(tok->attribute);
												break;
											case TYPE_DOUBLE_DATA:
												parametr->doubleVal = atof(tok->attribute);
												break;
											default: 
												ERR_PRINT(ERR_SEM,"Chyba v parametru pri nacitani funkce");	
												break;


										} // end switch
											tok = get();						

									}

										// cilove parametry
										param_dest = search_HashTable(search_func->table,prvni_param);
										prvni_param = param_dest->next_param;

										// if(DEBUG) {
										// 	check_item(param_dest);
										// 	check_item(parametr);
										// }

										generateInstruction(TYPE_ASSIG,param_dest,parametr,NULL); //-----------------------------------------INSTRUKCE PARAMETRY FUNKCE
										// tok=get();



									if ( tok-> type != TYPE_RIGHT_BRACKET){

										
										pa_comma();
										//ptok();
										if(tok->type != TYPE_COMMA){
											ERR_PRINT(ERR_SYN,"Musi nasledovat carka v parametru");	
										}

										tok=get();
										// Druhy argument
									}
									


								} // end for

									tok=get();
									//ptok();
									// tok=get();//ptok();

									// tok=get();//ptok();

								// generateInstruction(TYPE_ASSIG, item, search_prom, NULL); ---------------------------------------------INSTRUKCE FCE


						// int x = class.id;		
						}else if(tok->type == TYPE_SEMICOLON){

								// Vstup = game.x;
								// vyhleda nazev tridy podle bloku v item
								tabItem * search_class;
								search_class = search_HashTable(table_class,tmp.name);
								// nalezena trida je v search_class
								if (search_class == NULL){

									ERR_PRINT(ERR_SEM,"Trida nenalezena");

								}
								

								// check_item(search_class);
								// V nalezene tride game, vyhleda podle name z itemu
								tabItem *search_prom = search_HashTable(search_class->table,tmp_id);
								// nalezena promenna je v search_prom a vypada nasledovne ------------------------------>
								if (search_prom == NULL){
									ERR_PRINT(ERR_SEM,"prom ve tride nenalezena");
								}	

								// if(DEBUG) {
								// 	check_item(item);
								// 	check_item(search_prom);
								// }
								// generateInstruction(TYPE_ASSIG, item, search_prom, NULL); ---------------------------------------------INSTRUKCE PROM





						
						} else{
							
							ERR_PRINT(ERR_SYN,"error class.id -< ");
							
						
						}
						

		    		}else if(tok->type == TYPE_LEFT_BRACKET){
		    			// VSTUP: int x = func();
		    			// ID je funkce func()

		    			// kontrola jestli funkce uz je definovana
		    			tabItem *check_funkce;
		    			check_funkce = search_HashTable(class_save->table,tmp.name);
		    			if (check_funkce == NULL){
							ERR_PRINT(ERR_SEM,"funkce ve tride nenalezena");
						} else if(check_funkce->isfunc != 1){
							ERR_PRINT(ERR_SEM,"nejedna se o funkci");
						}
							// if(DEBUG) {
							// 	check_item(item);
							// 	check_item(check_funkce);
							// }

		    			generateInstruction(TYPE_JUMP,check_funkce,item,NULL); //------------------------------------------------- INSTRUKCE TODO FUNKCE
							tok = get(); // )
							tok = get(); // ;
							tok = get(); // -> next

		    		}else{
		    			ERR_PRINT(ERR_SEM,"sem error v definici prom");

		    		}
		    	} // end else if ifj16		


//VSTUP:  int x = 5;
			} else if((tok->type == TYPE_STRING_DATA)||(tok->type == TYPE_INT_DATA)||(tok->type == TYPE_DOUBLE_DATA) ){
				// if(DEBUG) // printf("TYPE_DATA int, string, double\n");
				
				tabItem *nactene = create_tabItem();
				tmp.name = tok->attribute;
				tmp.type = tok->type;
				// tmp.name = 
				tok = get();
				// if(DEBUG) //ptok();
				// if(DEBUG) check_item(item);
					if (tok->type == TYPE_SEMICOLON){
						
						switch(tmp.type){

							case TYPE_STRING_DATA:
								if(item->typeData != TYPE_STRING){
									ERR_PRINT(ERR_SEM,"prom neni typu string");
								}
								
								nactene->stringVal = tmp.name;
								nactene->typeData =TYPE_STRING_DATA;
								// generateInstruction(TYPE_ASSIG, item, nactene,NULL);
								break;

							case TYPE_INT_DATA:

								if(item->typeData != TYPE_INT){
									ERR_PRINT(ERR_SEM,"prom neni typu int");
								}
								nactene->intVal = atoi(tmp.name);
								nactene->typeData = TYPE_INT_DATA;
								// generateInstruction(TYPE_ASSIG, item, nactene,NULL);
								// 

								break;

							case TYPE_DOUBLE_DATA:
								if(item->typeData != TYPE_DOUBLE){
									ERR_PRINT(ERR_SEM,"prom neni typu double");
								}
								nactene->doubleVal = atof(tmp.name);
								nactene->typeData = TYPE_DOUBLE_DATA;
								// generateInstruction(TYPE_ASSIG, item, nactene,NULL);
								
								break;
								
							default : 
							
								break;



						}
						// if(DEBUG) {
						// 	// printf("//////////////////////////////////////////////////////////////\n");
						// 	check_item(item);
						// 	check_item(nactene);
						// }
						generateInstruction(TYPE_ASSIG,item,nactene,NULL); //-------------------------------------------------- INSTR
					}
					else{
							switch(tmp.type){

							case TYPE_STRING_DATA:
								if(item->typeData != TYPE_STRING){
									ERR_PRINT(ERR_SEM,"prom neni typu string");
								}
								
								nactene->stringVal = tmp.name;
								nactene->typeData =TYPE_STRING_DATA;
								// generateInstruction(TYPE_ASSIG, item, nactene,NULL);
								break;

							case TYPE_INT_DATA:

								if(item->typeData != TYPE_INT){
									ERR_PRINT(ERR_SEM,"prom neni typu int");
								}
								nactene->intVal = atoi(tmp.name);
								nactene->typeData = TYPE_INT_DATA;
								// generateInstruction(TYPE_ASSIG, item, nactene,NULL);
								// 

								break;

							case TYPE_DOUBLE_DATA:
								if(item->typeData != TYPE_DOUBLE){
									ERR_PRINT(ERR_SEM,"prom neni typu double");
								}
								nactene->doubleVal = atof(tmp.name);
								nactene->typeData = TYPE_DOUBLE_DATA;
								// generateInstruction(TYPE_ASSIG, item, nactene,NULL);
								
								break;
								
							default : 
							
								break;

						

						
						}
						pa();
						// if(DEBUG) {
						// 	// printf("//////////////////////////////////////////////////////////////\n");
						// 	check_item(item);
						// 	check_item(nactene);
						// }
						generateInstruction(TYPE_ASSIG,item,nactene,NULL); //-------------------------------------------------- INSTR




							

				
				}

				




			}else{
				// PA NENI! Podekovat ********* 
			}


	}else{

		// ERR_PRINT(ERR_SYN,"chyba pri declarovani");

	}



}
// vytvoreni nove funkce
void func_def(){

	tabItem *item;
	// if(DEBUG) // printf("function --\n");	

	// if(DEBUG) // printf("tmp NAME %s\n",tmp.name );
	if(sber){
		item = search_HashTable(class_save->table,tmp.name);
		if (item != NULL){
			// kontrola jestli je funkce definovana v dane tride

			ERR_PRINT(ERR_SEM,"Funkce uz je declarovana");
		}
	
		// kontrola funkce run v mainu
		if(in_main){
			// v mainu musi byt void RUN!
			if (!strcmp(tmp.name,"run")){
				if(tmp.type != TYPE_VOID){
					ERR_PRINT(ERR_SEM,"class main run musi byt void");
				}
				// poznacime,ze jsem narazily na run
				is_run = true;
			}
		}
	

		// vytvorime novou funkci v TS
		item = create_tabItem();
		item->name = tmp.name; //nazev fce
		item->typeData = tmp.type; // navratovy typ fce
		item->isfunc = true;	// definice ze jde o fci
		item->blok = class_save->name;	// definice bloku - tridy kde je fce definovna
		// item->param = tmp_param_pos;

		// item->param = tmp_param_pos;
		insert_into_HashTable(class_save->table,item); // vlozim do TS
		// if(sber) table_func = init_HashTable(10);	// inicializuji TS pro funkci- local
		// zachovame ukazatel na funkci - local
	}

	func_save = search_HashTable(class_save->table,tmp.name);
	// if(DEBUG){
	// 	// printf("func_save ---\n");
	// 	check_item(func_save);
	// }
	if(!sber) generateInstruction(I_RAMEC,func_save, NULL, NULL);

	tok = get();
// TS   -> class->func_def 
	if(sber) func_save->table = init_HashTable(100);

	// zpracovani seznamu parametru ve funkci
	if(tok->type != TYPE_RIGHT_BRACKET){
		if(sber){
			func_param();
		}else{
			to_RIGHT_BRACKET();
		}
	}
	func_save -> param = tmp_param_pos;
	func_save -> next_param = tmp_first_param;
	// // printf("--------xxxx-%s\n",tmp_first_param );
	insert_into_HashTable(class_save->table,func_save);
	// if(DEBUG){ 
	// 	// printf("Kontrola poctu param ve fce\n");
	// 	table_check(class_save->table,func_save->name);
	// }
	// 	// printf("PARAMETRY \n\n");
	// 	// printf("%s\n",func_save->name );
	// tabItem *parametry = search_HashTable(class_save->table,"run");
	// check_item(parametry);
	// tabItem *parametry_arg = search_HashTable(parametry->table,"a");
	// if (parametry_arg == NULL){
	// 	// printf("nenalezen\n");
	// }
	// check_item(parametry_arg);

	// generateInstruction(I_RAMEC,)

	
	// tabItem *test01;
	// test01 = search_HashTable(func_save->table,"b");
	// // printf("hledam A\n");
	// check_item(test01);

	// nasleduje telo funkce {
	tok = get();
	if( tok->type != TYPE_BLOCK_BEGIN){
		ERR_PRINT(ERR_SYN,"chybi { u tela funkce");
	}

	if(!sber){
		//zpracivani tela funkce
		func_body();
	}else{
		to_zavorka();
	}

}


/*
** @function get_variable()
** @description funkce zjistuje Typ a ID proměnné
*/

int get_variable() {
	
	switch (tok->type) {		// Check type of variable
		case TYPE_INT: 			//Integer
			break;
		case TYPE_DOUBLE: 		//Double
			break;
		case TYPE_STRING: 		//string
		 	break;
				
		default:
			return 1;
			break;
	}

	return 0;
}



void func_param(){

	
	tabItem *item = create_tabItem();
	item->blok = tmp.name;
	// tabItem *check;		


	// nacteni typu			
	if (get_variable() != 0){
		ERR_PRINT(ERR_SYN,"Wrong of variable type");
	}

	item->typeData = tok->type;
	tok = get(); //get ID of variable
		
	if (tok->type != TYPE_NAME) {
		ERR_PRINT(ERR_SYN,"Missing ID");
	}
	tmp_param_pos ++;
	tmp_first_param = tok->attribute;
	item->name = tok->attribute;
	item->defined = 1;
	item->param = 1;
	item->paramPos = tmp_param_pos;			
	previouse_param = tok->attribute;
	// // printf("x******%s\n",previouse_param );
	// vlozeni prvniho parametru do func_safe
	insert_into_HashTable(func_save->table,item);
	// check = search_HashTable(func_save->table,item->name);
	// if(DEBUG){
	// 	// printf(" --- PRVNI FUNC PARAM ---\n");
	// 	check_item(check);
	// }
	tok = get();
			
	switch (tok->type) {
			
		case TYPE_COMMA:
				tok = get();
												
				if (tok->type == TYPE_RIGHT_BRACKET) 
					ERR_PRINT(ERR_SYN,"In fucn param after comma can not be right bracket");
				func_param_next();	
				break;
					
		case TYPE_RIGHT_BRACKET: break;
					
		default:
			ERR_PRINT(ERR_SYN,"Wrong params in function");
	}

}

void next_next_param(){
		// if(DEBUG) // printf("%s\n",previouse_param );
			tabItem *tmp_param;	
			tmp_param = search_HashTable(func_save->table,previouse_param);
				// // printf("PREDESLI PARAMETR ---xxx\n");
			if (tmp_param == NULL){
				// // printf("nenalezeno\n");
			}
			tmp_param->next_param = tok->attribute;
				// check_item(tmp_param);
			insert_into_HashTable(func_save->table,tmp_param);	
			
			// table_check(func_save->table,previouse_param);
				previouse_param = tok->attribute;


}


void func_param_next(){

	tabItem *item = create_tabItem();
	item->blok = tmp.name;
	tabItem *check;
		

	if (tok->type != TYPE_RIGHT_BRACKET) {	// neni )
		
			// nacteni typu			
			if (get_variable() != 0){
				ERR_PRINT(ERR_SYN,"Wrong of variable type");
			}

			item->typeData = tok->type;
			tok = get(); //get ID of variable
		
			if (tok->type != TYPE_NAME) {
				ERR_PRINT(ERR_SYN,"Missing ID");
			}
			tmp_param_pos ++;
			item->name = tok->attribute;
			// tmp.name = tok->attribute;
			item->defined = 1;
			item->param = 1;
			item->paramPos = tmp_param_pos;			
			// check_item(item);
			check = search_HashTable(func_save->table,item->name);
			if (check != NULL){
				ERR_PRINT(ERR_SEM,"Prom ve funkci uz je deklarovana");
			}

			insert_into_HashTable(func_save->table,item);

			check = search_HashTable(func_save->table,item->name);
			// if(DEBUG){
			// 	// printf(" ---NEXT  FUNC PARAM ---\n");
			// 	table_check(func_save->table,item->name);
			// }
			
			next_next_param();

			tok = get();
			
			switch (tok->type) {
				
					case TYPE_COMMA:
						tok = get();
												
						if (tok->type == TYPE_RIGHT_BRACKET) 
							ERR_PRINT(ERR_SYN,"In fucn param after comma can not be right bracket");
						// printf("DALSI PARAM ---\n");
						break;
					
					case TYPE_RIGHT_BRACKET: break;
					
					default:
						ERR_PRINT(ERR_SYN,"Wrong params in function");
			}
				
		func_param_next();
	}

}



/*
Zpracovani tela funkce - vse je lokalni!
*/
void func_body(){

	tok = get();
	// // printf("PODMINKA -----------------%d\n",fce_podminka );
	// if(fce_podminka != 0){
	// 	ERR_PRINT(ERR_SEM,"Semantic ve funkci");
	// }
	// int x
	if (get_variable() == 0){

	
		// lokalni prom
		// ulozime typ
		tmp.type = tok->type;	
	
		tok = get(); //get ID of variable
		
		if (tok->type != TYPE_NAME) {
			ERR_PRINT(ERR_SYN,"Missing ID");
		}
		// id promenne
		tmp.name = tok->attribute;
		tmp.st_static = false;

		tok = get();
		// definujeme lokalni promennou,

		// //ptok();
		prom_def(false);	

		// // printf("%s\n", );		
	
	} // inicializace promenne nebo volani funkci
	else if (tok->type == TYPE_NAME)
    {

    	prom_def(true);

    	
    } else if(key_symbols() == 0){
//------
    	
    	parse_key_symbols();
		// tok = get();
		// // printf("id %d %s\n",tok->type, tok->attribute );
		// 	tok = get();
		// // printf("id %d %s\n",tok->type, tok->attribute );
		// 	tok = get();
		// if(DEBUG) // printf("id %d %s\n",tok->type, tok->attribute );


		
    } else if(tok->type == TYPE_BLOCK_END){
		
    	if(fce_podminka == 0){
    		if( func_save->typeData != TYPE_VOID && !returns){
    			ERR_PRINT(ERR_SEM,"hodnota returnu");
    		}
    		generateInstruction(TYPE_RETURN, func_save, NULL, NULL); 
    		returns = false;

    	}
    	// konec jeden funkce / if / while
    	else{
    	
    		fce_podminka --;
    	}
    	// vynorim se z fce_podminka
    	return;


    }
    else{
    	// if(DEBUG) // printf("id %d %s\n",tok->type, tok->attribute );
    	ERR_PRINT(ERR_SEM,"sem error");
    }

	// znovu zanoreni se do tela funkce		
    func_body();

}

// key_symbols
int key_symbols(){

	switch(tok->type){
		// typ tridy
		case TYPE_CLASS:
		// if 	
		case TYPE_IF:			
		case TYPE_ELSE:
		// true or false	 
		case TYPE_FALSE:			 
		case TYPE_TRUE:
		// while casti funkce			 
		case TYPE_WHILE:			
		case TYPE_DO:			
		case TYPE_CONTINUE:			  
		case TYPE_BREAK:

		case TYPE_RETURN:
			return 0;
			break; 
 	
		default: 
			return 1;
			break;
	}
	return 1;

}


void parse_key_symbols(){

	if( tok->type == TYPE_RETURN){

		// if(DEBUG) // printf("RETURN\n");
		tok = get();
		//ptok();
		// narazil jsem na return a teid kontroluji, zdali nasleduje strednik

		if((func_save->typeData == 0)&& (tok->type == TYPE_SEMICOLON)){
			ERR_PRINT(ERR_SEM,"Error, void funkce koncici return");	
		}

		else if((tok->type == TYPE_SEMICOLON && func_save->typeData != 0)){
			ERR_PRINT(ERR_SEM,"Funkce neni void, ale konci returnem bez parametru");	
		}

		returns = true;
		
		// if(DEBUG) //ptok();
	
		tabItem * vyraz = create_tabItem();
		vyraz->name= tok->attribute;
		vyraz->typeData = tok->type;
		// PA --
		// tabItem *param_return;
		// param_return = readExpression();
		//
		tok = get(); 
		// // printf("**********************//////////////////////////////////\n");
		// check_item(vyraz);	check_item(func_save);
		// // printf("%d\n",tok->type );
		generateInstruction(TYPE_RETURN, vyraz ,NULL, func_save);
	}

	else if( tok->type == TYPE_IF){
		// if(DEBUG) // printf("VSTUP DO IF\n");
		fce_podminka ++;
		// if(DEBUG) // printf("-------------------------------podminka %d\n",fce_podminka );
		generateInstruction(PODMINKA_begin,NULL,NULL,NULL);
		tok = get();
		if(tok->type != TYPE_LEFT_BRACKET){
			ERR_PRINT(ERR_SYN,"Chybi zavorka u IF -> (");		
		}
		generateInstruction(TYPE_IF,NULL,NULL,NULL);


		// tabItem * vyraz = create_tabItem();
		// vyraz->name= tok->attribute
		// vyraz->typeData = tok->type;
		// PA --
		// tabItem *param_if;
		// param_if = readExpression();
		tok = get();//smazat )
		tok = get();
		// if(DEBUG) //ptok();
		if( tok->type != TYPE_BLOCK_BEGIN){
			ERR_PRINT(ERR_SYN,"Chybi zavorka u IF -> {");
		}
		func_body();
		// if(DEBUG) // printf("-----------------------KONEC IF\n");
		
		generateInstruction(PODMINKA_end,NULL,NULL,NULL);


	}
	// else
	else if( tok->type == TYPE_ELSE){
		tok = get();
		// if(DEBUG) //ptok();
		if( tok->type != TYPE_BLOCK_BEGIN){
			ERR_PRINT(ERR_SYN,"Chybi zavorka u IF -> {");
		}
		generateInstruction(TYPE_ELSE,NULL,NULL,NULL);

		func_body();
		
		generateInstruction(PODMINKA_end,NULL,NULL,NULL);


	}




	else if( tok->type == TYPE_CONTINUE){

		generateInstruction(TYPE_CONTINUE, NULL, NULL, NULL);
		tok = get();

	}
	else if( tok->type == TYPE_BREAK){

		generateInstruction(TYPE_BREAK, NULL, NULL, NULL);
		tok = get();

	}else{
		ERR_PRINT(ERR_SEM,"Chyba - key_symbols");	
	}


}

tabItem *get_argument(){

	// if(DEBUG) // printf("*****************************GET ARGUMENT ****************************\n");


	tabItem *search_prom;


   	// vstup = id
    // if(DEBUG) //ptok();
    tmp.name = tok->attribute;
    tok = get();
    if(tok->type == TYPE_DOT){

    	// tabItem *check_class = create_tabItem();
    	// check_class->blok = tmp.name;

    	tok = get();
    	if (tok->type != TYPE_NAME){
			ERR_PRINT(ERR_SYN,"chybi ID // class.id");
		}

		// check_class->name = tok->attribute;

									
		// Vstup = game.x
		// vyhleda nazev tridy podle bloku v item
		tabItem * search_class;
		search_class = search_HashTable(table_class,tmp.name);
		// nalezena trida je v search_class
		if (search_class == NULL){

			ERR_PRINT(ERR_SEM,"Trida nenalezena");
		}
		else{
			// check_item(search_class);
			// V nalezene tride game, vyhleda podle name z itemu
			tabItem *search_prom = search_HashTable(search_class->table,tok->attribute);
			// nalezena promenna je v search_prom a vypada nasledovne ------------------------------>
			if (search_prom == NULL){
				ERR_PRINT(ERR_SEM,"funkce ve tride nenalezena");
			} else if(search_prom->isfunc == 1){
				ERR_PRINT(ERR_SEM,"nejedna se o prom");
			}

			return 	search_HashTable(search_class->table,tok->attribute);				
		}
	}else{
		
		search_prom = search_HashTable(func_save->table,tmp.name);
		if (search_prom == NULL){
			ERR_PRINT(ERR_SEM,"funkce ve tride nenalezena");
		} else if(search_prom->isfunc == 1){
			ERR_PRINT(ERR_SEM,"jedna se o funkci");
		}
			// check_item(search_prom);
	

		return search_HashTable(func_save->table, tmp.name);
	}

	return NULL;
	
}



int read_func(tabItem *item, char *func_name) {

	// if(DEBUG) // printf("finding function...\n");
		// printf("%s\n",func_name );

	//*************************************************
	//************************ length
	if (!strcmp(func_name,"length")) {
		// if(DEBUG) // printf("delka ..\n");
		

		tabItem *argument = create_tabItem();
		
		// prvni argument
		if (tok->type == TYPE_NAME){
			argument = get_argument();
			// check_item(argument);
			if (argument->typeData != TYPE_STRING){
				ERR_PRINT(ERR_SEM,"Fce, length, v argumentu musi byt string");	
			}

		}
		else{

			if (tok->type != TYPE_STRING_DATA){
				ERR_PRINT(ERR_SEM,"Fce, length, v argumentu musi byt string");	
			}
			argument->stringVal =tok->attribute;
			tok = get();
		}		

	


		// navratova hodnota fce
		if (item->typeData != TYPE_INT){
			ERR_PRINT(ERR_SEM,"Fce, prom do ktere se uklada sort musi byt string");	
		}
		
		
		// if(DEBUG)check_item(item);
		// if(DEBUG)check_item(argument);
		generateInstruction(INT_LENGTH, item, argument, NULL); //------------------------------------------------ INSTRUKCE length do item pridat argument
	} 
	//*************************************************
	//************************ SORT
	if (!strcmp(func_name,"sort")) {
		// if(DEBUG) // printf("delka ..\n");
		

		// prvni argument
		tabItem *argument = create_tabItem();		
		if (tok->type == TYPE_NAME){
			argument = get_argument();
			if (argument->typeData != TYPE_STRING){
				ERR_PRINT(ERR_SEM,"Fce, sort, v argumentu musi byt string");	
			}
		}
		else{

			if (tok->type != TYPE_STRING_DATA){
				ERR_PRINT(ERR_SEM,"Fce, sort, v argumentu musi byt string");	
			}
			argument->stringVal =tok->attribute;
			tok = get();

		}

	
		// navratova hodnota fce
		if (item->typeData != TYPE_STRING){
			ERR_PRINT(ERR_SEM,"Fce, prom do ktere se uklada sort musi byt string");	
		}
		
		
		// if(DEBUG)check_item(item);
		// if(DEBUG)check_item(argument);
		generateInstruction(STRING_SORT, item, argument, NULL); //------------------------------------------------ INSTRUKCE sort do item pridat argument
	} 
	//*************************************************
	//************************ COMPARE
	if (!strcmp(func_name,"compare")) {
		// if(DEBUG) // printf("compare ..\n");
		
		// prvni argument
		tabItem *argument = create_tabItem();		
		if (tok->type == TYPE_NAME){
			argument = get_argument();
			if (argument->typeData != TYPE_STRING){
				ERR_PRINT(ERR_SEM,"Fce, sort, v argumentu musi byt string");	
			}
		}
		else{

			if (tok->type != TYPE_STRING_DATA){
				ERR_PRINT(ERR_SEM,"Fce, sort, v argumentu musi byt string");	
			}
			argument->stringVal =tok->attribute;

		}


		// //ptok();
		tok = get();

		// if(DEBUG) //ptok();
		if(tok->type != TYPE_COMMA){
			ERR_PRINT(ERR_SYN,"Musi nasledovat carka v parametru");	
		}

		tok=get();
		// Druhy argument
		tabItem *argument2 = create_tabItem();		
		if (tok->type == TYPE_NAME){
			argument2 = get_argument();
			if (argument2->typeData != TYPE_STRING){
				ERR_PRINT(ERR_SEM,"Fce, sort, v prvnim argumentu musi byt string");	
			}
		}
		else{

			if (tok->type != TYPE_STRING_DATA){
				ERR_PRINT(ERR_SEM,"Fce, sort, v druhem argumentu musi byt string");	
			}
			argument2->stringVal =tok->attribute;
			tok = get();

		}

		// navratova hodnota fce
		if (item->typeData != TYPE_INT){
			ERR_PRINT(ERR_SEM,"Fce, prom do ktere se uklada compare musi byt int");	
		}
		
		// if(DEBUG)check_item(item);
		// if(DEBUG)check_item(argument);
		// if(DEBUG)check_item(argument2);
		generateInstruction(INT_COMPARE, item, argument, argument2); //------------------------------------------------ INSTRUKCE compare do item pridat argument
	} 

	//*************************************************
	//************************ find
	if (!strcmp(func_name,"find")) {
		// if(DEBUG) // printf("find ..\n");
		
		// prvni argument
		tabItem *argument = create_tabItem();		
		if (tok->type == TYPE_NAME){
			argument = get_argument();
			if (argument->typeData != TYPE_STRING){
				ERR_PRINT(ERR_SEM,"Fce, find, v argumentu musi byt string");	
			}
		}
		else{

			if (tok->type != TYPE_STRING_DATA){
				ERR_PRINT(ERR_SEM,"Fce, find, v argumentu musi byt string");	
			}
			argument->stringVal =tok->attribute;

		}


		// //ptok();
		tok = get();

		// if(DEBUG) //ptok();
		if(tok->type != TYPE_COMMA){
			ERR_PRINT(ERR_SYN,"Musi nasledovat carka v parametru");	
		}

		tok=get();
		// Druhy argument
		tabItem *argument2 = create_tabItem();		
		if (tok->type == TYPE_NAME){
			argument2 = get_argument();
			if (argument2->typeData != TYPE_STRING){
				ERR_PRINT(ERR_SEM,"Fce, find, v prvnim argumentu musi byt string");	
			}
		}
		else{

			if (tok->type != TYPE_STRING_DATA){
				ERR_PRINT(ERR_SEM,"Fce, find, v druhem argumentu musi byt string");	
			}
			argument2->stringVal =tok->attribute;
			tok = get();

		}

		// navratova hodnota fce
		if (item->typeData != TYPE_INT){
			ERR_PRINT(ERR_SEM,"Fce, prom do ktere se uklada find musi byt int");	
		}
		// if(DEBUG) // printf("FIND ----------\n");
		// if(DEBUG)check_item(item);
		// if(DEBUG)check_item(argument);
		// if(DEBUG)check_item(argument2);
		generateInstruction(INT_FIND, item, argument, argument2); //------------------------------------------------ INSTRUKCE compare do item pridat argument
	} 
	
	//*************************************************
	//************************ substr
	if (!strcmp(func_name,"substr")) {
		// if(DEBUG) // printf("substr ..\n");
		
		// prvni argument
		tabItem *argument = create_tabItem();		
		if (tok->type == TYPE_NAME){
			argument = get_argument();
			if (argument->typeData != TYPE_STRING){
				ERR_PRINT(ERR_SEM,"Fce, substr, v prvnim argumentu musi byt string");	
			}
		}
		else{

			if (tok->type != TYPE_STRING_DATA){
				ERR_PRINT(ERR_SEM,"Fce, substr, v prvni argumentu musi byt string");	
			}
			argument->stringVal =tok->attribute;

		}


		// //ptok();
		tok = get();

		
		if(tok->type != TYPE_COMMA){
			ERR_PRINT(ERR_SYN,"Musi nasledovat carka v parametru");	
		}

		tok=get();
		// Druhy argument
		tabItem *argument2 = create_tabItem();		
		if (tok->type == TYPE_NAME){
			argument2 = get_argument();
			if (argument2->typeData != TYPE_INT){
				ERR_PRINT(ERR_SEM,"Fce, substr, v druhem argumentu musi byt string");	
			}
		}
		else{

		// kontrola prom
		if (tok->type != TYPE_INT_DATA){
				ERR_PRINT(ERR_SEM,"Fce, substr, prom musi byt string");	
			}
			argument2->intVal =atoi(tok->attribute);
			tok = get();

		}


	
		if(tok->type != TYPE_COMMA){
			ERR_PRINT(ERR_SYN,"2Musi nasledovat carka v parametru");	
		}

		tok=get();
		// treti argument
		tabItem *argument3 = create_tabItem();		
		if (tok->type == TYPE_NAME){
			argument3 = get_argument();
			if (argument3->typeData != TYPE_INT){
				ERR_PRINT(ERR_SEM,"Fce, substr, v tretim argumentu musi byt int");	
			}
		}
		else{

		// kontrola prom
		if (tok->type != TYPE_INT_DATA){
				ERR_PRINT(ERR_SEM,"Fce, substr, prom musi byt int");	
			}
			argument3->intVal = atoi(tok->attribute);
			tok = get();

		}



		// navratova hodnota fce
		if (item->typeData != TYPE_STRING){
			ERR_PRINT(ERR_SEM,"Fce, prom do ktere se uklada substr musi byt string");	
		}
		

		generateInstruction(STRING_SUBSTR, argument, argument2, argument3); //------------------------------------------------ INSTRUKCE compare do item pridat argument
		generateInstruction(TYPE_ASSIG, item, NULL, NULL);// ------------------------------------------------ INSTRUKCE compare do item pridat argument
	

	} 

	//****************************************************************************************************************************
	//*************************************************************************Vestavˇené funkce pro naˇcítání literál°u a výpis term°u:
	
	//*************************************************
	//************************ readint
	if (!strcmp(func_name,"readInt")) {
		
		
		// navratova hodnota fce
		if(item->typeData != TYPE_DOUBLE){
			if (item->typeData != TYPE_INT){
				ERR_PRINT(ERR_SEM,"Fce, prom do ktere se uklada readInt musi byt int");	
			}
		
		}
		
		
		// if(DEBUG)check_item(item);
		// if(DEBUG)check_item(argument);
		generateInstruction(INT_READ_INT, item, NULL, NULL); //------------------------------------------------ INSTRUKCE length do item pridat argument
	} 
		//*************************************************
	//************************ readDouble
	if (!strcmp(func_name,"readDouble")) {
		
		
		// navratova hodnota fce
		if (item->typeData != TYPE_DOUBLE){
			ERR_PRINT(ERR_SEM,"Fce, prom do ktere se uklada readDouble musi byt double");	
		}
		
		
		// if(DEBUG)check_item(item);
		// if(DEBUG)check_item(argument);
		generateInstruction(DOUBLE_READ_DOUBLE, item, NULL, NULL); //------------------------------------------------ INSTRUKCE length do item pridat argument
	} 

	//*************************************************
	//************************ readString
	if (!strcmp(func_name,"readString")) {
		
		
		// navratova hodnota fce
		if (item->typeData != TYPE_STRING){
			ERR_PRINT(ERR_SEM,"Fce, prom do ktere se uklada readString musi byt string");	
		}
		
		
		// if(DEBUG)check_item(item);
		// if(DEBUG)check_item(argument);
		generateInstruction(STRING_READ_STRING, item, NULL, NULL); //------------------------------------------------ INSTRUKCE length do item pridat argument
	} 






	return 0;
}
