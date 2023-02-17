#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<locale.h>
#include<windows.h>

#define FAILURE 0
#define SUCCESS 1

#define STRCMP_EQUAL 0
#define SIZE_CHOICE_LENGTH 25
#define SIZE_TEXT_LENGTH 8192
#define SIZE_WORD_LENGTH 64
#define SIZE_WORD_SIZE 4096

#define STATE_PALATAL_ZERO 0
#define STATE_PALATAL_BACK 1
#define STATE_PALATAL_FRONT 2
#define STATE_LABIAL_ZERO 0
#define STATE_LABIAL_FLAT 1
#define STATE_LABIAL_ROUND 2

#define STR_ALPHABET "abcçdefgðhiýjklmnoöprsþtuüvyz"
#define STR_VOWEL "aeýioöuü"
#define STR_VOWEL_BACK "aýou"
#define STR_VOWEL_FRONT "eiöü"
#define STR_VOWEL_LARGER_FLAT "ae"
#define STR_VOWEL_NARROW_FLAT "ýi"
#define STR_VOWEL_LARGER_ROUND "oö"
#define STR_VOWEL_NARROW_ROUND "uü"
#define STR_CONSONANT "bcçdfgðhjklmnprsþtvyz"
#define STR_CONSONANT_STRONG "çfhkpsþt"
#define STR_CONSONANT_SOFT "bcdgðjlmnrvyz"
#define STR_PLURAL_1 "ler"
#define STR_PLURAL_2 "lar"
#define STR_PUNCTUATION ".:?!"

int menu(unsigned char choice[], unsigned char text[],
		 unsigned char words[][SIZE_WORD_LENGTH]);
int connexion(unsigned char words[][SIZE_WORD_LENGTH]);
int palatalHarmony(unsigned char words[][SIZE_WORD_LENGTH]);
int labialHarmony(unsigned char words[][SIZE_WORD_LENGTH]);
int strongConsonant(unsigned char text[]);
int softConsonant(unsigned char text[]);
int pluralWords(unsigned char text[]);
int numberOfWords(unsigned char words[][SIZE_WORD_LENGTH]);
int numberOfSentences(unsigned char text[]);
int numberOfLetters(unsigned char text[]);
int isInCharset(unsigned char ch, unsigned char charset[]);
void lowerText(unsigned char text[]);
void wordSet(unsigned char text[], unsigned char words[][SIZE_WORD_LENGTH]);

int main()
{
	int ret=0, i=0;
	unsigned char choice[SIZE_CHOICE_LENGTH] = {0};
	unsigned char text[SIZE_TEXT_LENGTH] = {0};
	unsigned char words[SIZE_WORD_SIZE][SIZE_WORD_LENGTH] = {0};

	setlocale(LC_ALL, "Turkish_Turkey.1254");
	SetConsoleOutputCP(1254);
	SetConsoleCP(1254);

	printf("Please enter the text!\n");
	fgets(text, SIZE_TEXT_LENGTH, stdin);
	text[strlen(text) - 1] = '\0';
	lowerText(text);

	wordSet(text, words);
	do
	{
		ret = menu(choice, text, words);
	}
	while (ret);

	return SUCCESS;
}

int menu(unsigned char choice[], unsigned char text[],
		 unsigned char words[][SIZE_WORD_LENGTH])
{
	printf("\nI'M CHECKING FOR: Ulama, labial-palatal harmony,"
			"hard-soft vowels, plural words, number of words,"
			"number of sentences, number of letters, etc.\n");
	printf("Is there a rule you want checked?\n");
	printf("Please type one of the following rules:\n");
	printf("*ulama\n");
	printf("*büyük ünlü uyumu\n");
	printf("*küçük ünlü uyumu\n");
	printf("*sert ünsüzler\n");
	printf("*yumuþak ünsüzler\n");
	printf("*çoðul kelimeler\n");
	printf("*kelime sayýsý\n");
	printf("*cümle sayýsý\n");
	printf("*harf sayýsý\n");
	printf("*çýkýþ\n");

	fgets(choice, SIZE_CHOICE_LENGTH, stdin);
	choice[strlen(choice) - 1] = '\0';

	if (strcmp(choice,"ulama") == STRCMP_EQUAL)
	{
		printf("%d number of ulama found\n", connexion(words));
	}
	else if (strcmp(choice,"büyük ünlü uyumu") == STRCMP_EQUAL)
	{
		printf("%d number of büyük ünlü uyumu found\n", palatalHarmony(words));
	}
	else if (strcmp(choice,"küçük ünlü uyumu") == STRCMP_EQUAL)
	{
		printf("%d number of küçük ünlü uyumu found\n", labialHarmony(words));
	}
	else if (strcmp(choice,"sert ünsüzler") == STRCMP_EQUAL)
	{
		printf("%d number of sert ünsüz found\n", strongConsonant(text));
	}
	else if (strcmp(choice,"yumuþak ünsüzler") == STRCMP_EQUAL)
	{
		printf("%d number of yumuþak ünsüz found\n", softConsonant(text));
	}
	else if (strcmp(choice,"çoðul kelimeler") == STRCMP_EQUAL)
	{
		printf("%d number of çoðul kelime found\n", pluralWords(text));
	}
	else if (strcmp(choice,"kelime sayýsý") == STRCMP_EQUAL)
	{
		printf("%d number of kelime found\n", numberOfWords(words));
	}
	else if (strcmp(choice,"cümle sayýsý") == STRCMP_EQUAL)
	{
		printf("%d number of cümle found\n", numberOfSentences(text));
	}
	else if (strcmp(choice,"harf sayýsý") == STRCMP_EQUAL)
	{
		printf("%d number of harf found\n", numberOfLetters(text));
	}
	else if (strcmp(choice,"çýkýþ") == STRCMP_EQUAL)
	{
		printf("Exiting...\n");
		return FAILURE;
	}
	else
	{
		printf("Wrong typed rule! Please try again!\n");
	}

	return SUCCESS;
}

int connexion(unsigned char words[][SIZE_WORD_LENGTH])
{
	int i=0, count=0;

	for (i=0; words[i+1][0] != '\0' ; ++i)
	{
		if (isInCharset(words[i][strlen(words[i])-1], STR_CONSONANT) &&
			isInCharset(words[i+1][0]               , STR_VOWEL))
		{
			printf("%s %s, ", words[i], words[i+1]);
			++count;
		}
	}

	return count;
}

int palatalHarmony(unsigned char words[][SIZE_WORD_LENGTH])
{
	int i=0, j=0, count=0, state=STATE_PALATAL_ZERO, suitable=SUCCESS;
	
	for (i=0; words[i][0] != '\0' ; ++i)
	{
		state = STATE_PALATAL_ZERO;
		suitable = SUCCESS;
		for(j=0; words[i][j] != '\0'; ++j)
		{
			if (isInCharset(words[i][j], STR_VOWEL_BACK))
			{
				if (state == STATE_PALATAL_FRONT)
				{
					suitable = FAILURE;
					break;
				}
				state = STATE_PALATAL_BACK;
			}
			else if (isInCharset(words[i][j], STR_VOWEL_FRONT))
			{
				if (state == STATE_PALATAL_BACK)
				{
					suitable = FAILURE;
					break;
				}
				state = STATE_PALATAL_FRONT;
			}
		}
		if (suitable == SUCCESS)
		{
			printf("%s, ", words[i]);
			++count;
		}
	}

	return count;
}

int labialHarmony(unsigned char words[][SIZE_WORD_LENGTH])
{
	int i=0, j=0, count=0, state=STATE_LABIAL_ZERO, suitable=SUCCESS;

	for (i=0; words[i][0] != '\0' ; ++i)
	{
		state = STATE_LABIAL_ZERO;
		suitable = SUCCESS;
		for(j=0; words[i][j] != '\0'; ++j)
		{
			if (isInCharset(words[i][j], STR_VOWEL_LARGER_FLAT))
			{
				state = STATE_LABIAL_FLAT;
			}
			else if (isInCharset(words[i][j], STR_VOWEL_NARROW_FLAT))
			{
				if (state == STATE_LABIAL_ROUND)
				{
					suitable = FAILURE;
					break;
				}
				state = STATE_LABIAL_FLAT;
			}
			else if (isInCharset(words[i][j], STR_VOWEL_NARROW_ROUND))
			{
				if (state == STATE_LABIAL_FLAT)
				{
					suitable = FAILURE;
					break;
				}
				state = STATE_LABIAL_ROUND;
			}
			else if (isInCharset(words[i][j], STR_VOWEL_LARGER_ROUND))
			{
				if (state != STATE_LABIAL_ZERO)
				{
					suitable = FAILURE;
					break;
				}
				state = STATE_LABIAL_ROUND;
			}
		}
		if (suitable == SUCCESS)
		{
			printf("%s, ", words[i]);
			++count;
		}
	}

	return count;
}

int strongConsonant(unsigned char text[])
{
	int i=0, count=0;
	
	for(i=0; text[i] != '\0'; ++i)
	{
		if (isInCharset(text[i], STR_CONSONANT_STRONG))
		{
			++count;
		}
	}

	return count;
}

int softConsonant(unsigned char text[])
{
	int i=0, count=0;
	
	for(i=0; text[i] != '\0'; ++i)
	{
		if (isInCharset(text[i], STR_CONSONANT_SOFT))
		{
			++count;
		}
	}

	return count;
}

int pluralWords(unsigned char text[])
{
	int i=0, count_ler=0, count_lar=0;
	unsigned char *ptr_ler = text;
	unsigned char *ptr_lar = text;

	while(ptr_ler = strstr(ptr_ler, STR_PLURAL_1))
	{
		++count_ler;
		++ptr_ler;
	}
	while(ptr_lar = strstr(ptr_lar, STR_PLURAL_2))
	{
		++count_lar;
		++ptr_lar;
	}

	return count_ler + count_lar;
}

int numberOfWords(unsigned char words[][SIZE_WORD_LENGTH])
{
	int i=0, count=0;
	
	for (i=0 ; words[i][0] != '\0' ; ++i)
	{
		++count;
	}

	return count;
}

int numberOfSentences(unsigned char text[])
{
	int i=0, count=0;
	
	for(i=0; text[i] != '\0'; ++i)
	{
		if (isInCharset(text[i], STR_PUNCTUATION))
		{
			++i;
			while (text[i] == '.')
			{
				++i;
			}
			if (text[i] == ' ')
			{
				++count;
			}
			else if (text[i] == '\0')
			{
				++count;
				break;
			}
		}
	}

	return count;
}

int numberOfLetters(unsigned char text[])
{
	int i=0, count=0;
	
	for(i=0; text[i] != '\0'; ++i)
	{
		if (isInCharset(text[i], STR_ALPHABET))
		{
			++count;
		}
	}

	return count;
}

int isInCharset(unsigned char ch, unsigned char charset[])
{
	int i=0;

	for (i=0 ; charset[i] != '\0' ; ++i)
	{
		if (ch == charset[i])
		{
			return SUCCESS;
		}
	}

	return FAILURE;
}

void lowerText(unsigned char text[])
{
	int i=0;
	
	for(i=0; text[i] != '\0'; ++i)
	{
		text[i] = tolower(text[i]);
	}
}

void wordSet(unsigned char text[], unsigned char words[][SIZE_WORD_LENGTH])
{
	int i=0;
	unsigned char *token;
	unsigned char delimiter[2] = " ";
	unsigned char text_copy[SIZE_TEXT_LENGTH];

	strcpy(text_copy, text);
	token = strtok (text_copy, delimiter);
	for (i=0 ; token != NULL ; ++i)
	{
		snprintf (words[i], SIZE_WORD_LENGTH, "%s", token);
		token = strtok (NULL, delimiter);
	}
}
