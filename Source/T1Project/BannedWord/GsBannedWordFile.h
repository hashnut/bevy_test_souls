#pragma once
#include "CoreMinimal.h"

/* 
 * 금칙어 번들 파일 로드 세이브 데이터 읽고 쓰는 기능
 * UE4는 UTF16 Text, AES-256 Pack 방식 사용하고 있음
 *		형식 : word \t lang \t country \t usage \n
 *		포맷 : UTF8 TextFile 
 *		패킹 : AES-128 + Zlib compress
 *		언패킹 : OpenSSL AES-128 key로 AES_cbc_encrypt 
 */
class FGsBannedWordFile
{
private:
	unsigned char AESKeySet[16] = { 0x38, 0x33, 0x31, 0x38, 0x37, 0x34, 0x39, 0x30, 0x36, 0x36, 0x33, 0x38, 0x38, 0x34, 0x39, 0x30 };
	
	TArray<unsigned char> _orgData;
	TArray<unsigned char> _processData;

	FString _banFildPath;
	uint32  _fileSize{ 0 };

public:
	void Reset();

	void OpenBannedWordFile(FString bannedFilePath);
	void OpenBannedWordFileWithData(FString bannedFilePath, const TArray<uint8>& downloadFileData, int32 size);
	void SaveBannedWordFile(bool useOrgData = true);
	void CloseBannedWordFile();

	bool DecryptionData(bool useOrgData = true);
	bool UncompressData(bool useOrgData = false);

	//현재 파일의 체크썸 비교
	bool IsSameMd5CheckSum(FString downMd5Key);
	bool IsSameFileSize(uint32 fileSize) { return _fileSize == fileSize; }
	uint32 GetFileSize() { return _fileSize; }

	int  GetBannedWordStringList(OUT TArray<FString>& stringList);
	bool IsValidBannedWordFile();
};

