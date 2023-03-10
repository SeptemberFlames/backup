
#ifndef LOCKER_HEAD_FILE
#define LOCKER_HEAD_FILE

#pragma once

#include "stdafx.h"
#include <atomic>

class CSpinMutex 
{
public:
	CSpinMutex();
	~CSpinMutex();

public:
	void lock();
	void unlock();

private:
	std::atomic_flag flag;
};

#endif