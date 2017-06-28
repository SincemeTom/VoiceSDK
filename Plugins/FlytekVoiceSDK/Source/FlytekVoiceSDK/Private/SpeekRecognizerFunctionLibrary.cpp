// Fill out your copyright notice in the Description page of Project Settings.

#include "SpeekRecognizerFunctionLibrary.h"
#include "FlytekVoiceSDK.h"


USpeechRecognizer* USpeekRecognizerFunctionLibrary::InitializeSpeechRecognizer(ESpeechLanguage InLanguage)
{
	return IFlytekVoiceSDK::Get().InitializeSpeechRecognize(InLanguage);
}
void USpeekRecognizerFunctionLibrary::StartLisening()
{
	IFlytekVoiceSDK::Get().SpeechRecStartListening();
}
void USpeekRecognizerFunctionLibrary::StopLisening()
{
	IFlytekVoiceSDK::Get().SpeechRecStopListening();
}
