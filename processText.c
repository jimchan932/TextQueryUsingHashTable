#include "processText.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

char *replaceSpace(char *paragraph)
{
	char *current_pos = strchr(paragraph, '\n');
	while(current_pos)
	{
		*current_pos = ' ';
		current_pos = strchr(current_pos, '\n');		
	}
	return paragraph;
}

char  *processText(char *filename)
{
	long length;
	char *buffer;

	FILE *f = fopen(filename, "r");

	if (f)
	{
	  fseek (f, 0, SEEK_END);
	  length = ftell (f);
	  fseek (f, 0, SEEK_SET);
	  buffer = malloc (length);
	  if (buffer)
	  {
		fread (buffer, 1, length, f);
	  }
	  fclose (f);
	}
	return buffer;
}

// return number of sentences 
void splitSentences(Text *SampleText)
{
	SampleText->num_of_sentences = 0;
	int src = 0;
	for(int i = 0; i < SampleText->paragraph_len; i++)
	{
		if(SampleText->paragraph[i] == '!' ||
		   SampleText->paragraph[i] == '.' ||
		   SampleText->paragraph[i] == '?')
		{
			strncpy(SampleText->sentences[SampleText->num_of_sentences++], SampleText->paragraph + src, i-src+1); 
			src = i+1;
			
		}		
	}
}
/*
void deleteTextSample(Text *SampleText)
{
	for(int i = 0; i < SampleText->num_of_sentences; i++)
	{
		free(SampleText->sentences[i]);
	}
}*/

int countWords(char *paragraph, int len)
{
	int count = 0;
	bool word_flag = false;
	for(int i = 0; i < len; i++)
	{
		bool sign_flag = paragraph[i] == ' ' || paragraph[i] == '.' ||
			            paragraph[i] == '?' || paragraph[i] == '!' ||
			            paragraph[i] == ',' == paragraph[i] == '\"';
		if(!word_flag)
		{
			if(sign_flag)
			{
				continue;
			}
			else
			{
				count++;
				word_flag = true;				
			}
		}
		else
		{
			if(sign_flag)
			{
				word_flag = false;
			}
			else continue;
		}
	}
	return count;
}
/*
int main(void)
{
	Text TextSample;
	TextSample.paragraph = processText("latin_text.txt");
	if (TextSample.paragraph)
	{
		printf("%s", TextSample.paragraph);
	}
	TextSample.paragraph = replaceSpace(TextSample.paragraph);
	TextSample.paragraph_len = strlen(TextSample.paragraph);	
	TextSample.num_of_words = countWords(TextSample.paragraph,
										 TextSample.paragraph_len); 	
	splitSentences(&TextSample);
	
	printf("%s", TextSample.sentences[0]);
	for(int i = 0; i < TextSample.num_of_sentences; i++)
	{
		printf("%d: %s\n", i, TextSample.sentences[i]); 
	}
	//	printf("%s", TextSample.sentences[TextSample.num_of_sentences-1]);
	//printf("%d", TextSample.num_of_words);
	deleteTextSample(&TextSample);
	return 0;
}

*/
