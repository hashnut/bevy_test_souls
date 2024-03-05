#pragma once

/*	
	핸들러중 예약 관련 처리
*/
class IGsReserveWork
{
public:
	virtual ~IGsReserveWork() = default;

public:
	// 데이터 예약이 있는가
	virtual bool IsReserveData() = 0;
	// 재시도 해라
	virtual void DoRetry() = 0;

	// 예약된 데이터 클리어해라
	virtual void ClearReserveData(){}
};