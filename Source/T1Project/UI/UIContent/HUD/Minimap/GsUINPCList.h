#pragma once

#include "CoreMinimal.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "Message/GsMessageGameObject.h"
#include "GsUINPCList.generated.h"

class UUserWidget;
class UScrollBox;
class UVerticalBox;
class UWidget;
class UGsDynamicPanelSlotHelper;

struct IGsMessageParam;


/*
	NPC LIST
*/
UCLASS(meta = (DisableNativeTick))
class UGsUINPCList : public UUserWidget
{
	GENERATED_BODY()

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

private:
	// gameobject 메시지 처리용
	MsgGameObjHandleArray _arrGameObjDelegate;

	// 가상 함수
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	virtual void BeginDestroy() override;

	// 메시지
public:
	// 갱신
	void OnDebugAIReserveListUpdate(const IGsMessageParam* In_param);

	// 로직 함수
public:
	// 디버그용 리스트 데이터 세팅
	void SetDebugListData();

	void InitializeMessage();
	void FinalizeMessage();
protected:
	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, UWidget* InEntry);
};