#pragma once

#include "GsTableManager.h"

#include "Management/GsScopeGlobal.h"
#include "Management/GsScopeHolder.h"
#include "GsManager.h"

#include "Message/GsMessageStage.h"

struct IGsMessageParam;

//-----------------------------------------------------------------------
// 데이터센터 매니저
//-----------------------------------------------------------------------

class T1PROJECT_API FGsDataCenterManager final : public IGsManager
{
private:
	UGsTableManager* _tableManager = nullptr;
	TArray<MsgStageHandle> _listStageHandleDelegate;

public:
	FGsDataCenterManager() = default;
	virtual ~FGsDataCenterManager() {}
	void Initialize() override;
	void Finalize() override;

public:
	const UGsTableManager* Get() const;

protected:
	void OnLobbyStateUpdateNotify(const IGsMessageParam* In_data);

#ifdef GS_PATCH_COMPLEATED_EDITOR_SIMULATION
private:
	MsgStageHandleArray _listStageDelegate;
	void PatchCompleteEditorSimulation();
#endif
};

#define GDataCenter() UGsScopeHolder::GetGlobalManagerFType<FGsDataCenterManager>(UGsScopeGlobal::EManagerType::DataCenter);
