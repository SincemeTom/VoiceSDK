// Copyright 2016 Gamemakin LLC. All Rights Reserved.

#pragma once

#include "ModuleManager.h"
#include "ModuleInterface.h"
#include "EngineMinimal.h"
#include "speech_recognizer.h"


struct speech_rec;
struct speech_rec_notifier;
DECLARE_LOG_CATEGORY_EXTERN(LogFlytekVoiceSDK, Verbose, All);
UENUM(BlueprintType)
enum class ESpeechLanguage : uint8
{
	EL_Chinese,
	EL_English
};
class IFlytekVoiceSDK : public IModuleInterface
{

public:

	static FName GetModularFeatureName()
	{
		static FName FeatureName = FName(TEXT("FlytekVoiceSDK"));
		return FeatureName;
	}
	virtual bool IsGameModule() const
	{
		return true;
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
	
	/**
	*Login voicesdk 
	*
	*/
	virtual void VoiceSDKLogin(const FString& UserName, const FString& Password, const FString& Params) = 0;

	virtual void VoiceSDKLogout() = 0;

	virtual int32 SpeechRecInit() = 0;

	virtual void SpeechRecStartListening() = 0;

	virtual void SpeechRecStopListening() = 0;

	virtual void SpeechRecUninit() = 0;

	virtual int32 SpeechRecWriteAudioData() = 0;


	virtual class USpeechRecognizer* InitializeSpeechRecognize(ESpeechLanguage InLanguage, bool IsContinuous) = 0;

	void ResetSpeechRecPtr() { SpeechRecPtr = nullptr; }

public:

	class USpeechRecognizer* SpeechRecPtr = nullptr;
private:
	
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

	virtual void VoiceSDKLogout() override;

	virtual int32 SpeechRecInit() override;

	virtual void SpeechRecStartListening() override;

	virtual void SpeechRecStopListening() override;

	virtual void SpeechRecUninit() override;

	virtual int32 SpeechRecWriteAudioData() override;

	virtual class USpeechRecognizer* InitializeSpeechRecognize(ESpeechLanguage InLanguage, bool IsContinuous) override;
private:
	//class USpeechRecognizer* SpeechRecPtr = nullptr;
	void* DllHandle;
	struct speech_rec SpeechRec;
	struct speech_rec_notifier RecNotifier;
	bool bLoginSuccessful;
	bool bInitSuccessful;
	bool bSpeeking;
};