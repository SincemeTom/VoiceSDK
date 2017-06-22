// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SpeechRecognizer.h"
#include "TaskGraphInterfaces.h"
/**
 * 
 */

class FSpeechRecognizeTask
{
public:
	template <typename T>
	FSpeechRecognizeTask(T *InObjectPtr, int32(T::*InFuncPtr)(const FString& UserName, const FString& Password, const FString& Params), const FString& InThreadName, const FString& InUserName, const FString& InPassword, const FString& InParams);

	template <typename T>
	FSpeechRecognizeTask(T *InObjectPtr, int32(T::*InFuncPtr)(), const FString& InThreadName);

	const TCHAR* GetTaskName()
	{
		return *ThreadName;
	}
	static ENamedThreads::Type GetDesiredThread()
	{
		return ENamedThreads::AnyThread;
	}

	FORCEINLINE static TStatId GetStatId()
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FTaskGraphTestTask, STATGROUP_TaskGraphTasks);
	}

	static ESubsequentsMode::Type GetSubsequentsMode()
	{
		return ESubsequentsMode::TrackSubsequents;
	}

	void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
	{
		//FPlatformAtomics::InterlockedIncrement(&TaskGraphTestTask::CompletedTasks);
		//ErrorCode = 
		if (LoginFunctionPtr)
		{
			ErrorCode = (this->ObjectPtr->*this->LoginFunctionPtr)(UserName, Password, Params);
		}
		if (InitFunctionPtr)
		{
			ErrorCode = (this->ObjectPtr->*this->InitFunctionPtr)();
		}
	}
	template <typename T>
	void InitLoginTask(T *InObjectPtr, int32(T::*InFuncPtr)(const FString& UserName, const FString& Password, const FString& Params), const FString& InThreadName, const FString& InUserName, const FString& InPassword, const FString& InParams)
	{
		ObjectPtr = InObjectPtr;
		LoginFunctionPtr = InFuncPtr;
		ThreadName = InThreadName;
		UserName = InUserName;
		Password = InPassword;
		Params = InParams;
	}

	template <typename T>
	void InitFuncTask(T *InObjectPtr, int32(T::*InFuncPtr)(), const FString& InThreadName)
	{
		ObjectPtr = InObjectPtr;
		InitFunctionPtr = InFuncPtr;
		ThreadName = InThreadName;
	}
public:
	int32 ErrorCode = 0;
protected:
private:
	USpeechRecognizer *ObjectPtr = nullptr;
	EThreadState State = ES_NULL;
	int32(USpeechRecognizer::*InitFunctionPtr)() = nullptr;
	int32(USpeechRecognizer::*LoginFunctionPtr)(const FString& UserName, const FString& Password, const FString& Params) = nullptr;
	FString ThreadName;
	FString UserName;
	FString Password;
	FString Params;
	int16 Index;
};