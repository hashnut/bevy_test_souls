#pragma once
#include "CoreMinimal.h"
#include "GsScopeGlobal.h"
#include "GsScopeLobby.h"
#include "GsScopeGame.h"
#include "Classes/GsSingleton.h"

#include "GsScopeHolder.generated.h"

class FGsMessageHolder;
class FGsFPSAnalyzer;

/*	
	매니지먼트 클래스들은 Global / Lobby / Game 영역으로 나누어 생명주기를 관리한다.
	GsScopeBase (생명주기 관리 기반 클래스) 매니지먼트 클래스들을 담고있음
*/

// 늦은 처리 펑션 타입
enum class EGsPostUpdateFuncType :uint8
{
	PROCESS_LOAD_TO_PO_FADE_OUT,		// 워프 카메라 연출 후 페이드 -> 모든 오브젝트 삭제, 레벨 로드 
};

UCLASS()
class T1PROJECT_API UGsScopeHolder : public UObject,	
	public TGsSingleton<UGsScopeHolder>
{
	GENERATED_BODY()

protected:
	UPROPERTY(Transient)
	UGsScopeGlobal*				_global = nullptr;

	UPROPERTY(Transient)
	UGsScopeLobby*				_lobby = nullptr;

	UPROPERTY(Transient)
	UGsScopeGame*				_game = nullptr;

private:
	FGsMessageHolder*			_message = nullptr;

private:
	FGsFPSAnalyzer*				_fpsAnalyzer = nullptr;

private:
	TArray<class IGsMessageHandler*> _messageHandlers;		// 
	// 늦은 처리 함수들
	TMap<EGsPostUpdateFuncType, TFunction<void()>> _mapPostUpdateFunc;

private:
	float						_testKeyBoardHeight = 0.0f;
	float						_testcontentScaleFactor = 0.0f;
	FVector						_testTouchPosition;

private: 
	static bool					_isGlobalCreated;
	static bool					_isLobbyCreated;
	static bool					_isGameCreated;

	bool _isGlobalInitAfterResourceDownload = false;
	bool _isLobbyInitAfterResourceDownload = false;

public:
	void Init();
	void Update(float inTick);
	void Shutdown();

public:
	void StartPlayGameModeLobby();
	void BeginDestroyGameModeLobby();

	void StartPlayGameModeWorld();
	void BeginDestroyGameModeWorld();

public:
	void ShowFPS();
	void ResetFPS();
	// 늦은 업데이트 처리 함수 추가
	void AddPostUpdateFunc(EGsPostUpdateFuncType In_type, TFunction<void()> In_func);

public:
	void OnDrawDebug(class AHUD* HUD, class UCanvas* Canvas, const class FDebugDisplayInfo& DisplayInfo, float& YL, float& YPos);
	void CallbackIOSKeyboardHeight(float InHeight);

	void OnTouchPress(ETouchIndex::Type FingerIndex, FVector Location); // Location Z == 1
	void OnTouchRelease(ETouchIndex::Type FingerIndex, FVector Location); // Location Z == 0
	void OnTouchMove(ETouchIndex::Type FingerIndex, FVector Location); // Location Z == 1

	void OnResourceDownloadComplete();

public:
	UGsScopeGlobal* GetGlobalScope() const { return _global; }
	UGsScopeLobby* GetLobbyScope() const { return _lobby; }
	UGsScopeGame* GetGameScope() const { return _game; }

public:
	FGsMessageHolder* GetMessageHolder() const { check(_message);  return _message; }

public:
	//U Type --------------------------------------------------------------------------------------
	template <typename T>
	static T* GetGlobalManager(uint8 inType)
	{
		if (_isGlobalCreated)
		{
			if (UGsScopeHolder* scopeHolder = TGsSingleton<UGsScopeHolder>::Instance())
			{
				verify(scopeHolder);
				if (UGsScopeBase* scope = scopeHolder->GetGlobalScope())
				{
					verify(scope);
					T* manager = scope->GetUObjectManager<T>(inType);
					verify(manager);
					return manager;
				}
			}
			// 초기화 이후 체크하자(CDO) 타이밍에 호출되는것이 있음
			verify(0);
		}
		return nullptr;
	}

	
	template <typename T>
	static T* GetLobbyManager(uint8 inType)
	{
		if (_isLobbyCreated)
		{
			if (UGsScopeHolder* scopeHolder = TGsSingleton<UGsScopeHolder>::Instance())
			{
				verify(scopeHolder);
				if (UGsScopeBase* scope = scopeHolder->GetLobbyScope())
				{
					verify(scope);
					T* manager = scope->GetUObjectManager<T>(inType);
					verify(manager);
					return manager;
				}
			}
			verify(0);
		}
		return nullptr;
	}
	
	template <typename T>
	static T* GetGameManager(uint8 inType)
	{
		if (_isGameCreated)
		{
			if (UGsScopeHolder* scopeHolder = TGsSingleton<UGsScopeHolder>::Instance())
			{
				verify(scopeHolder);
				if (UGsScopeBase* scope = scopeHolder->GetGameScope())
				{
					verify(scope);
					T* manager = scope->GetUObjectManager<T>(inType);
					verify(manager);
					return manager;
				}
			}
			verify(0);
		}
		return nullptr;
	}

	//F Type --------------------------------------------------------------------------------------
	template <typename T>
	static T* GetGlobalManagerFType(uint8 inType)
	{
		if (_isGlobalCreated)
		{
			if (UGsScopeHolder* scopeHolder = TGsSingleton<UGsScopeHolder>::Instance())
			{
				verify(scopeHolder);
				if (UGsScopeBase* scope = scopeHolder->GetGlobalScope())
				{
					verify(scope);
					T* manager = scope->GetFObjectManager<T>(inType);
					verify(manager);
					return manager;
				}
			}
			verify(0);
		}
		
		return nullptr;
	}

	template <typename T>
	static T* GetLobbyManagerFType(uint8 inType)
	{
		if (_isLobbyCreated)
		{
			if (UGsScopeHolder* scopeHolder = TGsSingleton<UGsScopeHolder>::Instance())
			{
				verify(scopeHolder);
				if (UGsScopeBase* scope = scopeHolder->GetLobbyScope())
				{
					verify(scope);
					T* manager = scope->GetFObjectManager<T>(inType);
					verify(manager);
					return manager;
				}
			}
			verify(0);
		}
		return nullptr;
	}

	template <typename T>
	static T* GetGameManagerFType(uint8 inType)
	{
		if (_isGameCreated)
		{
			if (UGsScopeHolder* scopeHolder = TGsSingleton<UGsScopeHolder>::Instance())
			{
				verify(scopeHolder);
				if (UGsScopeBase* scope = scopeHolder->GetGameScope())
				{
					verify(scope);
					T* manager = scope->GetFObjectManager<T>(inType);
					verify(manager);
					return manager;
				}
			}
			verify(0);
		}
		return nullptr;
	}
};

using GSScopeHolderSingle = TGsSingleton<UGsScopeHolder>;
#define GScope()	GSScopeHolderSingle::Instance()
