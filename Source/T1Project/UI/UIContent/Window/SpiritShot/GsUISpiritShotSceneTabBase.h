// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsUISpiritShotSceneTabBase.generated.h"

class UGsUISequencePlayer;
class UWidget;
class UUserWidget;
class UGsUIVFX;
class UCanvasPanel;
class UGsUISpiritShotLine;
class UGsUISpiritShotBlueStone;


/**
 * 사운드 연출 리소스 이름 (DTGsSoundResData 로부터 조회)
 */
namespace
{
	const FString SOUND_PREV_ENCHANT = TEXT("UI_SpiritShot_Prev_Enchant"); // (전조 연출)
	const FString SOUND_ALL_COMPLETE = TEXT("UI_SpiritShot_All_Complete"); // (판 완료)
	const FString SOUND_ENCHANT_SUCCESS = TEXT("UI_SpiritShot_Enchant_Success"); //	(강화 성공)
	const FString SOUND_ENCHANT_FAIL = TEXT("UI_SpiritShot_Enchant_Fail"); // (강화 실패하여 레벨 유지시)
	const FString SOUND_ENCHANT_FAIL_DOWN = TEXT("UI_SpiritShot_Enchant_Fail_Down"); // (강화 실패하여 레벨 하락시)
	const FString SOUND_ENCHANT_BLUE_STONE = TEXT("UI_SpiritShot_Enchant_StepChange_BlueStone"); //	(파란돌)
	const FString SOUND_ENCHANT_STEP_CHANGE = TEXT("UI_SpiritShot_Enchant_StepChange_Light"); // (5 레벨별 강화 성공시, 점등)
}


/**
 * 연출 변경 사유
 * 전조 연출을 키는 것과, 나머지 이벤트는 철저히 분리되어야 한다.
 * 재귀호출에 빠질 위험이 있기 때문이다.
 */
enum class EGsStateSpiritShotSceneState : uint8
{
	CATEGORY_SWITCH,		// 카테고리 탭 변경했을 경우
	ENCHANT_PREV_VFX,		// 강화 전조 연출
	ENCHANT_RESULT_VFX,		// 강화 결과 연출
};

/**
 * 강화 후, 레벨이 올랐는지, 유지 되었는지, 하락하였는지 여부
 */
enum class EGsStateSpiritShotLevel : uint8
{
	LEVEL_UP,		// 강화 레벨 상승
	LEVEL_SAME,		// 강화 레벨 유지
	LEVEL_DOWN		// 강화 레벨 하락
};


/**
 * 연출 UI 는  Level, 연출 여부를 참고한다
 * 강화 결과는 강화 결과 연출을 보여주지만, 카테고리 변경은 강화 결과 연출을 보여주지 않는다
 */
struct Parameters
{
	Level _level;
	EGsStateSpiritShotSceneState _sceneState;
	EGsStateSpiritShotLevel _levelState;
	SpiritShotCategoryId _categoryId;
	SpiritShotLevelId _levelId;
	int32 _rateModifyCount;
	TArray<ItemDBIdAmountPair> _materialInfos;
};


/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUISpiritShotSceneTabBase : public UUserWidget
{
	GENERATED_BODY()
	
			
	/************************************************************************/
	/* 프로퍼티                                                             */
	/************************************************************************/
protected:
	/** 전체 판이 흔들리는 전조 연출을 위한 시퀀스 플레이어 */
	UPROPERTY(BlueprintReadOnly, Transient, Category = "GsUI")
	UGsUISequencePlayer* _sequencePlayer;

	/** 전체 판이 흔들리는 애니메이션 */
	UPROPERTY(BlueprintReadOnly, Transient, Category = "GsUI", meta = (BindWidgetAnim))
	UWidgetAnimation* _shakeAni;

protected:
	/** 파란 돌. 5레벨 도달시 마다 빛이나는 효과 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISpiritShotBlueStone* _blueStoneFx;

	/** 만렙(25렙)을 도달했을 때 보여줄 연출 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _allCompleteVfx;

	/** 만렙(25렙)을 도달했을 때 계속 보여줄 연출. _allCompleteVfx 이후 보여진다 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UUserWidget* _allCompleteMaintain;

	
	/** 5레벨별 시작 위치 표시 링 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UUserWidget* _spiritShotActiveRingA;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UUserWidget* _spiritShotActiveRingB;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UUserWidget* _spiritShotActiveRingC;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UUserWidget* _spiritShotActiveRingD;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UUserWidget* _spiritShotActiveRingE;


	/** 중앙에 나올 성공 연출 효과 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _spiritShotLevelSuccessMain;	

	/** 5레벨별 성공 연출 효과 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _spiritShotLevelSuccessA;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _spiritShotLevelSuccessB;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _spiritShotLevelSuccessC;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _spiritShotLevelSuccessD;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _spiritShotLevelSuccessE;


	/** 중앙에 나올 실패 연출 효과 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _spiritShotLevelFailMain;


	/** 5레벨별 실패/유지 연출 효과 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _spiritShotLevelFailA;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _spiritShotLevelFailB;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _spiritShotLevelFailC;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _spiritShotLevelFailD;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _spiritShotLevelFailE;


	/** 5레벨별 완료 연출 위젯 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UUserWidget* _spiritShotLevelCompleteA;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UUserWidget* _spiritShotLevelCompleteB;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UUserWidget* _spiritShotLevelCompleteC;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UUserWidget* _spiritShotLevelCompleteD;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UUserWidget* _spiritShotLevelCompleteE;


	/** 5레벨별 잠식 라인 위젯 (5레벨씩 5단계로, 총 25개) */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISpiritShotLine* _spiritShotLineA1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISpiritShotLine* _spiritShotLineA2;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISpiritShotLine* _spiritShotLineA3;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISpiritShotLine* _spiritShotLineA4;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISpiritShotLine* _spiritShotLineA5;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISpiritShotLine* _spiritShotLineB1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISpiritShotLine* _spiritShotLineB2;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISpiritShotLine* _spiritShotLineB3;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISpiritShotLine* _spiritShotLineB4;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISpiritShotLine* _spiritShotLineB5;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISpiritShotLine* _spiritShotLineC1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISpiritShotLine* _spiritShotLineC2;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISpiritShotLine* _spiritShotLineC3;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISpiritShotLine* _spiritShotLineC4;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISpiritShotLine* _spiritShotLineC5;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISpiritShotLine* _spiritShotLineD1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISpiritShotLine* _spiritShotLineD2;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISpiritShotLine* _spiritShotLineD3;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISpiritShotLine* _spiritShotLineD4;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISpiritShotLine* _spiritShotLineD5;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISpiritShotLine* _spiritShotLineE1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISpiritShotLine* _spiritShotLineE2;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISpiritShotLine* _spiritShotLineE3;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISpiritShotLine* _spiritShotLineE4;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISpiritShotLine* _spiritShotLineE5;


	/**
	 * 부유물에 붙어 있는 패턴 조각들의 부모 패널 (탭별로 패턴의 갯수가 달라진다)
	 */
	/** 전조 연출 시, 깜빡거리는 효과를 주거나, 성공 시 모두 켜줄 패턴 조각들의 부모 패널 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _patternCanvasPanelBlink;

	/** 강화 실패 시, 어두운 효과를 줄 패턴 조각들의 부모 패널 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _patternCanvasPanelFail;

	/** 평상시, 은은한 빛 효과를 줄 패턴 조각들의 부모 패널 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _patternCanvasPanelNormal;

	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UGsUIVFX* _spiritShotPatternA;
	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UGsUIVFX* _spiritShotPatternB;
	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UGsUIVFX* _spiritShotPatternC;
	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UGsUIVFX* _spiritShotPatternD;
	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UGsUIVFX* _spiritShotPatternE;
	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UGsUIVFX* _spiritShotPatternF;
	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UGsUIVFX* _spiritShotPatternG;
	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UGsUIVFX* _spiritShotPatternH;


	/************************************************************************/
	/* 연출 위젯 및 VFX 를 담는 컨테이너                                    */
	/************************************************************************/
protected:
	/** 시작 지점 링 */
	TArray<TWeakObjectPtr<UUserWidget>> _activeRingWidgetList;

	/** 성공 효과 VFX */
	TArray<TWeakObjectPtr<UGsUIVFX>> _successVfxList;

	/** 실패 효과 VFX */
	TArray<TWeakObjectPtr<UGsUIVFX>> _failVfxList;

	/** 완료 연출 위젯 */
	TArray<TWeakObjectPtr<UUserWidget>> _completeWidgetList;

	/** 잠식 단계 표현 라인 위젯 */
	TArray<TWeakObjectPtr<UGsUISpiritShotLine>> _lineVfxList;

	/** 전조 연출 시, 깜빡거리거나 성공시, 모두 켜줄 패턴 위젯 */
	TArray<TWeakObjectPtr<UGsUIVFX>> _patternBlinkVfxList;

	/** 강화 실패 시, 어둡게 할 패턴 위젯 */
	TArray<TWeakObjectPtr<UGsUIVFX>> _patternFailVfxList;

	/** 평상시, 은은하게 빛날 패턴 위젯 */
	TArray<TWeakObjectPtr<UUserWidget>> _patternNormalVfxList;


private:
	/** 현재 Scene 이 바라보고 있는 레벨과 연출 상태 */
	Parameters _parameters;

	/** 패턴 색상값 캐싱 (애니메이션 끝나고 원상복구 시켜주기 위함) */
	//TArray<FLinearColor> _patternLinearColorList;

	/** 전조 연출에서, 랜덤하게 패턴을 깜빡거리게 만들 때 사용할 index 목록  */
	TArray<int32> _patternIndexList;

	/** 전조 연출에서, 마지막으로 깜빡거린 패턴 목록의 index */
	int32 _lastPatternIndex;

	/** 전조 연출에서, 패턴을 깜빡거리게 만들기 위해 사용할 TimerHandle */
	FTimerHandle _timerHandle;

	/** 전조 연출에서, 패턴을 묶을 각 Group 에 들어갈 패턴의 갯수 */
	UPROPERTY(BlueprintReadWrite, Category = "GsUI", meta = (AllowPrivateAccess = "true"))
	int32 _patternGroupCount;

	/** 전조 연출에서, 각 그룹 내에서 몇번째 패턴이 켜졌는지를 체크하는 인자 */
	int32 _patternGroupCountIndex;

	/** 전조 연출에서, 각 그룹내에서 패턴 문양을 반짝거리게 만들 때 사용할 시간 간격 */
	UPROPERTY(BlueprintReadWrite, Category = "GsUI", meta = (AllowPrivateAccess = "true"))
	float _patternBlinkStrideInternal;

	/** 전조 연출에서, 그룹 끼리 패턴 문양을 반짝거리게 만들 때 사용할 시간 간격 */
	UPROPERTY(BlueprintReadWrite, Category = "GsUI", meta = (AllowPrivateAccess = "true"))
	float _patternBlinkStrideExternal;

	/** 강화 결과 연출이 끝나고, 윈도우로 연출이 끝났음을 알려줄 때 사용할 TimerHandle */
	FTimerHandle _timerHandleEffect;

	/** 강화 결과 연출이 끝나고, 윈도우로 연출이 끝났음을 알려줄 때 사용할 Delay 값 (연출 시간 확보를 위함) */
	float _enchantEffectFinishedDelay = 0.75f;

	/** 강화 결과 연출이 끝나고, 패턴이 다시 은은하게 빛나도록 하기 전까지 걸리는 딜레이 */
	UPROPERTY(BlueprintReadWrite, Category = "GsUI", meta = (AllowPrivateAccess = "true"))
	float _patternNormalDelayAfterEnchant;

	/** 강화 결과 연출이 끝나고, 티커가 나오기까지 걸리는 딜레이를 위한 TimerHandle */
	FTimerHandle _timerHandleTicker;

	/** 강화 결과 연출이 끝나고, 티커가 나오기까지 걸리는 딜레이 (거의 동시에 나와도 된다) */
	UPROPERTY(BlueprintReadWrite, Category = "GsUI", meta = (AllowPrivateAccess = "true"))
	float _enchantResultTickerDelay = 0.001f;

private:
	/** 연출 계산 간격 */
	/*static */const int32 STRIDE = 5;

	/** 연출 총 단계의 갯수 */
	/*static */const int32 STEP = 5;


	/************************************************************************/
	/* overrides															*/
	/************************************************************************/
protected:
	void NativeOnInitialized() final;
	void NativeConstruct() final;
	void NativeDestruct() final;


	/************************************************************************/
	/* Behavior                                                             */
	/************************************************************************/
public:
	/** 카테고리 변경 혹은 강화 시도시, 연출 응답 */
	void SetParameters(Parameters&& InParameters);

private:
	/** 전조연출 재생 (쉐이크 + 패턴 깜빡임) */
	void PlayPrevAnimation();

	/** 전조 연출에서, 패턴 깜빡거림 효과 재생 */
	void PlayPrevPatternBlinkVfx();

	/** 강화 결과 연출 재생 */
	void PlayEnchantResultVfx();


	/** 파란돌 효과 재생 */
	void PlayBlueStoneVfx();

	/** 만렙 (25레벨) 달성 시, 1회 보여줄 연출 */
	void PlayAllCompleteVfx();

	/** 강화 성공/실패 Vfx 연출 */
	void PlaySuccessFailVfx();

	/** 패턴 개별 항목에 대한 연출 재생 (전조 연출용) */
	void PlayPatternVfxBlink();

	/** 강화 성공시 패턴에 대해 재생할 연출 (강화 성공 시, 모두 빛남) */
	void PlayPatternVfxSuccess();
	
	/** 강화 실패시 패턴에 대해 재생할 연출 (강화 성공 시, 모두 어두워짐) */
	void PlayPatternVfxFail();

	/** 패턴 개별 항목에 대한 평상시 연출 On (은은하게 빛남) */
	void ActivatePatternVfxNormal();

	/** 패턴 개별 항목에 대한 평상시 연출 Off (은은하게 빛남) */
	void DeactivatePatternVfxNormal();

	/** 파란돌 효과에 대한 상태 갱신 */
	void InvalidateBlueStone();

	/** 시작점 링 위젯 갱신 */
	void InvalidateRingStep();

	/** 각 단계 완료 위젯 갱신 */
	void InvalidateCompleteStep();

	/** 라인 위젯 갱신 (Parameter 값으로 알아서 갱신한다) */
	void InvalidateLine(bool InIsOn = true);

	/** 라인 위젯 갱신 (시작점부터 끝 인덱스 까지만 킨다) */
	void InvalidateLineInternal(bool InIsOn = true, Level InStart = 0, Level InEnd = 0);

	/** 완료시, 유지 연출 갱신 */
	void InvalidateAllCompleteMaintain();

	/** 강화 결과 연출이 완전히 끝났음을 알림 */
	void SetEnchantResultEffectFinished();

	/************************************************************************/
	/* 이벤트                                                               */
	/************************************************************************/
public:
	/** 강화 전조 연출이 끝났을 때 호출할 함수들 (연출만 정리해 준다) */
	UFUNCTION(BlueprintCallable)
	void OnFinishedPlayEnchantPrevAni(const UWidgetAnimation* InAnimation);

	/** 전조 연출이 정상적으로 끝나면 서버로 강화 요청 패킷 전송 (정상적이라는 것은, 애니메이션을 끝까지 봤다는 뜻이다) */
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnFinishedWatchPlayEnchantPrevAni();

	/** 파란돌 연출이 끝났을 때 할 작업 (파란돌 연출이 끝났을 때, 링과 완료 연출을 보여준다) */
	UFUNCTION(BlueprintCallable)
	void OnFinishedPlayBlueStoneVfx();

	/** 완료 연출이 끝났을 때 할 작업 (완료 유지 아지랑이 킴) */
	void OnFinishedPlayAllCompleteVfx();

	/** 성공 메인 연출이 끝났을 때 할 작업 (티커) */
	void OnFinishedPlaySuccessMainVfx();

	/** 실패 메인 연출이 끝났을 때 할 작업 (티커) */
	void OnFinishedPlayFailMainVfx();
};
