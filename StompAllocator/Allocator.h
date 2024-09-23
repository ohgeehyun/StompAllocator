#pragma once
#include<iostream>
/*--------------------------
		Base Allocator
----------------------------*/

using int32 = int32_t;
using namespace std;;

class BaseAllocator
{
public:
	static void* Alloc(int32 size);
	static void Release(void* ptr);


private:
};

class StompAllocator
{
	//�ּ� ����������
	enum { PAGE_SIZE = 0x1000 };

public:
	static void* Alloc(int32 size);
	static void  Release(void* ptr);
};
