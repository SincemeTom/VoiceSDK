// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Runnable.h"
#include "RunnableThread.h"
/**
 * 
 */


enum EThreadState
{
	ES_NULL,
	ES_LOGIN,
	ES_LOGOUT,
	ES_INIT,
	ES_UNINIT,
	ES_STARTLISTENING,
	ES_STOPLISTENING
};

class FThreadClass : public FRunnable
{
public:
	FThreadClass();

	~FThreadClass();

	template <typename T>
	FThreadClass(T *InObjectPtr, void(T::*InFuncPtr)(void), const FString& ThreadName, EThreadState InState) {

		InitSpeechInitThread(InObjectPtr, InFuncPtr, ThreadName, InState);
	}

	template <typename T>
	FThreadClass(T *InObjectPtr, int32(T::*InFuncPtr)(const FString& UserName, const FString& Password, const FString& Params), const FString& ThreadName, const FString& InUserName, const FString& InPassword, const FString& InParams, EThreadState InState) {

		InitLoginThread(InObjectPtr, InFuncPtr, InUserName, ThreadName, InPassword, InParams, InState);
	}

	virtual uint32 Run() override;

	virtual void Stop() override;

	virtual void Exit() override;

	template <typename T>
	void InitLoginThread(T *InObjectPtr, int32(T::*InFuncPtr)(const FString& UserName, const FString& Password, const FString& Params), const FString& ThreadName, const FString& InUserName, const FString& InPassword, const FString& InParams, EThreadState InState)
	{
		this->ObjectPtr = (UObject *)InObjectPtr;
		this->LoginFunctionPtr = (int32(UObject::*)(const FString& UserName, const FString& Password, const FString& Params))InFuncPtr;
		RunnableThread = FRunnableThread::Create(this, *ThreadName, 0, EThreadPriority::TPri_Lowest);
		UserName = InUserName;
		Password = InPassword;
		Params = InParams;
		SetThreadState(InState);
	}

	template <typename T>
	void InitLogoutThread(T *InObjectPtr, void(T::*InFuncPtr)(void), const FString& ThreadName, EThreadState InState) {

		this->ObjectPtr = (UObject *)InObjectPtr;
		this->LogoutFunctionPtr = (void(UObject::*)(void))InFuncPtr;
		RunnableThread = FRunnableThread::Create(this, *ThreadName, 0, EThreadPriority::TPri_Lowest);
		SetThreadState(InState);
	}

	template <typename T>
	void InitSpeechInitThread(T *InObjectPtr, void(T::*InFuncPtr)(void), const FString& ThreadName, EThreadState InState) {

		this->ObjectPtr = (UObject *)InObjectPtr;
		this->InitFunctionPtr = (void(UObject::*)(void))InFuncPtr;
		RunnableThread = FRunnableThread::Create(this, *ThreadName, 0, EThreadPriority::TPri_Lowest);
		SetThreadState(InState);
	}

	void Reset()
	{
		SetThreadState(ES_NULL);
		ObjectPtr = nullptr;
		InitFunctionPtr = nullptr;
		LogoutFunctionPtr = nullptr;
		LoginFunctionPtr = nullptr;
		RunnableThread = nullptr;
	}
	void SetThreadState(EThreadState InState) { State = InState; }
	EThreadState GetThreadState() const { return State; }
public:
	UObject *ObjectPtr = nullptr;
	FRunnableThread* RunnableThread;
private:
	EThreadState State = ES_NULL;

	void(UObject::*InitFunctionPtr)(void) = nullptr;
	void(UObject::*LogoutFunctionPtr)(void) = nullptr;
	int32(UObject::*LoginFunctionPtr)(const FString& UserName, const FString& Password, const FString& Params) = nullptr;
	FString UserName;
	FString Password;
	FString Params;
}; 


