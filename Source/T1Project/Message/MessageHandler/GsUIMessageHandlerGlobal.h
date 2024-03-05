#pragma once
#include "GsSystemMessageHandlerInterface.h"
#include "Message/GsMessageUI.h"

struct IGsMessageParam;

using FGsUIMessage = TGsMessageDelegateList<MUI, MessageUI>;

class T1PROJECT_API FGsUIMessageHandlerGlobal :
	public IGsMessageHandler, FGsUIMessage
{
public:
	FGsUIMessageHandlerGlobal() = default;
	virtual ~FGsUIMessageHandlerGlobal();

public:
	virtual void InitializeMessage() override;
	virtual void FinalizeMessage() override;

public:
	void OnMoviePlay(const IGsMessageParam*);
	void OnMovieStop(const IGsMessageParam*);
	void OnHideUIForMovie(const IGsMessageParam* InParam);

	void OnSelectLanguage(const IGsMessageParam* InParam);

	void OnPrevLoadMap(const IGsMessageParam*);
	void OnPostLoadMap(const IGsMessageParam*);

	void OnInvalidateUIScale(const IGsMessageParam* InParam);
};
