// Fill out your copyright notice in the Description page of Project Settings.

#include "SpeechRecognizer.h"
#include "FlytekVoiceSDK.h"
#include "ScopeLock.h"
#include "SpeechRecognizeTask.h"


#define  LANGUAGE_ENGLISH TEXT("en_us")
#define  LANGUAGE_CHINESE TEXT("zh_cn")

//Flytek appid
const FString LoginParam = TEXT("appid = 58d087b0, work_dir = .");

#define SPEECH_THREAD TEXT("SpeechThread")

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
	bContinuous = true;
	SRLanguage = ESpeechLanguage::EL_Chinese;
	Session_Begin_Param = TEXT("sub = iat, domain = iat, ptt = 0, language = zh_cn, accent = mandarin, sample_rate = 16000, result_type = plain, result_encoding = utf8, vad_enable  = 1, vad_eos = 1000");

	for (int32 i = 0; i <= ES_MAXSTATE; ++i)
	{
		ErrorResult[i] = -1;
	}
	pSpeechRecognizer = this;
	bLoginSuccessful = false;
	
}
USpeechRecognizer::~USpeechRecognizer()
{
	UE_LOG(LogFlytekVoiceSDK, Log, TEXT("SpeechRecognizer Destroyed"));
	IFlytekVoiceSDK::Get().ResetSpeechRecPtr();
	bLoginSuccessful = false;
	bInitSuccessful = false;
	bSpeeking = false;
	CallSRUninit();
	CallSRLogout();	
}
void USpeechRecognizer::ResetSpeechRecognizer()
{
	bSpeeking = false;
	bInitSuccessful = false;
	bOnSpeechRecBeginSuccesful = false;
	bOnSpeechRecEndSuccesful = false;
	bOnSpeechRecResultSuccesful = false;
	bContinuousSpeeking = false;
}
void USpeechRecognizer::PostInitProperties()
{
	Super::PostInitProperties();
}
void USpeechRecognizer::SetParams(ESpeechLanguage InLanguage, bool IsContinuous)
{
	ResetSpeechRecognizer();
	bContinuous = IsContinuous;
	SRLanguage = InLanguage;
	Session_Begin_Param = FString::Printf(TEXT("sub = iat, domain = iat, ptt = 0, language = %s, accent = mandarin, sample_rate = 16000, result_type = plain, result_encoding = utf8, vad_enable  = 1, vad_eos = 1000")
		, (SRLanguage == ESpeechLanguage::EL_Chinese)?LANGUAGE_CHINESE:LANGUAGE_ENGLISH);
}
void USpeechRecognizer::Tick(float DeltaTime)
{
	for (int16 i = ETaskAction::ES_NULL; i<ETaskAction::ES_MAXSTATE;i++)
	{
		if (SpeechRecognizeCompletion[i] && SpeechRecognizeCompletion[i]->IsComplete())
		{	
			if (!bLoginSuccessful && i == ETaskAction::ES_LOGIN)
			{
				UE_LOG(LogFlytekVoiceSDK, Log, TEXT("VoiceSDKLogin Completed ! Error code : %d"), ErrorResult[ES_LOGIN]);
				if (ErrorResult[ES_LOGIN] == 0)
				{
					bLoginSuccessful = true;
					SpeechRecInitRequest();
				}	
			}
			else if (!bInitSuccessful && i == ETaskAction::ES_INIT)
			{
				UE_LOG(LogFlytekVoiceSDK, Log, TEXT("VoiceSDK Init Completed ! Error code : %d"), ErrorResult[ES_INIT]);
				if (ErrorResult[ES_INIT] == 0)
				{
					bInitSuccessful = true;
				}
				InitResult.Broadcast(ErrorResult[ES_INIT]);
			}
			else if (i == ETaskAction::ES_STARTLISTENING)
			{
				if (ErrorResult[ES_STARTLISTENING] == 0)
				{
					UE_LOG(LogFlytekVoiceSDK, Log, TEXT("Start Speech!"))
				}
				else if (ErrorResult[ES_STARTLISTENING] != -1)
				{				
					UE_LOG(LogFlytekVoiceSDK, Log, TEXT("Start Speech faild! Error Code :%d"), ErrorResult[ES_STARTLISTENING]);
					if (bSpeeking)
					{
						bSpeeking = false;
					}
					if (bContinuous)
					{
						if (bContinuousSpeeking)
						{
							bContinuousSpeeking = false;
							SpeechRecStartListeningRequest();
						}
					}
				}
				ErrorResult[ES_STARTLISTENING] = -1;
			}
			else if (i == ETaskAction::ES_STOPLISTENING)
			{
				if (ErrorResult[ES_STOPLISTENING] == 0)
				{
					bOnSpeechRecEndSuccesful = true;
					bContinuousSpeeking = false;
					UE_LOG(LogFlytekVoiceSDK, Log, TEXT("Stop Speech!"))
				}
				else if (ErrorResult[ES_STOPLISTENING] != -1)
				{
					UE_LOG(LogFlytekVoiceSDK, Log, TEXT("Stop Speech faild! Error Code :%d"), ErrorResult[ES_STOPLISTENING])
				}
				ErrorResult[ES_STOPLISTENING] = -1;
			}
			
			SpeechRecognizeCompletion[i].SafeRelease();

		}
	}
	FScopeLock ScopeLock1(&AccessLock);
	if (bOnSpeechRecResultSuccesful)
	{
		CallbackResult.Broadcast(SpeechResultString);
		SpeeckResult.Broadcast(SpeechResultString);
		SpeechResultString = NULL;
		bOnSpeechRecResultSuccesful = false;
	}
	
	if (bOnSpeechRecEndSuccesful)
	{
		bOnSpeechRecEndSuccesful = false;
		bSpeeking = false;
		if (bContinuous)
		{
			if (bContinuousSpeeking)
			{
				SpeechRecStartListeningRequest();
			}
		}
	}
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
	SpeechRecognizeCompletion[ETaskAction::ES_LOGIN] = TGraphTask<FSpeechRecognizeTask>::CreateTask(NULL, ENamedThreads::GameThread).ConstructAndDispatchWhenReady(this, &USpeechRecognizer::CallSRLogin, SPEECH_THREAD, UserName, Password, Params);
}

void USpeechRecognizer::SpeechRecLogoutRequest()
{

}
void USpeechRecognizer::SpeechRecInitRequest()
{
	SpeechRecognizeCompletion[ETaskAction::ES_INIT] = TGraphTask<FSpeechRecognizeTask>::CreateTask(NULL, ENamedThreads::GameThread).ConstructAndDispatchWhenReady(this, &USpeechRecognizer::CallSRInit, SPEECH_THREAD);
}
void USpeechRecognizer::SpeechRecUninitRequest()
{
	SpeechRecognizeCompletion[ETaskAction::ES_UNINIT] = TGraphTask<FSpeechRecognizeTask>::CreateTask(NULL, ENamedThreads::GameThread).ConstructAndDispatchWhenReady(this, &USpeechRecognizer::CallSRUninit, SPEECH_THREAD);
}

void USpeechRecognizer::SpeechRecStartListeningRequest()
{
	if (!bLoginSuccessful)
	{
		UE_LOG(LogFlytekVoiceSDK, Log, TEXT("VoiceSDK  must Login first ! "));
		return;
	}
	if (!bInitSuccessful)
	{
		UE_LOG(LogFlytekVoiceSDK, Log, TEXT("VoiceSDK  must init first ! "));
		return;
	}
	if (bSpeeking)
	{
		UE_LOG(LogFlytekVoiceSDK, Log, TEXT("VoiceSDK  Speeking now! "));
		return;
	}
	bSpeeking = true;
	if (bContinuous)
	{
		bContinuousSpeeking = true;
	}
	SpeechRecognizeCompletion[ETaskAction::ES_STARTLISTENING] = TGraphTask<FSpeechRecognizeTask>::CreateTask(NULL, ENamedThreads::GameThread).ConstructAndDispatchWhenReady(this, &USpeechRecognizer::CallSRStartListening, SPEECH_THREAD);
}

void USpeechRecognizer::SpeechRecStopListeningRequest()
{
	if (!bLoginSuccessful)
	{
		UE_LOG(LogFlytekVoiceSDK, Log, TEXT("VoiceSDK  must Login first ! "));
		return;
	}
	if (!bInitSuccessful)
	{
		UE_LOG(LogFlytekVoiceSDK, Log, TEXT("VoiceSDK  must init first ! "));
		return;
	}
	if (!bSpeeking)
	{
		UE_LOG(LogFlytekVoiceSDK, Log, TEXT("VoiceSDK  dont need to stop "));
		return;
	}
	SpeechRecognizeCompletion[ETaskAction::ES_STOPLISTENING] = TGraphTask<FSpeechRecognizeTask>::CreateTask(NULL, ENamedThreads::GameThread).ConstructAndDispatchWhenReady(this, &USpeechRecognizer::CallSRStopListening, SPEECH_THREAD);

}
int32 USpeechRecognizer::CallSRLogin(const FString& UserName, const FString& Password, const FString& Params)
{
	FScopeLock ScopeLock1(&AccessLock);
	ErrorResult[ES_LOGIN] = sr_login(TCHAR_TO_ANSI(*UserName), TCHAR_TO_ANSI(*Password), TCHAR_TO_ANSI(*LoginParam));
	return ErrorResult[ES_LOGIN];
}
void USpeechRecognizer::CallSRLogout()
{
	FScopeLock ScopeLock1(&AccessLock);
	ErrorResult[ES_LOGOUT] = sr_logout();
}

int32 USpeechRecognizer::CallSRInit()
{
	
	RecNotifier = {
		OnSpeechResult,
		OnSpeechBeginResult,
		OnSpeechEndResult
	};
	
	 ErrorResult[ES_INIT] = sr_init(&SpeechRec, TCHAR_TO_ANSI(*Session_Begin_Param), SR_MIC, DEFAULT_INPUT_DEVID, &RecNotifier);
	 UE_LOG(LogFlytekVoiceSDK, Log, TEXT("Init! Error Code :%d"), ErrorResult[ES_INIT]);
	 return ErrorResult[ES_INIT];
}
int32 USpeechRecognizer::CallSRUninit()
{
	
	int32 ret = -1;
	if (bInitSuccessful)
	{
		sr_uninit(&SpeechRec);
		ret = 0;
	}
	bInitSuccessful = false;
	return ret;
}

int32 USpeechRecognizer::CallSRStartListening()
{
	
	if (bInitSuccessful)
	{
		ErrorResult[ES_STARTLISTENING] = sr_start_listening(&SpeechRec);
		UE_LOG(LogFlytekVoiceSDK, Log, TEXT("Start Recognizer Code :%d"), ErrorResult[ES_STARTLISTENING])
	}
	else
	{
		UE_LOG(LogFlytekVoiceSDK, Log, TEXT("Speek Recognizer uninitialized"))
	}
	return ErrorResult[ES_STARTLISTENING];
}

int32 USpeechRecognizer::CallSRStopListening()
{
	
	if (bInitSuccessful)
	{
		ErrorResult[ES_STOPLISTENING] = sr_stop_listening(&SpeechRec);
		UE_LOG(LogFlytekVoiceSDK, Log, TEXT("Stop Recognizer Code :%d"), ErrorResult[ES_STOPLISTENING])
	}
	else
	{
		UE_LOG(LogFlytekVoiceSDK, Log, TEXT("Speek Recognizer uninitialized"))
	}
	return ErrorResult[ES_STOPLISTENING];
}
void USpeechRecognizer::OnSpeechRecResult(const char* result, char is_last)
{
	FScopeLock ScopeLock1(&AccessLock);
	FString SpeechResultStr = UTF8_TO_TCHAR(result);
	if (bSpeeking)
	{
		if (is_last)
		{
			UE_LOG(LogFlytekVoiceSDK, Log, TEXT("Speeking Last words"), *SpeechResultStr);
			bOnSpeechRecResultSuccesful = true;
		}
		UE_LOG(LogFlytekVoiceSDK, Log, TEXT("Speeking String :[%s]"), *SpeechResultStr);
		SpeechResultString = SpeechResultStr;
		bOnSpeechRecResultSuccesful = true;
	}
}
void USpeechRecognizer::OnSpeechRecBegin()
{
	FScopeLock ScopeLock1(&AccessLock);
	UE_LOG(LogFlytekVoiceSDK, Log, TEXT("OnSpeechRecBegin"));
	bOnSpeechRecBeginSuccesful = true;

}
void USpeechRecognizer::OnSpeechRecEnd(int reason)
{
	FScopeLock ScopeLock1(&AccessLock);
	if (reason == END_REASON_VAD_DETECT)
	{	
		UE_LOG(LogFlytekVoiceSDK, Log, TEXT("Speeking Done"));
	}
	else
	{
		UE_LOG(LogFlytekVoiceSDK, Error, TEXT("On Speech Recognizer Error : %d"), reason)
			
	}
	bOnSpeechRecEndSuccesful = true;

}

