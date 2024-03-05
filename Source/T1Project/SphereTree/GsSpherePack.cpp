#include "GsSpherePack.h"



void FGsSpherePack::Init(int fifosize)
{
	m_nCount = 0;
	m_nStackPointer = 0;
	m_nBottom = 0;
	m_nFifoSize = fifosize;
	m_arrayFifo.SetNum(m_nFifoSize);
}

FGsSphereNode** FGsSpherePack::Push(FGsSphereNode* sphere)
{
	m_nCount++;

	FGsSphereNode** ret = &m_arrayFifo[m_nStackPointer];
	m_arrayFifo[m_nStackPointer] = sphere;

	m_nStackPointer++;

	//Recycle
	if (m_nStackPointer == m_nFifoSize)
	{
		m_nStackPointer = 0;
	}
	return ret;
}

FGsSphereNode* FGsSpherePack::Pop(void)
{
	while (m_nStackPointer != m_nBottom)
	{
		m_nCount--;

		FGsSphereNode* ret = m_arrayFifo[m_nBottom];

		m_nBottom++;

		//Recycle
		if (m_nBottom == m_nFifoSize)
		{
			m_nBottom = 0;
		}

		if (nullptr != ret)
		{
			return ret;
		}
	}
	return nullptr;
}


