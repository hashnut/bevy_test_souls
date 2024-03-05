#pragma once
//------------------------------------------------------------------------------
// 동적생성 싱글톤 -> 생성시점 제어판단을 호출시점에서 제어
// 해당 코드를 로직에서 사용하는것은 추천하지 않습니다.
// 언리얼객체에서 유일한 싱글톤인 게임인스턴스를 통한 싱글톤제어를 추천함(GsSingleton.h 참고)
//------------------------------------------------------------------------------
template<typename T>
class T1PROJECT_API TGsSingletonDynmaic
{
	static T* _instance;
public:
	static T* GetInstance()
	{
		if (nullptr == _instance)
		{
			_instance = new T();
		}
		return _instance;
	}
	static void RemoveInstance()
	{
		if (nullptr != _instance)
		{
			delete _instance;
			_instance = nullptr;
		}
	}
};
