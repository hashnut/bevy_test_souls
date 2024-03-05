#include "GsEventActionPlayMovie.h"

#include "GsSchemaBase.h"
#include "UI/UIContent/Tray/GsUITrayMovie.h"

void UGsEventActionPlayMovie::Initialize(const FGsSchemaBase* InEventActionData)
{
	Super::Initialize(InEventActionData);

	if (nullptr == InEventActionData)
	{
		GSLOG(Error, TEXT("nullptr == InEventActionData"));
	}
	else
	{
		_movieDataRowName = InEventActionData->GetRowName();
		GSLOG(Log, TEXT("_movieDataRowName: %s"), *_movieDataRowName.ToString());
	}
}

bool UGsEventActionPlayMovie::ShouldReplayOnReconnection() const
{
	return true;
}

bool UGsEventActionPlayMovie::OnPlay()
{
	bool Result = false;

	if (_movieDataRowName.IsNone())
	{
		GSLOG(Error, TEXT("_movieDataRowName.IsNone()"));
	}
	else
	{
		TWeakObjectPtr<UGsEventActionPlayMovie> This = this;
		TFunction<void()> MovieStopped = [This]() -> void
		{
			if (This.IsValid())
			{
				This->OnMovieStopped();
			}
		};

		Result = UGsUITrayMovie::PlayMovieCallbackFunc(_movieDataRowName.ToString(), MoveTemp(MovieStopped));
	}

	if (!Result)
	{
		GSLOG(Error, TEXT("UGsEventActionPlayMovie::OnPlay() return false!"));
	}

	return Result;
}

bool UGsEventActionPlayMovie::OnRewind()
{
	if (!IsPlaying())
	{
		// 플레이 중일때만 아래 작업 필요
		return false;
	}

	UGsUITrayMovie::StopMovieWithoutCallbackFunc();
	return true;
}

void UGsEventActionPlayMovie::OnMovieStopped()
{
	Finish();
}
