// Fill out your copyright notice in the Description page of Project Settings.

#include "SpeekRecognizerFunctionLibrary.h"
#include "FlytekVoiceSDK.h"



void USpeekRecognizerFunctionLibrary::MSRLogin(const FString& Params)
{
	IFlytekVoiceSDK::Get().VoiceSDKLogin(FString(), FString(), Params);
}

void USpeekRecognizerFunctionLibrary::StartLisening()
{
	if (IFlytekVoiceSDK::Get().SpeechRecInit() == 0)
	{
		if (IFlytekVoiceSDK::Get().SpeechRecStartListening() == 0)
		{
			UE_LOG(LogFlytekVoiceSDK, Log, TEXT("Start SpeechRecognize!"))
		}
	}	
}
void USpeekRecognizerFunctionLibrary::StopLisening()
{
	IFlytekVoiceSDK::Get().SpeechRecStopListening();
	IFlytekVoiceSDK::Get().SpeechRecUninit();
}
