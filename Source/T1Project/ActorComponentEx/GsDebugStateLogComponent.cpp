
#include "GsDebugStateLogComponent.h"

void UGsDebugStateLogComponent::AddLog(const FString& log)
{
	if (_listLog.Num() > _displayLogCount)
	{
		_listLog.RemoveAt(0);
	}
	_listLog.Emplace(log);
	UE_LOG(LogTemp, Warning, TEXT("[DebugGameObjectLog] [ %s ] %s"), *(FDateTime::Now().ToString()), *log);
};