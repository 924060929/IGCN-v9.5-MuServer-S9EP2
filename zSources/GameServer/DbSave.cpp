//////////////////////////////////////////////////////////////////////
// DbSave.cpp
// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------

// GS-N 0.99.60T 0x0041C820 Status : Completed
//	GS-N	1.00.18	0x004232A0	-	Completed

#include "stdafx.h"
#include "DbSave.h"
#include "WZQueue.h"
#include "TLog.h"
#include "GameMain.h"


static LRESULT WINAPI cSaveThreadProc(CDbSave * pThis);

CDbSave::CDbSave()
{
	this->m_lpWzQueue=NULL;
	this->m_bIsRunning=FALSE;
	this->Initialize();
}

CDbSave::~CDbSave()
{
	this->End();
	this->Feee();
}

BOOL CDbSave::Initialize()
{
	
	this->m_lpWzQueue  = new WZQueue(1280);

	if ( this->m_lpWzQueue == 0)
	{
		return FALSE;
	}
	
	InitializeCriticalSection(&this->criti );
	return TRUE;
}

BOOL CDbSave::Feee()
{
	if ( this->m_lpWzQueue != 0 )
	{
		delete this->m_lpWzQueue;
		this->m_lpWzQueue = 0;
	}

	DeleteCriticalSection(&this->criti );
	return TRUE;
}

BOOL CDbSave::Add(LPBYTE pObject, int nSize, BYTE headcode,  int index)
{
	EnterCriticalSection(&this->criti);

	BOOL bRet=this->m_lpWzQueue->AddToQueue(pObject, nSize, headcode, index);

	LeaveCriticalSection(&this->criti);
	return bRet;
}

BOOL CDbSave::Begin()
{
	if ( this->m_ThreadHandle != 0 )
	{
		this->End();
	}

	this->m_bIsRunning=TRUE;

	this->m_ThreadHandle=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)cSaveThreadProc, this, 0, &this->m_dwThreadID  );

	if ( this->m_ThreadHandle == 0 )
	{
		g_Log.MsgBox("Thread create error %s %d", __FILE__, __LINE__);
		return FALSE;
	}

	return TRUE;
}

static LRESULT WINAPI cSaveThreadProc(CDbSave * pThis)
{
	return pThis->ThreadProc();

}

void CDbSave::End()
{
	if ( this->m_ThreadHandle != 0 )
	{
		this->m_bIsRunning = FALSE;
		WaitForSingleObject(this->m_ThreadHandle , INFINITE);
		CloseHandle(this->m_ThreadHandle );
		this->m_ThreadHandle=0;
	}
}

DWORD CDbSave::ThreadProc()
{
	int count;
	BYTE RecvData[5000];
	UINT nSize;
	int headcode;
	INT uindex;

	while ( true )
	{
		EnterCriticalSection(&this->criti);
		count=this->m_lpWzQueue->GetCount();

		if ( count != 0 )
		{
			if (this->m_lpWzQueue->GetFromQueue(RecvData, &nSize, (UCHAR*)&headcode, &uindex) == 1 )
			{
				if (wsDataCli.DataSend((PCHAR)RecvData, nSize) == 0 )
				{
					g_Log.Add(0, __FILE__, __FUNCTION__, "[%d][%d] Character save fail", count, uindex);
				}
				else
				{
					g_Log.Add(0, __FILE__, __FUNCTION__, "[%d][%d] Character save success", count, uindex);
				}
			}
		}

		if ( this->m_bIsRunning == FALSE && count == 0)
		{
			break;
		}

		LeaveCriticalSection(&this->criti);
		WaitForSingleObject(this->m_ThreadHandle , 300);
	}
	return FALSE;
}

		





//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

