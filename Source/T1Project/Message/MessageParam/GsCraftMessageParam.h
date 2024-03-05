#pragma once

#include "GsMessageParam.h"
#include "Item/Craft/GsCraftTarget.h"
#include "Message/GsMessageContents.h"
#include "Net/MessageHandler/GsNetMessageHandlerCraft.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"
#include "Management/GsMessageHolder.h"

template<MessageContentCraft InMessageId>
struct FGsCraftMessageParamBase : public IGsMessageParam
{

	static constexpr MessageContentCraft MESSAGE_ID = InMessageId;

protected:
	FGsCraftMessageParamBase() = default;
	virtual ~FGsCraftMessageParamBase() = default;

public:
	void Send() const
	{
		FGsMessageHolder* MessageHolder = GMessage();
		if (nullptr == MessageHolder)
		{
			GSLOG(Error, TEXT("nullptr == MessageHolder"));
			return;
		}

		MessageHolder->GetCraft().SendMessage(MESSAGE_ID, this);
	}

};

/**
 * [패킷] 제작 제한 횟수가 변경되면 수신
 */
struct FGsCraftMessageParamNotifyCraftStatListWorldChanged final : public FGsCraftMessageParamBase<MessageContentCraft::NOTIFY_CRAFT_STAT_LIST_WORLD_CHANGED>
{
	virtual ~FGsCraftMessageParamNotifyCraftStatListWorldChanged() = default;
};

/**
 * [패킷] 특정 제작탭의 모든 서버/계정 생성 횟수 등을 수신받음
 */
struct FGsCraftMessageParamAckCraftStatList final : public FGsCraftMessageParamBase<MessageContentCraft::ACK_CRAFT_STAT_LIST>
{

	const CraftTabId _craftTabId;
	const TArray<CraftIdAmountPair> _craftStatListServer;
	const TArray<CraftIdAmountPair> _craftStatListServerAccount;

	FGsCraftMessageParamAckCraftStatList(const CraftTabId InCraftTabId, TArray<CraftIdAmountPair>&& InCraftStatListServer, TArray<CraftIdAmountPair>&& InCraftStatListServerAccount)
		: _craftTabId(InCraftTabId)
		, _craftStatListServer(MoveTemp(InCraftStatListServer))
		, _craftStatListServerAccount(MoveTemp(InCraftStatListServerAccount))
	{
	}
	virtual ~FGsCraftMessageParamAckCraftStatList() = default;
};

/**
 * [명령] Craft 상태가 아니면 상태를 변경하고, 제작창을 특정 파라미터와 함께 오픈함.
 *  - InCraftTargetId 에 원하는 제작ID 를 입력하면, 해당 제작대상으로 네비게이션.
 */
struct FGsCraftMessageParamOpenCraftWith final : public FGsCraftMessageParamBase<MessageContentCraft::OPEN_CRAFT_WITH>
{

	const CraftId _craftTargetId;

	explicit FGsCraftMessageParamOpenCraftWith(const CraftId InCraftTargetId)
		: _craftTargetId(InCraftTargetId)
	{
	}
	virtual ~FGsCraftMessageParamOpenCraftWith() = default;
};

/**
 * [명령] 제작대상을 즐겨찾기에 등록/해제 요청
 * InCraftTargetId 에 제작ID 를 입력하고, InOn 으로 등록할지 여부를 입력.
 */
struct FGsCraftMessageParamTryChangeFavoriteCraftTarget final : public FGsCraftMessageParamBase<MessageContentCraft::CHANGE_FAVORITE_TARGET>
{

	const CraftId _craftTargetId;

	const bool _on;

	FGsCraftMessageParamTryChangeFavoriteCraftTarget(const CraftId InCraftTargetId, const bool InOn)
		: _craftTargetId(InCraftTargetId)
		, _on(InOn)
	{
	}
	virtual ~FGsCraftMessageParamTryChangeFavoriteCraftTarget() = default;
};

/**
 * [이벤트] 즐겨찾기 목록이 새로 업데이트 되면 발생.
 * 주로 서버에서 즐겨찾기 목록을 받을 때 사용.
 */
struct FGsCraftMessageParamOnChangeFavoriteList final : public FGsCraftMessageParamBase<MessageContentCraft::CHANGE_FAVORITE_LIST>
{
	virtual ~FGsCraftMessageParamOnChangeFavoriteList() = default;
};

/**
 * [이벤트] 특정한 제작 대상의 데이터가 변경되면 발생.
 */
struct FGsCraftMessageParamOnChangeCraftTarget final : public FGsCraftMessageParamBase<MessageContentCraft::CHANGE_CRAFT_TARGET>
{

	const CraftId _craftTargetId;

	explicit FGsCraftMessageParamOnChangeCraftTarget(const CraftId InCraftTargetId)
		: _craftTargetId(InCraftTargetId)
	{
	}

	virtual ~FGsCraftMessageParamOnChangeCraftTarget() = default;

};

/**
 * [이벤트] 제작창의 탭이 열리면 발생.
 * 컨텐츠 특성 상, 특정 탭을 열 때 서버로 추가 정보를 요청하는 경우가 있어, 패킷 송신을 위해 주로 사용
 */
struct FGsCraftMessageParamOnOpenTab final : public FGsCraftMessageParamBase<MessageContentCraft::OPEN_TAB>
{

	const CraftTabId _craftTabId;

	explicit FGsCraftMessageParamOnOpenTab(const CraftTabId InCraftTabId)
		: _craftTabId(InCraftTabId)
	{
	}
	virtual ~FGsCraftMessageParamOnOpenTab() = default;
};

/**
 * [이벤트] 제작창의 "제작 전 연출"이 완료되면 발생.
 * 연출 도중에 각종 변경 사항(제작창 닫음 등)이 발생할 수 있으므로,
 * 연출 완료 후에 할 작업은 이 메세지를 보내서 이 후 작업의 가/부를 상황에 맞게 선택하기 위함.
 */
struct FGsCraftMessageParamOnEndSpelling final : public FGsCraftMessageParamBase<MessageContentCraft::END_SPELLING>
{

	const CraftId _craftTargetId;

	const int32 _craftCount;

	explicit FGsCraftMessageParamOnEndSpelling(const CraftId InCraftTargetId, const int32 InCraftCount)
		: _craftTargetId(InCraftTargetId)
		, _craftCount(InCraftCount)
	{
	}

	virtual ~FGsCraftMessageParamOnEndSpelling() = default;

};

/**
 * [이벤트] 제작 결과 내용을 수신받음.
 * InIsSuccess 는 제작 성공/대성공/실패와 무관하고, 제작 프로세스가 성공/실패했는지 여부임.
 */
struct FGsCraftMessageParamOnCraftResult final : public FGsCraftMessageParamBase<MessageContentCraft::RCV_RESULT>
{

	const bool _isSuccess;

	const CraftId _craftId;

	const TArray<ItemDBIdCraftResultPair>& _results;

	FGsCraftMessageParamOnCraftResult(const bool InIsSuccess, const CraftId InCraftId, const TArray<ItemDBIdCraftResultPair>& InResults)
		: _isSuccess(InIsSuccess)
		, _craftId(InCraftId)
		, _results(InResults)
	{
	}
	virtual ~FGsCraftMessageParamOnCraftResult() = default;
};