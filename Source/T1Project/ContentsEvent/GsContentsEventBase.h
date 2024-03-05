#pragma once

#include "GsContentsEventGroup.h"
#include "GsSchemaBase.h"
#include "GsTable.h"
#include "Management/ScopeGame/GsContentsEventManager.h"
#include "Shared/Client/SharedEnums/SharedEventEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"

class FGsContentsEventGroup;
struct FGsSchemaBase;

/**
 * 컨텐츠이벤트 베이스 클래스
 */
class FGsContentsEventBase
{

	friend class UGsContentsEventManager;

protected:
	/**
	 * 이 컨텐츠이벤트를 소유하고 있는 컨텐츠이벤트그룹의 ID
	 */
	const ContentsEventGroupId _contentsEventGroupId;

	/**
	 * 활성화 상태
	 */
	bool _isActivated;

protected:
	FGsContentsEventBase(const ContentsEventGroupId InContentsEventGroupId);

public:
	virtual ~FGsContentsEventBase() {}

public:
	/** 본 컨텐츠이벤트를 소유한 컨텐츠이벤트그룹의 포인터 */
	TSharedPtr<const FGsContentsEventGroup> GetContentsGroup() const;
	
	/** 이 컨텐츠이벤트의 타입 */
	virtual ContentsEventType GetContentsEventType() const;

	/** 이 컨텐츠이벤트의 ID */
	virtual ContentsEventId GetContentsEventId() const;

	/** 활성화 여부. 소유자인 컨텐츠이벤트그룹의 활성화 상태를 먼저 체크함. */
	bool IsActivated() const;

	/** 활성화/비활성화 상태와, 출력/비출력 상태는 공유할 수 없는 경우도 있으므로, 출력 여부를 별도 함수로 체크함. */
	bool CanDisplay() const;

private:
	/** 서버로부터 전달 받는 등, 실제 필요할 때 호출하므로 가능한 직접 호출하지 말것
	 *  활성화 상태를 변경함. 이전 활성화 상태와 동일하면 아무것도 하지 않고, 다를 경우에만 OnChangeActivate() 를 실행함.
	 */
	bool ChangeActivate(const bool InIsActivated);

protected:
	/** 이 컨텐츠이벤트를 소유한 컨텐츠이벤트그룹의 활성화 상태를 반환 */
	bool IsActivatedGroup() const;

	/** 이 컨텐츠이벤트를 소유한 컨텐츠이벤트그룹의 디스플레이 상태를 반환 */
	bool CanDisplayGroup() const;

	/** 본 인스턴스에서 체크 가능한 활성화/비활성화 상태를 반환. 오버라이드 필요. */
	virtual bool IsActivatedSelf() const
	{
		return _isActivated;
	}

	/** 본 인스턴스에서 체크 가능한 출력 여부를 반환. 오버라이드 필요. */
	virtual bool CanDisplaySelf() const
	{
		return false;
	}

	/** 활성화 상태가 변경되면 발생함. */
	virtual void OnChangeActivate(const bool InIsActivated)
	{
	}

};

/**
 * 컨텐츠이벤트의 공통 구현 가능 부분들을 구현한 템플릿
 */
template<ContentsEventType InContentsEventType, typename TSchemaType = FGsSchemaBase, typename TTableType = UGsTable>
class TGsContentsEventBase : public FGsContentsEventBase
{

public:
	static constexpr ContentsEventType CONTENTS_EVENT_TYPE = InContentsEventType;

private:
	using Super = FGsContentsEventBase;
	using Base = TGsContentsEventBase<CONTENTS_EVENT_TYPE, TSchemaType, TTableType>;

protected:
	/** 테이블데이터. 변경 불가. */
	const TSchemaType* _tableData;

public:
	TGsContentsEventBase(const ContentsEventGroupId InContentsEventGroupId, const ContentsEventId InContentsEventId)
		: Super(InContentsEventGroupId)
		, _tableData(Base::FindRowById(InContentsEventId))
	{
		// GSLOG(Log, TEXT("Creating ContentsEvent(%d, %d), type: %d"), InContentsEventGroupId, InContentsEventId, CONTENTS_EVENT_TYPE);
	}

	ContentsEventType GetContentsEventType() const final
	{
		return CONTENTS_EVENT_TYPE;
	}

	ContentsEventId GetContentsEventId() const final
	{
		return _tableData->id;
	}

private:
	/** 생성자에서 테이블 조회를 위해 사용함. */
	static const TSchemaType* FindRowById(const ContentsEventId InContentsEventId)
	{
		const UGsTable* TableBase = TSchemaType::GetStaticTable();
		if (nullptr == TableBase)
		{
			GSLOG(Error, TEXT("nullptr == TableBase, CONTENTS_EVENT_TYPE:%d"), CONTENTS_EVENT_TYPE);
			return nullptr;
		}

		const TTableType* Table = Cast<TTableType>(TableBase);
		if (nullptr == Table)
		{
			GSLOG(Error, TEXT("nullptr == Table, CONTENTS_EVENT_TYPE:%d"), CONTENTS_EVENT_TYPE);
			return nullptr;
		}

		const TSchemaType* Row = nullptr;
		if (!Table->FindRowById(InContentsEventId, Row))
		{
			GSLOG(Error, TEXT("!Table->FindRowById(%d, Row), CONTENTS_EVENT_TYPE:% d"), InContentsEventId, CONTENTS_EVENT_TYPE);
			return nullptr;
		}

		return Row;
	}

};
