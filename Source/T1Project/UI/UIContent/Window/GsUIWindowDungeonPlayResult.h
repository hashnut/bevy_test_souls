#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedPartyDungeonEnum.h"
#include "GsUIWindowDungeonPlayResult.generated.h"

// 
/**
 * 2022/06/09 PKT - 기존 UGsUIWindowGuildDungeonClear 이름이 길드 던전에 종속적이여서 이름을 수정함
 *					( 파티 던전이 추가 되면서 해당 Window를 같이 사용하게 되었음.)
 */
UCLASS()
class T1PROJECT_API UGsUIWindowDungeonPlayResult : public UGsUIWindow
{
	GENERATED_BODY()


	const static int32 RESULT_SWITCHER_SUCCESS = 0;
	const static int32 RESULT_SWITCHER_FAILED = 1;
	/************************************************************************/
	/* Widget                                                               */
	/************************************************************************/
protected:
	// 2022/06/09 PKT - 던전 명(Window Title)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _titleTextBlock;

	// 2022/06/09 PKT - 부제목( 파티 던전 같은 경우 난이도를 표기 함. Default : EGsUIColorType::NORMAL_TEXT_COLOR )
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _subTitleTextBlock;

	// 2022/06/10 PKT - 성공 / 실패 여부에 따른 스위쳐
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _resultSwitcher;

	// 2022/06/10 PKT - 성공( 각 던전마다 다르게 출력 될 수 있음. )
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _successTextBlock;

	// 2022/06/10 PKT - 실패( 각 던전마다 다르게 출력 될 수 있음. )
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _failedTextBlock;

	// 2022/06/10 PKT - 각 던전마다 플레이 시간 출력 방식이 다름.(클리어 시간 / 플레이 시간)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _playTimDescTextBlock;

	// 2022/06/09 PKT - 던전 내 활동 시간( ex:클리어 타임)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _playTimeTextBlock;

	// 2022/06/09 PKT - 던전 퇴장 까지 남은 시간
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _exitRemainingTimeTextBlock;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherRewardInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textEmptyRewardReason;

	// 2022/06/09 PKT - 나가기 버튼
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _exitButton;

	// 2022/06/09 PKT - 기본 보상에 대한 Desc(던전 마다 사용 하는 명칭이 각 다름.)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _basicRewardDescTextBlock;

	// 2022/06/09 PKT - 특별 보상에 대한 Desc(던전 마다 사용 하는 명칭이 각 다름.)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _specialRewardDescTextBlock;

	// 2022/06/09 PKT - 기본 보상에 대한 재화 이미지
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _basicGainCurrencyImage;

	// 2022/06/09 PKT - 특별 보상에 대한 재화 이미지
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _specialGainCurrencyImage;

	// 2022/06/09 PKT - 기본 보상 재화 수량
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _basicGainCurrencyAmountTextBlock;

	// 2022/06/09 PKT - 특별 보상 재화 수량
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _specialGainCurrencyAmountTextBlock;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UCanvasPanel* _canvasPanelPersonCurrency;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UCanvasPanel* _canvasPanelGuildCurrency;

	// 2022/06/09 PKT - 기본 보상 리스트
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UListView* _basicRewardListView;

	// 2022/06/09 PKT - 특별 보상 리스트
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UListView* _specialRewardListView;

	/************************************************************************/
	/* Text Format                                                          */
	/************************************************************************/
protected:
	// 2022/06/09 PKT - 각 던전마닥 남은 시간 멘트가 달라 Format을 따로 저장
	FText _exitRemainingTimeTextFormat;

	/************************************************************************/
	/* Data                                                                 */
	/************************************************************************/
protected:
	float _elapsedTime = 0.f;						// 2022/06/09 PKT - 경과 시간
	int32 _exitLimitTimeS = 0;						// 2022/06/09 PKT - 던전에 머무를 수 있는 시간

	bool _forceToExit = false;

	FDateTime _countRemainingTime;

	TFunction<void()> _exitSenderFunc = nullptr;	// 2022/06/09 PKT - 서버에 나가기 패킷을 보낼 객체
	
	/************************************************************************/
	/* Override                                                             */
	/************************************************************************/
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/************************************************************************/
	/* Set Display                                                          */
	/************************************************************************/
protected:
	void ClearData();

	void SetCurrencyImage(ItemId InItemId, CurrencyType InSpecialType);
	void SetCurrencyImage(CurrencyType InBasicType, CurrencyType InSpecialType);
	void InvalidTitle(int32 InLevel);		// 2022/06/09 PKT - Guild Dungeon Only
	void InvalidTitle(DungeonGroupId InDungeonGroupId, DungeonDifficulty InDifficulty);		// 2022/06/09 PKT - Party Dungeon Only
	void InvalidPlayTime(const FDateTime& InDateTime);
	void InvalidGuildReward(const TArray<CurrencyPair>& InCurrencyRewardSet, const TArray<ItemIdAmountPair>& InRewardSet);
	void InvalidPersonReward(const TArray<CurrencyPair>& InCurrencyRewardSet, const TArray<ItemIdAmountPair>& InRewardSet);
	void InvalidBasicReward(CurrencyDelta InAmount, const TArray<ItemIdAmountPair>& InRewardSet);
	void InvalidSpecialReward(CurrencyDelta InAmount, const TArray<ItemIdAmountPair>& InRewardSet);
	void InvalidExitRemainingTime(int32 InRemainingTime);

	/************************************************************************/
	/* Event                                                                */
	/************************************************************************/
protected:
	UFUNCTION()
	void OnClickedExit();

	/************************************************************************/
	/* Set Data                                                             */
	/************************************************************************/
public:
	void SetData(const struct FGsGuildDungeonClearMsgParam* inData);		// 2022/06/09 PKT - Guild Dungeon Only	
	void SetDataAgitDungeonRaid(const struct FGsGuildDungeonClearMsgParam* InData);
	
	void SetData(DungeonGroupId InDungeonGroupId, DungeonDifficulty InDifficulty
		, const struct FGsUIMsgParamPartyDungeonResult* inData);		// 2022/06/09 PKT - Party Dungeon Only

private:
	void SetGuildDungeonInter(const struct FGsGuildDungeonClearMsgParam* InData);
};