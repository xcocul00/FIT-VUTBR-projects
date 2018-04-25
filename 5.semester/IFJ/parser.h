
#ifndef _PARSER_H_
#define _PARSER_H_

/* Pomocna struktura pro ukladani informace o promenne/funkci   */
typedef struct{
    bool st_static;
    int type;
    char* name;

}tmp_struct;





/*
				TODO
- kontrolovat pocet argumentu u vestavenych funkci

*/
bool sber = true;

token *tok;

bool DEBUG = false;

// Pomocne prom ------
bool returns = false;
tmp_struct tmp;
int arg = 0;
bool is_run = false;	// CHECK -trida musi byt main a obsahovat run
bool is_main = false;
bool in_main = false;

//
int fce_podminka = 0;

// TS - class
hash_table *table_class;// main
tabItem *class_save;	// pomocna
// TS - func
tabItem *func_save;		//pomocna

//pozice parametru
int tmp_param_pos = 0;
char *tmp_first_param = NULL;
char *previouse_param = NULL;


int main(int argc, char *argv[]);
int parser();
void class_parse();
void class_body();
void prom_def(bool is_def);
void func_def();
int get_variable();
void func_param();
void func_param_next();
void func_body();
int key_symbols();
void parse_key_symbols();
tabItem *get_argument();
int read_func(tabItem *item, char *func_name);


#endif
