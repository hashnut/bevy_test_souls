#pragma once

namespace PD {

namespace GW {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not modify this file because it was generated automatically
// PD_Result_GWEnumClient.h
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM()
enum GWResult
{
	RESULT_SUCCESS = 0,	// 성공(로그 출력용)
	REGISTER_ERROR_UNKNOWN = 100,
	REGISTER_ERROR_MISMATCH_PLANET_WORLD_ID = 101,
	BM_SHOP_ERROR_GATEWAY_OPERATION_FAILED = 200,	// BM 상점 - 게이트 웨이 작업에 실패하였습니다
	BM_SHOP_ERROR_INAVLID_MAKETPID_FROM_CLIENT = 201,	// BM 상점 - 잘못된 정보의 maketPId 값입니다
	BM_SHOP_ERROR_DB_OPERATION_FAILED = 202,	// BM 상점 - 게이트 웨이 DB 작업에 실패하였습니다
	BM_SHOP_ERROR_DUPULICATION_HIVE_IAP_TRANSACTION_ID = 203,	// BM 상점 - 구매  중복된 HIVE_IAP_TRANSACTION_ID
	BM_SHOP_DONT_TRY_HIVE_RECEIPT_VERIFY = 204,	// BM 상점 - 하이브 영수증 검증 안함
	BM_SHOP_DONT_TRY_HIVE_IAP_ITEM_RESULT = 205,	// BM 상점 - 하이브 아이템 지급 결과 전송 안함
	HIVE_RECEIPT_VERIFY_ERROR_SETTING = 230,	// 하이브 영수증 검증 - 설정값 오류
	HIVE_RECEIPT_VERIFY_ERROR_HOST = 231,	// 하이브 영수증 검증 - 호스트 설정값 오류
	HIVE_RECEIPT_VERIFY_ERROR = 232,	// 하이브 영수증 검증 - 검증 실패
	HIVE_ERROR_RECEIPT_VERIFY_NO_REQUESTED_PARAMETER = 233,	// 하이브 영수증 검증 - 전송된 파라미터가 없을 때(1000001)
	HIVE_ERROR_RECEIPT_VERIFY_DB_CONNECTION = 234,	// 하이브 영수증 검증 - DB Connection이 안 될 때(1000003)
	HIVE_ERROR_RECEIPT_VERIFY_INTERNAL_SERVER = 235,	// 하이브 영수증 검증 - 내부 서버 에러(1000005)
	HIVE_ERROR_RECEIPT_VERIFY_NO_REQUIRED_PARAMETER = 236,	// 하이브 영수증 검증 - 필수 파라미터 값이 없을 때(1000006)
	HIVE_ERROR_RECEIPT_VERIFY_FAILED_TO_AUTHENTICATE = 237,	// 하이브 영수증 검증 - 영수증 검증 실패 또는 해킹된 영수증일 때(1000503)
	HIVE_ERROR_RECEIPT_VERIFY_GOOGLE_PUBLIC_KEY_NO_REGISTERED = 238,	// 하이브 영수증 검증 - 구글 영수증 검증을 위한 licence key가 등록되지 않았을 때(1000504)
	HIVE_ERROR_RECEIPT_VERIFY_ONE_STORE_NO_REGISTERED = 239,	// 하이브 영수증 검증 - 원스토어 영수증 검증을 위한 market appid가 등록되지 않았을 때(1000505)
	HIVE_ERROR_RECEIPT_VERIFY_SERVER_ABNORMAL = 240,	// 하이브 영수증 검증 - 마켓 영수증 검증 서버에 이상이 있을 경우(1000516)
	HIVE_ERROR_RECEIPT_VERIFY_PENDING_STATE = 241,	// 하이브 영수증 검증 - 느린 결제(예. 편의점 결제) 시도 시 결제 대기(Pending) 상태로 전송되는 영수증. 결제 전 영수증이기 때문에 어떠한 처리도 하지 않음(1000519)
	HIVE_ERROR_IAP_PURCHASE_CANCEL_STATE = 242,	// 하이브 영수증 검증 - 검증은 성공했으나 구매취소상태. 마켓이 애플 앱스토어나 원스토어일 경우에만 취소 상태 판별 가능
	HIVE_ERROR_RECEIPT_VERIFY_UNABLE_TO_CONNECT = 243,	// 하이브 영수증 검증 - 서버 연결 에러
	HIVE_ERROR_RECEIPT_VERIFY_PARSE = 244,	// 하이브 영수증 검증 - 파싱 에러
	HIVE_ERROR_RECEIPT_VERIFY_NO_RESULT = 245,	// 하이브 영수증 검증 - result 없음
	HIVE_ERROR_RECEIPT_VERIFY_NO_HIVEIAP_TRANSACTION_ID = 246,	// 하이브 영수증 검증 - 검증은 성공했으나 hiveiap_transaction_id 없음
	HIVE_ERROR_RECEIPT_VERIFY_UNKNOWN = 247,	// 하이브 영수증 검증 - 알 수 없는 오류(지정된 결과 코드가 아님)
	HIVE_ERROR_IAP_ITEM_RESULT_SETTING = 248,	// 하이브 아이템 지급 결과 전송 - 설정값 오류
	HIVE_ERROR_IAP_ITEM_RESULT_HOST = 249,	// 하이브 아이템 지급 결과 전송 - 호스트 설정값 오류
	HIVE_ERROR_IAP_ITEM_RESULT = 250,	// 하이브 아이템 지급 결과 전송 - 실패
	HIVE_ERROR_IAP_ITEM_RESULT_UNABLE_TO_CONNECT = 251,	// 하이브 아이템 지급 결과 전송 - 서버 연결 에러
	HIVE_ERROR_IAP_ITEM_RESULT_PARSE = 252,	// 하이브 아이템 지급 결과 전송 - 파싱 에러
	HIVE_ERROR_IAP_ITEM_RESULT_NO_RESULT = 253,	// 하이브 아이템 지급 결과 전송 - RESULT 없음
	HIVE_ERROR_IAP_ITEM_RESULT_UNKNOWN = 254,	// 하이브 아이템 지급 결과 전송 - 알 수 없는 오류(지정된 결과 코드가 아님)
	HIVE_ERROR_TIME_ZONE_RESULT_FAIL = 255,	// 하이브 타임존 결과 전송 - 실패
	HIVE_ERROR_TIME_ZONE_RESULT_UNKNOWN = 256,	// 하이브 타임존 결과 전송 - 알 수 없는 오류(지정된 결과 코드가 아님)
	HIVE_ERROR_TIME_ZONE_RESULT_NO_TIME_ZONE_INFO = 257,	// 하이브 타임존 결과 전송 - 타임존 결과 없음

	PACKET_RESULT_MAX = 258,
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline GWResult GWResultStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("RESULT_SUCCESS"), ESearchCase::IgnoreCase) == 0)	return GWResult::RESULT_SUCCESS;
	if (str.Compare(TEXT("REGISTER_ERROR_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return GWResult::REGISTER_ERROR_UNKNOWN;
	if (str.Compare(TEXT("REGISTER_ERROR_MISMATCH_PLANET_WORLD_ID"), ESearchCase::IgnoreCase) == 0)	return GWResult::REGISTER_ERROR_MISMATCH_PLANET_WORLD_ID;
	if (str.Compare(TEXT("BM_SHOP_ERROR_GATEWAY_OPERATION_FAILED"), ESearchCase::IgnoreCase) == 0)	return GWResult::BM_SHOP_ERROR_GATEWAY_OPERATION_FAILED;
	if (str.Compare(TEXT("BM_SHOP_ERROR_INAVLID_MAKETPID_FROM_CLIENT"), ESearchCase::IgnoreCase) == 0)	return GWResult::BM_SHOP_ERROR_INAVLID_MAKETPID_FROM_CLIENT;
	if (str.Compare(TEXT("BM_SHOP_ERROR_DB_OPERATION_FAILED"), ESearchCase::IgnoreCase) == 0)	return GWResult::BM_SHOP_ERROR_DB_OPERATION_FAILED;
	if (str.Compare(TEXT("BM_SHOP_ERROR_DUPULICATION_HIVE_IAP_TRANSACTION_ID"), ESearchCase::IgnoreCase) == 0)	return GWResult::BM_SHOP_ERROR_DUPULICATION_HIVE_IAP_TRANSACTION_ID;
	if (str.Compare(TEXT("BM_SHOP_DONT_TRY_HIVE_RECEIPT_VERIFY"), ESearchCase::IgnoreCase) == 0)	return GWResult::BM_SHOP_DONT_TRY_HIVE_RECEIPT_VERIFY;
	if (str.Compare(TEXT("BM_SHOP_DONT_TRY_HIVE_IAP_ITEM_RESULT"), ESearchCase::IgnoreCase) == 0)	return GWResult::BM_SHOP_DONT_TRY_HIVE_IAP_ITEM_RESULT;
	if (str.Compare(TEXT("HIVE_RECEIPT_VERIFY_ERROR_SETTING"), ESearchCase::IgnoreCase) == 0)	return GWResult::HIVE_RECEIPT_VERIFY_ERROR_SETTING;
	if (str.Compare(TEXT("HIVE_RECEIPT_VERIFY_ERROR_HOST"), ESearchCase::IgnoreCase) == 0)	return GWResult::HIVE_RECEIPT_VERIFY_ERROR_HOST;
	if (str.Compare(TEXT("HIVE_RECEIPT_VERIFY_ERROR"), ESearchCase::IgnoreCase) == 0)	return GWResult::HIVE_RECEIPT_VERIFY_ERROR;
	if (str.Compare(TEXT("HIVE_ERROR_RECEIPT_VERIFY_NO_REQUESTED_PARAMETER"), ESearchCase::IgnoreCase) == 0)	return GWResult::HIVE_ERROR_RECEIPT_VERIFY_NO_REQUESTED_PARAMETER;
	if (str.Compare(TEXT("HIVE_ERROR_RECEIPT_VERIFY_DB_CONNECTION"), ESearchCase::IgnoreCase) == 0)	return GWResult::HIVE_ERROR_RECEIPT_VERIFY_DB_CONNECTION;
	if (str.Compare(TEXT("HIVE_ERROR_RECEIPT_VERIFY_INTERNAL_SERVER"), ESearchCase::IgnoreCase) == 0)	return GWResult::HIVE_ERROR_RECEIPT_VERIFY_INTERNAL_SERVER;
	if (str.Compare(TEXT("HIVE_ERROR_RECEIPT_VERIFY_NO_REQUIRED_PARAMETER"), ESearchCase::IgnoreCase) == 0)	return GWResult::HIVE_ERROR_RECEIPT_VERIFY_NO_REQUIRED_PARAMETER;
	if (str.Compare(TEXT("HIVE_ERROR_RECEIPT_VERIFY_FAILED_TO_AUTHENTICATE"), ESearchCase::IgnoreCase) == 0)	return GWResult::HIVE_ERROR_RECEIPT_VERIFY_FAILED_TO_AUTHENTICATE;
	if (str.Compare(TEXT("HIVE_ERROR_RECEIPT_VERIFY_GOOGLE_PUBLIC_KEY_NO_REGISTERED"), ESearchCase::IgnoreCase) == 0)	return GWResult::HIVE_ERROR_RECEIPT_VERIFY_GOOGLE_PUBLIC_KEY_NO_REGISTERED;
	if (str.Compare(TEXT("HIVE_ERROR_RECEIPT_VERIFY_ONE_STORE_NO_REGISTERED"), ESearchCase::IgnoreCase) == 0)	return GWResult::HIVE_ERROR_RECEIPT_VERIFY_ONE_STORE_NO_REGISTERED;
	if (str.Compare(TEXT("HIVE_ERROR_RECEIPT_VERIFY_SERVER_ABNORMAL"), ESearchCase::IgnoreCase) == 0)	return GWResult::HIVE_ERROR_RECEIPT_VERIFY_SERVER_ABNORMAL;
	if (str.Compare(TEXT("HIVE_ERROR_RECEIPT_VERIFY_PENDING_STATE"), ESearchCase::IgnoreCase) == 0)	return GWResult::HIVE_ERROR_RECEIPT_VERIFY_PENDING_STATE;
	if (str.Compare(TEXT("HIVE_ERROR_IAP_PURCHASE_CANCEL_STATE"), ESearchCase::IgnoreCase) == 0)	return GWResult::HIVE_ERROR_IAP_PURCHASE_CANCEL_STATE;
	if (str.Compare(TEXT("HIVE_ERROR_RECEIPT_VERIFY_UNABLE_TO_CONNECT"), ESearchCase::IgnoreCase) == 0)	return GWResult::HIVE_ERROR_RECEIPT_VERIFY_UNABLE_TO_CONNECT;
	if (str.Compare(TEXT("HIVE_ERROR_RECEIPT_VERIFY_PARSE"), ESearchCase::IgnoreCase) == 0)	return GWResult::HIVE_ERROR_RECEIPT_VERIFY_PARSE;
	if (str.Compare(TEXT("HIVE_ERROR_RECEIPT_VERIFY_NO_RESULT"), ESearchCase::IgnoreCase) == 0)	return GWResult::HIVE_ERROR_RECEIPT_VERIFY_NO_RESULT;
	if (str.Compare(TEXT("HIVE_ERROR_RECEIPT_VERIFY_NO_HIVEIAP_TRANSACTION_ID"), ESearchCase::IgnoreCase) == 0)	return GWResult::HIVE_ERROR_RECEIPT_VERIFY_NO_HIVEIAP_TRANSACTION_ID;
	if (str.Compare(TEXT("HIVE_ERROR_RECEIPT_VERIFY_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return GWResult::HIVE_ERROR_RECEIPT_VERIFY_UNKNOWN;
	if (str.Compare(TEXT("HIVE_ERROR_IAP_ITEM_RESULT_SETTING"), ESearchCase::IgnoreCase) == 0)	return GWResult::HIVE_ERROR_IAP_ITEM_RESULT_SETTING;
	if (str.Compare(TEXT("HIVE_ERROR_IAP_ITEM_RESULT_HOST"), ESearchCase::IgnoreCase) == 0)	return GWResult::HIVE_ERROR_IAP_ITEM_RESULT_HOST;
	if (str.Compare(TEXT("HIVE_ERROR_IAP_ITEM_RESULT"), ESearchCase::IgnoreCase) == 0)	return GWResult::HIVE_ERROR_IAP_ITEM_RESULT;
	if (str.Compare(TEXT("HIVE_ERROR_IAP_ITEM_RESULT_UNABLE_TO_CONNECT"), ESearchCase::IgnoreCase) == 0)	return GWResult::HIVE_ERROR_IAP_ITEM_RESULT_UNABLE_TO_CONNECT;
	if (str.Compare(TEXT("HIVE_ERROR_IAP_ITEM_RESULT_PARSE"), ESearchCase::IgnoreCase) == 0)	return GWResult::HIVE_ERROR_IAP_ITEM_RESULT_PARSE;
	if (str.Compare(TEXT("HIVE_ERROR_IAP_ITEM_RESULT_NO_RESULT"), ESearchCase::IgnoreCase) == 0)	return GWResult::HIVE_ERROR_IAP_ITEM_RESULT_NO_RESULT;
	if (str.Compare(TEXT("HIVE_ERROR_IAP_ITEM_RESULT_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return GWResult::HIVE_ERROR_IAP_ITEM_RESULT_UNKNOWN;
	if (str.Compare(TEXT("HIVE_ERROR_TIME_ZONE_RESULT_FAIL"), ESearchCase::IgnoreCase) == 0)	return GWResult::HIVE_ERROR_TIME_ZONE_RESULT_FAIL;
	if (str.Compare(TEXT("HIVE_ERROR_TIME_ZONE_RESULT_UNKNOWN"), ESearchCase::IgnoreCase) == 0)	return GWResult::HIVE_ERROR_TIME_ZONE_RESULT_UNKNOWN;
	if (str.Compare(TEXT("HIVE_ERROR_TIME_ZONE_RESULT_NO_TIME_ZONE_INFO"), ESearchCase::IgnoreCase) == 0)	return GWResult::HIVE_ERROR_TIME_ZONE_RESULT_NO_TIME_ZONE_INFO;
	return GWResult::PACKET_RESULT_MAX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString GWResultEnumToString(GWResult value)
{
	switch (value)
	{
		case GWResult::RESULT_SUCCESS: return TEXT("RESULT_SUCCESS");
		case GWResult::REGISTER_ERROR_UNKNOWN: return TEXT("REGISTER_ERROR_UNKNOWN");
		case GWResult::REGISTER_ERROR_MISMATCH_PLANET_WORLD_ID: return TEXT("REGISTER_ERROR_MISMATCH_PLANET_WORLD_ID");
		case GWResult::BM_SHOP_ERROR_GATEWAY_OPERATION_FAILED: return TEXT("BM_SHOP_ERROR_GATEWAY_OPERATION_FAILED");
		case GWResult::BM_SHOP_ERROR_INAVLID_MAKETPID_FROM_CLIENT: return TEXT("BM_SHOP_ERROR_INAVLID_MAKETPID_FROM_CLIENT");
		case GWResult::BM_SHOP_ERROR_DB_OPERATION_FAILED: return TEXT("BM_SHOP_ERROR_DB_OPERATION_FAILED");
		case GWResult::BM_SHOP_ERROR_DUPULICATION_HIVE_IAP_TRANSACTION_ID: return TEXT("BM_SHOP_ERROR_DUPULICATION_HIVE_IAP_TRANSACTION_ID");
		case GWResult::BM_SHOP_DONT_TRY_HIVE_RECEIPT_VERIFY: return TEXT("BM_SHOP_DONT_TRY_HIVE_RECEIPT_VERIFY");
		case GWResult::BM_SHOP_DONT_TRY_HIVE_IAP_ITEM_RESULT: return TEXT("BM_SHOP_DONT_TRY_HIVE_IAP_ITEM_RESULT");
		case GWResult::HIVE_RECEIPT_VERIFY_ERROR_SETTING: return TEXT("HIVE_RECEIPT_VERIFY_ERROR_SETTING");
		case GWResult::HIVE_RECEIPT_VERIFY_ERROR_HOST: return TEXT("HIVE_RECEIPT_VERIFY_ERROR_HOST");
		case GWResult::HIVE_RECEIPT_VERIFY_ERROR: return TEXT("HIVE_RECEIPT_VERIFY_ERROR");
		case GWResult::HIVE_ERROR_RECEIPT_VERIFY_NO_REQUESTED_PARAMETER: return TEXT("HIVE_ERROR_RECEIPT_VERIFY_NO_REQUESTED_PARAMETER");
		case GWResult::HIVE_ERROR_RECEIPT_VERIFY_DB_CONNECTION: return TEXT("HIVE_ERROR_RECEIPT_VERIFY_DB_CONNECTION");
		case GWResult::HIVE_ERROR_RECEIPT_VERIFY_INTERNAL_SERVER: return TEXT("HIVE_ERROR_RECEIPT_VERIFY_INTERNAL_SERVER");
		case GWResult::HIVE_ERROR_RECEIPT_VERIFY_NO_REQUIRED_PARAMETER: return TEXT("HIVE_ERROR_RECEIPT_VERIFY_NO_REQUIRED_PARAMETER");
		case GWResult::HIVE_ERROR_RECEIPT_VERIFY_FAILED_TO_AUTHENTICATE: return TEXT("HIVE_ERROR_RECEIPT_VERIFY_FAILED_TO_AUTHENTICATE");
		case GWResult::HIVE_ERROR_RECEIPT_VERIFY_GOOGLE_PUBLIC_KEY_NO_REGISTERED: return TEXT("HIVE_ERROR_RECEIPT_VERIFY_GOOGLE_PUBLIC_KEY_NO_REGISTERED");
		case GWResult::HIVE_ERROR_RECEIPT_VERIFY_ONE_STORE_NO_REGISTERED: return TEXT("HIVE_ERROR_RECEIPT_VERIFY_ONE_STORE_NO_REGISTERED");
		case GWResult::HIVE_ERROR_RECEIPT_VERIFY_SERVER_ABNORMAL: return TEXT("HIVE_ERROR_RECEIPT_VERIFY_SERVER_ABNORMAL");
		case GWResult::HIVE_ERROR_RECEIPT_VERIFY_PENDING_STATE: return TEXT("HIVE_ERROR_RECEIPT_VERIFY_PENDING_STATE");
		case GWResult::HIVE_ERROR_IAP_PURCHASE_CANCEL_STATE: return TEXT("HIVE_ERROR_IAP_PURCHASE_CANCEL_STATE");
		case GWResult::HIVE_ERROR_RECEIPT_VERIFY_UNABLE_TO_CONNECT: return TEXT("HIVE_ERROR_RECEIPT_VERIFY_UNABLE_TO_CONNECT");
		case GWResult::HIVE_ERROR_RECEIPT_VERIFY_PARSE: return TEXT("HIVE_ERROR_RECEIPT_VERIFY_PARSE");
		case GWResult::HIVE_ERROR_RECEIPT_VERIFY_NO_RESULT: return TEXT("HIVE_ERROR_RECEIPT_VERIFY_NO_RESULT");
		case GWResult::HIVE_ERROR_RECEIPT_VERIFY_NO_HIVEIAP_TRANSACTION_ID: return TEXT("HIVE_ERROR_RECEIPT_VERIFY_NO_HIVEIAP_TRANSACTION_ID");
		case GWResult::HIVE_ERROR_RECEIPT_VERIFY_UNKNOWN: return TEXT("HIVE_ERROR_RECEIPT_VERIFY_UNKNOWN");
		case GWResult::HIVE_ERROR_IAP_ITEM_RESULT_SETTING: return TEXT("HIVE_ERROR_IAP_ITEM_RESULT_SETTING");
		case GWResult::HIVE_ERROR_IAP_ITEM_RESULT_HOST: return TEXT("HIVE_ERROR_IAP_ITEM_RESULT_HOST");
		case GWResult::HIVE_ERROR_IAP_ITEM_RESULT: return TEXT("HIVE_ERROR_IAP_ITEM_RESULT");
		case GWResult::HIVE_ERROR_IAP_ITEM_RESULT_UNABLE_TO_CONNECT: return TEXT("HIVE_ERROR_IAP_ITEM_RESULT_UNABLE_TO_CONNECT");
		case GWResult::HIVE_ERROR_IAP_ITEM_RESULT_PARSE: return TEXT("HIVE_ERROR_IAP_ITEM_RESULT_PARSE");
		case GWResult::HIVE_ERROR_IAP_ITEM_RESULT_NO_RESULT: return TEXT("HIVE_ERROR_IAP_ITEM_RESULT_NO_RESULT");
		case GWResult::HIVE_ERROR_IAP_ITEM_RESULT_UNKNOWN: return TEXT("HIVE_ERROR_IAP_ITEM_RESULT_UNKNOWN");
		case GWResult::HIVE_ERROR_TIME_ZONE_RESULT_FAIL: return TEXT("HIVE_ERROR_TIME_ZONE_RESULT_FAIL");
		case GWResult::HIVE_ERROR_TIME_ZONE_RESULT_UNKNOWN: return TEXT("HIVE_ERROR_TIME_ZONE_RESULT_UNKNOWN");
		case GWResult::HIVE_ERROR_TIME_ZONE_RESULT_NO_TIME_ZONE_INFO: return TEXT("HIVE_ERROR_TIME_ZONE_RESULT_NO_TIME_ZONE_INFO");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> DisplayKey (클라이언트 로컬라이징 번역 키를 처리하기 위해 활용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString GWResultEnumToDisplayKey(GWResult value)
{
	switch (value)
	{
		case GWResult::RESULT_SUCCESS: return TEXT("RESULT_SUCCESS");
		case GWResult::REGISTER_ERROR_UNKNOWN: return TEXT("");
		case GWResult::REGISTER_ERROR_MISMATCH_PLANET_WORLD_ID: return TEXT("");
		case GWResult::BM_SHOP_ERROR_GATEWAY_OPERATION_FAILED: return TEXT("");
		case GWResult::BM_SHOP_ERROR_INAVLID_MAKETPID_FROM_CLIENT: return TEXT("");
		case GWResult::BM_SHOP_ERROR_DB_OPERATION_FAILED: return TEXT("");
		case GWResult::BM_SHOP_ERROR_DUPULICATION_HIVE_IAP_TRANSACTION_ID: return TEXT("");
		case GWResult::BM_SHOP_DONT_TRY_HIVE_RECEIPT_VERIFY: return TEXT("");
		case GWResult::BM_SHOP_DONT_TRY_HIVE_IAP_ITEM_RESULT: return TEXT("");
		case GWResult::HIVE_RECEIPT_VERIFY_ERROR_SETTING: return TEXT("");
		case GWResult::HIVE_RECEIPT_VERIFY_ERROR_HOST: return TEXT("");
		case GWResult::HIVE_RECEIPT_VERIFY_ERROR: return TEXT("");
		case GWResult::HIVE_ERROR_RECEIPT_VERIFY_NO_REQUESTED_PARAMETER: return TEXT("");
		case GWResult::HIVE_ERROR_RECEIPT_VERIFY_DB_CONNECTION: return TEXT("");
		case GWResult::HIVE_ERROR_RECEIPT_VERIFY_INTERNAL_SERVER: return TEXT("");
		case GWResult::HIVE_ERROR_RECEIPT_VERIFY_NO_REQUIRED_PARAMETER: return TEXT("");
		case GWResult::HIVE_ERROR_RECEIPT_VERIFY_FAILED_TO_AUTHENTICATE: return TEXT("");
		case GWResult::HIVE_ERROR_RECEIPT_VERIFY_GOOGLE_PUBLIC_KEY_NO_REGISTERED: return TEXT("");
		case GWResult::HIVE_ERROR_RECEIPT_VERIFY_ONE_STORE_NO_REGISTERED: return TEXT("");
		case GWResult::HIVE_ERROR_RECEIPT_VERIFY_SERVER_ABNORMAL: return TEXT("");
		case GWResult::HIVE_ERROR_RECEIPT_VERIFY_PENDING_STATE: return TEXT("");
		case GWResult::HIVE_ERROR_IAP_PURCHASE_CANCEL_STATE: return TEXT("");
		case GWResult::HIVE_ERROR_RECEIPT_VERIFY_UNABLE_TO_CONNECT: return TEXT("");
		case GWResult::HIVE_ERROR_RECEIPT_VERIFY_PARSE: return TEXT("");
		case GWResult::HIVE_ERROR_RECEIPT_VERIFY_NO_RESULT: return TEXT("");
		case GWResult::HIVE_ERROR_RECEIPT_VERIFY_NO_HIVEIAP_TRANSACTION_ID: return TEXT("");
		case GWResult::HIVE_ERROR_RECEIPT_VERIFY_UNKNOWN: return TEXT("");
		case GWResult::HIVE_ERROR_IAP_ITEM_RESULT_SETTING: return TEXT("");
		case GWResult::HIVE_ERROR_IAP_ITEM_RESULT_HOST: return TEXT("");
		case GWResult::HIVE_ERROR_IAP_ITEM_RESULT: return TEXT("");
		case GWResult::HIVE_ERROR_IAP_ITEM_RESULT_UNABLE_TO_CONNECT: return TEXT("");
		case GWResult::HIVE_ERROR_IAP_ITEM_RESULT_PARSE: return TEXT("");
		case GWResult::HIVE_ERROR_IAP_ITEM_RESULT_NO_RESULT: return TEXT("");
		case GWResult::HIVE_ERROR_IAP_ITEM_RESULT_UNKNOWN: return TEXT("");
		case GWResult::HIVE_ERROR_TIME_ZONE_RESULT_FAIL: return TEXT("");
		case GWResult::HIVE_ERROR_TIME_ZONE_RESULT_UNKNOWN: return TEXT("");
		case GWResult::HIVE_ERROR_TIME_ZONE_RESULT_NO_TIME_ZONE_INFO: return TEXT("");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Validators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool IsValidGWResult(GWResult value) noexcept
{
	return (value >= GWResult::RESULT_SUCCESS && value < GWResult::PACKET_RESULT_MAX);
}

}	// namespace PD
}	// namespace GW
