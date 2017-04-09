// Fill out your copyright notice in the Description page of Project Settings.

#include "VoiceSDK.h"
#include "FlytekVoiceSDK.h"
#include "MyGameInstance.h"

void UMyGameInstance::Init()
{
	IFlytekVoiceSDK::Get().CallbackResult.AddUObject(this, &UMyGameInstance::OnSpeechRecognizeResult);
}




