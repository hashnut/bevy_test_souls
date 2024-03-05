#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeData.h"
#include "../MonsterKnowledgeCollection/EMonsterKnowledgeEnumData.h"
#include "../Message/MessageParam/GsMonsterKnowledgeCollection.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GSUIMonsterCollectionBasePage.generated.h"

enum class EGsMapGroupRefreshType : uint8
{
	NO_SCROLL,			// 맵 그룹 리스트 움직이지 않음 (지식 획득)
	SCROLL_TO_ITEM,	// 특정 리스트로 이동 (캡쳐 상황)
	SCROLL_TO_TOP,		// 스크롤을 초기화 (카테고리 변경)
};

UCLASS()
class T1PROJECT_API UGSUIMonsterCollectionBasePage : public UUserWidget
{
	GENERATED_BODY()
protected:
	const static int32 ALL_CATEGORY_INDEX = 0;
	const static int32 CATEGORY_INDEX_OFFSET = 1;

public:
	virtual void BeginDestroy() override;
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	virtual void Enter(struct FGsMonsterKnowledgeUICaptureData* InData);
	virtual void Leave() {}
	virtual void Capture(struct FGsMonsterKnowledgeUICaptureData* OutData) {}
	virtual bool CloseChildWidget() { return false; }

	virtual void InvalidateRedDot() {}

	virtual void InvalidateBook(uint32 InData);
	virtual void InvalidateBookSet(const TArray<FGsMonsterCollectionBookResult>* InData);

	virtual void InvalidateCodex();
	virtual void InvalidateKnowledgeGain();
	virtual void InvalidateMapGroupText();
	virtual void InvalidateCategoryExplore();
	virtual void InvalidateMapGroupList(EGsMapGroupRefreshType InScrollType = EGsMapGroupRefreshType::NO_SCROLL);

	virtual void InvalidateSlate(MonsterKnowledgeSlateId InId);
	virtual void InvalidateSlateCurrency();


protected:		
	virtual void BuildData(int32 InCategoryIndex) {}
};