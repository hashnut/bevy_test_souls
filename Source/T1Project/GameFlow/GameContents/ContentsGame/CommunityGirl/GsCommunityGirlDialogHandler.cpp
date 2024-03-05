
#include "GsCommunityGirlDialogHandler.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

void FGsComunityGirlDialogHandler::InitDialogTable()
{
	static TMap<EGsGirlAnimState, EGsStateBase> stateChange = {
		{EGsGirlAnimState::Spawn, EGsStateBase::Spawn},
		{EGsGirlAnimState::Idle, EGsStateBase::Idle},
		{EGsGirlAnimState::IdleBreak, EGsStateBase::Freeze},
		{EGsGirlAnimState::SelectTab, EGsStateBase::Social},
		{EGsGirlAnimState::Touch, EGsStateBase::SocialAction},
	};

	if (_stateDialogs.Num() > 0) return;

	if (const UGsTable* table = FGsSchemaCommunityDialog::GetStaticTable())
	{
		TArray<const FGsSchemaCommunityDialog*> list;
		table->GetAllRows<FGsSchemaCommunityDialog>(list);
		for (const auto row : list)
		{
			auto key = stateChange[row->Condition];
			auto iter = _stateDialogs.Find(key);
			if (iter == nullptr)
			{
				DialogProbability digItem;
				iter = &(_stateDialogs.Add(key, digItem));
				iter->_totalProbability = 0.f;
			}

			iter->_totalProbability += row->Probability;
			iter->_dlgList.Add(row);
		}
	}
}

const FGsSchemaCommunityDialog* FGsComunityGirlDialogHandler::GetDialog(EGsStateBase state)
{
	const FGsSchemaCommunityDialog* dlg = nullptr;
	if (auto iter = _stateDialogs.Find(state))
	{
		auto randPer = (rand() % 100) / 100.f;
		if (randPer <= iter->_totalProbability)
		{
			float total = 0.f;
			for (auto e : iter->_dlgList)
			{
				if (total > randPer) break;

				dlg = e;
				total += e->Probability;
			}
		}
	}
	return dlg;
}
