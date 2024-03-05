// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildOperationSanctum.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Common/Sanctum/GsUISanctumBuffInfoGuild.h"
#include "UI/UIContent/Window/Guild/Operation/GsUIGuildSanctumNexusStateText.h"
#include "UI/UIContent/Popup/GsUIPopupSanctumNexusBuffInfo.h"
#include "UI/UIContent/Popup/GsUIPopupYesNo.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGame/GsSanctumManager.h"
#include "DataSchema/Sanctum/GsSchemaSanctumNexusData.h"
#include "Guild/GsGuildSanctumData.h"
#include "Net/GsNetSendServiceGuild.h"


void UGsUIGuildOperationSanctum::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnTooltipNexus->OnClicked.AddDynamic(this, &UGsUIGuildOperationSanctum::OnClickTooltipNexus);
	_btnRemoveNexus->OnClicked.AddDynamic(this, &UGsUIGuildOperationSanctum::OnClickRemoveNexus);
}

void UGsUIGuildOperationSanctum::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	IGsInvalidateUIInterface::CheckInvalidate();
}

void UGsUIGuildOperationSanctum::InvalidateAllInternal()
{
	FGsGuildManager* guildMgr = GGuild();
	if (nullptr == guildMgr)
	{
		return;
	}

	// null일 수 없는 값이라 널체크 안함
	const FGsGuildSanctumData* sanctumData = guildMgr->GetSanctumData();

	// 수호탑 없을 경우 빈 페이지 출력
	if (GuildCampStateType::NONE == sanctumData->_nexusState)
	{
		_switcherNexus->SetActiveWidgetIndex(0);
		return;
	}

	_switcherNexus->SetActiveWidgetIndex(1);

	// 버프 정보 설정
	_buffInfoUI->SetData(sanctumData->_sanctumId);

	// 성소 표시
	_imgNexus->SetVisibility(ESlateVisibility::Hidden);
	const FGsSchemaSanctumNexusData* nexusData = FGsSanctumManager::GetTableSanctumNexusData(sanctumData->_nexusId);
	if (nexusData)
	{
		// 이름 세팅
		FText textNexusLevel;

		// TEXT: 수호탑 Lv.{0}
		FText textFormat;
		if (FText::FindText(TEXT("SanctumUIText"), TEXT("Info_Occupy_NeedLevel"), textFormat))
		{
			textNexusLevel = FText::Format(textFormat, sanctumData->_buildingLevel);
		}

		_textBlockNexusName->SetText(textNexusLevel);

		// 이미지 세팅		
		TWeakObjectPtr<UGsUIGuildOperationSanctum> weakThis(this);
		FSoftObjectPath path = nexusData->imagePath;
		FGsUIHelper::AsyncLoadTextureFromPath(path, FStreamableDelegateParam::CreateWeakLambda(this, [weakThis](UObject* inRes)
			{
				if (UGsUIGuildOperationSanctum* StrongThis = weakThis.Get())
				{
					// 로드 끝난 후라 새로 로드하지 않고, 로드된 항목을 찾아옴
					UTexture2D* loadedTexture = Cast<UTexture2D>(inRes);
					if (loadedTexture)
					{
						StrongThis->SetSanctumNexusImage(loadedTexture);
					}
				}
			}));
	}
	else
	{
		_textBlockNexusName->SetText(FText::GetEmpty());
		_imgNexus->SetVisibility(ESlateVisibility::Hidden);
	}

	// 성소 상태 세팅
	FText textNexusState;
	switch (sanctumData->_nexusState)
	{
	case GuildCampStateType::READY:
	{
		// TEXT: 수호탑 건설 중
		FText::FindText(TEXT("SanctumUIText"), TEXT("Info_Under_Constrction_Ani"), textNexusState);
	}
	break;
	case GuildCampStateType::COMPLETE:
	case GuildCampStateType::OCCUPIED:
	{
		// TEXT: 수호탑 활성화 중
		FText::FindText(TEXT("GuildText"), TEXT("UI_Camp_Desc_Live"), textNexusState);
	}
	break;
	}

	_nexusStateUI->SetTextState(textNexusState);
}

void UGsUIGuildOperationSanctum::Show(bool bIsMyGuild, bool IsStackRecoverFlag)
{
	InvalidateAll();
}

void UGsUIGuildOperationSanctum::Hide()
{
	if (FGsGuildManager* guildMgr = GGuild())
	{
		guildMgr->GetRedDotCheckData().UpdateSanctumCheckTime();
		guildMgr->CheckRedDotByType(FGsGuildManager::EGsGuildRedDotType::Sanctum);
	}
}

void UGsUIGuildOperationSanctum::OnReceiveBuildingList()
{
	// 정보 갱신이 된 후에 오는 패킷이므로 화면 갱신
	InvalidateAll();
}

void UGsUIGuildOperationSanctum::OnNexusDestruction()
{
	// 수호탑 해제 팝업 열려있을 경우 강제 닫기
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(TEXT("PopupYesNo"));
	if (widget.IsValid())
	{
		widget->Close();
	}

	InvalidateAll();
}

void UGsUIGuildOperationSanctum::OnClickTooltipNexus()
{
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupSanctumNexusBuffInfo"));
	if (widget.IsValid())
	{
		if (UGsUIPopupSanctumNexusBuffInfo* popup = Cast<UGsUIPopupSanctumNexusBuffInfo>(widget.Get()))
		{
			uint8 campLevel = 0;
			if (const FGsGuildDataMine* myGuildData = GGuild()->GetMyGuildData())
			{
				campLevel = myGuildData->_campLevel;
			}

			popup->SetData(campLevel);
		}
	}
}

void UGsUIGuildOperationSanctum::OnClickRemoveNexus()
{
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupYesNo"));
	if (false == widget.IsValid())
	{
		return;
	}

	// 디자인 확인해볼 것
	UGsUIPopupYesNo* popup = Cast<UGsUIPopupYesNo>(widget.Get());
	if (nullptr == popup)
	{
		return;
	}

	// TEXT: 수호탑을 해체하시겠습니까? 주의! 해체 시 적용 중인 수호탑의 효과는 즉시 제거됩니다.
	FText textDesc;
	FText::FindText(TEXT("GuildText"), TEXT("UI_Camp_Desc_Destruction_Popup"), textDesc);
	popup->SetData(textDesc, [](bool bIsOk)
		{
			if (bIsOk)
			{
				FGsNetSendServiceGuild::SendReqSanctumNexusDestruction();
			}
		});
}

void UGsUIGuildOperationSanctum::SetSanctumNexusImage(class UTexture2D* InTexture)
{
	_imgNexus->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_imgNexus->SetBrushFromSoftTexture(InTexture);
}
