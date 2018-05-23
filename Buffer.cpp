// Buffer.cpp: implementation of the CBuffer class.
//
//////////////////////////////////////////////////////////////////////
#include "Buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	CBuffer
// 
// DESCRIPTION:	Constructs the buffer with a default size
// 
// RETURNS:		
// 
// NOTES:	
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       270400		1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
CBuffer::CBuffer()
{
	// Initial size
	m_nSize = 0;

	m_pPtr = m_pBase = NULL;
}

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	~CBuffer
// 
// DESCRIPTION:	Deallocates the buffer
// 
// RETURNS:		
// 
// NOTES:	
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       270400		1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
CBuffer::~CBuffer()
{
	if (m_pBase != NULL)
	{
		free(m_pBase);
		m_pBase = NULL;
	}
}
	

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	Write
// 
// DESCRIPTION:	Writes data into the buffer
// 
// RETURNS:		
// 
// NOTES:	
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       270400		1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
bool CBuffer::Write(char* pData, uint nSize)
{
	if (ReAllocateBuffer(nSize + GetBufferLen()) == -1)
	{
		return false;
	}

	memcpy(m_pPtr, pData, nSize);

	// Advance Pointer
	m_pPtr+=nSize;
	return true;
}

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	Read
// 
// DESCRIPTION:	Reads data from the buffer and deletes what it reads
// 
// RETURNS:		
// 
// NOTES:	
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       270400		1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
uint CBuffer::Read(char* pData, uint nSize)
{
	// Trying to byte off more than ya can chew - eh?
	if (nSize > GetMemSize())
	{
		return 0;
	}

	// all that we have 
	if (nSize > GetBufferLen())
		nSize = GetBufferLen();

		
	if (nSize)
	{
		// Copy over required amount and its not up to us
		// to terminate the buffer - got that!!!
		memcpy(pData, m_pBase, nSize);
		
		// Slide the buffer back - like sinking the data
		memmove(m_pBase, m_pBase + nSize, GetMemSize() - nSize);
		m_pPtr -= nSize;
	}
		
	DeAllocateBuffer(GetBufferLen());
	return nSize;
}

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	GetMemSize
// 
// DESCRIPTION:	Returns the phyical memory allocated to the buffer
// 
// RETURNS:		
// 
// NOTES:	
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       270400		1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
uint CBuffer::GetMemSize() 
{
	return m_nSize;
}

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	GetBufferLen
// 
// DESCRIPTION:	Get the buffer 'data' length
// 
// RETURNS:		
// 
// NOTES:	
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       270400		1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
uint CBuffer::GetBufferLen() 
{
	if (m_pBase == NULL)
		return 0;

	int nSize = 
		m_pPtr - m_pBase;
	return nSize;
}

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	ReAllocateBuffer
// 
// DESCRIPTION:	ReAllocateBuffer the Buffer to the requested size
// 
// RETURNS:		
// 
// NOTES:	
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       270400		1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
uint CBuffer::ReAllocateBuffer(uint nRequestedSize)
{
	if (nRequestedSize < GetMemSize())
		return 0;

	// Allocate new size
	uint nNewSize = (uint) ceil(nRequestedSize / 1024.0) * 1024;

	// New Copy Data Over
	char* pNewBuffer = (char*)malloc(nNewSize);
	if (pNewBuffer == NULL)
		return -1;

	uint nBufferLen = GetBufferLen();
	memcpy(pNewBuffer, m_pBase, nBufferLen);

	if (m_pBase)
		free(m_pBase);

	// Hand over the pointer
	m_pBase = pNewBuffer;

	// Realign position pointer
	m_pPtr = m_pBase + nBufferLen;

	m_nSize = nNewSize;

	return m_nSize;
}

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	DeAllocateBuffer
// 
// DESCRIPTION:	DeAllocates the Buffer to the requested size
// 
// RETURNS:		
// 
// NOTES:	
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       270400		1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
uint CBuffer::DeAllocateBuffer(uint nRequestedSize)
{
	if (nRequestedSize < GetBufferLen())
		return 0;

	// Allocate new size
	uint nNewSize = (uint) ceil(nRequestedSize / 1024.0) * 1024;

	if (nNewSize < GetMemSize())
		return 0;

	// New Copy Data Over
	char* pNewBuffer = (char*)malloc(nNewSize);

	uint nBufferLen = GetBufferLen();
	memcpy(pNewBuffer, m_pBase, nBufferLen);
	free(m_pBase);

	// Hand over the pointer
	m_pBase = pNewBuffer;

	// Realign position pointer
	m_pPtr = m_pBase + nBufferLen;

	m_nSize = nNewSize;

	return m_nSize;
}

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	ClearBuffer
// 
// DESCRIPTION:	Clears/Resets the buffer
// 
// RETURNS:	
// 
// NOTES:	
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       270400		1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
void CBuffer::ClearBuffer()
{
	// Force the buffer to be empty
	m_pPtr = m_pBase;

	DeAllocateBuffer(1024);
}


////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	GetBuffer
// 
// DESCRIPTION:	Returns a pointer to the physical memory determined by the offset
// 
// RETURNS:	
// 
// NOTES:	
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       270400		1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
char* CBuffer::GetBuffer(uint nPos)
{
	return m_pBase+nPos;
}