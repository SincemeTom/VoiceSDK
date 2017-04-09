// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class VOICESDK_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:

	virtual void Init() override;

	UFUNCTION(BlueprintImplementableEvent, Category = SpeechRec)
		void OnSpeechRecognizeResult(const FString& OutString);
	//UPROPERTY(BlueprintAssignable, Category = SpeechRec)
	//	FSpeechRecognizeResultDelegate CallbackResult;
	
};
