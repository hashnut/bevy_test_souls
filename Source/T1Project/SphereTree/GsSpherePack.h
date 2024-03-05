#pragma once


class FGsSphereNode;

// 노드를 관리하기위한 선입선출 스텍
// 초기화를 하지 않고 인덱스 (stackPointer 와 bottom) 으로 Recycle 처리함

class FGsSpherePack
{
private:
	int				m_nCount;				// 입력된 갯수
	int				m_nStackPointer;		// 인덱서 (Up)
	int				m_nBottom;				// 바닥 인덱스 (Bottom)
	int				m_nFifoSize;			// 큐 싸이즈
	TArray<FGsSphereNode*> m_arrayFifo;				// 배열

public:
	void Init(int fifosize);

	// return : 디버깅을위해 저장하고 있는 저장주소 리턴
	FGsSphereNode** Push(FGsSphereNode* sphere);

	// return : 저장된 값
	FGsSphereNode*	Pop(void);

public:
	int GetCount(void) const { return m_nCount; }
};