#include "GsUIPopupEventPrayer.h"

#include "Management/ScopeGame/GsEventProgressManager.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "Net/GsNetSendServiceWorld.h"

#include "DataCenter/Public/EventProgress/GsEventProgressDefine.h"

void UGsUIPopupEventPrayer::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupEventPrayer::OnClickCancel);
	_btnOK->OnClicked.AddDynamic(this, &UGsUIPopupEventPrayer::OnClickOK);
}

void UGsUIPopupEventPrayer::OnClickCancel()
{
	Close();
}

void UGsUIPopupEventPrayer::OnClickOK()
{
	FGsNetSendServiceWorld::SendEventPlayerEffect(_targetNpcTblId);
	Close();
}

void UGsUIPopupEventPrayer::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UGsUIPopupEventPrayer::NativeDestruct()
{
	if(FGsEventProgressManager* eventProgressManager = GSEventProgress())
	{ 
		eventProgressManager->OnReqFinishEvent(EGsEventProgressType::UI_POPUP_EVENT_PRAYER);
	}
	Super::NativeDestruct();
}

void UGsUIPopupEventPrayer::SetEventPrayerData(CreatureId In_id, FText In_buffName, FText In_buffDetail)
{
	SetTargetNpcTblId(In_id);

	_textBuffName = In_buffName;
	_textBuffDetail = In_buffDetail;
}

void UGsUIPopupEventPrayer::OnInputCancel()
{
	OnClickCancel();
}