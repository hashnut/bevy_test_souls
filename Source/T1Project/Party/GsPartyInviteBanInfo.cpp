#include "GsPartyInviteBanInfo.h"

#include "Core/Public/Misc/DateTime.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

// 갱신
bool FGsPartyInviteBanInfo::Update()
{
	// 차단이면 15초 체크후 해제
	if (_isBan == true)
	{		
		int64 nowTick = FDateTime::UtcNow().GetTicks();
		int64 diffTick = nowTick - _banStartTime;

		static float maxBanTime = GData()->GetGlobalData()->_inviteBanTime * ETimespan::TicksPerSecond;

		if (diffTick >= maxBanTime)
		{
			// 밴 해제
			_isBan = false;
			// info 삭제 필요
			return false;
		}
	}
	// 차단이 아니면 저장된 초대 시간중 60초내에 3개이상 있으면 차단처리
	else
	{
		if (_arrInviteTime.Num() != 0)
		{
			TArray<int64> arrayRemoveTime;
			// 초대 시간 중 60초를 지난게 있으면 삭제
			static float checkTime =
				GData()->GetGlobalData()->_inviteLimitCheckTime * ETimespan::TicksPerSecond;

			for (int64 inviteTime : _arrInviteTime)
			{
				int64 nowTick = FDateTime::UtcNow().GetTicks();
				int64 diffTick = nowTick - inviteTime;

				// 60초 넘었으면
				if (diffTick >= checkTime)
				{
					arrayRemoveTime.Add(inviteTime);
				}
			}

			// 시간 지난거 삭제
			for (int64 delTime : arrayRemoveTime)
			{
				_arrInviteTime.Remove(delTime);
			}

			// 3개 이상 이면 밴 한다
			if (_arrInviteTime.Num() >= GData()->GetGlobalData()->_maxCountInvite)
			{
				_banStartTime = FDateTime::UtcNow().GetTicks();
				_isBan = true;

				// 밴이면 리스트 비운다
				_arrInviteTime.Empty();
			}
			// 하나도 없으면
			else if(_arrInviteTime.Num() == 0)
			{
				// info 삭제
				return false;
			}
		}
	}

	return true;
}

// 초대 시간 추가
void FGsPartyInviteBanInfo::AddInviteTime()
{
	int64 nowTick = FDateTime::UtcNow().GetTicks();
	_arrInviteTime.Add(nowTick);
}