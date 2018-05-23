#ifndef CBUFFER_H
#define CBUFFER_H
#include "GlobalDefine.h"
#include "Locker.h"
class CBuffer  
{
// Attributes
protected:
	char*	m_pBase;
	char*	m_pPtr;
	uint	m_nSize;

// Methods
protected:
	uint ReAllocateBuffer(uint nRequestedSize);
	uint DeAllocateBuffer(uint nRequestedSize);
	uint GetMemSize();
public:
	void ClearBuffer();

	uint Read(char* pData, uint nSize);
	bool Write(char* pData, uint nSize);
	uint GetBufferLen();

	char* GetBuffer(uint nPos=0);

	CBuffer();
	virtual ~CBuffer();

private:
	leogon::CLocker m_lock;
};
#endif