#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "Message/GsMessageContents.h"
#include "GsUITrayObserveTargetList.generated.h"

/*
* tray 관전 타겟 리스트
*/

struct IGsMessageParam;

class UUserWidget;
class UScrollBox;
class UVerticalBox;
class UWidget;
class UGsDynamicPanelSlotHelper;

UCLASS()
class UGsUITrayObserveTargetList : public UGsUITray
{
	GENERATED_BODY()
public:
	// 멤버 변수
protected:
	UPROPERTY(EditdefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetClass;

	// 스크롤
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBox;

	// 아래로 차례차례 추가
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _verticalBox;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _scrollBoxHelper;
	// 남은 관전 대상 숫자
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textRemainObserveTargetCount;

private:
	// 메시지 델리게이트
	TArray<TPair<MessageContentObserver, FDelegateHandle>> _msgContensObserverDelegates;

	// 가상 함수
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	virtual void BeginDestroy() override;

	
	// 메시지
public:
	// 관전 리스트 변경
	void OnChangeObserveTaretList(const IGsMessageParam* In_param);

	// 로직 함수
public:
	// 데이터 세팅
	void SetObserveTargetListData();
	void ClearAllMessage();
protected:
	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, UWidget* InEntry);
};