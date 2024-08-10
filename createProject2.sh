

rm -f start2

g++ -o start zad2.cpp Array.cpp SinglyLinkedList.cpp DoublyLinkedList.cpp ArrayLinkedList.cpp ArrayDoublyLinkedList.cpp MaxHeap.cpp Queue.cpp SinglyLinkedListQueue.cpp ArrayStack.h SinglyLinkedListStack.h  -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++20

./start2