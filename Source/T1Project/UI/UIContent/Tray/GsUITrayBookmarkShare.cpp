#include "GsUITrayBookmarkShare.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/GsUIPopupWarp.h"

#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Management/ScopeGame/GsBookmarkManager.h"
#include "Management/ScopeGame/GsWorldMapManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "Item/GsItemManager.h"
#include "Item/GsItem.h"

#include "Map/GsMapFunc.h"
#include "Map/Bookmark/GsBookmarkFunc.h"
#include "Map/WorldMap/GsWorldMapDefine.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "Net/GsNetSendServiceWorld.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "EventProgress/GsEventProgressDefine.h"

#include "UMG/Public/Components/ProgressBar.h"

#include "Core/Public/Delegates/Delegate.h"
#include "Currency/GsCostPackage.h"


// 거절 버튼
void UGsUITrayBookmarkShare::OnClickReject()
{
	// 그냥 닫기
	Close();
}
// 등록 버튼
void UGsUITrayBookmarkShare::OnClickEnroll()
{
	TArray<FGsBookmarkData*> arrInfo = GSBookmark()->GetBookmarkData();
	int maxCount =
		GData()->GetGlobalData()->_bookmarkListItemMaxCount;

	// 갯수 초과시 등록 안함
	if (arrInfo.Num() >= maxCount)
	{
		FText findText;
		FText::FindText(TEXT("UserTeleportText"), TEXT("ErrorCreateMaxValue"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return;
	}

	// 이동 서버 전송
	FGsBookmarkShareInfo shareInfo = GSBookmark()->GetShareInfo();

	// 등록 서버 전송
	FGsNetSendServiceWorld::SendSharedBookmarkRegister(
		shareInfo.GetBookmarkName(),
		shareInfo.GetMapId(),
		shareInfo.GetPos());

	Close();
}
// 이동 버튼
void UGsUITrayBookmarkShare::OnClickMove()
{
	// 개인 이벤트 채널에서의 이동을 막음
	// 싱글 필드도 막음
	if (GLevel()->IsMapType(MapType::INSTANCE_SINGLE) == true ||
		GLevel()->IsMapType(MapType::SINGLE_FIELD) == true)
	{
		FText findText;
		FText::FindText(TEXT("UserTeleportText"), TEXT("ErrorUseInvalidArea"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return;
	}

	// local이  상태이상이면 못보냄
	if (true == FGsMapFunc::IsLocalWarpImpossible())
	{
		return;
	}

	// 1. 아이템 체크
	// 2. 티커 출력
	// 3. 아이템 db id 구함
	uint64 itemDBId = 0;
	ItemId CurrentItemId = INVALID_ITEM_ID;
	if (FGsBookmarkFunc::IsBookmarkScrollExist(itemDBId, CurrentItemId) == false)
	{
		return;
	}

	// 이동 서버 전송
	FGsBookmarkShareInfo shareInfo = GSBookmark()->GetShareInfo();

	FString bookmarkName = shareInfo.GetBookmarkName();

	GSWorldMap()->ReserveWarpBookmarkShare(
		bookmarkName,
		FGsCostPackage::CreateSimple(CurrentItemId, 1),
		shareInfo.GetMapId(), shareInfo.GetPos(),
		itemDBId, shareInfo.GetChannelId());


	Close();
}

void UGsUITrayBookmarkShare::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnReject->OnClicked.AddDynamic(this, &UGsUITrayBookmarkShare::OnClickReject);
	_btnEnroll->OnClicked.AddDynamic(this, &UGsUITrayBookmarkShare::OnClickEnroll);
	_btnMove->OnClicked.AddDynamic(this, &UGsUITrayBookmarkShare::OnClickMove);
}

void UGsUITrayBookmarkShare::NativeConstruct()
{
	Super::NativeConstruct();

	// 2023/8/2 PKT - Local Player가 타서버에 있을때 이동 버튼을 막는다.( https://jira.com2us.com/jira/browse/C2URWQ-6211 )
	bool isEnable = GGameData()->GetLoadWorldId() == GGameData()->GetUserData()->mHomeWorldId;
	_btnMove->SetIsEnabled(isEnable);
	
	SetData();
}

// 데이터 세팅
void UGsUITrayBookmarkShare::SetData()
{
	FGsBookmarkShareInfo shareInfo = GSBookmark()->GetShareInfo();
	// 시작 시간 가져오기
	_startTime = shareInfo.GetStartTimeTick();

	// 이름
	FString userName = shareInfo.GetUserName();
	// 즐겨찾기 이름
	FString bookmarkName = shareInfo.GetBookmarkName();


	_textSenderInfo = FText::FromString(userName);
	_textBookmarkName = FText::FromString(bookmarkName);

	_isTimeover = false;
}

void UGsUITrayBookmarkShare::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_isTimeover == true)
	{
		return;
	}

	float rate = GetRemainTimeRate();

	_progressBarWaitClose->SetPercent(rate);

	if (rate >= 1.0f)
	{
		_isTimeover = true;

		// 취소랑 같다
		Close();
	}
}
// 남은시간 rate 값 구하기
float UGsUITrayBookmarkShare::GetRemainTimeRate()
{
	int64 nowTick = FDateTime::UtcNow().GetTicks();
	int64 diffTick = nowTick - _startTime;

	static float maxTimeTick = 
		GData()->GetGlobalData()->_bookmarkSharePopupWaitTime * ETimespan::TicksPerSecond;

	float rate = diffTick / maxTimeTick;
	return rate;
}