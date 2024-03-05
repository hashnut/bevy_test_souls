#pragma once

#include "CoreMinimal.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GsUIBuddySlot.generated.h"

class UGsButton;
class UImage;
class UTextBlock;
class FGsBuddyInfo;
class USizeBox;

UCLASS()
class UGsUIBuddySlot :
	public UUserWidget,
	public IGsToggleGroupEntry, 
	public IGsEntryInterface
{
	GENERATED_BODY()	

public:
	DECLARE_DELEGATE_OneParam(FOnClick, int /*InIndex*/);

	// 멤버 변수
protected:
	// 이름
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textUserName;

	// 접속중
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textLoginStatus;

	// 아이템 버튼(전체)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSlot;
	
	// 유저 정보 컨텍스트 UI 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnUserInfo;

	// X 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;
	
	// 수락
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnAccept;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgSelected;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	USizeBox* _sizeBoxTime;

	bool _bIsSelected = false;
	int _slotIndex = 0;
	int _tabIndex = 0;
	TSharedPtr<FGsBuddyInfo> _buddyInfo;

public:
	FOnClick OnClick;

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
	void OnClickUserInfo();
	UFUNCTION()
	void OnClickCancel();
	UFUNCTION()
	void OnClickAccept();
	// 아이템 클릭
	UFUNCTION()
	void OnClickSlot();
	// 데이터 세팅
	void SetData(int inSlotIndex, int inTabIndex, TSharedPtr<FGsBuddyInfo>& inBuddyInfo);

private:
	void SetSlotItem();
	void AllItemHide();
	void SetOnlineStatus();
};