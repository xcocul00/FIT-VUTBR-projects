/**
 * Predmet - Mikroprocesorove a vestavene systemy
 * Projekt - MSP430: Řízení výtahu
 * Autor   - Maros Cocula xcocul00
 * Kontakt - xcocul00@stud.fit.vutbr.cz
 * Pomer zmen v souboru - 94%
 */
#include <stdio.h>
#include <fitkitlib.h>
#include <keyboard/keyboard.h>
#include <lcd/display.h>
#include "FreeRTOS.h" // vice informaci na adrese http://www.freertos.org/
#include "task.h"

/*
* potrebne premenne
*/
int stop_button = 0;
volatile char pressed;

int kab_pos=0;
int schody_pos=0;
int kabina []= {-2,-2,-2,-2};
int schody []= {-2,-2,-2,-2};
pos1=0;
pos2=0;

int poschodie=1;
int pohyb=0;
int smer=0;
int dvere=0;

/**
 * Vytvorenie znakov sipky hore a dole pre vykreslenie na display
 */
unsigned char sipka_hore[8] = {0x04, 0x0e, 0x15, 0x04, 0x04, 0x04, 0x04, 0x04};
unsigned char sipka_dole[8] = {0x04, 0x04, 0x04, 0x04, 0x04, 0x15, 0x0e, 0x04};
unsigned char otvorene[8] = {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11};
unsigned char zatvorene[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
/**
 * Zistenie ci uz bolo tlacidlo naposchodi zavolane
 */
int is_free (int button, int x){
    int i;
    for(i=0;i<4;i++){
        if(x==0){
        if(kabina[i]==button){
            return 1;
        }}
        else{
            if(x==1){
                if(schody[i]==button){
                    return 1;   
            }
        }
    }}
    return 0;
}
/**
 * Vypis informacii na display
 */
static void print_pos(void *param) {
    char x[5];
    for(;;){

            LCD_clear();
            LCD_append_string("P=");
           if(poschodie==0){
                LCD_append_string("-1");
            }
           else{
            sprintf(x, "%d", poschodie); 
            LCD_append_string(x); 
            } 
           LCD_append_string(" S=");
           if(smer==1){
            LCD_append_char('\x1');
           }
           else if (smer==-1){
            LCD_append_char('\x2');
           }
           else
           LCD_append_string("-");    
            

          LCD_append_string(" D=");
          if(dvere==1){
            LCD_append_char('\3');
          }
          else
            LCD_append_char('\4');

         if(stop_button==1){
            LCD_clear();
            LCD_append_string("Vytah zaseknuty!");
         }
     vTaskDelay(100 / portTICK_RATE_MS); 
    }

}
/**
 * Inicializacia poli a premennych po restarte
 */
static void init_array(){
    int i;
    for(i=0;i<4;i++){
        kabina[i]=-2;
        schody[i]=-2;
    }
    smer=0;
    dvere=0;
}

/**
 * Task obsluhujici klavesnici.
 */
static void keyboardHandler(void *param) {
    char ch;

    pressed = 0;
    keyboard_init(); 
    for (;;) {
        ch = key_decode(read_word_keyboard_4x4());
        if (ch != pressed) {
            pressed = ch;
            // vratenie sa na zaciatok pola
            if (ch != 0) {
                if(kab_pos==4){
                    kab_pos=0;
                }
                if(schody_pos==4){
                    schody_pos=0;
                }
                // switch pre stlacene tlacidla
                switch (ch) {
                    // tlacidlo Total STOP
                    case '#':
                        if(stop_button==0){
                        stop_button=1;
                        term_send_str_crlf("\tVytah bol nudzovo zastaveny.");
                        }else
                        term_send_str_crlf("\tVytah nefunguje pockajte na servis.");
                        break;
                    // tlacidlo opravy
                    case '*':
                        if(stop_button==1){
                        init_array();
                        stop_button=0;
                        poschodie=1;
                        term_send_str_crlf("\tVytah opraveny, vytah je na 1 poschodi.");
                        }
                        break;
                    // tlacidla kabiny
                        // poschodie 3
                    case 'A':
                        if (stop_button == 1) {
                            term_send_str_crlf("\tVytah nefunguje, pockajte na servis.");
                        }else{
                                if(!is_free(3,0)){
                                    if (poschodie!=3){
                                    kabina[kab_pos]=3;
                                    term_send_str_crlf("\tKabina : tlacidlo 3.");
                                }else{
                                        dvere=1;
                                        vTaskDelay(2000 / portTICK_RATE_MS);
                                        dvere=0;
                                    }                  
                                }
                        }
                        break;
                        // poschodie 2
                    case 'B':
                        if (stop_button == 1) {
                            term_send_str_crlf("\tVytah nefunguje, pockajte na servis.");
                            break;
                        }else{
                                if(!is_free(2,0)){
                                    if (poschodie!=2){
                                    kabina[kab_pos]=2;
                                    term_send_str_crlf("\tKabina : tlacidlo 2.");                  
                                     }else{
                                        dvere=1;
                                        vTaskDelay(2000 / portTICK_RATE_MS);
                                        dvere=0;
                                    }}
                        }
                        break;
                        // poschodie 1
                    case 'C':
                        if (stop_button == 1) {
                            term_send_str_crlf("\tVytah nefunguje, pockajte na servis.");
                            break;
                        }else{
                                if(!is_free(1,0)){
                                if (poschodie!=1){
                                kabina[kab_pos]=1;
                                term_send_str_crlf("\tKabina : tlacidlo 1.");                  
                                }else{
                                        dvere=1;
                                        vTaskDelay(2000 / portTICK_RATE_MS);
                                        dvere=0;
                                    }}
                        }
                        break;
                        // poschodie -1
                    case 'D':
                        if (stop_button == 1) {
                            term_send_str_crlf("\tVytah nefunguje, pockajte na servis.");
                            break;
                        }else{
                                if(!is_free(0,0)){
                                if (poschodie!=0){
                                kabina[kab_pos]=0;
                                term_send_str_crlf("\tKabina : tlacidlo -1.");                  
                                }else{
                                        dvere=1;
                                        vTaskDelay(2000 / portTICK_RATE_MS);
                                        dvere=0;
                                    }}
                        }
                        break;
                        // tlacidla schodist
                        // poschodie 3
                    case '2':
                        if (stop_button == 1) {
                            term_send_str_crlf("\tVytah nefunguje, pockajte na servis.");
                            break;
                        }else{
                                if(!is_free(3,1)){
                                if (poschodie!=3){
                                schody[schody_pos]=3;
                                term_send_str_crlf("\tTlacidlo z poschodia 3.");                  
                                }else{
                                        dvere=1;
                                        vTaskDelay(2000 / portTICK_RATE_MS);
                                        dvere=0;
                                    }}
                        }
                        break;
                        // poschodie 2
                    case '5':
                        if (stop_button == 1) {
                            term_send_str_crlf("\tVytah nefunguje, pockajte na servis.");
                            break;
                        }else{
                                if(!is_free(2,1)){
                                if (poschodie!=2){
                                schody[schody_pos]=2;
                                term_send_str_crlf("\tTlacidlo z poschodia 2.");                  
                                }else{
                                        dvere=1;
                                        vTaskDelay(2000 / portTICK_RATE_MS);
                                        dvere=0;
                                    }}
                        }
                        break;
                        // poschodie 1
                    case '8':
                        if (stop_button == 1) {
                            term_send_str_crlf("\tVytah nefunguje, pockajte na servis.");
                            break;
                        }else{
                                if(!is_free(1,1)){
                                if (poschodie!=1){
                                schody[schody_pos]=1;
                                term_send_str_crlf("\tTlacidlo z poschodia 1.");                  
                                }else{
                                        dvere=1;
                                        vTaskDelay(2000 / portTICK_RATE_MS);
                                        dvere=0;
                                    }}
                        }
                        break;
                        // poschodie -1
                    case '0':
                        if (stop_button == 1) {
                            term_send_str_crlf("\tVytah nefunguje, pockajte na servis.");
                            break;
                        }else{
                                if(!is_free(0,1)){
                                if (poschodie!=0){
                                schody[schody_pos]=0;
                                term_send_str_crlf("\tTlacidlo z poschodia -1.");                  
                                }else{
                                        dvere=1;
                                        vTaskDelay(2000 / portTICK_RATE_MS);
                                        dvere=0;
                                    }}
                        }
                        break;  
                        //NAPOVEDA                     
                    default:
                        print_user_help();
                        break;
                }
                // posun v poliach
                kab_pos++;
                schody_pos++;
            }
        }
        vTaskDelay(15 / portTICK_RATE_MS); 
    }
}

/**
 * Presun vytahu medzi poschodiami
 */
static void presun_vytahu(int dlzka,int smer){
    if(dlzka==1){
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
    else if (dlzka==2){
        vTaskDelay(1000 / portTICK_RATE_MS);
        if(smer)
            poschodie++;
        else
            poschodie--;
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
    else if (dlzka==3){
        vTaskDelay(1000 / portTICK_RATE_MS);
        if(smer)
            poschodie++;
        else
            poschodie--;
        vTaskDelay(1000 / portTICK_RATE_MS);
        if(smer)
            poschodie++;
        else
            poschodie--;
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
    else if (dlzka==4){
        vTaskDelay(1000 / portTICK_RATE_MS);
        if(smer)
            poschodie++;
        else
            poschodie--;
        vTaskDelay(1000 / portTICK_RATE_MS);
        if(smer)
            poschodie++;
        else
            poschodie--;
        vTaskDelay(1000 / portTICK_RATE_MS);
        if(smer)
            poschodie++;
        else
            poschodie--;
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}



/**
 * Task obsluhujici kam pojde vytah
 */
static void vytahTask(void *param) {
    int sek=0;
    char x[15];
    term_send_str_crlf("Start vytah je na 1 poschodi\n");
    for(;;){
        if(pos1==4){
            pos1=0;
        }
        if(pos2==4){
            pos2=0;
        }
        //tlacidlo kabina vytah pojde hore
        if( (kabina[pos1]>poschodie) && (kabina[pos1]!=-2 && stop_button!=1)) {
                pohyb=1;
               term_send_str_crlf("Vytah pojde hore\n");
               smer=1;
               sek=kabina[pos1]-poschodie;
               term_send_str_crlf("Presun\n");
               presun_vytahu(sek,1);
               if(stop_button!=1){
                sprintf(x,"%d",kabina[pos1]);
                term_send_str("Poschodie :");
                if(kabina[pos1]==0){
                    term_send_str_crlf("-1");
                }else
                term_send_str_crlf(x);

               term_send_str_crlf("Otvara dvere\n");
               
               poschodie=kabina[pos1];
               smer=0;
               vTaskDelay(2000 / portTICK_RATE_MS);
               dvere=1;
               vTaskDelay(2000 / portTICK_RATE_MS);
               term_send_str_crlf("Zatvara dvere\n");
               dvere=0;
               }
               kabina[pos1]=-2;
               pos1++;
            }//tlacidlo kabina vytah pojde dole
        else {
                if((kabina[pos1]<poschodie) && (kabina[pos1]!=-2) && stop_button!=1){
                pohyb=1;
               term_send_str_crlf("Vytah pojde dole\n");
               smer=-1;
               sek=poschodie-kabina[pos1];
               term_send_str_crlf("Presun\n");
               presun_vytahu(sek,0);
               if(stop_button!=1){
                sprintf(x,"%d",kabina[pos1]);
                term_send_str("Poschodie :");
                if(kabina[pos1]==0){
                    term_send_str_crlf("-1");
                }else
                term_send_str_crlf(x);
               term_send_str_crlf("Otvara dvere\n");
               poschodie=kabina[pos1];
               smer=0;
               vTaskDelay(2000 / portTICK_RATE_MS);
               dvere=1;
               vTaskDelay(2000 / portTICK_RATE_MS);
               term_send_str_crlf("Zatvara dvere\n");
               dvere=0; 
               }
               kabina[pos1]=-2;
               pos1++;
            }
        else pos1++;
        }//tlacidlo schody vytah pojde hore
        if( (schody[pos2]>poschodie) && (schody[pos2]!=-2 && stop_button!=1)){
                pohyb=1;
               term_send_str_crlf("Vytah pojde hore\n");
               smer=1;
               sek=schody[pos2]-poschodie;
               term_send_str_crlf("Presun\n");
               presun_vytahu(sek,1);
               if(stop_button!=1){
                sprintf(x,"%d",schody[pos2]);
                term_send_str("Poschodie :");
                if(schody[pos2]==0){
                    term_send_str_crlf("-1");
                }else
                term_send_str_crlf(x);
                vTaskDelay(500 / portTICK_RATE_MS);
               term_send_str_crlf("Otvara dvere\n");

               poschodie=schody[pos2];
               smer=0;
               vTaskDelay(2000 / portTICK_RATE_MS);
               dvere=1;
               vTaskDelay(2000 / portTICK_RATE_MS);
               term_send_str_crlf("Zatvara dvere\n");
               dvere=0;
               }
               schody[pos2]=-2;
               pos2++;
            }//tlacidlo schody vytah pojde dole
        else {
                if((schody[pos2]<poschodie) && (schody[pos2]!=-2) && stop_button!=1){
                pohyb=1;
               term_send_str_crlf("Vytah pojde dole\n"); 
               smer=-1;
               sek=poschodie-schody[pos2];
               term_send_str_crlf("Presun\n");
               presun_vytahu(sek,0);
               if(stop_button!=1){
                sprintf(x,"%d",schody[pos2]);
                term_send_str("Poschodie :");
                if(schody[pos2]==0){
                    term_send_str_crlf("-1");
                }else
                term_send_str_crlf(x);
                vTaskDelay(500 / portTICK_RATE_MS);
               term_send_str_crlf("Otvara dvere\n");
               poschodie=schody[pos2];
               smer=0;
               vTaskDelay(2000 / portTICK_RATE_MS);
               dvere=1;
               vTaskDelay(2000 / portTICK_RATE_MS);
               term_send_str_crlf("Zatvara dvere\n");
               dvere=0;
               }
               schody[pos2]=-2;
               pos2++;
            }
        else pos2++;
        }

    pohyb=0;
    }
}



/**
 * Vypise napovedu
 */
void print_user_help(void) {
    term_send_str_crlf("**************\nNapoveda\nTlacidla : \nKabina - A-D (poschodie, kde A=3 a D=-1)\nPoschodia 2,5,8,0 (poschodie, kde 2=3 a 0=-1)\n# - STOP\n* - oprava vytahu\n**************\n");
}

unsigned char decode_user_cmd(char *cmd_ucase, char *cmd) {
}

/**
 * Init FPGA chipu
 */
void fpga_initialized() {
    LCD_init();
    LCD_load_char(1, sipka_hore);
    LCD_load_char(2, sipka_dole);
    LCD_load_char(3, otvorene);
    LCD_load_char(4, zatvorene);
    //LCD_append_string("MSP430: Vytah"); // napiseme uvodni text na displej    
}

/**
 * MAIN
 */
int main(void) {
    initialize_hardware();
    WDG_stop(); // zastavim Watch Dog
	term_send_crlf();
	// vytvoreni tasku
    xTaskCreate(vytahTask, "Vytah", 50, NULL, 1, NULL);
    xTaskCreate(keyboardHandler, "KeybordHandler", 32, NULL, 1, NULL);
    xTaskCreate(print_pos, "Printer", 80, NULL, 1, NULL);
    


    /**
     * FreeRTOS scheduler.
     */
    term_send_str_crlf("Start programu:");
    vTaskStartScheduler();

    return 0;
}
