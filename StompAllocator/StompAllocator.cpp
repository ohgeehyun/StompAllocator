#include <iostream>
#include <thread>
#include <atomic>
#include "Macro.h"
#include <windows.h>
#include "Memory.h"




void* operator new[](size_t size) {
    cout << "new[]! " << size << endl;
    void* ptr = malloc(size);
    return ptr;
}

void operator delete[](void* ptr) {
    cout << "delete[]!" << endl;
    free(ptr);
}

class Knight
{
public:
    Knight() { cout << "knight()" << endl; };
    ~Knight() { cout << "~Knight" << endl; };
    Knight(int32 hp) : _hp(hp) { cout << "Knight(hp)" << endl; };

    int32_t _hp = 100;
    int32_t _mp = 100;

private:
};


int main()
{
    //실질적으로 우리가 할당하는 메모리는 가상 메모리이다.
    //os에서 여러개의 프로세스를 메모리에 올려 실행하고 있을 것이고 만약 그럼 다른 프로세스에서 다른 프로세스의 메모리주소를 참조하면 값이 바뀌겟는가?
    //그렇기에 우리는 메모리의 부분에 가상 메모리라는부분을 올려서 맵핑하여 사용하는 것 이다.
    //일반적으로 우리가 int32데이터를 메모리에할당하면 4byte이다. but os에서 메모리를 할당하는 최소단위(페이지)가 있고 이것을 알고리즘을 통해 최적화하였다고 할 수 있다.

    SYSTEM_INFO info; //시스템에대한 정보를 받아옴
    GetSystemInfo(&info);

    info.dwPageSize;// 4KB 0x1000 왠만해선 4kB일것 운영체제가 메모리를 관리하는 가장 작은 단위
    info.dwAllocationGranularity;//64kb 0x10000 프로세스가 메모리를 요청할때, 운영체제가 실제로 할당해주는 최소 단위

    //표준 c++ 에서 new 또는 delete는 운영체제에게 바로 메모리할당 해제 명령을 하는 것이 아니다.
    //각 운영체제 linux , window에 맞게 내부적으로 메모리가 할당되고 해제되게 되어있을뿐

    //운영체제에게 바로 메모리를 할당하는 명령어 
    //인자는 자세히궁금하면 구글에 해당 함수 치면 바로 공식문서 나온다.3번째 4번째 인자
    //3번의 경우 메모리 할당의 유형 ,4번의 경우 할당할 페이지 영역에 대한 메모리 보호
    int* test = (int*)VirtualAlloc(NULL, 4, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    *test = 100;
    VirtualFree(test, 0, MEM_RELEASE);

    //굳이 new delete가 있는데 쓰는 이유? new랑 delete는 바로 해제되는 것이아니다(자기 딴에선 어느정도 메모리를 유동적으로 관리하기 위해서 인듯). 쓰레기값이 덮어저있다.
    //그렇기때문에 해당객체에 nullptr을 넣거나 vector를 clear()하더라도 다른곳에서 참조를 하고있고 거기에 write라도하면 큰일이난다. 전체 코드가 위험해짐 USE-After-free,댕글리 포인터 라고 도 한다.
    //그러나 virtualAlloc virtualFree를 활용하면 바로 메모리영역을 없애버려서 바로 crash가 나오게 가능.
    //비록 os에게 바로 요청을 하는 것이라 비용이들겠지만 바로 메모리 오염가능성이있다는것을 파악가능
    Knight* knight = xnew<Knight>(100);
    Xrelease(knight);
    knight->_hp = 100;

}