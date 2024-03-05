// Fill out your copyright notice in the Description page of Project Settings.


#include "GsCheatNet.h"
#include "Net/GsNetSendService.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

/*
	치트이름, 한 칸 띄고 첫 번째 인자, 한 칸 띄고 두 번째 인자…
	ex> NetCheat create_item 2 33		// 아이템 아이디가 2인 바나나를 33개 생성합니다.
					↑		 ↑  ↑
				  명령어	   인자 인자

	ex> NetCheat create_gold 1000		// 골드를 1000원 생성합니다.
					↑		   ↑
				  명령어		  인자

	명령목록 리스트는 아래 원노트에 업데이트 되니 참고 바랍니다.
	onenote:///\\192.168.1.41\raon\%5b테크%5d\SharedOneNote\서버\작업공유.one#M0%20서버%20치트키&section-id={853FE9EC-D9F0-467D-B484-03A27B2EFE58}&page-id={4AECBE43-E8B2-474B-983D-27F8D8488779}&end
*/
void UGsCheatNet::NC(const FString& in_val)
{
	FGsNetSendService::SendWorldAdminCommandQA(in_val);
}

/*
	위의 방법으론 인자값 노출이 불가능하니 원할 시 따로 추가 해 줍니다.
	위 포맷을 지킵니다.
	아래로 추가 해주세요.
*/

 //Npc 디스폰 처리
 //In_Radius : 반경
void UGsCheatNet::Despawn_npc(int In_Radius)
{
	const FString command = FString::Printf(
		TEXT("despawn_npc %d"), In_Radius);

	FGsNetSendService::SendWorldAdminCommandQA(command);
}