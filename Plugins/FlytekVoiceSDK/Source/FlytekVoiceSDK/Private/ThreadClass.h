// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Runnable.h"
#include "RunnableThread.h"
#include "SpeechRecognizer.h"
/**
 * 
 */



class FThreadClass : public FRunnable
{
public:
	FThreadClass();

	~FThreadClass();

	template <typename T>
	FThreadClass(T *InObjectPtr, void(T::*InFuncPtr)(void), const FString& InThreadName, EThreadState InState) {

		InitSpeechInitThread(InObjectPtr, InFuncPtr, InThreadName, InState);
	}

	template <typename T>
	FThreadClass(T *InObjectPtr, int32(T::*InFuncPtr)(const FString& UserName, const FString& Password, const FString& Params), const FString& ThreadName, const FString& InUserName, const FString& InPassword, const FString& InParams, EThreadState InState) {

		InitLoginThread(InObjectPtr, InFuncPtr, ThreadName, InUserName, InPassword, InParams, InState);
	}

	virtual uint32 Run() override;

	virtual void Stop() override;

	virtual void Exit() override;

	template <typename T>
	void InitLoginThread(T *InObjectPtr, int32(T::*InFuncPtr)(const FString& UserName, const FString& Password, const FString& Params), const FString& ThreadName, const FString& InUserName, const FString& InPassword, const FString& InParams, EThreadState InState);

	template <typename T>
	void InitSpeechInitThread(T *InObjectPtr, void(T::*InFuncPtr)(void), const FString& ThreadName, EThreadState InState); 


	void Reset()
	{
		//RunnableThread->Suspend()
		SetThreadState(ES_NULL);
		ObjectPtr = nullptr;
		InitFunctionPtr = nullptr;
		LoginFunctionPtr = nullptr;
		//RunnableThread = nullptr;
	}
	void SetThreadState(EThreadState InState) { State = InState; }
	EThreadState GetThreadState() const { return State; }
public:
	UObject *ObjectPtr = nullptr;
	FRunnableThread* RunnableThread;
private:
	EThreadState State = ES_NULL;

	void(UObject::*InitFunctionPtr)(void) = nullptr;
	int32(UObject::*LoginFunctionPtr)(const FString& UserName, const FString& Password, const FString& Params) = nullptr;
	FString UserName;
	FString Password;
	FString Params;
}; 


