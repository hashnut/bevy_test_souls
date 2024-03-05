
#include "GsTitleEquip.h"
#include "GsTitleData.h"
#include "Management/GsMessageHolder.h"
#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "Title/GsTitlePlayerData.h"
#include "Title/GsTitleHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "T1Project/T1Project.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Preset/GsPresetHelper.h"

FGsTitleEquip::FGsTitleEquip() : FGsTitleCollectionData(PassivityOwnerType::TITLE_EQUIP)
{
	/*None*/
}


void FGsTitleEquip::Clear()
{
	_frontData = nullptr;
	_backData = nullptr;

	CleanUp();
}


void FGsTitleEquip::Equip(FGsTitleData* InData, bool InIsShowTicker /*= true*/)
{
	if (nullptr == InData)
	{
		GSLOG(Error, TEXT("nullptr == InData"));
		return;
	}

	if (TitleEquipSlot::FRONT == InData->GetEquipSlotType())
	{
		if (nullptr != _frontData)
		{
			UnEquip(_frontData);
		}		
	}
	else if (TitleEquipSlot::BACK == InData->GetEquipSlotType())
	{
		if (nullptr != _backData)
		{
			UnEquip(_backData);
		}
	}

	// 2022/01/13 PKT - Add Passivity
	FGsTitleCollectionData::Add(InData->GetPassivityId(), true);

	// 2022/01/13 PKT - 위치에 맞게 Set Data
	if (TitleEquipSlot::FRONT == InData->GetEquipSlotType())
	{
		_frontData = InData;
		_frontData->SetChangedEquipState(true);
	}
	else if (TitleEquipSlot::BACK == InData->GetEquipSlotType())
	{
		_backData = InData;
		_backData->SetChangedEquipState(true);
	}
	else
	{
		GSLOG(Error, TEXT("Invalid Equip Slot Type"));
		return;
	}

	if (InIsShowTicker)
	{
		FText message;
		FGsTitleHelper::FindText(TEXT("Notify_Equip_Title"), message);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, message);
	}	
}


void FGsTitleEquip::UnEquip(FGsTitleData* InData)
{
	if (nullptr == InData)
	{
		GSLOG(Error, TEXT("nullptr == InData"));
		return;
	}

	// 2022/01/21 PKT - 로그인 했을 시 장착 해지도 있기 때문에.. 기존 갖고 있던 Data에 대한 유무성 검사는 패스..
	if (true == InData->IsAcquire())
	{	// 2022/01/21 PKT - 소유 하고 있음에도 해지 된것은 유저가 스스로 해지 한 것
		FText message;
		FGsTitleHelper::FindText(TEXT("Notify_UnEquip_Title"), message);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, message);
	}
	else
	{
		FText message;
		FGsTitleHelper::FindText(TEXT("Notify_Lost_UnEquip_Title"), message);

		FString msg = message.ToString();
		switch (InData->GetGrade())
		{
		case TitleGrade::NORMAL:
			msg = msg.Replace(TEXT("<Grade>"), TEXT("<GRADE_NORMAL>"), ESearchCase::CaseSensitive);
			break;
		case TitleGrade::MAGIC:
			msg = msg.Replace(TEXT("<Grade>"), TEXT("<GRADE_MAGIC>"), ESearchCase::CaseSensitive);
			break;
		case TitleGrade::RARE:
			msg = msg.Replace(TEXT("<Grade>"), TEXT("<GRADE_RARE>"), ESearchCase::CaseSensitive);
			break;
		case TitleGrade::EPIC:
			msg = msg.Replace(TEXT("<Grade>"), TEXT("<GRADE_EPIC>"), ESearchCase::CaseSensitive);
			break;
		case TitleGrade::UNIQUE:
			msg = msg.Replace(TEXT("<Grade>"), TEXT("<GRADE_UNIQUE>"), ESearchCase::CaseSensitive);
			break;
		case TitleGrade::LEGEND:
			msg = msg.Replace(TEXT("<Grade>"), TEXT("<GRADE_LEGEND>"), ESearchCase::CaseSensitive);
			break;
		default:
			msg = msg.Replace(TEXT("<Grade>"), TEXT("<GRADE_NORMAL>"), ESearchCase::CaseSensitive);
			break;
		}

		message = FText::Format(FText::FromString(msg), { InData->GetTitle() });
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, message);
	}

	if (_frontData && _frontData->GetId() == InData->GetId())
	{
		FGsTitleCollectionData::Remove(_frontData->GetPassivityId(), true);

		FGsTitleData* preData = _frontData;
		_frontData = nullptr;

		preData->SetChangedEquipState(false);
	}

	if (_backData && _backData->GetId() == InData->GetId())
	{
		FGsTitleCollectionData::Remove(_backData->GetPassivityId(), true);

		FGsTitleData* preData = _backData;
		_backData = nullptr;

		preData->SetChangedEquipState(false);
	}
}


void FGsTitleEquip::UnEquip(TitleId InId)
{
	if (_frontData && _frontData->GetId() == InId)
	{
		UnEquip(_frontData);
	}

	if (_backData && _backData->GetId() == InId)
	{
		UnEquip(_backData);
	}
}


const FGsTitleData* FGsTitleEquip::GetData(TitleEquipSlot InSlot) const
{
	if (InSlot == TitleEquipSlot::FRONT)
	{
		return _frontData;
	}
	else if (InSlot == TitleEquipSlot::BACK)
	{
		return _backData;
	}

	return nullptr;
}