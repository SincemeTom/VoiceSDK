// Fill out your copyright notice in the Description page of Project Settings.

#include "SpeechRecognizeCallbackProxy.h"
#include "FlytekVoiceSDK.h"



USpeechRecognizeCallbackProxyAdvaced* USpeechRecognizeCallbackProxyAdvaced::SpeechRecognize(ESpeechLanguage Language, bool AutoStop)
{
	USpeechRecognizeCallbackProxyAdvaced* Instance = NewObject<USpeechRecognizeCallbackProxyAdvaced>();
	Instance->PostActive(Language, AutoStop);
	return Instance;
}

void USpeechRecognizeCallbackProxyAdvaced::Activate()
{
	//IFlytekVoiceSDK::Get().InitializeSpeechRecognize()
	
}
void USpeechRecognizeCallbackProxyAdvaced::PostActive(ESpeechLanguage Language, bool AutoStop)
{
	SpeechLanguage = Language;
	AutoStop = bAutoStop;
}
void USpeechRecognizeCallbackProxyAdvaced::StopReconize()
{

}
/*
void USpeechRecognizeCallbackProxy::Tick(float DeltaTime)
{

	
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
}*/
