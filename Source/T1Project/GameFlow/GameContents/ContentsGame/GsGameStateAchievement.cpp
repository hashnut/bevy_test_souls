#include "GsGameStateAchievement.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Data/GsDataContainManager.h"
#include "DataSchema/GsSchemaEnums.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/GsMessageHolder.h"

#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/MessageParam/GsAchievementMessageParam.h"

#include "UI/UIContent/Window/GsUIWindowAchievement.h"

FGsGameStateAchievement::FGsGameStateAchievement() : FGsContentsGameBase(FGsContentsMode::InGame::ContentsAchievement)
{

}

FGsGameStateAchievement::~FGsGameStateAchievement()
{

}

void FGsGameStateAchievement::Enter()
{
	FGsContentsGameBase::Enter();

	if (UGsUIManager* uiManager = GUI())
	{
		TWeakObjectPtr<UGsUIWidgetBase> window = uiManager->OpenAndGetWidget(TEXT("WindowAchievement"));
		if (window.IsValid())
		{
			_windowUI = Cast<UGsUIWindowAchievement>(window);

			if (_windowUI.IsValid())
			{
				UGsUIWindowAchievement::UIInitData initParam;
				_windowUI.Get()->InitializeData(&initParam);
			}
		}
	}
	RegisterMessages();
	SetNavigationBar(CurrencyType::NONE);
}

void FGsGameStateAchievement::Exit()
{
	UnregisterMessages();

	//if (_windowUI.IsValid())
	//{
	//	GUI()->Close(_windowUI.Get());
	//}

	//_windowUI = nullptr;

	FGsContentsGameBase::Exit();
}

void FGsGameStateAchievement::Update(float In_deltaTime)
{
	FGsContentsGameBase::Update(In_deltaTime);
}

void FGsGameStateAchievement::Init()
{
	FGsContentsGameBase::Init();
}

void FGsGameStateAchievement::Close()
{
	FGsContentsGameBase::Close();
}

void FGsGameStateAchievement::RegisterMessages()
{
	auto& achievementMsgManager = GMessage()->GetAchievement();

	//_msgAchievementHandleList.Emplace(achievementMsgManager.AddRaw(MessageContentAchievement::UPDATE_THEME_ITEM,
	//	this, &FGsGameStateAchievement::OnUpdateThemeItem));

	_msgAchievementHandleList.Emplace(achievementMsgManager.AddRaw(MessageContentAchievement::UPDATE_ACHIEVEMENT_ITEM,
		this, &FGsGameStateAchievement::OnUpdateAchievementItem));

	//_msgAchievementHandleList.Emplace(achievementMsgManager.AddRaw(MessageContentAchievement::UPDATE_THEME_PROGRESS,
	//	this, &FGsGameStateAchievement::OnUpdateThemeProgress));

	_msgAchievementHandleList.Emplace(achievementMsgManager.AddRaw(MessageContentAchievement::UPDATE_ACHIEVEMENT_PROGRESS,
		this, &FGsGameStateAchievement::OnUpdateAchievementProgress));

	//_msgAchievementHandleList.Emplace(achievementMsgManager.AddRaw(MessageContentAchievement::CHANGE_THEME_LEVEL,
	//	this, &FGsGameStateAchievement::OnUpdateThemeLevelChanged));

	_msgAchievementHandleList.Emplace(achievementMsgManager.AddRaw(MessageContentAchievement::CHANGE_ACHIEVEMENT_LEVEL,
		this, &FGsGameStateAchievement::OnUpdateAchievementLevelChanged));

	//_msgAchievementHandleList.Emplace(achievementMsgManager.AddRaw(MessageContentAchievement::ACK_RECEIVE_THEME_REWARD,
	//	this, &FGsGameStateAchievement::OnAckReceiveThemeReward));

	_msgAchievementHandleList.Emplace(achievementMsgManager.AddRaw(MessageContentAchievement::ACK_RECEIVE_ACHIVEMENT_REWARD,
		this, &FGsGameStateAchievement::OnAckReceiveAchievementReward));

	_msgAchievementHandleList.Emplace(achievementMsgManager.AddRaw(MessageContentAchievement::ACK_RECEIVE_REWARDALL,
		this, &FGsGameStateAchievement::OnAckReceiveAchievementRewardAll));

	_msgRedDotHandle = GMessage()->GetHudRedDot().AddRaw(MessageContentHUDRedDot::ACHEIEVEMENT,
		this, &FGsGameStateAchievement::OnUpdateRedDot);
}

void FGsGameStateAchievement::UnregisterMessages()
{
	auto& achievementMsgManager = GMessage()->GetAchievement();

	for (auto handler : _msgAchievementHandleList)
	{
		achievementMsgManager.Remove(handler);
	}

	GMessage()->GetHudRedDot().Remove(_msgRedDotHandle);
}

void FGsGameStateAchievement::OnUpdateAchievementItem(const IGsMessageParam* InParam)
{
	if (const auto param = InParam->Cast<const FGsAchievementMsgParamChangeAchievement>())
	{
		_windowUI.Get()->OnUpdateAchievementItem(param->_data);
	}
}

void FGsGameStateAchievement::OnUpdateAchievementProgress(const IGsMessageParam* InParam)
{
	if (const auto param = InParam->Cast<const FGsAchievementMsgParamUpdateAchievement>())
	{
		_windowUI.Get()->OnUpdateAchievementItem(param->_data);
	}
}


void FGsGameStateAchievement::OnUpdateAchievementLevelChanged(const IGsMessageParam* InParam)
{
	if (const auto param = InParam->Cast<const FGsAchievementMsgParamChangeAchievement>())
	{
		_windowUI.Get()->OnUpdateAchievementItem(param->_data);
	}
}


void FGsGameStateAchievement::OnAckReceiveAchievementReward(const IGsMessageParam* InParam)
{
	if (const auto param = InParam->Cast<const FGsAchievementMsgParamAckAchievementReward>())
	{
		_windowUI.Get()->OnAckReceiveAchievementReward(param->_id);
	}
}

void FGsGameStateAchievement::OnAckReceiveAchievementRewardAll(const IGsMessageParam* InParam)
{
	if (const auto param = InParam->Cast<const FGsAchievementMsgParamAckRewardAll>())
	{
		_windowUI.Get()->OnAckReceiveAchievementRewardAll(param->_achievementIds); 
	}
}

void FGsGameStateAchievement::OnUpdateRedDot(bool bInActive)
{
	if (_windowUI.IsValid())
	{
		_windowUI.Get()->OnUpdateRedDot();
	}
}