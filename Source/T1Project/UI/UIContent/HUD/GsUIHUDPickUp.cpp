// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDPickUp.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsAIManager.h"

#include "Message/GsMessageGameObject.h"

#include "Message/MessageParam/GsInteractionMessageParam.h"

#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/Quest/GsQuestHandler.h"
#include "GameObject/ObjectClass/GsGameObjectDropObject.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/State/GsGameObjectStateManager.h"

#include "AI/Quest/GsAIQuestHandler.h"


void UGsUIHUDPickUp::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnPickUp->OnClicked.AddDynamic(this, &UGsUIHUDPickUp::OnClickPickUp);
}

void UGsUIHUDPickUp::OnClickPickUp()
{
	// TODO: 구현
	// 가장 가까우면서 룻 가능한 오브젝트 찾음
	bool isItemExist = false;
	if (UGsGameObjectDropObject* findDrop = GSGameObject()->FindDropObjectNearest())
	{
		// 메세지 Send
		FGsInteractionMessageParam messageParam(findDrop->GetGameId()
		, findDrop->GetObjectType()
		, findDrop->GetItemAmount()
		, false);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::DROPOBJECT_PICK, &messageParam);

		isItemExist = true;
	}
	else
	{
		isItemExist = false;
	}

	// 수동 루팅시 퀘스트 액티브면서 퀘스트 이동일때 
	// 퀘스트 액티브 끄기
	// https://jira.com2us.com/jira/browse/C2URWQ-8303
	UGsQuestManager* questManager = GSQuest();
	if (questManager == nullptr)
	{
		return;
	}

	FGsAIManager* aiMgr = GSAI();
	if (nullptr == aiMgr)
	{
		return;
	}
	FGsAIQuestHandler* aiQuestHandler = aiMgr->GetAIQuestHandler();
	if (nullptr == aiQuestHandler)
	{
		return;
	}
	// 퀘스트 액티브이면서 자동이동 데이터 있는데
	if (true == questManager->IsAutoProgressing() &&
		true == aiQuestHandler->IsReserveQuestMoveData())
	{
		if (UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
		{ 
			if(UGsGameObjectLocalPlayer* localPlayer = Cast<UGsGameObjectLocalPlayer>(local))
			{ 
				if (FGsQuestHandler* questHandler = localPlayer->GetQuestHandler())
				{
					questHandler->SetAutoMoveStop();

					// 아이템이 없으면(루팅하러 안가면)
					// 그냥 이동을 멈추게 하자
					if (isItemExist == false)
					{
						bool isAutoMove =  localPlayer->GetBaseFSM()->IsState(EGsStateBase::AutoMove);
						if(isAutoMove == true)
						{ 
							// 이동 예약 데이터 삭제
							localPlayer->ClearReserveDataByReserveWork();
							// 상태 auto move stop 으로 변경
							localPlayer->GetBaseFSM()->ProcessEvent(EGsStateBase::AutoMoveStop);
						}
					}
				}
			}
		}
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}
