#include "GsUINPCList.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/HUD/Minimap/GsUINPCListItem.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGame/GsInteractionManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsAIManager.h"



#include "AI/GsAIStateManager.h"
#include "AI/Data/GsAIReserveJob.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "GameObject/Interaction/GsInteractionHandlerLocalPlayer.h"

#include "GameObject/AI/GsAIReserveHandler.h"

#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"

#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"

#include "UTIL/GsTableUtil.h"
#include "UTIL/GsText.h"

#include "Map/Minimap/GsMinimapFunc.h"
#include "GameObject/Interaction/GsInteractionNpcInfo.h"

#include "UMG/Public/Components/ScrollBox.h"
#include "UMG/Public/Components/VerticalBox.h"



void UGsUINPCList::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_scrollBoxHelper->Initialize(_entryWidgetClass, _verticalBox);
	_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUINPCList::OnRefreshEntry);
}

void UGsUINPCList::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	int npcId = 0;
	FString npcName = "";
#ifdef OLD_NPC_FUNCTION_LIST
	TArray<FGsTownInteractionNpcInfo*> arrInfo = GSInteraction()->GetNowShowTownNpcListData();

	if (arrInfo.Num() <= InIndex)
	{
		return;
	}

	FGsTownInteractionNpcInfo* info = arrInfo[InIndex];
	npcId = info->_npcId;

	//NonPlayer 데이터 얻기
	const FGsSchemaNpcData* npcData = UGsTableUtil::FindRowById<UGsTableNpcData, FGsSchemaNpcData>(npcId);
	if (nullptr == npcData)
	{
		return;
	}
	npcName = npcData->nameText.ToString();

	// 클라 스폰에게 현재 맵에서 출력할 npc list 데이터 얻어오기
	UGsUINPCListItem* item = Cast<UGsUINPCListItem>(InEntry);
	if (item)
	{
		// 아이템에 데이터 세팅
		// 아이콘정보, 이름, npc id

		// 클릭시 npc id를 interaction manager에 던진다
		item->SetData(npcId, npcName, npcData->npcFunctionType);
	}
#endif

	FGsAIStateManager* aiStateManager = GSAI()->GetAIStateManager();
	if (aiStateManager == nullptr)
	{
		return;
	}

	FGsAIReserveHandler* aiReserveHandler = aiStateManager->GetAIReserveHandler();
	if (aiReserveHandler == nullptr)
	{
		return;
	}

	TArray<FGsAIReserveJob*> arrayReserveJob = aiReserveHandler->GetArrayReserveJob();

	if (arrayReserveJob.Num() <= InIndex)
	{
		return;
	}

	FGsAIReserveJob* reserveJob = arrayReserveJob[InIndex];

	if (reserveJob == nullptr)
	{
		return;
	}

	UGsUINPCListItem* item = Cast<UGsUINPCListItem>(InEntry);
	if (item)
	{
		FString enumString = FGsTextUtil::GetStringFromEnum(TEXT("EGsAIActionType"), reserveJob->_actionType);
		item->SetData(enumString);
	}
}

void UGsUINPCList::BeginDestroy()
{
	if (nullptr != _scrollBoxHelper)
	{
		_scrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUINPCList::OnRefreshEntry);
		_scrollBoxHelper = nullptr;
	}

	Super::BeginDestroy();
}

// 디버그용 리스트 데이터 세팅
void UGsUINPCList::SetDebugListData()
{
	FGsAIStateManager* aiStateManager = GSAI()->GetAIStateManager();
	if (aiStateManager == nullptr)
	{
		return;
	}

	FGsAIReserveHandler* aiReserveHandler = aiStateManager->GetAIReserveHandler();
	if(aiReserveHandler == nullptr)
	{
		return;
	}

	TArray<FGsAIReserveJob*> arrayReserveJob = aiReserveHandler->GetArrayReserveJob();
	int numReserveJob = arrayReserveJob.Num();


	_scrollBoxHelper->RefreshAll(numReserveJob);
}
void UGsUINPCList::InitializeMessage()
{
	FGsMessageHolder* msg = GMessage();
	if (nullptr == msg)
	{
		return;
	}

	MGameObject& msgObject = msg->GetGameObject();

	_arrGameObjDelegate.Emplace(msgObject.AddUObject(
		MessageGameObject::DEBUG_AI_RESERVE_LIST_UPDATE, this, &UGsUINPCList::OnDebugAIReserveListUpdate
	));
}
void UGsUINPCList::FinalizeMessage()
{
	FGsMessageHolder* msg = GMessage();
	if (nullptr == msg)
	{
		return;
	}

	if (0 != _arrGameObjDelegate.Num())
	{
		for (auto iter : _arrGameObjDelegate)
		{
			msg->GetGameObject().Remove(iter);
		}
		_arrGameObjDelegate.Empty();
	}
}

void UGsUINPCList::NativeConstruct()
{
	InitializeMessage();
	Super::NativeConstruct();
}

void UGsUINPCList::NativeDestruct()
{
	FinalizeMessage();
	Super::NativeDestruct();
}

// 갱신
void UGsUINPCList::OnDebugAIReserveListUpdate(const IGsMessageParam* In_param)
{
	SetDebugListData();
}