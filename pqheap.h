/* Name: Alyssa Choi
 * Section leader: Ayelet Drazen
 * Assignment brief: define pqheap class and methods
 */
#pragma once
#include "testing/MemoryUtils.h"
#include "datapoint.h"

/**
 * Priority queue of DataPoints implemented using a binary heap.
 */
class PQHeap {
public:
    /**
     * Creates a new, empty priority queue.
     */
    PQHeap();

    /**
     * Cleans up all memory allocated by this priorty queue.
     */
    ~PQHeap();

    /**
     * Adds a new element into the queue. This operation runs in time O(log n),
     * where n is the number of elements in the queue.
     *
     * @param element The element to add.
     */
    void enqueue(DataPoint element);

    /**
     * Removes and returns the element that is frontmost in the priority queue.
     * The frontmost element is the one with lowest priority value.
     *
     * If the priority queue is empty, this function calls error().
     *
     * This operation must run in time O(log n).
     *
     * @return The frontmost element, which is removed from queue.
     */
    DataPoint dequeue();

    /**
     * Returns, but does not remove, the element that is frontmost.
     *
     * If the priority queue is empty, this function calls error().
     *
     * This operation must run in time O(1).
     *
     * @return frontmost element
     */
    DataPoint peek() const;

    /**
     * Returns whether the priority queue is empty.
     *
     * This operation must run in time O(1).
     *
     * @return Whether the priority queue is empty.
     */
    bool isEmpty() const;

    /**
     * Returns the number of elements in this priority queue.
     *
     * This operation must run in time O(1).
     *
     * @return The number of elements in the priority queue.
     */
    int size() const;

    /**
     * Removes all elements from the priority queue.
     *
     * This operation must run in time O(1).
     */
    void clear();

    /*
     * This function exists purely for testing purposes. You can have it do whatever you'd
     * like and we won't be invoking it when grading. In the past, students have had this
     * function print out the array representing the heap, or information about how much
     * space is allocated, etc. Feel free to use it as you see fit!
     */
    void printDebugInfo();

    /*
     * This function exits purely for testing purposes. It verifies
     * that the internal state of the queue is valid/consistent.
     * If a problem is detected, this function calls error().
     * If no problem, the function returns normally.
     */
    void validateInternalState();

private:
    DataPoint* _heap;   // dynamic array
    int _numAllocated;      // number of slots allocated in array
    int _numFilled;         // number of slots filled in array

    void ensureCapacity(); // helper function expands the array size if run out of space
    void swap(int indexOne, int indexTwo); // helper function that swaps two elements
    void bubbleUp(); // helper function that bubbles up for enqueue
    void bubbleDown(); // helper function that bubbles down for dequeue

    /* While not a strict requirement, we strongly recommend implementing the
     * helper functions defined below. They will make your code much cleaner, and
     * will aid in the debuggin process.
     */

    /* These three functions are responsible for doing the math to translate
     * between an index in the array and the indexes of the elements parent, left
     * child and right child, respectively.
     */
    int getParentIndex(int curIndex);
    int getLeftChildIndex(int curIndex);
    int getRightChildIndex(int curIndex);

    /* Weird C++isms: C++ loves to make copies of things, which is usually a good thing but
     * for the purposes of this assignment requires some C++ knowledge we haven't yet covered.
     * This next line disables all copy functions to make sure you don't accidentally end up
     * debugging something that isn't your fault.
     *
     * Curious what this does? Take CS106L!
     */
    DISALLOW_COPYING_OF(PQHeap);
};
