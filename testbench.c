#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "hash.h"
#include "processText.h"

// http://www.cse.yorku.ca/~oz/hash.html
// Three hash functions

// 1. djb2

ub4
hash_djb2(ub1 *str)
{
	ub4 hash = 5381;
	int c;

	while (c = *str++)
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}

//2. sdbm
ub4
hash_sdbm(ub1 *str)
{
	ub4 hash = 0;
	int c;

	while (c = *str++)
		hash = c + (hash << 6) + (hash << 16) - hash;

	return hash;
}

// 3. K&R
ub4
hash_loseLose(ub1 *str)
{
	unsigned int hash = 0;
	int c;

	while (c = *str++)
		hash += c;

	return hash;
}
/*
unsigned int BKDRHash(const char* str, unsigned int length)
{
   unsigned int seed = 131; // 31 131 1313 13131 131313 etc.. 
   unsigned int hash = 0;
   unsigned int i    = 0;

   for (i = 0; i < length; ++str, ++i)
   {
      hash = (hash * seed) + (*str);
   }

   return hash;
}*/

// 4. 
ub4 hash_one_at_a_time(ub1 *key)
{
  ub4 len = strlen(key);
  ub4   hash, i;
  for (hash=0, i=0; i<len; ++i)
  {
    hash += key[i];
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }
  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);
  return hash;
} 




// test Latin sample text
void testbench(char *filename)
{
	Text TextSample;
	TextSample.paragraph = processText(filename);
	TextSample.paragraph_len = strlen(TextSample.paragraph);	
	TextSample.paragraph = replaceSpace(TextSample.paragraph);
	TextSample.num_of_words = countWords(TextSample.paragraph, TextSample.paragraph_len); 		
	splitSentences(&TextSample);

	char queryWord[30]; 
	scanf("%s", queryWord);
	
	ub4 key1 = hash_djb2(queryWord);
	ub4 key2 = hash_sdbm(queryWord);
	ub4 key3 = hash_loseLose(queryWord);
	ub4 key4 = hash_one_at_a_time(queryWord);

	// Test 1: different hash functions using linear probing
	printf("Test1: \n");
	printf("Hash Table addresing: Linear Probing\n");
	//
	printf("\n\n(1) Hash function: djb2\n");
	HashTable hashTableA1;
	initHashTable(&hashTableA1, TextSample.num_of_words);
	
	clock_t store_t1 = clock();
	storeWords(&hashTableA1, &linearProbeInsert, &hash_djb2,
			  TextSample.paragraph, TextSample.paragraph_len);
	store_t1 = clock() - store_t1;
	printf("Number of collisions is %d", hashTableA1.collisions); 	
	printf("\nDuration to insert %d words into hash table is %f\n",
		   TextSample.num_of_words, ((float)store_t1)/CLOCKS_PER_SEC);
	
	displayItem(linearProbeFind(&hashTableA1, key1, queryWord), TextSample.sentences);

	
	//
	printf("\n\n(2) Hash function: sdbm\n");
	HashTable hashTableA2;	
	initHashTable(&hashTableA2, TextSample.num_of_words);	
	clock_t store_t2 = clock();
	storeWords(&hashTableA2, &linearProbeInsert, &hash_sdbm,
			  TextSample.paragraph, TextSample.paragraph_len);
	store_t2 = clock() - store_t2;
	printf("Number of collisions is %d", hashTableA2.collisions); 		
	printf("\nDuration to insert %d words into hash table is %f\n",
		   TextSample.num_of_words, ((float)store_t2)/CLOCKS_PER_SEC);
	
	displayItem(linearProbeFind(&hashTableA2, key2, queryWord), TextSample.sentences);

	//
	printf("\n\n(3) Hash function: K & R\n");	
	HashTable hashTableA3;
	initHashTable(&hashTableA3, TextSample.num_of_words);	
	clock_t store_t3 = clock();
	storeWords(&hashTableA3, &linearProbeInsert, &hash_loseLose,
			  TextSample.paragraph, TextSample.paragraph_len);
	store_t3 = clock() - store_t3;
	printf("Number of collisions is %d", hashTableA3.collisions); 		
	printf("\nDuration to insert %d words into hash table is %f\n",
		   TextSample.num_of_words, ((float)store_t3)/CLOCKS_PER_SEC);	

	displayItem(linearProbeFind(&hashTableA3, key3, queryWord), TextSample.sentences);
	
	//
	printf("\n\n(4) Hash function: Jenkin's one at a time\n");	
	HashTable hashTableA4;
	initHashTable(&hashTableA4, TextSample.num_of_words);	
	clock_t store_t4 = clock();
	storeWords(&hashTableA4, &linearProbeInsert, &hash_one_at_a_time,
			  TextSample.paragraph, TextSample.paragraph_len);
	store_t4 = clock() - store_t4;
	printf("Number of collisions is %d", hashTableA4.collisions); 		
	printf("\nDuration to insert %d words into hash table is %f\n",
		   TextSample.num_of_words, ((float)store_t4)/CLOCKS_PER_SEC);

	displayItem(linearProbeFind(&hashTableA4, key4, queryWord), TextSample.sentences);
	
	deleteHashTable(&hashTableA1); 
	deleteHashTable(&hashTableA2);
	deleteHashTable(&hashTableA3);
	deleteHashTable(&hashTableA4);


	// Test 2: different hash functions using quadratic probing
	printf("Test1: \n");
	printf("Hash Table addresing: Quardatic Probing\n");
	//
	printf("\n\n(1) Hash function: djb2\n");
	HashTable hashTableB1;
	initHashTable(&hashTableB1, TextSample.num_of_words);
	
	store_t1 = clock();
	storeWords(&hashTableB1, &quadraticProbeInsert, &hash_djb2,
			  TextSample.paragraph, TextSample.paragraph_len);
	store_t1 = clock() - store_t1;
	printf("Number of collisions is %d", hashTableB1.collisions); 
	printf("\nDuration to insert %d words into hash table is %f\n",
		   TextSample.num_of_words, ((float)store_t1)/CLOCKS_PER_SEC);
	
	displayItem(quadraticProbeFind(&hashTableB1, key1, queryWord), TextSample.sentences);

	
	//
	printf("\n\n(2) Hash function: sdbm\n");
	HashTable hashTableB2;	
	initHashTable(&hashTableB2, TextSample.num_of_words);	
	store_t2 = clock();
	storeWords(&hashTableB2, &quadraticProbeInsert, &hash_sdbm,
			  TextSample.paragraph, TextSample.paragraph_len);
	store_t2 = clock() - store_t2;
	printf("Number of collisions is %d", hashTableB2.collisions); 	
	printf("\nDuration to insert %d words into hash table is %f\n",
		   TextSample.num_of_words, ((float)store_t2)/CLOCKS_PER_SEC);
	
	displayItem(quadraticProbeFind(&hashTableB2, key2, queryWord), TextSample.sentences);

	//
	printf("\n\n(3) Hash function: K & R\n");	
	HashTable hashTableB3;
	initHashTable(&hashTableB3, TextSample.num_of_words);	
	store_t3 = clock();
	storeWords(&hashTableB3, &quadraticProbeInsert, &hash_loseLose,
			  TextSample.paragraph, TextSample.paragraph_len);
	store_t3 = clock() - store_t3;
	printf("Number of collisions is %d", hashTableB3.collisions); 	
	printf("\nDuration to insert %d words into hash table is %f\n",
		   TextSample.num_of_words, ((float)store_t3)/CLOCKS_PER_SEC);	

	displayItem(quadraticProbeFind(&hashTableB3, key3, queryWord), TextSample.sentences);
	
	//
	printf("\n\n(4) Hash function: Jenkin's one at a time\n");	
	HashTable hashTableB4;
	initHashTable(&hashTableB4, TextSample.num_of_words);	
	store_t4 = clock();
	storeWords(&hashTableB4, &quadraticProbeInsert, &hash_one_at_a_time,
			  TextSample.paragraph, TextSample.paragraph_len);
	store_t4 = clock() - store_t4;
	printf("Number of collisions is %d", hashTableB4.collisions); 	
	printf("\nDuration to insert %d words into hash table is %f\n",
		   TextSample.num_of_words, ((float)store_t4)/CLOCKS_PER_SEC);

	displayItem(quadraticProbeFind(&hashTableB4, key4, queryWord), TextSample.sentences);
	
	deleteHashTable(&hashTableB1); 
	deleteHashTable(&hashTableB2);
	deleteHashTable(&hashTableB3);
	deleteHashTable(&hashTableB4);
}

int main(void)
{
	testbench("text.txt");
	return 0;
}


