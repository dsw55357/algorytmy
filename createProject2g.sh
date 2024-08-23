if [ -r zad2g ]; then
    rm -f zad2g
else
    echo "Plik nie istnieje"
fi

g++ -o zad2g zad2g.cpp  -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++20

if [ -r zad2g ]; then
    ./zad2g
else
    echo "plik nie istniej"
fi