#pragma once
#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUIPopup.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"

#include "GsUIPopupScanOption.generated.h"

/*
* popup 스캔 옵션 
*/

class UGsButton;
class UGsUIPartySettingTab;
class UPanelWidget;
class UGsUITooltipDesc;

UCLASS()
class T1PROJECT_API UGsUIPopupScanOption : public UGsUIPopup
{
	GENERATED_BODY()

	// 멤버 변수
protected:

	// 닫기 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	// boss on/ off
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPartySettingTab* _toggleBossTargetOnOff;

	// 보스 타입 선택 토글
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPartySettingTab* _toggleSelBossType;
	// named toggle
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPartySettingTab* _toggleSelNamedType;

	// user on/ off
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPartySettingTab* _toggleUserTargetOnOff;

	// now attacker remote toggle
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPartySettingTab* _toggleSelAttackerRemoteType;
	// enemy guild toggle
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPartySettingTab* _toggleSelEnemyGuildType;
	// alert target toggle(before implementation )
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPartySettingTab* _toggleSelAlertTargetType;
	// other server toggle
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPartySettingTab* _toggleSelOtherServerType;
	// chaos toggle
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPartySettingTab* _toggleSelChaosType;	
	// normal remote toggle
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPartySettingTab* _toggleSelNormalRemoteType;

	// monster on/off
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPartySettingTab* _toggleMonsterTargetOnOff;

	// now attacker monster toggle
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPartySettingTab* _toggleSelAttackerMonsterType;
	// quest monster toggle
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPartySettingTab* _toggleSelQuestMonsterType;
	// 일반 몬스터 타입 선택 토글
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPartySettingTab* _toggleSelNormalMonsterType;

	// etc on/off
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPartySettingTab* _toggleETCTargetOnOff;

	// treasure monster toggle
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPartySettingTab* _toggleSelTreasureMonsterType;
	// camp toggle
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPartySettingTab* _toggleSelCampType;
	
	// 자동 새로 고침 토글 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPartySettingTab* _toggleAutoRefreashScan;

	// 토글 그룹 관리
	FGsToggleGroup _toggleGroup;

	// 시간 선택 패널
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelSelectTime;

	// 3초
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPartySettingTab* _radioButtonSelTime_1;
	// 5초
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPartySettingTab* _radioButtonSelTime_2;
	// 3초
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPartySettingTab* _radioButtonSelTime_3;

	// 툴팁
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITooltipDesc* _tooltipInfo;
	// 툴팁 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTooltip;

	// 가상 함수
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// 로직 함수
public:
	// 닫기 클릭
	UFUNCTION()
	void OnClickClose();
	// 툴팁 클릭
	UFUNCTION()
	void OnClickTooltip();
	// 시간 클릭
	UFUNCTION()
	void OnClickSelectTime(int32 In_index);

	// boss type on/off(boss, named)
	void OnBossTypeOnOff(bool In_isSelect);
	// 보스 타입 선택
	void OnSelectBossType(bool In_isSelect);
	// named type select
	void OnSelectNamedType(bool In_isSelect);

	// user type on/off(attacker Remote, enemy guild, alert target, chaos, normal remote)
	void OnUserTypeOnOff(bool In_isSelect);
	// attacker remote type select
	void OnSelectAttackerRemoteType(bool In_isSelect);
	// enemy guild type select
	void OnSelectEnemyGuildType(bool In_isSelect);
	// alert target type select
	void OnSelectAlertTargetType(bool In_isSelect);
	// other server target type select
	void OnSelectOtherServerType(bool In_isSelect);
	// chaos type select
	void OnSelectChaosType(bool In_isSelect);
	// 타유저 타입 선택
	void OnSelectRemoteType(bool In_isSelect);

	// monster type on/off(attacker Monster, questMonster, normalMonster)
	void OnMonsterTypeOnOff(bool In_isSelect);
	// attacker monster type select
	void OnSelectAttackerMonsterType(bool In_isSelect);
	// quest monster type select
	void OnSelectQuestMonsterType(bool In_isSelect);
	// 몬스터 타입 선택
	void OnSelectNormalMonsterType(bool In_isSelect);

	// etc type on/off(tresure monster, camp)
	void OnETCTypeOnOff(bool In_isSelect);
	// treasure monster type select
	void OnSelectTreasureMonsterType(bool In_isSelect);
	// camp type select
	void OnSelectCampType(bool In_isSelect);

	// 자동 스캔 선택
	void OnSelectAutoScanType(bool In_isSelect);

	// 2022/10/20 PKT - KeyBoard Mapping
protected:
	virtual void OnInputCancel() override;
};