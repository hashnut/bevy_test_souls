#include "GsUIBillboardInteractionButton.h"

#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsPathManager.h"

#include "Option/GsGameUserSettings.h"

#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"

#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Define/GsIconDefine.h"
#include "UI/UIControlLib/Define/EGsIconType.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "UI/UIContent/Factory/GsIconFactory.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "UI/UILib/Define/GsUIDefine.h"

#include "UMG/Public/Components/WidgetSwitcher.h"
#include "UMG/Public/Components/SlateWrapperTypes.h"
#include "UMG/Public/Animation/WidgetAnimation.h"

#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/Image.h"

#include "Paper2D/Classes/PaperSprite.h"

#include "DataSchema/GsSchemaInteractionData.h"
#include "Runtime/DataCenter/Public/DataSchema/GsSchemaInteractionInvadeData.h"

#include "T1Project.h"



void UGsUIBillboardInteractionButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	
}

// 타겟 세팅
void UGsUIBillboardInteractionButton::SetData(UGsGameObjectBase* In_owner, NpcFunctionType In_functionType, int In_Index)
{
	SetTarget(In_owner);
	
	// 이미 만든게 있으면 해제
	ReleaseResource();
	if (In_functionType != NpcFunctionType::NONE)
	{
		// 아이콘 생성
		_loadedIcon = FGsIconFactory::CreateIcon(EGsIconType::ICON_INTERACTION, (int32)In_functionType);
		if (_loadedIcon.IsValid() &&
			_iconImageRoot != nullptr)
		{

			const FGsSchemaInteractionData* tableData = GetTableData(In_functionType);
			if (tableData)
			{
				if (tableData->_iconImagePath.Num() > In_Index)
				{
					_loadedIcon->SetIconImage(tableData->_iconImagePath[In_Index]);
					_loadedIcon->SetColorAndOpacity(tableData->_iconColor);
				}
			}

			UVerticalBoxSlot* panelSlot = _iconImageRoot->AddChildToVerticalBox(_loadedIcon.Get());
			if (panelSlot)
			{
				panelSlot->HorizontalAlignment = EHorizontalAlignment::HAlign_Center;
				panelSlot->VerticalAlignment = EVerticalAlignment::VAlign_Center;
			}
		}

	}
	
}

void UGsUIBillboardInteractionButton::SetQuestMarkData(UGsGameObjectBase* In_owner, bool In_isInvadeQuestNpc, NpcFunctionType In_functionType, int In_Index)
{
	SetTarget(In_owner);

	// 이미 만든게 있으면 해제
	ReleaseResource();
	if (In_functionType != NpcFunctionType::NONE)
	{
		_functionType = In_functionType;
		// 아이콘 생성
		_loadedIcon = FGsIconFactory::CreateIcon(EGsIconType::ICON_INTERACTION, (int32)In_functionType);
		if (_loadedIcon.IsValid() &&
			_iconImageRoot != nullptr)
		{
			if (In_isInvadeQuestNpc)
			{
				const FGsSchemaInteractionInvadeData* tableData = GetTableInvadeData(In_functionType);
				if (tableData)
				{
					if (tableData->_iconImagePath.Num() > In_Index)
					{
						_loadedIcon->SetIconImage(tableData->_iconImagePath[In_Index]);
						_loadedIcon->SetColorAndOpacity(tableData->_iconColor);
					}
				}
			}
			else
			{
				const FGsSchemaInteractionData* tableData = GetTableData(In_functionType);
				if (tableData)
				{
					if (tableData->_iconImagePath.Num() > In_Index)
					{
						_loadedIcon->SetIconImage(tableData->_iconImagePath[In_Index]);
						_loadedIcon->SetColorAndOpacity(tableData->_iconColor);
					}
				}
			}		

			UVerticalBoxSlot* panelSlot = _iconImageRoot->AddChildToVerticalBox(_loadedIcon.Get());
			if (panelSlot)
			{
				panelSlot->HorizontalAlignment = EHorizontalAlignment::HAlign_Center;
				panelSlot->VerticalAlignment = EVerticalAlignment::VAlign_Center;
			}
		}
	}
}

// 리소스 해제
void UGsUIBillboardInteractionButton::ReleaseResource()
{
	// 이미 만든게 있으면 해제
	if (_loadedIcon.IsValid())
	{
		_iconImageRoot->ClearChildren();

		FGsIconFactory::ReleaseIcon(_loadedIcon.Get());
		_loadedIcon = nullptr;
	}
}

const FGsSchemaInteractionData* UGsUIBillboardInteractionButton::GetTableData(NpcFunctionType InType) const
{
	const UGsTableInteractionData* tbl = Cast<UGsTableInteractionData>(FGsSchemaInteractionData::GetStaticTable());

	if (nullptr == tbl)
	{
		return nullptr;
	}

	const FGsSchemaInteractionData* findTbl = nullptr;
	if (false == tbl->FindRowById(InType, findTbl))
	{
		return nullptr;
	}

	return findTbl;
}

const FGsSchemaInteractionInvadeData* UGsUIBillboardInteractionButton::GetTableInvadeData(NpcFunctionType InType) const
{
	const UGsTableInteractionInvadeData* tbl = Cast<UGsTableInteractionInvadeData>(FGsSchemaInteractionInvadeData::GetStaticTable());

	if (nullptr == tbl)
	{
		return nullptr;
	}

	const FGsSchemaInteractionInvadeData* findTbl = nullptr;
	if (false == tbl->FindRowById(InType, findTbl))
	{
		return nullptr;
	}

	return findTbl;
}

// 인터랙션 버튼 출력 옵션 갱신
void UGsUIBillboardInteractionButton::UpdateShowInteractionButtonOption()
{
	/*if (_functionType == NpcFunctionType::QUEST_DIALOG)
		return;*/
	
	SetVisible(IsOptionEnabled());
}

bool UGsUIBillboardInteractionButton::IsOptionEnabled()
{
	// 예외 상황에 있을 땐 출력되도록 강제
	UGsGameUserSettings* gameUserSettings = GGameUserSettings();
	if (gameUserSettings == nullptr)
	{
		return true;
	}

	// 옵션 값 확인
	return (bool)gameUserSettings->GetCombatSetting(EGsOptionCombat::NPC_FUNCTION_ICON);
}

void UGsUIBillboardInteractionButton::NativeConstruct()
{
	Super::NativeConstruct();
	UpdateShowInteractionButtonOption();
}

void UGsUIBillboardInteractionButton::SetHide(EGsUIHideFlags InHideFlags)
{
	Super::SetHide(InHideFlags);

	if (_target.IsValid())
	{
		if (UGsGameObjectNonPlayer* nonPlayerTarget = Cast<UGsGameObjectNonPlayer>(_target))
		{
			nonPlayerTarget->UpdateWidgetShowInteractionButtonOption();
		}
	}
}

void UGsUIBillboardInteractionButton::OnChangeRenderState(bool bIsRendered)
{
	SetRenderOpacity(bIsRendered ? 1.f : 0.f);
}

void UGsUIBillboardInteractionButton::SetVisible(bool InVisibility)
{
	/*if (_functionType == NpcFunctionType::QUEST_DIALOG)
	{
		SetVisibility((InVisibility) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		return;
	}*/

	// check ui hide flag
	bool isUIHideFlag = false;
	if (UGsUIManager* uiMgr = GUI())
	{
		EGsUIHideFlags currentFlag = uiMgr->GetHideFlagsCurrent();
		isUIHideFlag = (EnumHasAnyFlags(currentFlag, GetHideFlagType())) ? true : false;
	}
	 
	// show if set value is true and hide flag is off, same time 
	ESlateVisibility visibility = 
		(InVisibility == true && isUIHideFlag == false) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;
	SetVisibility(visibility);
}

void UGsUIBillboardInteractionButton::OnClickButton()
{
	// 터치 이펙트 출력
	if (_clickVFXPath.IsValid())
	{
		UClass* vfxClass = GPath()->GetClassByObjectPath(_clickVFXPath);
		if (vfxClass)
		{
			UGsBlueprintFunctionLibraryUI::AddVFXToPointerPos(vfxClass, _clickVFXZOrder);
		}
	}

	StartButtonPressAnimation();
}
void UGsUIBillboardInteractionButton::NativeDestruct()
{
	// clear now
	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);
	Super::NativeDestruct();
}
void UGsUIBillboardInteractionButton::StartButtonPressAnimation()
{
	// clear now
	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);
	PlayAnimation(_aniButtonPress);
}