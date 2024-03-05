#pragma once

#include "CoreMinimal.h"

#include "UMG/Public/Blueprint/UserWidget.h"

#include "Scan/GsScanIconType.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedIffEnum.h"

#include "GsUIScanMemberItem.generated.h"

/*
* hud 스캔 멤버 item
*/

class UTextBlock;
class UProgressBar;
class UPanelWidget;
class UWidget;
class UImage;
class UWidgetSwitcher;
class UWidgetAnimation;

class UGsButton;
class FGsScanMemberInfo;
class FGsScanMemberInfoPlayer;

UCLASS(BlueprintType)
class UGsUIScanMemberItem : public UUserWidget
{
	GENERATED_BODY()
protected:
	// 타겟 이름
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textTargetName;
	// 타겟 이름 색상 조절
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textTargetNameColor;
	// 스캔 멤버 타겟 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnScanMemberTarget;
	// 순서 텍스트
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textOrder;

	// 타겟 bg(로컬이 타겟 잡은거)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _panelTargetBG;
	// 공격하는지(로컬을 공격하는지)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _panelAttackerBG;
	// 딤드 이미지
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _panelDimmedBG;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _symbolRoot;

	// 2023/12/21 PKT - Battle Arena [0 : Icon(길드,침공전 등), 1 : 그림자 전장]
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherSymbol;

	// 아이콘 이미지
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgIcon;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgIconDiplomacy;

	// animation
	UPROPERTY(BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* _aniMenu;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgGuildIffColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	FLinearColor _colorIconEnemy;

private:
	bool _isMonster = false;
	// 스캔 거리밖이거나 거리에의한 디스폰일때 딤드 처리인가
	bool _isDimmed = false;

	// NativeTick 에서 접근시 비정상 적인 FGsScanMemberInfo* 값을 멤버로 들고 있을때 있어서
	// 필요할때 마다 검색해서 쓰자
	int64 _scanMemberGameId;

	// 쉴드 계산용
private:
	int _currHp = 0;
	int _maxHp = 0;
	int _currShield = 0;

	// 가상 함수
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeDestruct() override;

	// 로직 함수
public:
	
	// 변경 된거 갱신
	void UpdateChangedInfo();
	// 아이콘, 이름 색상 갱신
	void UpdateIconAndNameColor(FGsScanMemberInfo* In_info);
	// 몬스터인지 갱신
	void UpdateIsMonster(FGsScanMemberInfo* In_info);

	// ui 이벤트
protected:
	// 스캔 타겟 클릭
	UFUNCTION()
	void OnClickScanTarget();

	// set
public:
	// 데이터 세팅
	void SetData(FGsScanMemberInfo* In_data);
	// 타겟 세팅
	void SetTargetBG(bool In_isTarget);
	// 공격자 세팅
	void SetAttackerBG(bool In_isAttacker);
	// 몬스터 아이콘 처리
	void SetMonsterIcon(EGsScanIconType In_monsterIconType);
	// 아이콘 show
	//void SetIconShow(bool In_isShow);
	void SetSymbolShow(bool InIsShow, int32 InIndex = 0);
	// 길드 엠블럼 아이콘 처리
	void SetGuildEmblemIcon(GuildEmblemId In_emblemId);
	// invasion enemy icon setting
	void SetInvasionEnemyIcon();
	// 2023/12/20 PKT - Battle Arena Team Mark
	void SetBattleArenaTeamMark(IffFactionType InIffFactionType);
	// 딤드 세팅
	void SetDimmed(bool In_isDimmed);
	// 길드 적대인지 동맹인지 표시 처리
	void SetGuildIffColorByPlayerData(FGsScanMemberInfoPlayer* In_playerData);
};