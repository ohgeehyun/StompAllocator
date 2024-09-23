#include "Allocator.h"
#include <windows.h>

using int8 = int8_t;
using int64 = int64_t;

void* BaseAllocator::Alloc(int32 size)
{
	return malloc(size);
}

void BaseAllocator::Release(void* ptr)
{
	free(ptr);
}

void* StompAllocator::Alloc(int32 size)
{
	//4096+4095 / 4096�� �ϸ� 1.9999�� �����µ� ������ ��¥��  �����̴� 1�� ���´�.
	//�ݿø� �ڵ��ε� �˰��������� ���ֳ����� ã�ƺ���
	const int64 pageCount = (size + PAGE_SIZE - 1) / PAGE_SIZE;
	const int64 dataOffset = pageCount * PAGE_SIZE - size;
	//�޸� ����÷ο�
	void* baseAddress = VirtualAlloc(NULL, pageCount * PAGE_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	return static_cast<void*>(static_cast<int8*>(baseAddress) + dataOffset);
}
void StompAllocator::Release(void* ptr)
{
	const int64 address = reinterpret_cast<int64>(ptr);
	const int64 baseAddress = address - (address % PAGE_SIZE);
	VirtualFree(reinterpret_cast<void*>(baseAddress), 0, MEM_RELEASE);
}