// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialGuildContents.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Tutorial/Condition/GsTutorialConditionOpenUICheckParam.h"
#include "UI/UIContent/Window/GsUIWindowGuildMain.h"


bool FGsTutorialGuildContents::CheckIsValidToStart()
{
	if (false == Super::CheckIsValidToStart())
	{
		return false;
	}
	
	// 티커 대기 후 실행이 되는데, 그 사이 기사단 탭을 빠르게 이동 시, 튜토리얼 꼬임 현상 발생함(C2URWQ-5036)
	// Start 직전 현재 탭과 조건 탭이 동일한지 다시한 번 체크한다
	if (EGsTutorialConditionType::OPEN_UI_CHECK_PARAM == GetConditionType())
	{
		FGsTutorialConditionOpenUICheckParam* conditionOpenUI = 
			static_cast<FGsTutorialConditionOpenUICheckParam*>(_condition.Get());
		
		if (nullptr == conditionOpenUI)
		{
			return false;
		}

		// 조건에 스트링 체크가 있을 경우
		FString conditionString = conditionOpenUI->GetParamString();
		if (false == conditionString.IsEmpty())
		{
			UGsUIManager* uiMgr = GUI();
			if (nullptr == uiMgr)
			{
				return false;
			}

			// 해당 UI가 열려있지 않을 경우 false
			if (false == uiMgr->IsActiveWidget(conditionOpenUI->GetWidgetKey()))
			{
				return false;
			}

			TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->GetWidgetByKey(conditionOpenUI->GetWidgetKey());
			if (false == widget.IsValid())
			{
				return false;
			}

			// 주의: UGsUIWindowGuildMain일 때만 탭 체크 한다. 다른 길드 컨텐츠 UI가 이 클래스를 사용할 경우 예외처리 필요.
			if (UGsUIWindowGuildMain* window = Cast<UGsUIWindowGuildMain>(widget.Get()))
			{
				FString currTab = window->GetCurrentTabString();

				// 빈 탭 값이 넘어올 경우 false
				if (currTab.IsEmpty())
				{
					return false;
				}

				// 현재 탭과 튜토리얼 조건 탭이 서로 다르면 false
				if (currTab != conditionString)
				{
					return false;
				}
			}
		}
	}
	
	return true;
}

bool FGsTutorialGuildContents::OnPrevStart()
{
	// 가입된 길드가 없으면 튜토리얼 바로 종료	
	// 조건체크 단계에서 하지 않는 이유는, 길드 재가입 시 튜토리얼 시작되지 않도록
	if (false == GGuild()->IsGuild())
	{
		return false;
	}

	return Super::OnPrevStart();
}
