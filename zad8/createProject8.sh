rm -f start8
clear

g++ -o start8 zad8.cpp tetrimino.h -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++20

./start8
