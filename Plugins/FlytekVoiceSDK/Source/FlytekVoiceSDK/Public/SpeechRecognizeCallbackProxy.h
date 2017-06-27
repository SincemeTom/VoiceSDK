// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Tickable.h"
#include "SpeechRecognizeCallbackProxy.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ESpeechLanguage : uint8
{
	EL_English,
	EL_Chinese
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSpeechRecognizeDelegate, const FString& , Result, int32 ,ErrorCode);

UCLASS()
class FLYTEKVOICESDK_API USpeechRecognizeCallbackProxyAdvaced : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),Category = "SpeechRecognizer")
		static USpeechRecognizeCallbackProxyAdvaced* SpeechRecognize(ESpeechLanguage Language, bool AutoStop);

	UPROPERTY(BlueprintAssignable)
		FSpeechRecognizeDelegate OnFaild;
	UPROPERTY(BlueprintAssignable)
		FSpeechRecognizeDelegate OnSuccess;
	UPROPERTY(BlueprintAssignable)
		FSpeechRecognizeDelegate OnTimeOut;
	void Activate() override;
	
	UFUNCTION(BlueprintCallable, Category = "SpeechRecognizer")
		void StopReconize();
private:
	void PostActive(ESpeechLanguage Language, bool AutoStop);
	ESpeechLanguage SpeechLanguage;
	bool bAutoStop;
	
};
/*
UCLASS()
class FLYTEKVOICESDK_API USpeechRecognizeCallbackProxy : public UObject, public FTickableGameObject
{
	GENERATED_BODY()
public:
		USpeechRecognizeCallbackProxy() {}
	~USpeechRecognizeCallbackProxy() {}

	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable()  const override;
	virtual TStatId GetStatId() const override;
	void StartListening();
private:

	int Result;

	bool IsCancel;
	bool CanFireCallback;



};*/
