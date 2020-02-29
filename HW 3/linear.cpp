//
//  linear.cpp
//  CS32 Homework 3 Zachary Chang
//
//  Created by Zachary Chang on 2/6/20.
//  Copyright © 2020 Zachary Chang. All rights reserved.
//

//#include <iostream> //DELETE THESE INCLUDES WHEN DONE
//#include <cassert>
//using namespace std;
//
//bool somePredicate(double x) //DELETE THIS FUNCTION WHEN DONE
//{
//    return x > 5;
//}

// Return true if the somePredicate function returns false  at
// least one of the array elements; return false otherwise.
bool anyFalse(const double a[], int n)
{
    if (n <= 0)
    {
        return false;
    }

    if (somePredicate(a[n-1]))
    {
        return anyFalse(a, n - 1);
    }
    return true;
}

// Return the number of elements in the array  which the
// somePredicate function returns true.
int countTrue(const double a[], int n)
{
    if (n <= 0)
    {
        return 0;
    }

    if (somePredicate(a[n-1]))
    {
        return 1 + countTrue(a, n - 1);
    }
    return countTrue(a, n - 1);
}

// Return the subscript of the first element in the array  which
// the somePredicate function returns true.  If there is no such
// element, return -1.
int firstTrue(const double a[], int n)
{
    if (n <= 0)
    {
        return -1;
    }

    if (somePredicate(a[0]))
    {
        return 0;
    }

    int first = firstTrue(a + 1, n - 1);

    if (first == -1)
    {
        return -1;
    }
    return 1 + first;
}

// Return the subscript of the smallest element in the array (i.e.,
// return the smallest subscript m such that a[m] <= a[k]  all
// k from 0 to n-1).  If the function is told to examine no
// elements, return -1.


int positionOfSmallest(const double a[], int n)
{
    if (n <= 0)
    {
        return -1;
    }

    if (n == 1)
    {
        return 0;
    }

    int smallest = positionOfSmallest(a, n - 1);
    if (a[smallest] < a[n - 1])
    {
        return smallest;
    }

    return n - 1;
}

// If all n2 elements of a2 appear in the n1 element array a1, in
// the same order (though not necessarily consecutively), then
// return true; otherwise (i.e., if the array a1 does not contain
// a2 as a not-necessarily-contiguous subsequence), return false.
// (Of course, if a2 is empty (i.e., n2 is 0), return true.)
// example, if a1 is the 7 element array
//    10 50 40 20 50 40 30
// then the function should return true if a2 is
//    50 20 30
// or
//    50 40 40
// and it should return false if a2 is
//    50 30 20
// or
//    10 20 20
bool contains(const double a1[], int n1, const double a2[], int n2)
{
    if (n2 <= 0)
    {
        return true;
    }

    if (n2 > n1 || n1 <= 0)
    {
        return false;
    }

    if (a2[n2 - 1] == a1[n1 - 1])
    {
        return contains(a1, n1-1, a2, n2 - 1);
    }
    else //added this else statement see if it works
    {
        return contains(a1, n1 - 1, a2, n2);
    }
}

/*
int main() //MAKE SURE TO DELETE THIS MAIN FUNCTION
{
    //PERFORMED WITH PREDICATE: return x > 5
    double arr[] = {8, 5, 5, 10, 8};
    double arr1[] = {5, 9, 20, 14};
    double arr2[] = {10, 11, 19, 20, 15}; //test number of operations
    double arr3[] = {4, 5, 3, 8, 9, 2};
    double arr4[] = {4, 3, 1, 2, 6};
    double arr5[] = {2, -10, 5, 3, 4};

    assert(anyFalse(arr, 5));
    assert(anyFalse(arr1, 4));
    assert(!anyFalse(arr, 1));
    assert(!anyFalse(arr2, 5));
    assert(!anyFalse(arr, 0));
    assert(!anyFalse(arr, -3));

    assert(countTrue(arr, 5) == 3);
    assert(countTrue(arr, 4) == 2);
    assert(countTrue(arr, 0) == 0);
    assert(countTrue(arr, -5) == 0);

    assert(firstTrue(arr3, 6) == 3);
    assert(firstTrue(arr4, 5) == 4);
    assert(firstTrue(arr3, -3) == -1);
    assert(firstTrue(arr5, 5) == -1);
    assert(firstTrue(arr4, 4) == -1);

    assert(positionOfSmallest(arr, 2) == 1);
    assert(positionOfSmallest(arr1, 4) == 0);
    assert(positionOfSmallest(arr1, 2) == 0);
    assert(positionOfSmallest(arr3, 6) == 5);
    assert(positionOfSmallest(arr3, 5) == 2);
    assert(positionOfSmallest(arr3, 2) == 0);
    assert(positionOfSmallest(arr5, 2) == 1);
    assert(positionOfSmallest(arr5, 1) == 0);
    assert(positionOfSmallest(arr5, 0) == -1);
    assert(positionOfSmallest(arr5, -10) == -1);

    double arr6[] = {3, 8, 10, 9, 4, 5}; //test duplicate lists
    double arr7[] = {8, 9, 4};
    double arr8[] = {3, 4, 1};
    double arr9[] = {3, 4, 1};

    assert(contains(arr6, 6, arr7, 3));
    assert(contains(arr6, 5, arr7, 3));
    assert(contains(arr6, 6, arr7, 2));
    assert(!contains(arr6, 3, arr7, 2));
    assert(contains(arr6, 4, arr7, 0));
    assert(contains(arr6, 0, arr7, 0));
    assert(contains(arr8, 3, arr9, 3));
    assert(!contains(arr7, 3, arr6, 6));

    cout << "PASSED ALL TESTS" << endl;
}
*/
