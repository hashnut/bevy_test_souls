#include "GsUISummonCardItem.h"

#include "Fairy/GsFairyData.h"

#include "Costume/GsCostumeData.h"

#include "Summon/GsSummonCardData.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsSoundManager.h"

#include "Management/ScopeGame/GsSummonManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

#include "UTIL/GsTableUtil.h"

#include "Message/GsMessageSystem.h"
#include "Message/MessageParam/GsMessageParam.h"

#include "GameFlow/GameContents/ContentsGame/Hud/Summon/GsSummonHandler.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"
#include "Data/GsGameClientVersion.h"

#include "Runtime/DataCenter/Public/DataSchema/Summon/GsSchemaSummonCostumeAppearResPath.h"

#include "Sound/GsSoundPlayer.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Tray/Summon/GsUISummonCardOpenHighEffect.h"

#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"

#include "Shared/Client/SharedEnums/SharedFairyEnum.h"

#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Client/SharedEnums/SharedCostumeEnum.h"

#include "Components/Image.h"

#include "Paper2D/Classes/PaperSprite.h"

#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/SlateWrapperTypes.h"
#include "UMG/Public/Animation/WidgetAnimation.h"
#include "UMG/Public/Components/Widget.h"
#include "UMG/Public/Components/WidgetSwitcher.h"

#include "Core/Public/Math/Color.h"
#include "Core/Public/Misc/DateTime.h"


// 데이터 세팅
void UGsUISummonCardItem::SetData(EGsSummonType In_type, FGsSummonCardData* In_data, float In_timeWait, bool In_isHighCard)
{
	_isOpen = false;

	if (In_data == nullptr)
	{
		return;
	}

	_isHightCard = In_isHighCard;

	_dataIndex = In_data->_dataIndex;

	int gradeIndex = 0;

	_summonType = In_type;

	// 새로운 카드인가(new 이미지 표시)
	bool isNewCard = false;

	FGsSummonHandler* summonHandler = UGsSummonManager::GetSummonHandler();
	if (summonHandler == nullptr)
	{
		return;
	}

	
	if (In_type == EGsSummonType::SUMMON_FAIRY)
	{
		FGsSummonCardDataFairy* fairyCardData = static_cast<FGsSummonCardDataFairy*>(In_data);

		if (fairyCardData == nullptr ||
			fairyCardData->_data.IsValid() == false)
		{
			return;
		}

		// 이름 세팅
		_textFairyName = fairyCardData->_data->GetFairyName();

		// 초상화 이미지 세팅
		UPaperSprite* spriteImg = FGsUIHelper::LoadSpriteFromPath(fairyCardData->_data->GetFairyPortraitPath());
		_imgPortrait->SetBrushFromAtlasInterface(spriteImg);

		FairyGrade fairyGrade = fairyCardData->_data->GetFairyGrade();

		gradeIndex = (int)fairyGrade - 1;

		_isHighSequenceCard = ((int)fairyGrade >= (int)FairyGrade::EPIC) ? true : false;

		if (_isHighSequenceCard == true &&
			GData()->GetGlobalData()->_summonFairyHighGetSequenceId.Num() == 3)
		{
			
			switch (fairyGrade)
			{
			case FairyGrade::EPIC:
			{
				_highSequenceId = GData()->GetGlobalData()->_summonFairyHighGetSequenceId[0];
				_currentHighGetPreEffectIndex = 0;
			}
			break;
			case FairyGrade::UNIQUE:
			{
				_highSequenceId = GData()->GetGlobalData()->_summonFairyHighGetSequenceId[1];
				_currentHighGetPreEffectIndex = 1;
			}
			break;
			case FairyGrade::LEGEND:
			{				
				_highSequenceId = GData()->GetGlobalData()->_summonFairyHighGetSequenceId[2];
				_currentHighGetPreEffectIndex = 2;
			}
			break;
			}
		}

		// bg 색상
		FLinearColor bgColor = FGsUIColorHelper::GetColorSummonFairyGradeBG(fairyGrade);
		_imgBg->SetColorAndOpacity(bgColor);
		_imgBackGradeEffect->SetColorAndOpacity(bgColor);
		_imgFrontGradeEffect->SetColorAndOpacity(bgColor);


		// text 색상
		_textBlockFairyName->SetColorAndOpacity(FGsUIColorHelper::GetColorSummonFairyGradeText(fairyGrade));

		if (_isHightCard == true)
		{
			if (UGsUISummonCardOpenHighEffect* cardOpenEffect = GetFairyOpenCardEffect(fairyGrade))
			{
				cardOpenEffect->SetAnimationFirstPos();
				_currentOpenEffect = cardOpenEffect;
			}

			// 애니메이션 위치 처음으로
			_fairyHighBackCardEffect->SetAnimationFirstPos();
		}
		else
		{
			// 저등급 카드도 출력
			// https://jira.com2us.com/jira/browse/CHR-21031
			// 애니메이션 위치 처음으로
			_fairyNormalBackCardEffect->SetAnimationFirstPos();
		}
		
		isNewCard = fairyCardData->_data->IsNewSummon();		
	}
	else
	{
		FGsSummonCardDataCostume* costumeCardData = static_cast<FGsSummonCardDataCostume*>(In_data);

		if (costumeCardData == nullptr ||
			costumeCardData->_data.IsValid() == false)
		{
			return;
		}
		_textFairyName = costumeCardData->_data->GetCostumeName();

		CreatureGenderType localGender = summonHandler->GetLocalGender();

		// 초상화 이미지 세팅
		UPaperSprite* spriteImg = FGsUIHelper::LoadSpriteFromPath(costumeCardData->_data->GetCostumePortraitPath(localGender));
		_imgPortrait->SetBrushFromAtlasInterface(spriteImg);

		CostumeGrade costumeGrade = costumeCardData->_data->GetCostumeGrade();

		gradeIndex = (int)costumeGrade - 1;

		_isHighSequenceCard = ((int)costumeGrade >= (int)CostumeGrade::EPIC) ? true : false;

		if (_isHighSequenceCard == true &&
			GData()->GetGlobalData()->_summonCostumeHighGetSequenceId.Num() == 3)
		{

			switch (costumeGrade)
			{
			case CostumeGrade::EPIC:
			{
				_highSequenceId = GData()->GetGlobalData()->_summonCostumeHighGetSequenceId[0];
				_currentHighGetPreEffectIndex = 0;
			}
			break;
			case CostumeGrade::UNIQUE:
			{
				_highSequenceId = GData()->GetGlobalData()->_summonCostumeHighGetSequenceId[1];
				_currentHighGetPreEffectIndex = 1;
			}
			break;
			case CostumeGrade::LEGEND:
			{
				// 뽑기 레전드 코스튬: 등장 시네마틱 분기(리소스 9월 말 앱빌드에 안들어가서 막는처리) 
				// https://jira.com2us.com/jira/browse/CHR-22631
 
				const FGsSchemaSummonCostumeAppearResPath* findTbl =
					UGsTableUtil::FindRowById<UGsTableSummonCostumeAppearResPath, FGsSchemaSummonCostumeAppearResPath>(costumeCardData->_data->_id);

				if (findTbl == nullptr)
				{
					return;
				}

				if (findTbl->appearSequenceId.Num() == 0)
				{
					return;
				}

				// 1개는 바로 처리

				if (findTbl->appearSequenceId.Num() == 1)
				{
					_highSequenceId = findTbl->appearSequenceId[0];
				}
				// 복수개는 랜덤
				// https://jira.com2us.com/jira/browse/CHR-24523
				else
				{
					// 인덱스 랜덤값
					int randomIndex = FMath::RandRange(0, findTbl->appearSequenceId.Num() - 1);
					_highSequenceId = findTbl->appearSequenceId[randomIndex];
				}

				
				_currentHighGetPreEffectIndex = 2;
			}
			break;
			}
		}

		// bg 색상
		FLinearColor bgColor = FGsUIColorHelper::GetColorSummonCostumeGradeBG(costumeGrade);
		_imgBg->SetColorAndOpacity(bgColor);
		_imgBackGradeEffect->SetColorAndOpacity(bgColor);
		_imgFrontGradeEffect->SetColorAndOpacity(bgColor);

		// text 색상
		_textBlockFairyName->SetColorAndOpacity(FGsUIColorHelper::GetColorSummonCostumeGradeText(costumeGrade));

		if (_isHightCard == true)
		{
			if (UGsUISummonCardOpenHighEffect* cardOpenEffect = GetCostumeOpenCardEffect(costumeGrade))
			{
				cardOpenEffect->SetAnimationFirstPos();
				_currentOpenEffect = cardOpenEffect;
			}
			// 애니메이션 위치 처음으로
			_costumeHighBackCardEffect->SetAnimationFirstPos();
			
		}
		else
		{
			// 저등급 카드도 출력
			// https://jira.com2us.com/jira/browse/CHR-21031
			// 애니메이션 위치 처음으로
			_costumeNormalBackCardEffect->SetAnimationFirstPos();
		}

		isNewCard = costumeCardData->_data->IsNewSummon();
	}


	SetGradeSwitcher(gradeIndex, In_type, _isHightCard);


	// 애니 연출
	_timeSecondWaitAniPlay = In_timeWait;
	_timeTickWaitAniPlay = _timeSecondWaitAniPlay * ETimespan::TicksPerSecond;
	
	_isWaitEnd = true;

	// 한번 클리어 해준다
	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);

	float endTime = _animation->GetEndTime();
	PlayAnimation(_animation, endTime, 1, EUMGSequencePlayMode::Reverse);

	_isCardAnimationStart = false;

	ESlateVisibility newImageVisible = (isNewCard == true)? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden;
	_newImage->SetVisibility(newImageVisible);

	// 초기화
	_isPlayedBackCardEffect = false;

	// 스케일된 연출이(커진거에서 작아지는 연출)
	// 아래 스킵 버튼을 막아서 일단 collision off
	_btnClick->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UGsUISummonCardItem::NativeConstruct()
{
	Super::NativeConstruct();

	float endTime = _animation->GetEndTime();
	PlayAnimation(_animation, endTime, 1, EUMGSequencePlayMode::Reverse);

}

void UGsUISummonCardItem::StartTimmer()
{
	_isWaitEnd = false;
	_startTime = FDateTime::UtcNow().GetTicks();	
}
// 연출 끝
void UGsUISummonCardItem::FinishAnimationBP()
{
	UnbindAnimation();

	if (_callbackFinishCardOpenAni != nullptr)
	{
		_callbackFinishCardOpenAni();
	}	
}

void UGsUISummonCardItem::Tick(float InDeltaTime)
{
	if (_isWaitEnd == true)
	{
		return;
	}

	int64 nowTick = FDateTime::UtcNow().GetTicks();
	int64 diffTick = nowTick - _startTime;

	if (diffTick >= _timeTickWaitAniPlay)
	{
		_isWaitEnd = true;
		StartAnimation();
	}

}

// 애니매이션 시작
void UGsUISummonCardItem::StartAnimation()
{	
	PlayAnimation(_animation);

	FScriptDelegate animationDelegate;
	animationDelegate.BindUFunction(this, "FinishAnimationBP");

	BindToAnimationFinished(_animation, FWidgetAnimationDynamicEvent(animationDelegate));	

	// 카드 떨어지는 소리
	UGsSoundPlayer* soundPlayer = GSoundPlayer();
	if (nullptr == soundPlayer)
		return;

	FString cardOpenSound = 
		(_isHightCard == true ) ? 
		"Snd_wav_UI_Gacha_CardDrop_High" : "Snd_wav_UI_Gacha_CardDrop";

	soundPlayer->PlaySound2D(cardOpenSound);

	if (_isHightCard == true && 
		_currentOpenEffect != nullptr)
	{		
		_currentOpenEffect->PlayEffectAnimation();
	}

	_isCardAnimationStart = true;

	_isOpen = true;
}

void UGsUISummonCardItem::NativeDestruct()
{
	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);
	Super::NativeDestruct();
}

void UGsUISummonCardItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnClick->OnClicked.AddDynamic(this, &UGsUISummonCardItem::OnClickCard);
}
void UGsUISummonCardItem::OnClickCard()
{
	if (_isOpen == true ||
		_isHightCard == false)
	{
		return;
	}

	_isOpen = true;

	// 테미르 시네마틱 연출 쓸지(8/9빌드에는 안들어감)
	bool isShowTemirSequence = GData()->GetGlobalData()->_isShowTemirSequence;

	// 이전 연출이면
	if(isShowTemirSequence == false)
	{ 
		if (_isHighSequenceCard == true)
		{
			// save data index
			GSSummon()->SetCurrentHighGetDataIndex(_dataIndex);

			UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);

			if (player == nullptr)
			{
				return;
			}

			FVector localPos = player->GetLocation();

			// make or get scene object
			AActor* sceneActor = GSSummon()->SpawnSceneObjectByType(EGsSummonSceneType::HighGet, localPos);
			// show sequence
			// 저사양 ios 메모리 부족 크래쉬 이슈
			// https://jira.com2us.com/jira/browse/C2URWQ-6794
			bool isStore = IsBranch(EGsGameClientBranchType::SUMMON_CINEMATIC_IOS_MEMORY_ISSUE) ? false : true;
			FGsSequenceMessageParam param(_highSequenceId, sceneActor, isStore, true);
			GMessage()->GetSystemParam().SendMessage(MessageSystem::SEQUENCE_PLAYER_START, &param);

			GSSummon()->SetIsPlaySequence(true);
		}
		else
		{
			StartAnimation();
		}
	}
	else
	{ 
		if (_isHightCard == true)
		{
			// 데이터 저장 해놓기
			GSSummon()->SetCurrentHighGetDataIndex(_dataIndex);

			int preHighSequenceId = 0;
			// 에픽이상 테미르 쾅까지
			if (_isHighSequenceCard == true)
			{
				GSSummon()->SetCurrentHighGetPreEffectIndex(_currentHighGetPreEffectIndex);

				FGsSummonHandler* summonHandler = UGsSummonManager::GetSummonHandler();
				if (summonHandler == nullptr)
				{
					return;
				}

				// 고등급 획득 연출 정보 handler에 저장
				// 원래는 카드에 있던 정보인데(클릭할때 출력용)
				// 획득 전 시네마틱(테미르 쾅) 추가 및 모두 획득 클릭시 시네마틱 출력 flow 변경으로
				summonHandler->SetHighGetSequenceInfo(_isHighSequenceCard, _highSequenceId);
			
				preHighSequenceId = GData()->GetGlobalData()->_summonHighPreSequenceId_OverEpic;
			}
			// 레어면 테미르 돌리고 돌리고 까지만
			else
			{
				preHighSequenceId = GData()->GetGlobalData()->_summonHighPreSequenceId_Rare;
			}

			UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);

			if (player == nullptr)
			{
				return;
			}

			FVector localPos = player->GetLocation();

			// make or get scene object
			AActor* sceneActor = GSSummon()->SpawnSceneObjectByType(EGsSummonSceneType::HighPre, localPos);
			// show sequence
			// 저사양 ios 메모리 부족 크래쉬 이슈
			// https://jira.com2us.com/jira/browse/C2URWQ-6794
			bool isStore = IsBranch(EGsGameClientBranchType::SUMMON_CINEMATIC_IOS_MEMORY_ISSUE) ? false : true;
			FGsSequenceMessageParam param(preHighSequenceId, sceneActor, isStore, true);
			GMessage()->GetSystemParam().SendMessage(MessageSystem::SEQUENCE_PLAYER_START, &param);

			GSSummon()->SetIsPlaySequence(true);
		}
	}


}

// grade switch
void UGsUISummonCardItem::SetGradeSwitcher(int In_gradeIndex, EGsSummonType In_summonType, bool In_isHigh)
{
	_switcherGradeBack->SetActiveWidgetIndex(In_gradeIndex);

	// 0: fairy normal, 1: fairy magic, 2: costume normal, 3: costume magic
	int coverSwitcherIndex = -1;

	if (In_summonType == EGsSummonType::SUMMON_FAIRY)
	{
		coverSwitcherIndex = (In_isHigh == false) ? 0 : 1;
		// hide costume grade
		_switcherCostumeGradeFrame->SetVisibility(ESlateVisibility::Hidden);

		// show fairy grade
		_switcherFariryGradeFrame->SetActiveWidgetIndex(In_gradeIndex);
		_switcherFariryGradeFrame->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		coverSwitcherIndex = (In_isHigh == false) ? 2 : 3;
		// hide fairy grade
		_switcherFariryGradeFrame->SetVisibility(ESlateVisibility::Hidden);

		// show costume grade
		_switcherCostumeGradeFrame->SetActiveWidgetIndex(In_gradeIndex);
		_switcherCostumeGradeFrame->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	_switcherCoverColor->SetActiveWidgetIndex(coverSwitcherIndex);
}

void UGsUISummonCardItem::StartAnimationRemainHighCard(float In_timeWait)
{
	// 애니 연출
	_timeSecondWaitAniPlay = In_timeWait;
	_timeTickWaitAniPlay = _timeSecondWaitAniPlay * ETimespan::TicksPerSecond;

	StartTimmer();
}

UGsUISummonCardOpenHighEffect* UGsUISummonCardItem::GetFairyOpenCardEffect(FairyGrade In_grade)
{
	int openEffectIndex = 0;
	// https://jira.com2us.com/jira/browse/CHR-20299
	// 0: epic
	// 1: unique
	// 2: legend

	// 유저들이 래어 뽑으면 대우 안해준다고 해서 대응
	//https://jira.com2us.com/jira/browse/CHR-20882
	// 0: rare
	// 1: epic
	// 2: unique
	// 3: legend

	switch (In_grade)
	{
	case FairyGrade::RARE:
	{
		openEffectIndex = 0;
	}
	break;
	case FairyGrade::EPIC:
	{
		openEffectIndex = 1;
	}
	break;
	case FairyGrade::UNIQUE:
	{
		openEffectIndex = 2;
	}
	break;
	case FairyGrade::LEGEND:
	{
		openEffectIndex = 3;
	}
	break;
	}
	if (_arrayFairyOpenEffect.Num() < 4)
	{
		return nullptr;
	}

	return 	_arrayFairyOpenEffect[openEffectIndex];	
}
UGsUISummonCardOpenHighEffect* UGsUISummonCardItem::GetCostumeOpenCardEffect(CostumeGrade In_grade)
{
	int openEffectIndex = 0;
	// https://jira.com2us.com/jira/browse/CHR-20299
	// 0: epic
	// 1: unique
	// 2: legend

	// 유저들이 래어 뽑으면 대우 안해준다고 해서 대응
	//https://jira.com2us.com/jira/browse/CHR-20882
	// 0: rare
	// 1: epic
	// 2: unique
	// 3: legend

	switch (In_grade)
	{
	case CostumeGrade::RARE:
	{
		openEffectIndex = 0;
	}
	break;
	case CostumeGrade::EPIC:
	{
		openEffectIndex = 1;
	}
	break;
	case CostumeGrade::UNIQUE:
	{
		openEffectIndex = 2;
	}
	break;
	case CostumeGrade::LEGEND:
	{
		openEffectIndex = 3;
	}
	break;
	}
	if (_arrayCostumeOpenEffect.Num() < 4)
	{
		return nullptr;
	}

	return 	_arrayCostumeOpenEffect[openEffectIndex];
}
// 타임라인 특정 시점에 고등급 뒷면 카드 이펙트 출력 처리
void UGsUISummonCardItem::PlayHighBackCardEffect()
{
	// visibility 꺼져있으면 처리 안함
	ESlateVisibility nowVisibility = GetVisibility();
	if (nowVisibility == ESlateVisibility::Collapsed)
	{
		return;
	}

	if (_isHightCard == true)
	{
		if (_summonType == EGsSummonType::SUMMON_FAIRY)
		{
			// 플레이 애니메이션
			_fairyHighBackCardEffect->PlayEffectAnimation();		
		}
		else
		{
			// 플레이 애니메이션
			_costumeHighBackCardEffect->PlayEffectAnimation();
		}
	}
	// 저등급 카드도 출력
	// https://jira.com2us.com/jira/browse/CHR-21031
	else
	{
		if (_summonType == EGsSummonType::SUMMON_FAIRY)
		{
			// 플레이 애니메이션
			_fairyNormalBackCardEffect->PlayEffectAnimation();
		}
		else
		{
			// 플레이 애니메이션
			_costumeNormalBackCardEffect->PlayEffectAnimation();
		}
	}

	UGsSoundPlayer* soundPlayer = GSoundPlayer();
	if (nullptr == soundPlayer)
		return;

	FString cardDownSound =
		(_isHightCard == true) ?
		"UI_Gacha_CardDown_High" : "UI_Gacha_CardDown";

	// 일반 카드 드랍 사운드 : UI_Gacha_CardDown
	// 고등급 카드 드랍 사운드 : UI_Gacha_CardDown_High
	// https://jira.com2us.com/jira/browse/CHR-20871
	soundPlayer->PlaySound2D(cardDownSound);

	_isPlayedBackCardEffect = true;

	// 스케일된 연출이(커진거에서 작아지는 연출)
	// 아래 스킵 버튼을 막아서 일단 collision off
	// 연출 끝나고 on
	_btnClick->SetVisibility(ESlateVisibility::Visible);
}

// 뒷면 이펙트 이벤트 호출 안된것들 플레이
void UGsUISummonCardItem::PlayNotCalledEventBackCardEffect()
{
	if (_isPlayedBackCardEffect == true)
	{
		return;
	}
	PlayHighBackCardEffect();
}

// 빠른 연타로 다시뽑기로 넘어갔는데 카드 애니메이션 콜백이 호출되어 버튼이 보이는 이슈 수정
// clear 시점에 bind 된 animation callback 정리하기
// https://jira.com2us.com/jira/browse/C2URWQ-6044
void UGsUISummonCardItem::UnbindAnimation()
{
	FScriptDelegate animationDelegate;
	animationDelegate.BindUFunction(this, "FinishAnimationBP");

	UnbindFromAnimationFinished(_animation, FWidgetAnimationDynamicEvent(animationDelegate));
}