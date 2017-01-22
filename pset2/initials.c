#include <stdio.h>
#include <cs50.h>
#include <ctype.h> // toupper()
#include <string.h> // strlen()

int main(void)
{
    //printf("What is your name?\n");
    string name = GetString();
    printf("%c", toupper(name[0]));

    for(int i = 0, n = strlen(name); i < n; i++)
    {
        if (name[i] == 32)
        {
            printf("%c", toupper((char) name[i + 1]));
        }
    }
    printf("\n");
}
