#pragma once
#include "CoreMinimal.h"

#include "Party/GsPartyDefine.h"

#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "UMG/Public/Blueprint/UserWidget.h"

#include "Core/Public/Internationalization/Text.h"
#include "SlateCore/Public/Layout/Geometry.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"

#include "GsUIPartyMemberItem.generated.h"

class UWidgetSwitcher;
class UImage;
class UPanelWidgt;
class UProgressBar;

class UGsUIIconBase;
class UGsButton;
class UGsUIPartyDrawMenu;

UCLASS()
class UGsUIPartyMemberItem : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

	// 공통
protected:
	// 0: 멤버
	// 1: 빈칸(leader)
	// 2: 초대 대기
	// 3: empty(member)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherType;

	// 멤버 상태

	// 슬롯 번호
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textSlotId;
	// 리더 이미지
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgLeader;
	// 무기 타입
	// 아이콘 이미지(로드방식->이미지에 sprite 밀어 넣는 방식으로 변경)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgWeapon;
	// 2022/06/17 PKT - 파티원의 준비 상태가 필요 할때 사용(파티 던전)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgReady;
	// 레벨
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textLevel;
	// 이름
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootBar;
	// hp 프로그래스 바
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _progressBarHp;
	// mp 프로그래스 바
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _progressBarMp;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _progressBarShieldFront;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _progressBarShieldBack;

	// 드로우 메뉴 열기 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDrawMenu;
	
	// 디스폰 표시 image
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _widgetDespawn;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPartyDrawMenu* _drawMenu;

	// dead image
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgDead;

	// 빈칸
protected:
	// 초대 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnInvite;

	// 초대 대기
protected:
	// 대기 남은시간 프로그래스바
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _progressBarInviteWait;

private:
	// 슬롯 id
	int _partySlotId;
	// 비어있는지
	bool _isEmpty = true;
	// 아이템 타입
	EGsPartyMemberItemType _memberItemType = EGsPartyMemberItemType::None;
	// 대기 시작 시간
	int64 _startTime;
	// 리더인가
	bool _isLeader = false;

	// 추방 눌렀는지
	bool _isSendKick = false;

	bool _isOpenDrawMenu = false;

	// 이전 데이터(다시 세팅 못하게 처리용)
protected:
	FString _oldName;
	CreatureWeaponType _oldWeaponType = CreatureWeaponType::NONE;
	int _oldLevel = -1;
	float _oldMpRate = -1.0f;

	int _currHp = 0;
	int _maxHp = 0;
	int _currShield = 0;


	// 가상 함수
protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;

	// ui 이벤트
public:
	// 드로우 메뉴 click
	UFUNCTION()
	void OnClickDrawMenu();
	// open menu
	bool OpenDrawMenu();
	
	// 리더 변경
	void OnClickChangeLeader();
	// 강퇴
	void OnClickKick();
	// 초대
	UFUNCTION()
	void OnClickInvite();
	// 채널 이동
	void OnClickChannelMove();
	// 유저 정보 컨텍스트
	void OnClickUserInfo();


	// 로직 함수
public:
	// 데이터 세팅
	void SetData(int In_slotId,
		bool In_isEmpty,
		const FString& In_name = FString(""),
		EGsPartyMemberItemType In_itemType = EGsPartyMemberItemType::None,
		CreatureWeaponType In_weaponType = CreatureWeaponType::NONE,
		bool In_isLeader = false,
		int In_level = -1,
		int In_currHp = 0,
		int In_maxHp = 0,
		int In_currShield = 0,
		float In_mpRate = -1.f,
		bool In_isCloseDrawMenu = false,
		bool In_isSpawned = true,
		bool In_isReady = false,
		WorldId In_homeWorldId = 0);

	// 무기 아이콘 만들기
	void MakeWeaponIcon(CreatureWeaponType In_weaponType);

	// 프로그래스바 시작
	void StartProgressBar(int64 In_startTime);
	// 드로우 메뉴 닫기
	void CloseDrawMenu();
	// info 변경되었는지 체크(정보 개별 수정)
	void UpdateChangedInfo();

	// 이전 데이터 클리어
	void ClearOldData();
	void UpdateHpShieldBar();

	// get, set
public:
	// 초대 대기 남은시간 rate 값 구하기
	float GetInviteWaitTimeRate();
	// 레벨 세팅
	void SetLevel(int In_level);
	// mp rate 세팅
	void SetMpRate(float In_mpRate);

	void SetHP(int32 InCurrent, int32 InMax, bool bInUpdateBar);	
	void SetShield(int32 InCurrent, bool bInUpdateBar);
		 
	// weapon type 변경
	void SetWeaponType(CreatureWeaponType In_type);
	// party slot id 세팅
	void SetPartySlotId(int In_partySlotId);
	// 이름 세팅
	void SetName(const FString& In_name, WorldId In_homeWorldId);
	// 리더인지 세팅
	void SetIsLeader(bool In_isLeader);
	// 스폰 되어있는가
	void SetIsSpawned(bool In_isSpawned);
	// 2022/06/17 PKT - 준비 상태
	void SetIsReaddy(bool In_isReady);
	// 상태이상 컨퓨즈(내 HP/MP 정보 가림)
	void UpdateAbnormalityConfuse();
	void SetIsConfused(bool bIsConfused);
};