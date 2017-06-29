// Fill out your copyright notice in the Description page of Project Settings.

#include "FlytekVoiceSDKPrivatePCH.h"
#include "SpeechRecognizeTask.h"

template <typename T>
FSpeechRecognizeTask::FSpeechRecognizeTask(T *InObjectPtr, int32(T::*InFuncPtr)(const FString& UserName, const FString& Password, const FString& Params), const FString& InThreadName, const FString& InUserName, const FString& InPassword, const FString& InParams)
{
	InitLoginTask(InObjectPtr, InFuncPtr, InThreadName, InUserName, InPassword, InParams);	
}
template <typename T>
FSpeechRecognizeTask::FSpeechRecognizeTask(T *InObjectPtr, int32(T::*InFuncPtr)(), const FString& InThreadName)
{
	InitFuncTask(InObjectPtr, InFuncPtr, InThreadName);
}