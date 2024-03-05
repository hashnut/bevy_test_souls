#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h" 
#include "../MonsterKnowledgeCollection/EMonsterKnowledgeEnumData.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeUICaptureData.h"
#include "../MonsterKnowledgeCollection/MonsterKnowledgeCollectionTypeDefine.h"
#include "../UI/UIContent/Helper/GsRedDotChecker.h"
#include "../UI/UIControlLib/Control/GsToggleGroup.h"
#include "MonsterKnowledge/GSUIMonsterCollectionBasePage.h"
#include "GsUIWindowMonsterCollection.generated.h"

class UGsButton;
class UWidgetSwitcher;
class UGsSwitcherButton;
class UGsUIRedDotDefault;

/**
 * 몬스터 도감 Window Class
 */
UCLASS()
class T1PROJECT_API UGsUIWindowMonsterCollection : public UGsUIWindow
{
	GENERATED_BODY()

protected:
	/************************************************************************/
	/* BP Bind                                                              */
	/************************************************************************/
protected:
	// 2021/11/23 PKT - 뒤로 가기
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;

	// 2021/11/23 PKT - 닫기
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	// 2021/11/23 PKT - Main Button
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _switcherBtnCodex;		// 2021/11/23 PKT - 0 : Off, 1: On

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotDefault* _redDotCodex;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _switcherBtnSlate;		// 2021/11/23 PKT - 0 : Off, 1: On

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotDefault* _redDotSlate;

	// 2021/11/23 PKT - Tab Button Siwcher
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherPage;

protected:
	// 2023/1/31 PKT - Main Tab Toggle
	FGsToggleGroup _toggleGroupMainTab;

	TWeakObjectPtr<UGSUIMonsterCollectionBasePage> _activePage;

	FGsMonsterKnowledgeUICaptureData _captureData;

	/************************************************************************/
	/* override                                                             */
	/************************************************************************/
public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual bool SyncContentState() override;
	virtual bool OnBack() override;

protected:
	UFUNCTION()
	void OnSelectedMainTab(int32 InIndex);

private:
	// 2023/2/3 PKT - Main Tab RedDot
	void InvalidateRedDot();

public:
	// 2023/1/31 PKT - Book 갱신
	void InvalidateBook(uint32 InData);
	void InvalidateBook(const TArray<FGsMonsterCollectionBookResult>* InData);	

	/** 2023/11/02 ejrrb10 - Codex 갱신 */ 
	void InvalidateCodex();

	/** 2023/11/02 ejrrb10 - 아이템 추가/변경/삭제 발생 시 호출됨. Codex 전용으로 쓰자 일단 */
	void OnChangeItem(const ItemId InItemId);

	/** 2023/11/02 ejrrb10 - Codex 지식 획득 탭 갱신  */
	void InvalidateKnowledgeTab();

	/** 2023/11/09 ejrrb10 - Codex 대륙 탐사 갱신  */
	void InvalidateCategoryLevelUp(CodexCategoryId InId = INVALID_CODEX_CATEGORY_ID, CodexCategoryLevel InLevel = INVALID_CODEX_CATEGORY_ID, int32 InMapGroupLevelUpCount = 0);

	/** 2023/11/09 ejrrb10 - Codex 맵 리스트 갱신  */
	void InvalidateMapGroupList();

	/** 2023/11/09 ejrrb10 - Codex 지식 누적치 갱신  */
	void InvalidateKnowledgeAmount();

	// 2023/1/31 PKT - Slat 갱신
	void InvalidateSlate(MonsterKnowledgeSlateId InId);
	void InvalidateSlateCurrency();

	void GoSelectedSlate(MonsterKnowledgeSlateId InId);

	//------------------------------------------------------------------------------------------------------------------
	// Tutorial
public:	
	UWidget* SetTutorial(bool bIsTutorial);
	UWidget* GetCodexPage();
};
