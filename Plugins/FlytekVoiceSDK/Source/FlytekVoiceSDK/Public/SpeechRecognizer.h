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

enum EThreadState
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

UCLASS()
class FLYTEKVOICESDK_API USpeechRecognizer : public UObject, public FTickableGameObject
{
	GENERATED_BODY()
public:
	USpeechRecognizer();
	~USpeechRecognizer();
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable()  const override;
	virtual TStatId GetStatId() const override;

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

private:

	//Call Plugin Functions
	int32 CallSRLogin(const FString& UserName, const FString& Password, const FString& Params);

	void CallSRLogout();

	int32 CallSRInit();

	void CallSRUninit();

	int32 CallSRStartListening();

	int32 CallSRStopListening();

	//CallBack
	void HandleOnLoginResult();

	

private:
	FGraphEventRef SpeechRecognizeCompletion[EThreadState::ES_MAXSTATE];
	FCriticalSection AccessLock;
	int32 ErrorResult[EThreadState::ES_MAXSTATE];
	struct speech_rec SpeechRec;
	struct speech_rec_notifier RecNotifier;
	bool bLoginSuccessful;
	bool bInitSuccessful;
	bool bSpeeking;
	TSharedPtr< class FThreadClass >  SpeechThread;
};
