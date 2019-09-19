//
//  main.cpp
//  Algorithms
//
//  Created by Aron Vischjager on 7/19/19.
//  Copyright © 2019 Aron Vischjager. All rights reserved.
//

#include <iostream>
#include <math.h>
#include <vector>
using namespace std;
// Printing --------------------------------------------------------------------------------------------------
void print(int * values, int n)
{
    for(int i=0; i<n; i++)
    {
        cout << values[i] << endl;
    }
    cout << "End Print" << endl;
}
// Printing end ----------------------------------------------------------------------------------------------
// Swap ------------------------------------------------------------------------------------------------------
void Swap(int &a, int &b)
{
    int temp = b;
    b = a;
    a = temp;
}
// Swap End --------------------------------------------------------------------------------------------------
// Sorting ---------------------------------------------------------------------------------------------------
// Bubble Sort
// Loop Invariant: P(i) = All items at index n-i or greater are sorted and items before the index are at most equivalent to the item at n-i
// Runtime: Always n^2
void bubbleSort(int * values, int n)
{
    for(int i=0; i<n-1; i++)
    {
        for(int j=0; j<n-1-i; j++)
        {
            if(values[j] > values[j+1])
                Swap(values[j], values[j+1]);
        }
    }
}

// Insertion Sort
// Loop Invariant: P(i) = All items at index i-2 or less are sorted relative to each other, but not relative to the entire list.
// Runtime: (n,n^2)
void insertionSort(int * values, int n)
{
    for(int i=1; i<n; i++)
    {
        for(int j=i; j>0; j--)
        {
            if(values[j] < values[j-1])
                 Swap(values[j], values[j-1]);
            else
                break;
        }
    }
}
// Selection Sort
// Loop Invariant: All items at index i-1 or less are sorted and all items after that index have a value that is at least as a large as the value at index i-1.
// Runtime: Always n^2
void selectionSort(int * values, int n) // Unstable
{
    int minIndex;
    for(int i=0; i<n-1; i++)
    {
        minIndex = i;
        for(int j=i+1; j<n; j++)
        {
            if(values[j] < values[minIndex])
                minIndex = j;
        }
        if(minIndex != i)
            // Unstable Sorting
            swap(values[minIndex], values[i]);
    }
}
// Quick Sort
// Loop Invariant:
// Runtime: (n log (n), n^2)
int partition(int * values, int l, int r)
{
    int index = (r+l)/2;
    int newIndex = index;
    while(l < r)
    {
        if((values[l] > values[index]) && (values[r] < values[index]))
        {
            // Unstable Sorting
            swap(values[l], values[r]);
            newIndex = l;
            l++;
            r--;
        }
        else
        {
            if(values[l] <= values[index])
                l++;
            if(values[r] >= values[index])
                r--;
        }
    }
    if(newIndex + 1 <= r)
        newIndex++;
    swap(values[newIndex], values[index]);
    return newIndex;
}

void quickSort(int * values, int l, int r)
{
    if(l < r)
    {
        int split = partition(values, l, r);
        quickSort(values, l, split-1);
        quickSort(values, split+1, r);
    }
}

// Merge Sort
// Loop Invariant: Merge sort recursive combines and sorts two sorted subarrays together into a larger sorted subarray until the subarray is equivalent to a sorted version of the original array. After merge is called on a subarray, all items at index i-1 or less are sorted within a temporary array that will become the new subarray at the end of the function.
// Runtime: Always n log (n)
void merge(int * values, int left, int right)
{
    int size = right-left + 1;
    int temp [size];
    int m = (left+right)/2;
    int index1 = left;
    int index2 = m + 1;
    // Iterates through the inner left and right sub arrays
    for(int i = 0; i<size; i++)
    {
        if(index2 > right)
        {
            temp[i] = values[index1];
            index1++;
        }
        else if(index1 > m)
        {
            temp[i] = values[index2];
            index2++;
        }
        // Stable Sorting
        else if(values[index1] <= values[index2])
        {
            temp[i] = values[index1];
            index1++;
        }
        else
        {
            temp[i] = values[index2];
            index2++;
        }
    }
    // Copies all the values
    for(int i=0; i<size; i++)
        values[left+i] = temp[i];
}

void mergeSort(int * values, int left, int right)
{
    if(left < right)
    {
        int m = (left + right)/2;
        mergeSort(values, left, m);
        mergeSort(values, m+1, right);
        merge(values, left, right);
    }
}

// Radix Sort ________________________________________________________________________________________________
void radixSort(int * values, int n)
{
    int maxValue = values[0];
    for(int i=1; i<n; i++)
    {
        if(values[i] > maxValue)
            maxValue = values[i];
    }
    int maxFactor = log10(maxValue);
    // Initilize vector of size 10!!!
    vector<vector<int>> temp (10);
    for(int fac = 0; fac<=maxFactor; fac++)
    {
        int currentFactor = pow(10, fac);
        // Reorders all numbers based on the digit at each index right to left
        for(int i=0; i<n; i++)
        {
            int remainder = (values[i]/currentFactor)%10;
            temp[remainder].push_back(values[i]);
        }
        // Copies numbers in the back into origianal array
        int currIndex = 0;
        for(int i=0; i<=9; i++)
        {
            if (!temp[i].empty())
            {
                for(int j=0; j<temp[i].size(); j++)
                {
                    values[currIndex] = temp[i][j];
                    currIndex++;
                }
            }
        }
        // Empties temp array
        for(int i=0; i<=9; i++)
        {
            temp[i].clear();
        }
    }
}
// Sorting End -----------------------------------------------------------------------------------------------


