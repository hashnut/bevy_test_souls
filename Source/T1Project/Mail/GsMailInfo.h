#pragma once
#include "CoreMinimal.h"
#include "UTIL/GsTableUtil.h"
#include "Mail/GsSchemaMailStringData.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UTIL/GsIntervalChecker.h"
#include "T1Project.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsLocalizationManager.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Mail/GsSchemaMailType.h"
#include "../Currency/GsCurrencyHelper.h"
#include "GsMailDefine.h"
#include "StringFormatArg.h"
#include "Mail/EGsMailStringParamterType.h"
#include "Culture.h"


struct FGsSchemaMailStringData;

/*
	우편 아이템 정보
*/
struct FGsMailAttachment
{
private:
	MailAttachmentType _type;
	MailAttachmentId _itemId = 0;
	MailAttachmentAmount _itemCount = 0;
	ItemDBId             _itemDbId = INVALID_ITEM_DB_ID;							// INVALID_ITEM_DB_ID(== 0)이 아닐 경우, 이미 생성되어 있는 아이템을 지급, t_item 의 c_storage_type 은 MAIL 일 것.
	Level                _itemLevel = 0;											// 아이템 레벨.
	EnchantShield        _iEnchantShield = UNCONFIRMED_ENCHANT_SHIELD;				// UNCONFIRMED_ENCHANT_SHIELD 일 경우 수령 시 랜덤 생성.
	EnchantBonusIndex    _iEnchantBonusIndex = UNCONFIRMED_ENCHANT_BONUS_INDEX;		// UNCONFIRMED_ITEM_LEVEL 일 경우 수령 시 랜덤 생성.


public:
	explicit FGsMailAttachment(MailAttachmentType InType, MailAttachmentId InItemId, MailAttachmentAmount InItemCount)
		:_type(InType),
		_itemId(InItemId),
		_itemCount(InItemCount)
	{
	}

	explicit FGsMailAttachment(MailAttachmentType InType, MailAttachmentId InItemId, MailAttachmentAmount InItemCount,
		ItemDBId InItemDbId, Level InItemLevel, EnchantShield InIEnchantShield, EnchantBonusIndex InIEnchantBonusIndex)
		:_type(InType),
		_itemId(InItemId),
		_itemCount(InItemCount),
		_itemDbId(InItemDbId),
		_itemLevel(InItemLevel),
		_iEnchantShield(InIEnchantShield),
		_iEnchantBonusIndex(InIEnchantBonusIndex)
	{
	}

public:
	// getter
	const MailAttachmentType GetType() const { return _type; }
	const MailAttachmentId GetId() const { return _itemId; }
	const MailAttachmentAmount GetAmount() const { return _itemCount; }
	const Level GetLevel() const { return _itemLevel; }
	const EnchantShield GetEnchantShield() const { return _iEnchantShield; }
	const EnchantBonusIndex GetEnchantBonusIndex() const { return _iEnchantBonusIndex; }

	void SetAmount(int inCount) { _itemCount += inCount; }
};

/*
	시스템 우편 스트링 정보
*/
struct FGsMailStringData
{
private:
	FString _type;
	FString _title;
	FString _content;

public:
	explicit FGsMailStringData(FString InType, FString InTitle, FString InContent)
		:_type(InType),
		_title(InTitle),
		_content(InContent)
	{
	}

public:
	// getter
	const FString GetType() const { return _type; }
	const FString GetTitle() const { return _title; }
	const FString GetContent() const { return _content; }
};

/*
	우편 정보
*/
class FGsMailInfo
{
private:
	// db 아이디
	MailDBId _mailDBId;
	// 메일박스 타입
	MailBoxType _mailBoxType = MailBoxType::USER;
	// 메일 타입 id 
	MailTypeId _mailTypeId = INVALID_MAIL_TYPE_ID;
	// 삭제 시간 정보
	FDateTime _expireTime;
	// 제목
	FText _textTitle = FText::GetEmpty();
	// 내용
	FText _textContent = FText::GetEmpty();
	// 읽었는가?
	bool _isRead = false;
	// 아이템 보상이 있는가?
	bool _isRewards = false;		
	// 아이템 정보	
	TArray<TSharedPtr<FGsMailAttachment>> _itemDataList;
	// 메일 가변파람 스트링
	FString _stringVarParamter;
	// 메일 카운트 포함 여부
	bool _isLimitedQuantity = true;
	// 직접 받은 메일 내용 및 제목	(운영에서 직접 입력 한 값)
	TArray<TSharedPtr<FGsMailStringData>> _mailStringDataList;
	// vid메일인가?
	bool _isVidMail = false;

public:
	FGsMailInfo() = default;
	 ~FGsMailInfo() = default;

	explicit FGsMailInfo(MailDBId InMailDBId, TArray<TSharedPtr<FGsMailAttachment>>& inMailAttachmentList, int InStringKey, FString InStringVarParamter)
		 :	_mailDBId(InMailDBId), _itemDataList(inMailAttachmentList)
	{
		_isRewards = (0 < inMailAttachmentList.Num()) ? true : false;
		SetStringData(InStringKey, InStringVarParamter);
	}
	explicit FGsMailInfo(MailDBId InMailDBId, MailBoxType InMailBoxType, MailTypeId InMailTypeId, const FDateTime& InExpireTime, bool InIsRead,
						 TArray<TSharedPtr<FGsMailAttachment>>& inMailAttachmentList, MailStringId InStringKey, 
						 FString InStringVarParamter, bool InIsLimitedQuantity, TArray<TSharedPtr<FGsMailStringData>>& InMailStringDataList)
		:	_mailDBId(InMailDBId), 
			_mailBoxType(InMailBoxType),
			_mailTypeId(InMailTypeId),
			_expireTime(InExpireTime),
			_isRead(InIsRead),
			_itemDataList(inMailAttachmentList),
			_isLimitedQuantity(InIsLimitedQuantity),
			_mailStringDataList(InMailStringDataList)
	{
		_isRewards = (0 < inMailAttachmentList.Num()) ? true : false;

		if (0 >= InMailStringDataList.Num())
		{
			SetStringData(InStringKey, InStringVarParamter);
		}

		_isVidMail = SetIsVidMail(InMailTypeId);
	}

 public:
	// getter
 	const MailDBId GetMailDBId() const { return _mailDBId; }
	const MailBoxType GetMailBoxType() const { return _mailBoxType; }	
	const MailTypeId GetMailTypeId() const { return _mailTypeId; }
	//const FText& GetTextTitle() const { return _textTitle; }
	//const FText& GetTextContent() const { return _textContent; }
	const TArray<TSharedPtr<FGsMailAttachment>>& GetMailAttachmentList() const { return _itemDataList; }
	const bool GetIsRewards() const { return _isRewards; }
	const bool GetIsRead() const { return  _isRead; }
	const FTimespan GetRemainTime() const { return _expireTime - FGsTimeSyncUtil::GetServerNowDateTime(); }
	const bool GetIsRemainTime() const { return 0 < GetRemainTime().GetTotalSeconds(); }
	const bool GetIsLimitedQuantity() const { return  _isLimitedQuantity; }
	const bool GetIsVIDMail() const { return _isVidMail; }

public:
	TWeakPtr<FGsMailStringData> GetLanguageMailStringData()
	{
		FString languageCode = UGsLocalizationManager::GetCurrCultureName();
		if (false == _mailStringDataList.ContainsByPredicate([languageCode](auto e)
			{
				// 운영팀에서 zh-TW 등을 줄 경우, zh-Hant 못찾을 수 있어서 변환해서 비교함
				FString strCulture = UGsLocalizationManager::GetPrioritizedCultureName(e->GetType());
				return strCulture == languageCode;
			}))
		{
			languageCode = TEXT("en");
		}
		
		if (auto tab = _mailStringDataList.FindByPredicate([languageCode](auto e) { return e->GetType() == languageCode; }))
		{
			return (*tab);
		}

		return nullptr;
	}

	const FText& GetTextTitle()
	{ 
		if (0 < _mailStringDataList.Num())
		{
			TWeakPtr<FGsMailStringData> mailStringData = GetLanguageMailStringData();
			if (mailStringData.IsValid())
			{
				FString strTitle = mailStringData.Pin()->GetTitle();
				_textTitle =  FText::FromString(strTitle);
			}
			else
			{
				return _textTitle = FText::GetEmpty();
			}
		}
		return _textTitle; 
	}

	const FText& GetTextContent()
	{
		if (0 < _mailStringDataList.Num())
		{
 			TWeakPtr<FGsMailStringData> mailStringData = GetLanguageMailStringData();
			if (mailStringData.IsValid())
			{
				FString strContent = mailStringData.Pin()->GetContent();
				_textContent = FText::FromString(strContent);
			}
			else
			{
				return _textContent = FText::GetEmpty();
			}
		}
		return _textContent;
	}

public:
	void SetRead() { _isRead = true; }

private:	
	const FGsSchemaMailStringData* GetStringData(int inStringKey) const
	{
		return UGsTableUtil::FindRowById<UGsTableMailStringData, FGsSchemaMailStringData>(inStringKey);
	}
	void SetStringData(int InStringKey, FString InStringVarParamter)
	{
		if (const FGsSchemaMailStringData* stringData = GetStringData(InStringKey))
		{
			_textTitle = stringData->title;
			_textContent = stringData->content;

			if (0 < InStringVarParamter.Len())
			{
				// 서버에서 넘겨준 파라미터 값
				TArray<FString> arrayString = {};
				InStringVarParamter.ParseIntoArray(arrayString, TEXT(","));
				int stringNum = arrayString.Num();

				if (0 < stringNum)
				{
					int outIndex = 0;
					TArray<FStringFormatArg> outTitleStringList;
					if (0 < stringData->titleParamterType.Num())
					{
						GetStringParamterData(0, stringData->titleParamterType, arrayString, outTitleStringList, outIndex);

						FString outString = FString::Format(*_textTitle.ToString(), outTitleStringList);

						//_textTitle = FText::Join(_textTitle, outTitleStringList);
						_textTitle = FText::FromString(outString);
					}

					TArray<FStringFormatArg> outContentStringList;
					if (0 < stringData->contentParamterType.Num())
					{
						GetStringParamterData(outIndex, stringData->contentParamterType, arrayString, outContentStringList, outIndex);

						FString outString = FString::Format(*_textContent.ToString(), outContentStringList);
						_textContent = FText::FromString(outString);
						//_textContent = FText::Join(_textContent, outContentStringList);
					}
				}
			}
		}
		
	}

	void GetStringParamterData(int InIndex, const TArray<EGsMailStringParamterType>& InParamterDataTypeList, 
		const TArray<FString>& InServerParamterDataList, OUT TArray<FStringFormatArg>& OutStringList, OUT int& OutIndex)
	{
		int paramterTypeNum = InParamterDataTypeList.Num();
		int paramterDataNum = InServerParamterDataList.Num();

		int j = InIndex;
		for (int i = 0; i < paramterTypeNum; ++i, ++j)
		{
			if (j < paramterDataNum)
			{
				if (InParamterDataTypeList[i] == EGsMailStringParamterType::NONE)
				{
					OutStringList.Emplace(FStringFormatArg(InServerParamterDataList[j]));
				}
				else if (InParamterDataTypeList[i] == EGsMailStringParamterType::CURRENCY)
				{
					OutStringList.Emplace(FStringFormatArg(InServerParamterDataList[j]));
					FString valueType = InServerParamterDataList[++j];
					FText typeText =
						FGsCurrencyHelper::GetCurrencyLocalizedText(
							static_cast<CurrencyType>(FCString::Atoi(*valueType)));
					OutStringList.Emplace(FStringFormatArg(typeText.ToString()));
				}
				else if (InParamterDataTypeList[i] == EGsMailStringParamterType::ITEM)
				{
					FString value = InServerParamterDataList[j];
					uint64 valueInt64 = FCString::Strtoui64(*value, nullptr, 10);
					OutStringList.Emplace(FStringFormatArg(GetItemName(valueInt64).ToString()));
				}
				else if (InParamterDataTypeList[i] == EGsMailStringParamterType::CREATURE)
				{
					OutStringList.Emplace(FStringFormatArg(InServerParamterDataList[j]));
				}
				else if (InParamterDataTypeList[i] == EGsMailStringParamterType::MAP)
				{
					int mapId = FCString::Atoi(*InServerParamterDataList[j]);
					OutStringList.Emplace(FStringFormatArg(GetMapName(mapId).ToString()));
				}
			}			
		}

		OutIndex = j;
	}

	const FText& GetMapName(int InMapId)
	{
		if (const FGsSchemaMapData* mapData = UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(InMapId))
		{
			return mapData->MapStringId;
		}
		return FText::GetEmpty();
	}

	const FText& GetItemName(const uint32 InTID) const
	{
		const FGsSchemaItemCommon* SchemaItemCommon =
			UGsTableUtil::FindRowById<UGsTableItemCommon, FGsSchemaItemCommon>(InTID);
		if (nullptr != SchemaItemCommon)
		{
			return SchemaItemCommon->name;
		}

		return FText::GetEmpty();
	}

	TArray<FString> ParsingMessage(FString Message, const TCHAR* OP)
	{
		TArray<FString> ParsingMessageArray = {};
		Message.ParseIntoArray(ParsingMessageArray, OP);
		return ParsingMessageArray;
	}

	bool SetIsVidMail(MailTypeId inStringKey) const
	{
		const FGsSchemaMailType* mailType = UGsTableUtil::FindRowById<UGsTableMailType, FGsSchemaMailType>(inStringKey);
		if (nullptr != mailType)
		{
			MailCategoryMain mailCategoryMain = mailType->categoryMain.GetMailCategoryMain();
			return (MailCategoryMain::Vid == mailCategoryMain) ? true : false;
		}

		return false;
	}	
};

