#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/GsBMShopProductData.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"
#include "DataSchema/GsSchemaEnums.h"
#include "T1Project.h"
#include "Item/GsItem.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsBMShopManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "GameObject/Define/EBMShopProductTagType.h"
#include "GameObject/Define/EBMShopHighlightType.h"


FGsBMShopProductData::FGsBMShopProductData(const int32 InProductID, const IAPV4Product* InHiveProductInfo)
	: _productID(InProductID)
{
	UpdateTableData(_productID);

	UpdateHiveProductInfo(InHiveProductInfo);

	_isRemoveDisplay = false;

	PrintHiveProductInfo();
}

FGsBMShopProductData::~FGsBMShopProductData()
{
	_tableData = nullptr;

	_hiveProductInfo.Reset(nullptr);

	if (nullptr != _startSaleTime)
	{
		delete _startSaleTime;
		_startSaleTime = nullptr;
	}

	if (nullptr != _endSaleTime)
	{
		delete _endSaleTime;
		_endSaleTime = nullptr;
	}
}

void FGsBMShopProductData::UpdateHiveProductInfo(const IAPV4Product* InHiveProductInfo)
{
	_hiveProductInfo.Reset(nullptr);

	if (nullptr != InHiveProductInfo)
	{
		_hiveProductInfo = MakeUnique<HiveProductInfo>(*InHiveProductInfo);
	}

	GSLOG(Log, TEXT("FGsBMShopProductData::UpdateHiveProductInfo()"));
	PrintHiveProductInfo();
}

void FGsBMShopProductData::InitCostInfo(const FGsSchemaBMShopProductData* InTableData)
{
	if (nullptr == InTableData)
		return;

	if (nullptr != _costInfo)
	{
		delete _costInfo;
		_costInfo = nullptr;
	}

	_costInfo = new FGsBMShopCostInfo(InTableData->purchaseCostType, InTableData->purchaseCurrencyType, InTableData->purchaseCurrencyItemId,
		InTableData->purchaseCurrencyPrice, CostType::ITEM == InTableData->purchaseCostType);
}

void FGsBMShopProductData::UpdateTableData(const int32 InProductID)
{
	const FGsSchemaBMShopProductData* findTableData = FindTableData();
	if (nullptr == findTableData)
	{
		GSLOG(Warning, TEXT("[FGsBMShopProductData::UpdateTableData] tableData is nullptr !! - InProductID : %d"), InProductID);
		return;
	}
	_tableData = findTableData;

	InitCostInfo(findTableData);

	_itemId = _tableData->itemId;
	_rebateCurrencyType = _tableData->rebateCurrencyType;
	_rebateCurrencyPrice = _tableData->rebateCurrencyAmount;
	_pcLevelLimit = _tableData->pcLevelLimit;
	_purchaseQuantityLimit = _tableData->quantityLimit;
	_limitBuyerType = _tableData->bmShopLimitBuyerType;
	_resetTimeTicketType = _tableData->resetTimeTicketType;
	_rewardGroupIdList = _tableData->rewardIdList;
	_displaySaleEndCountdownTimeValue = _tableData->displaySaleEndCountdownTimeValue;
	_guildLevelLimit = _tableData->guildLevelLimit;
	_isGuildGiftProduct = _tableData->isGuildGiftProduct;

	FDateTime time;
	if (nullptr == _startSaleTime && true == FDateTime::Parse(_tableData->saleStartTime, time))
	{
		_startSaleTime = new FDateTime(time.GetTicks());
	}
	if (nullptr == _endSaleTime && true == FDateTime::Parse(_tableData->saleEndTime, time))
	{
		_endSaleTime = new FDateTime(time.GetTicks());
	}
}

void FGsBMShopProductData::UpdateChildProductData(FGsBMShopProductData* InParentData)
{
	//FGsBMShopProductData* aa = new FGsBMShopProductData(*InParentData);
	if (nullptr == InParentData)
		return;

	if (nullptr != _costInfo)
	{
		delete _costInfo;
		_costInfo = nullptr;
	}

	FGsBMShopCostInfo* costInfo = InParentData->GetProductCostInfo();
	if (nullptr != costInfo)
	{
		_costInfo = new FGsBMShopCostInfo(costInfo->GetCostType(), costInfo->GetCurrencyType(), costInfo->GetCurrencyItemTid(),
			costInfo->GetAmount(), costInfo->IsItemCurrencyType());
	}

	_hiveProductInfo.Reset(nullptr);
	const HiveProductInfo* ParentHiveProductInfo = InParentData->GetHiveProduct();
	if (nullptr != ParentHiveProductInfo)
	{
		_hiveProductInfo = MakeUnique<HiveProductInfo>(*ParentHiveProductInfo);
	}

	_purchasedQuantiy = InParentData->GetPurchaseQuantiy();
	_itemId = InParentData->GetItemTID();
	_rebateCurrencyType = InParentData->GetRebateCurrencyType();
	_rebateCurrencyPrice = InParentData->GetRebateAmount();
	_pcLevelLimit = InParentData->GetLimitLevel();
	_purchaseQuantityLimit = InParentData->GetPurchaseQuantiyLimit();
	_limitBuyerType = InParentData->GetLimitBuyerType();
	_resetTimeTicketType = InParentData->GetResetTimeType();
	_rewardGroupIdList = InParentData->GetRewardIdList();
	_displaySaleEndCountdownTimeValue = InParentData->GetDisplaySaleEndCountdownTimeValue();
	_guildLevelLimit = InParentData->GetGuildGiftLimitLevel();
	_isGuildGiftProduct = InParentData->IsGuildGift();
	_iconPath = InParentData->GetIconPath();
	_isRemoveDisplay = InParentData->IsRemoveDisplay();

	InParentData->GetSaleStartTime(*_startSaleTime);
	InParentData->GetSaleEndTime(*_endSaleTime);
}

const FGsSchemaBMShopProductData* FGsBMShopProductData::FindTableData()
{
	const UGsTableBMShopProductData* table = Cast<UGsTableBMShopProductData>(FGsSchemaBMShopProductData::GetStaticTable());
	if (nullptr == table)
	{
		return nullptr;
	}

	TArray<const FGsSchemaBMShopProductData*> allrow;
	if (false == table->GetAllRows(allrow))
	{
		return nullptr;
	}

	for (const FGsSchemaBMShopProductData* iter : allrow)
	{
		if(nullptr == iter)
			continue;

		if (iter->id == _productID)
		{
			return iter;
		}
	}

	return nullptr;
}

const EBMShopProductTagType FGsBMShopProductData::GetTagType() const
{
	return _tableData->tagType;
}

const EBMShopHighlightType FGsBMShopProductData::GetHighLightType() const
{
	return _tableData->highlightType;
}

const FString FGsBMShopProductData::GetDisplayTime() const
{
	FString result;
	result.Empty();

	if (nullptr == _startSaleTime || nullptr == _endSaleTime)
	{
		return result;
	}

	// 판매 종료일보다 현재 시간이 더 흐른 상태
	if (true == IsExpireSale())
	{
		return result;
	}

	FGsBMShopManager* Mgr = GBMShop();
	FString resultStr;
	// TODO: 차후 국가별로 utc 시간 계산해서 출력해야함
	//FDateTime clientTime = FGsTimeSyncUtil::GetClientNowDateTime();
	FDateTime serverNowTime = FGsTimeSyncUtil::GetServerNowDateTimeUTC();
	if (*_startSaleTime > serverNowTime)
	{
		// 판매 시작까지 남은시간
		// __일 __시 __분 __초
		FTimespan timeSpan = *_startSaleTime - serverNowTime;
		return FGsBMShopManager::GetCalcRemainTime(timeSpan);

		/*
		GSLOG(Warning, TEXT("[GetDisplayTime] before sale - resultStr : %s"), *resultStr);
		*/
	}
	else
	{
		/*
		// 상시 판매 아이템은 종료일이 정해져 있지 않음
		if (true == _tableData->alwaysSoldFlag)
		{
			return result;
		}
		*/
		int32 countDownTime = _displaySaleEndCountdownTimeValue;
		if (0 >= countDownTime)
		{
			return result;
		}
		if (2999 == _endSaleTime->GetYear())
		{
			return result;
		}

		FDateTime countTimeData;
		if (false == FGsBMShopManager::GetMinHourDateTime(*_endSaleTime, countDownTime, countTimeData))
		{
			return result;
		}

		if (serverNowTime >= countTimeData)
		{
			// 남은시간 출력
			FTimespan spanTime = *_endSaleTime - serverNowTime;
			return FGsBMShopManager::GetCalcRemainTime(spanTime);
		}
		else
		{
			// 판매 종료 시간 출력
			// 판매 종료시간 자체를 디스플레이 하지 않았으면 한다고함
			// 일단 혹시몰라 주석처리함. 추후 필요하다고 할까봐

			//GSLOG(Warning, TEXT("[GetDisplayTime] sale End - GetDay : %d"), saleEndTime.GetDay());
		}
	}
	return result;
}

/*
const bool FGsBMShopProductData::IsDisplayRemainTimeAfterSale()
{
	// TODO: 차후 국가별로 utc 시간 계산해서 출력해야함
	FDateTime clientDataTime = FGsTimeSyncUtil::GetClientNowDateTime();

	// 상시 판매 아이템은 종료일이 정해져 있지 않음
	if (true == _tableData->alwaysSoldFlag)
	{
		return false;
	}

	int32 countDownTime = _tableData->displaySaleEndCountdownTimeValue;
	if (0 >= countDownTime)
	{
		return false;
	}

	FDateTime minTimeData = FDateTime(_endSaleTime->GetYear(), _endSaleTime->GetMonth(), _endSaleTime->GetDay(),
		_endSaleTime->GetHour() - countDownTime, _endSaleTime->GetMinute(), _endSaleTime->GetSecond());
	if (clientDataTime >= minTimeData)
	{
		// 남은시간 출력
		return true;
	}
	else
	{
		// 판매 종료 시간 출력
		// 판매 종료시간 자체를 디스플레이 하지 않았으면 한다고함
		// 일단 혹시몰라 주석처리함. 추후 필요하다고 할까봐
	
		GSLOG(Warning, TEXT("[GetDisplayTime] sale End - GetDay : %d"), saleEndTime.GetDay());
	
	}

	return false;
}
*/

const bool FGsBMShopProductData::IsExpireSale() const
{
	/*
	if (true == _tableData->alwaysSoldFlag)
	{
		return false;
	}
	*/
	if (nullptr == _endSaleTime)
	{
		// saleEndTime 데이터 입력 오류 시 강제로 true 반환함
		return true;
	}
	// TODO: 차후 국가별로 utc 시간 계산해서 출력해야함
	FDateTime serverDateTime = FGsTimeSyncUtil::GetServerNowDateTimeUTC();
	if (*_endSaleTime < serverDateTime) // 판매 종료일보다 현재 시간이 더 흐른 상태
	{
		return true;
	}

	return false;
}

const bool FGsBMShopProductData::IsSlodOut() const
{
	return ((0 < _purchaseQuantityLimit) && (_purchasedQuantiy >= _purchaseQuantityLimit));
	//return ((0 < _tableData->quantityLimit) && (_purchasedQuantiy >= _tableData->quantityLimit));
}
// 재구매 가능한지(이번, 다음번)
const bool FGsBMShopProductData::IsRetryBuyPossible() const
{
	// 제한이 없거나, 다음회차까지 가능하다면
	//return ((0 == _tableData->quantityLimit) || (_purchasedQuantiy +1 < _tableData->quantityLimit));
	return ((0 == _purchaseQuantityLimit) || (_purchasedQuantiy + 1 < _purchaseQuantityLimit));
}

const bool FGsBMShopProductData::IsEnoughLevelLimit() const
{
	// 레벨 제한 검사
	FGsGameDataManager* dataManager = GGameData();
	const FGsNetUserData* userData = dataManager->GetUserData();
	if (nullptr == userData)
		return false;

	return _pcLevelLimit <= userData->mLevel;
}

const bool FGsBMShopProductData::IsEnoughGuildGiftLevelLimit() const
{
	if (true == IsGuildGift() && 0 < GetGuildGiftLimitLevel())
	{
		FGsGuildManager* guildMgr = GGuild();
		if (false == guildMgr->IsGuild())
		{
			// 길드 미가입 상태
			return false;
		}

		if (guildMgr->GetGuildLevel() < GetGuildGiftLimitLevel())
		{
			// 길드 레벨 부족 상태
			return false;
		}
	}

	return true;
}

const bool FGsBMShopProductData::IsBeforSale() const
{	
	if (nullptr == _startSaleTime)
	{
		// saleStartTime 데이터 입력 오류 시 강제로 true 반환함
		return true;
	}

	FDateTime serverUTC = FGsTimeSyncUtil::GetServerNowDateTimeUTC();
	if (*_startSaleTime > serverUTC)
	{
		return true;
	}

	return false;
}

const bool FGsBMShopProductData::IsCompleteSteupCondition() const
{
	return _purchasedQuantiy >= _tableData->stepBuyLimitCount;
}

const bool FGsBMShopProductData::GetSaleStartTime(FDateTime& outData) const
{
	if (nullptr == _startSaleTime)
		return false;

	outData = *_startSaleTime;
	return true;
}

const bool FGsBMShopProductData::GetSaleEndTime(FDateTime& outData) const
{
	if (nullptr == _endSaleTime)
		return false;

	outData = *_endSaleTime;
	return true;
}

// 테이블에 utc0 시간을 클라이언트 타임 합산해서 출력
const bool FGsBMShopProductData::GetSaleDispalyStartTime(FDateTime& outData) const
{
	if (nullptr == _startSaleTime)
		return false;

	outData = FGsTimeSyncUtil::ConvertSeverDateTimeToClientDateTime(*_startSaleTime);
	return true;
}

// 테이블에 utc0 시간을 클라이언트 타임 합산해서 출력
const bool FGsBMShopProductData::GetSaleDispalyEndTime(FDateTime& outData) const
{
	if (nullptr == _endSaleTime)
		return false;

	outData = FGsTimeSyncUtil::ConvertSeverDateTimeToClientDateTime(*_endSaleTime);
	return true;
}

const FString FGsBMShopProductData::GetLimitSaleTimeText() const
{
	if (nullptr != _startSaleTime && nullptr != _endSaleTime)
	{
		FString resultString;
		FText findEndText;
		FText findMarkText;
		FText::FindText(TEXT("MarkFormatingText"), TEXT("SwungDash"), findMarkText);

		//if (2999 == _endSaleTime->GetYear() || 0 == _tableData->quantityLimit)
		if (2999 == _endSaleTime->GetYear())
		{
			// 별도 공지시 까지
			FText::FindText(TEXT("BMShopText"), TEXT("saleEndUndecided"), findEndText);
			//resultString = _startSaleTime->ToString() + " " + findMarkText.ToString()
				//+ " " + findEndText.ToString();
			resultString = findEndText.ToString();
		}
		else
		{
			FDateTime convertTime = FGsTimeSyncUtil::ConvertSeverDateTimeToClientDateTime(*_endSaleTime);
			
			FString time;
			FGsTimeStringHelper::GetTimeStringNYMDHMS(convertTime, time);
			FText::FindText(TEXT("BMShopText"), TEXT("purchaseLimitDate"), findEndText);
			resultString = findEndText.Format(findEndText, FText::FromString(time)).ToString();
		}

		return resultString;
	}

	return "";

	/*
	if (false == _tableData->alwaysSoldFlag)
	{
		if (nullptr != _startSaleTime && nullptr != _endSaleTime)
		{
			FString resultString;
			FText findEndText;
			FText findMarkText;
			FText::FindText(TEXT("MarkFormatingText"), TEXT("SwungDash"), findMarkText);

			//if (2999 == _endSaleTime->GetYear() || 0 == _tableData->quantityLimit)
			if (2999 == _endSaleTime->GetYear())
			{
				// 별도 공지시 까지
				FText::FindText(TEXT("BMShopText"), TEXT("saleEndUndecided"), findEndText);
				//resultString = _startSaleTime->ToString() + " " + findMarkText.ToString()
					//+ " " + findEndText.ToString();
				resultString = findEndText.ToString();
			}
			else
			{
				FText::FindText(TEXT("BMShopText"), TEXT("purchaseLimitDate"), findEndText);
				resultString = findEndText.Format(findEndText, FText::FromString(_endSaleTime->ToString())).ToString();
			}

			return resultString;
		}
	}

	return "";
	*/
}

void FGsBMShopProductData::GetAllActiveDisplayText(OUT TMap<FString, FString>& outDisplayStringMap) const
{
	// 순서 지켜야함
	// 판매기간
	outDisplayStringMap.Empty();

	/*
	if (false == _tableData->alwaysSoldFlag)
	{
		if (nullptr != _startSaleTime && nullptr != _endSaleTime)
		{
			FString resultString;
			FText findEndText;
			FText findMarkText;
			FText::FindText(TEXT("BMShopText"), TEXT("popupDescSaleTime"), findTitleText);
			FText::FindText(TEXT("MarkFormatingText"), TEXT("SwungDash"), findMarkText);

			//if (2999 == _endSaleTime->GetYear() || 0 == _tableData->quantityLimit)
			if (2999 == _endSaleTime->GetYear())
			{
				// 별도 공지시 까지
				FText::FindText(TEXT("BMShopText"), TEXT("saleEndUndecided"), findEndText);
				//resultString = _startSaleTime->ToString() + " " + findMarkText.ToString()
					//+ " " + findEndText.ToString();
				resultString = findEndText.ToString();
			}
			else
			{
				FText::FindText(TEXT("BMShopText"), TEXT("purchaseLimitDate"), findEndText);
				resultString = findEndText.Format(findEndText, FText::FromString(_endSaleTime->ToString())).ToString();
			}

			outDisplayStringMap.Emplace(findTitleText.ToString(), resultString);
		}
	}
	*/

	FText findTitleText;
	if (nullptr != _startSaleTime && nullptr != _endSaleTime)
	{
		FString resultString;
		FText findEndText;
		FText findMarkText;
		FText::FindText(TEXT("BMShopText"), TEXT("popupDescSaleTime"), findTitleText);
		FText::FindText(TEXT("MarkFormatingText"), TEXT("SwungDash"), findMarkText);

		//if (2999 == _endSaleTime->GetYear() || 0 == _tableData->quantityLimit)
		if (2999 == _endSaleTime->GetYear())
		{
			// 별도 공지시 까지
			FText::FindText(TEXT("BMShopText"), TEXT("saleEndUndecided"), findEndText);
			//resultString = _startSaleTime->ToString() + " " + findMarkText.ToString()
				//+ " " + findEndText.ToString();
			resultString = findEndText.ToString();
		}
		else
		{
			/*
			FGsTimeStringHelper::GetTimeStringNYMDHMS(*_endSaleTime, time);
			FText::FindText(TEXT("BMShopText"), TEXT("purchaseLimitDate"), findEndText);
			resultString = findEndText.Format(findEndText, FText::FromString(time)).ToString();
			*/

			FDateTime convertTime = FGsTimeSyncUtil::ConvertSeverDateTimeToClientDateTime(*_endSaleTime);

			FString time;
			FGsTimeStringHelper::GetTimeStringNYMDHMS(convertTime, time);
			FText::FindText(TEXT("BMShopText"), TEXT("purchaseLimitDate"), findEndText);
			resultString = findEndText.Format(findEndText, FText::FromString(time)).ToString();
		}
		outDisplayStringMap.Emplace(findTitleText.ToString(), resultString);
	}

	// 획득 아이템
	const FGsSchemaItemCommon* itemData = FGsItem::GetTableDataByID(GetItemTID());
	if (nullptr != itemData)
	{
		FText::FindText(TEXT("BMShopText"), TEXT("popupDescItemList"), findTitleText);
		outDisplayStringMap.Emplace(findTitleText.ToString(), itemData->name.ToString());
	}

	// 참고 및 주의사항
	if (false == _tableData->desc.IsEmpty())
	{
		FText::FindText(TEXT("BMShopText"), TEXT("popupDescCaution"), findTitleText);
		outDisplayStringMap.Emplace(findTitleText.ToString(), GetDesc().ToString());
	}
}

std::string FGsBMShopProductData::GetHiveMarketPID()
{
	return IsHiveProduct() ? _hiveProductInfo->_marketPID : "";
}

FString FGsBMShopProductData::GetConvertHiveMarketPID()
{
	if (GetHiveMarketPID().empty())
		return "";

	return FString(UTF8_TO_TCHAR(GetHiveMarketPID().c_str()));
}

FString FGsBMShopProductData::GetHiveCurrency() const
{
	return IsHiveProduct() ? FString(UTF8_TO_TCHAR(_hiveProductInfo->_currency.c_str())) : FString();
}

EGsBMShopCashCurrency FGsBMShopProductData::GetHiveCurrencyCode() const
{
	if (IsHiveProduct())
	{
		EGsBMShopCashCurrency CurrencyCode = EGsBMShopCashCurrency::XXX;
		const FString CurrencyCodeStr = GetHiveCurrency();
		if (!CurrencyCodeStr.IsEmpty())
		{
			LexFromString(CurrencyCode, *CurrencyCodeStr);
			return CurrencyCode;
		}
	}

	const UGsGlobalConstant* GlobalConstant = GData()->GetGlobalData();
	if (nullptr != GlobalConstant)
	{
		return GlobalConstant->_defaultCurrencyCode;
	}

	return EGsBMShopCashCurrency::XXX;
}

bool FGsBMShopProductData::IsHiveProduct() const
{
	return _hiveProductInfo.IsValid();
}

FString FGsBMShopProductData::GetConvertHiveTitle() const
{
	return IsHiveProduct() ? FString(UTF8_TO_TCHAR(_hiveProductInfo->_title.c_str())) : FString();
}

FString FGsBMShopProductData::GetConvertHiveDisplayPrice() const
{
	return IsHiveProduct() ? FString(UTF8_TO_TCHAR(_hiveProductInfo->_displayPrice.c_str())) : FString();
}


const bool FGsBMShopProductData::IsCouponProduct() const
{
	if (0 >= _tableData->discountCouponConditionGroupId.Num())
		return false;

	for (const int32 iter : _tableData->discountCouponConditionGroupId)
	{
		if (0 < iter)
		{
			return true;
		}
	}
	return false;
}

void FGsBMShopProductData::GetCouponGroupID(OUT TArray<int32>& outGroupIdList) const
{
	for (const int32 iter : _tableData->discountCouponConditionGroupId)
	{
		if (0 < iter)
		{
			outGroupIdList.Emplace(iter);
		}
	}
}

const bool FGsBMShopProductData::IsItemCurrencyType() const
{
	if (_costInfo)
	{
		return _costInfo->IsItemCurrencyType();
	}

	return false;
}

const CostType FGsBMShopProductData::GetPurchaseCostType() const
{
	if (_costInfo)
	{
		return _costInfo->GetCostType();
	}

	return CostType::MAX;
}

const uint32 FGsBMShopProductData::GetPurchaseCurrencyItemId() const 
{ 
	if (_costInfo)
	{
		return _costInfo->GetCurrencyItemTid();
	}

	return 0;
}

const CurrencyType FGsBMShopProductData::GetPurchaseCurrencyType() const 
{
	if (_costInfo)
	{
		return _costInfo->GetCurrencyType();
	}

	return CurrencyType::NONE;
}

const Currency FGsBMShopProductData::GetPurchasePrice() const 
{
	if (_costInfo)
	{
		return _costInfo->GetAmount();
	}

	return 0;
}

const bool FGsBMShopProductData::IsCashProductItem() const 
{
	if (_costInfo)
	{
		return CurrencyType::CASH == _costInfo->GetCurrencyType();
	}

	return false;
}

const FSoftObjectPath* FGsBMShopProductData::GetIconPath() const
{
	if (IsExistParentProductID())
	{
		if (nullptr != _iconPath)
		{
			return _iconPath;
		}
		else
		{
			if (_tableData)
			{
				return (_tableData->icon.IsValid()) ? &_tableData->icon : nullptr;
			}
		}
	}
	else
	{
		if (_tableData)
		{
			return (_tableData->icon.IsValid()) ? &_tableData->icon : nullptr;
		}
	}

	return nullptr;
}

const bool FGsBMShopProductData::IsCashRebate() const
{
	return CurrencyType::CASH_MILEAGE == _rebateCurrencyType;
}

uint32 FGsBMShopProductData::GetMaxCountSinglePurchase() const
{
	if (IsCashProductItem())
	{
		uint32 SequenceCashPurchaseHardcap = 0;

		if (UGsDataContainManager* dataManager = GData())
		{
			if (UGsGlobalConstant* globalData = dataManager->GetGlobalData())
			{
				SequenceCashPurchaseHardcap = globalData->_sequenceCashPurchaseHardcap;
			}
		}

		if (BMPurchaseLimitBuyerType::NONE == GetLimitBuyerType())
		{
			return SequenceCashPurchaseHardcap;
		}
		else
		{
			return GetPurchaseQuantiyLimit() - GetPurchaseQuantiy();
		}
	}
	else
	{
		return BMPurchaseLimitBuyerType::NONE == GetLimitBuyerType() ? 0 : GetPurchaseQuantiyLimit() - GetPurchaseQuantiy();
	}
}

FText FGsBMShopProductData::GetProductDisplayName() const
{
	return IsHiveProduct() ? FText::FromString(GetConvertHiveTitle()) : GetName();
}

FText FGsBMShopProductData::GetProductDisplayPrice() const
{
	return IsHiveProduct() ? FText::FromString(GetConvertHiveDisplayPrice()) : FText::AsNumber(GetPurchasePrice());
}

void FGsBMShopProductData::PrintHiveProductInfo() const
{
	FString logStr = FString::Printf(TEXT("_productID: %lu"), _productID);

	if (IsHiveProduct())
	{
		logStr.Appendf(TEXT(" _productTypeString: %s, _marketPID: %s, _currency: %s, _price: %f, _displayPrice: %s, _title: %s, _originalPrice: %f, _displayOriginalPrice: %s, _productDescription: %s, _iconURL: %s, _coinsReward: %d")
			, UTF8_TO_TCHAR(_hiveProductInfo->_productTypeString.c_str())
			, UTF8_TO_TCHAR(_hiveProductInfo->_marketPID.c_str())
			, UTF8_TO_TCHAR(_hiveProductInfo->_currency.c_str())
			, _hiveProductInfo->_price
			, UTF8_TO_TCHAR(_hiveProductInfo->_displayPrice.c_str())
			, UTF8_TO_TCHAR(_hiveProductInfo->_title.c_str())
			, _hiveProductInfo->_originalPrice
			, UTF8_TO_TCHAR(_hiveProductInfo->_displayOriginalPrice.c_str())
			, UTF8_TO_TCHAR(_hiveProductInfo->_productDescription.c_str())
			, (_hiveProductInfo->_iconURL.c_str())
			, _hiveProductInfo->_coinsReward
		);
	}

	GSLOG(Log, TEXT("FGsBMShopProductData::PrintHiveProductInfo() %s"), *logStr);
}

// 기록용 - 지우지마세요
// 추후 지우겠음
/*

		int32 year = 0;
		int32 month = 0;
		int32 day = 0;
		int32 hour = 0;
		hour = _endSaleTime->GetHour() - countDownTime;
		if(0 > hour)
		{
			int32 calcDay = countDownTime / 24;
			int32 calc = _endSaleTime->GetHour() - (countDownTime - (calcDay * 24));
			if (0 >= calc) // 24시간 기준 전날임
			{
				++calcDay;
				hour = 24 - abs(calc);
			}
			else
			{
				hour = calc;
			}

			int32 calcMonth = 0;
			day = _endSaleTime->GetDay() - calcDay;
			if (0 >= day)
			{
				int32 curMonthDays = FDateTime::DaysInMonth(_endSaleTime->GetYear(), _endSaleTime->GetMonth());
				int32 lastMonthDays = 0 >= _endSaleTime->GetMonth() - 1 ? FDateTime::DaysInMonth(_endSaleTime->GetYear() - 1, 12) :
					FDateTime::DaysInMonth(_endSaleTime->GetYear(), _endSaleTime->GetMonth() -1);

				day = lastMonthDays - day;
				if (0 >= day)
				{
					return result;
				}

				++calcMonth;
			}

			month = _endSaleTime->GetMonth() - calcMonth;
			if (0 >= month)
			{
				year = _endSaleTime->GetYear() - 1;
				month = 12;
			}
		}

		FDateTime countTimeData = FDateTime(0 == year ? _endSaleTime->GetYear() : year, 0 == month ? _endSaleTime->GetMonth() : month,
			0 == day ? _endSaleTime->GetDay() : day, hour, _endSaleTime->GetMinute(), _endSaleTime->GetSecond());

		GSLOG(Warning, TEXT("[GetDisplayTime] countDownTime :  %d"), countDownTime);
		GSLOG(Warning, TEXT("[GetDisplayTime] _endSaleTime.GetYear() :  %d"), _endSaleTime->GetYear());
		GSLOG(Warning, TEXT("[GetDisplayTime] _endSaleTime.GetMonth() :  %d"), _endSaleTime->GetMonth());
		GSLOG(Warning, TEXT("[GetDisplayTime] _endSaleTime.GetDay :  %d"), _endSaleTime->GetDay());
		GSLOG(Warning, TEXT("[GetDisplayTime] _endSaleTime.GetHour() :  %d"), _endSaleTime->GetHour());

		GSLOG(Warning, TEXT("----------------------------------------------------------------------------------------------"));

		GSLOG(Warning, TEXT("[GetDisplayTime] countTimeData.GetYear() :  %d"), countTimeData.GetYear());
		GSLOG(Warning, TEXT("[GetDisplayTime] countTimeData.GetMonth() :  %d"), countTimeData.GetMonth());
		GSLOG(Warning, TEXT("[GetDisplayTime] countTimeData.GetDay :  %d"), countTimeData.GetDay());
		GSLOG(Warning, TEXT("[GetDisplayTime] countTimeData.GetHour() :  %d"), countTimeData.GetHour());
*/