#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    float customer_owed;
    do
    {
        printf("How much is the customer owed?\n");
        customer_owed = GetFloat();
    } while (customer_owed < 0);


    customer_owed = customer_owed * 100;
    customer_owed = roundf(customer_owed);

    int quarters = 0;
    int dimes = 0;
    int nickles = 0;
    int pennies = 0;

    while (customer_owed > 0)
    {
        if (customer_owed >= 25)
        {
            quarters += 1;
            customer_owed -= 25;
        }
        else if (customer_owed >= 10)
        {
            dimes += 1;
            customer_owed -= 10;
        }
        else if (customer_owed >= 5)
        {
            nickles += 1;
            customer_owed -= 5;
        }
        else
        {
            pennies += 1;
            customer_owed -= 1;
        }
    }


    // printf("%i coins owed. %i quarters, %i dimes, %i nickles, %i pennies\n", quarters + dimes + nickles + pennies, quarters, dimes, nickles, pennies);
    printf("%i\n", quarters + dimes + nickles + pennies);

}
