// Fill out your copyright notice in the Description page of Project Settings.

#include "SpeekRecognizerFunctionLibrary.h"
#include "FlytekVoiceSDK.h"



void USpeekRecognizerFunctionLibrary::MSRLogin(const FString& Params)
{
	IFlytekVoiceSDK::Get().VoiceSDKLogin(FString(), FString(), Params);
}

void USpeekRecognizerFunctionLibrary::MSRInit()
{
	IFlytekVoiceSDK::Get().SpeechRecInit();
}
void USpeekRecognizerFunctionLibrary::StartLisening()
{
	IFlytekVoiceSDK::Get().SpeechRecStartListening();
}
void USpeekRecognizerFunctionLibrary::StopLisening()
{
	IFlytekVoiceSDK::Get().SpeechRecStopListening();
}
