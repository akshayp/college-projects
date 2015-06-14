// Perfect.cpp : Defines the entry point for the console application.
// Thread based version to count number of primes.

#include "stdafx.h"
#include "Windows.h"
#include "LIMITS.h"
static HANDLE Thread_semaphore;
long globalSum = 0;
HANDLE ghMutex; 


int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hThread1;
	DWORD dwThread1ID = 0;
	HANDLE hThread2;
	DWORD dwThread2ID = 0;
	HANDLE hThread3;
	DWORD dwThread3ID = 0;
	HANDLE hThread4;
	DWORD dwThread4ID = 0;
	INT nParameter = 1;
	DWORD WINAPI PerfectThread (LPVOID lpArg);
	DWORD time_count;
	unsigned long i, j, sum;
	bool flag;
	printf("Beginning count without threads:");
	printf("\n");
	time_count = GetTickCount();
	sum = 1;
	for (j=3; j<=500000; j++)
	{
		flag = 1;
		for (i=2; i<(j/2); i++)
		{
			if ((j % i) == 0){
				flag = 0;
				break;
			}
		}
		if (flag == 1) sum = sum + 1;
	}
	printf("\n\n %d prime numbers\n\n\r", sum-1);
	time_count = GetTickCount() - time_count;
	printf("\n\n Unthreaded: %d milliseconds elapsed time\n\n\r", time_count);
	Sleep(4000);
	// do it with threads
	printf("Beginning count using threads:");
	ghMutex = CreateMutex( 
        NULL,              // default security attributes
        FALSE,             // initially not owned
        NULL); 
	Thread_semaphore = CreateSemaphore(NULL, 0, 4, TEXT("Thread_Done"));
	time_count = GetTickCount();
	hThread1 = CreateThread( NULL, 0, PerfectThread, (LPVOID)nParameter, 0, &dwThread1ID);
	nParameter++;
	hThread2 = CreateThread( NULL, 0, PerfectThread, (LPVOID)nParameter, 0, &dwThread1ID);
	nParameter++;
	hThread3 = CreateThread( NULL, 0, PerfectThread, (LPVOID)nParameter, 0, &dwThread1ID);
	nParameter++;
	hThread4 = CreateThread( NULL, 0, PerfectThread, (LPVOID)nParameter, 0, &dwThread1ID);
	WaitForSingleObject(Thread_semaphore, INFINITE);
	WaitForSingleObject(Thread_semaphore, INFINITE);
	WaitForSingleObject(Thread_semaphore, INFINITE);
	WaitForSingleObject(Thread_semaphore, INFINITE);
	printf("\n\n %d prime numbers\n\n\r", globalSum-1);
	time_count = GetTickCount() - time_count;
	printf("\n\n Threaded: %d milliseconds elapsed time\n\r", time_count);
	Sleep(10000);
	getch();
	CloseHandle(hThread1);
	CloseHandle(hThread2);
	CloseHandle(hThread3);
	CloseHandle(hThread4);
	CloseHandle(ghMutex);
	return 0;
}
DWORD WINAPI PerfectThread (LPVOID lpArg) {
INT threadnumber = (INT) lpArg;
	unsigned long i, j, sum;
	bool flag;
	sum = 0;
	for (j=1 + threadnumber; j<=500000; j=j+4)
	{
		flag = 1;
		for (i=2; i<(j/2); i++)
		{
			if ((j % i) == 0){
				flag = 0;
				break;
			}
		}
		if (flag == 1) sum = sum + 1;
	}
	           
	WaitForSingleObject( ghMutex, INFINITE);  
	globalSum = globalSum + sum; 
	ReleaseMutex(ghMutex);

	ReleaseSemaphore(Thread_semaphore, 1, NULL);
	return 0;

}
