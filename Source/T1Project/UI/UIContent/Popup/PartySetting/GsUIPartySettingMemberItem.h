#pragma once

#include "CoreMinimal.h"

#include "Party/GsPartyDefine.h"

#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "UMG/Public/Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"

#include "SlateCore/Public/Layout/Geometry.h"
#include "Core/Public/Internationalization/Text.h"

#include "GsUIPartySettingMemberItem.generated.h"

class UWidgetSwitcher;
class UTextBlock;
class UImage;
class UCanvasPanel;
class UProgressBar;

class UGsUIIconBase;
class UGsButton;

UCLASS()
class UGsUIPartySettingMemberItem : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()
	// 멤버 변수

private:
	// 슬롯 id
	int _partySlotId;

	// 공통
protected:
	// 빈곳인지 스위치(0: 먼가 있음, 1: 빈 슬롯(leader), 2: empty(member))
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherEmpty;

	// 비어있을 때
protected:
	// 초대 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnInvite;

	// 정보 있을때 공통용
protected:
	// 0: 나
	// 1: 다른멤버(나 리더 아님)
	// 2: 다른멤버(나 리더)
	// 3: 초대 수락 대기
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherType;

	// 채널 캔버스 패널(InviteProgress 일때만 가린다)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _channelCanvalPanel;

	// 슬롯 번호
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textSlotId;
	// 리더 이미지
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgLeader;
	// 무기 타입
	// 아이콘 이미지 로드 해서 붙일곳
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _iconImageRoot;
	// 로딩된 아이콘
	UPROPERTY()
	UGsUIIconBase* _loadedIcon;
	// 레벨
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textLevel;
	// 이름
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textName;
	// 채널
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textChannel;

	// 내꺼 버튼
protected:
	// 탈퇴 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnLeave;

	// 나이외의 멤버들 버튼(내가 리더일때)
protected:
	// 추방 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnKick;
	// 방장 위침 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnChangeLeader;
	// 친구 추가 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnAddFriendImLeader;

	// 나이외의 멤버들 버튼(내가 리더 아닐때)
protected:
	// 친구 추가 버튼2
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnAddFriendImNotLeader;

	// 초대 대기
protected:
	// 대기 남은시간 프로그래스바
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _progressBarInviteWait;

	// 일반 멤버 변수
private:
	// 비어있는지
	bool _isEmpty = true;
	// 아이템 타입
	EGsPartyMemberItemType _memberItemType = EGsPartyMemberItemType::None;
	// 대기 시작 시간
	int64 _startTime;

	// 이전 데이터(다시 세팅 못하게 처리용)
protected:
	FString _oldName;
	CreatureWeaponType _oldWeaponType = CreatureWeaponType::NONE;
	int _oldLevel = -1;
	int _oldChannelId = -1;

	// 가상 함수
protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;	

public:
	virtual void OnShow() override;

	// 로직 함수
public:
	// 나가기(나자신)
	UFUNCTION()
	void OnClickLeave();
	// 추방
	UFUNCTION()
	void OnClickKick();
	// 리더 변경
	UFUNCTION()
	void OnClickChangeLeader();
	// 친구 추가
	UFUNCTION()
	void OnClickAddFriend();	
	// 초대
	UFUNCTION()
	void OnClickInvite();
	// 데이터 세팅
	void SetData(int In_slotId, 
		bool In_isEmpty,
		const FString& In_name = FString(""),
		EGsPartyMemberItemType In_itemType = EGsPartyMemberItemType::None,
		CreatureWeaponType In_weaponType = CreatureWeaponType::NONE, 
		bool In_isLeader = false, 
		int In_level = -1,
		int In_channelId = -1,
		WorldId In_homeWorldId = 0);
	// 프로그래스바 시작
	void StartProgressBar(int64 In_startTime);

	// 리소스 해제
	void ReleaseWeaponIcon();
	// 무기 아이콘 만들기
	void MakeWeaponIcon(CreatureWeaponType In_weaponType);

	// info 변경되었는지 체크(정보 개별 수정)
	void UpdateChangedInfo();
	// 이전 데이터 클리어
	void ClearOldData();

	// get, set
public:
	// 초대 대기 남은시간 rate 값 구하기
	float GetInviteWaitTimeRate();
	// 레벨 세팅
	void SetLevel(int In_level);
	// weapon type 변경
	void SetWeaponType(CreatureWeaponType In_type);
	// party slot id 세팅
	void SetPartySlotId(int In_partySlotId);
	// 비어있는지 세팅
	void SetIsEmpty(bool In_isEmpty, bool In_isLeader);
	// 이름 세팅
	void SetName(const FString& In_name, WorldId In_homeWorldId);
	// 아이템 타입 세팅
	void SetMemberItemType(EGsPartyMemberItemType In_itemType);
	// 리더인지 세팅
	void SetIsLeader(bool In_isLeader);
	// 채널 세팅
	void SetChannel(int In_channelId);
};