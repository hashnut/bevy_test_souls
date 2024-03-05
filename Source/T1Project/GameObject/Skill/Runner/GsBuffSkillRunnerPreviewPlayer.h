#pragma once
#include "Templates/Function.h"
#include "GameObject/Skill/Data/GsSkillDataBase.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

/**
 * Preview Player 용 심플 Buff Runner. 
 * Local과 기능은 같이하지만, Remote상속 객체임에 따라 따고 버프러너를 구분한다.
 */
class UGsGameObjectSkillPreviewPlayer;

class T1PROJECT_API FGsBuffSkillRunnerPreviewPlayer final
{
private:
	const FGsSchemaSkillSet* _currData = nullptr;
	UGsGameObjectSkillPreviewPlayer* _previewObject = nullptr;

	TFunction<void(int)>		_callbackEndRun;

	float						_startTime = 0.f;
	float						_totalRuntime = 0.f;

public:
	FGsBuffSkillRunnerPreviewPlayer(UGsGameObjectSkillPreviewPlayer* previewObject, TFunction<void(int)> EndRun);
	~FGsBuffSkillRunnerPreviewPlayer() = default;

public:
	void StartRunner(const FGsSchemaSkillSet* Data, SkillLevel level);

	void CancelRunner();
	void EndRunner();
	bool Update(float DeltaTime);

protected:
	void ClearRunner();

public:
	bool IsValid() const;
	float GetElapsed() const;
};
