#include "SpinMutex.h"

CSpinMutex::CSpinMutex()
{
	flag.clear(std::memory_order_release);
}

CSpinMutex::~CSpinMutex()
{

}

void CSpinMutex::lock()
{
	while (flag.test_and_set(std::memory_order_acquire));
}

void CSpinMutex::unlock()
{
	flag.clear(std::memory_order_release);
}

