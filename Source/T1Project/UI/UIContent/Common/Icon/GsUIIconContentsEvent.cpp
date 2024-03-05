#include "GsUIIconContentsEvent.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UILib/Base/GsUITooltip.h"

void UGsUIIconContentsEvent::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 버튼 이벤트 연결
	_button->OnClicked.AddDynamic(this, &UGsUIIconContentsEvent::OnClickedButton);
}

void UGsUIIconContentsEvent::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGsUIIconContentsEvent::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGsUIIconContentsEvent::SetParameters(const UGsUIIconContentsEvent::Parameters& InParameters)
{
	// 아이콘이 꺼진채로 Pool에 반납되어도 문제 없도록 처리
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	// 이미지 세팅	
	SetIconImage(InParameters._iconPath);
}

void UGsUIIconContentsEvent::OnClickedButton()
{
	OnClickIcon.ExecuteIfBound();
}
