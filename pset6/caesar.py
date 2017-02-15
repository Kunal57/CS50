import sys

def main():
    # error for incorrect number of command line arguments
    if len(sys.argv) != 2:
        print("Invalid command line arguments!")
        exit(1)

    k = sys.argv[1]
    k = int(k) % 26

    print("{}".format(k))

    upper_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    lower_alphabet = "abcdefghijklmnopqrstuvwxyz"

    plaintext = input("Plaintext: ")
    ciphertext = ""

    for letter in plaintext:
        if letter in upper_alphabet or letter in lower_alphabet:
            if letter == letter.upper():
                index = upper_alphabet.index(letter)
                new_letter = (index + k) % 26
                ciphertext += upper_alphabet[new_letter]
            else:
                index = lower_alphabet.index(letter)
                new_letter = (index + k) % 26
                ciphertext += lower_alphabet[new_letter]
        else:
            ciphertext += letter

    print("Ciphertext: {}".format(ciphertext))
    exit(0)


if __name__ == "__main__":
    main()
