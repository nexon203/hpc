#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>
#include <pthread.h>

/******************************************************************************
  Demonstrates how to crack an encrypted password using a simple
  "brute force" algorithm. Works on passwords that consist only of 2 uppercase
  letters and a 2 digit integer. Your personalised data set is included in the
  code. 

  Compile with:
    cc -o MThread MThread.c -lcrypt -pthread

  If you want to analyse the results then use the redirection operator to send
  output to a file that you can view using an editor or the less utility:

    ./MThread > MThread_results.txt

  Dr Kevan Buckley, University of Wolverhampton, 2018
******************************************************************************/
int n_passwords = 4;

char *encrypted_passwords[] = {
   "$6$KB$lkcN6OFGr2XTQGSj29X7iYQqA.GBFY5FjWQOy/HCqYu3oJ.YDciGTMrG.ZJa18HSuZGkLFiwiVY3mqTFDRVTN0",
  "$6$KB$HFnduiDmstEaBhQWTKqaCaKQaVQFrPOOH5OtLmr4k4kIRGy3rQT0o7ImUJFBOAennJLxlhc1n7cRQzjAMd7Hx1",
  "$6$KB$Z/wMw97.MbsjxFvFZacJuQYiyegBaODZm0KBZFiXMVhWxywV/LC5UD9YRr0WYUnzLcc65r886iJwORXyEf2eY.",
  "$6$KB$tzBwz4gFF2QOZNG1XM5MhMRRg/QdPA/py7xqovYTI/5UM7IeTkHuCpCWseSppiBEi2613pDOCBcR0zRuPSG1L."
};

/**
 Required by lack of standard function in C.   
*/

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

/**
 This function can crack the kind of password explained above. All combinations
 that are tried are displayed and when the password is found, #, is put at the 
 start of the line. Note that one of the most time consuming operations that 
 it performs is the output of intermediate results, so performance experiments 
 for this kind of program should not include this. i.e. comment out the printfs.
*/

void *function()
{
   int i;
   pthread_t t1, t2;
    void *kernel_function_1();
   void *kernel_function_2();

   for(i=0;i<n_passwords;i<i++) {
     pthread_create(&t1, NULL, kernel_function_1, encrypted_passwords[i]);
     pthread_create(&t2, NULL, kernel_function_2, encrypted_passwords[i]);

     pthread_join(t1, NULL);
     pthread_join(t2, NULL);

  }
}

void *kernel_function_1(char *salt_and_encrypted){
  int C,D,F;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space for \0
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(C='A'; C<='M'; C++){
    for(D='A'; D<='Z'; D++){
      for(F=0; F<=99; F++){
        sprintf(plain, "%c%c%02d", C, D, F); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        }
// else {
         // printf(" %-8d%s %s\n", count, plain, enc);
        //}
      }
    }
  }
  printf("%d solutions explored\n", count);
}
void *kernel_function_2(char *salt_and_encrypted){
  int F, U, C;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space for \0
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(F='N'; F<='Z'; F++){
    for(U='A'; U<='Z'; U++){
      for(C=0; C<=99; C++){
        sprintf(plain, "%c%c%02d", F, U, C); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } //else {
         // printf(" %-8d%s %s\n", count, plain, enc);
       // }
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
  long long int time_elapsed;

  clock_gettime(CLOCK_MONOTONIC, &start);
 
  
  
    function();
  

  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, 
         (time_elapsed/1.0e9)); 

  return 0;
}