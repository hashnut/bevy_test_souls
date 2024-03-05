#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIBillboard.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "GsUIBillboardInteractionButton.generated.h"

class UGsButton;
class UGsGameObjectBase;
class UGsUIIconBase;

class UImage;
class UWidgetSwitcher;
class UVerticalBox;
class UWidgetAnimation;

//----------------------------------------
// 인터랙션 버튼
//----------------------------------------
UCLASS()
class UGsUIBillboardInteractionButton : public UGsUIBillboard
{
	GENERATED_BODY()

	// 멤버 변수
protected:
	// OLD_INTERACTION_ICON(삭제 예정)
	// 아이콘 이미지 로드 해서 붙일곳
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _iconImageRoot;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	FSoftObjectPath _clickVFXPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	int32 _clickVFXZOrder = 1000;

	UPROPERTY(BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* _aniButtonPress;

	// 로딩된 아이콘
	TWeakObjectPtr<UGsUIIconBase> _loadedIcon;

	NpcFunctionType _functionType;

	

protected:
	virtual void NativeDestruct() override;
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	virtual void SetHide(EGsUIHideFlags InHideFlags) override;

public:
	virtual void OnChangeRenderState(bool bIsRendered) override;

public:
	void OnClickButton();
	void StartButtonPressAnimation();

	// 로직 함수
public:
	// 데이터 세팅
	void SetData(UGsGameObjectBase* In_owner ,NpcFunctionType In_functionType, int In_Index = 0);
	// 퀘스트 npc 데이터 세팅
	void SetQuestMarkData(UGsGameObjectBase* In_owner, bool In_isInvadeQuestNpc, NpcFunctionType In_functionType, int In_Index);
	// 리소스 해제
	void ReleaseResource();

	// 인터랙션 버튼 출력 옵션 갱신
	void UpdateShowInteractionButtonOption();
	bool IsOptionEnabled();
	void SetVisible(bool InVisibility);

protected:
	const struct FGsSchemaInteractionData* GetTableData(NpcFunctionType InType) const;
	const struct FGsSchemaInteractionInvadeData* GetTableInvadeData(NpcFunctionType InType) const;

};