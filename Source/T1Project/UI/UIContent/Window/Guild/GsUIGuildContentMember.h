// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Guild/GsUIGuildContentBase.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "Message/GsMessageContents.h"
#include "GsUIGuildContentMember.generated.h"

class UPanelWidget;
class UGsSwitcherButton;

/**
 * 길드메인 창 길드원 페이지
 */
UCLASS()
class T1PROJECT_API UGsUIGuildContentMember : public UGsUIGuildContentBase
{
	GENERATED_BODY()

public:
	using FilterSortFunc = TFunction<void(int32)>;

	enum EGsFilterType
	{
		None = -1,
		Level = 0,
		Name,
		MemberGrade,
		Time,
		Contribution,
		AccumContribution,
		Attendance,
	};

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _entryWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnFilterLevel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnFilterName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnFilterMemberGrade;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnFilterTime;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnFilterContribution;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnFilterAccumContribution;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnFilterAttendance;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UScrollBox* _scrollBox;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelScrollRoot;

	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _slotHelper;

protected:
	TArray<UGsSwitcherButton*> _filterList;
	TArray<FilterSortFunc> _sortFuncList;

	TArray<const struct FGsGuildMemberData*> _dataList;
	FGsToggleGroup _toggleGroup;
	EGsFilterType _currFilter = EGsFilterType::MemberGrade;

	// User Context Data
	TArray<TPair<MessageContentUserContext, FDelegateHandle>> _userContextMsgHandlerList;
	UPROPERTY()
	TWeakObjectPtr<UWidget> _lastClickedButton;
	UserDBId _lastClickedUserDBId = INVALID_USER_DB_ID;

	float _panelMoveOffset;
	float _scrollStartOffset;
	float _scrollMoveOffset;
	float _time = 0.f;
	bool _isReverse = false;

public:
	virtual FString GetTabName() const override { return TEXT("Member"); }

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	// IGsInvalidateUIInterface override
	virtual void InvalidateAllInternal() override;

public:
	virtual void SendRequestPacket() override;
	virtual void Show(bool bIsMyGuild, bool IsStackRecoverFlag = false) override;	
	virtual void OnResetTimeContents(ResetTimeTicket InType) override;	
	virtual void OnNotifyChangeMemberGrade(const struct FGsGuildMsgParamMemberGrade* InParam) override;		
	virtual void OnChangeMyAuthority() override;

public:
	// 멤버 리스트 받음
	void OnRecieveMemberList();
	// 멤버 등급 변경
	void OnReceiveChangeMemberGrade(UserDBId InUserDBId);
	// 멤버 가입 노티
	void OnNotifyJoinMember();
	// 멤버 삭제(강퇴, 탈퇴, ...)
	void OnReceiveRemoveMember();

protected:
	void InvalidateList();

protected:
	void OnSelectChangeMemberEntry(int32 InIndex);

	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, class UWidget* InEntry);
		
	UFUNCTION()
	void OnClickFilterLevel();
	UFUNCTION()
	void OnClickFilterName();
	UFUNCTION()
	void OnClickFilterMemberGrade();
	UFUNCTION()
	void OnClickFilterTime();
	UFUNCTION()
	void OnClickFilterContribution();
	UFUNCTION()
	void OnClickFilterAccumContribution();
	UFUNCTION()
	void OnClickAttendance();

	void OnOpenUserContext(const struct IGsMessageParam* InParm);
	void OnCloseUserContext(const struct IGsMessageParam* InParm);

protected:
	void SetFilter(EGsFilterType InType, int32 InState);

	void SortByLevel(int32 InState);
	void SortByName(int32 InState);
	void SortByMemberGrade(int32 InState); // 기본값
	void SortByTime(int32 InState);
	void SortByContribution(int32 InState);
	void SortByAccumContribution(int32 InState);
	void SortByAttendance(int32 InState);

	// InState: 0: 없음, 1: 오름차순, 2: 내림차순
	void SetUISelectFilter(EGsFilterType InType, int32 InState);
};
