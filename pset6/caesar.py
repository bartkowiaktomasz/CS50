# Encrypts messages using Caesar's cipher.

import cs50, sys

def main(argv):
    if len(sys.argv) != 2:
        print("Error, provide one command-line argument. Usage: python caesar.py key")
        return 1

    key = int(sys.argv[1])

    print("plaintext: ", end="")
    plaintext = cs50.get_string()
    ciphertext = []


    for character in plaintext:
        if str.isalpha(character) and str.islower(character):
            ciphertext.append(chr(ord('a') + (ord(character) - ord('a') + key)%26))
        elif str.isalpha(character) and str.isupper(character):
            ciphertext.append(chr(ord('A') + (ord(character) - ord('A') + key)%26))
        else:
            ciphertext.append(character)
    print("ciphertext: {}".format(''.join(ciphertext)))

if __name__ == "__main__":
    main(sys.argv[1:])