/*
 * autori: 	David Prexta, Maros Cocula
 * predmet: IMS
 * projekt: Model sítě hasičských stanic
 * file: 	firefighters.cc
 */

#include "firefighters.h"


//funkcia init_stations - inicializuje stanice podla poctu stanic zadanych parametrom
void init_stations(){
	const int *x_coords;
	const int *y_coords;

	//podla poctu stanic sa nacitaju pozicie pre dany pocet
	switch(stat_count){
		case 10:
				x_coords = x_coords10;
				y_coords = y_coords10;
				break;
		case 12:
				x_coords = x_coords12;
				y_coords = y_coords12; 
				break;
		case 16:
				x_coords = x_coords16;
				y_coords = y_coords16;
				break;
		case 17:
				x_coords = x_coords17;
				y_coords = y_coords17;
				break;
		case 20:
				x_coords = x_coords20;
				y_coords = y_coords20;
				break;
	}

	//pre kazdu stanicu sa nastavi pozicia na mape a id auta tejto stanice
	for (int i = 0; i < stat_count; ++i){	
		stations[i].car = i;
		stations[i].x = x_coords[i];
		stations[i].y = y_coords[i];
		map[x_coords[i]][y_coords[i]] = 1;
	}
}


//funckia get_closest_station - vracia id najblizsej volnej stanice
//prametre x_pos a y_pos urcuju poziciu poziaru
//v pripade ze nie je volna ziadna stanica funkcia vracia -1
int get_closest_station(int x_pos, int y_pos){
	int result = -1;
	int station_id = -1;

	for (int i = 0; i < stat_count; ++i){
		if(!cars[stations[i].car].Busy()){
			int distance = sqrt(abs(stations[i].x - x_pos)*abs(stations[i].x - x_pos) + abs(stations[i].y - y_pos)*abs(stations[i].y - y_pos));
			if(distance < result || result == -1){
				result = distance;
				station_id = i;
			}
		}
	}

	return station_id;
}

//funckia get_cars_on_place - vracia pocet aut na mieste poziaru
double get_cars_on_place(int car_id[]){
	double result = 0;

	for (int i = 0; i < 3; ++i){
		if(car_id[i] != -1){
			result++;
		}
	}

	return result;
}

//funkcia get_damage_pet_minute
//v zavislosti od intenzity vracia skodu vzniknutu za 1 minutu poziaru
double get_damage_per_minute(double intensity){

	if(intensity > 0 && intensity <= 130){
		return Normal(6,3);
	}else if(intensity > 130 && intensity <= 230){
		return Normal(30, 5);
	}else if(intensity > 230 && intensity <= 340){
		return Normal(50,5);
	}else{
		return Normal(100,20);
	}
}

class Fire;

//trieda Moving_to_fire - reprezentuje presun auta k poziaru
//jej chovanie je popoisane nizssie
class Moving_to_fire : public Process {
public:
	Fire *F; 	//odkaz na poziar ku ktoremu sa auto presuva
	int id; 	//id auta, ktore sa presuva

    Moving_to_fire(Fire *f,int car_id) : Process(), F(f), id(car_id)  {  
    };
    
    void Behavior();  
};


//trieda Locker - sluzi pre zabranie a uvolnenie Facility auto
//kazdy poziar obsahuje 3 instancie tejto tiredy (poziar moze zabrat max 3 auta)
class Locker : public Process{

	public:
		int id;				//id auta, ktore sa zaberie
		double start_time;	//cas kedy bolo auto zabrane

	Locker(int _id): Process(), id(_id), start_time(Time) {};

	void Behavior(){
		start_time = Time;
		Seize(cars[id]);

		Passivate();

		Release(cars[id]);
	}
};

//Trieda Fire - reprezentuje poziar
class Fire : public Process {
	public:
        int x,y;			//pozicia poziaru
        int car_id[3];		//pole obsahujuce id aut nachadzajucich sa na mieste poziaru
		double intensity;	//intenzita poziaru

	void Behavior(){

	  double fire_start_time = Time;	//pociatocny cas poziaru
	  double damage_done = 0;			//skoda sposobena poziarom
	  int stations_taken = 0;			//pocet stanic zabranych pre tento poziar
	  double cars_on_place = 0;			//pocet aut na mieste poziaru
	  Locker *lockers[] = {(new Locker(-1)),(new Locker(-1)),(new Locker(-1))}; //3 instancie triedy Locker pre zaberanie aut

	  //navestie rep - opakuje sa raz za 1 min. kym nie je poziar uhaseny
	  rep:
	  	//pocet stanic potrebnych pre poziar
	  	int stations_req = (int) intensity/100;

	  	//pokial je pocet stanic potrebnych pre uhasenie mansi ako pocet zabranych stanic tak sa poziaru pokusi priradit dalsia stanica
	  	if(stations_taken < stations_req){
	  		
	  		Seize(get_station);

	  		//vrati sa id najblizsej stanice, ktoru je potrebne zabrat
		  	int take_station = get_closest_station(x,y);

		  	//pokial funkcia get_closest_station nevrati -1 tak sa zaberie stanica
		  	if(take_station != -1){
		  		
		  		//vypocet vzdialenosti od stanice
		  		int distance = sqrt(abs(stations[take_station].x - x)*abs(stations[take_station].x - x) + abs(stations[take_station].y - y)*abs(stations[take_station].y - y));
		  		
		  		//prvej volej instancii tiredy Locker( ktora neobsahuje id stanice -1) sa priradi id zaberanej stanice sa Locker sa aktivuje -> zaberie sa stanica
		  		switch(stations_taken){
		  			case 0: lockers[stations_taken]->id = (take_station); lockers[stations_taken]->Activate(); break;
		  			case 1: lockers[stations_taken]->id = (take_station); lockers[stations_taken]->Activate(); break;
		  			case 2: lockers[stations_taken]->id = (take_station); lockers[stations_taken]->Activate(); break;
		  		}
		  		avgDistance((double)distance);
		  		
		  		//time_to_come - cas za ktory sa auto dostavy k poziaru
		  		double time_to_come = distance/MOVE_COEF + Normal(3,1);

		  		//instancia triedy Moving_to_fire - aktivue sa za cas time_to_come
		  		//po aktivacii je auto na mieste poziaru
		  		(new Moving_to_fire(this, take_station))->Activate(Time+(time_to_come));
		  		carMovement(time_to_come);
		  		stations_taken++;
		  	}
		  	Release(get_station);
	  	}

	  	// volana funkcia pre ziskanie skod z minutu
	  	damage_done += get_damage_per_minute(intensity);
	  	
	  	//volana funkcia ktora vrati pocet aut na mieste poziaru
	  	cars_on_place = get_cars_on_place(car_id);


	  	//pokial su na mieste poziaru nejake auta tak sa poziar hasi
	  	//intenzita sa znizuje v zavislosti od intenzity poziaru a poctu aut na mieste
	  	//hodnoty znizovania intenzity boli zistene experimentalne
	  	if(cars_on_place != 0){
	  		if(intensity > 0 && intensity <=130){

	  			if(cars_on_place == 1){
	  				intensity -= Normal(10,1);
	  			}else if(cars_on_place == 2){
	  				intensity -= Normal(19,1);
	  			}else{
	  				intensity -= Normal(37.5,2.5);
	  			}

	  		}else if(intensity > 130 && intensity <= 230){

	  			if(cars_on_place == 1){
	  				intensity -= Normal(3.5,0.1);
	  			}else if(cars_on_place == 2){
	  				intensity -= Normal(4.5,0.8);
	  			}else{
	  				intensity -= Normal(7,0.9);
	  			}
	  		}else{
	  			//v pripade vysokej intenzity poziaru sa poziar neda s 1 autom uhasit
	  			// v tomto pripade sa da poziar len kontrolovat, intenzita sa stale zvysuje ale pomaly
	  			if(cars_on_place == 1){
	  				intensity += 0.03;
	  			}else if(cars_on_place == 2){
	  				intensity -= Normal(0.02, 0.01);
	  			}else{
	  				intensity -= Normal(0.15, 0.02);
	  			}
	  		}
	  	
	  	//v pripade ze na mieste poziaru nie je ziadne auto tak sa intenzita poziaru zvysuje v zavislosti od intenzity
	  	//poziar s vyssou intenzitou sa siri rychlejsie -> intenzita stupa rychlejsie	
	  	}else{

	  		if(intensity > 0 && intensity <=130){
	  			intensity += 1;
	  		}else if(intensity > 130 && intensity <= 230){
	  			intensity += 1.5;
	  		}else{
	  			intensity += 3;
	  		}
	  		
	  		if(intensity > 350)
	  			intensity = 350;
	  	}

	  	//intenzia poziaru je mensia ako 0 -> poziar bol uhaseny
	  	if(intensity <= 0){

	  		avgFireTime(Time - fire_start_time);	//ulozi sa cas poziaru

	  		//prejdu sa auta na mieste poziaru a spusta sa odpocitavanie pre navrat aut
	   		for (int i = 0; i < 3; ++i){
	   			if(lockers[i]->id != -1){
	   				bool arrived = false;

		  			for (int j = 0; j < 3; ++j){
		  				if(car_id[j] == lockers[i]->id){
		  					arrived = true;
		  					break;
		  				}
		  			}

		  			
		  			//ak je premenna arrived True znamena to ze auto sa nachadzalo na mieste poziaru
		  			//pocita sa s plnou cestou poziaru od stanice
		  			if(arrived){
				  		int distance = sqrt(abs(stations[lockers[i]->id].x - x)*abs(stations[lockers[i]->id].x - x) + abs(stations[lockers[i]->id].y - y)*abs(stations[lockers[i]->id].y - y));
		  				//Instancia triedy locker s danym autom sa aktivuje po case potrebnom pre navrat auta -> auto sa uvolni
		  				lockers[i]->Activate(Time+(distance/MOVE_BACK_COEF));

		  				//k danej stanicy sa pripocitaju naklady za vyjazd
		  				stations[lockers[i]->id].expenses += (((Time - lockers[i]->start_time) + distance/MOVE_BACK_COEF)*3.5);
		  				cenyVyjazdu((((Time - lockers[i]->start_time) + distance/MOVE_BACK_COEF)*3.5));
		  			//ak je premenna arrived False - auto nedoslo na miesto poziaru - poziar bol uhaseny pocas cesty k nemu
		  			//pocita sa s cestou od stanice az na miesto kam auto doslo
		  			}else{
		  				int distance = sqrt(abs(stations[lockers[i]->id].x - x)*abs(stations[lockers[i]->id].x - x) + abs(stations[lockers[i]->id].y - y)*abs(stations[lockers[i]->id].y - y));
		  				lockers[i]->Activate(Time+( ( ((distance*Time-lockers[i]->start_time)/(distance*MOVE_COEF)) / MOVE_BACK_COEF) ));
		  			}
	   			}
	   		}

			avgDamage(damage_done); //pricita sa skoda vzinknuta pri poziare
	  		map[x][y] = 0;
		
		//intenzita poziaru je > 0 -> poziar nebol uhaseny - uspi sa na 1 min a potom pokracuje skokom na navestie rep
	  	}else{

	  		Wait(1);
	  		goto rep;
	  	}
	}
};

//Popis chovania triedy Moving_to_fire
//atuo dorazilo na miesto -> v premennej car_id konkretneho poziaru sa nastavi id auta ktore na miesto dorazilo
void Moving_to_fire::Behavior(){
	
	for (int i = 0; i < 3; ++i){
    	if(F->car_id[i] == -1){
    		F->car_id[i] = id;
    		break;
    	}
    }
}

//trieda fireGenerator - generuje pozriaz 
//kazdemu poziaru je vygenerovana nahodna pozicia na mape na ktorej vznikne a
//pociatocna intenzita poziaru
class fireGenerator : public Event {
    void Behavior() {

    	Fire *f = new Fire;
    	
    	//vygeneruje sa pozicia poziaru
    	while(1){
    		int new_x = Random() * MAP_SIZE;
    		int new_y = Random() * MAP_SIZE;

    		if(!map[new_x][new_y]){
    			f->x = new_x;
    			f->y = new_y;
    			map[new_x][new_y] = 1;
    			break;
    		}
    	}

    	//vygeneruje sa pociatocna intenzita
    	double intensity = Random();

    	if(intensity < 0.73){
    		f->intensity = 100;
    	}else if(intensity >= 0.74 &&  intensity < 0.98){
    		f->intensity = 200;
    	}else{
    		f->intensity = 300;
    	}
    	
    	for (int i = 0; i < 3; ++i){
 			f->car_id[i] = -1;
    	}  	
    	total_fire_count++;

    	f->Activate();
        Activate(Time + Exponential(8*60));
    }
};


int main(int argc,char* argv[])
{	
	//kontorla poctu parametrov
	if(argc != 2){
		std::cout << help_msg;
		return 1;
	}

	//kontrola parametru urcujuceho pocet stanic, musi byt 10,12,16,17 alebo 20
	char *endp;
	int stations_count = strtol (argv[1], &endp, 10);
	if(*endp){
		std::cout << help_msg;
		return 1;
	}

	switch(stations_count){
		case 10: stat_count = 10; break;
		case 12: stat_count = 12; break;
		case 16: stat_count = 16; break;
		case 17: stat_count = 17; break;
		case 20: stat_count = 20; break;
		default: 
				std::cout << help_msg;
				return 1;
	}

	//nastavenie vystupneho suboru
	std::string filename(argv[1]);
	filename = "simulation_" + filename + "_stations.dat";
	SetOutput(filename.c_str());

	//inicializacia - obdobie 1 rok	
	Init(0,60*24*365); 
	RandomSeed(time(NULL));
	
	//inicializacia mapy
	for (int i = 0; i < MAP_SIZE; ++i){
		for (int j = 0; j < MAP_SIZE; ++j){
			map[i][j] = 0;
		}
	}

	cars = new Facility[stat_count];
	stations = new Station[stat_count];
	(new fireGenerator)->Activate(Time + Exponential(8*60));

	//inicializacia stanic
	init_stations();
	Run();

	//vystup simulacie
	carMovement.Output();
	avgDistance.Output();
	avgFireTime.Output();
	avgDamage.Output();
	cenyVyjazdu.Output();
	Print("Pocet vzniknutych poziarov:\t%d\n", total_fire_count);
	Print("Celkove skody:\t%f\n", avgDamage.Sum());
	double n = 0;
	for (int i = 0; i < stat_count; ++i){
		n += stations[i].expenses;
	}
	Print("Celkove naklady:\t%f\n",n);
};
