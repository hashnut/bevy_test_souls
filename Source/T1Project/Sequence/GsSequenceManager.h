#pragma once

#include "CoreMinimal.h"
#include "../Classes/GsManager.h"
#include "UObject/ScriptDelegates.h"
#include "Management/GsScopeHolder.h"
#include "GsSequencePlayer.h"
#include "GsSequenceManager.generated.h"


enum class EGsObjectHideFlags : int32
{
	HIDE_NONE			= 0x00000000,

	HIDE_LOCAL_PC		= 0x00000001,
	HIDE_PC				= 0x00000002,
	HIDE_NPC			= 0x00000004,
	HIDE_MONSTER		= 0x00000008,
	HIDE_SEQUENCE_PC	= 0x00000010,
	HIDE_SEQUENCE_OBJECT= 0x00000020,

	HIDE_ALL			= 0x0fffffff,
};

ENUM_CLASS_FLAGS(EGsObjectHideFlags);


class UGsSequencePlayer;
UCLASS()
class T1PROJECT_API UGsSequenceManager :
	public UObject,
	public IGsManager
{
	GENERATED_BODY()


protected:
	UPROPERTY()
	UGsSequencePlayer*	_sequencePlayer = nullptr;

	int32 _playingSequenceResId = 0;
	bool _isWatchMovieRewarded = false;

	MsgSystemHandleArray	_listSystemParamDelegate;
	
public:
	virtual void BeginDestroy() override;

public:
	//IGsManager
	virtual void Initialize() override;
	virtual void Finalize() override;

	// °»½Å
	virtual void Update(float In_delta) override;

public:
	UGsSequencePlayer* GetSequencePlayer();
	bool GetIsWatchMovieReward();
	int32 GetPlayingSequenceResId() { return _playingSequenceResId;  }

	void SetCinematicStart(int32 inSequenceResId, bool inIsAalreadyRewarded);
	void SetCinematicEnd(int32 inSequenceResId, bool inIsGetReward);

	void OpenPopupWatchMovieReward(TFunction<void(bool)> inCallback);

private:
	void ClearWatchMovieReward();
	void PlaySequence(const IGsMessageParam* inParam);
	void ShowRewardTicker(int32 inSequenceResId);
};

#define GSequence() UGsScopeHolder::GetGlobalManager<UGsSequenceManager>(UGsScopeGlobal::EManagerType::Sequence)
#define GSequencePlayer() (nullptr == GSequence())? nullptr : GSequence()->GetSequencePlayer()

