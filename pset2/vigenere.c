#include <stdio.h>
#include <cs50.h>
#include <stdlib.h> // atoi()
#include <string.h> // strlen()
#include <ctype.h> // isaplha(), isupper(), islower()

int main(int argc, string argv[])
{
    string keyword = argv[1];

    // check to see if program was executed with exactly 2 command-line arguments
    if (argc != 2)
    {
        printf("Not valid input.\n");
        return 1;
    }

    // loop through the keyword and check if each character is a letter in the alphabet
    for (int i = 0, n = strlen(keyword); i < n; i++)
    {
        if (!isalpha(keyword[i]))
        {
            printf("Not valid input.\n");
            return 1;
        }
    }

    // convert string to be all capitalized
    for (int z = 0, y = strlen(keyword); z < y; z++)
    {
        keyword[z] = toupper(keyword[z]);
    }


    // get string to encrypt from user
    // printf("Message to Encrypt:\n");
    string message = GetString();

    // encrypt the message
    string upper_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string lower_alphabet = "abcdefghijklmnopqrstuvwxyz";
    int counter = 0;
    int keyword_length = strlen(keyword);
    for (int a = 0, b = strlen(message); a < b; a++)
    {
        if (isalpha(message[a]))
        {
            if isupper(message[a])
            {
                int keyword_int = keyword[counter % keyword_length] - 65;
                int message_int = message[a] - 65;
                int encrypted_letter = keyword_int + message_int;

                // printf("%i\n", keyword_int);
                // printf("%i\n", message_int);
                // printf("%i\n", encrypted_letter);

                if (encrypted_letter > 25)
                {
                    encrypted_letter = encrypted_letter % 26;
                }
                counter++;
                printf("%c", upper_alphabet[encrypted_letter]);
            }
            else
            {
                int keyword_int = keyword[counter % keyword_length] - 65;
                int message_int = message[a] - 97;
                int encrypted_letter = keyword_int + message_int;

                // printf("%i\n", keyword_int);
                // printf("%i\n", message_int);
                // printf("%i\n", encrypted_letter);

                if (encrypted_letter > 25)
                {
                    encrypted_letter = encrypted_letter % 26;
                }

                counter ++;
                printf("%c", lower_alphabet[encrypted_letter]);
            }
        }
        else
        {
            printf("%c", message[a]);
        }
    }

    // print a new line & return 0 if the program is correctly executed
    printf("\n");
    return 0;
}
