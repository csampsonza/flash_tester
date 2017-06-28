#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <bits/time.h>
#include <time.h>

#define MAX_FILE_SIZE  0x1FFFFFF // 31meg max size

void delta_time (struct timespec *t1, struct timespec *t2 )
   {
   time_t sec_d = t2->tv_sec - t1->tv_sec  ;
   time_t nsec_d = (t2->tv_nsec - t1->tv_nsec) ;
   float msec = (float)nsec_d / 1000000000 + (float)sec_d;

   printf(",%d",(uint)(msec * 1000.0)) ;
   fflush( stdout );
   }

const char *filename = "flash_test.txt";

int flash_test( uint block_sz  )
   {
   int rc ;

   struct timespec t1, t2;

   size_t sz;
   FILE *fptr;
   int *ram_block = malloc(block_sz * sizeof(int));
   int *test_block = malloc(block_sz * sizeof(int));

   if (block_sz == 0) return 0 ;


   for (uint i = 0; i < block_sz; i++)
      {
      ram_block[i] = rand();
      }


   clock_gettime(CLOCK_MONOTONIC, &t1);

   fptr = fopen(filename, "w+");
   assert ( fptr );
   sz = fwrite( ram_block, sizeof(int), block_sz , fptr );
   assert ( fclose(fptr) == 0 );

   clock_gettime(CLOCK_MONOTONIC, &t2);
   delta_time ( &t1 , &t2 );

   usleep(5000); // Give process a chance.


   assert ( sz != block_sz*sizeof(uint) );

   clock_gettime(CLOCK_MONOTONIC, &t1);
   sync();
   clock_gettime(CLOCK_MONOTONIC, &t2);
   delta_time ( &t1 , &t2 );

   usleep(5000); // Give process a chance.



   clock_gettime(CLOCK_MONOTONIC, &t1);
   fptr = fopen(filename , "r");
   assert ( fptr );
   sz = fread(test_block,sizeof(int), block_sz,fptr);
   assert ( fclose(fptr) == 0);

   clock_gettime(CLOCK_MONOTONIC, &t2);
   delta_time ( &t1 , &t2 );

   usleep(5000); // Give process a chance.


   assert ( sz != block_sz*sizeof(uint) );

   clock_gettime(CLOCK_MONOTONIC, &t1);
   sync();
   clock_gettime(CLOCK_MONOTONIC, &t2);
   delta_time ( &t1 , &t2 );

   usleep(5000); // Give process a chance.



   clock_gettime(CLOCK_MONOTONIC, &t1);
   assert ( remove(filename) == 0 );
   clock_gettime(CLOCK_MONOTONIC, &t2);
   delta_time ( &t1 , &t2 );

   usleep(5000); // Give process a chance.

   clock_gettime(CLOCK_MONOTONIC, &t1);
   sync();
   clock_gettime(CLOCK_MONOTONIC, &t2);
   delta_time ( &t1 , &t2 );

   rc = memcmp(test_block,ram_block,block_sz*sizeof(int)) ;

   free(test_block);
   free(ram_block);

   return rc;
   }



int main(int argc , char *argv[])
   {
   uint seed =0 ;
   uint val, size ;

   if (argc > 1)
      {
      uint input ;
      if ( sscanf(argv[1],"%u", &input) == 1)
         {
         seed = input ;
         }
      printf("%s %d\n",argv[1], seed);
      }
   srand(seed);


   while (1)
      {
      // Creates values from 0 - 8megs
      val = (uint) rand();
      size = val & 0x0FFFF << (val >> 28);
      size &= MAX_FILE_SIZE ;
      if ( size == 0 ) continue;

      printf("%d,%d", seed, size);
      fflush(stdout);

      int rc = flash_test(size);

      printf(",%d\n", rc);
      usleep(50000);
      }
   return 0;
   }