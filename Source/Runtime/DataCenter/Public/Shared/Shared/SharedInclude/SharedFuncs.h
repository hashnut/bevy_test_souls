#pragma once


#include "SharedTypes.h"

#ifndef _SERVER
#include "../../Client/SharedEnums/SharedSkillEnum.h"
#include "../../Client/SharedEnums/SharedMarketEnum.h"
#include "../../Client/SharedEnums/SharedStatEnum.h"
#include "Internationalization/Regex.h"
#else
#include "../../Server/Enums/SharedSkillEnum.h"
#include "../../Server/Enums/SharedMarketEnum.h"
#include "../../Server/Enums/SharedStatEnum.h"

#include <string>
#include <regex>
#include <locale>
#include <codecvt>

#endif

#ifdef _SERVER_ENGINE
#pragma warning(push)
#pragma warning(disable: 4505)	// "사용하지 않는 함수가 존재한다"에 대한 워닝
#endif // #ifdef _SERVER_ENGINE

namespace SharedFunc
{
	static bool IsPlayer(const IffFactionActor& iffFactionActor)
	{
		return IffFactionType::PLAYER == iffFactionActor.mIffFactionType ||
			IffFactionType::PLAYER_INVADE_DEFENSE == iffFactionActor.mIffFactionType ||
			IffFactionType::PLAYER_INVADE_ATTACK == iffFactionActor.mIffFactionType ||
			IffFactionType::PLAYER_ARENA_TEAM_RED == iffFactionActor.mIffFactionType ||
			IffFactionType::PLAYER_ARENA_TEAM_BLUE == iffFactionActor.mIffFactionType ||
			IffFactionType::PLAYER_ALLY == iffFactionActor.mIffFactionType ||
			IffFactionType::PLAYER_ENEMY == iffFactionActor.mIffFactionType;
	}

	static bool IsMonster(const IffFactionActor& iffFactionActor)
	{
		return IffFactionType::MONSTER == iffFactionActor.mIffFactionType ||
			IffFactionType::MONSTER_SPACECRACK == iffFactionActor.mIffFactionType ||
			IffFactionType::MONSTER_TREASURE == iffFactionActor.mIffFactionType;
	}

	static bool IsNpc(const IffFactionActor& iffFactionActor)
	{
		return IffFactionType::NPC_ESCORT == iffFactionActor.mIffFactionType ||
			IffFactionType::NPC_GUARD == iffFactionActor.mIffFactionType ||
			IffFactionType::NPC_INVASIONGUARDIAN == iffFactionActor.mIffFactionType ||
			IffFactionType::NPC_WAGON == iffFactionActor.mIffFactionType ||
			IffFactionType::NPC_WAGONGUARD == iffFactionActor.mIffFactionType ||
			IffFactionType::NPC_WAGONPEDDLER == iffFactionActor.mIffFactionType ||
			IffFactionType::NPC_WAGONTHIEF == iffFactionActor.mIffFactionType ||
			IffFactionType::NPC_ARENA_TEAM_RED == iffFactionActor.mIffFactionType ||
			IffFactionType::NPC_ARENA_TEAM_BLUE == iffFactionActor.mIffFactionType;

	}



	static bool CheckDefault(const IffFactionActor&, const IffFactionActor& other, const IffFactionType checkIffFactionType)
	{
		return other.mIffFactionType == checkIffFactionType;
	}

	static bool CheckPlayer(const IffFactionActor&, const IffFactionActor& other, const IffFactionType)
	{
		return IsPlayer(other);
	}
	static bool CheckMonster(const IffFactionActor&, const IffFactionActor& other, const IffFactionType)
	{
		return IsMonster(other);
	}
	static bool CheckNpc(const IffFactionActor&, const IffFactionActor& other, const IffFactionType)
	{
		return IsNpc(other);
	}
	static bool CheckNpcArenaTeamRed(const IffFactionActor&, const IffFactionActor& other, const IffFactionType)
	{
		if (false == IsNpc(other))
			return false;

		return (IffFactionType::PLAYER_ARENA_TEAM_RED == other.mIffFactionType) || (IffFactionType::NPC_ARENA_TEAM_RED == other.mIffFactionType);
	}
	static bool CheckNpcArenaTeamBlue(const IffFactionActor&, const IffFactionActor& other, const IffFactionType)
	{
		if (false == IsNpc(other))
			return false;

		return (IffFactionType::PLAYER_ARENA_TEAM_BLUE == other.mIffFactionType) || (IffFactionType::NPC_ARENA_TEAM_BLUE == other.mIffFactionType);
	}

	static bool CheckPlayerInvadeAttack(const IffFactionActor&, const IffFactionActor& other, const IffFactionType)
	{
		if (false == IsPlayer(other))
			return false;

		return IffInvadeType::INVADE == other.mIffInvadeType;
	}
	static bool CheckPlayerInvadeDefence(const IffFactionActor&, const IffFactionActor& other, const IffFactionType)
	{
		if (false == IsPlayer(other))
			return false;

		return IffInvadeType::DEFENCE == other.mIffInvadeType;
	}
	static bool CheckPlayerArenaTeamRed(const IffFactionActor&, const IffFactionActor& other, const IffFactionType)
	{
		if (false == IsPlayer(other))
			return false;

		return (IffFactionType::PLAYER_ARENA_TEAM_RED == other.mIffFactionType) || (IffFactionType::NPC_ARENA_TEAM_RED == other.mIffFactionType);
	}
	static bool CheckPlayerArenaTeamBlue(const IffFactionActor&, const IffFactionActor& other, const IffFactionType)
	{
		if (false == IsPlayer(other))
			return false;

		return (IffFactionType::PLAYER_ARENA_TEAM_BLUE == other.mIffFactionType) || (IffFactionType::NPC_ARENA_TEAM_BLUE == other.mIffFactionType);
	}
	static bool CheckPlayerEnemy(const IffFactionActor& mine, const IffFactionActor& other, const IffFactionType)
	{
		if (false == IsPlayer(other))
			return false;

		bool isApprove = false;

		do 
		{
			// 대상이 카오라면
			if (PVPAlignmentGrade::CHAOTIC == other.mPVPAlignmentGrade)
			{
				isApprove = true;
				break;
			}

			// 대상이 경계 등록 대상이라면
			if (mine.mUserDBId > 0 && other.mUserDBId > 0)
			{
				for (int idx = 0; idx < MAX_COUNT_OF_DETECTION_TARGET_COUNT; ++idx)
				{
					if (mine.mMonitoringUserDBIdArray[idx] == other.mUserDBId)
					{
						isApprove = true;
						break;
					}
				}
			}
			if (true == isApprove)
				break;

			// 대상이 적대 기사단이라면
			if (mine.mGuildId > 0 && other.mGuildId > 0)
			{
				for (int idx = 0; idx < MAX_DIPLOMACY_GUILDS_COUNT; ++idx)
				{
					if (mine.mEnemyGuildArray[idx] == other.mGuildId)
					{
						isApprove = true;
						break;
					}
				}
			}
			if (true == isApprove)
				break;

			// 침공전 진행 중 인데, 서버가 다르다면
			if (IffInvadeType::NONE != mine.mIffInvadeType && IffInvadeType::NONE != other.mIffInvadeType)
			{
				if (mine.mPlanetWorldId != other.mPlanetWorldId)
				{
					isApprove = true;
					break;
				}
			}

			// 그림자 전장 참여 중인데, 팀이 다르다면
			if ((IffFactionType::PLAYER_ARENA_TEAM_RED == mine.mIffFactionType || IffFactionType::PLAYER_ARENA_TEAM_BLUE == mine.mIffFactionType) &&
				(IffFactionType::PLAYER_ARENA_TEAM_RED == other.mIffFactionType || IffFactionType::PLAYER_ARENA_TEAM_BLUE == other.mIffFactionType))
			{
				if (mine.mIffFactionType != other.mIffFactionType)
				{
					isApprove = true;
					break;
				}
			}

		} while (0);
		
		return isApprove;
	}
	static bool CheckPlayerAlly(const IffFactionActor& mine, const IffFactionActor& other, const IffFactionType checkIffFactionType)
	{
		if (false == IsPlayer(other))
			return false;

		// CheckPlayerEnemy 에 부합하지 않는 모든
		return !CheckPlayerEnemy(mine, other, checkIffFactionType);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static SkillCategorySet GetSkillCategorySet(SkillCategory category)
	{
		SkillCategorySet categorySet = SkillCategorySet::MAX;

		switch (category)
		{
		case SkillCategory::NORMAL:
		case SkillCategory::COMBO:
		//case SkillCategory::DASH_INVOCATION:
		case SkillCategory::CAST:
			categorySet = SkillCategorySet::ACTIVE;
			break;
		case SkillCategory::BUFF:
			categorySet = SkillCategorySet::BUFF;
			break;
		case SkillCategory::DASH:
			categorySet = SkillCategorySet::DASH;
			break;
		case SkillCategory::PASSIVE:
			categorySet = SkillCategorySet::PASSIVE;
			break;
		default:
			break;
		}

		return categorySet;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static uint64 CalcWeatherIdx(const uint64 weatherArraySize, time_t worldTime, int weatherMinuteUnit = 30)
	{
		// 날씨 변화 시간 단위로 나눈다
		const uint64 idx = worldTime / (weatherMinuteUnit * 60);

		// 배열 사이즈로 나눈 나머지를 반환한다
		return (0 == weatherArraySize) ? 0 : (idx % weatherArraySize);
	}

	struct IsAbleFunctionTable
	{
		bool (*IsAbleIffFactionTypeChecker[static_cast<uint8_t>(IffFactionType::MAX)])(
			const IffFactionActor& mine,
			const IffFactionActor& other,
			const IffFactionType checkFactionType
			) = { CheckDefault };


	explicit IsAbleFunctionTable()
		{
			IsAbleIffFactionTypeChecker[static_cast<uint8_t>(IffFactionType::NONE)] = CheckDefault;


			IsAbleIffFactionTypeChecker[static_cast<uint8_t>(IffFactionType::PLAYER)] = CheckPlayer;
			IsAbleIffFactionTypeChecker[static_cast<uint8_t>(IffFactionType::PLAYER_INVADE_DEFENSE)] = CheckPlayerInvadeDefence;
			IsAbleIffFactionTypeChecker[static_cast<uint8_t>(IffFactionType::PLAYER_INVADE_ATTACK)] = CheckPlayerInvadeAttack;
			IsAbleIffFactionTypeChecker[static_cast<uint8_t>(IffFactionType::PLAYER_ARENA_TEAM_RED)] = CheckPlayerArenaTeamRed;
			IsAbleIffFactionTypeChecker[static_cast<uint8_t>(IffFactionType::PLAYER_ARENA_TEAM_BLUE)] = CheckPlayerArenaTeamBlue;

			IsAbleIffFactionTypeChecker[static_cast<uint8_t>(IffFactionType::PLAYER_ALLY)] = CheckPlayerAlly;
			IsAbleIffFactionTypeChecker[static_cast<uint8_t>(IffFactionType::PLAYER_ENEMY)] = CheckPlayerEnemy;


			IsAbleIffFactionTypeChecker[static_cast<uint8_t>(IffFactionType::MONSTER)] = CheckDefault;
			IsAbleIffFactionTypeChecker[static_cast<uint8_t>(IffFactionType::MONSTER_SPACECRACK)] = CheckDefault;
			IsAbleIffFactionTypeChecker[static_cast<uint8_t>(IffFactionType::MONSTER_TREASURE)] = CheckDefault;


			IsAbleIffFactionTypeChecker[static_cast<uint8_t>(IffFactionType::NEUTRAL)] = CheckDefault;


			IsAbleIffFactionTypeChecker[static_cast<uint8_t>(IffFactionType::NPC_ESCORT)] = CheckDefault;
			IsAbleIffFactionTypeChecker[static_cast<uint8_t>(IffFactionType::NPC_GUARD)] = CheckDefault;
			IsAbleIffFactionTypeChecker[static_cast<uint8_t>(IffFactionType::NPC_INVASIONGUARDIAN)] = CheckDefault;
			IsAbleIffFactionTypeChecker[static_cast<uint8_t>(IffFactionType::NPC_WAGON)] = CheckDefault;
			IsAbleIffFactionTypeChecker[static_cast<uint8_t>(IffFactionType::NPC_WAGONGUARD)] = CheckDefault;

			IsAbleIffFactionTypeChecker[static_cast<uint8_t>(IffFactionType::NPC_WAGONPEDDLER)] = CheckDefault;
			IsAbleIffFactionTypeChecker[static_cast<uint8_t>(IffFactionType::NPC_WAGONTHIEF)] = CheckDefault;
			IsAbleIffFactionTypeChecker[static_cast<uint8_t>(IffFactionType::NPC_ARENA_TEAM_RED)] = CheckNpcArenaTeamRed;
			IsAbleIffFactionTypeChecker[static_cast<uint8_t>(IffFactionType::NPC_ARENA_TEAM_BLUE)] = CheckNpcArenaTeamBlue;

#ifdef _SERVER
			for (uint8_t iffFactionTypeValue = 0; iffFactionTypeValue < static_cast<uint8_t>(IffFactionType::MAX); ++iffFactionTypeValue)
			{
				if (nullptr == IsAbleIffFactionTypeChecker[iffFactionTypeValue])
					_ASSERT_DEBUG(0);
			}
#endif
		}
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// IFF 를 처리해주는 헬퍼 클래스
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct IffFactionHellper
	{
	public:
		IffFactionActor			mOwnerIffFaction;  // 내 자신 데이터 


	public:
		// 인터렉션을 할수 있는지 체크 
		// 현재는 기사단 건물에서 사용
		bool CanInteraction(const IffFactionActor& other)
		{
			// 같은 길드 인 경우 인터렉션이 가능함
			if (mOwnerIffFaction.mGuildId > 0 && other.mGuildId > 0)
			{
				if (mOwnerIffFaction.mGuildId == other.mGuildId)
					return true;
			}

			if (true == other.mIsAllianceNpc)
				return true;

			return false;
		}


	public:
		bool IsScanable(const IffFactionActor& other)
		{
			// TODO: 스캔을 위한 IFF Match 검사
			return IsAble(other, IffCheckType::SCAN);
		}

		bool IsTargetable(const IffFactionActor& other)
		{
			// TODO: 대상 선정을 위한 IFF Match 검사
			return IsAble(other, IffCheckType::TARGET);
		}

		bool IsAttackable(const IffFactionActor& other)
		{
			// TODO: 스킬 발동을 위한 IFF Match 검사
			return IsAble(other, IffCheckType::ATTACK);
		}

		bool IsExtensiveAttackable(const IffFactionActor& other)
		{
			// TODO: 스킬 효과(Skill Notify)의 추가 대상 선정을 위한 IFF Match 검사
			return IsAble(other, IffCheckType::EXTENSIVE);
		}

		// 가능한지 체크
		bool IsAble(const IffFactionActor& other, IffCheckType checkType = IffCheckType::ATTACK)
		{
			bool isAble = false;
			bool isPvp = false;

			if (true == IsPlayer(mOwnerIffFaction) && true == IsPlayer(other))
				isPvp = true;

			do
			{
				if (true == isPvp)
				{
					// PVP 일 경우만 검증할 구역

					// 대결(아레나) 중이라면 아레나의 룰에 따름
					if (true == IsInArena(other))
					{
						// 같은 팀이면 적대적으로 할 수 없다
						isAble = IsSameArenaTeam(other);
						break;
					}

					// PK(Player Kill) 할수 있는지 체크
					// 맵에서 PK 가능한지 체크하는 부분
					if (false == CanPK(other))
						break;
				}
				else
				{
					// PVP 가 아닌 모든 경우만 검증할 구역

					// 내가 솬한 보물 몬스터는 대상 설정할 수 없다
					if (true == IsTreasureMonster(other))
						break;

					//  [3/22/2023 sunho]
					// 길드 캠프 인경우에는  다른사라만 공격 가능함 
					if (true == IsGuildCamp(other))
						break;

					if (true == IsSanctumNexus(other))
						break;				
				}

				// 모든 경우에 대한 검증 구역

				// 비활성화 상태서는 설정할 수 없다
				if (false == IsEnable(other))
					break;
				
				// territoryType에 따라 설정 가능한지 체크하는 부분
				if (false == CanAttack(other))
					break;

				// IffFactionType인지 체크하여 대상 설정할 수 없으면 안된다
				if (false == IsAbleIffFactionType(other, checkType))
					break;

				// 대상을 설정할 수 있다
				isAble = true;

			} while (0);

			return isAble;
		}

		// 적대적인지 체크 
		bool IsEnemy(const IffFactionActor& other)
		{
			bool isEnemy = false;
			bool isPvp = false;

			if (true == IsPlayer(mOwnerIffFaction) && true == IsPlayer(other))
				isPvp = true;

			do
			{
				if (true == isPvp)
				{
					// PVP 일 경우만 검증할 구역

					// 대결(아레나) 중이라면 아레나의 룰에 따름
					if (true == IsInArena(other))
					{
						// 같은 팀이면 적대적으로 할 수 없다
						isEnemy = IsSameArenaTeam(other);
						break;
					}

					// PK(Player Kill) 할수 있는지 체크
					// 맵에서 PK 가능한지 체크하는 부분
					if (false == CanPK(other))
						break;

					// 적으로 판단할 조건이 아니라면 제외
					if (false == CheckPlayerEnemy(mOwnerIffFaction, other, IffFactionType::PLAYER_ENEMY))
						break;
				}
				else
				{
					// PVP 가 아닌 모든 경우만 검증할 구역

					// 내가 솬한 보물 몬스터는 적대적으로 할 수 없다
					if (true == IsTreasureMonster(other))
						break;

					if (true == IsSanctumNexus(other))
						break;

					//  [04/03/2023 sunho]
					// 길드 캠프 인경우에는  다른사라만 공격 가능함 
					if (true == IsGuildCamp(other))
						break;

					if (true == other.mIsAllianceNpc)
						break;
				}

				// 모든 경우에 대한 검증 구역

				// 비활성화 상태서는 공격할수 없다
				if (false == IsEnable(other))
					break;

				// 때릴 수 없으면 적대적으로 할 수 없다
				// territoryType에 따라 타격 가능한지 체크하는 부분
				if (false == CanAttack(other))
					break;

				// 같은 파티면 제외
				if (true == IsSameParty(other))
					break;

				// 같은 길드면 제외
				if (true == IsSameGuild(other))
					break;

				// 동맹 길드면 제외
				if (true == IsAllianceGuild(other))
					break;

				// 적군이 아니라면 패스
				//if (true == CheckPlayerAlly(mOwnerIffFaction, other, IffFactionType::PLAYER_ALLY))
				//	break;

				// 적대적인 IffFactionType인지 체크하여 적대 설정할 수 없으면 안된다
				//if (false == IsAbleIffFactionType(other, IffCheckType::ATTACK))
				//	break;

				// https://jira.com2us.com/jira/browse/C2URWQ-3857 해결을 위해 정윤일파트장님과 논의 후 몬스터와 몬스터의 IsEnemy, false 로 처리.
				if ((IffFactionType::MONSTER == mOwnerIffFaction.mIffFactionType) && (IffFactionType::MONSTER == other.mIffFactionType))
					break;

				// 대상을 적대 설정할 수 있다
				isEnemy = true;

			} while (0);

			return isEnemy;
		}


	public:
		bool IsInArena(const IffFactionActor& other)
		{
			if (INVALID_ARENA_ID < other.mArenaId || INVALID_ARENA_ID < mOwnerIffFaction.mArenaId)
			{
				return true;
			}
			else
				return false;
		}

		bool IsSameArenaTeam(const IffFactionActor& other)
		{
			// 같은 결투장인경우 같은팀인지 체크한다. 
			if (other.mArenaId == mOwnerIffFaction.mArenaId)
				return (other.mArenaTeam != mOwnerIffFaction.mArenaTeam);	// 다른팀일 경우 적대
			// 서로 결투중인 경우가 아니면 무조건 아군으로 취급한다.
			else
				return false;
		}

		bool IsEnable(const IffFactionActor& other)
		{
			// 비활성화 상태서는 공격할수 없다.
			if (false == other.mEnable || false == mOwnerIffFaction.mEnable)
				return false;

			return true;
		}

		// 같은 파티 인지 체크 
		// 같은 파티 인경우에는 공격 불가
		bool IsSameParty(const IffFactionActor& other)
		{
			if (mOwnerIffFaction.mPartyId > 0 && other.mPartyId > 0)
			{
				if (mOwnerIffFaction.mPartyId == other.mPartyId)
					return true;
			}

			return false;
		}

		// 길드 체크 
		bool IsSameGuild(const IffFactionActor& other)
		{
			if (mOwnerIffFaction.mGuildId > 0 && other.mGuildId > 0)
			{
				// 동일한 길드는 공격 불가
				if (mOwnerIffFaction.mGuildId == other.mGuildId)
					return true;
			}

			return false;
		}

		bool IsAllianceGuild(const IffFactionActor& other)
		{
			// 그림자 전장에서 상대 진영이라면 동맹 길드 체크 제외
			if ((IffFactionType::PLAYER_ARENA_TEAM_RED == mOwnerIffFaction.mIffFactionType && IffFactionType::PLAYER_ARENA_TEAM_BLUE == other.mIffFactionType) ||
				(IffFactionType::PLAYER_ARENA_TEAM_BLUE == mOwnerIffFaction.mIffFactionType && IffFactionType::PLAYER_ARENA_TEAM_RED == other.mIffFactionType))
			{
				return false;
			}

			if (mOwnerIffFaction.mGuildId > 0 && other.mGuildId > 0)
			{
				for (int idx = 0; idx < MAX_DIPLOMACY_GUILDS_COUNT; ++idx)
				{
					if (mOwnerIffFaction.mAllianceGuildArray[idx] == other.mGuildId)
						return true;
				}
			}

			return false;
		}

		// 동일 진영인지 체크
		bool SameIffFaction(const IffFactionActor& other)
		{
			if (mOwnerIffFaction.mIffFactionType == other.mIffFactionType)
				return true;

			return false;
		}

		bool IsTreasureMonster(const IffFactionActor& other)
		{
			if (mOwnerIffFaction.mTreasureMonsterGameId > 0)
			{
				if (mOwnerIffFaction.mTreasureMonsterGameId == other.mGameId)
					return true;
			}

			return false;
		}


		bool IsGuildCamp(const IffFactionActor& other)
		{
			if (mOwnerIffFaction.mGuildId > 0)
			{
				//if (true == other.mIsCampNpc)
				if(true == other.mIffSanctumType.st.isSanctumNexus)//camp도 같이 사용
				{
					if (mOwnerIffFaction.mGuildId == other.mGuildId)
						return true;
				}
			}

			return false;
		}

		bool IsSanctumNexus(const IffFactionActor& other)
		{			
			if (true == other.mIffSanctumType.st.isSanctumNexus)//넥서스일때..
			{
				if (false == other.mIffSanctumType.st.canBattlSanctum)//성소 점령전 시작상태가 아니라면.
					return true;//공격안함

				if (mOwnerIffFaction.mGuildId > 0)
				{
					if (mOwnerIffFaction.mGuildId == other.mGuildId)	//같은 기사단이라면..
						return true;//공격안함
				}				
			}
			return false;	//공격 함
		}

		bool CanAttack(const IffFactionActor& other)
		{

			// 대상과 나의 관계(PVP, PVE, EVE)에 따라 공격 가능한지 체크
			if (true == IsPlayer(mOwnerIffFaction) && true == IsPlayer(other))
			{
				if (false == CanPVP(other))
					return false;
			}
			else if ((true == IsPlayer(mOwnerIffFaction) && true != IsPlayer(other)) ||
				(true != IsPlayer(mOwnerIffFaction) && true == IsPlayer(other)))
			{
				if (false == CanPVE(other))
					return false;
			}
			else
			{
				if (false == CanEVE(other))
					return false;
			}

			return true;
		}
		
		bool CanPK(const IffFactionActor& other)
		{
			return mOwnerIffFaction.mCanPK && other.mCanPK;
		}

		bool IsAbleIffFactionType(const IffFactionActor& other, IffCheckType checkType)
		{
			bool result = false;

			static IsAbleFunctionTable mFunctionTable;

			do 
			{
				for (uint8_t iffFactionTypeValue = 0; iffFactionTypeValue < static_cast<uint8_t>(IffFactionType::MAX); ++iffFactionTypeValue)
				{
					if (true == mOwnerIffFaction.mEnemyFactionTypeArray[static_cast<uint8_t>(checkType)][iffFactionTypeValue])
					{
						bool check = mFunctionTable.IsAbleIffFactionTypeChecker[iffFactionTypeValue](
							mOwnerIffFaction,
							other,
							static_cast<IffFactionType>(iffFactionTypeValue)
							);

						// 하나라도 걸리면 적대적으로 설정 가능
						if (true == check)
						{
							result = true;
							break;
						}
					}
				}

				if (true == result)
					break;
				
			} while (0);

			return result;
		}

		bool IsPlanetWorldId(const IffFactionActor& other)
		{
			if (mOwnerIffFaction.mPlanetWorldId != other.mPlanetWorldId)
				return true;

			return false;
		}

		bool CompareFactionOR(IffFactionType lhs, IffFactionType rhs, IffFactionType compare)
		{
			return (lhs == compare) || (rhs == compare);
		}

		bool CanPVP(const IffFactionActor& other)
		{
			// 양측 다 할 수 있어야 한다
			return other.mCanPVP && mOwnerIffFaction.mCanPVP;
		}

		bool CanPVE(const IffFactionActor& other)
		{
			// 양측 다 할 수 있어야 한다
			return other.mCanPVE && mOwnerIffFaction.mCanPVE;
		}

		bool CanEVE(const IffFactionActor& other)
		{
			// 양측 다 할 수 있어야 한다
			return other.mCanEVE && mOwnerIffFaction.mCanEVE;
		}
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 내 이름 앞에 접두어 출력 여부 반환.
	//  - 다른 서버로 침공을 갔거나 인터서버로 간 경우, 내 이름 앞에도 Prefix를 출력하는 기획일지는 확인이 필요합니다.
	//  - 출력이 필요하다고 할 경우는 아래 함수를 사용하여판별합니다.
	//  - 1 번째 인자: SC_LOAD_TOPO 의 EnterWorldServerType
	//  - 2 번째 인자: SC_LOAD_TOPO 의 worldId
	//  - 3 번째 인자: SC_USER_BASIC_DATA 의 worldId
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static bool HasMyUserNamePrefix(
		EnterWorldServerType serverType, WorldId /*mapWorldId*/, WorldId /*myUserHomeWorldId*/
	)
	{
		// U1: 인터서버일 경우 내 캐릭터 무조건 접두어 출력(23.07.18)
		return (serverType == EnterWorldServerType::INTER_SERVER) ? true : false;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 다른 유저 이름 앞에 접두어 출력 여부 반환.
	//  - 1 번째 인자: SC_LOAD_TOPO 의 EnterWorldServerType
	//  - 2 번째 인자: SC_LOAD_TOPO 의 worldId
	//  - 3 번째 인자: SC_USER_BASIC_DATA 의 worldId
	//  - 4 번째 인자: SC_SPAWN_USER / SC_CHAT / GC_CHAT / SC_ACK_RESULT_DEFEAT_FILEDBOSS 의 homeWorldPrefix
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static bool HasOtherUserNamePrefix(
		EnterWorldServerType serverType, WorldId /*mapWorldId*/, WorldId myUserHomeWorldId,
		WorldId otherUserHomeWorldId
	)
	{
		// 나의 홈월드ID와 다르면 접두어 출력. 인터서버이면 남이면 무조건 접두어 출력(23.07.17)
		return (serverType == EnterWorldServerType::INTER_SERVER) || (myUserHomeWorldId != otherUserHomeWorldId) ? true : false;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 다른 유저 이름을 빨간색으로 출력할지 여부 반환.
	//  - 1 번째 인자: SC_LOAD_TOPO 의 EnterWorldServerType
	//  - 2 번째 인자: SC_LOAD_TOPO 의 worldId
	//  - 3 번째 인자: SC_USER_BASIC_DATA 의 worldId
	//  - 4 번째 인자: SC_SPAWN_USER / SC_CHAT / GC_CHAT / SC_ACK_RESULT_DEFEAT_FILEDBOSS 의 homeWorldPrefix
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static bool IsOtherUserNameRed(
		EnterWorldServerType serverType, WorldId /*mapWorldId*/, WorldId myUserHomeWorldId,
		WorldId otherUserHomeWorldId
	)
	{
		// 나의 홈월드ID와 다르면 빨간색.
		return serverType != EnterWorldServerType::INTER_SERVER && (myUserHomeWorldId != otherUserHomeWorldId) ? true : false;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// NPC 이름을 빨간색으로 출력할지 여부 반환.
	//  - 1 번째 인자: SC_LOAD_TOPO 의 EnterWorldServerType
	//  - 2 번째 인자: Npc의 NpcFunctionType
	//  - 3 번째 인자: SC_LOAD_TOPO 의 worldId
	//  - 4 번째 인자: SC_USER_BASIC_DATA 의 worldId
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static bool IsNpcNameRed(
		EnterWorldServerType serverType, NpcFunctionType npcFunctionType, WorldId mapWorldId,
		WorldId myUserHomeWorldId
	)
	{
		bool beUseNpc =
			(npcFunctionType == NpcFunctionType::COMMUNITY) ||
			(npcFunctionType == NpcFunctionType::SHOP) ||
			(npcFunctionType == NpcFunctionType::SHOP_SKILL_BOOK) ||
			(npcFunctionType == NpcFunctionType::REVIVAL) ||
			(npcFunctionType == NpcFunctionType::DEPOT) ||
			(npcFunctionType == NpcFunctionType::SHOP_EQUIP) ||
			(npcFunctionType == NpcFunctionType::EVENT_SHOP) ||
			(npcFunctionType == NpcFunctionType::QUEST_BOARD);

		// 맵의 월드ID와 나의 홈월드Id가 다르고 내가 침공중일때만 빨간색.
		return beUseNpc && serverType == EnterWorldServerType::INVADE_SERVER && (mapWorldId != myUserHomeWorldId) ? true : false;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static MarketCategory GetMarketCategory(ItemCategorySub itemCategorySub)
	{
		auto retMarketCategory = MarketCategory::MARKET_CATEGORY_ETC_ETC;

		if (false == IsValidItemCategorySub(itemCategorySub))
			return retMarketCategory;

		switch (itemCategorySub)
		{
			//WEAPON
		case ItemCategorySub::SWORD:
			retMarketCategory = MarketCategory::MARKET_CATEGORY_WEAPON_SWORD;
			break;
		case ItemCategorySub::DAGGER:
			retMarketCategory = MarketCategory::MARKET_CATEGORY_WEAPON_DAGGER;
			break;
		case ItemCategorySub::BOW:
			retMarketCategory = MarketCategory::MARKET_CATEGORY_WEAPON_BOW;
			break;
		case ItemCategorySub::WAND:
			retMarketCategory = MarketCategory::MARKET_CATEGORY_WEAPON_WAND;
			break;
		case ItemCategorySub::WAEPON_ORB:
			retMarketCategory = MarketCategory::MARKET_CATEGORY_WEAPON_ORB;
			break;
		case ItemCategorySub::SHOTGUN:
			retMarketCategory = MarketCategory::MARKET_CATEGORY_WEAPON_SHOTGUN;
			break;
			//WEAPON - END
			
			//ARMOR
		case ItemCategorySub::HELMET:
			retMarketCategory = MarketCategory::MARKET_CATEGORY_ARMOR_HELMET;
			break;
		case ItemCategorySub::BODY_ARMOR:
			retMarketCategory = MarketCategory::MARKET_CATEGORY_ARMOR_BODY_ARMOR;
			break;
		case ItemCategorySub::GLOVE:
			retMarketCategory = MarketCategory::MARKET_CATEGORY_ARMOR_GLOVE;
			break;
		case ItemCategorySub::SHOES:
			retMarketCategory = MarketCategory::MARKET_CATEGORY_ARMOR_SHOES;
			break;
		case ItemCategorySub::BELT:
			retMarketCategory = MarketCategory::MARKET_CATEGORY_ARMOR_BELT;
			break;
			//ARMOR - END
			
			//ACCESSORY
		case ItemCategorySub::EAR_RING:
			retMarketCategory = MarketCategory::MARKET_CATEGORY_ACCESSORY_EARRING;
			break;
		case ItemCategorySub::NECKLACE:
			retMarketCategory = MarketCategory::MARKET_CATEGORY_ACCESSORY_NECKLACE;
			break;
		case ItemCategorySub::SCARF:
			retMarketCategory = MarketCategory::MARKET_CATEGORY_ACCESSORY_SCARF;
			break;
		case ItemCategorySub::RING:
			retMarketCategory = MarketCategory::MARKET_CATEGORY_ACCESSORY_RING;
			break;
		case ItemCategorySub::CHARM:
			retMarketCategory = MarketCategory::MARKET_CATEGORY_ACCESSORY_CHARM;
			break;
		case ItemCategorySub::ORB:
		case ItemCategorySub::ORB_2:
			retMarketCategory = MarketCategory::MARKET_CATEGORY_ACCESSORY_ORB;
			break;
		case ItemCategorySub::BRACELET:
			retMarketCategory = MarketCategory::MARKET_CATEGORY_ACCESSORY_BRACELET;
			break;
			//ACCESSORY - END

			//SKILL_BOOK
		case ItemCategorySub::COMMON_SKILL_BOOK:
			retMarketCategory = MarketCategory::MARKET_CATEGORY_SKILL_BOOK_COMMON;
			break;
		case ItemCategorySub::SWORD_SKILL_BOOK:
			retMarketCategory = MarketCategory::MARKET_CATEGORY_SKILL_BOOK_SWORD;
			break;
		case ItemCategorySub::DAGGER_SKILL_BOOK:
			retMarketCategory = MarketCategory::MARKET_CATEGORY_SKILL_BOOK_DAGGER;
			break;
		case ItemCategorySub::BOW_SKILL_BOOK:
			retMarketCategory = MarketCategory::MARKET_CATEGORY_SKILL_BOOK_BOW;
			break;
		case ItemCategorySub::WAND_SKILL_BOOK:
			retMarketCategory = MarketCategory::MARKET_CATEGORY_SKILL_BOOK_WAND;
			break;
		case ItemCategorySub::ORB_SKILL_BOOK:
			retMarketCategory = MarketCategory::MARKET_CATEGORY_SKILL_BOOK_ORB;
			break;
		case ItemCategorySub::SHOTGUN_SKILL_BOOK:
			retMarketCategory = MarketCategory::MARKET_CATEGORY_SKILL_BOOK_SHOTGUN;
			break;
			//SKILL_BOOK - END

			//COMSUME_SCROLL
		case ItemCategorySub::WARP_TO_TOWN_SCROLL:
		case ItemCategorySub::WARP_TO_BOOKMARK_SCROLL:
		case ItemCategorySub::WARP_TO_RANDOM_SCROLL:
		case ItemCategorySub::SCROLL_BUFF:
		case ItemCategorySub::STATUE_BUFF:
		case ItemCategorySub::ENCHANT_WEAPON_SCROLL:
		case ItemCategorySub::BLESSED_ENCHANT_WEAPON:
		case ItemCategorySub::CURSED_ENCHANT_WEAPON:
		case ItemCategorySub::ENCHANT_ARMOR:
		case ItemCategorySub::BLESSED_ENCHANT_ARMOR:
		case ItemCategorySub::CURSED_ENCHANT_ARMOR:
		case ItemCategorySub::RESET_BONUS_STAT:
		case ItemCategorySub::ENCHANT_ACCESSORY:
		case ItemCategorySub::BLESSED_ENCHANT_ACCESSORY:
		case ItemCategorySub::CURSED_ENCHANT_ACCESSORY:
		case ItemCategorySub::OPTION_STONE:
		case ItemCategorySub::BLESSED_OPTION_STONE:
		case ItemCategorySub::WARP_TO_AGIT:
			retMarketCategory = MarketCategory::MARKET_CATEGORY_COMSUME_SCROLL;
			break;
		case ItemCategorySub::QUEST_REPEAT:
		case ItemCategorySub::FOOD_BUFF:
			retMarketCategory = MarketCategory::MARKET_CATEGORY_COMSUME_ETC;
			break;
			//COMSUME_SCROLL - END

			//MARKET_CATEGORY_ETC
		case ItemCategorySub::REWARD_BOX:
			retMarketCategory = MarketCategory::MARKET_CATEGORY_ETC_BOX;
			break;
		case ItemCategorySub::ENCHANT_SKILL_SCROLL:
		case ItemCategorySub::INGREDIENT_CRAFT:
		case ItemCategorySub::MATERIALS:
		case ItemCategorySub::ESSENCE:
		case ItemCategorySub::RECIPE:
		case ItemCategorySub::INK:
		case ItemCategorySub::MYSTERIOUS_POWDER:
			retMarketCategory = MarketCategory::MARKET_CATEGORY_ETC_INGREDIENT;
			break;
		case ItemCategorySub::DECOMPOSE_EQUIP_KIT:
		case ItemCategorySub::DECOMPOSE_SKILLBOOK_KIT:
		case ItemCategorySub::ARROW:
		case ItemCategorySub::SPIRIT_SHOT:
		case ItemCategorySub::MAGIC_STONE:
		case ItemCategorySub::SPELL_STONE:
		case ItemCategorySub::ARCAS:
		case ItemCategorySub::PVP_ALIGNMENT:
		case ItemCategorySub::NONE:
		case ItemCategorySub::SUMMON_FAIRY:
		case ItemCategorySub::ENCHANT_FAIRY:
		case ItemCategorySub::SUMMON_COSTUME:
			retMarketCategory = MarketCategory::MARKET_CATEGORY_ETC_ETC;
			break;
			//MARKET_CATEGORY_ETC - END

		default:
			retMarketCategory = MarketCategory::MARKET_CATEGORY_ETC_ETC;

		}

		return retMarketCategory;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static MarketPrice GetItemPricePerUnit(const MarketPrice itemPrice, const ItemAmount itemAmount)
	{
		//아이템 단가
		MarketPrice pricePerUnit = static_cast<MarketPrice>(itemPrice / itemAmount);

		//소수 4번째 자리를 구하기 위해 * 10000
		MarketPrice pricePerUnitX10000 = pricePerUnit * 10'000;

		//5번째 이하 소수점 제거
		MarketPrice priceNoDecimal = static_cast<MarketPrice>(static_cast<uint64_t>(pricePerUnitX10000));

		//5번째 이하에 소수점이 0.1보다 크다면 올림
		MarketPrice decimalLeft = pricePerUnitX10000 - priceNoDecimal;
		if (0.1 <= decimalLeft)
		{
			priceNoDecimal += 1;
		}

		//소수 올림이 끝났다면 곱해준 수를 다시 나눠 줌
		MarketPrice pricePerUnitCeiling = priceNoDecimal * 0.0001;

		//소수 5자리에서 올림(소수 4자리까지 표현)
		return pricePerUnitCeiling;
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static StatType DamageStatTypeToStatType(DamageStatType damageStatType)
	{
#ifdef _SERVER
		TRACE;
#endif

		switch (damageStatType)
		{
		case DamageStatType::HP: return StatType::HP;
		case DamageStatType::HP_SHIELD: return StatType::HP_SHIELD;
		case DamageStatType::MP: return StatType::MP;
		case DamageStatType::SENSITIVE: return StatType::SENSITIVE;
		default:
#ifdef _SERVER
			_ASSERT_CRASH_FMT(L"type switch Case 추가:{}", damageStatType);
#endif
			break;
		}

		return StatType::NONE;

#ifdef _SERVER
		TRACE_END;
#endif
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static int GetIgnoreCollisionRangeQuestNpc()
	{
		return 1000;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static bool StatTypeHidden(StatType statType)
	{
		// true 이면 데이터센터에서 Enum이 안보임(기존의 UMETA(Hidden))
		// false 이면 데이터센터에서 Enum이 보임

#ifdef _SERVER
		TRACE;
#endif
		const bool RESULT_HIDDEN = true;
		const bool RESULT_SHOW = false;

		switch (statType)
		{
		case StatType::NONE: 											return RESULT_SHOW;

			// STAT_ORDER_0 - 현재 상태 값
		case StatType::HP:												return RESULT_SHOW;
		case StatType::MP:												return RESULT_SHOW;
		case StatType::WEIGHT:											return RESULT_SHOW;
		case StatType::SENSITIVE:										return RESULT_SHOW;
		case StatType::COMBAT_POWER:										return RESULT_SHOW;

			// STAT_ORDER_1 - 기초 스탯
		case StatType::STR:												return RESULT_SHOW;
		case StatType::DEX:												return RESULT_SHOW;
		case StatType::INT:												return RESULT_SHOW;
		case StatType::AGI:												return RESULT_SHOW;
		case StatType::WIS:												return RESULT_SHOW;
		case StatType::CON:												return RESULT_SHOW;
		
			// STAT_ORDER_1 - 명중
		case StatType::HIT:												return RESULT_SHOW;
		case StatType::BONUS_HIT_MELEE:									return RESULT_HIDDEN;
		case StatType::BONUS_HIT_RANGE:									return RESULT_HIDDEN;
		case StatType::BONUS_HIT_MAGIC:									return RESULT_HIDDEN;
		case StatType::BONUS_HIT_PVP:									return RESULT_SHOW;
		case StatType::BONUS_HIT_TARGET_MELEE:							return RESULT_HIDDEN;
		case StatType::BONUS_HIT_TARGET_RANGE:							return RESULT_HIDDEN;
		case StatType::BONUS_HIT_TARGET_MAGIC:							return RESULT_HIDDEN;
		case StatType::BONUS_HIT_BOSS:									return RESULT_SHOW;
		case StatType::HIT_RATE_ABSOLUTE:								return RESULT_SHOW;
		case StatType::HIT_RATE_NORMAL_ABSOLUTE:						return RESULT_SHOW;
		case StatType::HIT_RATE_SKILL_ABSOLUTE:							return RESULT_SHOW;

			// STAT_ORDER_1 - 회피
		case StatType::BONUS_DODGE_MELEE:								return RESULT_HIDDEN;
		case StatType::BONUS_DODGE_RANGE:								return RESULT_HIDDEN;
		case StatType::BONUS_DODGE_MAGIC:								return RESULT_HIDDEN;
		case StatType::BONUS_DODGE_PVP:									return RESULT_SHOW;
		case StatType::BONUS_DODGE_BOSS:								return RESULT_SHOW;
		case StatType::DODGE_RATE_ABSOLUTE:								return RESULT_SHOW;
		case StatType::DODGE_RATE_NORMAL_ABSOLUTE:						return RESULT_SHOW;
		case StatType::DODGE_RATE_SKILL_ABSOLUTE:						return RESULT_SHOW;

			// STAT_ORDER_1 - 무기 공격력
		case StatType::WEAPON_ATTACK_MIN:								return RESULT_SHOW;
		case StatType::WEAPON_ATTACK_MAX:								return RESULT_SHOW;
		case StatType::WEAPON_ATTACK_AMPLIFIER_RATE:					return RESULT_SHOW;
		
			// STAT_ORDER_1 - 공격력
		case StatType::ATTACK:											return RESULT_SHOW;
		case StatType::BONUS_ATTACK_MELEE:								return RESULT_HIDDEN;
		case StatType::BONUS_ATTACK_RANGE:								return RESULT_HIDDEN;
		case StatType::BONUS_ATTACK_MAGIC:								return RESULT_HIDDEN;
		case StatType::BONUS_ATTACK_PVP:								return RESULT_SHOW;
		case StatType::BONUS_ATTACK_BOSS:								return RESULT_SHOW;
		case StatType::BONUS_ATTACK_TARGET_MELEE:						return RESULT_HIDDEN;
		case StatType::BONUS_ATTACK_TARGET_RANGE:						return RESULT_HIDDEN;
		case StatType::BONUS_ATTACK_TARGET_MAGIC:						return RESULT_HIDDEN;
		case StatType::ATTACK_AMPLIFIER_RATE:							return RESULT_SHOW;
		case StatType::ATTACK_AMPLIFIER_RATE_BOSS:						return RESULT_SHOW;
		case StatType::ATTACK_AMPLIFIER_RATE_NORMAL:					return RESULT_SHOW;
		case StatType::ATTACK_AMPLIFIER_RATE_PVP:						return RESULT_SHOW;
		case StatType::DEFENCE_IGNORE_DAMAGE:							return RESULT_HIDDEN;
		
			// STAT_ORDER_1 - 방어력
		case StatType::BONUS_DEFENCE:									return RESULT_HIDDEN;
		case StatType::BONUS_DEFENCE_MELEE:								return RESULT_HIDDEN;
		case StatType::BONUS_DEFENCE_RANGE:								return RESULT_HIDDEN;
		case StatType::BONUS_DEFENCE_MAGIC:								return RESULT_HIDDEN;
		case StatType::BONUS_DEFENCE_PVP:								return RESULT_HIDDEN;
		case StatType::BONUS_DEFENCE_BOSS:								return RESULT_HIDDEN;
		
			// STAT_ORDER_1 - HP, MP
		case StatType::HP_SHIELD:										return RESULT_SHOW;
		case StatType::HP_TICK_RECOVERY_ABSOLUTE:						return RESULT_SHOW;
		case StatType::MP_TICK_RECOVERY_ABSOLUTE:						return RESULT_SHOW;
		case StatType::HP_TICK_RECOVERY_IGNORE_RATE:					return RESULT_SHOW;
		case StatType::MP_TICK_RECOVERY_IGNORE_RATE:					return RESULT_SHOW;
		case StatType::HP_POTION_RECOVER_VARIATION_RATE:				return RESULT_SHOW;
		case StatType::HP_POTION_RECOVER_VARIATION_AMOUNT:				return RESULT_SHOW;
		case StatType::MAX_HP_VARIATION_AMOUNT:							return RESULT_SHOW;
		case StatType::MAX_MP_VARIATION_AMOUNT:							return RESULT_SHOW;
		case StatType::MAX_HP_VARIATION_RATE:							return RESULT_SHOW;
		case StatType::MAX_MP_VARIATION_RATE:							return RESULT_SHOW;
		
			// STAT_ORDER_1 - 스킬저항
		case StatType::BONUS_SKILL_RESIST:								return RESULT_SHOW;

			// STAT_ORDER_1 - 환경속성 패널티/저항
		case StatType::ENVIRONMENT_PENALTY_HIT_AMOUNT:					return RESULT_SHOW;
		case StatType::ENVIRONMENT_PENALTY_MOVE_RATE:					return RESULT_SHOW;
		case StatType::ENVIRONMENT_PENALTY_ATTACK_SPEED_RATE:			return RESULT_SHOW;
		case StatType::ENVIRONMENT_PENALTY_FRAME_RESIST_RATE:			return RESULT_SHOW;
		case StatType::ENVIRONMENT_PENALTY_FROST_RESIST_RATE:			return RESULT_SHOW;
		case StatType::ENVIRONMENT_PENALTY_DARK_RESIST_RATE:			return RESULT_SHOW;
		case StatType::ENVIRONMENT_PENALTY_HOLY_RESIST_RATE:			return RESULT_SHOW;
		case StatType::ENVIRONMENT_PENALTY_DEFENCE_AMOUNT:				return RESULT_SHOW;
		case StatType::ENVIRONMENT_PENALTY_HP_RECOVERY_RATE:			return RESULT_SHOW;
		case StatType::ENVIRONMENT_PENALTY_DODGE_AMOUNT:				return RESULT_SHOW;
		case StatType::ENVIRONMENT_PENALTY_RESIST_ALL_AMOUNT:			return RESULT_SHOW;
		
			// STAT_ORDER_1 - 치명타
		case StatType::CRITICAL_RATE:									return RESULT_SHOW;
		case StatType::BONUS_CRITICAL_RATE_MELEE:						return RESULT_HIDDEN;
		case StatType::BONUS_CRITICAL_RATE_RANGE:						return RESULT_HIDDEN;
		case StatType::BONUS_CRITICAL_RATE_MAGIC:						return RESULT_HIDDEN;
		case StatType::BONUS_CRITICAL_RATE_PVP:							return RESULT_SHOW;
		case StatType::BONUS_CRITICAL_RATE_BOSS:						return RESULT_SHOW;
		case StatType::BONUS_CRITICAL_RESIST_RATE:						return RESULT_SHOW;
		case StatType::BONUS_CRITICAL_RESIST_RATE_MELEE:				return RESULT_HIDDEN;
		case StatType::BONUS_CRITICAL_RESIST_RATE_RANGE:				return RESULT_HIDDEN;
		case StatType::BONUS_CRITICAL_RESIST_RATE_MAGIC:				return RESULT_HIDDEN;
		case StatType::BONUS_CRITICAL_RESIST_RATE_PVP:					return RESULT_SHOW;
		case StatType::BONUS_CRITICAL_RESIST_RATE_BOSS:					return RESULT_SHOW;
		case StatType::CRITICAL_DAMAGE_REDUCTION:						return RESULT_SHOW;
		case StatType::CRITICAL_DAMAGE_AMPLIFIER_RATE:					return RESULT_SHOW;
		case StatType::CRITICAL_DAMAGE_AMPLIFIER_RATE_MELEE:			return RESULT_SHOW;
		case StatType::CRITICAL_DAMAGE_AMPLIFIER_RATE_RANGE:			return RESULT_SHOW;
		case StatType::CRITICAL_DAMAGE_AMPLIFIER_RATE_MAGIC:			return RESULT_SHOW;
		case StatType::CRITICAL_DAMAGE_AMPLIFIER_RATE_PVP:				return RESULT_SHOW;
		case StatType::CRITICAL_DAMAGE_AMPLIFIER_RATE_BOSS:				return RESULT_SHOW;
		case StatType::CRITICAL_DAMAGE_AMPLIFIER_RESIST_RATE:			return RESULT_SHOW;			
		case StatType::CRITICAL_DAMAGE_AMPLIFIER_RESIST_RATE_MELEE:		return RESULT_SHOW;
		case StatType::CRITICAL_DAMAGE_AMPLIFIER_RESIST_RATE_RANGE:		return RESULT_SHOW;
		case StatType::CRITICAL_DAMAGE_AMPLIFIER_RESIST_RATE_MAGIC:		return RESULT_SHOW;
		case StatType::CRITICAL_DAMAGE_AMPLIFIER_RESIST_RATE_PVP:		return RESULT_SHOW;
		case StatType::CRITICAL_DAMAGE_AMPLIFIER_RESIST_RATE_BOSS:		return RESULT_SHOW;

			// STAT_ORDER_1 - 관통
		case StatType::PENETRATION:										return RESULT_HIDDEN;
		case StatType::BONUS_PENETRATION_MELEE:							return RESULT_HIDDEN;
		case StatType::BONUS_PENETRATION_RANGE:							return RESULT_HIDDEN;
		case StatType::BONUS_PENETRATION_MAGIC:							return RESULT_HIDDEN;
		case StatType::BONUS_PENETRATION_PVP:							return RESULT_HIDDEN;
		case StatType::BONUS_PENETRATION_BOSS:							return RESULT_HIDDEN;
		
			// STAT_ORDER_1 - 피해감소
		case StatType::BONUS_DAMAGE_REDUCTION:							return RESULT_HIDDEN;
		case StatType::BONUS_DAMAGE_REDUCTION_MELEE:					return RESULT_HIDDEN;
		case StatType::BONUS_DAMAGE_REDUCTION_RANGE:					return RESULT_HIDDEN;
		case StatType::BONUS_DAMAGE_REDUCTION_MAGIC:					return RESULT_HIDDEN;

			// STAT_ORDER_1 - 피해감소 무시
		case StatType::BONUS_DAMAGE_REDUCTION_IGNORE_MELEE:				return RESULT_SHOW;
		case StatType::BONUS_DAMAGE_REDUCTION_IGNORE_RANGE:				return RESULT_SHOW;
		case StatType::BONUS_DAMAGE_REDUCTION_IGNORE_MAGIC:				return RESULT_SHOW;

		case StatType::BONUS_DAMAGE_REDUCTION_PVP:						return RESULT_SHOW;		
		case StatType::BONUS_DAMAGE_REDUCTION_BOSS:						return RESULT_SHOW;
		case StatType::BONUS_DAMAGE_REDUCTION_RATE:						return RESULT_SHOW;
		case StatType::BONUS_DAMAGE_REDUCTION_RATE_MELEE:				return RESULT_SHOW;
		case StatType::BONUS_DAMAGE_REDUCTION_RATE_RANGE:				return RESULT_SHOW;
		case StatType::BONUS_DAMAGE_REDUCTION_RATE_MAGIC:				return RESULT_SHOW;
		case StatType::BONUS_DAMAGE_REDUCTION_RATE_NORMAL:				return RESULT_SHOW;
		case StatType::BONUS_DAMAGE_REDUCTION_RATE_SKILL:				return RESULT_SHOW;
		case StatType::BONUS_DAMAGE_REDUCTION_RATE_PVP:					return RESULT_SHOW;
		case StatType::BONUS_DAMAGE_REDUCTION_RATE_BOSS:				return RESULT_SHOW;

			// STAT_ORDER_1 - 속성 공격력
		case StatType::BASE_ELEMENT_DARK_ATTACK:						return RESULT_SHOW;
		case StatType::BASE_ELEMENT_HOLY_ATTACK:						return RESULT_SHOW;
		case StatType::BASE_ELEMENT_FLAME_ATTACK:						return RESULT_SHOW;
		case StatType::BASE_ELEMENT_FROST_ATTACK:						return RESULT_SHOW;
		
			// STAT_ORDER_1 - 속성 증폭/피해감소/저항
		case StatType::ELEMENT_ATTACK_AMPLIFIER:						return RESULT_SHOW;
		case StatType::ELEMENT_DAMAGE_REDUCTION:						return RESULT_SHOW;
		case StatType::ELEMENT_RESIST_RATE:								return RESULT_SHOW;

			// STAT_ORDER_1 - 스킬 공격력 증폭률
		case StatType::SKILL_ATTACK_AMPLIFIER_RATE:						return RESULT_SHOW;

			// STAT_ORDER_1 - 이상 상태 시간 감소률
		case StatType::ABNORMALITY_TIME_REDUCTION_RATE:					return RESULT_SHOW; // [R3 상태이상]
		case StatType::ABNORMALITY_TIME_INCREASE_RATE:					return RESULT_SHOW; // [R3 상태이상]
		
			// STAT_ORDER_1 - 각종 속도
		case StatType::CASTING_SPEED_RATE:								return RESULT_SHOW;
		
			// STAT_ORDER_1 - 유틸
		case StatType::EXP_GAIN_VARIATION_RATE:							return RESULT_SHOW;
		case StatType::EXP_GAIN_VARIATION_RATE_ARCAS:					return RESULT_HIDDEN;
		case StatType::GOLD_GAIN_VARIATION_RATE:						return RESULT_SHOW;
		case StatType::BONUS_ARCASPOINT_INCREASE_AMOUNT_RATE:			return RESULT_HIDDEN;
		case StatType::BONUS_ARCASPOINT_USAGE_DISCOUNT_RATE:			return RESULT_HIDDEN;
		case StatType::BONUS_BLESS_MAX_INCREASE_VALUE:					return RESULT_HIDDEN;
		
			// STAT_ORDER_1 - IMMUNE(이뮨, 면역)
		case StatType::IMMUNE_DAMAGE:									return RESULT_SHOW;
		case StatType::IMMUNE_DAMAGE_MELEE:								return RESULT_SHOW;
		case StatType::IMMUNE_DAMAGE_RANGE:								return RESULT_SHOW;
		case StatType::IMMUNE_DAMAGE_MAGIC:								return RESULT_SHOW;
		case StatType::IMMUNE_DAMAGE_SKILL:								return RESULT_SHOW;
		case StatType::IMMUNE_ABNORMALITY:								return RESULT_SHOW; // [R3 상태이상]
		case StatType::IMMUNE_ABNORMALITY_STUN:							return RESULT_SHOW; // [R3 상태이상]
		case StatType::IMMUNE_ABNORMALITY_BIND:							return RESULT_SHOW; // [R3 상태이상]
		case StatType::IMMUNE_ABNORMALITY_FEAR:							return RESULT_SHOW; // [R3 상태이상]
		case StatType::IMMUNE_ABNORMALITY_OTHER:						return RESULT_SHOW; // [R3 상태이상]
		case StatType::IMMUNE_ABNORMALITY_SILENCE:						return RESULT_SHOW;
		case StatType::IMMUNE_ABNORMALITY_TAUNT:						return RESULT_SHOW;
		case StatType::IMMUNE_ABNORMALITY_POLYMORPH:					return RESULT_SHOW;
		case StatType::IMMUNE_ABNORMALITY_BLOCKADE:						return RESULT_SHOW;
		case StatType::IMMUNE_ABNORMALITY_SLOW:							return RESULT_SHOW;
			

			// STAT_ORDER_1 - STIFF (경직)
		case StatType::STIFF_HIT_RATE:									return RESULT_SHOW;
		case StatType::STIFF_RESIST_RATE:								return RESULT_SHOW;
		case StatType::IMMUNE_STIFF:									return RESULT_SHOW;

			// STAT_ORDER_1 - ETC(기타 등등)
		case StatType::BONUS_APPROACH_RANGE:							return RESULT_SHOW;
		case StatType::BONUS_APPROACH_RANGE_RATE:						return RESULT_SHOW;
		case StatType::SKILL_COOLTIME_RATE:								return RESULT_SHOW;
		case StatType::MAX_SENSITIVE:									return RESULT_SHOW;
		
			// STAT_ORDER_1 - 천마석 사용일 때와 스킬일 때의 가산 스탯
		case StatType::SPIRIT_ATTACK:									return RESULT_SHOW;
		case StatType::SPIRIT_WEAPON_ATTACK_MAX:						return RESULT_SHOW;
		case StatType::SPIRIT_CRITICAL_DAMAGE:							return RESULT_SHOW;
		case StatType::SPIRIT_WEAPON_ATTACK_AMPLIFIER_RATE:				return RESULT_SHOW;
		case StatType::SPIRIT_PVP_ATTACK:								return RESULT_SHOW;
		case StatType::SPIRIT_HIT:										return RESULT_SHOW;
		case StatType::SKILL_HIT:										return RESULT_SHOW;
		case StatType::SKILL_DODGE:										return RESULT_SHOW;

			// STAT_ORDER_2 - 기초 스탯
		case StatType::MAX_WEIGHT:										return RESULT_SHOW;
		case StatType::MAX_HP:											return RESULT_SHOW;
		case StatType::MAX_MP:											return RESULT_SHOW;

			// STAT_ORDER_2 - 공격력 증폭률
		case StatType::ATTACK_MELEE_AMPLIFIER_RATE:						return RESULT_SHOW;
		case StatType::ATTACK_RANGE_AMPLIFIER_RATE:						return RESULT_SHOW;
		case StatType::ATTACK_MAGIC_AMPLIFIER_RATE:						return RESULT_SHOW;

			// STAT_ORDER_2 - 공격력
		case StatType::ATTACK_MELEE:									return RESULT_SHOW;
		case StatType::ATTACK_RANGE:									return RESULT_SHOW;
		case StatType::ATTACK_MAGIC:									return RESULT_SHOW;

			// STAT_ORDER_2 - 방어력
		case StatType::DEFENCE:											return RESULT_SHOW;

			// STAT_ORDER_2 - 회피
		case StatType::DODGE:											return RESULT_SHOW;

			// STAT_ORDER_2 - 스킬 회복(HP 회복, 추후 MP회복도 가능성 있다고함, 페어로 추가 필요)
		case StatType::HP_BONUS_HEAL_RATE:								return RESULT_SHOW;
		case StatType::HP_BONUS_HEAL_ACCEPT_RATE:						return RESULT_SHOW; // 전투 공식에만 적용, 보여지거나 다른 스탯에 영향을 주지 않음
		case StatType::HP_TICK_RECOVERY_PENALTY_RATE:					return RESULT_SHOW; // 전투 공식에만 적용, 보여지거나 다른 스탯에 영향을 주지 않음

			// STAT_ORDER_2 - 자연회복
		case StatType::HP_TICK_RECOVERY:								return RESULT_SHOW;
		case StatType::MP_TICK_RECOVERY:								return RESULT_SHOW;

			// STAT_ORDER_2 - 스킬저항
		case StatType::SKILL_RESIST:									return RESULT_SHOW;

			// STAT_ORDER_2 - 환경속성 패널티/저항
		case StatType::ENVIRONMENT_PENALTY_RESIST_HIT_AMOUNT:			return RESULT_SHOW;
		case StatType::ENVIRONMENT_PENALTY_RESIST_MOVE_RATE:			return RESULT_SHOW;
		case StatType::ENVIRONMENT_PENALTY_RESIST_ATTACK_SPEED_RATE:	return RESULT_SHOW;
		case StatType::ENVIRONMENT_PENALTY_RESIST_FRAME_RESIST_RATE:	return RESULT_SHOW;
		case StatType::ENVIRONMENT_PENALTY_RESIST_FROST_RESIST_RATE:	return RESULT_SHOW;
		case StatType::ENVIRONMENT_PENALTY_RESIST_DARK_RESIST_RATE:		return RESULT_SHOW;
		case StatType::ENVIRONMENT_PENALTY_RESIST_HOLY_RESIST_RATE:		return RESULT_SHOW;
		case StatType::ENVIRONMENT_PENALTY_RESIST_DEFENCE_AMOUNT:		return RESULT_SHOW;
		case StatType::ENVIRONMENT_PENALTY_RESIST_HP_RECOVERY_RATE:		return RESULT_SHOW;
		case StatType::ENVIRONMENT_PENALTY_RESIST_DODGE_AMOUNT:			return RESULT_SHOW;

			// STAT_ORDER_2 - 치명타
		case StatType::CRITICAL_DAMAGE:									return RESULT_SHOW;
		case StatType::CRITICAL_RATE_MELEE:								return RESULT_SHOW;
		case StatType::CRITICAL_RATE_RANGE:								return RESULT_SHOW;
		case StatType::CRITICAL_RATE_MAGIC:								return RESULT_SHOW;
		case StatType::CRITICAL_RESIST_RATE:							return RESULT_SHOW;

			// STAT_ORDER_2 - 관통
		case StatType::PENETRATION_MELEE:								return RESULT_SHOW;
		case StatType::PENETRATION_RANGE:								return RESULT_SHOW;
		case StatType::PENETRATION_MAGIC:								return RESULT_SHOW;

			// STAT_ORDER_2 - 피해감소
		case StatType::DAMAGE_REDUCTION_IGNORE:							return RESULT_SHOW;
		case StatType::DAMAGE_REDUCTION:								return RESULT_SHOW;

			// STAT_ORDER_2 - 속성 증폭/피해감소/저항
		case StatType::ELEMENT_DARK_AMPLIFIER:							return RESULT_SHOW;
		case StatType::ELEMENT_HOLY_AMPLIFIER:							return RESULT_SHOW;
		case StatType::ELEMENT_FLAME_AMPLIFIER:							return RESULT_SHOW;
		case StatType::ELEMENT_FROST_AMPLIFIER:							return RESULT_SHOW;
		case StatType::ELEMENT_DARK_REDUCTION:							return RESULT_SHOW;
		case StatType::ELEMENT_HOLY_REDUCTION:							return RESULT_SHOW;
		case StatType::ELEMENT_FLAME_REDUCTION:							return RESULT_SHOW;
		case StatType::ELEMENT_FROST_REDUCTION:							return RESULT_SHOW;

			// STAT_ORDER_2 - 이상 상태 내성
		case StatType::ABNORMALITY_STUN_HIT_RATE:						return RESULT_SHOW; // [R3 상태이상]
		case StatType::ABNORMALITY_BIND_HIT_RATE:						return RESULT_SHOW; // [R3 상태이상]
		case StatType::ABNORMALITY_FEAR_HIT_RATE:						return RESULT_SHOW; // [R3 상태이상]
		case StatType::ABNORMALITY_OTHER_HIT_RATE:						return RESULT_SHOW; // [R3 상태이상]
		
		case StatType::ABNORMALITY_STUN_RESIST_RATE:					return RESULT_SHOW; // [R3 상태이상]
		case StatType::ABNORMALITY_BIND_RESIST_RATE:					return RESULT_SHOW; // [R3 상태이상]
		case StatType::ABNORMALITY_FEAR_RESIST_RATE:					return RESULT_SHOW; // [R3 상태이상]
		case StatType::ABNORMALITY_OTHER_RESIST_RATE:					return RESULT_SHOW; // [R3 상태이상]

			// STAT_ORDER_3 - 공격력
		case StatType::PVP_ATTACK_MELEE:								return RESULT_SHOW;
		case StatType::PVP_ATTACK_RANGE:								return RESULT_SHOW;
		case StatType::PVP_ATTACK_MAGIC:								return RESULT_SHOW;

			// STAT_ORDER_3 - 회피
		case StatType::DODGE_MELEE:										return RESULT_SHOW;
		case StatType::DODGE_RANGE:										return RESULT_SHOW;
		case StatType::DODGE_MAGIC:										return RESULT_SHOW;

			// STAT_ORDER_3 - 자연회복
		case StatType::MP_TICK_RECOVERY_NORMAL:							return RESULT_SHOW;
		case StatType::MP_TICK_RECOVERY_PENALTY:						return RESULT_SHOW;

			// STAT_ORDER_3 - 치명타
		case StatType::PVP_CRITICAL_RATE_MELEE:							return RESULT_SHOW;
		case StatType::PVP_CRITICAL_RATE_RANGE:							return RESULT_SHOW;
		case StatType::PVP_CRITICAL_RATE_MAGIC:							return RESULT_SHOW;
		case StatType::CRITICAL_RESIST_RATE_MELEE:						return RESULT_SHOW;
		case StatType::CRITICAL_RESIST_RATE_RANGE:						return RESULT_SHOW;
		case StatType::CRITICAL_RESIST_RATE_MAGIC:						return RESULT_SHOW;

			// STAT_ORDER_3 - 관통
		case StatType::PVP_PENETRATION_MELEE:							return RESULT_HIDDEN;
		case StatType::PVP_PENETRATION_RANGE:							return RESULT_HIDDEN;
		case StatType::PVP_PENETRATION_MAGIC:							return RESULT_HIDDEN;

			// STAT_ORDER_3 - 피해감소
		case StatType::DAMAGE_REDUCTION_MELEE:							return RESULT_SHOW;
		case StatType::DAMAGE_REDUCTION_RANGE:							return RESULT_SHOW;
		case StatType::DAMAGE_REDUCTION_MAGIC:							return RESULT_SHOW;

			// STAT_ORDER_3 - 환경속성 패널티/저항
		case StatType::ENVIRONMENT_PENALTY_RESIST_HIT:					return RESULT_HIDDEN;
		case StatType::ENVIRONMENT_PENALTY_RESIST_MOVE:					return RESULT_HIDDEN;
		case StatType::ENVIRONMENT_PENALTY_RESIST_ATTACK_SPEED:			return RESULT_HIDDEN;
		case StatType::ENVIRONMENT_PENALTY_RESIST_FRAME_RESIST:			return RESULT_HIDDEN;
		case StatType::ENVIRONMENT_PENALTY_RESIST_FROST_RESIST:			return RESULT_HIDDEN;
		case StatType::ENVIRONMENT_PENALTY_RESIST_DARK_RESIST:			return RESULT_HIDDEN;
		case StatType::ENVIRONMENT_PENALTY_RESIST_HOLY_RESIST:			return RESULT_HIDDEN;
		case StatType::ENVIRONMENT_PENALTY_RESIST_DEFENCE:				return RESULT_HIDDEN;
		case StatType::ENVIRONMENT_PENALTY_RESIST_HP_RECOVERY:			return RESULT_HIDDEN;
		case StatType::ENVIRONMENT_PENALTY_RESIST_DODGE:				return RESULT_HIDDEN;

			// STAT_ORDER_3 - 속성 공격력
		case StatType::ELEMENT_DARK_ATTACK:								return RESULT_HIDDEN;
		case StatType::ELEMENT_HOLY_ATTACK:								return RESULT_HIDDEN;
		case StatType::ELEMENT_FLAME_ATTACK:							return RESULT_HIDDEN;
		case StatType::ELEMENT_FROST_ATTACK:							return RESULT_HIDDEN;

			// STAT_ORDER_4 - 각종 속도
		case StatType::MOVE_SPEED_RATE:									return RESULT_SHOW;
		case StatType::ATTACK_SPEED_RATE:								return RESULT_SHOW;
		case StatType::ELEMENT_FLAME_RESIST_RATE:						return RESULT_SHOW;
		case StatType::ELEMENT_FROST_RESIST_RATE:						return RESULT_SHOW;
		case StatType::ELEMENT_DARK_RESIST_RATE:						return RESULT_SHOW;
		case StatType::ELEMENT_HOLY_RESIST_RATE:						return RESULT_SHOW;

			// STAT_ORDER_4 - 명중
		case StatType::HIT_MELEE:										return RESULT_SHOW;
		case StatType::HIT_RANGE:										return RESULT_SHOW;
		case StatType::HIT_MAGIC:										return RESULT_SHOW;

			// STAT_ORDER_4 - 회피
		case StatType::PVP_DODGE_MELEE:									return RESULT_SHOW;
		case StatType::PVP_DODGE_RANGE:									return RESULT_SHOW;
		case StatType::PVP_DODGE_MAGIC:									return RESULT_SHOW;

			// STAT_ORDER_4 - 방어력
		case StatType::DEFENCE_MELEE:									return RESULT_HIDDEN;
		case StatType::DEFENCE_RANGE:									return RESULT_HIDDEN;
		case StatType::DEFENCE_MAGIC:									return RESULT_HIDDEN;

			// STAT_ORDER_4 - 치명타 저항률
		case StatType::PVP_CRITICAL_RESIST_RATE_MELEE:					return RESULT_SHOW;
		case StatType::PVP_CRITICAL_RESIST_RATE_RANGE:					return RESULT_SHOW;
		case StatType::PVP_CRITICAL_RESIST_RATE_MAGIC:					return RESULT_SHOW;

			// STAT_ORDER_4 - 자연회복
		case StatType::HP_TICK_RECOVERY_NORMAL:							return RESULT_SHOW;
		case StatType::HP_TICK_RECOVERY_PENALTY:						return RESULT_SHOW;

			// STAT_ORDER_4 - 피해 감소
		case StatType::PVP_DAMAGE_REDUCTION_MELEE:						return RESULT_SHOW;
		case StatType::PVP_DAMAGE_REDUCTION_RANGE:						return RESULT_SHOW;
		case StatType::PVP_DAMAGE_REDUCTION_MAGIC:						return RESULT_SHOW;

			// STAT_ORDER_5 - 속도
		case StatType::MOVE_SPEED:										return RESULT_HIDDEN;

			// STAT_ORDER_5 - 명중
		case StatType::PVP_HIT_MELEE:									return RESULT_HIDDEN;
		case StatType::PVP_HIT_RANGE:									return RESULT_HIDDEN;
		case StatType::PVP_HIT_MAGIC:									return RESULT_HIDDEN;
		
			// STAT_ORDER_5 - 방어력
		case StatType::PVP_DEFENCE_MELEE:								return RESULT_HIDDEN;
		case StatType::PVP_DEFENCE_RANGE:								return RESULT_HIDDEN;
		case StatType::PVP_DEFENCE_MAGIC:								return RESULT_HIDDEN;
		case StatType::MAX:												return RESULT_HIDDEN;


			// 23Y10M4W BM 확장을 위한 PVE 스탯 25종 추가
		case StatType::PVE_ATTACK_MELEE:								return RESULT_SHOW;
		case StatType::PVE_ATTACK_RANGE:								return RESULT_SHOW;
		case StatType::PVE_ATTACK_MAGIC:								return RESULT_SHOW;
		case StatType::PVE_HIT_MELEE:									return RESULT_SHOW;
		case StatType::PVE_HIT_RANGE:									return RESULT_SHOW;
		case StatType::PVE_HIT_MAGIC:									return RESULT_SHOW;
		case StatType::BONUS_CRITICAL_RATE_PVE:							return RESULT_SHOW;
		case StatType::BONUS_CRITICAL_RESIST_RATE_PVE:					return RESULT_SHOW;
		case StatType::PVE_DODGE_MELEE:									return RESULT_SHOW;
		case StatType::PVE_DODGE_RANGE:									return RESULT_SHOW;
		case StatType::PVE_DODGE_MAGIC:									return RESULT_SHOW;
		case StatType::PVE_DAMAGE_REDUCTION_MELEE:						return RESULT_SHOW;
		case StatType::PVE_DAMAGE_REDUCTION_RANGE:						return RESULT_SHOW;
		case StatType::PVE_DAMAGE_REDUCTION_MAGIC:						return RESULT_SHOW;
		case StatType::PVE_CRITICAL_RATE_MELEE:							return RESULT_SHOW;
		case StatType::PVE_CRITICAL_RATE_RANGE:							return RESULT_SHOW;
		case StatType::PVE_CRITICAL_RATE_MAGIC:							return RESULT_SHOW;
		case StatType::PVE_CRITICAL_RESIST_RATE_MELEE:					return RESULT_SHOW;
		case StatType::PVE_CRITICAL_RESIST_RATE_RANGE:					return RESULT_SHOW;
		case StatType::PVE_CRITICAL_RESIST_RATE_MAGIC:					return RESULT_SHOW;
		case StatType::SPIRIT_PVE_ATTACK:								return RESULT_SHOW;
		case StatType::BONUS_ATTACK_PVE:								return RESULT_SHOW;
		case StatType::BONUS_HIT_PVE:									return RESULT_SHOW;
		case StatType::BONUS_DODGE_PVE:									return RESULT_SHOW;
		case StatType::BONUS_DAMAGE_REDUCTION_PVE:						return RESULT_SHOW;

		default:
#ifdef _SERVER
			_ASSERT_CRASH_FMT(L" switch Case 추가 체크 필요  StatType:{} - {}", 
				statType, StatTypeEnumToString(statType)
			);
#endif
			return true;	// Stat 추가 瑛 , 안보여야, 여기다 코딩하기에 return true
		}
		
#ifdef _SERVER
		TRACE_END;
#endif
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static bool IsAbnormalityOverTime(AbnormalityEffectType abnormalityEffectType)
	{
		switch (abnormalityEffectType)
		{
		case AbnormalityEffectType::OVERTIME_DAMAGE:
		case AbnormalityEffectType::OVERTIME_HOT:
		case AbnormalityEffectType::OVERTIME_SENSITIVE:
			return true;
		//default:
		//	return false;
		}

		return false;
	}	

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static int GetTerritoryPriority(TerritoryType territoryType, TerritoryCheckType checkType = TerritoryCheckType::NONE)
	{
		switch (territoryType)
		{
		case TerritoryType::MAX: return -1;
		case TerritoryType::Env: return -1;
		case TerritoryType::SOUND: return -1;
		case TerritoryType::NONE: return 0;		
		case TerritoryType::SPACE_CRACK: return 1;
		case TerritoryType::SAFE_ZONE: return 10;
		case TerritoryType::NON_PK: return 11;
		case TerritoryType::TOWN: return 12;
		case TerritoryType::NO_CAMP_ZONE: return 13;
		case TerritoryType::TOWN_CHAOS: 
		{
			if (TerritoryCheckType::INVADE == checkType) return 16;
			else return 14;
		}
		case TerritoryType::ARENA_READY: return 15;		
		case TerritoryType::ARENA: return 17;
		case TerritoryType::ARRIVAL: return 18;					
		
		default:
#ifdef _SERVER
			_ASSERT_CRASH_FMT(L" switch Case 추가 체크 필요  TerritoryType:{} - {}",
				territoryType, TerritoryTypeEnumToString(territoryType)
			);
#endif
			return 0;
		}
	}

	// 수정 할 때 서버-클라 함수 함께 수정 요망.
#ifndef _SERVER
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 글자수 계산. 영문, 숫자, 공백을 제외한 모든 문자를 2자로 계산
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// [클라] 수정 할 때 클라-서버 함수 함께 수정 요망.
	static int GetStringLength(const FString& InString)
	{
		if (InString.IsEmpty())
		{
			return 0;
		}

		FString strPattrn = TEXT("[^0-9a-zA-Z\\s]");
		FRegexPattern pattern(strPattrn);

		int32 count = 0;
		for (int32 i = 0; i < InString.Len(); ++i)
		{
			FString tempCh;
			tempCh.AppendChar(InString[i]);

			FRegexMatcher matcher(pattern, tempCh);
			if (matcher.FindNext())
			{
				count += 2;
			}
			else
			{
				count += 1;
			}
		}

		return count;
	}
#else
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 글자수 계산. 영문, 숫자, 공백을 제외한 모든 문자를 2자로 계산
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// [서버] 수정 할 때 클라-서버 함수 함께 수정 요망.
	static int GetStringLength(const std::wstring& InString)
	{
		if (InString.empty())
		{
			return 0;
		}

		std::wregex pattern(L"[^0-9a-zA-Z\\s]");

		int count = 0;
		for (size_t i = 0; i < InString.length(); ++i)
		{
			std::wstring tempCh(1, InString[i]);

			std::wsmatch matcher;
			if (std::regex_search(tempCh, matcher, pattern))
			{
				count += 2;
			}
			else
			{
				count += 1;
			}
		}

		return count;
	}
#endif
	
#ifndef _SERVER
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// B1: 닉네임, 기사단명이 길어지는 현상 때문에 이름 타입의 경우 영어 숫자 상관없이 2로 처리 요청(CHR-18925)	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// [클라] 수정 할 때 클라-서버 함수 함께 수정 요망.
	static int GetStringLengthNameType(const FString& InString)
	{
		return InString.Len() * 2;
	}
#else
	// [서버] 수정 할 때 클라-서버 함수 함께 수정 요망.
	static int GetStringLengthNameType(const std::wstring& InString)
	{
		return static_cast<int>(InString.length() * 2);
	}
#endif

	// 서버에 FString 활용이 가능한지 몰라서 일단 전부 막고 올립니다
#ifndef _SERVER
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 이름에 쓰이는 문자 체크. 숫자, 제공언어(한영간번일)만 허용함
	// 관련 문서: https://jira.com2us.com/wiki/x/maBAEg
	// 주의: 코드 파일 포맷이 UTF8이 아니라 ANSI 이면 이 식이 제대로 동작 안함에 유의
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// [클라] 수정 할 때 클라-서버 함수 함께 수정 요망.
	static bool IsValidCharNameType(const FString& InString)
	{
		//FString strPattrn = TEXT("[^0-9a-zA-Z가-힣\\x{30a0}-\\x{30ff}\\x{3040}-\\x{309f}\\x{4e00}-\\x{9fff}]");
		 
		// B2: 국내 오픈에 한글, 영문, 숫자만 허용하도록 요청 옴
		FString strPattrn = TEXT("[^0-9a-zA-Z가-힣]");

		FRegexPattern pattern(strPattrn);
		FRegexMatcher matcher(pattern, InString);
		if (matcher.FindNext())
		{
			return false;
		}

		return true;
	}

#else
	// [서버] 수정 할 때 클라-서버 함수 함께 수정 요망.
	static bool IsValidCharNameType(const std::wstring& InString)
	{
		std::wregex pattern(L"[^0-9a-zA-Z가-힣"
			//L"\u30a0-\u30ff" // Katakana
			//L"\u3040-\u309f" // Hiragana
			//L"\u4e00-\u9fff" // CJK Unified Ideographs
			L"]");

		std::wsmatch matcher;
		if (std::regex_search(InString, matcher, pattern))
		{
			return false;
		}

		return true;
	}

	// [서버] 수정 할 때 클라-서버 함수 함께 수정 요망.(GM 전용, '[', ']' 추가)
	static bool IsValidGMCharNameType(const std::wstring& InString)
	{
		std::wregex pattern(L"^0-9a-zA-Z가-힣"
			//L"\u30a0-\u30ff" // Katakana
			//L"\u3040-\u309f" // Hiragana
			//L"\u4e00-\u9fff" // CJK Unified Ideographs
			);

		std::wsmatch matcher;
		if (std::regex_search(InString, matcher, pattern))
		{
			return false;
		}

		return true;
	}

	// [서버] 수정 할 때 클라-서버 함수 함께 수정 요망.
	static bool IsValidChatRoomNameType(const std::wstring& InString)
	{
		std::wregex pattern(L"[^0-9a-zA-Z가-힣"
			//L"\u30a0-\u30ff" // Katakana
			//L"\u3040-\u309f" // Hiragana
			//L"\u4e00-\u9fff" // CJK Unified Ideographs
			L"]");

		std::wsmatch matcher;
		if (std::regex_search(InString, matcher, pattern))
		{
			return false;
		}

		return true;
	}
#endif

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// NPC 상인 세금 포함 가격. 반올림 처리
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Currency GetTaxIncludedPrice(int32 InTaxPercent, Currency InPrice)
	{
		double taxRate = (InTaxPercent * 0.01) + 1;
		double price = InPrice * taxRate;
		uint64 priceNoDecimal = static_cast<uint64>(price);

		double decimalLeft = price - static_cast<double>(priceNoDecimal);
		// 반올림
		if (0.5 <= decimalLeft)
		{
			priceNoDecimal += 1;
		}

		return static_cast<Currency>(priceNoDecimal);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 거래소 세금 소수점 한자리 반올림
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static MarketPrice GetMarketTaxPrice(TaxRate taxRate, MarketPrice price)
	{
		double taxPrice = price * (taxRate * 0.0001); //세금
		//반올림
		taxPrice += 0.5;
		return static_cast<MarketPrice>(taxPrice);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 성(영지) 수입
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Currency GetCollectedTaxPrice(TaxPercent collectedTaxPercent, Currency InPrice)
	{
		double taxRate = (collectedTaxPercent * 0.01);
		double price = InPrice * taxRate;
		
		return static_cast<Currency>(price);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// UserDBId 에서 planetId를 가져온다.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static PlanetId GetPlanetId(UserDBId userDBId)
	{
		uint16 planet_world_id = (userDBId >> 48) & 0xFFFF;
		PlanetWorldId planetWorldId(planet_world_id);
		return planetWorldId.st.planetId;		
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 참과 같이 장착 위치를 가지는 장비 ItemEquipSlotIndex 로 슬롯의 위치(0, 1) 구한다.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static int GetEquipSlotLocation(PresetId presetId, ItemEquipSlotIndex equpLocationValue)
	{
		return ((equpLocationValue & (1 << (presetId - 1))) != 0) ? 1 : 0;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// GuildDBId 에서 planetId를 가져온다.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static WorldId GetWorldIdInGuildDBId(GuildDBId guidDBId)
	{
		uint16 planet_world_id = (guidDBId >> 48) & 0xFFFF;
		PlanetWorldId planetWorldId(planet_world_id);
		return planetWorldId.st.worldId;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Obstacle  NPC 인지 체크 
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static bool IsObstacleNpc(NpcFunctionType npcFunctionType)
	{
		if (npcFunctionType == NpcFunctionType::MAGIC_OBJECT
			|| npcFunctionType == NpcFunctionType::CART
			|| npcFunctionType == NpcFunctionType::SPACE_CRACK_CONTAMINANT
			|| npcFunctionType == NpcFunctionType::DOOR)
		{
			return true;
		}

		return false;
	}
}

#ifdef _SERVER_ENGINE
#pragma warning(pop)
#endif // #ifdef _SERVER_ENGINE
