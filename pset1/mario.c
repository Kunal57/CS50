#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int pyramid_height;

    do
    {
        printf("How tall should Mario's pyramid be? (1-23)\n");
        pyramid_height = GetInt();
    } while (pyramid_height < 0 || pyramid_height > 23);

    for(int i = 1; i <= pyramid_height; i++)
    {

        for(int y = 0; y < pyramid_height - i; y++)
        {
            printf(" ");
        }

        for(int x = 1; x < i + 2; x++)
        {
            printf("#");
        }

        printf("\n");
    }
}
