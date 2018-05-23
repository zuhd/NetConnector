#include "CocoSocket.h"
#include "GlobalDefine.h"
#include "Utilty.h"
#include "CocoThread.h"


CCocoThread::CCocoThread()
{

}

CCocoThread::~CCocoThread()
{

}

void CCocoThread::Run()
{
	uint uiLastTime = leogon::GetCurTime();
	uint uiCurTime;
	int uiElapse = 0;
	while(true)
	{
		uiCurTime = leogon::GetCurTime();		
		uiElapse = uiCurTime - uiLastTime;
		if (uiElapse > 200)
		{
			CocoSocketReactor::getInstance().update(uiElapse);
		}
		p_sleep(50);
	}
}

