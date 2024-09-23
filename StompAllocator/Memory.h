#pragma once
#include "Allocator.h"
#include <utility>

template<typename Type, typename ...Args>
//우측값 참조와 완벽한전달
//&의 경우 좌측값 참조로 어느 메모리에할당되어있느 값을 참조
//&&의 경우 우측값 참조로 메모리에할당되어있지않은 임시값
//Args는 가변인자 템플릿이다. 여러 인자를 받기위해 사용
Type* xnew(Args&&...args)
{
	Type* memory = static_cast<Type*>(BaseAllocator::Alloc(sizeof(Type)));
	//여기까지만 한다면 메모리는 할당해주지만 class의경우 생성자와 소멸자가있는데 그것을 자동으로 호출해주진않는다.
	//placement new 라는 것이 메모리를 할당하고 생성자를 호출하여준다.
	//메모리는 할당되어있으니 생성자를 호출해달라고 하는 것
	//인자가 있는 경우의 생성자도 있을 것이다.c++11이전에는 인자의 타입마다 하나하나 선언했었다고한다.(노가다)
	new(memory)Type(forward<Args>(args)...);

	return memory;
};

template<typename Type>
void xdelete(Type* obj)
{
	obj->~Type();
	BaseAllocator::Release(obj);
}

