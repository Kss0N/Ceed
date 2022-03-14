#include "utils.h"

#include <stdbool.h>

#include <stdio.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <sysinfoapi.h>







void cdAssert(int condition, const char* msg) {
	if (condition != false) return;
	cdExitUnsuccessful(msg);
}



#define internal_buffer_alignment 32

buffer cdCreateBuffer(size_t initBufSize) {
	size_t size = initBufSize * sizeof(char) + sizeof(char*);
	char* buffer = (char*)_aligned_malloc(size, internal_buffer_alignment);
	cdAssert(buffer != NULL, "unable to allocate buffer");

	*(char**)buffer = buffer + size + (internal_buffer_alignment-size%internal_buffer_alignment);
	buffer += sizeof(char*);
	cdAddResource(buffer, rscBUFFER);
	return buffer;
}
void cdFreeBuffer(buffer* buffer) {
	char* begin = *buffer - sizeof(char*);
	_aligned_free(begin);
	*buffer = NULL;
}

char* cdGetEndOfBuffer(buffer buffer) {
	return (char*)*(buffer - sizeof(char*));
}



size_t cdGetBufferSize(_In_ buffer buffer) {
	return cdGetEndOfBuffer(buffer) - buffer;
}
bool cdIsAtEndOfBuffer(_In_ buffer buffer, _In_ char* bufferIterator) {
	return cdGetEndOfBuffer(buffer) - bufferIterator == 0;
}

void cdReallocateBuffer(_In_ buffer* pBuffer, size_t newSize) {
	size_t size		= cdGetBufferSize(*pBuffer) + newSize;
	buffer newBuffer= cdCreateBuffer(size);
	char* itOld		= *pBuffer;
	char* itNew		= newBuffer;
	while (	!cdIsAtEndOfBuffer(newBuffer,	itNew) 
		&&  !cdIsAtEndOfBuffer(*pBuffer,	itOld)){
		*itNew++ = *itOld++;
	}
	cdFreeBuffer(pBuffer);
	*pBuffer = newBuffer;
}

/*Check if a realloc is needed*/
bool cdCheck_n_reallocBuffer
(buffer* pBuffer, char** pIterator, size_t newSize)
{
	size_t offset = *pIterator - *pBuffer;
	size_t oldSize = cdGetBufferSize(*pBuffer);

	if (offset >= oldSize)
	{
		//idk 256 or 128 don't want to do realloc too often but don't want to
		//waste memory either
		//On second note I do realize that I don't live in the 80s and 
		//so I decided to go with 512 
		cdReallocateBuffer(pBuffer, newSize);
		*pIterator = *pBuffer + offset;
		return TRUE;
	}
	return FALSE;
}




struct rscNode {
	struct rscNode* next;
	struct rscNode* prev;
	void* id;
	char type;
};

//Really hasn't got anything of value except a rscNode* to the actual first node
struct rscNode gFrontRscNode = {
	.next = NULL
};

void cdAddResource(void* rsc, enum rscType eType) {
	struct rscNode* tail;
	for (tail = &gFrontRscNode; tail->next != NULL; tail = tail->next) {}
	struct rscNode* node = malloc(sizeof(struct rscNode));
	node->next = NULL;
	node->id = rsc;
	node->type = eType;
}

void freeResource(struct rscNode* node) {
	switch (node->type)
	{
	case rscBUFFER:
		cdFreeBuffer(&((buffer)node->id));
	case rscFILE:
		fclose((FILE*)node->id);
	default:
		break;
	}	
	free(node);
}

void cdRemoveResource(void* rsc) {
	struct rscNode* node;
	for (node = gFrontRscNode.next; node->next != NULL && node->id != rsc; node = node->next);
	if (node->next == NULL && node->id != rsc) return; /*Doesn't exist, there's nothing to remove*/
	node->prev->next = node->next;
	freeResource(node);
}
void cdRemoveAllResources() {
	struct rscNode* node = gFrontRscNode.next;
	while (node != NULL) {
		struct rscNode* next = node->next;
		freeResource(node);
		node = next; 
	}
}

void cdExitUnsuccessful(const char* msg) {
	FILE* pFile = NULL;
	fopen_s(&pFile, "logFile.txt", "a+");
	if (pFile) {
		SYSTEMTIME time;
		GetSystemTime(&time);
		fprintf(pFile, "[%d/%d/%d](%d:%d) ", 
			time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute);
		fprintf(pFile, msg);
		fclose(pFile);
	}
	cdRemoveAllResources();
	exit(1);
}