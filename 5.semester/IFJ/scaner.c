#include "scaner.h"

#define BUFF_SIZE 101
char actual_char = '\0';   // premenna na zapamatanie si aky znak mam a kam sa posuniem
char next_char = '\0';
static FILE* file;
// token *tok;
/*
 * citanie zo subora ak je zadane (parametrom)
 */
void read_file(int argc, char **argv){
	if(argc == 2){
		file = fopen(argv[1],"r");
		if(file == NULL){
			ERR_PRINT(ERR_INSIDE,"Soubor se nepodarilo otevrit");
		}
		// TODO ak sa nepodari subor otvorit
	}}

/*
 * nasledne zatvorenie suboru
 */
void close_file(){
	if (file != NULL){
		fclose(file);
	}}

/*
 *	funkcia vrati jeden znak zo suboru alebo stdin (podla parametru)
 */
char get_char(){
	if(file==NULL)
		return getc(stdin);
	else
		return getc(file);
}

/*
 * vytvorenie tokenu
 */
token * init_token(){
	token *token = malloc (sizeof (token));
	token->attribute = NULL;
	token->type = TYPE_UNDEFINED;
	return token;
}

/*
 * funkcia ktora vrati token, hlavna funkcia skenera
 */
token * get(){
	token *token;
	token = init_token();
	int counter = 1;
	int lenght = 1;
	// zaciatok automatu
	if (actual_char == '\0'){
		actual_char = get_char();
		skip_dust();
	}
	// ak je biely znak odstrani ho aj s komentami
	if(is_white(actual_char)){
		skip_dust();
	}
	// End of file
	if(actual_char == EOF){
		token->type=END_OF_FILE;
		return token;
	}
	// nacitame dalsi znak na porovnanie ci ide o operator,key atd.
	if(next_char == '\0'){
		next_char=get_char();
	}
	// operator zo skupin <=,>=,==,!=
	if(is_double_operand(actual_char,next_char)){
		token->type=two_char_type(actual_char,next_char);
		next_char='\0';
	}
	// ostatne operatory
	else if (is_single_operand(actual_char)){
		token->type=char_type(actual_char);
	}
	// identifikator moze zacinat _ , $ alebo pismenom 
	else if (is_word(actual_char) || actual_char == '_' || actual_char == '$'){
		char buffer[BUFF_SIZE];
		buffer[0]=actual_char;
		// identifikator moze obsahovat _ , $ , pismeno alebo cislo
		while ((next_char != EOF) && (is_word(next_char) || next_char == '_' || next_char == '$' || isnumber(next_char)) ){
			buffer[lenght]=next_char;
			counter++;
			lenght=counter % BUFF_SIZE;
			next_char=get_char();
		}
		buffer[lenght]='\0';
		token->attribute=(char *)malloc(100);
		strncpy(token->attribute,buffer,lenght);
		// zistime ci neide o klucove slovo
		for (int i=0; i<KEYS_COUNT; i++){
			if(!(strcmp(key[i],buffer))){
				token->type=i+1;
				token->attribute=NULL;
			}
		}
		// ak neide o klucove slovo ide o novy identifikator
		if(token->type == TYPE_UNDEFINED)
		{
			token->type=TYPE_NAME;
		}
	
	}
	// ak je znak cislom
	else if (isnumber(actual_char)){
		char buffer[BUFF_SIZE];
		bool flag=false;
		buffer[0]=actual_char;
		// nacitame pokial je cislo
		while ((next_char != EOF) && (isnumber(next_char)) && (counter<= BUFF_SIZE)){
		buffer[lenght]=next_char;
		counter++;
		lenght=counter % BUFF_SIZE;
		next_char=get_char();
		}
		// ak ide o double cislo s desatinnou castou
		//PUSH
		if(next_char == '.'){
			buffer[lenght]=next_char;
			counter++;
			lenght=counter % BUFF_SIZE;
			next_char=get_char();
			if(!isnumber(next_char)){
				ERR_PRINT(ERR_LEX,"Lexikalna chyba za . nasleduje nepovoleny znak");
			}
			while ((next_char != EOF) && (isnumber(next_char)) && (counter<= BUFF_SIZE)){
			buffer[lenght]=next_char;
			counter++;
			lenght=counter % BUFF_SIZE;
			next_char=get_char();
			}
			if (next_char == 'e' || next_char == 'E'){
			buffer[lenght]=next_char;
			counter++;
			lenght=counter % BUFF_SIZE;
			next_char=get_char();
			if(next_char == '+' || next_char == '-'){
				buffer[lenght]=next_char;
				counter++;
				lenght=counter % BUFF_SIZE;
				flag=true;
			}
			else if(isnumber(next_char)){
				buffer[lenght]=next_char;
				counter++;
				lenght=counter % BUFF_SIZE;
			}
			else if(!(isnumber(next_char))){
				ERR_PRINT(ERR_LEX,"Lexikalna chyba za E nenasleduje cislo");
			}
			next_char = get_char();
			if(flag){
				if(!(isnumber(next_char))){
				ERR_PRINT(ERR_LEX,"Lexikalna chyba za +/- nenasleduje cislo");
				}
			}
			while ((next_char != EOF) && (isnumber(next_char)) && (counter<= BUFF_SIZE)){
					buffer[lenght]=next_char;
					counter++;
					lenght=counter % BUFF_SIZE;
					next_char=get_char();
			}}
			if (!is_white(next_char) && !(next_char==';' || next_char=='+' || next_char=='-' || next_char=='*' || next_char=='/' || next_char==')'))
			{
				ERR_PRINT(ERR_LEX,"Lexikalna chyba za cislom nasleduje nepovoleny znak");
			}
			token->type=TYPE_DOUBLE_DATA;
		}
		// ak ide o double s exponencialnou castou
		else if (next_char == 'e' || next_char == 'E'){
			buffer[lenght]=next_char;
			counter++;
			lenght=counter % BUFF_SIZE;
			next_char=get_char();
			if(next_char == '+' || next_char == '-'){
				buffer[lenght]=next_char;
				counter++;
				lenght=counter % BUFF_SIZE;
				flag=true;
			}
			else if(isnumber(next_char)){
				buffer[lenght]=next_char;
				counter++;
				lenght=counter % BUFF_SIZE;
			}
			else if(!(isnumber(next_char))){
				ERR_PRINT(ERR_LEX,"Lexikalna chyba za E nenasleduje cislo");
			}
			next_char = get_char();
			if(flag){
				if(!(isnumber(next_char))){
				ERR_PRINT(ERR_LEX,"Lexikalna chyba za +/- nenasleduje cislo");
				}
			}
			while ((next_char != EOF) && (isnumber(next_char)) && (counter<= BUFF_SIZE)){
					buffer[lenght]=next_char;
					counter++;
					lenght=counter % BUFF_SIZE;
					next_char=get_char();
			}
			token->type=TYPE_DOUBLE_DATA;
			if (!is_white(next_char) && !(next_char==';' || next_char=='+' || next_char=='-' || next_char=='*' || next_char=='/'|| next_char==')'))
			{
				ERR_PRINT(ERR_LEX,"Lexikalna chyba za cislom nasleduje nepovoleny znak");
			}
		}
		else
		{
			if (!is_white(next_char) && !(next_char==';' || next_char=='+' || next_char=='-' || next_char=='*' || next_char=='/' || next_char==')'))
			{
				ERR_PRINT(ERR_LEX,"Lexikalna chyba za cislom nasleduje nepovoleny znak");
			}
		}
		buffer[lenght]='\0';
		token->attribute=(char *)malloc(100);
		strncpy(token->attribute,buffer,lenght);
		if(token->type == TYPE_UNDEFINED)
		{
			token->type=TYPE_INT_DATA;
		}

	}
	// zaciatok retazca
	else if (actual_char == '"')
	{	
		char buffer[BUFF_SIZE];
		bool str_flag = false;
		lenght=0;
		counter =0;
		actual_char=next_char;
		while(actual_char!=EOF && !str_flag){
			if(actual_char=='\\'){
				next_char=get_char();
				// escapovane znaky \\, \t, \n, \"
				switch (next_char){
					case '"' : 
							actual_char = '"';
							break;
					case 'n' :
							actual_char = '\n';
							break;
					case 't' :
							actual_char = '\t';
							break;
					case '\\' :
							actual_char = '\\';
							break;
				}
				// prvod octalu do desiatkovej sustavy /ddd			
				if(isnumber(next_char)){
					int a=octal(next_char);
					if(a==0){
						ERR_PRINT(ERR_LEX,"Lexikalna chyba nepovolena octalova escape sekvencia");
					}
					else{
						actual_char=(char)a;
						token->type=TYPE_STRING_DATA;
					}

				}
				next_char='\0';
				buffer[lenght]=actual_char;
				counter++;
				lenght=counter % BUFF_SIZE;
				actual_char=get_char();
			}
			else if(actual_char=='\n'){
				ERR_PRINT(ERR_LEX,"Not escaped line");
			}
			else if(next_char=='"' || actual_char=='"'){
				//prazdny string
				buffer[lenght]='\0';
				counter++;
				next_char=get_char();
				lenght=counter % BUFF_SIZE;
				token->attribute=(char *)malloc(100);
				strncpy(token->attribute,buffer,lenght);
				token->type=TYPE_STRING_DATA;
				str_flag=true;
			}
			else
				// koniec retazca
			{	if(actual_char=='"'){
				str_flag=true;
				}
				else{
					buffer[lenght]=actual_char;
					counter++;
					actual_char=get_char();
					lenght=counter % BUFF_SIZE;
				}
			}

		}
		// ak retazec zacal ale do konca suboru neskoncil 
		if (actual_char == EOF){
				ERR_PRINT(ERR_LEX,"Lexikalna chyba neukonceny retazec");
			}
		token->attribute=(char *)malloc(100);
		strncpy(token->attribute,buffer,lenght);
	}
	else
	{
		ERR_PRINT(ERR_LEX,"Lexikalna chyba nerozpoznany znak");
	}

	

	actual_char=next_char;
	next_char='\0';
	return token;
}

/*
 * funkcia ktora vymaze komentare
 */
void skip_comment(){
	char x = get_char();
	bool stop = false;
	if(x == '/')
		while((actual_char != EOF) && (actual_char != 10 && actual_char != 10)){
				actual_char=get_char();
			}
	else if (x == '*'){
				while ((actual_char != EOF) && !stop){
					actual_char=get_char();
					if (actual_char == '*'){
						next_char=get_char();
						if(next_char == '/'){
							stop=true;
						}
					}
				}
			}
	next_char = '\0';
}

/*
 * funkcia ktora vymaze komentare
 * TODO moze nastat /blablablabla... ? dorobit ak hej
 */
 void skip_dust(){
 	while (actual_char != EOF && (is_white(actual_char) || actual_char == '/')){
 		if(actual_char == '/'){
 			skip_comment();
 			actual_char = get_char();
 		}
 		else if (is_white(actual_char)){
 			actual_char = get_char();
 		}
 	}
 }



/*
 * funkcia zisti ci znak patri do skupiny operatorov <=,==,>=,!=
 */
bool is_double_operand(char x, char y){
	if((y == '=') && (x == '<' || x == '=' || x == '>' || x == '!')){
		return true;
	}
	else
		return false;
}

/*
 * funkcia zisti ci znak patri do skupiny operatorov 
 */
bool is_single_operand(char x){
	int asci = (int) x;
	if ((asci == 36)|| (asci >= 40 && asci <= 47 ) || (asci >= 59 && asci <= 62) || (asci == 123) || (asci == 125)){
		return true;
	}
	else
		return false;
}
/*
 * funkcia zisti ci je znak cislo
 */
bool isnumber(char x){
	if (((int)x >= (int)'0' ) && ((int)x<=(int)'9'))
		return true;
	else
		return false;
}

/*
 * funkcia zisti ci je znak pismeno
 */
bool is_word(char x){
	if((((int)x >= (int)'A' ) && ((int)x<=(int)'Z')) || (((int)x >= (int)'a' ) && ((int)x<=(int)'z')))
		return true;
	else
		return false;
}


/*
 * funkcia zisti ci je znak biely znak
 */
bool is_white(char x){
	if (((int)x == (int)' ' ) || ((int)x == (int)'\t' ) || ((int)x == (int)'\n' ) || ((int)x == (int)'\v' ) || ((int)x == (int)'\f' ) || ((int)x == (int)'\r' ))
		return true;
	else
		return false;
}

/*
 * funkcia prekonvertuje octalove cislo na desiatkove
 * PUSH
 */
int octal(char x){
 	char num[3];
 	int result;
 	int check=0;
 	num [0]=x;
 	x=get_char();
 	if(isnumber(x)){
 		num[1]=x;
 		x=get_char();
 		if(isnumber(x)){
 			num[2]=x;
 			check=atoi(num);
 			if(check<1 || check > 377){
 				return 0;
 			}
 			result=(int)strtol(num,NULL,8);
 			if(result>=1 && result<=255)
 				return result;
 			else
 				return 0;
 		}
 		else
 			return 0;
 	}
 	else
 		return 0;
 }

/*
 * funkcia zisti o aky znak sa jedna, vrati jeho typ
 */
Tok_type char_type (char x){
	Tok_type character = TYPE_UNDEFINED;
	switch (x) {
		case '+':
			character = TYPE_ADD;
			break;
		case '-':
			character = TYPE_SUB;
			break;
		case '*':
			character = TYPE_MUL;
			break;
		case '/':
			character = TYPE_DIV;
			break;
		case '=':
			character = TYPE_ASSIG;
			break;
		case '<':
			character = TYPE_LESS;
			break;
		case '>':
			character = TYPE_GREAT;
			break;
		case '{':
			character = TYPE_BLOCK_BEGIN;
			break;
		case '}':
			character = TYPE_BLOCK_END;
			break;
		case '(':
			character = TYPE_LEFT_BRACKET;
			break;
		case ')':
			character = TYPE_RIGHT_BRACKET;
			break;
		case '.':
			character = TYPE_DOT;
			break;
		case ',':
			character = TYPE_COMMA;
			break;
		case ';':
			character = TYPE_SEMICOLON;
			break;
		case '$':
			character = TYPE_DOLAR;
			break;
	}
	return character;
}

/*
 * funkcia zisti o aky znak sa jedna, vrati jeho typ
 */
Tok_type two_char_type (char x, char y){
	Tok_type character = TYPE_UNDEFINED;
	if (y == '=')
	{
		switch (x) {
			case '<' :
				character = TYPE_LESS_OR_EQ;
				break;
			case '>' :
				character = TYPE_GREAT_OR_EQ;
				break;
			case '=' :
				character = TYPE_EQUAL;
				break;
			case '!' :
				character = TYPE_NOT_EQUAL;
				break;
		}
	}
	return character;
}
///////////////////////////////////////////////////////////////////////////////////////////
/******************************** KONIEC SCANERA *****************************************/
// ERR_PRINT a MAIN premiestnit do parsera aby sa to dalo pouzit odvsadial bez zbytocneho 
// includovanie scanera, alebo to vyhodit uplne
/*
 * 		ukoncenie programu
 */
// int main(int argc, char *argv[])
//  {
//  	printf("---------------------------------------\n");
// 	read_file(argc,argv);
	
// 	init_token();
	
// 	// nastaveni tabulek
// 	// inicializace seznamu instrukci

	
// 	while ((tok = get())->type != END_OF_FILE) {
// 		printf("***\n Attribute:%s \n type:%d \n***\n", tok->attribute,tok->type);
// 	}
//  	return 0;
//  }
void ERR_PRINT(Err_value code, const char* message){
	fprintf(stderr,"ERR:%d, %s\n",code,message);
	exit(code);
}
/*
 * 		MAIN SLUZI PRE DEBUG POTOM HO OD... parser staci aby poznal funkciu get() ktora mu vrati token 
 */ 