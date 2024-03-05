#include "GsContentsEventBase.h"

#include "GsContentsEventGroup.h"
#include "Management/GsScopeGame.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGame/GsContentsEventManager.h"
#include "Message/MessageParam/GsContentsEventMessageParam.h"
#include "Shared/Client/SharedEnums/SharedEventEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

FGsContentsEventBase::FGsContentsEventBase(const ContentsEventGroupId InContentsEventGroupId)
	: _contentsEventGroupId(InContentsEventGroupId)
{
}

TSharedPtr<const FGsContentsEventGroup> FGsContentsEventBase::GetContentsGroup() const
{
	const UGsContentsEventManager* ContentsEventManager = GSContentsEvent();
	if (nullptr == ContentsEventManager)
	{
		return nullptr;
	}

	const TWeakPtr<const FGsContentsEventGroup> ContentsEventGroup = ContentsEventManager->GetContentsEventGroup(_contentsEventGroupId);
	if (!ContentsEventGroup.IsValid())
	{
		return nullptr;
	}

	return ContentsEventGroup.Pin();
}

ContentsEventType FGsContentsEventBase::GetContentsEventType() const
{
	return ContentsEventType::UNUSED_COMMON;
}

ContentsEventId FGsContentsEventBase::GetContentsEventId() const
{
	return INVALID_CONTENTS_EVENT_ID;
}

bool FGsContentsEventBase::IsActivated() const
{
	return IsActivatedGroup() && IsActivatedSelf();
}

bool FGsContentsEventBase::CanDisplay() const
{
	return CanDisplayGroup() && CanDisplaySelf();
}

bool FGsContentsEventBase::ChangeActivate(const bool InIsActivated)
{
	if (_isActivated == InIsActivated)
	{
		return false;
	}

	_isActivated = InIsActivated;

	OnChangeActivate(_isActivated);

	FGsContentsEventMessageParamContentsEventStateChanged(GetContentsEventType(), GetContentsEventId(), _isActivated).Send();

	return true;
}

bool FGsContentsEventBase::IsActivatedGroup() const
{
	const TSharedPtr<const FGsContentsEventGroup> ContentsEventGroup = GetContentsGroup();
	return ContentsEventGroup.IsValid() && ContentsEventGroup->IsActivated();
}

bool FGsContentsEventBase::CanDisplayGroup() const
{
	const TSharedPtr<const FGsContentsEventGroup> ContentsEventGroup = GetContentsGroup();
	return ContentsEventGroup.IsValid() && ContentsEventGroup->CanDisplay();
}
