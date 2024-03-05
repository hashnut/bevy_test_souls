#pragma once

#include "CoreMinimal.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "GsUIBookmarkItem.generated.h"

class UGsButton;

class UImage;
class UWidgetSwitcher;

UCLASS()
class UGsUIBookmarkItem : 
	public UUserWidget,
	public IGsToggleGroupEntry, 
	public IGsEntryInterface
{
	GENERATED_BODY()
	// 멤버 변수
protected:
	// 공유 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnShare;
	// 이동 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnMove;
	// 자동 이동 버튼
	// https://jira.com2us.com/jira/browse/CHR-23756
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnAutoMove;
	// 아이템 버튼(전체)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnItem;
	// 이름
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textName;
	// 선택 이미지
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgSelected;
	// 북마크 id
	uint64 _bookmarkId;
	bool _bIsSelected = false;
	// 아이템 인덱스
	int _itemIndex = -1;

	// 0. 기본 아이템
	// 1. 송신: 공유 버튼들(파티, 길드)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherType;

	// 공유(송신)
	// 파티 공유
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnShareParty;
	// 길드 공유
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnShareGuild;
	// 공유메뉴 닫기
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnShareMenuClose;

	// 가상 함수
public:
	// UUserWidget override
	virtual void NativeOnInitialized() override;

	// IGsToggleGroupEntry override
	virtual void SetIsSelected(bool bInIsSelected) override;
	virtual bool GetIsSelected() const override;

	// 로직 함수
public:
	UFUNCTION()
	void OnClickShare();
	UFUNCTION()
	void OnClickMove();
	// 자동 이동 클릭
	UFUNCTION()
	void OnClickAutoMove();
	// 아이템 클릭
	UFUNCTION()
	void OnClickItem();

	// 파티 공유 클릭
	UFUNCTION()
	void OnClickShareParty();
	// 길드 공유 클릭
	UFUNCTION()
	void OnClickShareGuild();
	// 공유 메뉴 닫기 클릭
	UFUNCTION()
	void OnClickShareMenuClose();

	// 데이터 세팅
	void SetData(bool In_isShowSharedMenu , 
		int In_itemIndex, 
		uint64 In_bookmarkId, FString In_name);

	// 공유 메뉴 닫기
	void CloseShareMenu();


	// get, set
public:
	// 북마크 id 접근
	uint64 GetBookmarkId() { return _bookmarkId; }
	// 공유 메뉴 인덱스 세팅
	void SetShareMenuIndex(int In_index);
};