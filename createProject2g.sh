if [ -r start2g ]; then
    rm -f start2g
else
    echo "Plik nie istnieje"
fi

g++ -o start2g zad2g.cpp  -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++20

if [ -r start2g ]; then
    ./start2g
else
    echo "plik nie istniej"
fi