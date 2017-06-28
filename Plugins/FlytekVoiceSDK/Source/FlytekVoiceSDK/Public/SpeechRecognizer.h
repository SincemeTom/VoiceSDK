// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Tickable.h"
#include "FlytekVoiceSDK.h"
#include "TaskGraphInterfaces.h"
#include "SpeechRecognizer.generated.h"

/**
 * 
 */

enum ETaskAction
{
	ES_NULL,
	ES_LOGIN,
	ES_LOGOUT,
	ES_INIT,
	ES_UNINIT,
	ES_STARTLISTENING,
	ES_STOPLISTENING,
	ES_MAXSTATE
};

DECLARE_MULTICAST_DELEGATE_OneParam(FSpeechRecognizeResultDelegate, const FString&);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpeechRecognizeResult, const FString&, ResultText);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInitializeResult, int32, ErrorCode);


UCLASS(BlueprintType)
class FLYTEKVOICESDK_API USpeechRecognizer : public UObject, public FTickableGameObject
{
	GENERATED_BODY()
public:
	USpeechRecognizer();
	~USpeechRecognizer();
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable()  const override;
	virtual TStatId GetStatId() const override;
	virtual void PostInitProperties();
	//Login in IFlytek Voice Plugin
	void SpeechRecLoginRequest(const FString& UserName, const FString& Password, const FString& Params);

	void SpeechRecLogoutRequest();

	void SpeechRecInitRequest();

	void SpeechRecUninitRequest();

	void SpeechRecStartListeningRequest();

	void SpeechRecStopListeningRequest();


	void OnSpeechRecResult(const char* result, char is_last);

	void OnSpeechRecBegin();

	void OnSpeechRecEnd(int reason);
	
	void SetParams(ESpeechLanguage InLanguage);


private:

	//Call Plugin Functions
	int32 CallSRLogin(const FString& UserName, const FString& Password, const FString& Params);

	void CallSRLogout();

	int32 CallSRInit();

	int32 CallSRUninit();

	int32 CallSRStartListening();

	int32 CallSRStopListening();

	//CallBack
	void HandleOnLoginResult();


public:
	UPROPERTY(BlueprintAssignable, Category = SpeechRec)
	FOnSpeechRecognizeResult SpeeckResult;
	UPROPERTY(BlueprintAssignable, Category = SpeechRec)
	FOnInitializeResult InitResult;
	FSpeechRecognizeResultDelegate CallbackResult;
	
	bool bAutoStop;
	bool bLoginSuccessful;
protected:
	ESpeechLanguage SRLanguage;
	FString Session_Begin_Param;
	FGraphEventRef SpeechRecognizeCompletion[ETaskAction::ES_MAXSTATE];
	FCriticalSection AccessLock;
	int32 ErrorResult[ETaskAction::ES_MAXSTATE];
	struct speech_rec SpeechRec;
	struct speech_rec_notifier RecNotifier;
	bool bInitSuccessful;
	bool bSpeeking;
	bool bOnSpeechRecResultSuccesful;
	bool bOnSpeechRecBeginSuccesful;
	bool bOnSpeechRecEndSuccesful;
	FString SpeechResultString;
};
