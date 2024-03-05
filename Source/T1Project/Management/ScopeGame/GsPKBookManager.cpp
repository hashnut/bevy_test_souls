

#include "GsPKBookManager.h"

#include "../ScopeGlobal/GsGameDataManager.h"
#include "../ScopeGlobal/GsNetManager.h"
#include "../GsMessageHolder.h"
#include "../Message/MessageParam/GsMessageParam.h"
#include "../Message/GsMessageContentHud.h"

#include "GsTable.h"
#include "DataSchema/PKBook/GsSchemaPKBookConfig.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"

#include "../UTIL/GsTimeSyncUtil.h"
#include "../Net/GsNetSendServiceWorld.h"
#include "../T1Project.h"
#include "../PKBook/GsPKBookHelper.h"

#include "../UI/UIContent/Helper/GsUIHelper.h"
#include "../UI/UIContent/Tray/NotiMessage/GsUISectionMessageDefine.h"
#include "../UI/UILib/Base/GsUITray.h"



void FGsPKBookManager::Initialize()
{
	_PKRecordDataViewSet.Empty();
	_PKDetectionTargetDataViewSet.Empty();

	_logInFilter = false;
	_activePKAutoMessage = false;	
	_PKTeaseMessage = FText::GetEmpty();
	_PKRevengeMessage = FText::GetEmpty();
	
	_useMockeryMessage = true;
	_mockeryMessage = FText::GetEmpty();

	// 2022/09/28 PKT - Red Dot 초기화
	SetRedDot(false);

	LoadConfig();
}

void FGsPKBookManager::Finalize()
{
	_configData = nullptr;

	_PKRecordDataViewSet.Empty();
	_PKDetectionTargetDataViewSet.Empty();

	_logInFilter = false;
	_activePKAutoMessage = false;
	_PKTeaseMessage = FText::GetEmpty();
	_PKRevengeMessage = FText::GetEmpty();
	
	_useMockeryMessage = true;
	_mockeryMessage = FText::GetEmpty();
}

bool FGsPKBookManager::CheckRedDot(bool bInUpdate /*= true*/)
{
	bool isRedDot = false;

	do 
	{
		if (false == bInUpdate)
		{
			isRedDot = IsRedDot();
			break;
		}

		for (const auto& item : _PKRecordDataHolder)
		{
			if (_checkOutTime < StaticCast<int64>(item.GetOccurrenceTime()))
			{
				isRedDot = true;
				break;
			}
		}

	} while (0);

	if (isRedDot != IsRedDot())
	{	
		SetRedDot(isRedDot);
		// 2022/09/20 PKT - 현재 상태와 다음 상태가 같지 않다면 HUD 업데이트
		GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::PK_BOOK, false);
	}

	return isRedDot;
}

void FGsPKBookManager::MakeRecordDataSet(PD::SC::PKT_SC_PK_BOOK_LIST_READ& InPacket)
{
	// 2022/09/21 PKT - Set CheckOutTime
	_checkOutTime = int64(InPacket.LastCheckOutTime());

	// 2022/09/21 PKT - Set Record Data Set
	const int32 maxCount = InPacket.GetPkRecordInfoListCount();
	_PKRecordDataHolder.Empty(maxCount);

	for (PD::SC::PKT_SC_PK_BOOK_LIST_READ::PkRecordInfoListIterator iter = InPacket.GetFirstPkRecordInfoListIterator();
		iter != InPacket.GetLastPkRecordInfoListIterator(); ++iter)
	{
		FGsPKRecordingContributorData mainContributorData;
		if (PKContributorType::USER == iter->ContributorType())
		{
			MakeContributorDataByUser(mainContributorData
				, iter->PkBookResult()
				, iter->PkContributorId()
				, FText::FromString(iter->PkBookUserName())
				, iter->GuildEmblemId()
				, FText::FromString(iter->PkBookGuildName())
				, CreatureWeaponType::NONE
				, 0				
				, iter->UserWorldId());
		}
		else if (PKContributorType::NPC == iter->ContributorType())
		{
			MakeContributorDataByNPC(mainContributorData, iter->PkBookResult(), iter->PkContributorId());
		}
		else
		{
			GSLOG(Error, TEXT("Invalid ContributorType"));
			continue;
		}

		_PKRecordDataHolder.Emplace(FGsPKRecordData(iter->PkRecordId(), iter->CreatedTime(), mainContributorData, iter->MapId()));
	}
	
	_PKRecordDataHolder.Sort([](const FGsPKRecordData& lhr, const FGsPKRecordData& rhr)
		{	// 2022/09/20 PKT - 최근 시간이 위로 간다.
			return lhr.GetOccurrenceTime() > rhr.GetOccurrenceTime();
		}
	);

	// 2022/09/20 PKT - PK 정보 최대 갯수를 맞춤
	const int32 limitCount = _configData ? _configData->maximumCountOfPKRecord : 0;
	CutCountRecordSet(limitCount);

	// 2022/09/20 PKT - Send Message To Window(Red Dot Only)
	SendWindowMessage(true);
}

void FGsPKBookManager::AddPKRecordData(PD::SC::PKT_SC_ADD_PK_BOOK_INFO_READ& InPacket)
{
	FGsPKRecordingContributorData mainContributorData;
	if (PKContributorType::USER == InPacket.ContributorType())
	{
		MakeContributorDataByUser(mainContributorData, InPacket.PkBookResult(), InPacket.PkContributorId(), FText::FromString(InPacket.PkBookUserName())
			, InPacket.GuildEmblemId(), FText::FromString(InPacket.PkBookGuildName()), CreatureWeaponType::NONE, 0, InPacket.UserWorldId());
	}
	else if (PKContributorType::NPC == InPacket.ContributorType())
	{
		MakeContributorDataByNPC(mainContributorData, InPacket.PkBookResult(), InPacket.PkContributorId());
	}
	else
	{
		GSLOG(Error, TEXT("Invalid ContributorType"));
		return;
	}

	const static int32 PUSH_INDEX = 0;
	_PKRecordDataHolder.Insert(
		FGsPKRecordData(InPacket.PkRecordId(), InPacket.CreatedTime(), mainContributorData, InPacket.MapId())
		, PUSH_INDEX
	);
	
	// 2022/09/20 PKT - PK 정보 최대 갯수를 맞춤
	const int32 limitCount = _configData ? _configData->maximumCountOfPKRecord : 0;
	CutCountRecordSet(limitCount);

	// 2022/09/20 PKT - Send Message To Window
	SendWindowMessage(true, MessageContentPKBook::UI_INVALID_RECORD);	
}

void FGsPKBookManager::SetPKRecordDetailData(PD::SC::PKT_SC_ACK_PK_BOOK_DETAIL_READ& InPacket)
{
	PKRecordDetailInfo detailInfo = InPacket.PkRecordDetailInfo();

	int32 findIndex = FindIndexByRecordDataId(detailInfo.mPKRecordId);
	if (INDEX_NONE >= findIndex)
	{
		GSLOG(Error, TEXT("not find PKRecord Data : [%lu]"), detailInfo.mPKRecordId);
		return;
	}

	// 2022/09/21 PKT - Contributor Record Set
	TArray<FGsPKRecordingContributorData> subContributorRecordSet;
	using ContributorListIterator = PD::SC::PKT_SC_ACK_PK_BOOK_DETAIL_READ::PkContributorListIterator;
	for (ContributorListIterator iter = InPacket.GetFirstPkContributorListIterator(); iter != InPacket.GetLastPkContributorListIterator(); ++iter)
	{
		const PKContributorRecord& contributorRecord = iter->PkContributor();

		FGsPKRecordingContributorData subContributorData;		
		if (PKContributorType::USER == contributorRecord.mContributorType)
		{
			MakeContributorDataByUser(subContributorData
				, contributorRecord.mPkBookResult
				, contributorRecord.mContributorId
				, FText::FromString(iter->UserName())
				, contributorRecord.mGuildEmblemId
				, FText::FromString(iter->GuildName())
				, contributorRecord.mWeaponType
				, contributorRecord.mDamage
				, contributorRecord.mWorldId);
		}
		else if (PKContributorType::NPC == contributorRecord.mContributorType)
		{
			MakeContributorDataByNPC(subContributorData
				, contributorRecord.mPkBookResult
				, contributorRecord.mContributorId
				, contributorRecord.mDamage);
		}
		else
		{
			GSLOG(Error, TEXT("Invalid ContributorType"));
			continue;
		}

		subContributorRecordSet.Emplace(subContributorData);
	}
	
	if (_PKRecordDataHolder.IsValidIndex(findIndex))
	{
		_PKRecordDataHolder[findIndex].SetDetailData(
			subContributorRecordSet, detailInfo.mLostExp, detailInfo.GetExpPercent(), detailInfo.mRemainActionCount);

		SendWindowMessage(true, MessageContentPKBook::UI_OPEN_DETAIL_POPUP, _PKRecordDataHolder[findIndex].GetId());
	}
}

void FGsPKBookManager::SetRevengeRemaindCount(PD::SC::PKT_SC_ACK_PK_BOOK_REVENGE_READ& InPacket)
{
	int32 findIndex = FindIndexByRecordDataId(InPacket.RecordId());
	if (INDEX_NONE >= findIndex)
	{
		GSLOG(Error, TEXT("not find PKRecord Data : [%lu]"), InPacket.RecordId());
		return;
	}

	if (_PKRecordDataHolder.IsValidIndex(findIndex))
	{
		if (false == _PKRecordDataHolder[findIndex].IsDetailDataCaching())
		{
			GSLOG(Error, TEXT(" Has Not been Cached [%lu]"), InPacket.RecordId());
			return;
		}

		_PKRecordDataHolder[findIndex].SetActionRemaindCount(InPacket.RemainCount());

		// 2022/09/20 PKT - RedDot 영향 없음
		SendWindowMessage(false, MessageContentPKBook::UI_INVALID_RECORD_DETAILDATA, InPacket.RecordId());
	}
}

void FGsPKBookManager::SetTeaseRemaindCount(PD::SC::PKT_SC_ACK_PK_BOOK_TEASE_READ& InPacket)
{
	int32 findIndex = FindIndexByRecordDataId(InPacket.RecordId());
	if (INDEX_NONE >= findIndex)
	{
		GSLOG(Error, TEXT("not find PKRecord Data : [%lu]"), InPacket.RecordId());
		return;
	}

	if (_PKRecordDataHolder.IsValidIndex(findIndex))
	{
		if (false == _PKRecordDataHolder[findIndex].IsDetailDataCaching())
		{
			GSLOG(Error, TEXT(" Has Not been Cached [%lu]"), InPacket.RecordId());
			return;
		}

		_PKRecordDataHolder[findIndex].SetActionRemaindCount(InPacket.RemainCount());

		// 2022/09/20 PKT - RedDot 영향 없음
		SendWindowMessage(false, MessageContentPKBook::UI_INVALID_RECORD_DETAILDATA, InPacket.RecordId());
	}
}

void FGsPKBookManager::MakePKDetectionTargetSet(PD::SC::PKT_SC_MORNITORING_USER_LIST_READ& InPacket)
{
	const int32 maxCount = InPacket.GetMornitoringUserListCount();

	_PKDetectionTargetDataHolder.Empty(maxCount);

	for (PD::SC::PKT_SC_MORNITORING_USER_LIST_READ::MornitoringUserListIterator iter = InPacket.GetFirstMornitoringUserListIterator();
		iter != InPacket.GetLastMornitoringUserListIterator(); ++iter)
	{
		FGsPKDetectionTargetData data;
		data._userDBId = iter->UserDBId();
		data._targetName = FText::FromString(FString(iter->UserName()));
		data._lastAccess = iter->LastLogoutTime();
		data._worldId = iter->UserWorldId();

		_PKDetectionTargetDataHolder.Emplace(data);
	}

	SortDetectionTargetSet();

	// 2022/09/21 PKT - 갱신 필요 없음.
	SendWindowMessage(false);
}

void FGsPKBookManager::AddPKDetectionTarget(PD::SC::PKT_SC_ACK_ADD_MONITORING_USER_READ& InPacket)
{
	if (INDEX_NONE < FindIndexByDetectionTargetUserDBId(InPacket.UserDBId()))
	{
		GSLOG(Error, TEXT("Already Detection Target : [%lu]"), InPacket.UserDBId());
		return;
	}

	FGsPKDetectionTargetData data;
	data._userDBId = InPacket.UserDBId();
	data._targetName = FText::FromString(FString(InPacket.UserName()));
	data._lastAccess = InPacket.LastLogoutTime();
	data._worldId = InPacket.UserWorldId();

	_PKDetectionTargetDataHolder.Emplace(data);

	SortDetectionTargetSet();

	// 2022/10/13 PKT - 티커 출력
	FText format;
	FGsPKBookHelper::FindText(TEXT("Message_Add_DetectionTarget"), format);

	FText name = FText::FromString(FGsTextUtil::CombineUserAndPrefix(data._targetName, data._worldId));
	FText message = FText::Format(format, { name });

	FGsUIHelper::TrayMessageTicker(message, true);

	// 2022/09/20 PKT - RedDot 영향 없음
	SendWindowMessage(false, MessageContentPKBook::UI_INVALID_DETECTIONTARGET_LIST);
}

void FGsPKBookManager::RemovePKDetectionTarget(PD::SC::PKT_SC_ACK_DELETE_MONITORING_USER_READ& InPacket)
{
	int32 findIndex = FindIndexByDetectionTargetUserDBId(InPacket.UserDBId());
	if (INDEX_NONE >= findIndex)
	{
		GSLOG(Error, TEXT("not Find Detection Target : [%lu]"), InPacket.UserDBId());
		return;
	}

	if (_PKDetectionTargetDataHolder.IsValidIndex(findIndex))
	{
		// 2022/10/13 PKT - 티커 출력
		FText format;
		FGsPKBookHelper::FindText(TEXT("Message_Clear_DetectionTarget"), format);

		FText name = FText::FromString(FGsTextUtil::CombineUserAndPrefix(_PKDetectionTargetDataHolder[findIndex]._targetName, _PKDetectionTargetDataHolder[findIndex]._worldId));
		FText message = FText::Format(format, name);
		FGsUIHelper::TrayMessageTicker(message, true);

		// 2022/10/13 PKT - 대상 삭제
		_PKDetectionTargetDataHolder.RemoveAt(findIndex);
		
		// 2022/09/20 PKT - RedDot 영향 없음
		SendWindowMessage(false, MessageContentPKBook::UI_INVALID_DETECTIONTARGET_LIST);
	}
}

void FGsPKBookManager::MakePKMessageConfig(PD::SC::PKT_SC_PK_MESSAGE_INFO_READ& InPacket)
{
	_logInFilter = InPacket.CheckIsLogin();
	_activePKAutoMessage = InPacket.IsUsePkMessage();
	_useMockeryMessage = InPacket.UseTeaseMessage();
	_PKTeaseMessage = FText::FromString(InPacket.KillMessage());
	_PKRevengeMessage = FText::FromString(InPacket.DieMessage());	
	_mockeryMessage = FText::FromString(InPacket.TeaseMessage());
}

void FGsPKBookManager::LogInDetectionTarget(PD::SC::PKT_SC_MONITORING_USER_LOGIN_INFO_READ& InPacket)
{
	int32 findIndex = FindIndexByDetectionTargetUserDBId(InPacket.UserDBId());
	if (INDEX_NONE >= findIndex)
	{
		GSLOG(Error, TEXT("not Find Detection Target : [%lu]"), InPacket.UserDBId());
		return;
	}

	if (_PKDetectionTargetDataHolder.IsValidIndex(findIndex))
	{
		if (InPacket.IsLogin())
		{	// 2022/09/21 PKT - 로그인 했다면 마지막 로그인 시간을 지운다.
			static const int64 CLEAR_LAST_ACCESS = 0;
			_PKDetectionTargetDataHolder[findIndex]._lastAccess = CLEAR_LAST_ACCESS;
		}
		else
		{	
			// 2023/5/24 PKT - https://jira.com2us.com/jira/browse/C2URWQ-2301
			// 2023/5/24 PKT - https://jira.com2us.com/jira/browse/C2URWQ-2310
			if (InPacket.WorldId() == GGameData()->GetUserData()->mHomeWorldId)
			{
				// 2022/09/21 PKT - UTC+0 시를 기준으로 넣어 준다.
				_PKDetectionTargetDataHolder[findIndex]._lastAccess = FGsTimeSyncUtil::GetServerNowSecond();
			}
			else
			{
				static const int32 UNKNOWN_LAST_ACCESS = -1;
				_PKDetectionTargetDataHolder[findIndex]._lastAccess = UNKNOWN_LAST_ACCESS;
			}
		}

		// 2022/09/21 PKT - RedDot 과는 관련 없음.
		SendWindowMessage(false, MessageContentPKBook::UI_INVALID_DETECTIONTARGET, InPacket.UserDBId());
	}
}

void FGsPKBookManager::SavePKMessageConfig(PD::SC::PKT_SC_ACK_SAVE_PK_MESSAGE_READ& InPacket)
{
	_activePKAutoMessage = InPacket.UsePKMessage();
	_PKTeaseMessage = FText::FromString(InPacket.KillMessage());
	_PKRevengeMessage = FText::FromString(InPacket.DieMessage());
}

void FGsPKBookManager::SaveMockeryMessage(PD::SC::PKT_SC_ACK_SAVE_PK_TEASE_MESSAGE_READ& InPacket)
{
	_useMockeryMessage = InPacket.UseTeaseMessage();
	_mockeryMessage = FText::FromString(InPacket.TeaseMessage());
}

void FGsPKBookManager::Send_CheckOut(int64 InCheckOut)
{
	_checkOutTime = InCheckOut;

	FGsNetSendServiceWorld::Send_PKBookCheckOutTime(InCheckOut);

	// 2022/09/22 PKT - Red Dot 만 갱신
	SendWindowMessage(true, MessageContentPKBook::NONE);
}


void FGsPKBookManager::CutCountRecordSet(const int32 InLimitCount)
{
	while (InLimitCount < _PKRecordDataHolder.Num())
	{
		int32 maxCount = _PKRecordDataHolder.Num();
		_PKRecordDataHolder.RemoveAt(maxCount - 1);
	}
}

void FGsPKBookManager::SendWindowMessage(bool InUpdateRedDot, MessageContentPKBook InMessage /*= MessageContentPKBook::NONE*/, int64 InParam /*= 0*/)
{
	if (true == InUpdateRedDot)
	{
		CheckRedDot(true);
	}

	// 2022/09/20 PKT - Send Message To Window
	if (MessageContentPKBook::NONE != InMessage)
	{
		const FGsPrimitiveUInt64 param = FGsPrimitiveUInt64(InParam);
		GMessage()->GetPKBook().SendMessage(InMessage, &param);
	}	
}

int32 FGsPKBookManager::FindIndexByRecordDataId(PKRecordId InId) const
{
	return _PKRecordDataHolder.IndexOfByPredicate([InId](const FGsPKRecordData& InData)
		{
			return (InData.GetId() == InId);
		}
	);
}

int32 FGsPKBookManager::FindIndexByDetectionTargetUserDBId(UserDBId InId) const
{
	return _PKDetectionTargetDataHolder.IndexOfByPredicate([InId](const FGsPKDetectionTargetData& InData)
		{
			return (InData._userDBId == InId);
		}
	);
}

const FGsSchemaPKBookConfig* FGsPKBookManager::GetConfig() const
{
	return _configData;
}

int64 FGsPKBookManager::GetCheckOutTime() const
{
	return _checkOutTime;
}

int32 FGsPKBookManager::MakeRecordDataViewSet(EPKBookFilter InFilter)
{
	_PKRecordDataViewSet.Empty();

	for (const auto& item : _PKRecordDataHolder)
	{
		if (EPKBookFilter::All == InFilter)
		{
			_PKRecordDataViewSet.Emplace(&item);
		}
		else if (EPKBookFilter::Kill == InFilter && PKBookResult::KILL == item.GetReason())
		{
			_PKRecordDataViewSet.Emplace(&item);
		}
		else if (EPKBookFilter::Assist == InFilter && PKBookResult::ASSIST == item.GetReason())
		{
			_PKRecordDataViewSet.Emplace(&item);
		}
		else if (EPKBookFilter::Die == InFilter && PKBookResult::DIE == item.GetReason())
		{
			_PKRecordDataViewSet.Emplace(&item);
		}
	}

	return _PKRecordDataViewSet.Num();
}

const TArray<const FGsPKRecordData*>& FGsPKBookManager::GetRecordDataViewSet() const
{
	return _PKRecordDataViewSet;
}

const FGsPKRecordData* FGsPKBookManager::FindPKRecordDataByPKRecordId(PKRecordId InPKRecordId) const
{
	int32 findIndex = FindIndexByRecordDataId(InPKRecordId);

	return _PKRecordDataHolder.IsValidIndex(findIndex) ? &(_PKRecordDataHolder[findIndex]) : nullptr;
}

const int32 FGsPKBookManager::GetDetectionTargetCountAll() const
{
	return _PKDetectionTargetDataHolder.Num();
}

// 2023/3/16 PKT - 경계대상 Index로 찾는다.(View Data가 아닌 점에 주의!!! 지금은 절전 모드에서 사용)
const FGsPKDetectionTargetData* FGsPKBookManager::GetDetectionTargetAt(int32 InIndex) const
{
	return _PKDetectionTargetDataHolder.IsValidIndex(InIndex) ? &(_PKDetectionTargetDataHolder[InIndex]) : nullptr;
}

int32 FGsPKBookManager::MakeDetectionTargetViewSet(EPKDetectionTargetFilter InFilter)
{
	_PKDetectionTargetDataViewSet.Empty();

	const static int32 LOGIN_INDEX = 0;

	for (const auto& item : _PKDetectionTargetDataHolder)
	{
		if (EPKDetectionTargetFilter::All == InFilter)
		{
			_PKDetectionTargetDataViewSet.Emplace(&item);
		}
		else if (EPKDetectionTargetFilter::Connect == InFilter && LOGIN_INDEX == item._lastAccess )
		{	// 2022/11/04 PKT - 타 서버는 _lastAccess가 0보다 작은 수이기 때문에... 0과 같은 지를 검사
			_PKDetectionTargetDataViewSet.Emplace(&item);
		}
	}

	return _PKDetectionTargetDataViewSet.Num();
}

const TArray<const FGsPKDetectionTargetData*>& FGsPKBookManager::GetDetectionTargetDataViewSet() const
{
	return _PKDetectionTargetDataViewSet;
}

const FGsPKDetectionTargetData* FGsPKBookManager::FindDetectionTargetDataById(UserDBId InUserDBId) const
{
	int32 findIndex = FindIndexByDetectionTargetUserDBId(InUserDBId);

	return _PKDetectionTargetDataHolder.IsValidIndex(findIndex) ? &(_PKDetectionTargetDataHolder[findIndex]) : nullptr;
}

bool FGsPKBookManager::CheckDetectionTarget(UserDBId InUserDBId, const FText& InUserName) const
{
	auto result = FGsTextUtil::IsValidUserSearchNameOnly(InUserName.ToString(), false);
	if (FGsTextUtil::EUserNameValid::Valid != result.Get<0>())
	{
		return false;
	}

	return nullptr != _PKDetectionTargetDataHolder.FindByPredicate([InUserDBId, result](const FGsPKDetectionTargetData& InData)
		{
			if (0 < InData._userDBId && InData._userDBId == InUserDBId)
			{
				return true;
			}
			else if (InData._targetName.EqualTo(result.Get<2>()) && InData._worldId == result.Get<1>())
			{
				return true;
			}
			return false;
		}
	);
}

bool FGsPKBookManager::CheckDetectionTarget(UserDBId InUserDBId, WorldId InWorldId, const FText& InUserName) const
{
	return nullptr != _PKDetectionTargetDataHolder.FindByPredicate([InUserDBId, InWorldId, InUserName](const FGsPKDetectionTargetData& InData)
		{
			if (0 < InData._userDBId && InData._userDBId == InUserDBId)
			{
				return true;
			}
			else if (InData._targetName.EqualTo(InUserName) && InWorldId == InData._worldId)
			{
				return true;
			}
			return false;
		}
	);
}

bool FGsPKBookManager::ActionCostInfo(PKBookResult InResult, CurrencyType& OutType, Currency& OutAmount) const
{
	OutType = CurrencyType::NONE;
	OutAmount = 0;

	if (nullptr == _configData)
	{
		GSLOG(Error, TEXT("nullptr == _configData"));
		return false;
	}

	if (PKBookResult::DIE == InResult)
	{
		OutType = _configData->revengeCostType;
		OutAmount = _configData->revengeCost;
	}
	else
	{
		OutType = _configData->teaseCostType;
		OutAmount = _configData->teaseCost;
	}

	return true;
}

bool FGsPKBookManager::IsShowLogInDetectionTargetOnly() const
{
	return _logInFilter;
}
bool FGsPKBookManager::IsPKMessageActive() const
{
	return _activePKAutoMessage;
}

const FText& FGsPKBookManager::GetTeasePKMessage() const
{
return _PKTeaseMessage;
}

const FText& FGsPKBookManager::GetRevengePKMessage() const
{
	return _PKRevengeMessage;
}

bool FGsPKBookManager::IsUseMockeryMessage() const
{
	return _useMockeryMessage;
}

const FText& FGsPKBookManager::GetMockeryMessage() const
{
	return _mockeryMessage;
}

void FGsPKBookManager::LoadConfig()
{
	const UGsTable* table = FGsSchemaPKBookConfig::GetStaticTable();
	if (nullptr == table)
	{
		GSLOG(Error, TEXT("nullptr == table"));
		return;
	}

	const TMap<FName, uint8*>* OutRowMap;
	if (false == table->GetRowMap(OutRowMap))
	{
		GSLOG(Error, TEXT("false == table->GetRowMap(OutRowMap)"));
		return;
	}

	// 2022/06/09 PKT - Config Data는 한개만 존재해야 한다.
	const static int32 CONFIG_DATA_ONLY_ONE = 1;
	if (CONFIG_DATA_ONLY_ONE != OutRowMap->Num())
	{
		GSLOG(Warning, TEXT("CONFIG_DATA_ONLY_ONE != OutRowMap->Num()"));
	}

	for (const auto& pair : (*OutRowMap))
	{	// 2022/06/09 PKT - 하나만 적용 시키고 빠져 나간다.
		_configData = reinterpret_cast<const FGsSchemaPKBookConfig*>(pair.Value);
		break;
	}
}

void FGsPKBookManager::MakeContributorDataByUser(
	FGsPKRecordingContributorData& OutData, PKBookResult InResult, PKContributorId InId, const FText& InUserName, GuildEmblemId InEmblemId
	, const FText& InGuildName, CreatureWeaponType InWeaponType /*= CreatureWeaponType::NONE*/, int32 InDamegeAmount /*= 0*/, WorldId InHomeWorldId /*= INVALID_WORLD_ID*/)
{
	OutData._reason = InResult;
	OutData._contributorType = PKContributorType::USER;
	OutData._contributorId = InId;
	OutData._name = InUserName;
	OutData._guildEmblemId = InEmblemId;
	OutData._guildName = InGuildName;
	OutData._weaponType = InWeaponType;
	OutData._damegeAmount = InDamegeAmount;
	OutData._homeWorldId = InHomeWorldId;
}

void FGsPKBookManager::MakeContributorDataByNPC(
	FGsPKRecordingContributorData& OutData, PKBookResult InResult, PKContributorId InId, int32 InDamegeAmount /*= 0*/)
{
	OutData._reason = InResult;
	OutData._contributorType = PKContributorType::NPC;
	OutData._contributorId = InId;
	OutData._guildEmblemId = INVALID_GUILD_EMBLEM_ID;
	OutData._guildName = FText::GetEmpty();
	OutData._weaponType = CreatureWeaponType::NONE;
	OutData._damegeAmount = InDamegeAmount;


	const UGsTableNpcData* npcDataTable = Cast<UGsTableNpcData>(FGsSchemaNpcData::GetStaticTable());
	const FGsSchemaNpcData* findNpcData = nullptr;
	if (nullptr == npcDataTable || false == npcDataTable->FindRowById(StaticCast<int32>(InId), findNpcData))
	{	// 2022/10/12 PKT - "UnknownNPC : 알 수 없는 몬스터"
		FGsPKBookHelper::FindText(TEXT("Message_Not_Find_ContributorDataNPC"), OutData._name);
	}
	else
	{
		OutData._name = findNpcData->nameText;
	}
}

void FGsPKBookManager::SortDetectionTargetSet()
{
	_PKDetectionTargetDataHolder.Sort([](const FGsPKDetectionTargetData& lhr, const FGsPKDetectionTargetData& rhr)
		{
			return lhr._targetName.ToString() < rhr._targetName.ToString();
		}
	);
}
