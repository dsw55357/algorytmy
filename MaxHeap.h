#ifndef MAXHEAP_H
#define MAXHEAP_H

#include "Circle.h"
#include <vector>
#include <stdexcept>

class MaxHeap {
private:
    std::vector<Circle> heap;

    void heapifyUp(int index) {
        if (index == 0) return;
        int parent = (index - 1) / 2;
        if (heap[index] > heap[parent]) {
            std::swap(heap[index], heap[parent]);
            heapifyUp(parent);
        }
    }

    void heapifyDown(int index) {
        int leftChild = 2 * index + 1;
        int rightChild = 2 * index + 2;
        int largest = index;

        if (leftChild < heap.size() && heap[leftChild] > heap[largest])
            largest = leftChild;

        if (rightChild < heap.size() && heap[rightChild] > heap[largest])
            largest = rightChild;

        if (largest != index) {
            std::swap(heap[index], heap[largest]);
            heapifyDown(largest);
        }
    }

public:
    void insert(const Circle& value) {
        heap.push_back(value);
        heapifyUp(heap.size() - 1);
    }

    Circle extractMax() {
        if (heap.empty()) throw std::runtime_error("Heap is empty");
        Circle maxValue = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        heapifyDown(0);
        return maxValue;
    }

    const Circle& getMax() const {
        if (heap.empty()) throw std::runtime_error("Heap is empty");
        return heap[0];
    }

    bool isEmpty() const {
        return heap.empty();
    }

    int getSize() const {
        return heap.size();
    }

    const std::vector<Circle>& getHeap() const {
        return heap;
    }

};

#endif // MAXHEAP_H
