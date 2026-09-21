William Azevedo-Homan
811#: 811720785

Compiling and Running
---------------------
To compile, place lab05.c in the same directory as the Makefile, then run:

   	    make compile

This produces an executable named lab05.out. Run it with one of the following forms:

     	      ./lab05.out -decimal <x> -bits <n>
	      ./lab05.out -bits <n> -decimal <x>
	      
	      Converts the signed decimal integer x to its n-bit two's
	      complement binary represenation.

	      ./lab05.out -binary <bitString>

	      Converts the given two's complement bit string to its signed
	      decimal value.

	      Example:

	      ./lab05.out -decimal -16 -bits 12

To remove the compiled executable, run:

   	      make clean
