// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameStatePKBook.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsPKBookManager.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Message/GsMessageUI.h"
#include "Message/MessageParam/GsMessageParam.h"

#include "../UTIL/GsTimeSyncUtil.h"





FGsGameStatePKBook::FGsGameStatePKBook() : FGsContentsGameBase(FGsContentsMode::InGame::ContentsPKBook)
{
}

FGsGameStatePKBook::~FGsGameStatePKBook()
{
}

void FGsGameStatePKBook::RegisterMessages()
{
	MPKBook& MessageUiHolder = GMessage()->GetPKBook();

	_listPKBookDelegate.Emplace(
		MessageUiHolder.AddRaw(MessageContentPKBook::UI_INVALID_RECORD
			, this, &FGsGameStatePKBook::ON_MSG_UI_INVALID_RECORD)
	);

	_listPKBookDelegate.Emplace(
		MessageUiHolder.AddRaw(MessageContentPKBook::UI_OPEN_DETAIL_POPUP
			, this, &FGsGameStatePKBook::ON_MSG_UI_OPEN_DETAIL_POPUP)
	);

	_listPKBookDelegate.Emplace(
		MessageUiHolder.AddRaw(MessageContentPKBook::UI_INVALID_RECORD_DETAILDATA
			, this, &FGsGameStatePKBook::ON_MSG_UI_INVALID_RECORD_DETAILDATA)
	);

	_listPKBookDelegate.Emplace(
		MessageUiHolder.AddRaw(MessageContentPKBook::UI_INVALID_DETECTIONTARGET
			, this, &FGsGameStatePKBook::ON_MSG_UI_INVALID_DETECTIONTARGET)
	);

	_listPKBookDelegate.Emplace(
		MessageUiHolder.AddRaw(MessageContentPKBook::UI_INVALID_DETECTIONTARGET_LIST
			, this, &FGsGameStatePKBook::ON_MSG_UI_INVALID_DETECTIONTARGET_LIST)
	);
}

void FGsGameStatePKBook::UnregisterMessages()
{
	MPKBook& MessageUiHolder = GMessage()->GetPKBook();
	for (MsgPKBookHandle& msgHandler : _listPKBookDelegate)
	{
		MessageUiHolder.Remove(msgHandler);
	}
	_listPKBookDelegate.Empty();
}

void FGsGameStatePKBook::ON_MSG_UI_INVALID_RECORD(const IGsMessageParam* InParam)
{
	if (_windowUI.IsValid())
	{
		_windowUI->InvalidRecoredDataSet();
	}
}

void FGsGameStatePKBook::ON_MSG_UI_OPEN_DETAIL_POPUP(const IGsMessageParam* InParam)
{
	const FGsPrimitiveUInt64* Param = StaticCast<const FGsPrimitiveUInt64*>(InParam);
	if (nullptr == Param)
	{
		GSLOG(Error, TEXT("nullptr == Param"));
		return;
	}

	if (_windowUI.IsValid())
	{
		_windowUI->OpenRecordDetail(StaticCast<PKRecordId>(Param->_data));
	}
}

void FGsGameStatePKBook::ON_MSG_UI_INVALID_RECORD_DETAILDATA(const IGsMessageParam* InParam)
{
	const FGsPrimitiveUInt64* Param = StaticCast<const FGsPrimitiveUInt64*>(InParam);
	if (nullptr == Param)
	{
		GSLOG(Error, TEXT("nullptr == Param"));
		return;
	}

	if (_windowUI.IsValid())
	{
		_windowUI->InvalidRecordDetail(StaticCast<PKRecordId>(Param->_data));
	}
}

void FGsGameStatePKBook::ON_MSG_UI_INVALID_DETECTIONTARGET(const IGsMessageParam* InParam)
{
	const FGsPrimitiveUInt64* Param = StaticCast<const FGsPrimitiveUInt64*>(InParam);
	if (nullptr == Param)
	{
		GSLOG(Error, TEXT("nullptr == Param"));
		return;
	}

	if (_windowUI.IsValid())
	{
		_windowUI->InvalidDetectionTarget(StaticCast<UserDBId>(Param->_data));
	}
}

void FGsGameStatePKBook::ON_MSG_UI_INVALID_DETECTIONTARGET_LIST(const IGsMessageParam* InParam)
{
	if (_windowUI.IsValid())
	{
		_windowUI->InvalidDetectionTargetSet();
	}
}

void FGsGameStatePKBook::Enter()
{
	FGsContentsGameBase::Enter();

	if (UGsUIManager* uiManager = GUI())
	{
		auto widget = uiManager->OpenAndGetWidget(TEXT("WindowPKBook"));
		if (widget.IsValid())
		{
			_windowUI = Cast<UGsUIWindowPKBook>(widget);
		}
	}

	// 2022/09/21 PKT - Message 등록
	RegisterMessages();

	// 네비게이션 바 재화 세팅 (드롭다운 메뉴 감춤, 선택 슬롯 감춤)
	SetNavigationBar(CurrencyType::NONE);
}

void FGsGameStatePKBook::Exit()
{
	// 주의: Exit 시 Window 닫기를 하지 않음(UI Stack과 State연계로직 때문. OnBack, CloseAll등의 로직에 의해 닫음)
	_windowUI = nullptr;

	// 2022/09/21 PKT - Message 해지
	UnregisterMessages();

	// 2022/09/21 PKT - Check Out
	if (FGsPKBookManager* PKBookManager = GsPKBook())
	{
		PKBookManager->Send_CheckOut(StaticCast<int64>(FGsTimeSyncUtil::GetServerNowSecond()));
	}

	// HUD 관련 레드닷 갱신 요청	
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::PK_BOOK, true);

	FGsContentsGameBase::Exit();
}
