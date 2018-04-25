#/*
# * author:  David Prexta
# * subject: IMS
# * project: Model sítě hasičských stanic
# * file:    Makefile
# */

CC=g++

all: firefighters.cc
	$(CC) -g -O2 -o firefighters firefighters.cc -lsimlib -lm 

run: firefighters
	./firefighters 12
	./firefighters 16
	./firefighters 17
	./firefighters 20

clean:
	rm firefighters
	rm simulation_12_stations.dat
	rm simulation_16_stations.dat
	rm simulation_17_stations.dat
	rm simulation_20_stations.dat
