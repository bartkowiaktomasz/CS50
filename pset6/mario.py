# Prints out a half-pyramid of a specified height.
 
import cs50
 
def main():
    height = -1;    # initial height outside the [0, 23] to get into while loop
    while height < 0 or height > 23:
        print("Height:", end="")
        height = cs50.get_int()
    for i in range(height):
        for j in range((height-1)-i):
            print(" ", end="")
        for k in range(i+2):
            print("#", end="")
        print("")
 
if __name__ == '__main__':
    main()