// Fill out your copyright notice in the Description page of Project Settings.


#include "GsMailManager.h"

#include "Engine/World.h"

#include "Shared/Shared/SharedInclude/SharedDefine.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Net/GsNetSendServiceWorld.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/GsScopeGlobal.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/GsMessageHolder.h"
#include "Message/GsMessageContents.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/GsContentsMode.h"

#include "Mail/GsMailInfo.h"
#include "Mail/GsMailDefine.h"

#include "Item/GsItemManager.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/GsUIPopupItemReceive.h"
#include "UTIL/GsTimeSyncUtil.h"

#include "T1Project.h"
#include "JsonReader.h"
#include "JsonSerializer.h"
#include "JsonObject.h"
#include "TimerManager.h"
#include "FileHelper.h"

void FGsMailManager::Initialize()
{	
	FGsMessageHolder* msg = GMessage();
	
	_gameObjectDelegate = msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE,
			this, &FGsMailManager::OnLocalPlayerSpawnComplete);

	_gameObjecReviveDelegate = msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_REVIVE,
		this, &FGsMailManager::OnLocalPlayerSpawnComplete);

	_mailtDelegate = msg->GetMail().AddRaw(MessageContentMail::NOTIFY_MAIL_UNREAD,
		this, &FGsMailManager::MailNotifyUnread);

	if (UWorld* world = GUI()->GetWorld())
	{
		// 존재하고 있는 타이머가 이미 있다면 제거
		if (_timerHandle.IsValid())
		{
			world->GetTimerManager().ClearTimer(_timerHandle);
		}

		world->GetTimerManager().SetTimer(_timerHandle,
			FTimerDelegate::CreateRaw(this, &FGsMailManager::OnTimer), 5.f, true);
	}
}

void FGsMailManager::InitializeMailData()
{
	Clear();
}

void FGsMailManager::Finalize()
{
	Clear();

	FGsMessageHolder* msg = GMessage();
	msg->GetGameObject().Remove(_gameObjectDelegate);
	msg->GetGameObject().Remove(_gameObjecReviveDelegate);
	msg->GetMail().Remove(_mailtDelegate);

	if (_timerHandle.IsValid())
	{
		if (UWorld* world = GUI()->GetWorld())
		{
			world->GetTimerManager().ClearTimer(_timerHandle);
			_timerHandle.Invalidate();
		}
	}
}

void FGsMailManager::OnTimer()
{
	bool isUpdateRedDot = false;
	for (int i = 0; i < (int)MailBoxType::MAX; ++i)
	{
		if (const TMap<MailDBId, TSharedPtr<FGsMailInfo>>* mailMap = GetMailBoxTypeData((MailBoxType)i))
		{
			for (auto iter = mailMap->CreateConstIterator(); iter; ++iter)
			{
				TSharedPtr<FGsMailInfo> data = iter->Value;
				if (data.IsValid())
				{
					if (false == data->GetIsRemainTime())
					{
						DeleteMail(data->GetMailBoxType(), data->GetMailDBId());
						isUpdateRedDot = true;
						++iter;
					}
				}
			}
		}
	}
	
	if (isUpdateRedDot)
	{
		GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::MAIL, false);
	}
}

void FGsMailManager::Clear()
{
	_mailMap.Empty();
	_guildMailMap.Empty();
	_accountMailMap.Empty();
}

void FGsMailManager::OnLocalPlayerSpawnComplete(const IGsMessageParam*)
{
	FGsGameDataManager* gameDataManager = GGameData();
	if (nullptr == gameDataManager)
		return;

	if (gameDataManager->IsUnReadMail())
	{
		SendReqMailList();		
	}

	if (gameDataManager->IsNewSystemMail())
	{
		SendReqNewSystmeMailRecv();
	}
}

void FGsMailManager::MailNotifyUnread(const IGsMessageParam* inParam)
{
	FGsGameFlowManager* flowManager = GMode();
	if (nullptr == flowManager)
		return;
	
	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
		return;

	if (nullptr != inParam)
	{
		const FGsPrimitiveInt32* param = inParam->Cast<const FGsPrimitiveInt32>();
		bool inNewTickerEffect = static_cast<bool>(param->_data);

		if (inNewTickerEffect)
		{
			FText findText;
			// 새로운 우편이 도착했습니다.
			FText::FindText(TEXT("MailText"), TEXT("Mail_New"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
		}
	}

	// 레드닷 갱신
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::MAIL, true);
}

// 메일리스트를 서버에서 받다.
void FGsMailManager::Set(PD::SC::PKT_SC_ACK_MAIL_LIST_READ& Packet)
{
	for (PD::SC::PKT_SC_ACK_MAIL_LIST_READ::MailDataListIterator iter = Packet.GetFirstMailDataListIterator();
		iter != Packet.GetLastMailDataListIterator(); ++iter)
	{
		// 메일 DB아이디
		MailDBId		mailDBId		= iter->MailDBId();
		// 메일 박스 타입 (개인, 길드, 계정)
		MailBoxType		boxType			= iter->BoxType();
		// 메일 타입 아이디
		MailTypeId		mailTypeId		= iter->MailTypeId();
		// 메일 스트링 테이블 아이디
		MailStringId	mailStringId	= iter->MailStringId();
		// 메일이 삭제되는 시간
		time_t			expireTime		= iter->TimeExpire();
		// 레드닷
		bool			isRead			= (0 < iter->TimeRead())? true : false;
		// 메일 가변파람 스트링
		const FString stringVarParamter = iter->MailStringVarParamter();
		// 메일 카운트 포함 여부
		bool		isLimitedQuantity	= iter->IsLimitedQuantity();
		// 직접 입력한 메일 제목, 내용
		FString mailString				= iter->MailString();
		
		TArray<TSharedPtr<FGsMailStringData>> mailStringDataList;
		GetMailString(mailString, mailStringDataList);

#if WITH_EDITOR
		GSLOG(Log, TEXT("[Mail] ACK_MAIL_LIST ID: %llu"), mailDBId);
#endif
		TArray<TSharedPtr<FGsMailAttachment>> itemDataList;
		for (PD::SC::PKT_SC_ACK_MAIL_LIST_READ::MailDataList::MailAttachmentArrayIterator mailAttachmentIter = iter->GetFirstMailAttachmentArrayIterator();
			mailAttachmentIter != iter->GetLastMailAttachmentArrayIterator(); ++mailAttachmentIter)
		{
			// 보상타입 (아이템, 재화..)
			MailAttachmentType		type	= mailAttachmentIter->Type();
			// 보상타입이 재화면 재화타입, 아이템이면 아이템아이디
			MailAttachmentId		id		= mailAttachmentIter->Id();
			// 갯수
			MailAttachmentAmount	amount	= mailAttachmentIter->Amount();
			// 이미 생성되어있는 아이템일 경우, 필요한 값. (클라는 무시해도 되는 변수???) 
			ItemDBId itemDBId = mailAttachmentIter->ItemDBId();
			// 아이템의 레벨
			Level itemLevel = mailAttachmentIter->ItemLevel();
			// 아이템의 내구도, UNCONFIRMED_ENCHANT_SHIELD 일 경우는 수령 시 결정 / 아닐 경우는 확정된 내구도 값
			EnchantShield enchantShield = mailAttachmentIter->EnchantShield();
			// 아이템의 특성, UNCONFIRMED_ENCHANT_BONUS_INDEX 일 경우는 시 결정 / 아닐 경우는 확정된 특성 인덱스
			EnchantBonusIndex enchantBonusIndex = mailAttachmentIter->EnchantBonusIndex();
			
			itemDataList.Emplace(
				MakeShareable(new FGsMailAttachment(type, id, amount, itemDBId, itemLevel, enchantShield, enchantBonusIndex)));
		}		

		// 저장
		FDateTime expireDateTime = FGsTimeSyncUtil::ConvertToDateTime(expireTime);

		if (0 < expireTime)
		{
#if WITH_EDITOR
			GSLOG(Warning, TEXT("[Mail] MailDBId : %llu, ExpireTime: %s / ServerTime: %s, UtcNow: %s, Now: %s"), mailDBId,
				*expireDateTime.ToString(), *FGsTimeSyncUtil::GetServerNowDateTime().ToString(), *FDateTime::UtcNow().ToString(), *FDateTime::Now().ToString());
#endif
		}

		AddMail(MakeShareable(new FGsMailInfo(mailDBId, (MailBoxType)boxType, mailTypeId, expireDateTime, isRead,
															itemDataList, mailStringId, stringVarParamter, isLimitedQuantity, mailStringDataList)));
		
	}

	// UI 갱신
	GMessage()->GetMail().SendMessage(MessageContentMail::ACTIVE_WINDOW_INVALIDTEALL, nullptr);
	// 레드닷 갱신
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::MAIL, false);
	// BlockUI 닫기 
	FGsUIHelper::HideBlockUI();
}

// 우편 수령(삭제) or 모두 수령(삭제) 요청 응답을 받는다.
void FGsMailManager::Set(PD::SC::PKT_SC_ACK_MAIL_RECV_READ& Packet)
{
	int32_t result = Packet.Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		// 응답받은 메일갯수 모두 차감
		// 실패가 왔으면 모두 받기 중지
		// 받은 만큼 팝업 오픈
		ResetRequestCount();
		// 티커 연출
		MailErrorMsg(static_cast<PD::Result>(result));

		if (static_cast<PD::Result>(result) == PD::Result::MAIL_ERROR_ALREADY_OHTER_SERVER_RECV)
		{
			if (MailBoxType::ACCOUNT == Packet.BoxType())
			{
				MailDBId recvedMailDBId = Packet.RecvedMailDBId();
				TSharedPtr<FGsMailInfo> outData;
				if (true == GetMailData(recvedMailDBId, outData))
				{
					if (outData->GetIsVIDMail())
					{
						DeleteMail(MailBoxType::ACCOUNT, recvedMailDBId);
					}
				}
			}
		}
	}
	else	
	{
		MailDBId mailDBId = Packet.RecvedMailDBId();

		//if (_reqType == EGMailReqType::ReceiveAll)
			_requestList.Add(mailDBId);	
			/*else
				DeleteMail((MailBoxType)Packet.BoxType(), mailDBId);*/

		// 응답받은 메일갯수 차감
		RemoveRequestCount();
	}
	
	// 응답을 모두받았는가?
	if (IsRequestOver())
	{
		// BlockUI 닫기 
		FGsUIHelper::HideBlockUI();

		/*if (_reqType == EGMailReqType::ReceiveAll)
		{*/
			if (0 < _requestList.Num())
				OpenItemReceive();	
			/*	}
				else
				{
					if(result == PACKET_RESULT_SUCCESS)
						MailReqMsg(_reqType);
				}*/

		SetReqType(EGMailReqType::None);
	}

	// UI 갱신
	GMessage()->GetMail().SendMessage(MessageContentMail::ACTIVE_WINDOW_INVALIDTEALL, nullptr);
}

// 읽음 상태 전송 결과를 받는다.
void FGsMailManager::Set(PD::SC::PKT_SC_ACK_MAIL_READ_READ& Packet)
{
	MailDBId mailDBId = Packet.ReadedMailDBId();

	TSharedPtr<FGsMailInfo> mailInfo;
	GetMailData(mailDBId, mailInfo);

	if (mailInfo.IsValid())
	{
		mailInfo->SetRead();
	}
}

// 메일 추가
void FGsMailManager::AddMail(TSharedPtr<FGsMailInfo> inDataMailData)
{
	MailBoxType boxType = inDataMailData->GetMailBoxType();
	MailDBId mailDBId = inDataMailData->GetMailDBId();


	if (TMap<MailDBId, TSharedPtr<FGsMailInfo>>* mailMap = GetMailBoxTypeData(boxType))
	{		
		if (!mailMap->Contains(mailDBId))
		{
			mailMap->Emplace(mailDBId, inDataMailData);
		}

		if (false == inDataMailData->GetIsLimitedQuantity())
		{
			return;
		}

		MailDBId oldMailDBId = 0;
		int mailCount = 0;
		for (auto& iter : *mailMap)
		{
			TSharedPtr<FGsMailInfo> mailInfo = iter.Value;
			if (mailInfo.IsValid())
			{
				if (true == mailInfo->GetIsLimitedQuantity())
				{
					++mailCount;
					if (0 == oldMailDBId)
					{
						oldMailDBId = mailInfo->GetMailDBId();
					}
					else
					{
						if (oldMailDBId > mailInfo->GetMailDBId())
							oldMailDBId = mailInfo->GetMailDBId();
					}					
				}
			}
		}

		if (mailCount > MAX_LIMITED_QUANTITY_MAIL_LOAD_COUNT)
		{
			mailMap->Remove(oldMailDBId);
		}
	}
}

// 메일 삭제
void FGsMailManager::DeleteMail(MailBoxType inBoxType, MailDBId inMailDBId)
{
	if (TMap<MailDBId, TSharedPtr<FGsMailInfo>>* mailMap = GetMailBoxTypeData(inBoxType))
	{
		mailMap->Remove(inMailDBId);

		// UI 갱신
		GMessage()->GetMail().SendMessage(MessageContentMail::ACTIVE_WINDOW_INVALIDTEALL, nullptr);
		GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::MAIL, false);
	}
}

// 우편 신규 시스템 메일 우편함에 지급 요청
void FGsMailManager::SendReqNewSystmeMailRecv()
{
	// 우편리스트를 받으면 우편으로 다시 판단 할 것이기에 상태를 false로 바꾼다.
	GGameData()->SetNewSystemMail(false);
	FGsNetSendServiceWorld::SendReqNewSystmeMailRecv();
}

// 우편 리스트 요청
void FGsMailManager::SendReqMailList()
{
	// 우편리스트를 받으면 우편으로 다시 판단 할 것이기에 상태를 false로 바꾼다.
	GGameData()->SetUnReadMail(false);
	FGsNetSendServiceWorld::SendReqMailList();
}

// 모두 받기 요청
void FGsMailManager::SendMailReceiveAll(MailBoxType inBoxType)
{
	// 아이템이 있는 애들만 모아서 리스트만들기
	TArray<MailDBId> outMailDBIdList;
	GetMailDBIdList(inBoxType, true, outMailDBIdList);

	int count = outMailDBIdList.Num();
	if (0 < count)
	{
		if (MailBoxType::ACCOUNT == inBoxType)
		{
			FText msg;
			FText::FindText(TEXT("MailText"), TEXT("MailAccept_Server"), msg);

			FGsUIHelper::PopupSystemYesNo(msg, [this, inBoxType, outMailDBIdList, count](bool bYes)
				{
					if (bYes)
					{
						TArray<MailDBId> mailDBIdList = outMailDBIdList;
						FGsNetSendServiceWorld::SendReqMailRecv(inBoxType, mailDBIdList);
						// 모두 받기 메일 갯수 저장
						SetRequestCount(count);
						// 모두 받기
						SetReqType(EGMailReqType::ReceiveAll);
					}
				});
		}
		else
		{
			FGsNetSendServiceWorld::SendReqMailRecv(inBoxType, outMailDBIdList);
			// 모두 받기 메일 갯수 저장
			SetRequestCount(count);
			// 모두 받기
			SetReqType(EGMailReqType::ReceiveAll);
		}		
	}
}

// 모두 삭제 요청
void FGsMailManager::SendMailDeleteAll(MailBoxType inBoxType)
{
	// 아이템 없는 애들만 모아서 리스트만들기

	TArray<MailDBId> outMailDBIdList;
	GetMailDBIdList(inBoxType, false, outMailDBIdList);

	int count = outMailDBIdList.Num();
	if (0 < count)
	{	
		FGsNetSendServiceWorld::SendReqMailRecv(inBoxType, outMailDBIdList);
		// 모두 삭제 메일 갯수 저장
		SetRequestCount(count);
		// 모두 삭제
		SetReqType(EGMailReqType::DeleteAll);
	}
	else
	{
		FText findText;
		// 삭제할 수 있는 우편이 없습니다. 보상이 담긴 우편은 삭제할 수 없습니다.
		FText::FindText(TEXT("MailText"), TEXT("MailDelete_Fail"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
	}
}

// 받기 요청
void FGsMailManager::SendMailReceive(MailBoxType inBoxType, MailDBId inMailDBId)
{
	TSharedPtr<FGsMailInfo> outData;
	if (false == GetMailData(inMailDBId, outData))
	{
		MailErrorMsg(PD::Result::MAIL_ERROR_INVALID_MAIL_DB_ID);
		return;
	}
	
	if (outData.IsValid())
	{
		if (true == outData->GetIsRemainTime())
		{
			TArray<MailDBId> mailDBIdList;
			mailDBIdList.Add(inMailDBId);

			if (MailBoxType::ACCOUNT == inBoxType)
			{
				FText msg;
				if (outData->GetIsVIDMail())
				{					
					FText::FindText(TEXT("MailText"), TEXT("MailAccept_Account"), msg);
				}
				else
				{
					FText::FindText(TEXT("MailText"), TEXT("MailAccept_Server"), msg);
				}

				FGsUIHelper::PopupSystemYesNo(msg, [this, inBoxType, mailDBIdList](bool bYes)
				{
					if (bYes)
					{
						TArray<MailDBId> sendMailDBIdList = mailDBIdList;
						FGsNetSendServiceWorld::SendReqMailRecv(inBoxType, sendMailDBIdList);
						// 받기
						SetReqType(EGMailReqType::ReceiveOne);
					}
				});
			}			
			else
			{
				FGsNetSendServiceWorld::SendReqMailRecv(inBoxType, mailDBIdList);
				// 받기
				SetReqType(EGMailReqType::ReceiveOne);
			}
		}
		else
		{
			DeleteMail(inBoxType, inMailDBId);
			MailErrorMsg(PD::Result::MAIL_ERROR_RECV_ALREADY_EXPIRED);
		}
	}
}

// 삭제 요청
void FGsMailManager::SendMailDelete(MailBoxType inBoxType, MailDBId inMailDBId)
{
	TSharedPtr<FGsMailInfo> outData;
	if (false == GetMailData(inMailDBId, outData))
	{
		MailErrorMsg(PD::Result::MAIL_ERROR_INVALID_MAIL_DB_ID);
		return;
	}

	if (outData.IsValid())
	{
		if (true == outData->GetIsRemainTime())
		{
			TArray<MailDBId> mailDBIdList;
			mailDBIdList.Add(inMailDBId);

			FGsNetSendServiceWorld::SendReqMailDelete(inBoxType, mailDBIdList);
			// 삭제
			SetReqType(EGMailReqType::DeleteOne);
		}
		else
		{
			DeleteMail(inBoxType, inMailDBId);
			MailErrorMsg(PD::Result::MAIL_ERROR_RECV_ALREADY_EXPIRED);
		}
	}
}

// 읽음 상태 전송
void FGsMailManager::SendReqMailRead(MailBoxType inBoxType, MailDBId inMailDBId)
{
	TSharedPtr<FGsMailInfo> mailInfo;
	GetMailData(inMailDBId, mailInfo);

	if (mailInfo.IsValid())
	{
		if(false == mailInfo->GetIsRead())
			FGsNetSendServiceWorld::SendReqMailRead(inBoxType, inMailDBId);
	}
}

// 타입별 메일데이터 리스트
TMap<MailDBId, TSharedPtr<FGsMailInfo>>* FGsMailManager::GetMailBoxTypeData(MailBoxType inBoxType)
{
	switch (inBoxType)
	{
	case MailBoxType::USER:
		return &_mailMap;
		break;
	case MailBoxType::GUILD:
		return &_guildMailMap;
		break;
	case MailBoxType::ACCOUNT:
		return &_accountMailMap;
		break;
	default:
		break;
	}

	return nullptr;
}

// 타입별 아이템포함 여부분류별 메일데이터 리스트
void FGsMailManager::GetMailDBIdList(MailBoxType inBoxType, bool isRecv, OUT TArray<MailDBId>& outMailDBIdList)
{
	outMailDBIdList.Empty();

	if (const TMap<MailDBId, TSharedPtr<FGsMailInfo>>* mailMap = GetMailBoxTypeData(inBoxType))
	{
		for (auto& iter : *mailMap)
		{
			TSharedPtr<FGsMailInfo> mailInfo = iter.Value;
			if (mailInfo.IsValid())
			{
				if (mailInfo->GetIsRewards() == isRecv)
				{
					if (inBoxType == MailBoxType::ACCOUNT)
					{
						if (true == mailInfo->GetIsVIDMail())
						{
							continue;
						}
					}
					outMailDBIdList.Add(iter.Key);
				}
			}
		}
	}
}

// 메일타입 별 우편리스트
void FGsMailManager::GetMailList(MailBoxType inBoxType, OUT TArray<TSharedPtr<FGsMailInfo>>& outList)
{
	outList.Empty();

	if (TMap<MailDBId, TSharedPtr<FGsMailInfo>>* mailMap = GetMailBoxTypeData(inBoxType))
	{	
		for (auto& iter : *mailMap)
		{
			TSharedPtr<FGsMailInfo> mailInfo = iter.Value;
			if (mailInfo.IsValid())
			{				
				outList.Emplace(mailInfo);				
			}
		}
	}

	// [B1] | ejrrb10 | 우편 취합 이슈(C2URWQ-5054) 1번 수정 (신규 메일이 위로 오도록 정렬) 
	outList.Sort([](const TSharedPtr<FGsMailInfo>& a, const TSharedPtr<FGsMailInfo>& b) {
		if (a.IsValid() && b.IsValid())
		{
			return a->GetMailDBId() > b->GetMailDBId();
		}
		else
		{
			return false;
		}
		});
}

// 우편ID로 우편데이터 찾기
bool FGsMailManager::GetMailData(MailDBId inMailDBId, OUT TSharedPtr<FGsMailInfo>& outData)
{
	for (int i = 0; i < (int)MailBoxType::MAX; ++i)
	{
		if (const TMap<MailDBId, TSharedPtr<FGsMailInfo>>* mailMap = GetMailBoxTypeData((MailBoxType)i))
		{
			if (mailMap->Contains(inMailDBId))
			{
				outData = mailMap->FindRef(inMailDBId);
				return true;
			}
		}
	}

	return false;
}

// 우편타입 별 우편리스트 갯수
int FGsMailManager::GetMailBoxTypeDataListCount(MailBoxType inBoxType)
{
	const TMap<MailDBId, TSharedPtr<FGsMailInfo>>* mailMap = GetMailBoxTypeData(inBoxType);
	return (nullptr == mailMap)? 0 : mailMap->Num();
}

int FGsMailManager::GetMailCount()
{
	int outCount = 0;
	for (int i = 0; i < (int)MailBoxType::MAX; ++i)
	{
		if (const TMap<MailDBId, TSharedPtr<FGsMailInfo>>* mailMap = GetMailBoxTypeData((MailBoxType)i))
		{
			outCount += mailMap->Num();
		}
	}

	return outCount;
}

void FGsMailManager::IsRedDot(MailBoxType inBoxType, OUT bool& outIsRedDot)
{
	if (const TMap<MailDBId, TSharedPtr<FGsMailInfo>>* mailMap = GetMailBoxTypeData(inBoxType))
	{
		outIsRedDot = false;
		TArray<MailDBId> deletList;
		for (auto& iter : *mailMap)
		{
			TSharedPtr<FGsMailInfo> mailInfo = iter.Value;
			if (mailInfo.IsValid())
			{
				if (mailInfo->GetIsRemainTime())
				{
					bool tempValue = (mailInfo->GetIsRead()) ? false : true;
					outIsRedDot = outIsRedDot | tempValue;
					outIsRedDot = outIsRedDot | mailInfo->GetIsRewards();
				}
			}
		}
	}
}

bool FGsMailManager::IsRedDot()
{
	if (true == GGameData()->IsUnReadMail())
	{
		/*if (0 == GetMailCount())
			return false;
		else
			return true;*/

		return true;
	}

	bool isRedDot = false;
	for (int i = 0; i < (int)MailBoxType::MAX; ++i)
	{
		bool outIsRedDot = false;
		IsRedDot((MailBoxType)i, outIsRedDot);

		isRedDot = isRedDot | outIsRedDot;
	}
	return isRedDot;
}

// 모두 받기 시 보상리스트 팝업
void FGsMailManager::OpenItemReceive()
{
	TArray<TSharedPtr<FGsMailAttachment>> attachmentList;
	AllItemList(attachmentList);

	if (0 < attachmentList.Num())
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupItemReceive"));
		if (widget.IsValid())
		{
			if (UGsUIPopupItemReceive* popup = Cast<UGsUIPopupItemReceive>(widget.Get()))
			{
				popup->SetData(attachmentList);
			}
		}
	}
}

// 모두받기로 받은 첨부보상아이템 리스트
void FGsMailManager::AllItemList(OUT TMap<uint32, int>& outCurrencyList, OUT TMap<uint32, int>& outItemList)
{
	for (MailDBId id : _requestList)
	{
		TSharedPtr<FGsMailInfo> info;
		GetMailData(id, info);

		if (true == info->GetIsRewards())
		{
			const TArray<TSharedPtr<FGsMailAttachment>>& Attachment = info->GetMailAttachmentList();
			for (TSharedPtr<FGsMailAttachment> attachmentIter : Attachment)
			{
				const MailAttachmentId attachmentId = attachmentIter->GetId();
				const MailAttachmentAmount attachmentAmount = attachmentIter->GetAmount();
				uint32 amount = attachmentAmount;

				switch (attachmentIter->GetType())
				{
				case MailAttachmentType::CURRENCY:
				{
					if (true == outCurrencyList.Contains(attachmentId))
						amount += outCurrencyList.FindRef(attachmentId);

					outCurrencyList.Add(attachmentId, amount);
				}
				break;
				case MailAttachmentType::ITEM:
				{					
					if (true == outItemList.Contains(attachmentId))
						amount += outItemList.FindRef(attachmentId);				
					
					outItemList.Add(attachmentId, amount);					
				}
				break;
				default:
					break;
				}
			}
		}

		// 팝업 넘겼으니 삭제!!
		DeleteMail(info->GetMailBoxType(), info->GetMailDBId());
	}

	_requestList.Empty();
}

void FGsMailManager::AllItemList(OUT TArray<TSharedPtr<FGsMailAttachment>>& outItemList)
{
	for (MailDBId id : _requestList)
	{
		TSharedPtr<FGsMailInfo> info;
		GetMailData(id, info);

		if (true == info->GetIsRewards())
		{
			const TArray<TSharedPtr<FGsMailAttachment>>& Attachment = info->GetMailAttachmentList();
			for (TSharedPtr<FGsMailAttachment> attachmentIter : Attachment)
			{				
				outItemList.Emplace(attachmentIter);
			}
		}

		// 팝업 넘겼으니 삭제!!
		DeleteMail(info->GetMailBoxType(), info->GetMailDBId());
	}

	_requestList.Empty();
}

// 서버에서 내려준 에러타입 별 티커
void FGsMailManager::MailErrorMsg(PD::Result inResult)
{
	FText findText;

	switch (inResult)
	{
	case PD::Result::MAIL_ERROR_RECV_ALREADY_EXPIRED:
		// 우편 받기 실패 - 만료된 우편
		FText::FindText(TEXT("MailText"), TEXT("MailError_Expired"), findText);
		break;
	case PD::Result::MAIL_ERROR_CURRENCY_EXCEEDED:
		// 우편 - 재화 보유량 초과
		FText::FindText(TEXT("MailText"), TEXT("MailError_Exceeded"), findText);
		break;
	case PD::Result::MAIL_ERROR_RECV_INVENTORY_FULL:
	case PD::Result::ITEM_ERROR_ADD_INVENTORY_FULL:
		// 아이템 추가 실패 - 인벤토리가 가득 참
		// 우편 받기 실패 - 인벤토리가 가득 참
		FText::FindText(TEXT("MailText"), TEXT("MailError_InventoryFull"), findText);
		break;
	case PD::Result::MAIL_ERROR_INVALID_MAIL_DB_ID:
		// 우편 - 유효하지 않은 MailDBId
		FText::FindText(TEXT("MailText"), TEXT("MailError_Invalid"), findText);
		break;
	case PD::Result::ITEM_ERROR_ADD_WEIGHT_FULL:
		// 소지 무게가 최대치입니다.
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Weight_Full"), findText);
		break;
	case PD::Result::MAIL_ERROR_ALREADY_OHTER_SERVER_RECV:
		// 계정당 1회 수령을 이미 한 상태에서 다시 수령 시
		FText::FindText(TEXT("MailText"), TEXT("MailAccept_Account_Already"), findText);
		break;
	default:
		// 알 수 없는 에러가 발생했습니다.
		FText::FindText(TEXT("MailText"), TEXT("MailError_Unknown"), findText);
#if WITH_EDITOR
		GSLOG(Log, TEXT("[Mail]ACK_MAIL_ERROR Failed, Result = %d"), (int)inResult);
		FGsUIHelper::PopupNetError(inResult);
#endif
		break;
	}

	if (!findText.IsEmpty())
	{
		FGsUIHelper::TrayMessageTicker(findText);
	}
}

// 서버요청에 대한 결과 티커
void FGsMailManager::MailReqMsg(EGMailReqType inReqType)
{
	FText findText;
	switch (inReqType)
	{
	case EGMailReqType::None:
		break;
	case EGMailReqType::ReceiveOne:
		// 우편에 담긴 보상을 받았습니다.
		FText::FindText(TEXT("MailText"), TEXT("MailAccept_One"), findText);
		break;
	case EGMailReqType::ReceiveAll:
		break;
	case EGMailReqType::DeleteOne:
		// 우편을 삭제했습니다.
		FText::FindText(TEXT("MailText"), TEXT("MailDelete_One"), findText);
		break;
	case EGMailReqType::DeleteAll:
		// 보상이 담긴 우편을 제외한 모든 우편을 삭제했습니다.
		FText::FindText(TEXT("MailText"), TEXT("MailDelete_All"), findText);
		break;
	default:
		break;
	}

	if (!findText.IsEmpty())
	{
		FGsUIHelper::TrayMessageTicker(findText);
	}
}

void FGsMailManager::GetMailString(FString inString, OUT TArray<TSharedPtr<FGsMailStringData>>& outMailStringDataList)
{
	FString rootPaths = FPaths::RootDir();
	FString engineFolder = TEXT("GitEngine");
	uint32 rootSize = rootPaths.GetCharArray().Num();
	uint32 engineFolderSize = engineFolder.GetCharArray().Num();

	rootPaths.RemoveAt(rootSize - engineFolderSize, engineFolderSize);
	const FString file = rootPaths / TEXT("RAON") / TEXT("Release") / TEXT("Server") / TEXT("Config") / TEXT("MailStingList.json");

	FString fileData = "";
	FFileHelper::LoadFileToString(fileData, *file);

	TSharedRef< TJsonReader<TCHAR> > JsonReader1 = TJsonReaderFactory<TCHAR>::Create(fileData);
	TSharedPtr<FJsonObject> jsonObj1 = MakeShareable(new FJsonObject());
	if (FJsonSerializer::Deserialize(JsonReader1, jsonObj1))
	{
		const TArray<TSharedPtr<FJsonValue>> mailStringList = jsonObj1->GetArrayField(TEXT("mailStringList"));
		for (int32 i = 0; i < mailStringList.Num(); ++i)
		{
			TSharedPtr<FJsonObject> mailStringSet = mailStringList[i]->AsObject();
			if (mailStringSet)
			{
				FString type = mailStringSet->GetStringField(TEXT("type"));
				FString title = mailStringSet->GetStringField(TEXT("title"));
				FString content = mailStringSet->GetStringField(TEXT("content"));

				outMailStringDataList.Emplace(
					MakeShareable(new FGsMailStringData(type, title, content)));
			}
		}
	}

	TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(inString);
	TSharedPtr<FJsonObject> jsonObj = MakeShareable(new FJsonObject());
	if (FJsonSerializer::Deserialize(JsonReader, jsonObj))
	{
		const TArray<TSharedPtr<FJsonValue>> mailStringList = jsonObj->GetArrayField(TEXT("mailStringList"));
		for (int32 i = 0; i < mailStringList.Num(); ++i)
		{
			TSharedPtr<FJsonObject> mailStringSet = mailStringList[i]->AsObject();
			if (mailStringSet)
			{
				FString type = mailStringSet->GetStringField(TEXT("type"));
				FString title = mailStringSet->GetStringField(TEXT("title"));
				FString content = mailStringSet->GetStringField(TEXT("content"));

				outMailStringDataList.Emplace(
					MakeShareable(new FGsMailStringData(type, title, content)));
			}
		}
	}
}