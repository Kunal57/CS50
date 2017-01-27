/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */

#include <cs50.h>
#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // return false if a negative array size is entered
    if (n < 0)
    {
        return false;
    }

    // declare the initial beginning, end, and length of the array
    int beginning = 0;
    int end = n - 1;
    int length = n;
    int middle;

    // run this loop while the array has at least 1 value in it
    while (length > 0)
    {
        // determine the new length and mid-point(index) of the array to search
        length = (end - beginning) + 1;
        middle = ((end - beginning) / 2) + beginning;

        // if the middle index is the value, return true
        if (value == values[middle])
        {
            return true;
        }
        // if the value is greater than the middle index value, the new beginning point is the middle index plus 1
        else if (value > values[middle])
        {
            beginning = middle + 1;
        }
        // if the value is less than the middle index value, the new end point is the middle index minus 1
        else if (value < values[middle])
        {
            end = middle - 1;
        }
    }

    // // Linear Search
    // for(int i = 0; i < n; i++)
    // {
    //     if (values[i] == value)
    //     {
    //         return true;
    //     }
    // }

    // return false if value is not found in array
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // implement an O(n^2) sorting algorithm
    int counter = 1;
    while (counter > 0)
    {
        // set counter to 0 and increment if any pairs are changed
        counter = 0;
        for (int i = 0; i < n; i++)
        {
            // if (i + 1) is greater than the size of the array, we are checking the last value
            // the last value can't be compared to the value next to it because it is the last value
            // as a result, we stop running the loop because all the values have been checked and sorted
            if (i + 1 < n)
            {
                int first = values[i];
                int next = values[i + 1];

                // if the first value is greater than the second value, the two values are flipped
                // the smaller value is placed infront of the greater value
                if (first > next)
                {
                    values[i] = next;
                    values[i + 1] = first;
                    counter ++;
                }
            }
        }
    }
    // return the sorted array (arrays are destructive in C) at the end of the program
    // this is because the while loop won't loop if the counter is 0 because no changer were made on the last pass through
    return;
}
