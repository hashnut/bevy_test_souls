#pragma once

//------------------------------------------------------------------------------
// 게임내에서 동적 싱글톤을 사용하지 않는다.
// 언리얼 고유 싱글톤 객체인 UGameInstance를 활용하여 객체 생성, 소멸처리할것을 권장함
// 해당 싱글톤은 코드에서 사용성(접근성)을 올리는 용처로만 쓰임
//------------------------------------------------------------------------------
template<typename T>
class T1PROJECT_API TGsSingleton
{
protected:
	static T* _instance;

protected:
	virtual ~TGsSingleton() {}

protected:
	static void InitInstance(T* inThis)
	{
		_instance = inThis;
	}
	static void RemoveInstance()
	{
		_instance = nullptr;
	}

public:
	static T* Instance()
	{
		return _instance;
	}
};


