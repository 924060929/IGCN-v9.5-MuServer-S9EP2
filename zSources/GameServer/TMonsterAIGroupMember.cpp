// TMonsterAIGroupMember.cpp: implementation of the TMonsterAIGroupMember class.
//	GS-N	1.00.18	JPN	0x005616A0	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TMonsterAIGroupMember.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TMonsterAIGroupMember::TMonsterAIGroupMember()
{
	this->Reset();
}

TMonsterAIGroupMember::~TMonsterAIGroupMember()
{
	return;
}


void TMonsterAIGroupMember::Reset()
{
	this->m_iGroupNumber = -1;
	this->m_iGuid = -1;
	this->m_iClass = -1;
	this->m_iRank = -1;
	this->m_iStartAI = 0;
	this->m_iAI01 = 0;
	this->m_iAI02 = 0;
	this->m_iAI03 = 0;
	this->m_iCreateType = -1;
	this->m_iMapNumber = -1;
	this->m_iStartX = -1;
	this->m_iStartY = -1;
	this->m_iStartDir = -1;
	this->m_iRegenType = -1;
	this->m_iObjIndex = -1;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

