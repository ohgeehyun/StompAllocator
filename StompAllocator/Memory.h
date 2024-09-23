#pragma once
#include "Allocator.h"
#include <utility>

template<typename Type, typename ...Args>
//������ ������ �Ϻ�������
//&�� ��� ������ ������ ��� �޸𸮿��Ҵ�Ǿ��ִ� ���� ����
//&&�� ��� ������ ������ �޸𸮿��Ҵ�Ǿ��������� �ӽð�
//Args�� �������� ���ø��̴�. ���� ���ڸ� �ޱ����� ���
Type* xnew(Args&&...args)
{
	Type* memory = static_cast<Type*>(BaseAllocator::Alloc(sizeof(Type)));
	//��������� �Ѵٸ� �޸𸮴� �Ҵ��������� class�ǰ�� �����ڿ� �Ҹ��ڰ��ִµ� �װ��� �ڵ����� ȣ���������ʴ´�.
	//placement new ��� ���� �޸𸮸� �Ҵ��ϰ� �����ڸ� ȣ���Ͽ��ش�.
	//�޸𸮴� �Ҵ�Ǿ������� �����ڸ� ȣ���ش޶�� �ϴ� ��
	//���ڰ� �ִ� ����� �����ڵ� ���� ���̴�.c++11�������� ������ Ÿ�Ը��� �ϳ��ϳ� �����߾��ٰ��Ѵ�.(�밡��)
	new(memory)Type(forward<Args>(args)...);

	return memory;
};

template<typename Type>
void xdelete(Type* obj)
{
	obj->~Type();
	BaseAllocator::Release(obj);
}

