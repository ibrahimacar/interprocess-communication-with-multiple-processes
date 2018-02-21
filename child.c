
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
//struct of threats
typedef struct
{
	char product [10];
	int threadNo;
	int sum;
}THREAD_PARAMETERS;

DWORD WINAPI threadWork(LPVOID parameters);
//day defined here because we need each day to reach it
char day[1];


//read function from file

int read(char day[], char product[])
{
	printf("product is: %s\n", product);
	//Reads from my path, Needs to change
	static const char filename[] = "C:\\Users\\Asus\\Desktop\\2017-2018\\OS\\Homework #1-20171107\\market.txt";
	FILE *file = fopen(filename, "r");
	char start[50];
	char end[50];


	strcpy(start, "#START DAY ");
	strcat(start, day);
	strcat(start, "#");
	strcpy(end, "#END DAY ");
	strcat(end, day);
	strcat(end, "#");
	int counter=0;
	if (file != NULL)
	{

		char line[128]; /* or other suitable maximum line size */
		while (fgets(line, sizeof line, file) != NULL) /* read a line */
		{
			if (strstr(line, start) != NULL) {
				while (fgets(line, sizeof line, file) != NULL && strstr(line, end) == NULL) {

					if (strstr(line, product) != NULL) {
						counter += 1;
					}
				}
			}
		}
		fclose(file);

	}
	else
	{
		perror(filename); /* for file open error */
	}
	return counter;
}



DWORD WINAPI threadWork(LPVOID parameters)
{
	THREAD_PARAMETERS* param = (THREAD_PARAMETERS*)parameters;

	param->sum =read(day,param->product);

	return 1;

}
int main(int argc, char* argv[])
{

	HANDLE read,write;
	printf("aaxxxd");
	read= GetStdHandle(STD_INPUT_HANDLE);
	write = GetStdHandle(STD_OUTPUT_HANDLE);
	printf("asdad");
	HANDLE* handles;
	THREAD_PARAMETERS* lpParameter;
	int numberOfBytesToRead=0;
	int *bytesWritten = 0;
	int threadCount = 0;
	int* threadID;
	int i = 0;
	int totalMilk = 0;
	int totalCoke = 0;
	int totalChips = 0;
	int totalBiscuit = 0;
	int totalSales[4];
	//Check arguments for error
	if (argc != 2)
	{
		printf("error in child process...now exiting %s\n", argv[0]);
		system("pause");
		exit(0);
	}
	//read thread count
	threadCount = atoi(argv[1]);

	//allocate memory
	handles = malloc(sizeof(HANDLE)* threadCount);
	lpParameter = malloc(sizeof(THREAD_PARAMETERS)* threadCount);
	threadID = malloc(sizeof(int)* threadCount);
	numberOfBytesToRead = strlen(day);
	numberOfBytesToRead++;

	//read message from pipe
	printf("asdada");
	if (!ReadFile(read, day, numberOfBytesToRead, &bytesWritten, 0))

	{

		printf("cant read");

	}
	printf("asdad");


	printf("message from parent process: %s\n", day);
	//Assign products to threads
	strcpy(lpParameter[0].product, "MILK");
	strcpy(lpParameter[1].product, "BISCUIT");
	strcpy(lpParameter[2].product, "COKE");
	strcpy(lpParameter[3].product, "CHIPS");

	for (i = 0; i < threadCount; i++)
	{
		lpParameter[i].threadNo = i + 1;
		handles[i] = CreateThread(NULL, 0, threadWork, &lpParameter[i], 0, &threadID[i]);

		//check errors in creation
		if (handles[i] == INVALID_HANDLE_VALUE)
		{
			printf("error when creating thread\n");
			system("pause");
			exit(0);
		}
		printf("thread %d has started working with id: %d\n", i + 1, threadID[i]);

	}

	WaitForMultipleObjects(threadCount, handles, TRUE, INFINITE);

	//total products
	totalMilk = totalMilk + (int)lpParameter[0].sum;
	totalBiscuit = totalBiscuit + (int)lpParameter[1].sum;
	totalCoke = totalCoke + (int)lpParameter[2].sum;
	totalChips = totalChips + (int)lpParameter[3].sum;

	printf("total Milk sold: %d\n", totalMilk);
	printf("total Biscuit sold: %d\n", totalBiscuit);
	printf("total Coke sold: %d\n", totalCoke);
	printf("total Chips sold: %d\n", totalChips);

	//Create Message
	totalSales[0] = totalMilk;
	totalSales[1] = totalBiscuit;
	totalSales[2] = totalCoke;
	totalSales[3] = totalChips;

	int bytes = 0;
	int den[4];
	den[0] = totalSales[0];
	den[1] = totalSales[1];
	den[2] = totalSales[2];
	den[3] = totalSales[3];

	//send message to parent
	if (!WriteFile(write, den, sizeof(den), &bytes, NULL)) {
		printf("Write error!\n");
	}


	for (i = 0; i < threadCount; i++) {
		CloseHandle(handles[i]);
	}
	//free handles ,parameters
	free(handles);
	free(lpParameter);
	free(threadID);
	system("pause");
	return 1;
}
