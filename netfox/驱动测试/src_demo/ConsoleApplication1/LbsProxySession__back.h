#pragma once

#include "stdafx.h"
#include <mongoc.h>
#include <stdio.h>
#include <stdlib.h>
#include <bson.h>
#include <bcon.h>
#include <time.h>
#include <iostream>

class CLbsProxySession
{
	//构造函数
	CLbsProxySession();
	//析构函数
	virtual ~CLbsProxySession();

public:
	bool Init(const loss::CJsonObject& conf);

protected:
private:
};