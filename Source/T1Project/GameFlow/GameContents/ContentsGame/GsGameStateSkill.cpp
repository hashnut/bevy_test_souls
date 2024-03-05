// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameStateSkill.h"

#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGame/GsSkillPreviewManager.h"
#include "Management/ScopeGame/GsEventProgressManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsTutorialManager.h"

#include "Message/MessageParam/GsUIMessageParam.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"

#include "UI/UIContent/Window/GsUIWindowSkill.h"
#include "UI/UIContent/Popup/GsUIPopupSkillEnchant.h"
#include "UI/UIContent/Tray/GsUITraySkillEnchantWait.h"
#include "UI/UIContent/Tray/GsUITraySkillEnchantResult.h"

#include "DataSchema/Skill/Level/GsSchemaSkillLevelSet.h"


FGsGameStateSkill::FGsGameStateSkill() : FGsContentsGameBase(FGsContentsMode::InGame::ContentsSkill)
{
}

FGsGameStateSkill::~FGsGameStateSkill()
{
}

void FGsGameStateSkill::Enter()
{
	FGsContentsGameBase::Enter();

	RegisterMessages();

	//지형 꺼짐현상 제거
	UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (player)
	{
		GLevel()->HoldStreamingLevelByLocation(player->GetLocation());
	}

	// 튜토리얼일 경우 EventProgress 우선순위 무시
	GSEventProgress()->OnReqStartEvent(EGsEventProgressType::SKILL_PREVIEW_ROOM, GSSkillPreview(), 
		GSTutorial()->IsActiveTutorial());
	
	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->SetHideFlags(EGsUIHideFlags::STATE_SCENE_WINDOW);
		uiMgr->Open(TEXT("WindowSkill"));
	}

	// 네비게이션 바 재화 세팅 (드롭다운 메뉴 감춤, 선택 슬롯 감춤)
	SetNavigationBar(CurrencyType::NONE);

	// 환경효과 이벤트 전달(이전 환경효과가 페어리 화면에 영향주지 않게 하기 위함)
	FGsPrimitiveInt32 param(static_cast<int32>(EGsEnvEvent::FAIRY_ROOM));
	GMessage()->GetGameObject().SendMessage(MessageGameObject::ENV_START_EVENT, &param);
}

void FGsGameStateSkill::Exit()
{
	DeregisterMessages();

	if (UGsSkillPreviewManager* skillPreviewMgr = GSSkillPreview())
	{
		skillPreviewMgr->DespawnAllGameObject();
		skillPreviewMgr->DespawnSceneObject();
	}

	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->ClearHideFlags(EGsUIHideFlags::STATE_SCENE_WINDOW);
	}

	if (_trayEnchantWait.IsValid())
	{
		_trayEnchantWait->Close();
	}
	_trayEnchantWait = nullptr;		

	if (FGsMessageHolder* messageMgr = GMessage())
	{
		// 환경효과 이벤트 전달(이전 환경효과로 복원 위함)
		FGsPrimitiveInt32 param(static_cast<int32>(EGsEnvEvent::FAIRY_ROOM));
		messageMgr->GetGameObject().SendMessage(MessageGameObject::ENV_END_EVENT, &param);

		// HUD 스킬버튼 갱신요청
		messageMgr->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_SKILL);

		// 레드닷 갱신
		messageMgr->GetHudRedDot().SendMessage(MessageContentHUDRedDot::SKILL, false);
	}

	_enchantSkillId = INVALID_SKILL_ID;
	_resultSkillId = INVALID_SKILL_ID;	
	_enchantResult = SkillEnchantResult::NONE;
	_bIsEnchantProgressEnd = false;

	// 스킬창에서 게임종료할 경우 크래시 날 수 있으므로 매니저들은 널체크 필수
	if (FGsEventProgressManager* eventProgressMgr = GSEventProgress())
	{
		eventProgressMgr->OnReqFinishEvent(EGsEventProgressType::SKILL_PREVIEW_ROOM);
	}

	FGsContentsGameBase::Exit();
}

void FGsGameStateSkill::OnReconectionEnd()
{
	// Reconnect 시 CloseAllStack을 하는데 굳이 여기서 닫아주는 이유는
	// 창에서 캐싱해 둔 스킬의 포인터들로 인해 문제(쓰레기 포인터로 인한 크래시)가 될 수 있어 미리 닫는 것
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(TEXT("WindowSkill"));
	if (widget.IsValid())
	{
		if (UGsUIWindowSkill* window = Cast<UGsUIWindowSkill>(widget.Get()))
		{
			window->Close();
		}
	}

	// 강화연출 도중 끊어졌다가 복구되었음
	if (_trayEnchantWait.IsValid())
	{
		// 패킷을 이미 보냈고 연출중인 상태이면, 응답을 못받을 것이므로 창을 바로 제거.
		if (_trayEnchantWait->IsProgressState())
		{
			OpenResult();
		}
	}
}

void FGsGameStateSkill::RegisterMessages()
{
	FGsMessageHolder* message = GMessage();

	MUI& msgUI = message->GetUI();
	_msgUIHandleList.Emplace(msgUI.AddRaw(MessageUI::SKILL_ENCHANT_EFFECT_START,
		this, &FGsGameStateSkill::OnSkillEnchantEffectStart));
	_msgUIHandleList.Emplace(msgUI.AddRaw(MessageUI::SKILL_ENCHANT_EFFECT_RESULT,
		this, &FGsGameStateSkill::OnSkillEnchantEffectResult));
	_msgUIHandleList.Emplace(msgUI.AddRaw(MessageUI::SKILL_ENCHANT_EFFECT_END_PROGRESS,
		this, &FGsGameStateSkill::OnSkillEnchantEffectEndProgress));
}

void FGsGameStateSkill::DeregisterMessages()
{
	FGsMessageHolder* message = GMessage();

	MUI& msgUI = message->GetUI();
	for (MsgUIHandle& msgUIHandle : _msgUIHandleList)
	{
		msgUI.Remove(msgUIHandle);
	}
	_msgUIHandleList.Empty();
}

void FGsGameStateSkill::OnSkillEnchantEffectStart(const IGsMessageParam* InParam)
{
	// 결과 창 닫을 때 리셋은 하지만 한 번 더 진행
	_enchantResult = SkillEnchantResult::NONE;

	const FGsUIMsgParamEnchantSkill* param = InParam->Cast<const FGsUIMsgParamEnchantSkill>();
	if (param)
	{
		_enchantSkillId = param->_skillId;
		TWeakObjectPtr<UGsUIWidgetBase> widgetTray = GUI()->OpenAndGetWidget(TEXT("TraySkillEnchantWait"));
		if (widgetTray.IsValid())
		{
			_trayEnchantWait = Cast<UGsUITraySkillEnchantWait>(widgetTray.Get());
			if (_trayEnchantWait.IsValid())
			{
				_trayEnchantWait->SetData(_enchantSkillId);
			}
		}
	}
}

void FGsGameStateSkill::OnSkillEnchantEffectResult(const IGsMessageParam* InParam)
{
	// param: SkillEnchantResult
	const FGsUIMsgParamSkillEnchantResult* param = InParam->Cast<const FGsUIMsgParamSkillEnchantResult>();
	if (param)
	{
		_enchantResult = param->_result;
		_enchantSkillId = param->_skillId;
		_resultSkillId = param->_resultSkillId;
		
		if (SkillEnchantResult::NONE != _enchantResult)
		{
			if (_bIsEnchantProgressEnd)
			{
				OpenResult();
			}
		}
		else
		{
			// 정보 없으면 함수 내부에서 닫기, 변수 리셋 등 진행
			OpenResult();
		}
	}
}

void FGsGameStateSkill::OnSkillEnchantEffectEndProgress(const IGsMessageParam*)
{
	_bIsEnchantProgressEnd = true;

	if (SkillEnchantResult::NONE != _enchantResult)
	{
		OpenResult();
	}
}

void FGsGameStateSkill::OpenResult()
{
	bool bIsSuccess = false;
	TWeakObjectPtr<UGsUIWidgetBase> widget;
	if (SkillEnchantResult::SUCCESS == _enchantResult)
	{
		widget = GUI()->OpenAndGetWidget(TEXT("TraySkillEnchantSuccess"));
		bIsSuccess = true;
	}
	else if (SkillEnchantResult::FAILED == _enchantResult)
	{
		widget = GUI()->OpenAndGetWidget(TEXT("TraySkillEnchantFail"));
		bIsSuccess = false;
	}

	// 성공/실패 정보 있을때만 연다. 
	if (widget.IsValid())
	{
		UGsUITraySkillEnchantResult* resultUI = Cast<UGsUITraySkillEnchantResult>(widget.Get());
		if (resultUI)
		{
			if (const FGsSkill* skill = GetResultSkill())
			{
				bool bIsUpgrade = (_enchantSkillId != _resultSkillId) ? true : false;
				resultUI->SetData(skill, bIsUpgrade, bIsSuccess);
			}
		}
	}

	// 강화팝업 갱신(아이템 수량, 레벨등의 갱신 위함)
	TWeakObjectPtr<UGsUIWidgetBase> popupWidget = GUI()->GetWidgetByKey(TEXT("PopupSkillEnchant"));
	if (popupWidget.IsValid())
	{
		if (UGsUIPopupSkillEnchant* popup = Cast<UGsUIPopupSkillEnchant>(popupWidget.Get()))
		{
			if (bIsSuccess)
			{
				if (const FGsSkill* skill = GetResultSkill())
				{
					popup->SetData(skill);
				}
				else
				{
					popup->InvalidateAll();
				}
			}
			else
			{
				popup->InvalidateAll();
			}
		}
	}

	// 결과 정보 리셋
	_enchantSkillId = INVALID_SKILL_ID;
	_resultSkillId = INVALID_SKILL_ID;
	_enchantResult = SkillEnchantResult::NONE;
	_bIsEnchantProgressEnd = false;

	// Wait 창 닫기
	if (_trayEnchantWait.IsValid())
	{
		_trayEnchantWait->Close();		
	}
	_trayEnchantWait = nullptr;
}

const FGsSkill* FGsGameStateSkill::GetResultSkill() const
{
	if (UGsSkillManager* skillMgr = GSkill())
	{
		return skillMgr->FindSkill(_resultSkillId, CreatureWeaponType::MAX, true);
	}

	return nullptr;
}