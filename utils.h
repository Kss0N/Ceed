#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdbool.h>




enum rscType {
	rscBUFFER = 0,
	rscFILE = 1,
	rscTHREAD = 2,
};

void cdAddResource(void* rsc, enum rscType eType);
void cdRemoveResource(void* rsc);
void cdRemoveAllResources();

void cdExitUnsuccessful(const char* msg);

/**
	\brief Calls ExitUnsuccessful if condition is false
	@param condition is the condition to be tested
	@param msg is the msg that is printed into the log file at UnsuccesfulExit()

*/
void cdAssert(int condition, const char* msg);


typedef char* buffer; 



char* cdCreateBuffer(size_t initSize);

void cdFreeBuffer(buffer* buffer);

char* cdGetEndOfBuffer(buffer buffer);

bool cdIsAtEndOfBuffer(_In_ buffer buffer, _In_ char* bufferIterator);

size_t cdGetBufferSize(_In_ buffer buffer);

void cdReallocateBuffer(_In_ buffer* pBuffer, size_t newSize);

bool cdCheck_n_reallocBuffer
(buffer* pBuffer, char** pIterator, size_t newSize);

#endif // !UTILS_H