#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"


void createItem(Item *item, ub4 key, ub1 * word, int sentence_num)
{
	item->key = key;
	strcpy(item->word, word);
	item->frequency = 0;
	item->sentence_location[item->frequency++] = sentence_num;
}


// the length of the hash table is based on 2^n
void initHashTable(HashTable *ht, int num_of_words)
{
	ht->collisions = 0;
	ht->D = 5;
	while(loadfactor(num_of_words, ht->D) > OPTIMAL_LOAD_FACTOR)
	{
		ht->D++;
	}
	int hashSize = hashsize(ht->D);
	//printf("size: %d\n", hashSize);
	ht->hashTB = (Item *)malloc(sizeof(Item) * hashSize);
	ht->empty = (bool *)malloc(sizeof(bool) * hashSize);
	for(int i = 0; i < hashSize; i++)
	{
		ht->empty[i] = true;
	}	
}

void deleteHashTable(HashTable *ht)
{
	free(ht->hashTB);
	free(ht->empty);		 	
}


int linearProbeFindIndex(HashTable *ht, ub4 key, ub1 *word, bool countCollide)
{
	int index = (key & hashmask(ht->D));
	int j = index;

	if(countCollide)
	{
		if(!ht->empty[j] && !(strcmp(ht->hashTB[j].word,word) == 0))
		{
			ht->collisions++;
		}
	}
	do
	{
		if(ht->empty[j] || strcmp(ht->hashTB[j].word,word) == 0)
		{
			return j;
		}		
		j = ((j+1) & hashmask(ht->D));
	} while(j != index);    
	return j; // hash table is full
}
//ht->hashTB[j].key == key 
Item *linearProbeFind(HashTable *ht, ub4 key, ub1 *word)
{
	clock_t t  = clock();
	int b = linearProbeFindIndex(ht, key, word, false);
	t = clock() - t;
	printf("Duration to search:  %f\n", ((float)t)/CLOCKS_PER_SEC);
	if(ht->empty[b] || strcmp(ht->hashTB[b].word, word) != 0) return NULL;
	return &ht->hashTB[b];
}

bool linearProbeInsert(HashTable *ht, ub4 key, ub1 *word, int sentence_num)
{
	int b = linearProbeFindIndex(ht, key, word, true);

	if(ht->empty[b])
	{
		// empty slot for insertion.
		createItem(&ht->hashTB[b], key, word, sentence_num);
		ht->empty[b] = false;
		//printf("\nSuccess: %d", b); 
		return true;
	}
	else if(!ht->empty[b] && strcmp(word, ht->hashTB[b].word) == 0)
	{
		// same word, record the sentence number and frequency
		ht->hashTB[b].sentence_location[ht->hashTB[b].frequency++] = sentence_num;
		return true;
	}
	// table is full
	return false;
}

int quadraticProbeFindIndex(HashTable *ht, ub4 key, ub1 *word, bool countCollisions)
{
	int index = (key & hashmask(ht->D));
	int j = index;
	int i = 0;
	if(countCollisions)
	{
		if(!ht->empty[j] && !(strcmp(ht->hashTB[j].word, word) == 0))
			ht->collisions++;
	}
	do
	{	   
		if(ht->empty[j] || strcmp(ht->hashTB[j].word,word) == 0)
		{
			return j;
		}

		
		j = ((j + i*i) & hashmask(ht->D));
		i++;
	}while(j != index);
	return j; // hash table is full
}
//ht->hashTB[j].key == key 
Item *quadraticProbeFind(HashTable *ht, ub4 key, ub1 *word)
{
	clock_t t  = clock();
	int b = quadraticProbeFindIndex(ht, key, word, false);
	t = clock() - t;
	printf("Duration to search:  %f\n", ((float)t)/CLOCKS_PER_SEC);
	if(ht->empty[b] || strcmp(ht->hashTB[b].word, word) != 0) return NULL;
	return &ht->hashTB[b];
}

bool quadraticProbeInsert(HashTable *ht, ub4 key, ub1 *word, int sentence_num)
{
	int b = quadraticProbeFindIndex(ht, key, word, true);

	if(ht->empty[b])
	{
		// empty slot for insertion.
		createItem(&ht->hashTB[b], key, word, sentence_num);
		ht->empty[b] = false;
		//	printf("Success: %d", b); 
		return true;
	}
	else if(!ht->empty[b] && strcmp(word, ht->hashTB[b].word) == 0)
	{
		// same word, record the sentence number and frequency
		ht->hashTB[b].sentence_location[ht->hashTB[b].frequency++] = sentence_num;
		return true;
	}
	// table is full
	return false;
}

void storeWords(HashTable *hTable,
				bool (*hInsert)(HashTable *, ub4, ub1 *, int),
				ub4 (*hashFunction)(ub1 *),
				ub1 *paragraph, int paragraph_len)
{
	bool process_word_flag = false;
	int sentence_num = 0;
	int src = 0;
	
	for(int i = 0; i < paragraph_len; i++)
	{

		bool skipFlag = paragraph[i] == ' ' || paragraph[i] == ',' || paragraph[i] == '\n';
		if(!process_word_flag)
		{
			if(paragraph[i] == '!' || paragraph[i] == '.' || paragraph[i] == '?')
			{
				sentence_num++;
			}
			else if(skipFlag)
			{
				continue;
			}					
			else
			{
				process_word_flag = true;				
				src = i;
			}			   				
		}
		else
		{
			char word[20] = {0};			
			if(paragraph[i] == '!' || paragraph[i] == '.' || paragraph[i] == '?')
			{				
				strncpy(word, paragraph + src, i-src);
				unsigned long key = hashFunction(word);		
				hInsert(hTable, key, word, sentence_num);				
				process_word_flag = false;
				sentence_num++; // increment the sentence number				
			}
			else if(skipFlag)
			{
				strncpy(word, paragraph + src, i-src);
				unsigned long key = hashFunction(word);		
				hInsert(hTable, key, word, sentence_num);			
				process_word_flag = false;
			}
			else continue;
		}
	}
}

void displayItem(Item *item, char sentences[1000][400])
{
	if(item == NULL)
	{
		printf("Query word not found");
		return;
	}
	printf("Query word: %s, Key: %lu\n", item->word, item->key); 
	printf("Query result:\n");
	for(int i = 0; i < item->frequency; i++)
	{
		printf("Sentence %d: %s\n", i+1, sentences[item->sentence_location[i]]);
	}
}

void traverse(HashTable *ht)
{
	int count = 1;
	for(int i = 0; i < hashsize(ht->D); i++)
	{
		if(!ht->empty[i])
			printf("%d: %s\n", count++, ht->hashTB[i].word); 
	}
}
