#include <stdio.h>
#include <cs50.h>
#include <stdlib.h> // atoi()
#include <string.h> // strlen()
#include <ctype.h> // isaplha(), isupper(), islower()

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Invalid number of arguments. Please try again.\n");
        return 1;
    }

    int k = atoi(argv[1]);

    // printf("Message to Encrypt:\n");
    string message = GetString();

    for(int i = 0, n = strlen(message); i < n; i++)
    {
        char character = message[i];
        if (isalpha(character))
        {
            if(isupper(character))
            {
                string upper_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
                for (int a = 0, b = strlen(upper_alphabet); a < b; a++)
                {
                    if (character == upper_alphabet[a])
                    {
                        int c = a + k;
                        if (c > 25)
                        {
                            c = c % 26;
                        }
                        printf("%c", (upper_alphabet[c]));
                    }
                }
            }
            else
            {
                string lower_alphabet = "abcdefghijklmnopqrstuvwxyz";
                for (int d = 0, e = strlen(lower_alphabet); d < e; d++)
                {
                    if (character == lower_alphabet[d])
                    {
                        int f = d + k;
                        if (f > 25)
                        {
                            f = f % 26;
                        }
                        printf("%c", (lower_alphabet[f]));
                    }
                }
            }
        }
        else
        {
            printf("%c", character);
        }
    }
    printf("\n");

    return 0;
}
