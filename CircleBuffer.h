/************************************************************************/
/* CopyRight @ 2010 by Anyone. No rights is reserved.
/* @Author: ZUHD
/* @Date: 11/11/2010
/* @Description:   ���ƻ�����
/************************************************************************/
#ifndef CIRCLEBUFFER_H
#define CIRCLEBUFFER_H
namespace leogon
{
	template<int nMaxBufferLen = 1024 * 10>
	class CCircleBuffer
	{
	public:
		CCircleBuffer(void)
		{
			m_nCurSize = 0;
			m_nWriteOffset = 0;
			m_nReadOffset = 0;
		}
		~CCircleBuffer(void){}
		bool Push(const char* pData, int nLen)
		{
			if(m_nCurSize + nLen > nMaxBufferLen)
				return false;
			WriteData(pData, nLen);
			return true;
		}
		bool Pop(char* pData, int& nMaxReadLen)
		{
			if(m_nCurSize <= 0)
			{
				pData = 0;
				nMaxBufferLen = 0;
				return false;
			}
			else
			{
				ReadData(pData, nMaxReadLen);
				return true;
			}
		}
		int GetSize() { return m_nCurSize; }
	protected:
		void WriteData(const char* pData, int nLen)
		{
			int nLeft = nMaxBufferLen - m_nWriteOffset;
			if(nLeft < nLen) //����
			{
				memcpy(&m_pData[m_nWriteOffset], pData, nLeft);
				memcpy(&m_pData[0], pData[nLeft], nLen - nLeft);
			}
			else
			{
				memcpy(&m_pData[m_nWriteOffset], pData, nLen);
			}
			m_nWriteOffset = (m_nWriteOffset + nLen) % nMaxBufferLen;
			m_nCurSize += nLen;
		}
		void ReadData(char* pData, int& nMaxReadLen)
		{
			nMaxReadLen = m_nCurSize > nMaxReadLen ? nMaxReadLen : m_nCurSize; //�Ƴ�����
			int nLeft = nMaxBufferLen - nMaxReadLen;
			if(nLeft < nMaxReadLen) //����
			{
				memcpy(pData, &m_pData[m_nReadOffset], nLeft);
				memcpy(&pData[nLeft], &m_pData[0], nMaxReadLen - nLeft);
			}
			else
			{
				memcpy(pData, &m_pData[m_nReadOffset], nMaxBufferLen);
			}
			m_nReadOffset = (m_nReadOffset + nMaxReadLen) % nMaxBufferLen;
			m_nCurSize -= nMaxReadLen;
		}
	private:
		char m_pData[nMaxBufferLen]; ///������
		int m_nWriteOffset; ///д����
		int m_nReadOffset; ///������
		int m_nCurSize; ///��������ǰ���ݴ�С
	};
}

#endif
