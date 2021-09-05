/* Name: Alyssa Choi
 * Section leader: Ayelet Drazen
 * Assignment brief: define the PQHeap (priority queue) class and methods using a binary min-heap.
 */
#include "pqheap.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "testing/SimpleTest.h"
using namespace std;

const int INITIAL_CAPACITY = 10;

/*
 * The constructor initializes all of the member variables needed for
 * an instance of the PQSortedArray class. The allocated capacity
 * is initialized to a starting constant and a dynamic array of that
 * size is allocated. The number of filled slots is initially zero.
 */
PQHeap::PQHeap() {
    _numAllocated = INITIAL_CAPACITY;
    _heap = new DataPoint[_numAllocated];
    _numFilled = 0;
}

/* The destructor is responsible for cleaning up any resources
 * used by this instance of the PQSortedArray class. The array
 * memory used for elements is deallocated here.
 */
PQHeap::~PQHeap() {
    delete[] _heap;
}

/* HELPER FUNCTION: helper function for enqueueing. It ensures that there is always extra capacity.
 * This function expands the array if we run out of space by doubling the number of
 * allocated slots. It takes in no parameters.
 */
void PQHeap::ensureCapacity() {
    if (_numFilled == _numAllocated) {
        // 1. Create a new larger array by asking for memory space
        DataPoint* _newHeap = new DataPoint[_numAllocated * 2];

        // 2. copy the old array elts into the new array
        for (int i = 0; i < _numFilled; i++) {
            _newHeap[i] = _heap[i];
        }

        // 3. delete (free) the old array
        delete[] _heap;

        // 4. point old array variable to new array
        _heap = _newHeap;

        // 5. Update the capacity variable for the new array
        _numAllocated *= 2;
    }
}

/* HELPER FUNCTION: helper function for 'bubbling up' and 'bubbling down'.
 * This function swaps two elements.
 */
void PQHeap::swap(int indexOne, int indexTwo) {
    DataPoint temp = _heap[indexOne];
    _heap[indexOne] = _heap[indexTwo];
    _heap[indexTwo] = temp;
}

/* HELPER FUNCTION: 'bubbling up' for enqueue.
 */
void PQHeap::bubbleUp() {
    int index = _numFilled - 1;
    int parentIndex = getParentIndex(index);
    while(parentIndex >= 0 && _heap[parentIndex].priority > _heap[index].priority) {
        swap(index, getParentIndex(index));
        index = getParentIndex(index);
        parentIndex = getParentIndex(index);
    }
}

/*
 * This function enqueues by adding the new element to the end of the heap. Then it bubbles up the
 * element to its proper location so that the min heap property is satisfied.
 */
void PQHeap::enqueue(DataPoint elem) {
    ensureCapacity(); // first check that there is enough space to add it.
    _heap[_numFilled] = elem;
    _numFilled++; // update _numFilled value
    bubbleUp();
}

/*
 * This function returns, but does not remove, the element that is frontmost.
 * If the priority queue is empty, this function calls error().
 * Since the element with the highest priority (lowest priority value) is always the
 * 'root' of the heap, it is located in the first filled
 * slot of the array, at index 0. This function returns the element at index 0.
 */
DataPoint PQHeap::peek() const {
    if (isEmpty()) {
        error("Cannot peek empty pqheap!");
    }
    return _heap[0];
}

/* HELPER FUNCTION: 'bubbling down' for dequeue.
 */
void PQHeap::bubbleDown() {
    int index = 0;
    int leftChildIndex = getLeftChildIndex(index);
    int rightChildIndex = getRightChildIndex(index);
    while(leftChildIndex < _numFilled) {
        int smallerChildIndex = leftChildIndex;
        // figure out which child index to compare current index to. If there are 2 children, figure out which child has the smaller priority value.
        if (rightChildIndex < _numFilled && _heap[rightChildIndex].priority < _heap[leftChildIndex].priority) {
            smallerChildIndex = rightChildIndex;
        }

        // now swap if needed.
        if (_heap[smallerChildIndex].priority < _heap[index].priority) {
            swap(index, smallerChildIndex);
            index = smallerChildIndex;
            leftChildIndex = getLeftChildIndex(index);
            rightChildIndex = getRightChildIndex(index);
        }
        else {
            break;
        }
    }
}

/*
 * This function removes and returns the frontmost (lowest priority value = highest priority) element
 * of the heap, also known as the root. The bottom/rightmost element gets pushed to the root position,
 * and gets bubbled down to its proper location so that the min heap property is satisfied.
 */
DataPoint PQHeap::dequeue() {
    if (isEmpty()) {
        error("Cannot dequeue empty pqheap!");
    }
    DataPoint dequeueElt = _heap[0];
    _heap[0] = _heap[_numFilled-1];
    _numFilled--;
    bubbleDown();
    return dequeueElt;
}

/*
 * This function returns whether the priority queue heap is empty or not.
 * Returns true if there are currently 0 elements in the queue, and
 * false otherwise. This utilizes the other member function 'size()'.
 */
bool PQHeap::isEmpty() const {
    return size() == 0;
}

/*
 * This function returns the number of elements in the priority queue heap. It has no parameters.
 * The count of enqueued elements is tracked in the
 * member variable _numFilled.
 *
 */
int PQHeap::size() const {
    return _numFilled;
}

/*
 * This function removes all elements from this pq heap. It takes in nothing and returns nothing.
 * Updates internal state to reflect that the queue is empty, e.g. count
 * of filled slots is reset to zero. The array memory stays allocated
 * at current capacity. The previously stored elements do not need to
 * be cleared; those slots will be overwritten when additional elements
 * are enqueued.
 */
void PQHeap::clear() {
    _numFilled = 0;
}

/*
 * Prints the contents of internal array (the pqheap elts stored in array).
 */
void PQHeap::printDebugInfo() {
    for (int i = 0; i < size(); i++) {
        cout << "[" << i << "] = " << _heap[i] << endl;
    }
}

/*
 * This function traverses the heap array and ensures that the heap property
 * holds for all elements in the array. If elements are found that
 * violate the heap property, an error should be thrown.
 * It confirms if the internal state of member variables appears valid
 * Report an error if problems found.
 */
void PQHeap::validateInternalState() {
    /*
     * If there are more elements than spots in the array, we have a problem.
     */
    if (_numFilled > _numAllocated) error("Too many elements in not enough space!");

    /* Loop over the elements in the array and compare children to their parents. If the child
     * priority value is less than its parent's, throw an error.
     */
    for (int i = 1; i < size(); i++) {
        if (_heap[i].priority < _heap[getParentIndex(i)].priority) {
            error("Array elements out of order at index " + integerToString(i));
        }
    }
}

/*
 * This helper function takes in the current index (int) and returns the index (int) of its parent, using simple math.
 */
int PQHeap::getParentIndex(int curIndex) {
    int parentIndex = (curIndex - 1) / 2;
    return parentIndex;
}

/*
 * This helper function takes in the current index (int) and returns the index (int) of its left child, using simple math.
 */
int PQHeap::getLeftChildIndex(int curIndex) {
    int leftChildIndex = (2*curIndex) + 1;
    return leftChildIndex;
}

/*
 * This helper function takes in the current index (int) and returns the index (int) of its right child, using simple math.
 */
int PQHeap::getRightChildIndex(int curIndex) {
    int rightChildIndex = (2*curIndex) + 2;
    return rightChildIndex;
}

/* * * * * * Test Cases Below This Point * * * * * */

STUDENT_TEST("Testing enqueue and dequeue in simple example") {
    PQHeap pq;
    cout << endl;
    DataPoint A = {"A", 5};
    DataPoint B = {"B", 6};
    DataPoint C = {"C", 4};
    pq.enqueue(A);
    pq.enqueue(B);
    pq.enqueue(C); // C should bubble up to become the root
    pq.printDebugInfo();

    EXPECT_EQUAL(pq.dequeue(), C);
    EXPECT_EQUAL(pq.dequeue(), A);
    EXPECT_EQUAL(pq.dequeue(), B);
    EXPECT(pq.isEmpty());
}

STUDENT_TEST("Testing expand using enqueues") {
    PQHeap pq;
    Vector<DataPoint> input = {
        { "R", 4 }, { "A", 5 }, { "B", 3 }, { "K", 7 }, { "G", 2 },
        { "V", 9 }, { "T", 1 }, { "O", 8 }, { "S", 6 }, {"C", 0},
        {"D", 10} };

    pq.validateInternalState();
    for (auto dp : input) {
        pq.enqueue(dp);
        pq.validateInternalState();
    }
    cout << endl;
    pq.printDebugInfo();
}

STUDENT_TEST("Testing isEmpty, size, peek, and clear") {
    PQHeap pq;
    EXPECT(pq.isEmpty());

    DataPoint A = {"", 1};
    DataPoint B = {"", 0};

    pq.enqueue(A);
    pq.enqueue(B);
    EXPECT_EQUAL(pq.size(), 2);
    EXPECT_EQUAL(pq.peek(), B);
    pq.clear();
    EXPECT(pq.isEmpty());
}

STUDENT_TEST("Testing dequeue with duplicates") {
    PQHeap pq;
    DataPoint A = {"A", 4};
    DataPoint B = {"B", 4};
    DataPoint C = {"C", 4};
    pq.enqueue(A);
    pq.enqueue(B);
    pq.enqueue(C);
    pq.validateInternalState();

    EXPECT_EQUAL(pq.dequeue(), A);
    pq.validateInternalState();
    EXPECT_EQUAL(pq.dequeue(), C);
    pq.validateInternalState();
    EXPECT_EQUAL(pq.dequeue(), B);
    pq.validateInternalState();
}

STUDENT_TEST("PQHeap, ascending and descending sequences") {
    PQHeap pq;
    for (int i = 0; i < 20; i++) {
        pq.enqueue({ "a" + integerToString(i), 2 * i });
    }
    for (int i = 19; i >= 0; i--) {
        pq.enqueue({ "b" + integerToString(i), 2 * i + 1 });
    }

    EXPECT_EQUAL(pq.size(), 40);
    for (int i = 0; i < 40; i++) {
        DataPoint removed = pq.dequeue();
        EXPECT_EQUAL(removed.priority, i);
    }
}

STUDENT_TEST("PQHeap, duplicate elements") {
    PQHeap pq;
    for (int i = 0; i < 20; i++) {
        pq.enqueue({ "a" + integerToString(i), i });
    }
    for (int i = 19; i >= 0; i--) {
        pq.enqueue({ "b" + integerToString(i), i });
    }

    EXPECT_EQUAL(pq.size(), 40);
    for (int i = 0; i < 20; i++) {
        DataPoint one = pq.dequeue();
        DataPoint two = pq.dequeue();

        EXPECT_EQUAL(one.priority, i);
        EXPECT_EQUAL(two.priority, i);
    }
}

STUDENT_TEST("PQHeap, interleave enqueue/dequeue") {
    PQHeap pq;
    int n = 100;
    for (int i = n / 2; i < n; i++) {
        pq.enqueue({"", i});
    }
    EXPECT_EQUAL(pq.size(), 50);
    for (int i = n / 2; i < n; i++) {
        EXPECT_EQUAL(pq.dequeue().priority, i);
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT(pq.isEmpty());

    pq.validateInternalState();

    for (int i = 0; i < n / 2; i++) {
        pq.enqueue({"", i});
    }
    EXPECT_EQUAL(pq.size(), 50);
    for (int i = 0; i < n / 2; i++) {
        EXPECT_EQUAL(pq.dequeue().priority, i);
    }
    EXPECT_EQUAL(pq.size(), 0);
}

STUDENT_TEST("PQHeap stress test, cycle 10000 random elements in and out") {
    PQHeap pq;
    int n = 10000;
    DataPoint max = {"max", 106106106};
    DataPoint min = {"min", -106106106};

    pq.enqueue(min);
    pq.enqueue(max);
    for (int i = 0; i < n; i++) {
        int randomPriority = randomInteger(-10000, 10000);
        pq.enqueue({ "", randomPriority });
    }
    EXPECT_EQUAL(pq.size(), n + 2);
    pq.validateInternalState();

    EXPECT_EQUAL(pq.dequeue(), min);
    for (int i = 0; i < n; i++) {
        pq.dequeue();
    }
    EXPECT_EQUAL(pq.dequeue(), max);
}

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("PQHeap example from writeup, validate each step") {
    PQHeap pq;
    Vector<DataPoint> input = {
        { "R", 4 }, { "A", 5 }, { "B", 3 }, { "K", 7 }, { "G", 2 },
        { "V", 9 }, { "T", 1 }, { "O", 8 }, { "S", 6 } };

    pq.validateInternalState();
    for (auto dp : input) {
        pq.enqueue(dp);
        pq.validateInternalState();
    }
    while (!pq.isEmpty()) {
        pq.dequeue();
        pq.validateInternalState();
    }
}

//static void fillQueue(PQHeap& pq, int n) {
//    pq.clear(); // start with empty queue
//    for (int i = 0; i < n; i++) {
//        pq.enqueue({ "", i });
//    }
//}

//static void emptyQueue(PQHeap& pq, int n) {
//    for (int i = 0; i < n; i++) {
//        pq.dequeue();
//    }
//}

//PROVIDED_TEST("PQHeap timing test, fillQueue and emptyQueue") {
//    PQHeap pq;

//    TIME_OPERATION(40000, fillQueue(pq, 40000));
//    TIME_OPERATION(40000, emptyQueue(pq, 40000));
//}

