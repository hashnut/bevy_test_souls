#include "GsScanMemberInfo.h"

#include "ActorEx/GsCharacterBase.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectCreature.h"
#include "GameObject/Data/GsGameObjectData.h"
#include "GameObject/Stat/GsStatInfo.h"
#include "GameObject/Target/GsTargetHandlerBase.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"

#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"

#include "Management/ScopeGame/GsGameObjectManager.h"

#include "UI/UIContent/Billboard/GsUIBillboardCreature.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Shared/SharedInclude/SharedFuncs.h"

#include "UTIL/GsText.h"

// 이름 접근
FString FGsScanMemberInfo::GetName()
{
	return _name;
}
// game id(서버 사용 고유키)
int64 FGsScanMemberInfo::GetGameID()
{
	if (_targetObject.IsValid() == false)
	{
		return 0;
	}

	FGsGameObjectData* data = _targetObject->GetData();
	if (data == nullptr)
	{
		return 0;
	}

	return data->GetGameId();
}
// 현재 hp
int FGsScanMemberInfo::GetNowHP()
{
	if (_targetObject.IsValid() == false)
	{
		return 0;
	}
	if (UGsGameObjectCreature* creature = _targetObject->CastGameObject<UGsGameObjectCreature>())
	{
		if (const FGsStatInfo* hpStat = creature->GetCreatureStatInfoByType(StatType::HP))
		{
			return hpStat->GetStatValue();
		}
	}
	return 0;
}
// 최대 hp
int FGsScanMemberInfo::GetMaxHP()
{
	if (_targetObject.IsValid() == false)
	{
		return 0;
	}

	if (UGsGameObjectCreature* creature = _targetObject->CastGameObject<UGsGameObjectCreature>())
	{
		if (const FGsStatInfo* hpStat = creature->GetCreatureStatInfoByType(StatType::HP))
		{
			return hpStat->GetMaxStatValue();
		}
	}
	return 0;
}
// 거리 값
int FGsScanMemberInfo::GetDistance()
{
	if (_targetObject.IsValid() == false)
	{
		return 0;
	}

	FVector2D targetObjectPos = FVector2D(_targetObject->GetLocation());
	FVector2D localObjectPos = FVector2D(_localObject->GetLocation());

	return (localObjectPos - targetObjectPos).Size();
}
// 타게팅인지(local이)
bool FGsScanMemberInfo::GetIsTargeting()
{
	if (_targetObject.IsValid() == false)
	{
		return false;
	}

	FGsTargetHandlerBase* targetHandler = _localObject->GetTargetHandler();
	if (targetHandler == nullptr)
	{
		return false;
	}

	FGsTargetHandlerLocalPlayer* localTargetHandler = static_cast<FGsTargetHandlerLocalPlayer*>(targetHandler);

	if (localTargetHandler == nullptr)
	{
		return false;
	}

	UGsGameObjectBase* localTargetObj = localTargetHandler->GetLastTarget();
	if (localTargetObj == nullptr ||
		localTargetObj != _targetObject)
	{
		return false;
	}

	return true;
}
// 공격중인지
bool FGsScanMemberInfo::GetIsAttacker()
{
	return GSGameObject()->GetIsAttackerId(GetGameID());
}

// 빌보드 스캔 넘버 처리
void FGsScanMemberInfo::SetBillboardScanNumber()
{
	// 빌보드 스캔 넘버 처리하자
	if (_targetObject.IsValid() == true)
	{
		if (UGsGameObjectCreature* creature = _targetObject->CastGameObject<UGsGameObjectCreature>())
		{
			if (AGsCharacterBase* character = creature->GetCharacter())
			{
				if (UGsUIBillboardCreature* widget = character->GetWidget())
				{
					widget->SetScanMember(true);

					FText orderText = FText::AsNumber(_orderNumber);
					widget->SetScanOrderText(orderText);
				}
			}
		}
	}
}
// 초기화
void FGsScanMemberInfo::Initialize()
{
	SetBillboardScanNumber();
}
// 정리할때
void FGsScanMemberInfo::Finalize()
{
	// 빌보드 스캔 넘버 hide 처리하자
	if (_targetObject.IsValid() == true)
	{
		if (UGsGameObjectCreature* creature = _targetObject->CastGameObject<UGsGameObjectCreature>())
		{
			if (AGsCharacterBase* character = creature->GetCharacter())
			{
				if (UGsUIBillboardCreature* widget = character->GetWidget())
				{
					widget->SetScanMember(false);
				}
			}			
		}
	}

	_targetObject = nullptr;
	_localObject = nullptr;
}

// 갱신 정보 추가
void FGsScanMemberInfo::AddDirtyInfo(EGsScanMemberInfoType In_type)
{
	_dirtyFlag |= (uint32)In_type;
}

// 갱신 정보 초기화
void FGsScanMemberInfo::ClearDirtyInfo()
{
	_dirtyFlag = 0;
}

// 이름 초반에 얻어 오기(딤드 상태에서 object 디스폰일때 이름 갱신용)
void FGsScanMemberInfo::UpdateName()
{	
	if (_targetObject.IsValid() == false)
	{
		_name =  "";
		return;
	}

	FGsGameObjectData* data = _targetObject->GetData();
	if (data == nullptr)
	{
		_name = "";
		return;
	}
	// 1. check remote
	// 2. check inter server map
	// 3. check other server user

	FString scanName = FGsTextUtil::GetTargetName( data, _targetObject->GetObjectType());
	_name = scanName;
}