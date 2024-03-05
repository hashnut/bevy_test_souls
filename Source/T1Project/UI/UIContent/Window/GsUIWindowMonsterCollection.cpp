#include "GsUIWindowMonsterCollection.h"

#include "MonsterKnowledge/GSUIMonsterCollectionBookPage.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeData.h"
#include "../MonsterKnowledgeCollection/EMonsterKnowledgeEnumData.h"

#include "../Management/GsMessageHolder.h"
#include "../Management/ScopeGame/GsMonsterCollectionManager.h"
#include "../Management/ScopeGame/GsCodexManager.h"
#include "../Management/ScopeGlobal/GsUIManager.h"

#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsMonsterKnowledgeCollection.h"

#include "GameFlow/GameContents/GsContentsMode.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIContent/Window/MonsterKnowledge/GsUIMonsterCollectionCodexPage.h"
#include "UI/UIContent/Common/RedDot/GsUIRedDotDefault.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Tray/GsUITrayTickerCollection.h"

#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"

#include "DataSchema/GsSchemaEnums.h"

#include "Net/GsNetSendServiceWorld.h"

#include "T1Project.h"
#include "Management/ScopeGame/GsStatChangeNotifyManager.h"





void UGsUIWindowMonsterCollection::BeginDestroy()
{
	if (_btnBack)
	{
		_btnBack->OnClicked.RemoveDynamic(this, &UGsUIWindowMonsterCollection::OnClickBack);
	}
	_btnBack = nullptr;

	if (_btnClose)
	{
		_btnClose->OnClicked.RemoveDynamic(this, &UGsUIWindowMonsterCollection::OnClickCloseAllStack);
	}
	_btnClose = nullptr;

	UGsUIWindow::BeginDestroy();
}

void UGsUIWindowMonsterCollection::NativeOnInitialized()
{
	UGsUIWindow::NativeOnInitialized();

	_helpContentsKey = EGsHelpContents::KnowledgeCollection;

	_toggleGroupMainTab.Clear();
	_toggleGroupMainTab.AddToggleWidget(_switcherBtnCodex);
	_toggleGroupMainTab.AddToggleWidget(_switcherBtnSlate);
	_toggleGroupMainTab.OnSelectChanged.BindUObject(this, &UGsUIWindowMonsterCollection::OnSelectedMainTab);

	// 상단 버튼
	_btnBack->OnClicked.AddDynamic(this, &UGsUIWindowMonsterCollection::OnClickBack);
	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowMonsterCollection::OnClickCloseAllStack);
}

void UGsUIWindowMonsterCollection::NativeConstruct()
{
	UGsUIWindow::NativeConstruct();

	// 2023/2/20 PKT - 되돌아 오는 경우 혹은 예약된 동작이 존재 하면 기존 캡쳐 데이터는 삭제
	if (false == IsStackRecoverFlag())
	{
		_captureData = FGsMonsterKnowledgeUICaptureData();
	}

	_activePage.Reset();

	int32 selectedTab = _captureData._mainTab;
	_toggleGroupMainTab.SetSelectedIndex(selectedTab, true);

	_captureData = FGsMonsterKnowledgeUICaptureData();

	// 지역 일람 진입 시, 지식 관련 데이터를 얻기 위해 패킷을 전송. 카테고리 ID 는 필요 없으니 첫 요소를 전송
	FGsNetSendServiceWorld::Send_Codex(GsCodex()->GetCategoryIdWithIndex(0));

	if (FGsStatChangeNotifyManager* statChangeNotifyMgr = GSStatChangeNotify())
	{
		statChangeNotifyMgr->Save(StatUpdateReason::CODEX_CATEGORY_LEVEL_UP);
	}
}

void UGsUIWindowMonsterCollection::NativeDestruct()
{	
	SetTutorial(false);

	if (_activePage.IsValid())
	{
		_activePage.Get()->Capture(&_captureData);
	}
	_captureData._mainTab = StaticCast<uint32>(_toggleGroupMainTab.GetSelectedIndex());
	
	// 대륙 탐사의 경우, 윈도우 닫으면 전투력 UI 나올 수 있도록 수정 (특수 노드는 즉시)
	if (FGsStatChangeNotifyManager* statChangeNotifyMgr = GSStatChangeNotify())
	{
		statChangeNotifyMgr->CheckNotify(StatUpdateReason::CODEX_CATEGORY_LEVEL_UP);
	}

	UGsUIWindow::NativeDestruct();
}

bool UGsUIWindowMonsterCollection::SyncContentState()
{
	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsMonsterCollection))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_MONSTER_COLLECTION);
		return true;
	}

	return false;
}

bool UGsUIWindowMonsterCollection::OnBack()
{
	if (_activePage.IsValid() && _activePage->CloseChildWidget())
	{
		return true;
	}

	return Super::OnBack();
}

void UGsUIWindowMonsterCollection::OnSelectedMainTab(int32 InIndex)
{
	if (_activePage.IsValid())
	{
		_activePage.Get()->Leave();
	}
	_switcherPage->SetActiveWidgetIndex(InIndex);
	
	_activePage = Cast<UGSUIMonsterCollectionBasePage>(_switcherPage->GetActiveWidget());
	if (_activePage.IsValid())
	{
		_activePage.Get()->Enter(&_captureData);
	}

	InvalidateRedDot();
}

void UGsUIWindowMonsterCollection::InvalidateRedDot()
{
	//bool isRedDot = GsMonsterCollection()->CheckRedDot(EMonsterCollectionMainTab::Codex);
	bool isRedDot = GsCodex()->CheckRedDot();
	_redDotCodex->SetRedDot(isRedDot);

	isRedDot = GsMonsterCollection()->CheckRedDot(EMonsterCollectionMainTab::Slate);
	_redDotSlate->SetRedDot(isRedDot);
}

void UGsUIWindowMonsterCollection::InvalidateBook(uint32 InData)
{
	InvalidateRedDot();

	if (EMonsterCollectionMainTab::Codex == StaticCast<EMonsterCollectionMainTab>(_switcherPage->GetActiveWidgetIndex()))
	{
		TWeakObjectPtr<UGSUIMonsterCollectionBasePage> activeWidget = Cast<UGSUIMonsterCollectionBasePage>(_switcherPage->GetActiveWidget());
		if (activeWidget.IsValid())
		{
			activeWidget.Get()->InvalidateBook(InData);
		}
	}
}

void UGsUIWindowMonsterCollection::InvalidateBook(const TArray<FGsMonsterCollectionBookResult>* InData)
{
	InvalidateRedDot();

	if (EMonsterCollectionMainTab::Codex == StaticCast<EMonsterCollectionMainTab>(_switcherPage->GetActiveWidgetIndex()))
	{
		TWeakObjectPtr<UGSUIMonsterCollectionBasePage> activeWidget = Cast<UGSUIMonsterCollectionBasePage>(_switcherPage->GetActiveWidget());
		if (activeWidget.IsValid())
		{
			activeWidget.Get()->InvalidateBookSet(InData);
		}
	}
}

void UGsUIWindowMonsterCollection::InvalidateCodex()
{
	InvalidateRedDot();
	
	if (nullptr == _switcherPage)
		return;

	if (EMonsterCollectionMainTab::Codex == StaticCast<EMonsterCollectionMainTab>(_switcherPage->GetActiveWidgetIndex()))
	{
		TWeakObjectPtr<UGSUIMonsterCollectionBasePage> activeWidget = Cast<UGSUIMonsterCollectionBasePage>(_switcherPage->GetActiveWidget());
		if (activeWidget.IsValid())
		{
			activeWidget.Get()->InvalidateCodex();
		}
	}
}

void UGsUIWindowMonsterCollection::OnChangeItem(const ItemId InItemId)
{
	if (nullptr == _switcherPage)
		return;

	if (EMonsterCollectionMainTab::Codex == StaticCast<EMonsterCollectionMainTab>(_switcherPage->GetActiveWidgetIndex()))
	{
		TWeakObjectPtr<UGSUIMonsterCollectionBasePage> activeWidget = Cast<UGSUIMonsterCollectionBasePage>(_switcherPage->GetActiveWidget());
		if (activeWidget.IsValid())
		{
			activeWidget.Get()->InvalidateCodex();
		}
	}
}

void UGsUIWindowMonsterCollection::InvalidateKnowledgeTab()
{
	InvalidateRedDot();

	if (nullptr == _switcherPage)
		return;

	if (EMonsterCollectionMainTab::Codex == StaticCast<EMonsterCollectionMainTab>(_switcherPage->GetActiveWidgetIndex()))
	{
		TWeakObjectPtr<UGSUIMonsterCollectionBasePage> activeWidget = Cast<UGSUIMonsterCollectionBasePage>(_switcherPage->GetActiveWidget());
		if (activeWidget.IsValid())
		{
			// 여기에 도달했다는 건 도감 표인트 변환이 성공했다는 뜻이므로, 티커를 띄워준다
			FText tickerMessage;
			FText::FindText(TEXT("CodexUIText"), TEXT("UI_MapList_Ticker_ConversionDone"), tickerMessage);
			FGsUIHelper::TrayTickerLauncher(tickerMessage);

			activeWidget.Get()->InvalidateKnowledgeGain();
		}
	}
}

void UGsUIWindowMonsterCollection::InvalidateCategoryLevelUp(CodexCategoryId InId /*= INVALID_CODEX_CATEGORY_ID*/, CodexCategoryLevel InLevel /*= INVALID_CODEX_CATEGORY_ID*/, int32 InMapGroupLevelUpCount /*= 0*/)
{
	InvalidateRedDot();

	if (InId != INVALID_CODEX_CATEGORY_ID)
	{
		UGsUIManager* uiManager = GUI();
		if (nullptr == uiManager)
		{
			GSLOG(Error, TEXT("nullptr == uiManager"));
			return;
		}

		UGsUITrayTickerCollection* ticker = Cast<UGsUITrayTickerCollection>(uiManager->OpenAndGetWidget(TEXT("TrayTickerCollection")).Get());
		if (nullptr == ticker)
		{
			GSLOG(Error, TEXT("nullptr == Ticker"));
			return;
		}

		FText titleText;
		FText::FindText(TEXT("CodexUIText"), TEXT("UI_MapList_Ticker_CategoryLevelUp_Title"), titleText);

		FText nameText;
		FText::FindText(TEXT("CodexUIText"), TEXT("UI_MapList_Ticker_CategoryLevelUp_CategoryNameAndLevel"), nameText);
		nameText = FText::Format(nameText, GsCodex()->GetCategoryNameWithId(InId), FText::FromString(FString::FromInt(InMapGroupLevelUpCount)));

		FText descText = GsCodex()->GetPassivityTextWithCategoryIdAndLevel(InId, InLevel);

		ticker->SetData(titleText, nameText, descText);
	}

	if (nullptr == _switcherPage)
		return;

	if (EMonsterCollectionMainTab::Codex == StaticCast<EMonsterCollectionMainTab>(_switcherPage->GetActiveWidgetIndex()))
	{
		TWeakObjectPtr<UGSUIMonsterCollectionBasePage> activeWidget = Cast<UGSUIMonsterCollectionBasePage>(_switcherPage->GetActiveWidget());
		if (activeWidget.IsValid())
		{
			activeWidget.Get()->InvalidateCategoryExplore();
		}
	}
}

void UGsUIWindowMonsterCollection::InvalidateMapGroupList()
{
	InvalidateRedDot();

	if (EMonsterCollectionMainTab::Codex == StaticCast<EMonsterCollectionMainTab>(_switcherPage->GetActiveWidgetIndex()))
	{
		TWeakObjectPtr<UGSUIMonsterCollectionBasePage> activeWidget = Cast<UGSUIMonsterCollectionBasePage>(_switcherPage->GetActiveWidget());
		if (activeWidget.IsValid())
		{
			activeWidget.Get()->InvalidateMapGroupList();
		}
	}
}

void UGsUIWindowMonsterCollection::InvalidateKnowledgeAmount()
{
	InvalidateRedDot();

	if (nullptr == _switcherPage)
		return;

	if (EMonsterCollectionMainTab::Codex == StaticCast<EMonsterCollectionMainTab>(_switcherPage->GetActiveWidgetIndex()))
	{
		TWeakObjectPtr<UGSUIMonsterCollectionBasePage> activeWidget = Cast<UGSUIMonsterCollectionBasePage>(_switcherPage->GetActiveWidget());
		if (activeWidget.IsValid())
		{
			activeWidget.Get()->InvalidateKnowledgeGain();
			activeWidget.Get()->InvalidateMapGroupText();
		}
	}
}

void UGsUIWindowMonsterCollection::InvalidateSlate(MonsterKnowledgeSlateId InId)
{
	InvalidateRedDot();

	if (EMonsterCollectionMainTab::Slate == StaticCast<EMonsterCollectionMainTab>(_switcherPage->GetActiveWidgetIndex()))
	{
		TWeakObjectPtr<UGSUIMonsterCollectionBasePage> activeWidget = Cast<UGSUIMonsterCollectionBasePage>(_switcherPage->GetActiveWidget());
		if (activeWidget.IsValid())
		{
			activeWidget.Get()->InvalidateSlate(InId);
		}
	}
}

void UGsUIWindowMonsterCollection::GoSelectedSlate(MonsterKnowledgeSlateId InId)
{
	InvalidateRedDot();

	_captureData._reservedId = InId;

	_toggleGroupMainTab.SetSelectedIndex(StaticCast<int32>(EMonsterCollectionMainTab::Slate), true);

	_captureData = FGsMonsterKnowledgeUICaptureData();
}

void UGsUIWindowMonsterCollection::InvalidateSlateCurrency()
{
	InvalidateRedDot();

	if (EMonsterCollectionMainTab::Slate == StaticCast<EMonsterCollectionMainTab>(_switcherPage->GetActiveWidgetIndex()))
	{
		TWeakObjectPtr<UGSUIMonsterCollectionBasePage> activeWidget = Cast<UGSUIMonsterCollectionBasePage>(_switcherPage->GetActiveWidget());
		if (activeWidget.IsValid())
		{
			activeWidget.Get()->InvalidateSlateCurrency();
		}
	}
}

UWidget* UGsUIWindowMonsterCollection::SetTutorial(bool bIsTutorial)
{
	if (bIsTutorial)
	{
		if (static_cast<int32>(EMonsterCollectionMainTab::Codex) == _switcherPage->GetActiveWidgetIndex())
		{
			if (UWidget* codexWidget = GetCodexPage())
			{
				if (UGsUIMonsterCollectionCodexPage* codexPage = Cast<UGsUIMonsterCollectionCodexPage>(codexWidget))
				{
					return codexPage->SetTutorial(true);
				}
			}
		}
	}

	return nullptr;
}

UWidget* UGsUIWindowMonsterCollection::GetCodexPage()
{
	return _switcherPage->GetWidgetAtIndex(static_cast<int32>(EMonsterCollectionMainTab::Codex));
}
