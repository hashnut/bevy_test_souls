#pragma once
#include "CoreMinimal.h"
#include "GsContentsGameBase.h"

/*
	´ë·ú¸Ê ÄÁÅÙÃ÷ ÄÚµå
*/
class UGsUIWindowContinentalMap;

class FGsGameStateContinentalMap : public FGsContentsGameBase
{
	using Super = FGsContentsGameBase;
private:
	TWeakObjectPtr<UGsUIWindowContinentalMap> _windowUI;

public:
	FGsGameStateContinentalMap();

	// static func
public:
	static void MakeContinentalSpaceCrackListSortInfo(int In_continentalMapId);


	// °¡»ó ÇÔ¼ö
public:
	virtual void Enter() override;
	virtual void Exit() override;
	
};