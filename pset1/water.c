#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int length_of_shower;
    do
    {
        printf("How many minutes do you shower for?\n");
        length_of_shower = GetInt();
    } while (length_of_shower <= 0);
    int bottles_of_water = length_of_shower * 12;
    printf("You use %i bottles worth of water when you shower!\n", bottles_of_water);
}
