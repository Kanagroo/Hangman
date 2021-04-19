#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define WORD_MAX 50

int nround(char *word) {
	int wordlen = strlen(word), charCount=0, tries=0,wrongTries=0;
	char wrongChars[26] = {};
	char correctChars[wordlen];

	while (charCount < wordlen) {
		/* Visuals */
		printf("Word: ");
		for (int i=0;i<wordlen;i++)
			if (correctChars[i] == word[i])
				printf("%c", correctChars[i]);
			else
				printf("_");

		printf("\nWrong guesses: ");
		for (int i=0;wrongChars[i]!='\0';i++)
			printf("%c ", wrongChars[i]);

		printf("\nYour guess: ");

		/* Guess */
		char guess;
		char *fndptr, *buf = word;
		fflush(stdin);
		scanf(" %c", &guess);

		fndptr = strchr(word, guess);
		do {
			if (fndptr == NULL) {
				if (strchr(wrongChars, guess) != NULL) {tries--;break;}
				wrongChars[strchr(wrongChars, '\0')-wrongChars] = guess;
				wrongTries ++;
				break;
			}
			else {
				correctChars[fndptr - buf] = guess;
				charCount++;
			}
			word = fndptr + 1;
			fndptr = strchr(word, guess);
		} while (fndptr != NULL);
		word = buf;

		tries ++;
		printf("\x1b[1A\x1b[2K\x1b[2A");
	}
	printf("Word: ");
	for (int i=0;i<wordlen;i++)
		if (correctChars[i] == word[i])
			printf("%c", correctChars[i]);
		else
			printf("_");

	printf("\nWrong guesses: ");
	for (int i=0;wrongChars[i]!='\0';i++)
		printf("%c ", wrongChars[i]);

	printf("\nYou found the word after \033[1m%d guesses \033[0mwith \033[1m%d wrong guesses\n", tries, wrongTries);

	return 0;
}

int main(int argc, char const *argv[]) {
	#if defined WIN32 || defined _WIN32 || defined WIN64 || defined _WIN64
		HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD dwMode = 0;
		GetConsoleMode(hconsole, &dwMode);
		dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		SetConsoleMode(hconsole, dwMode);
	#endif

	int wordc;
	FILE * fp;

	fp = fopen ("words.txt", "r");
	if (fp == NULL) return 0;

	fscanf(fp, "%d", &wordc);
	char words[wordc][WORD_MAX];

	for (int i=0;i<wordc;i++) 
		fscanf(fp, "%s", words[i]);

	fclose(fp);

	srand(time(NULL));

	char *word;
	char ans;
	do {
		printf("\n");

		word = words[rand() % wordc];
		nround(word);

		printf("Play again y/n ?\033[0m ");
		fflush(stdin);
		scanf(" %c", &ans);
	} while (ans == 'y' || ans == 'Y');
	return 0;
}