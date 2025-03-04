// CrywolfUtil.h: interface for the CCrywolfUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRYWOLFUTIL_H__8D1A179E_3D04_4578_A15B_3DAD0E55783B__INCLUDED_)
#define AFX_CRYWOLFUTIL_H__8D1A179E_3D04_4578_A15B_3DAD0E55783B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "user.h"

class CCrywolfScoreSort	// Completed
{

public:

	bool operator()(struct OBJECTSTRUCT * const lpObj1, struct OBJECTSTRUCT * const lpObj2)
	{
		if(lpObj1->Type != OBJ_USER || lpObj2->Type != OBJ_USER)
		{
			return 0;
		}

		bool bResult = ( lpObj1->m_iCrywolfMVPScore >= lpObj2->m_iCrywolfMVPScore ) ? TRUE : FALSE;

		if ( lpObj1->m_iCrywolfMVPScore == lpObj2->m_iCrywolfMVPScore )
			bResult = ( lpObj1->m_PlayerData->Experience < lpObj2->m_PlayerData->Experience ) ? TRUE : FALSE;

		return bResult;
	}
};

class CCrywolfUtil
{

public:

  CCrywolfUtil();
  virtual ~CCrywolfUtil();

  void __cdecl SendMapServerGroupMsg(LPSTR lpszMsg, ...);
  void SendAllUserAnyData(LPBYTE lpMsg, int iSize);
  void __cdecl SendAllUserAnyMsg(int iType, LPSTR lpszMsg, ...);
  void SendCrywolfUserAnyData(LPBYTE lpMsg, int iSize);
  void __cdecl SendCrywolfUserAnyMsg(int iType, LPSTR lpszMsg, ...);
  void __cdecl SendCrywolfChattingMsg(int iObjIndex, LPSTR lpszMsg, ...);
  int CrywolfMVPLevelUp(int iUserIndex, int iAddExp);

};
extern CCrywolfUtil UTIL;
#endif // !defined(AFX_CRYWOLFUTIL_H__8D1A179E_3D04_4578_A15B_3DAD0E55783B__INCLUDED_)


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

