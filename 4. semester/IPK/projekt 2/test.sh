#!/bin/sh

echo "Spustam test"
echo "Zacinam preklad"

make

echo "Spustam server na porte 100 000"
./server -p 100000  &

echo "Pokus o upload suboru test.txt"
./client -h 127.0.0.1 -p 100000 -u test.txt

echo "Pokus o upload obrazku fit.jpg"
./client -h 127.0.0.1 -p 100000 -u fit.png

echo "Pokus o download suboru test.txt"
./client -h 127.0.0.1 -p 100000 -d test.txt

echo "Pokus o stiahnutie neexistujuceho suboru"
./client -h 127.0.0.1 -p 100000 -d neexist.txt

echo "pokus o prepisanie server.cpp"
./client -h 127.0.0.1 -p 100000 -u server.cpp

echo "Koncim server"
kill $!
echo "Spustam make clean"
make clean