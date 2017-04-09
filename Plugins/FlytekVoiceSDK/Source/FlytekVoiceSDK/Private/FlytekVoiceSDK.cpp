// Copyright 2016 Gamemakin LLC. All Rights Reserved.


#include "FlytekVoiceSDK.h"
#include "Paths.h"
#include "IPluginManager.h"
#include "WindowsPlatformProcess.h"
#include "MessageDialog.h"
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
	VoiceSDKLogin(FString(), FString(), LoginParams);
}

void FFlytekVoiceSDKModule::ShutdownModule()
{
	if (DllHandle)
	{
		FPlatformProcess::FreeDllHandle(DllHandle);
	}
	DllHandle = nullptr;
	VoiceSDKLogout();
}

void FFlytekVoiceSDKModule::VoiceSDKLogin(const FString& UserName, const FString& Password, const FString& Params)
{
	//TCHAR_TO_ANSI
	
	auto Result = sr_login(TCHAR_TO_ANSI(*UserName), TCHAR_TO_ANSI(*Password), TCHAR_TO_ANSI(*Params));
	if (Result == 0)
	{
		UE_LOG(LogFlytekVoiceSDK, Log, TEXT("VoiceSDKLogin Successful ! "))
	}
	else
	{
		UE_LOG(LogFlytekVoiceSDK, Error, TEXT("VoiceSDKLogin Faild ! Error code : %d"), Result)
	}
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
	sr_uninit(&SpeechRec);
}
int32 FFlytekVoiceSDKModule::SpeechRecStartListening()
{
	int32 ErrorCode = sr_start_listening(&SpeechRec);
	return ErrorCode;
}

int32 FFlytekVoiceSDKModule::SpeechRecStopListening()
{
	return sr_stop_listening(&SpeechRec);
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
	if (is_last && bSpeeking)
	{
		UE_LOG(LogFlytekVoiceSDK, Log, TEXT("Speeking String :[%s]"), *SpeechResultStr)
	}
	else
	{
		UE_LOG(LogFlytekVoiceSDK, Log, TEXT("Speeking String :[%s]"), *SpeechResultStr)
	}
}
void FFlytekVoiceSDKModule::OnSpeechRecBegin()
{
	bSpeeking = true;
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
		UE_LOG(LogFlytekVoiceSDK, Error, TEXT("Recognizer Error : %d"), reason)
	}

	
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FFlytekVoiceSDKModule, FlytekVoiceSDK)
DEFINE_LOG_CATEGORY(LogFlytekVoiceSDK);