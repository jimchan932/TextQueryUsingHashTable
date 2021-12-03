#ifndef PROCESSTEXT
#define PROCESSTEXT

typedef struct Text Text;
struct Text
{
	char *paragraph;
	int paragraph_len;
	char sentences[1000][400];
	int num_of_sentences;
	int num_of_words;

};

char *replaceSpace(char *paragraph);

char  *processText(char *filename);

void splitSentences(Text *SampleText);

void deleteTextSample(Text *SampleText);

int countWords(char *paragraph, int len);
#endif
