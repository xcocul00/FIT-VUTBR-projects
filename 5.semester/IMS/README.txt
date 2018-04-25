Projekt 1 - 20/20


firefighters

Autori:
	David Prexta, xprext00
	Maros Cocula, xcocul00

Popis programu:
	Program pre simulaciu siete hasicskych stanic. Simulacia prebieha na mape o velkosti 100x100 km.
	Pocet stanic s ktorymi simulacia pobezi je mozne zvolit z 10,12,16,17 alebo 20 stanic. Simulacny cas
	je 1 rok. Vystup simulacie sa ulozi do suboru s nazvom:

	simulation_N_stations.dat

	kde N je pocet stanic, s ktorymi bola simulacia spustena.

Preklad:
	Pre prelozenie programu je prilozeny subor Makefile. Pre preklad je potrebna kniznica simlib.

	Príklad:
		$ make 		- prelozi program
		$ make run 	- spusti simulacie pre 10,12,16,17 a 20 stanic
		$ make clean 	- odstranenie prelozeneho programu a vystupov simulacii.

Spustenie:
	
	$ ./firefighters N

	N 	- pocet stanic s pre ktore ma simulacia bezat
		- povolene hodnoty su 10,12,16,17 a 20

	- pri spusteni programu chybne sa vypise napoveda

Priklady spustenia:
		$ ./firefighters 16
			-spusti simulaciu pre 16 stanic
			-vysledok sa ulozi do suboru simulation_16_stations.dat

Zoznam suborov:
	firefighters.cc	- subor so zdrojovym kodom programu
	firefighters.h  - kniznica
	Makefile 	- subor pre preklad
	Readme  	- tento subor
	dokumentacia.pdf	- dokumentacia
