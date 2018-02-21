
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tchar.h>

#define NO_OF_PROCESS 7 
//Find Largest element of an int array

int findLargest(int salesList[]) {

	int i;
	int n = sizeof(salesList);
	// Initialize maximum element
	int max = salesList[0];

	// Traverse array elements from second and
	// compare every element with current max  
	for (i = 1; i < n; i++)
		if (salesList[i] > max)
			max = salesList[i];

	return max;
}
//Finds indexes of element which is max, I add that function because we cant know whether 1 max or more
int findIndexOfElement(int arrayy[], int indexList[],int element) {
	int i;
	int count = 0;
	
	for (i = 0; i < sizeof(arrayy); i++) {
		if (arrayy[i] == element) {
			indexList[count] = i;
			count++;
		
		}
	}
	return count;
}

//prints total most sold item or items
void printTotalMostSoldItem(int totalMilk,int totalBiscuit,int totalCoke,int totalChips) {
	int indexArray[4];
	int totalSalesList[4];

	totalSalesList[0] = totalMilk;
	totalSalesList[1] = totalBiscuit;
	totalSalesList[2] = totalCoke;
	totalSalesList[3] = totalChips;
	int max = findLargest(totalSalesList);
	int count = findIndexOfElement(totalSalesList, indexArray, max);
	char mostSoldItem[100];
	strcpy(mostSoldItem, "Total Most sold item or items:");
	for (int i = 0; i < sizeof(indexArray); i++) {

		if (indexArray[i] == 0) { strcat(mostSoldItem, "Milk  "); }
		if (indexArray[i] == 1) { strcat(mostSoldItem, "Biscuit  "); }
		if (indexArray[i] == 2) { strcat(mostSoldItem, "Coke  "); }
		if (indexArray[i] == 3) { strcat(mostSoldItem, "Chips  "); }
	}
	strcat(mostSoldItem, "\n");
	printf(mostSoldItem);

	printf("total milk sold: %d\n", totalMilk);
	printf("total biscuit sold: %d\n", totalBiscuit);
	printf("total coke sold: %d\n", totalCoke);
	printf("total chips sold: %d\n", totalChips);
}
//prints most sold item of day
void printMostSoldItemOfDay(int salesList[],char day[]) {

	int indexArrayforDay[4];
	char mostSoldItemOfDay[100];
	
	
	strcpy(mostSoldItemOfDay, "Most Sold Item or items of day ");
	strcat(mostSoldItemOfDay, day);
	strcat(mostSoldItemOfDay, " is:");


	int maxOfday = findLargest(salesList);
	int numberOfMaxes = findIndexOfElement(salesList, indexArrayforDay, maxOfday);


	for (int a = 0; a < numberOfMaxes; a++) {

		if (indexArrayforDay[a] == 0) { strcat(mostSoldItemOfDay, "Milk  "); }
		if (indexArrayforDay[a] == 1) { strcat(mostSoldItemOfDay, "Biscuit  "); }
		if (indexArrayforDay[a] == 2) { strcat(mostSoldItemOfDay, "Coke  "); }
		if (indexArrayforDay[a] == 3) { strcat(mostSoldItemOfDay, "Chips  "); }
	}
	strcat(mostSoldItemOfDay, "\n");
	printf(mostSoldItemOfDay);
}
//prints sold items day by day
void printSoldItemsOfDay(int daySells[],char day) {
	printf("Sales of day is:");
	printf("Milk: %d",daySells[0]);
	printf("Biscuit: %d", daySells[1]);
	printf("Coke: %d", daySells[2]);
	printf("Chips: %d", daySells[3]);
	printf("------------------------");
		
}

int main(int argc, char* argv[])
{

	STARTUPINFO si[NO_OF_PROCESS];
	PROCESS_INFORMATION pi[NO_OF_PROCESS];
	SECURITY_ATTRIBUTES sa[NO_OF_PROCESS];
	HANDLE writePipe[NO_OF_PROCESS],readPipe[NO_OF_PROCESS];
	HANDLE writePipe2[NO_OF_PROCESS], readPipe2[NO_OF_PROCESS];
	HANDLE processHandles[NO_OF_PROCESS];
	char* lpCommandLine[NO_OF_PROCESS] = { "child.exe 4", "child.exe 4", "child.exe 4","child.exe 4","child.exe 4","child.exe 4","child.exe 4" };	
	char *message[1];
	
	DWORD bytesToWrite = 0;
	LPDWORD *bytesWritten = 0;
	int i = 0;


	for (i = 0; i < NO_OF_PROCESS; i++)
	{	
		SecureZeroMemory(&sa[i], sizeof(SECURITY_ATTRIBUTES));
		sa[i].bInheritHandle = TRUE;
		sa[i].lpSecurityDescriptor = NULL;
		sa[i].nLength = sizeof(SECURITY_ATTRIBUTES);
		

		
		//pirpe that sends to child
		if (!CreatePipe(&readPipe[i], &writePipe[i], &sa[i], 0)) //use the default buffer size.
		{
			printf("unable to create pipe\n");
			system("pause");
			exit(0);
		}
		//pipe that takes message from child
		if (!CreatePipe(&readPipe2[i], &writePipe2[i], &sa[i], 0)) //use the default buffer size.
		{
			printf("unable to create pipe\n");
			system("pause");
			exit(0);
		}
		SecureZeroMemory(&pi[i], sizeof(PROCESS_INFORMATION));
		SecureZeroMemory(&si[i], sizeof(STARTUPINFO));
		si[i].cb = sizeof(STARTUPINFO);
		si[i].hStdInput = readPipe[i];
		si[i].hStdOutput = writePipe2[i];
		si[i].hStdError = GetStdHandle(STD_ERROR_HANDLE);
		si[i].dwFlags = STARTF_USESTDHANDLES;		
		if (!CreateProcess(NULL,
			lpCommandLine[i],
			NULL,
			NULL,
			TRUE,
			CREATE_NEW_CONSOLE,
			NULL,
			NULL,
			&si[i],
			&pi[i]))
		{
			printf("unable to create process: %d\n", i);
			system("pause");
			ExitProcess(0);
		}
		else
		{
			processHandles[i] = pi[i].hProcess;
			
		}
	
		//check errors 
		// WriteFile function writes data to the specified file or input / output(I / O) device.
		
		_itoa(i+1,message,10);
		bytesToWrite = strlen(message);
		bytesToWrite++;
		//writes to child
		if (!WriteFile(writePipe[i], message, bytesToWrite, &bytesWritten, NULL))
		{
			printf("unable to write to pipe\n");
			system("pause");
			exit(0);
		}
		
	}


	WaitForMultipleObjects(NO_OF_PROCESS, processHandles, TRUE, INFINITE);
	

	//instances used for printing
	int bytes=0;
	int salesList[4]={ 0,0,0,0 };
	int totalMilk = 0;
	int totalBiscuit = 0;
	int totalCoke = 0;
	int totalChips = 0;	
	
	for (i = 0; i < NO_OF_PROCESS; i++) {
		
		if (!ReadFile(readPipe2[i], salesList, sizeof(salesList), &bytes, NULL)) {
			printf("Unable to read pipe!");
		}
		char *day[1];
		_itoa(i + 1, day, 10);
		printMostSoldItemOfDay(salesList,day);
		
		//print daily sales
		printf("Sales of day %s is:\n",day);
		printf("Milk: %d\n", salesList[0]);
		printf("Biscuit: %d\n", salesList[1]);
		printf("Coke: %d\n", salesList[2]);
		printf("Chips: %d\n", salesList[3]);
		printf("------------------------\n");

		//total sales of day
		totalMilk += salesList[0];
		totalBiscuit += salesList[1];
		totalCoke += salesList[2];
		totalChips += salesList[3];
		



	}
	
	printTotalMostSoldItem(totalMilk, totalBiscuit, totalCoke, totalChips);

	

	for (i = 0; i < NO_OF_PROCESS; i++)
	{
		CloseHandle(pi[i].hThread);
		CloseHandle(pi[i].hProcess);
	}
	
	
	system("pause");
	return 1;
}

