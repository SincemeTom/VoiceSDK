// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Tickable.h"
#include "FlytekVoiceSDK.h"
#include "SpeechRecognizer.generated.h"

/**
 * 
 */

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

	void CallSRInit();

	void CallSRUninit();

	void CallSRStartListening();

	void CallSRStopListening();

	//CallBack
	void HandleOnLoginResult();

	
	
	/*
	virtual void SpeechRecLogout();

	//Init SpeechRec
	virtual int32 SpeechRecInit();

	virtual void SpeechRecStartListening();

	virtual void SpeechRecStopListening();

	virtual void SpeechRecUninit();

	virtual int32 SpeechRecWriteAudioData();

	//Callback
	*/
	

private:
	FCriticalSection AccessLock;
	int32 ErrorResult = -1;
	struct speech_rec SpeechRec;
	struct speech_rec_notifier RecNotifier;
	bool bLoginSuccessful;
	bool bInitSuccessful;
	bool bSpeeking;
	TSharedPtr<class FThreadClass> SpeechThread;
};
