// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsUIController.generated.h"

class UGsUIWidgetBase;

/**
 * 위젯 관리 클래스. 생성, Cache, Stack등을 관리
 * 규칙
 * - 같은 창을 여러개 열지 않는 경우가 많으므로, 1개의 위젯만 캐싱하여 재사용 한다. 
 * - 중복 생성된 인스턴스들은 UsingWidgetArray에서 제거되면(사용하지 않으면) 해제된다.
 * - Stack의 Top에 있는 위젯를 또 Add하려고 할 경우, 재생성하지 않음.
 * - Stack의 Top 위로 같은 위젯을 쌓아야 할 경우, 중복허용 옵션(CanMultipleInstance)이 켜진 위젯이어야 함.
 * - Tray는 Stack 관리하지 않으므로 재생성을 원치 않으면 중복체크를 해줘야 한다.
 */
UCLASS()
class T1PROJECT_API UGsUIController : public UObject
{
	GENERATED_BODY()

protected:
	// 화면에서 현재 사용중인 위젯들을 관리. 뷰포트에서 제거하기 위해 관리함.
	UPROPERTY()
	TArray<UGsUIWidgetBase*> _usingWidgetArray;
	
	// _cachedWidgetArray가 원본을 가지며, 검색 편의을 위해 _cachedWidgetMap 활용.
	// CreateWidget을 자주하지 않기 위해 캐싱해두며, 한개만 저장하여 재활용한다.
	// 한 개 이후부터 중복 생성된 인스턴스들은 _usingWidgetArray에서 빠지면 파괴된다.
	UPROPERTY()
	TArray<UGsUIWidgetBase*> _cachedWidgetArray;
	
	TMap<FName, TWeakObjectPtr<UGsUIWidgetBase>> _cachedWidgetMap; // Key: FileName

	// 일반 스택
	UPROPERTY()
	TArray<UGsUIWidgetBase*> _stackableArray;

	// 시스템 팝업이 사용하는 스택. 항상 위에 그려지며, StackableArray보다 먼저 Pop한다.
	UPROPERTY()
	TArray<UGsUIWidgetBase*> _systemStackArray;

protected:
	bool bIsRemovingWidgets = false;

public:
	void Finalize();

public:
	virtual void BeginDestroy() override;

protected:
	void AddToViewport(UGsUIWidgetBase* InWidget);
	
public:
	bool IsRemovingWidgets() const { return bIsRemovingWidgets; }

	// 모두 삭제
	void RemoveAll();

	// 파괴되지 않는 위젯을 제외하고 삭제. SystemStack 유지.
	void RemoveWhenFlowChanged();
	
	// 최근에 사용한 위젯, 파괴되지 않는 위젯을 제외하고 삭제. SystemStack 유지.
	void RemoveWhenLevelChanged();

	// 위젯을 생성하거나 캐시된 UI를 찾아 화면에 추가한다.
	UGsUIWidgetBase* CreateAndAdd(class UGameInstance* InOwner, TSubclassOf<UGsUIWidgetBase> InClass, const FName& InKey);

	// 화면에서 삭제
	void RemoveWidget(UGsUIWidgetBase* InWidget);

	// 스텍에서 제거
	void PopStack(UGsUIWidgetBase* InWidget, bool bInRecoverPrevStack = true);

	// 일반 스택에 있는 것 모두 닫기(시스템 스택 제외)
	void CloseAllStack(UGsUIWidgetBase* InCloseWidget = nullptr, bool bInRecoverDefaultState = true);

	// 안드로이드 백키처리를 위한 인터페이스
	bool Back();

	// 캐시된 UI 가져오기. 현재 사용여부 체크 파라미터.
	UGsUIWidgetBase* GetCachedWidgetByName(const FName& InKey, bool InActiveCheck);

	// 위젯을 생성만 해두는 기능 (AddToViewport 안함)
	UGsUIWidgetBase* CreateNotAddToViewport(class UGameInstance* InOwner, TSubclassOf<UGsUIWidgetBase> InClass, const FName& InKey);

	bool IsActiveWidget(const FName& InKey) const;
	// active중인 ui중 자동으로 close 되는거 막는 ui가 1개라도 있는지
	bool IsExistWidgetPreventAutoClose();

	UGsUIWidgetBase* StackPeek(bool InCheckSystemStack) const;

	bool IsBackDelayed();
protected:
	// 화면에 사용 중인 위젯들 삭제. 로딩 중 파괴하지 않는 위젯은 제외하고 지움.
	void RemoveUsingWidgets(); 
	void AddWidgetInter(UGsUIWidgetBase* InWidget);
	void RemoveWidgetInter(UGsUIWidgetBase* InWidget);

	// 일반 스택. Window로 가려질 때 RemoveFromParent 수행. Window Pop될 때 이전 스택 복구.
	void PushStack(UGsUIWidgetBase* InWidget);
	void RecoverPrevStack();

	// Window에 의해 가려지지 않으며, Winodw가 Pop되도, 이전 스택 복구로직을 수행하지 않음.
	void PushSystemStack(UGsUIWidgetBase* InWidget);

	bool IsTopInStack(const UGsUIWidgetBase* InWidget, bool InCheckSystemStack) const;
	UGsUIWidgetBase* FindCachedWidget(const FName& InKey);

public:
	void PrintLog();
};
