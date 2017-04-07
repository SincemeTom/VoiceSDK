// Copyright 2016 Gamemakin LLC. All Rights Reserved.

#pragma once

#include "ModuleManager.h"
#include "ModuleInterface.h"

DECLARE_LOG_CATEGORY_EXTERN(LogFlytekVoiceSDK, Verbose, All);




class IFlytekVoiceSDK : public IModuleInterface
{

public:

	static FName GetModularFeatureName()
	{
		static FName FeatureName = FName(TEXT("FlytekVoiceSDK"));
		return FeatureName;
	}

	/**
	* Singleton-like access to this module's interface.  This is just for convenience!
	* Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	*
	* @return Returns singleton instance, loading the module on demand if needed
	*/
	static inline IFlytekVoiceSDK& Get()
	{
		return FModuleManager::LoadModuleChecked< IFlytekVoiceSDK >("FlytekVoiceSDK");
	}

	/**
	* Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	*
	* @return True if the module is loaded and ready to use
	*/
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("FlytekVoiceSDK");
	}
	//int MSPAPI MSPLogin(const char* usr, const char* pwd, const char* params);
	virtual void VoiceSDKLogin(const FString& UserName, const FString& Password, const FString& Params) = 0;
};
class FFlytekVoiceSDKModule : public IFlytekVoiceSDK
{
public:
	FFlytekVoiceSDKModule();
	~FFlytekVoiceSDKModule();
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	virtual void VoiceSDKLogin(const FString& UserName, const FString& Password, const FString& Params) override;
private:
	void* DllHandle;
};