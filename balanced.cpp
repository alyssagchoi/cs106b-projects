/*
 * Name: Alyssa Choi
 * Section leader: Ayelet Drazen
 * Assignment brief: use recursion to detect if a string containing brackets is balanced or not.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "testing/SimpleTest.h"
#include "strlib.h"

using namespace std;

/*
 * Helper function: returns a string consisting of only the bracketing characters from the input string.
 * The function operates recursively to process the string.
 */
string operatorsFrom(string str) {
    Vector<char> brackets = {'(', ')', '{', '(', '[', ']', ')', '(', '(', ')', ')', '}'};

    // Base case: empty string
    if (str.empty()) {
        return "";
    }

    // Recursive case: first check that the current char is a bracket. If not, recursively call operatorsFrom
    // on the string starting from the next char
    else {
        char curChar = str[0];
        if (brackets.contains(curChar)) {
            return curChar + operatorsFrom(str.substr(1));
        }
        return operatorsFrom(str.substr(1));
    }
}

/*
 * Returns a boolean saying whether or not the operators are matched. The input is a string of bracket operators.
 * The function has 3 main cases: if there is an uneven length of the input, return false; if the string is empty,
 * return true; if the string is NOT empty then recursively check that it is balanced.
 */
bool operatorsAreMatched(string ops) {

    // check that all char in ops are brackets
    Vector<char> brackets = {'(', ')', '{', '(', '[', ']', ')', '(', '(', ')', ')', '}'};
    for (int i = 0; i < ops.length(); i++) {
        if (!brackets.contains(ops[i])) {
            return false;
        }
    }


    // if there is an uneven number of chars, then return false immediately
    if (ops.length() % 2 != 0) {
        return false;
    }

    // base case: empty string
    if (ops.empty()) {
        return true;
    }

    // recursive case: check that ops contains "{}" "[]" or "()", then check the rest of the string for this
    else {
        if (stringContains(ops, "{}")) {
            int index = stringIndexOf(ops, "{}");
            ops.erase(index, 2);
            return operatorsAreMatched(ops);
        }
        else if (stringContains(ops, "[]")) {
            int index = stringIndexOf(ops, "[]");
            ops.erase(index, 2);
            return operatorsAreMatched(ops);
        }
        else if (stringContains(ops, "()")) {
            int index = stringIndexOf(ops, "()");
            ops.erase(index, 2);
            return operatorsAreMatched(ops);
        }
        return false;

    }
}

/*
 * The isBalanced function assumes correct implementation of
 * the above two functions operatorsFrom and operatorsMatch.
 * It uses operatorsFrom to extract the operator characters
 * from the input string and then confirms that those
 * operators are balanced by using operatorsMatch.
 * You should not modify the provided code in the isBalanced
 * function.  If the previous two functions have been implemented
 * correctly, the provided isBalanced will correctly report whether
 * the input string has balanced bracketing operators.
 */
bool isBalanced(string str) {
    string ops = operatorsFrom(str);
    return operatorsAreMatched(ops);
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom("vec[3]"), "[]");
}

PROVIDED_TEST("operatorsAreMatched on simple example") {
    EXPECT(operatorsAreMatched("{}"));
}

PROVIDED_TEST("isBalanced on example from writeup") {
    string example ="int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }";
    EXPECT(isBalanced(example));
}

PROVIDED_TEST("isBalanced on non-balanced examples from writeup") {
    EXPECT(!isBalanced("( ( [ a ] )"));
    EXPECT(!isBalanced("3 ) ("));
    EXPECT(!isBalanced("{ ( x } y )"));
}


// STUDENT TESTS

STUDENT_TEST("operatorsFrom on example with space at the end") {
    EXPECT_EQUAL(operatorsFrom("Hello!{Alyssa} "), "{}");
}

STUDENT_TEST("operatorsFrom on example with no brackets") {
    EXPECT_EQUAL(operatorsFrom("Hello, Alyssa "), "");
}

STUDENT_TEST("operatorsFrom on empty string / only a space char") {
    EXPECT_EQUAL(operatorsFrom(""), "");
    EXPECT_EQUAL(operatorsFrom(" "), "");
}

STUDENT_TEST("operatorsAreMatched on empty string") {
    EXPECT_EQUAL(operatorsAreMatched(""), true);
}

STUDENT_TEST("operatorsAreMatched on unbalanced string") {
    EXPECT_EQUAL(operatorsAreMatched("[(])"), false);
}

STUDENT_TEST("operatorsAreMatched on uneven length string") {
    EXPECT_EQUAL(operatorsAreMatched("[]()}"), false);
}

STUDENT_TEST("operatorsAreMatched on string with non-bracket chars") {
    EXPECT_EQUAL(operatorsAreMatched("[hi]"), false);
}
