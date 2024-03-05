/*
# Desc : PSO용 자동 데이터 수집 실행 차후 테스트 완료되면 editor, engine으로 이동, 자동화 설계에 초석

# 테스트를 위한 최소한의 필요한 기능
1. 선택 폴더에서 PSO(파티클 시스템) 데이터 애셋 검색 및 리스트화(차후에 메쉬나 다른부분에도 생기면 그때 추가, 현재는 테스트에 집중)
2. PSO Map(Level)에서 즉시 로드, 플레이
3. PSO 데이터 세이브(자동세이브이긴한데, 명령하는 커맨드도 있음)
*/

#pragma once

class UParticleSystem;
class UWorld;

namespace PSOTools
{
	//static TArray<UParticleSystem*> ParticleSystemlist;

	void AutoPSOGetheringPSOData();
	void LoadImmediatelyAndRun(UWorld* InWorld);
	void Save();
	/*class FAutoPSOGetheringPSOData
	{
	};*/
}