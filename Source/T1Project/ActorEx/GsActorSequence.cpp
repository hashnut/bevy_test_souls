#include "GsActorSequence.h"

#include "T1Project/Message/MessageParam/GsMessageParam.h"
#include "Management/GsMessageHolder.h"


// 초기화
void AGsActorSequence::BeginPlay()
{
	Super::BeginPlay();

	if (FGsMessageHolder* message = GMessage())
	{
		_listSystemDelegate.Emplace(
			message->GetSystemParam().AddUObject(MessageSystem::SEQUENCE_PLAYER_START, this, &AGsActorSequence::OnSequencePlayerStart));

		_listSystemDelegate.Emplace(
			message->GetSystemParam().AddUObject(MessageSystem::SEQUENCE_PLAYER_END, this, &AGsActorSequence::OnSequencePlayerEnd));
	}
}
// 종료 정리
void AGsActorSequence::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (nullptr == GScope())
	{
		return;
	}

	if (FGsMessageHolder* message = GMessage())
	{
		if (_listSystemDelegate.Num() != 0)
		{
			for (auto iter : _listSystemDelegate)
			{
				message->GetSystemParam().Remove(iter);
			}
			_listSystemDelegate.Empty();
		}
	}

	Super::EndPlay(EndPlayReason);
}

void AGsActorSequence::OnSequencePlayerStart(const IGsMessageParam* inParam)
{
	StartSequence();
}
void AGsActorSequence::OnSequencePlayerEnd(const IGsMessageParam* In_param)
{
	EndSequence();
}