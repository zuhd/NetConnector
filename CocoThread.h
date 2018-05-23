#pragma once
#include "Thread.h"
class CCocoThread : public leogon::CThread
{
public:
	CCocoThread();
	virtual ~CCocoThread();
	virtual void Run();
};