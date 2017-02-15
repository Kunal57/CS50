# define main function
def main():
    pyramid_height = -1

    # while the inputted pyramid_height is less than 0 or more than 23
    while pyramid_height < 0 or pyramid_height > 23:
        pyramid_height = input("Height: ")
        # try converting the input into an int
        try:
            int(pyramid_height)
        # if the input can't be converted to an int
        except:
            print("Must be an int!")
            pyramid_height = -1
        # if the input is successfully converted to an int
        else:
            pyramid_height = int(pyramid_height)

    # for a range of 0 through the inputted length
    for i in range(pyramid_height):
        spaces = pyramid_height - (i + 1)
        hashes = i + 2
        # print spaces followed by hashes
        # spaces should be the height subtracted by i plus 1 (starting at index of 0)
        # hashes shold be i + 2 since we're starting at an index of 0
        print("{}{}".format(" " * spaces, "#" * hashes))

# call main function as default
if __name__ == "__main__":
    main()
