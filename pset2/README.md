### Cyrptography

###### Initials
Implement a program that, given a person’s name, prints a person’s initials, per the below.

```
$ ./initials
Regulus Arcturus Black
RAB

$ ./initials
  robert   thomas bowden
RTB
```

###### Caesar
Implement a program that encrypts messages using Caesar’s cipher, per the below.
Usage: `./caesar key`

```
$ ./caesar 13
plaintext:  HELLO
ciphertext: URYYB

$ ./caesar 13
plaintext:  be sure to drink your Ovaltine
ciphertext: or fher gb qevax lbhe Binygvar
```

###### Crack
Implement a program that cracks passwords, per the below. The password has been hashed with C's DES-based (not MD5-based) crypt function. The password is no longer than four characters and is composed entirely of alphabetical characters (uppercase and/or lowercase).
Usage: `./crack hash`

```
$ ./crack 50fkUxYHbnXGw
rofl
```
