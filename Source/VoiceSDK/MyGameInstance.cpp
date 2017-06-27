// Fill out your copyright notice in the Description page of Project Settings.

#include "VoiceSDK.h"
#include "FlytekVoiceSDK.h"
#include "SpeechRecognizer.h"
#include "MyGameInstance.h"

void UMyGameInstance::Init()
{
	//SpeechRecUObj = IFlytekVoiceSDK::Get().InitializeSpeechRecognize();
	//SpeechRecUObj->CallbackResult.AddUObject(this, &UMyGameInstance::OnSpeechRecognizeResult);
	IFlytekVoiceSDK::Get().InitializeSpeechRecognize()->CallbackResult.AddUObject(this, &UMyGameInstance::OnSpeechRecognizeResult);
}




