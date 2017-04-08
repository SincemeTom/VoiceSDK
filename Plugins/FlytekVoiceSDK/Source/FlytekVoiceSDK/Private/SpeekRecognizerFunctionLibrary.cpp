// Fill out your copyright notice in the Description page of Project Settings.

#include "SpeekRecognizerFunctionLibrary.h"
#include "FlytekVoiceSDK.h"



void USpeekRecognizerFunctionLibrary::MSRLogin(const FString& Params)
{
	IFlytekVoiceSDK::Get().VoiceSDKLogin(FString(), FString(), Params);
}
