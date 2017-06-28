// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SpeechRecognizer.h"
#include "SpeekRecognizerFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class FLYTEKVOICESDK_API USpeekRecognizerFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:


	UFUNCTION(BlueprintCallable, Category = SpeekRecognizer)
	static class USpeechRecognizer* InitializeSpeechRecognizer(ESpeechLanguage InLanguage);

	UFUNCTION(BlueprintCallable, Category = SpeekRecognizer)
	static void StartLisening();

	UFUNCTION(BlueprintCallable, Category = SpeekRecognizer)
	static void StopLisening();
	
};
