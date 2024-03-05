#include "GsEventActionTutorial.h"
#include "Management/GsScopeHolder.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsTutorialMessageParam.h"
#include "DataSchema/Tutorial/GsSchemaTutorial.h"


void UGsEventActionTutorial::BeginDestroy()
{
	RemoveObserver();

	Super::BeginDestroy();
}

void UGsEventActionTutorial::Initialize(const FGsSchemaBase* EventActionData)
{
	const FGsSchemaTutorial* tutorialTable = StaticCast<const FGsSchemaTutorial*>(EventActionData);
	if (nullptr == tutorialTable)
	{
		GSLOG(Error, TEXT("Fail to cast EventActionData. nullptr == tutorialTable"));
		return;
	}

	_tutorialId = tutorialTable->id;
}

bool UGsEventActionTutorial::OnAfterPlay()
{
	AddObserver();

	if (0 >= _tutorialId)
	{
		// ID에 문제가 있으면 바로 종료시켜버림
		return false;
	}

	FGsTutorialMsgParamTutorialId param(_tutorialId);
	GMessage()->GetTutorial().SendMessage(MessageContentTutorial::EVENT_ACTION_TUTORIAL_START, &param);

	return true;
}

void UGsEventActionTutorial::OnFinish()
{
	RemoveObserver();

	_tutorialId = 0;
}

void UGsEventActionTutorial::AddObserver()
{
	if (_msgHandleTutorial.Value.IsValid())
	{
		GSLOG(Error, TEXT("_msgHandleTutorial.Value.IsValid(), _tutorialId: %d"), _tutorialId);
	}

	if (FGsMessageHolder* messageMgr = GMessage())
	{
		_msgHandleTutorial = messageMgr->GetTutorial().AddUObject(MessageContentTutorial::COMPLETE_TUTORIAL,
			this, &UGsEventActionTutorial::OnCompleteTutorial);
	}
}

void UGsEventActionTutorial::RemoveObserver()
{
	if (false == _msgHandleTutorial.Value.IsValid())
	{
		return;
	}

	if (GScope())
	{
		if (FGsMessageHolder* messageMgr = GMessage())
		{
			messageMgr->GetTutorial().Remove(_msgHandleTutorial);
		}
	}
}

void UGsEventActionTutorial::OnCompleteTutorial(const IGsMessageParam* InParam)
{
	if (const FGsTutorialMsgParamTutorialId* param = InParam->Cast<const FGsTutorialMsgParamTutorialId>())
	{
		if (param->_data == _tutorialId)
		{
			Finish();
		}
	}
}
