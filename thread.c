

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <threadpoolapiset.h>


struct myThreadArgument
{
	INT32 a;
};

DWORD WINAPI myThreadProcedure(LPVOID lpParams) {
	int a = ((struct myThreadArgument*)lpParams)->a;

}

int test()
{
	struct myThreadArgument args = {
		.a = 5
	};
	int threadId;
	SECURITY_DESCRIPTOR secDesc;
	InitializeSecurityDescriptor(&secDesc, SECURITY_DESCRIPTOR_REVISION);
	SECURITY_ATTRIBUTES securityAttributes = {
		.nLength = sizeof(SECURITY_ATTRIBUTES),
		.lpSecurityDescriptor = &secDesc, //Security descriptor
		.bInheritHandle = TRUE //Handle is inherited to child process
	};
	SIZE_T stackSize = 0; //Default is thus chosen, which is 1 MB = 2^20 bytes.
	DWORD creationFlags = CREATE_SUSPENDED; //It's in a suspended ( sleeping state)
	HANDLE hThread = CreateThread(&securityAttributes, stackSize,
		myThreadProcedure, &args, creationFlags, &threadId);
	
}

/*
	Mutex
*/

int lock = 0;
