// Fill out your copyright notice in the Description page of Project Settings.

#include "SpeechRecognizeCallbackProxy.h"
#include "FlytekVoiceSDK.h"



void USpeechRecognizeCallbackProxyAdvaced::Activate()
{
	
	//IFlytekVoiceSDK::Get().SpeechRecStartListening();
}

void USpeechRecognizeCallbackProxy::Tick(float DeltaTime)
{
	//ThreadManager::Create(this, &USpeechRecognizeCallbackProxy::StartListening, TEXT("StartListeningThread"));
	
}
void USpeechRecognizeCallbackProxy::StartListening()
{
	
}
bool USpeechRecognizeCallbackProxy::IsTickable() const
{
	return true;
}

TStatId USpeechRecognizeCallbackProxy::GetStatId() const
{
	return TStatId();
}
