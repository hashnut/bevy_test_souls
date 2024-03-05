#include "GsBannedWordFile.h"
#include "Serialization/Archive.h"
#include "Misc/SecureHash.h"
#include "Misc/Compression.h"
#include "openssl/aes.h"
#include "FileManager.h"

void FGsBannedWordFile::Reset() 
{
	_orgData.Reset();
	_processData.Reset();
	_banFildPath.Empty();

	_fileSize = 0;
}

void FGsBannedWordFile::OpenBannedWordFile(FString bannedFilePath)
{
	_banFildPath = bannedFilePath;
	_fileSize = IFileManager::Get().FileSize(*bannedFilePath);

	_orgData.Reset();
	if (FArchive* arReader = IFileManager::Get().CreateFileReader(*bannedFilePath))
	{
		*arReader << _orgData;
		arReader->Close();
		arReader = nullptr;
	}

	_processData.Reset();
	_processData.Append(_orgData.GetData(), _orgData.Num());
}

void FGsBannedWordFile::OpenBannedWordFileWithData(FString bannedFilePath, const TArray<uint8>& downloadFileData, int32 size)
{
	Reset();

	_banFildPath = bannedFilePath;
	_orgData.Append(downloadFileData.GetData(), size);
	_processData.Append(_orgData.GetData(), _orgData.Num());
}

void FGsBannedWordFile::SaveBannedWordFile(bool useOrgData)
{
	if (IsValidBannedWordFile())
	{
		auto& saveData = useOrgData ? _orgData : _processData;

		if (FArchive* arWriter = IFileManager::Get().CreateFileWriter(*_banFildPath))
		{
			*arWriter << saveData;

			arWriter->Close();

			delete arWriter;
			arWriter = nullptr;
		}

		_fileSize = IFileManager::Get().FileSize(*_banFildPath);
	}
}

void FGsBannedWordFile::CloseBannedWordFile()
{
	Reset();
}

bool FGsBannedWordFile::DecryptionData(bool useOrgData /*= true*/)
{
	if (IsValidBannedWordFile())
	{
		if (useOrgData)
		{
			_processData.Reset();
			_processData.Append(_orgData.GetData(), _orgData.Num());
		}

		if (unsigned char* useData = _processData.GetData())
		{
			TArray<uint8> outData;
			outData.AddZeroed(_processData.Num());

			unsigned char iv[AES_BLOCK_SIZE];
			FMemory::Memset(iv, 0x00, AES_BLOCK_SIZE);

			AES_KEY dec_key;
			AES_set_decrypt_key(AESKeySet, 128, &dec_key);
			AES_cbc_encrypt(useData, outData.GetData(), _processData.Num(), &dec_key, iv, AES_DECRYPT);

			Swap(_processData, outData);

			//auto aesKey = keySet.GetCharArray().GetData();
			//auto ansiKey = (ANSICHAR*)TCHAR_TO_ANSI(aesKey);
			//FAES::DecryptData(useData, _processData.Num(), aesKey); 256bit key 사용

			return true;
		}
	}

	return false;
}

bool FGsBannedWordFile::UncompressData(bool useOrgData /*= false*/)
{
	if (IsValidBannedWordFile())
	{
		if (useOrgData)
		{
			_processData.Reset();
			_processData.Append(_orgData.GetData(), _orgData.Num());
		}

		if (unsigned char* useData = _processData.GetData())
		{
			TArray<uint8> outputArray;
			outputArray.AddZeroed(_processData.Num() * 5);

			//uncompres size가 다르면 return 값이 false 이다. 체크하지 않는다.
			FCompression::UncompressMemory(NAME_Zlib, outputArray.GetData(), outputArray.Num(), useData, _processData.Num(), ECompressionFlags::COMPRESS_ZLIB); 

			_processData.Reset();
			_processData.Append(outputArray.GetData(), outputArray.Num());

			return true;
		}
	}

	return false;
}

bool FGsBannedWordFile::IsSameMd5CheckSum(FString downMd5Key)
{
	if (IsValidBannedWordFile())
	{
		FMD5 md5;
		FString md5InputKey = md5.HashBytes(_orgData.GetData(), _orgData.Num());

		return downMd5Key == md5InputKey;
	}
	return false;
}

int FGsBannedWordFile::GetBannedWordStringList(OUT TArray<FString>& data)
{
	FString unParsedWord = UTF8_TO_TCHAR(_processData.GetData());
	unParsedWord.ParseIntoArray(data, TEXT("\n"), true);

	return data.Num();
}

bool FGsBannedWordFile::IsValidBannedWordFile()
{
	return !_banFildPath.IsEmpty() && _orgData.Num() > 0;
}

