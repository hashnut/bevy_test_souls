#pragma once
#include "CoreMinimal.h"

#include "Summon/GsSummonDefine.h"

#include "UMG/Public/Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"

#include "Shared/Shared/SharedInclude/SharedPrimitiveTypes.h"

#include "Shared/Client/SharedEnums/SharedFairyEnum.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Client/SharedEnums/SharedCostumeEnum.h"


#include "GsUISummonCardItem.generated.h"

/*
	ui 소환 카드
*/

class UImage;
class UTextBlock;
class UWidgetAnimation;
class UWidget;
class UWidgetSwitcher;
class UGsButton;

struct FGsFairyData;

class FGsSummonCardData;

class UGsUISummonCardOpenHighEffect;

UCLASS()
class T1PROJECT_API UGsUISummonCardItem : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()
protected:
	// 이름
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textFairyName;
	// 초상화 이미지
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgPortrait;
	// 초상화 bg
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgBg;
	// 이름 등급 색상 조절용
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockFairyName;
	// 후면 등급 이펙트 이미지
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgBackGradeEffect;
	// 전면 이펙트 이미지
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgFrontGradeEffect;
	// 연출 애니매이션
	UPROPERTY(BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* _animation;
	// 애니 타겟
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _widgetAniTarget;

	// card back grade
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherGradeBack;
	// fairy card frame grade
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherFariryGradeFrame;
	// costume card frame grade
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherCostumeGradeFrame;
	// 0: fairy normal, 1: fairy magic, 2: costume normal, 3: costume magic
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherCoverColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UGsButton* _btnClick;

	// fairy open effect(0:rare, 1: epic, 2: unique)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TArray<UGsUISummonCardOpenHighEffect*> _arrayFairyOpenEffect;
	// costume open effect(0:rare, 1: epic, 2: unique)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TArray<UGsUISummonCardOpenHighEffect*> _arrayCostumeOpenEffect;
	// 페어리 고등급 뒷면 이펙트
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UGsUISummonCardOpenHighEffect* _fairyHighBackCardEffect;
	// 코스튬 고등급 뒷면 이펙트
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UGsUISummonCardOpenHighEffect* _costumeHighBackCardEffect;

	// 페어리 저등급 뒷면 이펙트
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UGsUISummonCardOpenHighEffect* _fairyNormalBackCardEffect;
	// 코스튬 저등급 뒷면 이펙트
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UGsUISummonCardOpenHighEffect* _costumeNormalBackCardEffect;
	// 새로 얻은 코스튬 or 페어리 인가(red dot과 같은 기능)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _newImage;

	UGsUISummonCardOpenHighEffect* _currentOpenEffect;

	// 애니 연출 대기 시간(초)
	float _timeSecondWaitAniPlay = 0.0f;
	// 애니 연출 대기 시간(틱)
	float _timeTickWaitAniPlay = 0.0f;
	// 대기 시작 시간
	int64 _startTime;
	// 대기 끝인가
	bool _isWaitEnd = true;

	bool _isHightCard = false;

	// 에픽이상 등장 연출있는 카드
	bool _isHighSequenceCard = false;

	bool _isOpen = false;

	int _highSequenceId = 0;

	int _dataIndex =0;
	// 카드 애니메이션 요청 시작했는지
	bool _isCardAnimationStart = false;

	EGsSummonType _summonType;

	// 카드 뒷면 이펙트 플레이 되었는지
	bool _isPlayedBackCardEffect = false;
	// 테미르 쾅 결정 등급 이펙트 인덱스
	int _currentHighGetPreEffectIndex;

public:
	// 카드 오픈 애니 콜백
	TFunction<void()> _callbackFinishCardOpenAni;

protected:
	virtual void NativeDestruct() override;
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	// bp call function
public:
	// 연출 끝
	UFUNCTION()
	void FinishAnimationBP();

	UFUNCTION()
	void OnClickCard();

	// 타임라인 특정 시점에 고등급 뒷면 카드 이펙트 출력 처리
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "SummonCardItem")
	void PlayHighBackCardEffect();

	// 로직 함수
public:
	// 데이터 세팅
	void SetData(EGsSummonType In_type , FGsSummonCardData* In_data, float In_timeWait,
	bool In_isHighCard);
	
	// 애니매이션 시작
	void StartAnimation();

	// hidden 일땐 갱신 안되어서 tray summon에서 tick 처리 호출
	void Tick(float InDeltaTime);

	void StartTimmer();
	// grade switch
	void SetGradeSwitcher(int In_gradeIndex, EGsSummonType In_summonType, bool In_isHigh);

	void StartAnimationRemainHighCard(float In_timeWait);
	// 뒷면 이펙트 이벤트 호출 안된것들 플레이
	void PlayNotCalledEventBackCardEffect();
	// 빠른 연타로 다시뽑기로 넘어갔는데 카드 애니메이션 콜백이 호출되어 버튼이 보이는 이슈 수정
	// clear 시점에 bind 된 animation callback 정리하기
	// https://jira.com2us.com/jira/browse/C2URWQ-6044
	void UnbindAnimation();

	// get
public:
	bool GetIsHighCard()
	{
		return _isHightCard;
	}
	bool GetIsHighSequenceCard()
	{
		return _isHighSequenceCard;
	}
	bool GetIsOpended()
	{
		return _isOpen;
	}

	UGsUISummonCardOpenHighEffect* GetFairyOpenCardEffect(FairyGrade In_grade);
	UGsUISummonCardOpenHighEffect* GetCostumeOpenCardEffect(CostumeGrade In_grade);

	bool GetIsCardAnimationStart()
	{
		return _isCardAnimationStart;
	}

	void SetTimeTickWaitAniPlay( float In_val)
	{
		_timeTickWaitAniPlay = In_val;
	}
};