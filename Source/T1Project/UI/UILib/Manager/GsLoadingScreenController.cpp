// Fill out your copyright notice in the Description page of Project Settings.


#include "GsLoadingScreenController.h"
#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Loading/GsSchemaLoadingScreen.h"
#include "DataSchema/Loading/GsSchemaLoadingScreenGroup.h"
#include "DataSchema/Loading/GsSchemaLoadingScreenPlayTip.h"
#include "Management/ScopeGlobal/GsPathManager.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "UI/UILib/Define/GsUIDefine.h"
#include "UI/UIContent/Loading/GsUILoadingScreen.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UTIL/GsTableUtil.h"


void UGsLoadingScreenController::Initialize()
{
	_loadingScreenForModule = nullptr;
	_loadingScreenUI = nullptr;
}

void UGsLoadingScreenController::Finalize()
{
	_loadingScreenForModule = nullptr;
	_loadingScreenUI = nullptr;
	_bIsAfterDownload = false;
}

void UGsLoadingScreenController::InitializeAfterResourceDownload()
{
	_bIsAfterDownload = true;
}

void UGsLoadingScreenController::OnPrevLoadMap()
{
	if (false == _bIsAfterDownload)
	{
		return;
	}

	// UI를 꺼서 모듈쪽에서 그리는 화면이 보이게 함. 이건 그냥 에디터 실행에서는 안된다.
	if (_loadingScreenUI)
	{
		_loadingScreenUI->OnStartLoadingModule();
	}
}

void UGsLoadingScreenController::OnPostLoadMap()
{
	if (false == _bIsAfterDownload)
	{
		return;
	}

	// UI켜기
	if (_loadingScreenUI)
	{
		_loadingScreenUI->OnEndLoadingModule();
	}
}

void UGsLoadingScreenController::PreCreateLoadingWidget()
{
	if (false == _bIsAfterDownload)
	{
		return;
	}

	if (nullptr != _loadingScreenForModule &&
		nullptr != _loadingScreenUI)
	{
		return;
	}

	TArray<const FGsSchemaLoadingScreenGroup*> groupList;
	if (const UGsTable* table = FGsSchemaLoadingScreenGroup::GetStaticTable())
	{
		table->GetAllRows(groupList);
	}

	for (const FGsSchemaLoadingScreenGroup* iter : groupList)
	{
		if (EGsLoadingScreenGroupType::DEFAULT == iter->groupType)
		{
			_defaultGroupId = iter->id;
		}
		else if (EGsLoadingScreenGroupType::INVADE == iter->groupType)
		{
			_invadeGroupId = iter->id;
		}
	}

	if (UGsPathManager* pathMgr = GPath())
	{
		if (UClass* widgetClass = pathMgr->GetClass(EGsPathType::UI, TEXT("LoadingScreen")))
		{
			if (widgetClass)
			{
				// 모듈 출력을 위한 위젯
				_loadingScreenForModule = Cast<UGsUILoadingScreen>(CreateWidget(GetWorld(), widgetClass));
				_loadingScreenUI = Cast<UGsUILoadingScreen>(CreateWidget(GetWorld(), widgetClass));				
			}
		}
	}
}

void UGsLoadingScreenController::ShowLoading(int32 InMapId, bool bIsInvadeWorld, int32 InPlayerLevel)
{
	FGsUIHelper::ForcedHideBlockUI(EGsHideBlockUIFactor::SHOW_LOADING);

	if (false == _bIsAfterDownload)
	{
		return;
	}

	PreCreateLoadingWidget();

	if (nullptr == _loadingScreenForModule ||
		nullptr == _loadingScreenUI)
	{
		return;
	}

	// 로딩화면, 문구 세팅
	const FGsSchemaLoadingScreenGroup* loadingGroup = GetLoadingGroup(InMapId, bIsInvadeWorld);
	if (nullptr == loadingGroup)
	{
		return;
	}

	int32 idIndex = FMath::RandRange(0, loadingGroup->loadingScreenList.Num() - 1);
	if (loadingGroup->loadingScreenList.IsValidIndex(idIndex))
	{
		if (const FGsSchemaLoadingScreen* table = loadingGroup->loadingScreenList[idIndex].GetRow())
		{
			_loadingScreenForModule->SetData(table);
			_loadingScreenUI->SetData(table);
		}
	}

	// 플레이 팁 세팅
	TArray<const FGsSchemaLoadingScreenPlayTip*> tempTipList;
	GetTipIdList(InPlayerLevel, tempTipList);
	int32 tipIndex = FMath::RandRange(0, tempTipList.Num() - 1);

	if (tempTipList.IsValidIndex(tipIndex))
	{
		const FGsSchemaLoadingScreenPlayTip* table = tempTipList[tipIndex];
		_loadingScreenForModule->SetTextTooltip(table->tipText, table->tipColor);
		_loadingScreenUI->SetTextTooltip(table->tipText, table->tipColor);
	}	

	_loadingScreenUI->SetIsForUI(true);
	_loadingScreenUI->AddToViewport(UI_DEPTH_LOADING);
}

void UGsLoadingScreenController::HideLoading()
{
	if (false == _bIsAfterDownload)
	{
		return;
	}

	if (_loadingScreenUI)
	{
		_loadingScreenUI->Remove();
	}
}

bool UGsLoadingScreenController::IsVisible() const
{
	if (_loadingScreenUI)
	{
		return _loadingScreenUI->GetIsVisible();
	}

	return false;
}

const FGsSchemaLoadingScreenGroup* UGsLoadingScreenController::GetLoadingGroup(int32 InMapId, bool bIsInvadeWorld)
{
	if (false == _bIsAfterDownload)
	{
		return nullptr;
	}

	const FGsSchemaLoadingScreenGroup* outGroup = nullptr;
	if (bIsInvadeWorld)
	{
		outGroup = UGsTableUtil::FindRowById<UGsTableLoadingScreenGroup, FGsSchemaLoadingScreenGroup>(_invadeGroupId);
	}
	else
	{
		if (const FGsSchemaMapData* mapData = UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(InMapId))
		{
			outGroup = mapData->loadingScreenGroup.GetRow();
		}
	}

	if (nullptr == outGroup)
	{
		outGroup = UGsTableUtil::FindRowById<UGsTableLoadingScreenGroup, FGsSchemaLoadingScreenGroup>(_defaultGroupId);
	}
	
	return outGroup;
}

void UGsLoadingScreenController::GetTipIdList(int32 InPlayerLevel, 
	OUT TArray<const FGsSchemaLoadingScreenPlayTip*>& OutList)
{
	TArray<const FGsSchemaLoadingScreenPlayTip*> tipList;
	if (const UGsTable* table = FGsSchemaLoadingScreenPlayTip::GetStaticTable())
	{
		table->GetAllRows(tipList);
	}

	for (const FGsSchemaLoadingScreenPlayTip* iter : tipList)
	{
		if (iter->minLevel <= InPlayerLevel &&
			iter->maxLevel >= InPlayerLevel)
		{
			OutList.Emplace(iter);
		}
	}
}
