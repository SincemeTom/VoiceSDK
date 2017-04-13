// Fill out your copyright notice in the Description page of Project Settings.

#include "ThreadClass.h"
#include "FlytekVoiceSDK.h"


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
