//////////////////////////////////////////////////////////////////////
// Guardian.h
#ifndef GUARDIAN_H
#define GUARDIAN_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGuardian
{
public:

	CGuardian();
	virtual ~CGuardian();

	BOOL CreateGuardian(int iIndex);
	void GuardianAct(int iIndex);

private:
	
	
};

extern CGuardian g_CsNPC_Guardian;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

