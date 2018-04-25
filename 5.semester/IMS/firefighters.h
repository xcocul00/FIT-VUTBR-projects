/*
 * autori: 	David Prexta, Maros Cocula
 * predmet: IMS
 * projekt: Model sítě hasičských stanic
 * file: 	firefighters.h
 */

#include "simlib.h"
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <time.h>

#define MAP_SIZE 100			//velkost mapy
#define MOVE_COEF 0.85			//koeficient pre vypocet rychlosti presunu auta k poziaru
#define MOVE_BACK_COEF 0.65		//koeficient pre vypocet rychlosti presunu auta na stanicu

const std::string help_msg = "Spustenie: ./firefighters N\n" \
			"N -- pocet stanic, s ktorymi sa simulacia spusti - povolene hodnoty su 10,12,16,17 a 20\n";

Facility *cars;
Facility get_station("Pokus o zabratie stanice");

Stat carMovement("Presun_aut");
Stat avgDistance("Vzdialenost");
Stat avgFireTime("Cas_poziaru");
Stat avgDamage("Priemerna_skoda");
Stat cenyVyjazdu("Cena_vyjazdu");

int map[MAP_SIZE][MAP_SIZE];
int total_fire_count = 0;
int stat_count = 16;

//struktura reprezentujuca poziarnu stanicu
struct Station{
	int car;
	int x,y;
	double expenses;

	Station():
		car(1),
        x(0), 
        y(0),
        expenses(196000) { }
};

Station *stations;

const int x_coords10[] = {25, 50, 75, 25, 50, 75, 25, 50, 75, 51};
const int y_coords10[] = {25, 25, 25, 50, 50, 50, 75, 75, 75, 51};

const int x_coords12[] = {20, 40, 60, 80, 20, 40, 60, 80, 20, 40, 60, 80};
const int y_coords12[] = {25, 25, 25, 25, 50, 50, 50, 50, 75, 75, 75, 75};

const int x_coords16[] = {20, 45, 70, 95, 20, 45, 70, 95, 20, 45, 70, 95, 20, 45, 70, 95};
const int y_coords16[] = {20, 20, 20, 20, 40, 40, 40, 40, 60, 60, 60, 60, 80, 80, 80, 80};

const int x_coords17[] = {20, 45, 70, 95, 20, 45, 70, 95, 20, 45, 70, 95, 20, 45, 70, 95, 50};
const int y_coords17[] = {20, 20, 20, 20, 40, 40, 40, 40, 60, 60, 60, 60, 80, 80, 80, 80, 50};

const int x_coords20[] = {20, 45, 70, 95, 20, 45, 70, 95, 20, 45, 70, 95, 20, 45, 70, 95, 20, 45, 70, 95};
const int y_coords20[] = {15, 15, 15, 15, 35, 35, 35, 35, 55, 55, 55, 55, 75, 75, 75, 75, 95, 95, 95, 95};