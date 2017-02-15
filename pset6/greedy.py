import pdb

#
def main():
    customer_owed = -1.0

    while customer_owed < 0.0:
        customer_owed = input("How much is the customer owed?\n")
        try:
            float(customer_owed)
        except:
            customer_owed = -1.0
        else:
            customer_owed = float(customer_owed)

    customer_owed = customer_owed * 100
    customer_owed = round(customer_owed, 2)

    quarters = 0
    dimes = 0
    nickles = 0
    pennies = 0

    while customer_owed > 0:
        if customer_owed >= 25:
            quarters += 1
            customer_owed -= 25
        elif customer_owed >= 10:
            dimes += 1
            customer_owed -= 10
        elif customer_owed >= 5:
            nickles += 1
            customer_owed -= 5
        else:
            pennies += 1
            customer_owed -= 1

    print("{}".format(quarters + dimes + nickles + pennies))


if __name__ == "__main__":
    main()
