#pragma once

/**
* file		GsTitlePlayerData.h
* @brief	FGsGameObjectDataPlayer 에 저장하는 플레이어의 Title(칭호) 정보
* @author	PKT
* @date		2022/02/03
**/
#include "CoreMinimal.h"
#include "Shared/Client/SharedEnums/SharedTitleEnum.h"


class T1PROJECT_API FGsTitlePlayerData final
{
public:
	// 2022/02/03 PKT - HUD 출력에 필요한 형식으로 Type 재정의
	using TitleSet = TPair<FText, FLinearColor>;

	static const TitleSet InitData;

private:
	// 2022/02/03 PKT - 앞에 붙는 칭호
	TitleSet _frontSet = InitData;
	// 2022/02/03 PKT - 뒤에 붙는 칭호
	TitleSet _backSet = InitData;
	
public:
	void CleanUp();

	void Set(const TitleSet& InFront, const TitleSet& InBack);

	void SetFront(const TitleSet& InFront);

	void SetBack(const TitleSet& InBack);

	const TitleSet& Get(TitleEquipSlot InEquipSlot) const;	
};
