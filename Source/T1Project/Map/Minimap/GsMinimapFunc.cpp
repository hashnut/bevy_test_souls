#include "GsMinimapFunc.h"

#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsPathManager.h"

#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGame/GsPartyManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

#include "Quest/Management/GsQuestTargetManagement.h"

#include "GameObject/Quest/GsQuestHandler.h"
#include "GameObject/Data/GsGameObjectData.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "DataSchema/GameObject/Npc/GsSchemaNpcFunctionData.h"

#include "Party/GsPartyMemberInfo.h"

#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"

#include "UI/UILib/Container/GsWidgetPool.h"
#include "UI/UILib/Manager/GsWidgetPoolManager.h"

#include "UTIL/GsGameObjectUtil.h"

#include "Shared/Client/SharedEnums/SharedQuestEnum.h"

#include "UMG/Public/Blueprint/UserWidget.h"

EGsMapIconType FGsMinimapFunc::GetMapIconTypeByNpcFunctionType(NpcFunctionType In_npcFunctionType)
{
	EGsMapIconType findType = EGsMapIconType::None;
	switch (In_npcFunctionType)
	{
	case NpcFunctionType::PURCHASE:
		findType = EGsMapIconType::Purchase;
		break;
	case NpcFunctionType::REVIVAL:
		findType = EGsMapIconType::Revival;
		break;
	case NpcFunctionType::SHOP:
		findType = EGsMapIconType::Sale;
		break;
	case NpcFunctionType::DEPOT:
		findType = EGsMapIconType::Depot;
		break;
	case NpcFunctionType::AGIT_DUNGEON:
		findType = EGsMapIconType::Agit_Dungeon;
		break;
	}
	return findType;
}

// npc balance 테이블의 rank 타입으로 미니맵 아이콘 타입 구하는것
EGsMapIconType FGsMinimapFunc::GetMapIconTypeByCreatureRankType(
	CreatureRankType In_rankType, int In_npcTblId)
{
	EGsMapIconType findType = EGsMapIconType::None;
	switch (In_rankType)
	{
	case CreatureRankType::NORMAL:
	case CreatureRankType::ELITE:
	case CreatureRankType::NAMED:
	{
		// 퀘스트 몬스터인지 체크해본다
		if (FGsQuestHandler* questHandler = GSQuest()->GetQuestHandler())
		{
			if (FGsQuestTargetManagement* questTargetManagement = questHandler->GetQuestTargetManagement())
			{
				QuestType outQuestType;
				bool isQuestMonster = questTargetManagement->IsQuestMonster(In_npcTblId, outQuestType);

				// 퀘스트는 메인과 반복으로 표시
				if (isQuestMonster == true)
				{
					if (outQuestType == QuestType::REPEAT)
					{
						findType = EGsMapIconType::RepeatQuest;
					}
					else
					{
						findType = EGsMapIconType::MainQuest;
					}
				}
				else
				{
					findType = EGsMapIconType::Monster;
				}
			}
		}
	}
		break;
	case CreatureRankType::BOSS:
	case CreatureRankType::RAID:
		findType = EGsMapIconType::BossMonster;
		break;
	}
	return findType;
}

// npc function data 접근
const FGsSchemaNpcFunctionData* FGsMinimapFunc::GetNpcFunctionData(NpcFunctionType InType)
{
	const UGsTableNpcFunctionData* tbl = Cast<UGsTableNpcFunctionData>(FGsSchemaNpcFunctionData::GetStaticTable());
	if (nullptr == tbl)
	{
		return nullptr;
	}

	const FGsSchemaNpcFunctionData* findTbl = nullptr;
	if (false == tbl->FindRowById(InType, findTbl))
	{
		return nullptr;
	}

	return findTbl;
}

// 파티 인지(아이콘 타입도 가져오기)
 bool FGsMinimapFunc::GetPartyIconType(int64 In_gameId, EGsMapIconType& Out_iconType)
{	 
	 if (GSParty()->GetIsInParty() == false)
	 {
		 Out_iconType = EGsMapIconType::RemoteTeamRed;
		 return false;
	 }

	 FGsPartyMemberInfo* memberInfo = GSParty()->FindPartyMemberInfoByGameId(In_gameId);
	 if (nullptr == memberInfo)
	 {
		 Out_iconType = EGsMapIconType::RemoteTeamRed;
		 return false;
	 }
	 int slotId = memberInfo->GetPartySlotId();
	 int memberChannelId = memberInfo->GetChannelId();
	 int localChannelId = GGameData()->GetCurrentChannel();
	 bool isSameChannelWithLocal = (memberChannelId == localChannelId) ? true : false;
	 switch (slotId)
	 {
	 case 0:
		 Out_iconType = (isSameChannelWithLocal == true) ? EGsMapIconType::Party1 : EGsMapIconType::Party1_OtherChannel;
		 break;
	 case 1:
		 Out_iconType = (isSameChannelWithLocal == true) ? EGsMapIconType::Party2 : EGsMapIconType::Party2_OtherChannel;
		 break;
	 case 2:
		 Out_iconType = (isSameChannelWithLocal == true) ? EGsMapIconType::Party3 : EGsMapIconType::Party3_OtherChannel;
		 break;
	 case 3:
		 Out_iconType = (isSameChannelWithLocal == true) ? EGsMapIconType::Party4: EGsMapIconType::Party4_OtherChannel;
		 break;
	 }

	 return true;
}
 // check remote iff, red or blue icon
 void FGsMinimapFunc::GetRemoteIffIconType(int64 In_gameId, EGsMapIconType& Out_iconType)
 {
	 Out_iconType = EGsMapIconType::RemoteTeamRed;

	 UGsGameObjectBase* findObj =
		 GSGameObject()->FindObject(EGsGameObjectType::RemotePlayer, In_gameId);

	 if (findObj == nullptr)
	 {		 
		 return;
	 }
	
	 Out_iconType = (UGsGameObjectUtil::IsEnemy(findObj) == true) ? EGsMapIconType::RemoteTeamRed : EGsMapIconType::RemoteTeamBlue;
 }
 // 방향 표시 위젯 만들기
 UUserWidget* FGsMinimapFunc::MakeArrowWidget()
 {
	 UClass* widgetClass = nullptr;
	 if (UGsPathManager* pathMgr = GPath())
	 {
		 widgetClass = pathMgr->GetClassByObjectPath(GData()->GetGlobalData()->_arrowWidgetPath);
	 }

	 if (nullptr == widgetClass)
	 {
		 return nullptr;
	 }

	 UUserWidget* widget = Claim(widgetClass);
	 if (widget)
	 {
		 widget->AddToViewport();
	 }
	 return widget;
 }

 // 클래스로 위젯 생성
 UUserWidget* FGsMinimapFunc::Claim(UClass* In_class)
 {
	 UGsWidgetPoolManager* poolMgr = UGsBlueprintFunctionLibraryUI::GetWidgetPoolManager();
	 if (poolMgr == nullptr)
	 {
		 return nullptr;
	 }

	 UGsWidgetPool* pool = poolMgr->FindPool(In_class);
	 if (nullptr == pool)
	 {
		 pool = poolMgr->CreatePool(In_class);
		 if (nullptr == pool)
		 {
			 return nullptr;
		 }
	 }

	 return pool->Claim<UUserWidget>();
 }

 // 위젯 해제
 void FGsMinimapFunc::ReleaseWidget(UUserWidget* In_widget)
 {
	 if (In_widget == nullptr)
	 {
		 return;
	 }
	 UGsWidgetPoolManager* poolMgr = UGsBlueprintFunctionLibraryUI::GetWidgetPoolManager();
	 if (poolMgr == nullptr)
	 {
		 return;
	 }
	 if (poolMgr != nullptr)
	 {
		 In_widget->RemoveFromParent();
		 poolMgr->ReleaseWidget(In_widget->GetClass(), In_widget);
	 }
 }