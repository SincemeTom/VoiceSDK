// Copyright 2016 Gamemakin LLC. All Rights Reserved.

#pragma once

#include "ModuleManager.h"
#include "ModuleInterface.h"
#include "speech_recognizer.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FSpeechRecognizeResultDelegate, const FString&);


struct speech_rec;
struct speech_rec_notifier;
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


	virtual void OnSpeechRecResult(const char* result, char is_last) = 0;
	virtual void OnSpeechRecBegin() = 0;
	virtual void OnSpeechRecEnd(int reason) = 0;

	/*
	int sr_logout();
	int sr_init(struct speech_rec * sr, const char * session_begin_params, enum sr_audsrc aud_src, int devid, struct speech_rec_notifier * notifier);
	int sr_start_listening(struct speech_rec *sr);
	int sr_stop_listening(struct speech_rec *sr);
	// only used for the manual write way. 
	int sr_write_audio_data(struct speech_rec *sr, char *data, unsigned int len);
	// must call uninit after you don't use it 
	void sr_uninit(struct speech_rec * sr);
	*/
public:
	FSpeechRecognizeResultDelegate CallbackResult;
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

	virtual void OnSpeechRecResult(const char* result, char is_last) override;
	virtual void OnSpeechRecBegin() override;
	virtual void OnSpeechRecEnd(int reason) override;

	virtual void StartListening();
private:
	void* DllHandle;
	struct speech_rec SpeechRec;
	struct speech_rec_notifier RecNotifier;
	bool bLoginSuccessful;
	bool bInitSuccessful;
	bool bSpeeking;
};