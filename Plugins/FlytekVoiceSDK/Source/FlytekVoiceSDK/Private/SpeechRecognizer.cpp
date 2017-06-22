// Fill out your copyright notice in the Description page of Project Settings.

#include "SpeechRecognizer.h"
#include "FlytekVoiceSDK.h"
#include "ThreadClass.h"
#include "ScopeLock.h"
#include "SpeechRecognizeTask.h"


const FString Session_Begin_Param = TEXT("sub = iat, domain = iat, language = zh_cn, accent = mandarin, sample_rate = 16000, result_type = plain, result_encoding = utf-8");

const FString LoginParam = TEXT("appid = 58d087b0, work_dir = .");
//DEFINE_LOG_CATEGORY(LogFlytekVoiceSDK);

#define SPEECH_THREAD TEXT("SpeechThread")

//const FString Session_Begin_Params = TEXT("sub = iat, domain = iat, language = zh_cn, accent = mandarin, sample_rate = 16000, result_type = plain, result_encoding = utf-8");

USpeechRecognizer* pSpeechRecognizer = nullptr;

extern "C"
{
	void OnSpeechResult(const char* result, char is_last)
	{
		if (pSpeechRecognizer)
		{
			pSpeechRecognizer->OnSpeechRecResult(result, is_last);
		}	
	}

	void OnSpeechBeginResult()
	{
		if (pSpeechRecognizer)
		{
			pSpeechRecognizer->OnSpeechRecBegin();
		}
	}

	void OnSpeechEndResult(int reason)
	{
		if (pSpeechRecognizer)
		{
			pSpeechRecognizer->OnSpeechRecEnd(reason);
		}
	}
}



USpeechRecognizer::USpeechRecognizer()
{
	for (int32 i = 0; i <= ES_MAXSTATE; ++i)
	{
		ErrorResult[i] = -1;
	}
	pSpeechRecognizer = this;
	bLoginSuccessful = false;
	SpeechThread = nullptr;// MakeShareable(new FThreadClass());
}
USpeechRecognizer::~USpeechRecognizer()
{
	//SpeechThread->Exit();
	SpeechThread = nullptr;
}

void USpeechRecognizer::Tick(float DeltaTime)
{
	for (int16 i = EThreadState::ES_NULL; i<EThreadState::ES_MAXSTATE;i++)
	{
		if (SpeechRecognizeCompletion[i] && SpeechRecognizeCompletion[i]->IsComplete())
		{	
			if (!bLoginSuccessful && i == EThreadState::ES_LOGIN)
			{
				UE_LOG(LogFlytekVoiceSDK, Log, TEXT("VoiceSDKLogin Completed ! Error code : %d"), ErrorResult[ES_LOGIN]);
				if (ErrorResult[ES_LOGIN] == 0)
				{
					bLoginSuccessful = true;
				}	
			}
			else if (!bInitSuccessful && i == EThreadState::ES_INIT)
			{
				UE_LOG(LogFlytekVoiceSDK, Log, TEXT("VoiceSDK Init Completed ! Error code : %d"), ErrorResult[ES_INIT]);
				if (ErrorResult[ES_INIT] == 0)
				{
					bInitSuccessful = true;
				}
			}
			//SpeechRecognizeCompletion[i]->Release();
			//TaskArray[i]
		}
	}
	


	/*
	FScopeLock ScopeLock1(&AccessLock);
	if (SpeechThread.IsValid())
	{
		//if (ErrorResult != -1)
		//{
			//switch (SpeechThread->GetThreadState())
			//{
			//case ES_NULL:
			//	break;
			//case ES_LOGIN:
				if (ErrorResult[ES_LOGIN] == 0)
				{
					UE_LOG(LogFlytekVoiceSDK, Log, TEXT("VoiceSDKLogin Successful ! "))
					bLoginSuccessful = true;
					HandleOnLoginResult();			
				}
				else if (ErrorResult[ES_LOGIN] != -1)
				{
					bLoginSuccessful = false;
					UE_LOG(LogFlytekVoiceSDK, Error, TEXT("VoiceSDKLogin Faild ! Error code : %d"), ErrorResult[ES_LOGIN])
				}		
				ErrorResult[ES_LOGIN] = -1;
			//	break;
		//	case ES_LOGOUT:
				if (ErrorResult[ES_LOGOUT] == 0)
				{
					UE_LOG(LogFlytekVoiceSDK, Log, TEXT("VoiceSDKLogout Successful ! "))
				}
				else if (ErrorResult[ES_LOGOUT] != -1)
				{
					UE_LOG(LogFlytekVoiceSDK, Error, TEXT("VoiceSDKLogout Faild ! Error code : %d"), ErrorResult[ES_LOGOUT])
				}
				ErrorResult[ES_LOGOUT] = -1;
			//	break;
			//case ES_INIT:
				if (ErrorResult[ES_INIT] == 0)
				{
					UE_LOG(LogFlytekVoiceSDK, Log, TEXT("VoiceSDK Init Successful ! "))
					bInitSuccessful = true;
					
				}
				else if (ErrorResult[ES_INIT] != -1)
				{
					UE_LOG(LogFlytekVoiceSDK, Error, TEXT("VoiceSDK Init Faild ! Error code : %d"), ErrorResult[ES_INIT])
					bInitSuccessful = false;					
				}
				ErrorResult[ES_INIT] = -1;
			//	break;
			//case ES_UNINIT:		
			//	break;
		//	case ES_STARTLISTENING:
				if (ErrorResult[ES_STARTLISTENING] == 0)
				{
					UE_LOG(LogFlytekVoiceSDK, Log, TEXT("Start Speech!"))
				}
				else if (ErrorResult[ES_STARTLISTENING] != -1)
				{
					UE_LOG(LogFlytekVoiceSDK, Log, TEXT("Start Speech faild! Error Code :%d"), ErrorResult[ES_STARTLISTENING])
				}
				ErrorResult[ES_STARTLISTENING] = -1;
			//	break;
			//case ES_STOPLISTENING:
				if (ErrorResult[ES_STOPLISTENING] == 0)
				{
					UE_LOG(LogFlytekVoiceSDK, Log, TEXT("Stop Speech!"))
				}
				else if(ErrorResult[ES_STOPLISTENING] != -1)
				{
					UE_LOG(LogFlytekVoiceSDK, Log, TEXT("Stop Speech faild! Error Code :%d"), ErrorResult[ES_STOPLISTENING])
				}
				ErrorResult[ES_STOPLISTENING] = -1;
			//	break;
			//default:
		//		break;
			//}
		//	SpeechThread->SetThreadState(ES_NULL);
			
		//}
	}
	*/
}
bool USpeechRecognizer::IsTickable() const
{
	return true;
}

TStatId USpeechRecognizer::GetStatId() const
{
	return TStatId();
}

void USpeechRecognizer::HandleOnLoginResult()
{
	UE_LOG(LogFlytekVoiceSDK, Log, TEXT("OnLoginCallback ! "))
	SpeechRecInitRequest();
}
void USpeechRecognizer::SpeechRecLoginRequest(const FString& UserName, const FString& Password, const FString& Params)
{
	if (bLoginSuccessful)
	{
		UE_LOG(LogFlytekVoiceSDK, Log, TEXT("VoiceSDK has already Login Successful ! "))
		return;
	}
	SpeechRecognizeCompletion[EThreadState::ES_LOGIN] = TGraphTask<FSpeechRecognizeTask>::CreateTask(NULL, ENamedThreads::GameThread).ConstructAndDispatchWhenReady(this, &USpeechRecognizer::CallSRLogin, SPEECH_THREAD, UserName, Password, Params);
}

void USpeechRecognizer::SpeechRecLogoutRequest()
{

}
void USpeechRecognizer::SpeechRecInitRequest()
{
	if (bInitSuccessful)
	{
		UE_LOG(LogFlytekVoiceSDK, Log, TEXT("VoiceSDK has already Init Successful ! "));
		return;
	}
	SpeechRecognizeCompletion[EThreadState::ES_INIT] = TGraphTask<FSpeechRecognizeTask>::CreateTask(NULL, ENamedThreads::GameThread).ConstructAndDispatchWhenReady(this, &USpeechRecognizer::CallSRInit, SPEECH_THREAD);
}
void USpeechRecognizer::SpeechRecUninitRequest()
{
	if (SpeechThread.IsValid())
	{
		SpeechThread->Reset();
		SpeechThread->InitSpeechInitThread(this, &USpeechRecognizer::CallSRUninit, SPEECH_THREAD, ES_UNINIT);
		SpeechThread->Run();
	
	}
	else
	{
		//SpeechThread =new FThreadClass(this, &USpeechRecognizer::CallSRUninit, SPEECH_THREAD, ES_UNINIT);
		//SpeechThread->Run();
	}
}

void USpeechRecognizer::SpeechRecStartListeningRequest()
{
	if (SpeechThread.IsValid())
	{
		SpeechThread->Reset();
		SpeechThread->InitSpeechInitThread(this, &USpeechRecognizer::CallSRStartListening, SPEECH_THREAD, ES_STARTLISTENING);
		SpeechThread->Run();
	}
	else
	{
		//SpeechThread = MakeShareable(new FThreadClass(this, &USpeechRecognizer::CallSRStartListening, SPEECH_THREAD, ES_STARTLISTENING));
		//SpeechThread->Run();
	}
}

void USpeechRecognizer::SpeechRecStopListeningRequest()
{
	if (SpeechThread.IsValid())
	{
		SpeechThread->Reset();
		SpeechThread->InitSpeechInitThread(this, &USpeechRecognizer::CallSRStopListening, SPEECH_THREAD, ES_STOPLISTENING);
		SpeechThread->Run();
	}
	else
	{
		//SpeechThread = new FThreadClass(this, &USpeechRecognizer::CallSRStopListening, SPEECH_THREAD, ES_STOPLISTENING);
		//SpeechThread->Run();
	}
}
int32 USpeechRecognizer::CallSRLogin(const FString& UserName, const FString& Password, const FString& Params)
{
	//FScopeLock ScopeLock1(&AccessLock);
	ErrorResult[ES_LOGIN] = sr_login(TCHAR_TO_ANSI(*UserName), TCHAR_TO_ANSI(*Password), TCHAR_TO_ANSI(*LoginParam));
	return ErrorResult[ES_LOGIN];
}
void USpeechRecognizer::CallSRLogout()
{
	//FScopeLock ScopeLock1(&AccessLock);
	ErrorResult[ES_LOGOUT] = sr_logout();
}

int32 USpeechRecognizer::CallSRInit()
{
	//FScopeLock ScopeLock1(&AccessLock);
	RecNotifier = {
		OnSpeechResult,
		OnSpeechBeginResult,
		OnSpeechEndResult
	};
	
	 ErrorResult[ES_INIT] = sr_init(&SpeechRec, TCHAR_TO_ANSI(*Session_Begin_Param), SR_MIC, DEFAULT_INPUT_DEVID, &RecNotifier);
	 UE_LOG(LogFlytekVoiceSDK, Log, TEXT("Init! Error Code :%d"), ErrorResult[ES_INIT]);
	 return ErrorResult[ES_INIT];
}
void USpeechRecognizer::CallSRUninit()
{
	//FScopeLock ScopeLock1(&AccessLock);
	if (bInitSuccessful)
	{
		sr_uninit(&SpeechRec);
	}
	bInitSuccessful = false;
}

void USpeechRecognizer::CallSRStartListening()
{
	//FScopeLock ScopeLock1(&AccessLock);
	if (bInitSuccessful)
	{
		ErrorResult[ES_STARTLISTENING] = sr_start_listening(&SpeechRec);
		UE_LOG(LogFlytekVoiceSDK, Log, TEXT("Start Recognizer Code :%d"), ErrorResult[ES_STARTLISTENING])
	}
	else
	{
		UE_LOG(LogFlytekVoiceSDK, Log, TEXT("Speek Recognizer uninitialized"))
	}
}

void USpeechRecognizer::CallSRStopListening()
{
	//FScopeLock ScopeLock1(&AccessLock);
	if (bInitSuccessful)
	{
		ErrorResult[ES_STOPLISTENING] = sr_stop_listening(&SpeechRec);
		UE_LOG(LogFlytekVoiceSDK, Log, TEXT("Stop Recognizer Code :%d"), ErrorResult[ES_STOPLISTENING])
	}
	else
	{
		UE_LOG(LogFlytekVoiceSDK, Log, TEXT("Speek Recognizer uninitialized"))
	}
}
void USpeechRecognizer::OnSpeechRecResult(const char* result, char is_last)
{
	FString SpeechResultStr = UTF8_TO_TCHAR(result);
	if (bSpeeking)
	{
		if (is_last)
		{
			bSpeeking = false;
			UE_LOG(LogFlytekVoiceSDK, Log, TEXT("Speeking Last words"), *SpeechResultStr)

		}
		UE_LOG(LogFlytekVoiceSDK, Log, TEXT("Speeking String :[%s]"), *SpeechResultStr)
	}
}
void USpeechRecognizer::OnSpeechRecBegin()
{
	bSpeeking = true;
	UE_LOG(LogFlytekVoiceSDK, Log, TEXT("OnSpeechRecBegin"))
}
void USpeechRecognizer::OnSpeechRecEnd(int reason)
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

