#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>

/***********************************************************************
*******
  Demonstrates how to crack an encrypted password using a simple
  "brute force" algorithm. Works on passwords that consist only of 2
uppercase
  letters and a 2 digit integer. Your personalised data set is included
in the
  code.

  Compile with:
    cc -o CrackAZ99-With-Data 3initialpasswordcrack.c -lcrypt -pthread

  If you want to analyse the results then use the redirection operator
to send
  output to a file that you can view using an editor or the less
utility:

    ./CrackAZ99-With-Data >crack.txt

  Dr Kevan Buckley, University of Wolverhampton, 2018
************************************************************************
******/
int n_passwords = 4;

char *encrypted_passwords[] = {

  "$6$KB$USbZToxSR.NNRAtXir98EwcU5ilQT0MNLvEIQiQeX7WpJEK/ey3U/L9DB1J5y.30LDIce1xu0RiS5WwY4vgI4/",
  "$6$KB$9b40a.jXbR.UCKxKbcJCA8pDac43HeeNKh11haB1jdC9XVgC5RgeGrHQXC6Lug4TAHly4R3hiSlQx0AIhX51H1",
  "$6$KB$TXpHr7EJl1nt0zn.PfixteNX02v9joyslUtz/OERahB/tS8WQlu7s43Kn75ou40JIgYoOvVUD8GU4Fmevfpgm1",
  "$6$KB$EQNNwMIddTvap2NhJEhL9p3DhryTPGc/onLK44dWJrpW/Cxh0Udu3dMk.vO79jQlsZa4IYMNR/EVVDAKJVJje/",
};

/**
 Required by lack of standard function in C.   
*/

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

/**
 This function can crack the kind of password explained above. All
combinations
 that are tried are displayed and when the password is found, #, is put
at the
 start of the line. Note that one of the most time consuming operations
that
 it performs is the output of intermediate results, so performance
experiments
 for this kind of program should not include this. i.e. comment out the
printfs.
*/

void crack(char *salt_and_encrypted){
  int w, x, y, z;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space

  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);
	for(w='A'; w<='Z'; w++){
	  for(x='A'; x<='Z'; x++){
	    for(y='A'; y<='Z'; y++){
	      for(z=0; z<=99; z++){
		sprintf(plain, "%c%c%c%02d", w, x, y, z);
		enc = (char *) crypt(plain, salt);
		count++;
		if(strcmp(salt_and_encrypted, enc) == 0){
		 // printf("#%-8d%s %s\n", count, plain, enc);
		} else {
		  //printf(" %-8d%s %s\n", count, plain, enc);
		}
	      }
	    }
	  }
	}
  printf("%d solutions explored\n", count);
}
int time_difference(struct timespec *start, struct timespec *finish,
                    long long int *difference) {
  long long int ds =  finish->tv_sec - start->tv_sec; 
  long long int dn =  finish->tv_nsec - start->tv_nsec; 

  if(dn < 0 ) {
    ds--;
    dn += 1000000000; 
  } 
  *difference = ds * 1000000000 + dn;
  return !(*difference > 0);
}

int main(int argc, char *argv[]){

  struct timespec start, finish;
  long long int difference;   
  int account = 0;
  clock_gettime(CLOCK_MONOTONIC, &start);

  int i;

  for(i=0;i<n_passwords;i<i++) {
    crack(encrypted_passwords[i]);
  }

  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &difference);

  printf("Elapsed Time: %9.5lfs\n", difference/1000000000.0);
  printf("\n");

  return 0;
}
