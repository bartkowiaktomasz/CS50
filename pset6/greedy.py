# Calculates the minimum number of coints required to give a user change

import cs50

def main():
    change = -1
    while change < 0:
        print("O hai! How much change is owed?")
        change = cs50.get_float()
    
    intChange = 100*change
    counter = 0
    i = 0
    coins = [25, 10, 5, 1]
    for coin in coins:
        while intChange >= coin:
            i+=(intChange - intChange % coin)/coin
            intChange-= coin*i
            counter+=i
            break
        i = 0
    print("{0:.0f}".format(counter))

if __name__ == '__main__':
    main()