
#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "DataTableConverter/GsDataTableConvertBase.h"

#include "GsDataTable.h"
#include "Helper/RSQLEditorUtil.h"

#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h" 

#include "CoreUObject/Public/UObject/UnrealType.h"
#include "Core/Public/HAL/FileManager.h"
#include "Core/Public/GenericPlatform/GenericPlatformFile.h"
#include "Shared/Client/SharedEnums/PD_ResultEnum.h"

#include "DataSchema/NeverCook/GsSchemaNetErrorDataExcel.h"
#include "DataSchema/Net/GsSchemaNetErrorUI.h"
#include "StringTableCore.h"

#define DEF_ERR_POPUP_MESSAGE(Message)\
FNotificationInfo Info(Message);\
Info.ExpireDuration = 10.0f;\
Info.bFireAndForget = true;\
Info.Image = FCoreStyle::Get().GetBrush(TEXT("MessageLog.Warning"));\
FSlateNotificationManager::Get().AddNotification(Info);\
UE_LOG(LogTemp, Error, TEXT("[NetErrorProcess Converter] %s"), *Message.ToString());\
return false;

const static FString rootDataTablePath = TEXT("/Game/DataCenter/DataTable");
const static FString netErrorExcelSheetPath = TEXT("NeverCook/DTGsNetErrorDataExcel.DTGsNetErrorDataExcel");
const static FString netErrorDataTablePath = TEXT("Net/DTGsNetErrorUI.DTGsNetErrorUI");

bool ConvertNetErrorProcessData(const UGsDataTable* excelSheetTable, UGsDataTable* dataTable)
{
	TMap<int32, FGsSchemaNetErrorUI> list;
	const TMap<FName, uint8*>& rowMap = excelSheetTable->GetRowMap();
	for (const auto& e : rowMap)
	{
		if (const auto excelData = reinterpret_cast<const FGsSchemaNetErrorDataExcel*>(e.Value))
		{
			FGsSchemaNetErrorUI data;
			{
				int32 id = data.result = static_cast<int32>(PD::ResultStringToEnum(excelData->result));

				FText msg = excelData->exText.IsEmpty() ? excelData->text.IsEmpty() ? FText::GetEmpty() : excelData->text : excelData->exText;

				if (msg.ToString() == FStringTableEntry::GetPlaceholderSourceString())
					msg = FText::GetEmpty();
				
				data.text = msg;
				data.RowName = excelData->RowName;
				data.isDefault =  excelData->isDefault;
				data.desc = excelData->result;

				if (id == 0)
				{
					FText failedMessage = FText::Format(NSLOCTEXT("FDataTableConverterNetErrorProcessExcelData", "InvalidResult", "{0} : No Exist Result Type{1}"),
						FText::FromName(excelData->RowName), FText::FromString(excelData->result));

					DEF_ERR_POPUP_MESSAGE(failedMessage)
				}

				id = id == 0 ? 1 : id * 100;
				while (id != 0 && list.Find(id) != nullptr) { id++; }

				data.id = id;
				data.uiType = excelData->uiType;
				data.popupKey = excelData->customUIKey;
				data.option = excelData->option;
			}

			list.Add(data.id, data);
		}
	}

	for (auto& e : list)
	{
		dataTable->AddRow(e.Value.RowName, e.Value);
	}

	return true;
}

IMPLEMENT_DATATABLE_CONVERTER(FDataTableConverterNetErrorProcessExcelData)
bool FDataTableConverterNetErrorProcessExcelData::Convert()
{
	UGsDataTable* dataTable;
	if (false == FGsDataTableConvertBase::LoadDataTable(rootDataTablePath / netErrorDataTablePath, dataTable))
	{
		FText message = FText::FromString(FString::Printf(TEXT("not found DT TableAsset : [%s]"), *netErrorDataTablePath));
		DEF_ERR_POPUP_MESSAGE(message);
	}

	UGsDataTable* excelSheetTable;
	if (false == FGsDataTableConvertBase::LoadDataTable(rootDataTablePath / netErrorExcelSheetPath, excelSheetTable))
	{
		FText message = FText::FromString(FString::Printf(TEXT("not found ExcelSheet TableAsset : [%s]"), *netErrorExcelSheetPath));
		DEF_ERR_POPUP_MESSAGE(message);
	}
	dataTable->EmptyTable();

	if (ConvertNetErrorProcessData(excelSheetTable, dataTable))
	{
		dataTable->SavePackage();
		return true;
	}

	return false;
}