// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Runnable.h"
/**
 * 
 */
class TreadClass :public FRunnable
{
public:
	TreadClass();
	~TreadClass();

	virtual uint32 Run() override;

	virtual void Stop() override;

	virtual void Exit() override;
}; 
