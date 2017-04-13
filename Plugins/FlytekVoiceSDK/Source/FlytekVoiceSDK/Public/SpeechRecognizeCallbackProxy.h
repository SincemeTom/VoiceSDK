// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Tickable.h"
#include "SpeechRecognizeCallbackProxy.generated.h"

/**
 * 
 */
UCLASS()
class FLYTEKVOICESDK_API USpeechRecognizeCallbackProxyAdvaced : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	void Activate() override;
	
	
};

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
	FCriticalSection AccessLock;

	int Result;

	bool IsCancel;
	bool CanFireCallback;



};
