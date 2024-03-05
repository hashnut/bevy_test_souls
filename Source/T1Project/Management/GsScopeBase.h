// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Classes/GsManager.h"
#include "GsScopeBase.generated.h"

/**
 *  생명주기를 관리하는 기본 클래스
 */
UCLASS()
class T1PROJECT_API UGsScopeBase : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(Transient)
	TArray<UObject*>	_uObjectManagerList;	// UObject 상속객체 매니저

	TArray<IGsManager*>	_fObjectManagerList;	// UObject 상속하지 않은(delete 처리 필요한) 매니저

protected:
	TArray<IGsManager*>	_managerList;			// 모든 매니저(검색용)

	TMap<int32, int32> _managerToType;	// 매니저와 타입 연동(GameManager 의 Index -> UGsScopeGame::EManagerType 으로 캐스팅해 주어야 하는 숫자)

	// fType이 먼저 선언되어야 할수 밖에 없는 경우(Enum 순서가 결국 초기화 순서)에 인덱스 맞추기위한 플레그
	bool				_uTypeFirstOrder = true;
	int					_fTypeCount = 0;

public:
	virtual void Create() {};

	// 매니저 생성, 초기화
	virtual void Init();

	// 리소스 다운로드 후 초기화해야하는 것들
	// 주의: ScopeGame은 리소스 다운이 된 시점에 생성되므로, Init 후 바로 호출
	virtual void InitAfterResourceDownload();

	// 매니져 Tick 처리
	void Update(float elapsed);

	// 매니저 삭제
	virtual void Shutdown();
	virtual void Finalize();

	// 2021/06/29 PKT - App 종료( UGsGameInstance::QuitGame 함수 호출 )
	virtual void QuitGame() final;

public:
	// UObject Manager Type(InType : 0 ~ ObjectMgrMax)
	template <typename T = UObject>
	T* GetUObjectManager(uint8 InType) const
	{
		if (false == _uTypeFirstOrder)	// 선정렬이 fType이라면
		{
			InType -= _fTypeCount;
		}
		if (InType < _uObjectManagerList.Num())
		{
			return Cast<T>(_uObjectManagerList[InType]);
		}
		return nullptr;
	}
	
	// Not UObject Manager Type(InType : NotObjectMgrStart ~ NotObjectMgrMax)
	template <typename T>
	T* GetFObjectManager(uint8 InMgrType) const
	{
		static_assert(TIsDerivedFrom<T, IGsManager>::IsDerived, "T must be derived from IGsManager");

		if (IGsManager* manager = GetManager(InMgrType))
		{
			return static_cast<T*>(manager);
		}

		return nullptr;
	}

protected:
	// ObjectManager : 0 ~ ObjectMgrMax
	// NotObjectManager : NotObjectMgrStart ~ NotObjectMgrMax
	IGsManager* GetManager(uint8 InType) const
	{
		if (InType < _managerList.Num())
		{
			return _managerList[InType];
		}

		return nullptr;
	}

	template <typename T>
	T* CreateUObjectManager(UObject* InOuter, int32 InManagerTypeIndex = -1)
	{
		T* inst = NewObject<T>(InOuter);
		check(inst);

		int32 addedIndex = _managerList.AddUnique(inst);
		check(-1 != addedIndex);

		_uObjectManagerList.Add(inst);

		if (InManagerTypeIndex != -1)
			_managerToType.Emplace(addedIndex, InManagerTypeIndex);

		return inst;
	}

	template <typename T>
	T* CreateFObjectManager(int32 InManagerTypeIndex = -1)
	{
		T* inst = new T();
		check(inst);

		int32 addedIndex = _managerList.AddUnique(inst);
		check(-1 != addedIndex);
		
		_fObjectManagerList.Add(inst);

		if (InManagerTypeIndex != -1)
			_managerToType.Emplace(addedIndex, InManagerTypeIndex);

		return inst;
	}
};
