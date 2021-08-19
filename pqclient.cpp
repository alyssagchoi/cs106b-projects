/* Name: Alyssa Choi
 * Section leader: Ayelet Drazen
 * Assignment brief: define method(s) that deal with retuning data from a priority queue.
 */
#include "pqclient.h"
#include "pqsortedarray.h"
#include "pqheap.h"
#include "vector.h"
#include "strlib.h"
#include <sstream>
#include "testing/SimpleTest.h"
using namespace std;

/* Using the Priority Queue data structure as a tool to sort, neat! */
void pqSort(Vector<DataPoint>& v) {
    PQHeap pq;

    /* Add all the elements to the priority queue. */
    for (int i = 0; i < v.size(); i++) {
        pq.enqueue(v[i]);
    }

    /* Extract all the elements from the priority queue. Due
     * to the priority queue property, we know that we will get
     * these elements in sorted order, in order of increasing priority
     * value. Store elements back into vector, now in sorted order.
     */
    for (int i = 0; i < v.size(); i++) {
        v[i] = pq.dequeue();
    }
}

/* This function takes in a stream of DataPoints and an int k. The function returns a Vector<DataPoint>
 * which contains the top k DataPoints with the highest priority values. From left to right, the Vector
 * contains the elements in descending priority value order.
 * The function uses a priority queue as an intermediary data structure to hold the elts that will be
 * presented in the final Vector result.
 * The function uses a while loop to look at each DataPoint in the stream, and decides if it should be
 * enqueued to the priority queue or not. Lastly, the function translates the priority queue to a Vector.
 */
Vector<DataPoint> topK(istream& stream, int k) {
    PQHeap pq;
    DataPoint point;

    while (stream >> point) {
        // while the vector is not filled up with 5 elts, add the current DP to it.
        if (pq.size() < k) {
            pq.enqueue(point);
        }
        // otherwise: check that it's qualified to be added to the pq. If so, replace it with the elt in the vector that had the lowest priority value:
        else {
            // edge case: first check that the pq size is not 0 (which would mean there are 0 elts in stream)
            if (pq.size() != 0) {
                int lowestPriorityVal = pq.peek().priority;
                if (point.priority > lowestPriorityVal) {
                    pq.dequeue();
                    pq.enqueue(point);
                }
            }
        }
    }

    // edge case: first check that the pq size is not 0 (which would mean there were 0 elts in stream)
    if (pq.size() == 0) {
        return {};
    }

    // otherwise:
    int size = pq.size();
    Vector<DataPoint> result(size);
    for (int i = 0; i < size; i++) {
        int index = (size-1) - i;
        result[index] = pq.dequeue();
    }

    return result;
}



/* * * * * * Test Cases Below This Point * * * * * */

/* Helper function that, given a list of data points, produces a stream from them. */
stringstream asStream(const Vector<DataPoint>& dataPoints) {
    stringstream result;
    for (const DataPoint& pt: dataPoints) {
        result << pt;
    }
    return result;
}

/* Helper function that, given range start and stop, produces a stream of values. */
stringstream asStream(int start, int stop) {
    stringstream result;
    for (int i = start; i <= stop; i++) {
        DataPoint pt = { "", i };
        result << " " << pt << " ";
    }
    return result;
}

/* STUDENT TESTS */

STUDENT_TEST("test where the stream size is less than  k") {
    stringstream stream = asStream({ { "A", 1 }, { "B", 2 }, { "C", 3 }, { "D", 4 } });
    Vector<DataPoint> expected = { { "D", 4 }, { "C", 3 }, { "B", 2 }, { "A", 1 } };
    Vector<DataPoint> result = topK(stream, 5);
    EXPECT_EQUAL(result, expected);
}

STUDENT_TEST("test where all elts in stream have same priority") {
    stringstream stream = asStream({ { "A", 1 }, { "B", 1 }, { "C", 1 }, { "D", 1 } });
    Vector<DataPoint> expected = { { "B", 1 }, { "A", 1 } };
    Vector<DataPoint> result = topK(stream, 2);
    EXPECT_EQUAL(result, expected);
}

STUDENT_TEST("PQSortedArray topK: time trial, holding n constant and varying k") {
    // do these tests when using a PQSortedArray implementation
    int n = 1600000;
    int kStartSize = 50;
    for (int k = kStartSize; k < 10*kStartSize; k *= 2) {
        Vector<DataPoint> input;
        for (int i = 0; i < n; i++) {
            input.add({ "", randomInteger(1, n) });
        }
        stringstream stream = asStream(input);
        TIME_OPERATION(k, topK(stream, k));
     }
}

STUDENT_TEST("PQHeap pqSort time trial") {
    // do these tests when using a PQHeap implementation
    // the input size is just bigger than the PQSortedArray pqSort time trials
    int startSize = 8000;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> v;
        for (int i = 0; i < n; i++) {
            v.add({ "", randomInteger(1, n) });
        }
        TIME_OPERATION(n, pqSort(v));
    }
}

STUDENT_TEST("PQHeap topK: time trial, holding n constant and varying k") {
    // do these tests when using a PQHeap implementation
    // the n/k sizes are just bigger for these tests
    int n = 3200000;
    int kStartSize = 100;
    for (int k = kStartSize; k < 10*kStartSize; k *= 2) {
        Vector<DataPoint> input;
        for (int i = 0; i < n; i++) {
            input.add({ "", randomInteger(1, n) });
        }
        stringstream stream = asStream(input);
        TIME_OPERATION(k, topK(stream, k));
     }
}

STUDENT_TEST("PQHeap topK: time trial, holding k constant and varying n") {
    // do these tests when using a PQHeap implementation
    // the n/k sizes are bigger than for the PQSortedArray topK tests
    int startSize = 1600000;
    int k = 50;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> input;
        for (int i = 0; i < n; i++) {
            input.add({ "", randomInteger(1, n) });
        }
        stringstream stream = asStream(input);
        TIME_OPERATION(n, topK(stream, k));
    }
}


/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("pqSort 100 random elements") {
    Vector<DataPoint> input;
    Vector<int> expectedOutput;
    for (int i = 0; i < 100; i++) {
        int randomValue = randomInteger(1, 10);
        input.add({ "", randomValue});
        expectedOutput.add(randomValue);
    }
    expectedOutput.sort();
    pqSort(input);
    for (int i = 0; i < 100; i++) {
        EXPECT_EQUAL(input[i].priority, expectedOutput[i]);
    }
}

PROVIDED_TEST("PQSortedArray pqSort time trial") {
    int startSize = 1000;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> v;
        for (int i = 0; i < n; i++) {
            v.add({ "", randomInteger(1, n) });
        }
        TIME_OPERATION(n, pqSort(v));
    }
}


/* Constant used for sizing the tests below this point. */
const int kMany = 100000;

PROVIDED_TEST("topK: stream 0 elements, ask for top 1") {
    stringstream stream = asStream({});
    Vector<DataPoint> expected = {};
    EXPECT_EQUAL(topK(stream, 1), expected);
}

PROVIDED_TEST("topK: stream 1 element, ask for top 1") {
    stringstream stream = asStream({ { "" , 1 } });
    Vector<DataPoint> expected = { { "" , 1 } };
    EXPECT_EQUAL(topK(stream, 1), expected);
}

PROVIDED_TEST("topK: small hand-constructed input") {
    Vector<DataPoint> input = { { "A", 1 }, { "B", 2 }, { "C", 3 }, { "D", 4 } };
    stringstream stream = asStream(input);
    Vector<DataPoint> expected = { { "D", 4 }, { "C", 3 } };
    EXPECT(topK(stream, 2) == expected);

    stream = asStream(input);
    expected = { { "D", 4 }, { "C", 3 }, { "B", 2 } };
    EXPECT(topK(stream, 3) == expected);

    stream = asStream(input);
    expected = { { "D", 4 }, { "C", 3 }, { "B", 2 }, { "A", 1 } };
    EXPECT(topK(stream, 4) == expected);
}

PROVIDED_TEST("topK: stream many elements, ask for top 1") {
    Vector<DataPoint> vec;
    for (int i = 0; i < kMany; i++) vec.add({ "", i });
    stringstream stream = asStream(vec);
    Vector<DataPoint> expected = { { "" , kMany - 1 } };
    EXPECT_EQUAL(topK(stream, 1), expected);
}

PROVIDED_TEST("topK: stream many elements, ask for top 5") {
    Vector<DataPoint> vec;
    for (int i = 0; i < kMany; i++) vec.add({ "", i });
    stringstream stream = asStream(vec);
    Vector<DataPoint> expected = {
        { "" , kMany - 1 },
        { "" , kMany - 2 },
        { "" , kMany - 3 },
        { "" , kMany - 4 },
        { "" , kMany - 5 }
    };
    EXPECT_EQUAL(topK(stream, 5), expected);
}

PROVIDED_TEST("topK: stress test, many elements, random values") {
    Vector<int> sorted;
    Vector<DataPoint> points;
    for (int i = 0; i < 10000; i++) {
        int weight = randomInteger(0, 100000);
        sorted.add(weight);
        points.add({ "" , weight});
    }

    stringstream stream = asStream(points);
    sort(sorted.begin(), sorted.end(), greater<int>());
    Vector<DataPoint> result = topK(stream, 10);

    EXPECT_EQUAL(result.size(), 10);
    for (int i = 0; i < 10; i++) {
        EXPECT_EQUAL(result[i].priority, sorted[i]);
    }
}

PROVIDED_TEST("topK: stress test - many elements, ask for top half") {
    stringstream stream = asStream(1, kMany);
    Vector<DataPoint> result = topK(stream, kMany/2);
    EXPECT_EQUAL(result.size(), kMany/2);
    EXPECT_EQUAL(result[0].priority, kMany);
    EXPECT_EQUAL(result[result.size()-1].priority, kMany - result.size() + 1);
}

PROVIDED_TEST("PQSortedArray topK: time trial, holding k constant and varying n") {
    int startSize = 200000;
    int k = 10;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> input;
        for (int i = 0; i < n; i++) {
            input.add({ "", randomInteger(1, n) });
        }
        stringstream stream = asStream(input);
        TIME_OPERATION(n, topK(stream, k));
    }
}

