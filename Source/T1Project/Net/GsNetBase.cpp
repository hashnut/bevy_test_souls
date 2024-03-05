
#include "GsNetBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "StackTracker.h"
#include "T1Project.h"

#include "Message/GsMessageSystem.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsHiveManager.h"

// connection 처리를 한다.
void FGsNetBase::Enter()
{
	GSLOG(Warning, TEXT("Net Enter : %s"), *_sessionName);

	/*FGsMessageHolder* messageMgr = GMessage();
	if (nullptr == messageMgr)
		return;

	MSystem& mSystem = messageMgr->GetSystem();

	_msgHandleSystemList.Emplace(mSystem.AddRaw(MessageSystem::HIVE_CHECK_MAINTENANCE_TRUE,
		this, &FGsNetBase::HiveCheckMaintenanceResultSucceeded));
	_msgHandleSystemList.Emplace(mSystem.AddRaw(MessageSystem::HIVE_CHECK_MAINTENANCE_FALSE,
		this, &FGsNetBase::HiveCheckMaintenanceResultFailed));*/
};

void FGsNetBase::Exit()
{
	GSLOG(Warning, TEXT("Net Exit : %s"), *_sessionName);
	Disconnect();

	if (nullptr != _session)
	{
		_session->ClearDelegate();
	}

	/*if (FGsMessageHolder* messageMgr = GMessage())
	{
		MSystem& mSystem = messageMgr->GetSystem();
		for (MsgSystemHandle& handleSystem : _msgHandleSystemList)
		{
			mSystem.Remove(handleSystem);
		}
	}
	_msgHandleSystemList.Empty();

	StopCheckMaintenanceTimer();*/
};

void FGsNetBase::Update(float indeltaTime)
{
	if (nullptr != _session)
	{
		_session->Tick(indeltaTime);
	}
};

//세션 객체 생성처리
void FGsNetBase::InitConnection()
{
	if (nullptr == _session)
	{
		_session = TUniquePtr<FClientSession>(new FClientSession());
	}

	if (nullptr == _localSendBuffer)
	{
		_localSendBuffer = TUniquePtr<FGsNetBuffer>(new FGsNetBuffer(SEND_BUFFER_SIZE));
	}
}

void FGsNetBase::SetPort(unsigned short port)
{
	_port = port;	
}

void FGsNetBase::Send(int size, bool ignoreSendObserver)
{
	// 임시로 예외처리 합니다.
	// TODO: 주석처리되어 있는, FGsNetGame::Send 의 _isSpawnMeCompleted 판별 처리가 완료되면, 코드 정리 필요
	if (false == IsVaildSessionCondition())
	{
		GSLOG(Error, TEXT("-------FGsNetBase::Send, IsVaildSessionCondition false ------------"));
		return;
	}

	_session->Send(_localSendBuffer->GetBuffer(), size);

	// 패킷Send 관찰모드에 진입하면, 콜스택을 로그로 찍는다.
	if (ignoreSendObserver == false  && _observerSending)
	{
		GSLOG(Error, TEXT("----------------- FGsNetGame::Send Try CallStack  -------------------"));
		FStackTracker stack(nullptr, nullptr, true);
		stack.CaptureStackTrace();
		stack.DumpStackTraces(0, *GLog);
	}
}

void FGsNetBase::Disconnect(bool inNotify)
{
	if (nullptr != _session)
	{
		// bak1210 : 중요 !!
		// 디스컨텍트 함수호출시 노티파이옵션을 true로 호출하면 내부에서 델리게이트중 DisconnectionComplateDelegate이 호출되는데..
		// 이함수에서는 재연결 프로세스가 돌면서 Connent 함수를 호출하게되고 Session에 필요한 델리게이트를 재할당한다.
		// 그럼으로 ClearDelegate함수는 Session->Disconnect(false); 인상태에서 호출하는것이 맞다.
		if(_session.IsValid())
			_session->Disconnect(inNotify);		

		// 점검 중인지 체크 - 2022-06-28 로비플로우 개선하면서 삭제
		//OnCheckMaintenance();
		//StartCheckMaintenanceTimer();
	}
}

// bak1210 : 외부에서 메시지가 제대로 동작하는지 확인하기 위한 용처
void FGsNetBase::TestErrorMessageFunc(int32 inErrCode)
{
	if (nullptr != _session && _session->ErrorMessageDelegate.IsBound())
	{
		_session->ErrorMessageDelegate.Broadcast(inErrCode);
	}
}

// 임시 : DeviceId를 가져와서 String의 Hash값을 사용
uint64 FGsNetBase::GetDId() const
{
	FString deviceId = UKismetSystemLibrary::GetDeviceId();
	uint64 hashCode = (uint64)(GetTypeHash(deviceId));
	GSLOG(Log, TEXT("===== DeviceId: %s, Vid(HashCode): %d ====="), *deviceId, hashCode);
	return hashCode;
}

bool FGsNetBase::IsVaildSessionCondition() const
{
	if (nullptr == _session)
	{
		return false;
	}

	return _session->IsConnected() && _session->IsHandshakeCompleted();
}

void FGsNetBase::SetPacketSendBlock(bool inSend)
{
	GSLOG(Log, TEXT("FGsNetBase::SetPacketSendBlock %d"), inSend);

	_sendblock = inSend;
}

bool FGsNetBase::IsBlockingSend()
{
	return _sendblock;
}

//void FGsNetBase::OnCheckMaintenance()
//{
//#ifdef WINDOWS_CROS_PLAY_PROCESS
//#if !WITH_EDITOR
//	GHive()->CheckMaintenance();
//#endif
// #else
//#if PLATFORM_ANDROID || PLATFORM_IOS
//	GHive()->CheckMaintenance();
//#endif
// #endif
//}
//
//void FGsNetBase::StartCheckMaintenanceTimer()
//{
//	const UWorld* world = GHive()->GetWorld();
//	if (nullptr == world)
//		return;
//
//	if (_checkMaintenanceTimer.IsValid())
//	{
//		world->GetTimerManager().ClearTimer(_checkMaintenanceTimer);
//	}
//
//	if (nullptr != world)
//	{
//		world->GetTimerManager().SetTimer(_checkMaintenanceTimer,
//			FTimerDelegate::CreateRaw(this, &FGsNetBase::OnCheckMaintenance),
//			3.0f, true);
//	}
//}
//
//void FGsNetBase::StopCheckMaintenanceTimer()
//{
//	const UWorld* world = GHive()->GetWorld();
//	if (nullptr == world)
//		return;
//
//	if (_checkMaintenanceTimer.IsValid())
//	{
//		world->GetTimerManager().ClearTimer(_checkMaintenanceTimer);
//	}
//}
//
//// 하이브 HiveCheckMaintenance 실패
//void FGsNetBase::HiveCheckMaintenanceResultFailed()
//{
//	GSLOG(Log, TEXT("FGsNetBase::HiveCheckMaintenanceResultFailed() false"));
//}
//
//// 하이브 HiveCheckMaintenance 성공
//void FGsNetBase::HiveCheckMaintenanceResultSucceeded()
//{
//	GSLOG(Log, TEXT("FGsNetBase::HiveCheckMaintenanceResultSucceeded() true"));
//}