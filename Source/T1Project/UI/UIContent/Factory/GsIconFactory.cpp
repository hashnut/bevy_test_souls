// Fill out your copyright notice in the Description page of Project Settings.


#include "GsIconFactory.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "UI/UIControlLib/Define/EGsIconType.h"
#include "UI/UILib/Container/GsWidgetPool.h"
#include "UI/UILib/Manager/GsWidgetPoolManager.h"
#include "Management/ScopeGlobal/GsPathManager.h"
#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"
#include "DataSchema/Icon/GsSchemaCommonIconData.h"


UGsUIIconBase* FGsIconFactory::CreateIcon(EGsIconType InIconType, int32 InIconKey, UWidget* InOwner, bool bInUsePool)
{
	return CreateCommonIcon(InIconType, InOwner, bInUsePool);
}

UGsUIIconBase* FGsIconFactory::CreateIcon(FSoftObjectPath In_path)
{
	UClass* widgetClass = nullptr;
	if (UGsPathManager* pathMgr = GPath())
	{
		widgetClass = pathMgr->GetClassByObjectPath(In_path);
	}

	if (nullptr == widgetClass)
	{
		return nullptr;
	}

	UGsUIIconBase* icon = Claim(widgetClass);
	if (icon)
	{
		icon->AddToViewport();
	}

	return icon;
}

UGsUIIconBase* FGsIconFactory::Claim(UClass* In_class)
{
	UGsWidgetPoolManager* poolMgr = UGsBlueprintFunctionLibraryUI::GetWidgetPoolManager();
	if (nullptr == poolMgr)
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

	UGsUIIconBase* iconWidget = pool->Claim<UGsUIIconBase>();
	if (iconWidget)
	{
		iconWidget->SetIsManagedByPool(true);
	}

	return iconWidget;
}

void FGsIconFactory::WarmUpIcon(EGsIconType InIconType , int32 InWarmUpCount)
{
	UGsWidgetPoolManager* poolMgr = UGsBlueprintFunctionLibraryUI::GetWidgetPoolManager();
	if (nullptr == poolMgr)
	{
		return;
	}

	const UGsTableCommonIconData* table = Cast<UGsTableCommonIconData>(FGsSchemaCommonIconData::GetStaticTable());
	if (nullptr == table)
	{
		return;
	}

	const FGsSchemaCommonIconData* row = nullptr;
	if (false == table->FindRowById(InIconType, row))
	{
		return;
	}

	UClass* iconClass = nullptr;
	iconClass = GPath()->GetClassByObjectPath(row->widgetPath);
	UGsWidgetPool* pool = poolMgr->FindPool(iconClass);
	if (nullptr == pool)
	{
		if (nullptr == pool)
		{
			return;
		}
	}

	// 현재 pool에 생성된 itemicon Widget count 확인하여 파라미터에 InWarmUpCount 보다 작으면 
	// 모자란 갯수만큼 widget 을 생성하여 pooling 처리 함
	int32 count = pool->GetTotalRunningCount();
	if (InWarmUpCount > count)
	{
		pool->WarmUp(InWarmUpCount);
	}
}

void FGsIconFactory::ReleaseIcon(UGsUIIconBase* In_icon)
{
	if (nullptr == In_icon)
	{
		return;
	}
	In_icon->RemoveFromParent();
	In_icon->ReleaseToPool();
}

UGsUIIconBase* FGsIconFactory::CreateCommonIcon(EGsIconType InIconType, class UWidget* InOwner, bool bInUsePool)
{
	const UGsTableCommonIconData* table = Cast<UGsTableCommonIconData>(FGsSchemaCommonIconData::GetStaticTable());
	if (nullptr == table)
	{
		return nullptr;
	}
	
	const FGsSchemaCommonIconData* row = nullptr;
	if (false == table->FindRowById(InIconType, row))
	{
		return nullptr;
	}

	UClass* iconClass = nullptr;
#if WITH_EDITORONLY_DATA
	if (InOwner)
	{
		if (InOwner->IsDesignTime())
		{
			iconClass = GetClassByObjectPath(row->widgetPath);

			// 에디터 미리보기 시에는 풀을 사용하지 않음
			bInUsePool = false;
		}
		else
		{
			iconClass = GPath()->GetClassByObjectPath(row->widgetPath);
		}
	}
	else
	{
		iconClass = GPath()->GetClassByObjectPath(row->widgetPath);
	}	
#else
	iconClass = GPath()->GetClassByObjectPath(row->widgetPath);
#endif

	if (nullptr == iconClass)
	{
		return nullptr;
	}

	if (bInUsePool)
	{
		return Claim(iconClass);
	}
	else
	{
		return CreateWidget<UGsUIIconBase>(InOwner, iconClass);
	}

	return nullptr;
}

#if WITH_EDITORONLY_DATA
UClass* FGsIconFactory::GetClassByObjectPath(const FSoftObjectPath& InPath)
{
	if (false == InPath.IsValid())
	{
		return nullptr;
	}

	FSoftObjectPath classObjPath = FSoftObjectPath(InPath.ToString() + TEXT("_C"));

	TSoftClassPtr<UObject> classPtr = TSoftClassPtr<UObject>(classObjPath);

	return classPtr.LoadSynchronous();
}
#endif
