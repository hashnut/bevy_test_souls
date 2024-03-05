#include "GsArenaDefine.h"


void ArenaUserData::SetData(UserDBId InUserDBId, Level InLevel, FString InUserName, bool InIsVisible)
{
	_targetUserDBId = InUserDBId;
	_targetLevel = InLevel;
	_targetUserName = InUserName;
	_targetVisible = InIsVisible;
}

void ArenaUserData::Reset()
{
	_targetUserDBId = INVALID_USER_DB_ID;
	_targetLevel = INVALID_LEVEL;
	_targetUserName.Empty();
	_targetVisible = false;
}
