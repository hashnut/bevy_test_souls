#pragma once

#include "T1Project.h"

//----------------------------------------------------------------
// 매니져 클래스 기본인터페이스
// 추상을 쓰지 않은 이유는, 템플릿 + 추상클래스 조합이 모바일빌드에서 에러 뿜뿜
//----------------------------------------------------------------
class IGsManager
{
public:
	virtual ~IGsManager() {}

public:
	virtual void Initialize() {};
	virtual void Finalize() {};
	virtual void Shutdown() { Finalize(); }

	virtual void Update(float inTick) {};
	
	// 리소스 다운로드 후 초기화해야하는 것들
	// 주의: ScopeGame은 리소스 다운이 된 시점에 생성되므로, Init 후 바로 호출
	virtual void InitializeAfterResourceDownload() {};
};