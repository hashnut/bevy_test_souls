#include "GsLocalizationUtil.h"

#include "DataSchema/GsSchemaResetTimeTicketConfig.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "T1Project.h"
#include "UTIL/GsCronUtil.h"

FText FGsEnumLocalizedText::GetText(const ResetTimeTicket InValue)
{
	FText Text;
	const UGsTableResetTimeTicketConfig* ResetTimeTicketConfigTable = Cast<const UGsTableResetTimeTicketConfig>(FGsSchemaResetTimeTicketConfig::GetStaticTable());
	if (nullptr != ResetTimeTicketConfigTable)
	{
		const FGsSchemaResetTimeTicketConfig* ResetTimeTicketConfig = nullptr;
		if (ResetTimeTicketConfigTable->FindRowById(InValue, ResetTimeTicketConfig))
		{
			FString HumanReadableString;
			FGsCronUtil::GetCronString(ResetTimeTicketConfig->makeCron, HumanReadableString);
			Text = FText::FromString(HumanReadableString);
		}
	}

	//switch (InValue)
	//{
	//case ResetTimeTicket::RTT_DAILY: FText::FindText(TEXT("UICommonText"), TEXT("Daily"), Text); break;
	//case ResetTimeTicket::RTT_WEEKLY: FText::FindText(TEXT("UICommonText"), TEXT("Weekly"), Text); break;
	//case ResetTimeTicket::RTT_MONTHLY: FText::FindText(TEXT("UICommonText"), TEXT("Monthly"), Text); break;
	//case ResetTimeTicket::RTT_PASS_DAILY: FText::FindText(TEXT("BattlePassText"), TEXT("RTT_PASS_DAILY"), Text); break;
	//case ResetTimeTicket::RTT_PASS_WEEKLY: FText::FindText(TEXT("BattlePassText"), TEXT("RTT_PASS_WEEKLY"), Text); break;
	//default: GSLOG(Error, TEXT("unknown enum text:%d"), InValue); break;
	//}

	return Text;
}