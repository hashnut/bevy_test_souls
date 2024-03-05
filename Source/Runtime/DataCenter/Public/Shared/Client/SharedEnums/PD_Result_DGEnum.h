#pragma once

namespace PD {

namespace DG {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not modify this file because it was generated automatically
// PD_Result_DGEnumClient.h
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM()
enum DGResult
{
	RESULT_SUCCESS = 0,	// 성공(로그 출력용)
	LOGIN_ERROR_UNKNOWN = 100,
	LOGIN_ERROR_INTERNAL_SERVER_ERROR = 101,
	LOGIN_ERROR_INVALID_ACCOUNT_ID = 102,
	LOGIN_ERROR_INVALID_VID = 103,
	LOGIN_ERROR_INCORRECT_TOKEN = 104,
	LOGIN_ERROR_CANNOT_LOAD_ACCOUNT_USER_DB = 105,
	LOGIN_ERROR_TIME_OUT = 106,
	LOGIN_ERROR_INVALID_AUTH_STATUS = 107,
	LOGIN_ERROR_ACCOUNT_EXPIRING_RETRY_AGAIN = 108,
	LOGIN_ERROR_SERVER_MAINTENANCE = 109,

	PACKET_RESULT_MAX = 110,
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline DGResult DGResultStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("RESULT_SUCCESS"), ESearchCase::IgnoreCase) == 0)	return DGResult::RESULT_SUCCESS;
	if (str.Compare(TEXT("LOGIN_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return DGResult::LOGIN_ERROR_UNKNOWN;
	if (str.Compare(TEXT("LOGIN_ERROR_INTERNAL_SERVER_ERROR"), ESearchCase::IgnoreCase) == 0)	return DGResult::LOGIN_ERROR_INTERNAL_SERVER_ERROR;
	if (str.Compare(TEXT("LOGIN_ERROR_INVALID_ACCOUNT_ID"), ESearchCase::IgnoreCase) == 0)	return DGResult::LOGIN_ERROR_INVALID_ACCOUNT_ID;
	if (str.Compare(TEXT("LOGIN_ERROR_INVALID_VID"), ESearchCase::IgnoreCase) == 0)	return DGResult::LOGIN_ERROR_INVALID_VID;
	if (str.Compare(TEXT("LOGIN_ERROR_INCORRECT_TOKEN"), ESearchCase::IgnoreCase) == 0)	return DGResult::LOGIN_ERROR_INCORRECT_TOKEN;
	if (str.Compare(TEXT("LOGIN_ERROR_CANNOT_LOAD_ACCOUNT_USER_DB"), ESearchCase::IgnoreCase) == 0)	return DGResult::LOGIN_ERROR_CANNOT_LOAD_ACCOUNT_USER_DB;
	if (str.Compare(TEXT("LOGIN_ERROR_TIME_OUT"), ESearchCase::IgnoreCase) == 0)	return DGResult::LOGIN_ERROR_TIME_OUT;
	if (str.Compare(TEXT("LOGIN_ERROR_INVALID_AUTH_STATUS"), ESearchCase::IgnoreCase) == 0)	return DGResult::LOGIN_ERROR_INVALID_AUTH_STATUS;
	if (str.Compare(TEXT("LOGIN_ERROR_ACCOUNT_EXPIRING_RETRY_AGAIN"), ESearchCase::IgnoreCase) == 0)	return DGResult::LOGIN_ERROR_ACCOUNT_EXPIRING_RETRY_AGAIN;
	if (str.Compare(TEXT("LOGIN_ERROR_SERVER_MAINTENANCE"), ESearchCase::IgnoreCase) == 0)	return DGResult::LOGIN_ERROR_SERVER_MAINTENANCE;
	return DGResult::PACKET_RESULT_MAX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString DGResultEnumToString(DGResult value)
{
	switch (value)
	{
		case DGResult::RESULT_SUCCESS: return TEXT("RESULT_SUCCESS");
		case DGResult::LOGIN_ERROR_UNKNOWN: return TEXT("LOGIN_ERROR_UNKNOWN");
		case DGResult::LOGIN_ERROR_INTERNAL_SERVER_ERROR: return TEXT("LOGIN_ERROR_INTERNAL_SERVER_ERROR");
		case DGResult::LOGIN_ERROR_INVALID_ACCOUNT_ID: return TEXT("LOGIN_ERROR_INVALID_ACCOUNT_ID");
		case DGResult::LOGIN_ERROR_INVALID_VID: return TEXT("LOGIN_ERROR_INVALID_VID");
		case DGResult::LOGIN_ERROR_INCORRECT_TOKEN: return TEXT("LOGIN_ERROR_INCORRECT_TOKEN");
		case DGResult::LOGIN_ERROR_CANNOT_LOAD_ACCOUNT_USER_DB: return TEXT("LOGIN_ERROR_CANNOT_LOAD_ACCOUNT_USER_DB");
		case DGResult::LOGIN_ERROR_TIME_OUT: return TEXT("LOGIN_ERROR_TIME_OUT");
		case DGResult::LOGIN_ERROR_INVALID_AUTH_STATUS: return TEXT("LOGIN_ERROR_INVALID_AUTH_STATUS");
		case DGResult::LOGIN_ERROR_ACCOUNT_EXPIRING_RETRY_AGAIN: return TEXT("LOGIN_ERROR_ACCOUNT_EXPIRING_RETRY_AGAIN");
		case DGResult::LOGIN_ERROR_SERVER_MAINTENANCE: return TEXT("LOGIN_ERROR_SERVER_MAINTENANCE");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> DisplayKey (클라이언트 로컬라이징 번역 키를 처리하기 위해 활용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString DGResultEnumToDisplayKey(DGResult value)
{
	switch (value)
	{
		case DGResult::RESULT_SUCCESS: return TEXT("RESULT_SUCCESS");
		case DGResult::LOGIN_ERROR_UNKNOWN: return TEXT("");
		case DGResult::LOGIN_ERROR_INTERNAL_SERVER_ERROR: return TEXT("");
		case DGResult::LOGIN_ERROR_INVALID_ACCOUNT_ID: return TEXT("");
		case DGResult::LOGIN_ERROR_INVALID_VID: return TEXT("");
		case DGResult::LOGIN_ERROR_INCORRECT_TOKEN: return TEXT("");
		case DGResult::LOGIN_ERROR_CANNOT_LOAD_ACCOUNT_USER_DB: return TEXT("");
		case DGResult::LOGIN_ERROR_TIME_OUT: return TEXT("");
		case DGResult::LOGIN_ERROR_INVALID_AUTH_STATUS: return TEXT("");
		case DGResult::LOGIN_ERROR_ACCOUNT_EXPIRING_RETRY_AGAIN: return TEXT("");
		case DGResult::LOGIN_ERROR_SERVER_MAINTENANCE: return TEXT("");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Validators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool IsValidDGResult(DGResult value) noexcept
{
	return (value >= DGResult::RESULT_SUCCESS && value < DGResult::PACKET_RESULT_MAX);
}

}	// namespace PD
}	// namespace DG
