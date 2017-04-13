// Copyright 2016 Gamemakin LLC. All Rights Reserved.


#include "FlytekVoiceSDK.h"
#include "Paths.h"
#include "ThreadClass.h"
#include "IPluginManager.h"
#include "WindowsPlatformProcess.h"
#include "MessageDialog.h"
#include "SpeechRecognizer.h"
//#include "speech_recognizer.h"




const FString LoginParams = TEXT("appid = 58d087b0, work_dir = .");
const FString Session_Begin_Params = TEXT("sub = iat, domain = iat, language = zh_cn, accent = mandarin, sample_rate = 16000, result_type = plain, result_encoding = utf-8");

class FlytekVoiceSDK;
IFlytekVoiceSDK *gFFlytexSDK = nullptr;

#define LOCTEXT_NAMESPACE "FFlytekVoiceSDKModule"

extern "C"
{
	void OnResult(const char* result, char is_last)
	{
		if (gFFlytexSDK)
		{
			gFFlytexSDK->OnSpeechRecResult(result, is_last);
		}
	}

	void OnSpeechBegin()
	{
		if (gFFlytexSDK)
		{
			gFFlytexSDK->OnSpeechRecBegin();
		}
	}

	void OnSpeechEnd(int reason)
	{
		if (gFFlytexSDK)
		{
			gFFlytexSDK->OnSpeechRecEnd(reason);
		}
	}
}


FFlytekVoiceSDKModule::FFlytekVoiceSDKModule()
{
	gFFlytexSDK = this;
}
FFlytekVoiceSDKModule::~FFlytekVoiceSDKModule()
{

}
void FFlytekVoiceSDKModule::StartupModule()
{
	if (IsAvailable())
	{
		UE_LOG(LogFlytekVoiceSDK, Log, TEXT("%s"), TEXT("Module Started"));
	}
#if 1
	// Get the base directory of this plugin
	FString BaseDir = IPluginManager::Get().FindPlugin("FlytekVoiceSDK")->GetBaseDir();

	// Add on the relative location of the third party dll and load it
	FString LibraryPath;
#if PLATFORM_WINDOWS
#if PLATFORM_64BITS
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/IFlytekSDK/Win64/msc_x64.dll"));
#else
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/IFlytekSDK/Win64/msc_x64.dll"));
#endif
#endif

	/*DllHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;

	if (DllHandle)
	{
		// Call the test function in the third party library that opens a message box
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("ThirdPartyLibraryError", "Failed to load voice sdk third party library"));
	}
	*/

	// Add on the relative location of the third party dll and load it
	FString DllLibraryPath;
#if PLATFORM_WINDOWS
#if PLATFORM_64BITS
	
	DllLibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/IFlytekSDK/Win64/iat_record_sample.dll"));
#else
	DllLibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/IFlytekSDK/Win64/iat_record_sample.dll"));
#endif
#endif

	DllHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;

	if (DllHandle)
	{
		// Call the test function in the third party library that opens a message box
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("ThirdPartyLibraryError", "Failed to load iat_record_sample third party library"));
	}

#endif	
	/*VoiceSDKLogin(FString(), FString(), LoginParams);
	if (bLoginSuccessful)
	{
		auto error = SpeechRecInit();
		if (error == 0)
		{
			bInitSuccessful = true;
			UE_LOG(LogFlytekVoiceSDK, Log, TEXT("Speech recognizer init successful ! "))
		}
		else
		{
			bInitSuccessful = false;
			UE_LOG(LogFlytekVoiceSDK, Log, TEXT("Speech recognizer init faild ! ErrorCode : %d"), error)
		}
	}*/
	
}

void FFlytekVoiceSDKModule::ShutdownModule()
{
	if (DllHandle)
	{
		FPlatformProcess::FreeDllHandle(DllHandle);
	}
	DllHandle = nullptr;
	//SpeechRecUninit();
	VoiceSDKLogout();

}

void FFlytekVoiceSDKModule::VoiceSDKLogin(const FString& UserName, const FString& Password, const FString& Params)
{
	USpeechRecognizer* SpeechRecObj = NewObject<USpeechRecognizer>();
	SpeechRecObj->SpeechRecLoginRequest(FString(), FString(), LoginParams);
	//TCHAR_TO_ANSI
	/*
	auto Result = sr_login(TCHAR_TO_ANSI(*UserName), TCHAR_TO_ANSI(*Password), TCHAR_TO_ANSI(*Params));
	if (Result == 0)
	{
		bLoginSuccessful = true;
		UE_LOG(LogFlytekVoiceSDK, Log, TEXT("VoiceSDKLogin Successful ! "))
	}
	else
	{
		bLoginSuccessful = false;
		UE_LOG(LogFlytekVoiceSDK, Error, TEXT("VoiceSDKLogin Faild ! Error code : %d"), Result)
	}*/
	
}
void FFlytekVoiceSDKModule::VoiceSDKLogout()
{
	auto Result = sr_logout();
	if (Result == 0)
	{
		UE_LOG(LogFlytekVoiceSDK, Log, TEXT("VoiceSDKLogout Successful ! "))
	}
	else
	{
		UE_LOG(LogFlytekVoiceSDK, Error, TEXT("VoiceSDKLogout Faild ! Error code : %d"), Result)
	}
}

int32 FFlytekVoiceSDKModule::SpeechRecInit()
{
	RecNotifier = {
		OnResult,
		OnSpeechBegin,
		OnSpeechEnd
	};
	int32 ErrorCode = sr_init(&SpeechRec, TCHAR_TO_ANSI(*Session_Begin_Params), SR_MIC, DEFAULT_INPUT_DEVID, &RecNotifier);
	return ErrorCode;
}
void FFlytekVoiceSDKModule::SpeechRecUninit()
{
	if (bInitSuccessful)
	{
		sr_uninit(&SpeechRec);
	}
	bInitSuccessful = false;
}
void FFlytekVoiceSDKModule::SpeechRecStartListening()
{
//	ThreadManager::Create(this, &FFlytekVoiceSDKModule::StartListening, TEXT("StartListeningThread"));

	if (bInitSuccessful)
	{
		int32 ErrorCode = sr_start_listening(&SpeechRec);
		if (ErrorCode == 0)
		{
			UE_LOG(LogFlytekVoiceSDK, Log, TEXT("Start Speech!"))
		}
		else
		{
			UE_LOG(LogFlytekVoiceSDK, Log, TEXT("Start Speech faild! Error Code :%d"),ErrorCode)
		}
	}
	else
	{
		UE_LOG(LogFlytekVoiceSDK, Log, TEXT("Speek Recognizer uninitialized"))
	}
}

void FFlytekVoiceSDKModule::SpeechRecStopListening()
{
	if (bInitSuccessful)
	{
		int32 ErrorCode = sr_stop_listening(&SpeechRec);
		if (ErrorCode == 0)
		{
			UE_LOG(LogFlytekVoiceSDK, Log, TEXT("Stop Speech!"))
		}
		else
		{
			UE_LOG(LogFlytekVoiceSDK, Log, TEXT("Stop Speech faild! Error Code :%d"), ErrorCode)
		}		
	}
	else
	{
		UE_LOG(LogFlytekVoiceSDK, Log, TEXT("Speek Recognizer uninitialized"))
	}
}
int32 FFlytekVoiceSDKModule::SpeechRecWriteAudioData()
{
	char data;
	int len = 0;
	return sr_write_audio_data(&SpeechRec, &data, len);
}

void FFlytekVoiceSDKModule::OnSpeechRecResult(const char* result, char is_last)
{
	FString SpeechResultStr = UTF8_TO_TCHAR(result);
	if (bSpeeking)
	{
		CallbackResult.Broadcast(SpeechResultStr);
		if (is_last)
		{
			bSpeeking = false;
			UE_LOG(LogFlytekVoiceSDK, Log, TEXT("Speeking Last words"), *SpeechResultStr)
				
		}
		UE_LOG(LogFlytekVoiceSDK, Log, TEXT("Speeking String :[%s]"), *SpeechResultStr)
	}
}
void FFlytekVoiceSDKModule::OnSpeechRecBegin()
{
	bSpeeking = true;
	UE_LOG(LogFlytekVoiceSDK, Log, TEXT("OnSpeechRecBegin"))
}
void FFlytekVoiceSDKModule::OnSpeechRecEnd(int reason)
{
	if (reason == END_REASON_VAD_DETECT)
	{
		bSpeeking = false;
		UE_LOG(LogFlytekVoiceSDK, Log, TEXT("Speeking Done"))			
	}
	else
	{
		bSpeeking = false;
		UE_LOG(LogFlytekVoiceSDK, Error, TEXT("On Speech Recognizer Error : %d"), reason)
	}

}
void FFlytekVoiceSDKModule::StartListening()
{
	//return sr_start_listening(&SpeechRec);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FFlytekVoiceSDKModule, FlytekVoiceSDK)
DEFINE_LOG_CATEGORY(LogFlytekVoiceSDK);