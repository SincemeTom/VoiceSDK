// Copyright 2016 Gamemakin LLC. All Rights Reserved.


#include "FlytekVoiceSDK.h"
#include "Paths.h"
#include "IPluginManager.h"
#include "WindowsPlatformProcess.h"
#include "MessageDialog.h"
#include "SpeechRecognizer.h"
//#include "speech_recognizer.h"





class FlytekVoiceSDK;
IFlytekVoiceSDK *gFFlytexSDK = nullptr;

#define LOCTEXT_NAMESPACE "FFlytekVoiceSDKModule"


FFlytekVoiceSDKModule::FFlytekVoiceSDKModule()
{
	gFFlytexSDK = this;
}
FFlytekVoiceSDKModule::~FFlytekVoiceSDKModule()
{
	gFFlytexSDK = nullptr;
	ResetSpeechRecPtr();
}
void FFlytekVoiceSDKModule::StartupModule()
{
	if (IsAvailable())
	{
		UE_LOG(LogFlytekVoiceSDK, Log, TEXT("%s"), TEXT("FlytekVoice Module Started"));
	}
#if 1
	// Get the base directory of this plugin
	FString BaseDir = IPluginManager::Get().FindPlugin("FlytekVoiceSDK")->GetBaseDir();

	// Add on the relative location of the third party dll and load it
	FString MSCLibraryPath;
#if PLATFORM_WINDOWS
#if PLATFORM_64BITS
	MSCLibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/IFlytekSDK/Win64/msc_x64.dll"));
#else
	MSCLibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/IFlytekSDK/Win64/msc_x64.dll"));
#endif
#endif


	// Add on the relative location of the third party dll and load it
	FString DllLibraryPath;
#if PLATFORM_WINDOWS
#if PLATFORM_64BITS
	
	//DllLibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/IFlytekSDK/Win64/iat_record_sample.dll"));
#else
	//DllLibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/IFlytekSDK/Win64/iat_record_sample.dll"));
#endif
#endif

	DllHandle = !MSCLibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*MSCLibraryPath) : nullptr;

	if (DllHandle)
	{
		// Call the test function in the third party library that opens a message box
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("ThirdPartyLibraryError", "Failed to load iat_record_sample third party library"));
	}

#endif	
}

void FFlytekVoiceSDKModule::ShutdownModule()
{
	if (DllHandle)
	{
		FPlatformProcess::FreeDllHandle(DllHandle);
	}
	DllHandle = nullptr;
	//SpeechRecPtr->ConditionalBeginDestroy();
	SpeechRecPtr = nullptr;
	if (IsAvailable())
	{
		UE_LOG(LogFlytekVoiceSDK, Log, TEXT("%s"), TEXT("FlytekVoice Module Shutdown"));
	}
}
USpeechRecognizer* FFlytekVoiceSDKModule::InitializeSpeechRecognize(ESpeechLanguage InLanguage)
{
	if (!SpeechRecPtr)
	{
		SpeechRecPtr = NewObject<USpeechRecognizer>();	
		SpeechRecPtr->SetParams(InLanguage);
		SpeechRecPtr->AddToRoot();
		SpeechRecPtr->SpeechRecLoginRequest(FString(), FString(), FString());
	}
	else 
	{
		SpeechRecPtr->SetParams(InLanguage);
		if (!SpeechRecPtr->bLoginSuccessful)
		{
			SpeechRecPtr->SpeechRecLoginRequest(FString(), FString(), FString());
		}
		else
		{
			SpeechRecPtr->SpeechRecInitRequest();
		}
	}
	return SpeechRecPtr;
}

void FFlytekVoiceSDKModule::VoiceSDKLogin(const FString& UserName, const FString& Password, const FString& Params)
{
	if (SpeechRecPtr && !SpeechRecPtr->IsPendingKill())
	{
		SpeechRecPtr->SpeechRecLoginRequest(FString(), FString(), FString());
	}
	else
	{
		SpeechRecPtr = NewObject<USpeechRecognizer>();
		SpeechRecPtr->SpeechRecLoginRequest(FString(), FString(), FString());
	}
	
}
void FFlytekVoiceSDKModule::VoiceSDKLogout()
{
	if (SpeechRecPtr && !SpeechRecPtr->IsPendingKill())
	{
		SpeechRecPtr->SpeechRecLogoutRequest();
	}
}

int32 FFlytekVoiceSDKModule::SpeechRecInit()
{
	if (SpeechRecPtr && !SpeechRecPtr->IsPendingKill())
	{
		SpeechRecPtr->SpeechRecInitRequest();
	}
	return 0;
}
void FFlytekVoiceSDKModule::SpeechRecUninit()
{
	if (SpeechRecPtr && !SpeechRecPtr->IsPendingKill())
	{
		SpeechRecPtr->SpeechRecUninitRequest();
	}

}
void FFlytekVoiceSDKModule::SpeechRecStartListening()
{
	if (SpeechRecPtr && !SpeechRecPtr->IsPendingKill())
	{
		SpeechRecPtr->SpeechRecStartListeningRequest();
	}
}

void FFlytekVoiceSDKModule::SpeechRecStopListening()
{
	if (SpeechRecPtr && !SpeechRecPtr->IsPendingKill())
	{
		SpeechRecPtr->SpeechRecStopListeningRequest();
	}
}
int32 FFlytekVoiceSDKModule::SpeechRecWriteAudioData()
{
	char data;
	int len = 0;
	return sr_write_audio_data(&SpeechRec, &data, len);
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FFlytekVoiceSDKModule, FlytekVoiceSDK)
DEFINE_LOG_CATEGORY(LogFlytekVoiceSDK);