/* Name: Alyssa Choi
 * Section leader: Ayelet Drazen
 * Assignment brief: make a recursive quicksort algorithm for linked lists
 */

#include "listnode.h"
#include "vector.h"
#include "testing/SimpleTest.h"
#include "random.h"
using namespace std;

// prototypes
int lengthOf(ListNode* list);
void partition(ListNode*& front, ListNode*& less, ListNode*& equal, ListNode*& greater, int pivot);
void concatenate(ListNode*& front, ListNode*& less, ListNode*& equal, ListNode*& greater);

/*
 * This is a quickSort recursive algorithm that takes in the head of a linked list and sorts it in ascending numerical order.
 * It implements partition and concatenate helpers.
 * The function does not return anything.
 */
void quickSort(ListNode*& front) {
    if (lengthOf(front) <= 1) return;

    ListNode* less = nullptr;
    ListNode* equal = nullptr;
    ListNode* greater = nullptr;
    int pivot = front->data;

    partition(front, less, equal, greater, pivot);

    quickSort(less);
    quickSort(greater);

    front = nullptr;

    concatenate(front, less, equal, greater);
}

/*
 * This function is the 'partition' helper function that takes in the front of a linked list,
 * and 3 empty linked lists representing the elts less than, equal to, and greater than the given pivot.
 * It does not return anything.
 */
void partition(ListNode*& front, ListNode*& less, ListNode*& equal, ListNode*& greater, int pivot) {
    ListNode* curr = front;
    while (curr != nullptr) {
        if (curr->data < pivot) {
            ListNode* temp = less;
            ListNode* next = curr->next;
            less = curr;
            less->next = temp;
            curr = next;
        }
        else if (curr->data == pivot) {
            ListNode* temp = equal;
            ListNode* next = curr->next;
            equal = curr;
            equal->next = temp;
            curr = next;
        }

        else {
            ListNode* temp = greater;
            ListNode* next = curr->next;
            greater = curr;
            greater->next = temp;
            curr = next;
        }
    } //end of for loop
}

/*
 * This function helps the concatenate function to avoid repetitive code.
 */
void concatenateHelper(ListNode*& front, ListNode*& subList, ListNode*& curr) {
    if (lengthOf(front) == 0) {
        front = subList;
    }
    else {
        while (curr->next != nullptr) {
        curr = curr->next;
        }
        curr->next = subList;
    }
}

/*
 * This function is the 'concatenate' helper that takes in the front of a linked list, and 3 linked lists
 * representing elements less than, equal to, and greater than the pivot declared in 'quickSort'.
 * The function joins together the 3 linked lists by putting them all back in the 'front' linked list, in ascending numerical order.
 * The function does not return anything.
 */
void concatenate(ListNode*& front, ListNode*& less, ListNode*& equal, ListNode*& greater) {
    // add all elts of less to final linked list
    if (lengthOf(less) > 0) {
        front = less;
    }
    // add all elts of equal
    ListNode* curr = front;
    concatenateHelper(front, equal, curr);

    // add all elts of greater
    curr = front;
    concatenateHelper(front, greater, curr);
}


/* * * * * * Test Code Below This Point * * * * * */

/*
 * We have provided this utility function as a convenience. This
 * function traverses the linked list represented by the provided
 * front pointer and prints out the data stored in the list along
 * the way in a convenient format. You may find it to be helpful to
 * use this function as a debugging aid.
 */
void printList(ListNode* front) {
   cout << "{";
   while (front != nullptr) {
       cout << front->data;
       if (front->next != nullptr){
           cout << ", ";
       }
       front = front->next;
   }
   cout << "}" << endl;
}

/*
 * This utility function deallocates the memory for all the nodes in a
 * given linked list. It can be used to recycle the memory allocated
 * during a test case and avoid memory leaks.
 *
 * Citation: Lecture 21 code, linkedlist.cpp, `freeList` function
 */
void deallocateList(ListNode* front) {
    while (front != nullptr) {
        // store next ListNode in a temporary variable
        ListNode* next = front->next;
        delete front;
        front = next;
    }
}

/*
 * This utility function is given a vector of values and constructs an equivalent
 * linked list containing the same values in the same order. It can be used to construct
 * a linked list to use as an input to a function being tested.
 */
ListNode* createList(Vector<int> values){
    if (values.isEmpty()) {
        return nullptr;
    }
    ListNode* front = new ListNode;
    ListNode* curr = front;
    for (int i = 0; i < values.size(); i++) {
        if (i == values.size()-1) {
            curr->data = values[i];
            curr->next = nullptr;
            break;
        }
        curr->data = values[i];
        ListNode* next = new ListNode;
        curr->next = next;
        curr = next;
    }
    return front;
}

/* HELPER FUNCTION: Given a linked list, returns the length of that list.
 * Citation: Lecture 21 code, linkedlist.cpp, `freeList` function
*/
int lengthOf(ListNode* list) {
    int result = 0;
    while (list != nullptr) {
        result++;
        list = list->next;
    }
    return result;
}

/*
 * This utility function compares a linked list and a vector for
 * equivalence and returns true if both contain the same values in the
 * same order. It can be used to confirm a linked list's contents match
 * the expected.
 */
bool areEquivalent(ListNode* front, Vector<int> v){
    if (lengthOf(front) != v.size()) {
        return false;
    }
    ListNode* curr = front;
    for (int i = 0; i < v.size(); i++) {
        if (curr->data != v[i]) {
            return false;
        }
        curr = curr->next;
    }
    return true;
}

/* * * TESTING * * */

STUDENT_TEST("Test partition helper and concatenate helper, pt. 1") {
    Vector<int> values = {2, 3, 1, 5};
    ListNode* front = createList(values);
    ListNode* less = nullptr;
    ListNode* equal = nullptr;
    ListNode* greater = nullptr;
    int pivot = front->data;

    partition(front, less, equal, greater, pivot);
    cout << endl;
    printList(less);
    printList(equal);
    printList(greater);

    front = nullptr;
    concatenate(front, less, equal, greater);
    printList(front);

    /* Avoid memory leaks by deallocating lists */
    deallocateList(front);
}

STUDENT_TEST("Test partition helper and concatenate helper, pt. 2") {
    Vector<int> values = {4, 5, -1, 6, 7, 2, 3, 4, -10, 11};
    ListNode* front = createList(values);
    ListNode* less = nullptr;
    ListNode* equal = nullptr;
    ListNode* greater = nullptr;
    int pivot = front->data;

    partition(front, less, equal, greater, pivot);
    cout << endl;
    printList(less);
    printList(equal);
    printList(greater);

    front = nullptr;
    concatenate(front, less, equal, greater);
    printList(front);

    /* Avoid memory leaks by deallocating lists */
    deallocateList(front);
}

STUDENT_TEST("This is a skeleton to demonstrate an end-to-end student test."){
    /* Initialize vector to sequence of values */
    Vector<int> values = {1, 2, 3, 4};
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("My own end-to-end tests."){
    // sort empty list
    Vector<int> values = {};
    ListNode* list = createList(values);
    quickSort(list);
    values.sort();
    EXPECT(areEquivalent(list, values));
    deallocateList(list);

    // sort single-element list
    Vector<int> values1 = {1};
    ListNode* list1 = createList(values1);
    quickSort(list1);
    values1.sort();
    EXPECT(areEquivalent(list1, values1));
    deallocateList(list1);

    // sort reverse-ordered list
    Vector<int> values2 = {3, 2, 1};
    ListNode* list2 = createList(values2);
    quickSort(list2);
    values2.sort();
    EXPECT(areEquivalent(list2, values2));
    deallocateList(list2);

    // sort list w/ some duplicates
    Vector<int> values3 = {3, 3, 3, 4};
    ListNode* list3 = createList(values3);
    quickSort(list3);
    values3.sort();
    EXPECT(areEquivalent(list3, values3));
    deallocateList(list3);

    // sort list w/ positive and negative numbers
    Vector<int> values4 = {0, -1, 5, 6, -9};
    ListNode* list4 = createList(values4);
    quickSort(list4);
    values4.sort();
    EXPECT(areEquivalent(list4, values4));
    deallocateList(list4);

    // sort long list of random integers, positive and negative integers
    Vector<int> values5;
    int length = 100;
    for (int i = 0; i < length; i++) {
        values5.add(randomInteger(-100, 100));
    }
    ListNode* list5 = createList(values5);
    quickSort(list5);
    values5.sort();
    EXPECT(areEquivalent(list5, values5));
    deallocateList(list5);
}

PROVIDED_TEST("This tests some of the testing helper functions you are recommended to implement."){
    /* Creates a small test list containing the values 1->2->3. */
    ListNode* testList = nullptr;
    testList = new ListNode(3, testList);
    testList = new ListNode(2, testList);
    testList = new ListNode(1, testList);

    cout << endl;
    printList(testList);

    /* Test equality comparison function. */
    EXPECT(areEquivalent(testList, {1, 2, 3}));
    EXPECT(!areEquivalent(testList, {1, 2}));
    EXPECT(!areEquivalent(testList, {1, 2, 3, 4}));
    EXPECT(!areEquivalent(testList, {1, 3, 2}));

    /* Test create list function. */
    ListNode* studentList = createList({1, 2, 3, 4});
    printList(studentList);
    ListNode* cur = studentList;
    for (int i = 1; i <= 4; i++){
        EXPECT(cur != nullptr);
        EXPECT_EQUAL(cur->data, i);
        cur = cur->next;
    }

    /* Test deallocate list functions. There should be no memory leaks from this test. */
    deallocateList(studentList);
    deallocateList(testList);
}

STUDENT_TEST("Time trials on quickSort") {
    // average scenario: large list of random numbers, size of 5k
    ListNode* list = nullptr;
    for (int i = 0; i < 5000; i++) {
        list = new ListNode(randomInteger(-100, 100), list);
    }
    TIME_OPERATION(5000, quickSort(list));
    deallocateList(list);

    // average scenario: large list of random numbers, size of 8k
    ListNode* list1 = nullptr;
    for (int i = 0; i < 8000; i++) {
        list1 = new ListNode(randomInteger(-100, 100), list1);
    }
    TIME_OPERATION(8000, quickSort(list1));
    deallocateList(list1);

    // worst-case scenario: large list of reverse sorted order, size of 8k
    ListNode* list2 = nullptr;
    for (int i = 0; i < 8000; i++) {
        list2 = new ListNode(i, list2);
    }
    TIME_OPERATION(8000, quickSort(list2));
    deallocateList(list2);

    // worst-case scenario: large list of reverse sorted order, size of 5k
    ListNode* list3 = nullptr;
    for (int i = 0; i < 5000; i++) {
        list3 = new ListNode(i, list3);
    }
    TIME_OPERATION(5000, quickSort(list3));
    deallocateList(list3);
}

/*
 * The provided test case demonstrates a comparsion between the same
 * algorithm (quicksort) operating on a linked list versus a vector.
 */
PROVIDED_TEST("Time linked list quicksort vs vector quicksort") {
    int startSize = 50000;

    for(int n = startSize; n < 10*startSize; n *= 2) {
        Vector<int> v(n);
        ListNode* list = nullptr;

        /* Create linked list and vector with the same random sequence. */
        for (int i = n-1; i >= 0; i--) {
            v[i] = randomInteger(-10000, 10000);
            list = new ListNode(v[i], list);
        }

        /* NOTE: This test does not check correctness of the linked list sort
         * function. It only times the two operations to compare relative speed. */
        TIME_OPERATION(n, quickSort(list));
        TIME_OPERATION(n, v.sort());        /* Standard vector sort operation is backed
                                               with quicksort algorithm. */

        deallocateList(list);
    }
}

