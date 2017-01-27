/**
 * generate.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Generates pseudorandom numbers in [0,LIMIT), one per line.
 *
 * Usage: generate n [s]
 *
 * where n is number of pseudorandom numbers to print
 * and s is an optional seed
 */

#define _XOPEN_SOURCE

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// constant
#define LIMIT 65536

int main(int argc, string argv[])
{
    // If the argument count isn't either 2 or 3, return "Usage: generate n [s]"
    if (argc != 2 && argc != 3)
    {
        printf("Usage: generate n [s]\n");
        return 1;
    }

    // Declare variable n and assign it to the the value of argument 1 converted to an integer (pseudorandom numbers to print)
    int n = atoi(argv[1]);

    // If the argument count equals 3, then take the 3rd argument (seed) and convert it into an integer
    // to generate the pseudo-random number sequence associated with that seed. Else generate a random
    // pseudo-random number without a seed.
    if (argc == 3)
    {
        srand48((long int) atoi(argv[2]));
    }
    else
    {
        srand48((long int) time(NULL));
    }

    // Loops through the code n times (the value inputed for the 2nd argument).
    // Prints an integer to the screen. The The value of the ingeter is a random number between 0.0 & 1.0
    // multipled by the LIMIT which is set to 65536 at the top of the program. The output type, double is converted into an int.
    for (int i = 0; i < n; i++)
    {
        printf("%i\n", (int) (drand48() * LIMIT));
    }

    // success
    return 0;
}
