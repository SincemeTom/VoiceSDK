// Fill out your copyright notice in the Description page of Project Settings.

#include "ThreadClass.h"


FThreadClass::FThreadClass()
{
}

FThreadClass::~FThreadClass()
{
	Reset();
}
uint32 FThreadClass::Run()
{
	if (LoginFunctionPtr)
	{
		(this->ObjectPtr->*this->LoginFunctionPtr)(UserName, Password, Params);
	}
	if (InitFunctionPtr)
	{
		(this->ObjectPtr->*this->InitFunctionPtr)();
	}
	return 0;
}

void FThreadClass::Stop()
{

}

void FThreadClass::Exit()
{

}
template <typename T>
void FThreadClass::InitSpeechInitThread(T *InObjectPtr, void(T::*InFuncPtr)(void), const FString& ThreadName, EThreadState InState) {

	this->ObjectPtr = (UObject *)InObjectPtr;
	this->InitFunctionPtr = (void(UObject::*)(void))InFuncPtr;
	SetThreadState(InState);
	if (!RunnableThread)
	{
		RunnableThread = FRunnableThread::Create(this, *ThreadName, 0, EThreadPriority::TPri_Lowest);
	}
}
template <typename T>
void FThreadClass::InitLoginThread(T *InObjectPtr, int32(T::*InFuncPtr)(const FString& UserName, const FString& Password, const FString& Params), const FString& ThreadName, const FString& InUserName, const FString& InPassword, const FString& InParams, EThreadState InState)
{
	this->ObjectPtr = (UObject *)InObjectPtr;
	this->LoginFunctionPtr = (int32(UObject::*)(const FString& UserName, const FString& Password, const FString& Params))InFuncPtr;
	UserName = InUserName;
	Password = InPassword;
	Params = InParams;
	SetThreadState(InState);
	if (!RunnableThread)
	{
		RunnableThread = FRunnableThread::Create(this, *ThreadName, 0, EThreadPriority::TPri_Lowest);
	}
}
