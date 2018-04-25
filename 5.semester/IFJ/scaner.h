#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#define KEYS_COUNT 17

/*
 *	enum pre rozne typy tokenu
 */
typedef enum {
	TYPE_UNDEFINED,   	//0
	// KEYS
	TYPE_BOOLEAN,		//1
	TYPE_BREAK,			//2
	TYPE_CLASS,			//3
	TYPE_CONTINUE,		//4
	TYPE_DO,			//5
	TYPE_DOUBLE,		//6
	TYPE_ELSE,			//7
	TYPE_FALSE,			//8
	TYPE_FOR,			//9
	TYPE_IF,			//10
	TYPE_INT,			//11
	TYPE_RETURN,		//12
	TYPE_STRING,		//13
	TYPE_STATIC,		//14
	TYPE_TRUE,			//15
	TYPE_VOID,			//16
	TYPE_WHILE,			//17
	// OPERATORS
	//ARYTHMETIC
	TYPE_ADD,			//18 +
	TYPE_SUB,			//19 -
	TYPE_MUL,			//20 *
	TYPE_DIV,			//21 /

	// PRECEDENT ANALYSE
	TYPE_ID,			//22 i
	TYPE_DOLAR,			//23 $
	
	// COMPARISON
	TYPE_ASSIG,			//24 =
	TYPE_LESS, 			//25 <
	TYPE_GREAT,			//26 >
	TYPE_LESS_OR_EQ,	//27 <=
	TYPE_GREAT_OR_EQ,	//28 >=
	TYPE_EQUAL,			//29 ==
	TYPE_NOT_EQUAL,		//30 !=

	// OTHER
	TYPE_BLOCK_BEGIN,	//31 {
	TYPE_BLOCK_END,		//32 }
	TYPE_LEFT_BRACKET,	//33 (
	TYPE_RIGHT_BRACKET,	//34 )
	TYPE_DOT,			//35 .
	TYPE_COMMA,			//36 ,
	TYPE_SEMICOLON,		//37 ;
		TYPE_PTE,			//38 PTE


	//DATA TYPES
	TYPE_INT_DATA,		//39
	TYPE_DOUBLE_DATA,	//40
	TYPE_STRING_DATA,	//41
	END_OF_FILE,		//42
	TYPE_NAME,			//43

	//BUILT-IN FUNCTIONS
	INT_READ_INT,    	//44
	DOUBLE_READ_DOUBLE, 	//45
	STRING_READ_STRING, 	//46
	VOID_PRINT, 			//47

	INT_LENGTH,			//48
	STRING_SUBSTR,	 	//49
	INT_COMPARE, 		//50
	INT_FIND,			//51
	STRING_SORT, 		//52

	TYPE_JUMP	,		//53
	// I_PRINT				//54 - toto ide dopice yachvilu
	I_RAMEC ,	//54
	I_ASSIGN_ARG,	//55
	// I_RETURN,	//55
	// I_BREAK,	//56
	// I_CONTINUE, //57
	TYPE_PT_ERROR, //56
	PODMINKA_begin, //57
	PODMINKA_end,	//58
	TYPE_FUNC_END // 59


} Tok_type;

/*
 *	enum pre chyby TODO moze sa to potom dat do hlavicky parseru aby to bolo po kope !!!! 
 */
typedef enum {
	ERR_OK, 				//0
	ERR_LEX,				//1
	ERR_SYN,				//2
	ERR_SEM,				//3
	ERR_SEM_TYPE,			//4
	ERR_SEM_OTHERS,			//5
	ERR_INTERPRET_NUMBER,	//6
	ERR_INTERPRET_VAL,		//7
	ERR_INTERPRET_OTHER,	//8
	ERR_INSIDE=99,			//99
}Err_value;

/*
 *	struktura TOKEN  
 * DOLEZITE PRE PARSER
 */
typedef struct {
	char *attribute;
	Tok_type type;
}token;




/*
 *	konstanty pre klucove slova
 */
const char * const key[] = {
	"boolean",
	"break",
	"class",
	"continue",
	"do",
	"double",
	"else",
	"false",
	"for",
	"if",
	"int",
	"return",
	"String",
	"static",
	"true",
	"void",
	"while",
};


/* PROTOTYPY */
void read_file(int argc, char **argv);
void close_file();
char get_char();
token * init_token();
token * get();

bool isnumber(char x);
bool ishexanubmer(char x);
bool is_white(char x);
bool is_double_operand(char x, char y);
bool is_single_operand(char x);
bool is_word(char x);
int octal(char x);

Tok_type char_type (char x);
Tok_type two_char_type (char x, char y);

void skip_dust();
//odstranit
void ERR_PRINT(Err_value code, const char* message);
