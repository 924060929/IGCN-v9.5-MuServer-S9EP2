// JewelOfHarmonySystem.cpp: implementation of the CJewelOfHarmonySystem class.
//	GS-N	1.00.18	JPN	0x00571D60	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JewelOfHarmonySystem.h"
#include "Gamemain.h"
#include "TLog.h"
#include "CastleSiegeSync.h"
#include "winutil.h"
#include "DSProtocol.h"
#include "ChaosBox.h"
#include "configread.h"
#include "ItemSocketOptionSystem.h"
#include "LuckyItemManager.h"

CJewelOfHarmonySystem g_kJewelOfHarmonySystem;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJewelOfHarmonySystem::CJewelOfHarmonySystem()
{
	this->m_bEnable = TRUE;
	memset(this->m_itemOption, 0, sizeof(this->m_itemOption));
}

CJewelOfHarmonySystem::~CJewelOfHarmonySystem()
{
	return;
}


void CJewelOfHarmonySystem::_InitOption()
{
	this->JEWEL_OF_HARMONY_ITEMINDEX = ITEMGET(14,41);
	this->JEWEL_OF_HARMONY_PURITY_ITEMINDEX = ITEMGET(14,42);
	this->JEWEL_OF_HARMONY_SMELT_NOR_ITEMINDEX = ITEMGET(14,43);
	this->JEWEL_OF_HARMONY_SMELT_EXT_ITEMINDEX = ITEMGET(14,44);

	memset(this->m_itemOption, 0, sizeof(this->m_itemOption));
	CIniReader ReadHarmony(g_ConfigRead.GetPath("IGC_HarmonySystem.ini"));

	this->m_bSystemPrutiyJewel			= ReadHarmony.ReadInt("HarmonySystem", "HarmonyJewelMix", 0);
	this->m_bSystemMixSmeltingStone		= ReadHarmony.ReadInt("HarmonySystem", "RefiningStoneMix", 0);
	this->m_bSystemRestoreStrengthen	= ReadHarmony.ReadInt("HarmonySystem", "RestoreStrengthenItem", 0);
	this->m_bSystemStrengthenItem		= ReadHarmony.ReadInt("HarmonySystem", "StrengthenItem", 0);
	this->m_bSystemSmeltingItem			= ReadHarmony.ReadInt("HarmonySystem", "SmeltItem", 0);
	
	this->m_iRatePuritySuccess			= ReadHarmony.ReadInt("HarmonyMix", "JewelOfHarmonyMixSuccessRate", 0);
	this->m_iZenForPurity				= ReadHarmony.ReadInt("HarmonyMix", "JewelOfHarmonyMixReqMoney", 0);
	this->m_iRateMixSmeltingStoneNor	= ReadHarmony.ReadInt("HarmonyMix", "LowerRefiningStoneMixSuccessRate", 0);
	this->m_iRateMixSmeltingStoneExt	= ReadHarmony.ReadInt("HarmonyMix", "HigherRefiningStoneMixSuccessRate", 0);
	this->m_iZenForMixSmeltingStone		= ReadHarmony.ReadInt("HarmonyMix", "RefiningStoneMixReqMoney", 0);
	this->m_iRateStrengthenSuccess		= ReadHarmony.ReadInt("HarmonyMix", "StrengthenItemSuccessRate", 0);
	this->m_iRateSmeltingSuccessNor		= ReadHarmony.ReadInt("HarmonyMix", "SmeltingItemSuccessRate_Normal", 0);
	this->m_iRateSmeltingSuccessExt		= ReadHarmony.ReadInt("HarmonyMix", "SmeltingItemSuccessRate_Enhanced", 0);
}



BOOL CJewelOfHarmonySystem::LoadScript(LPSTR lpszFileName)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(lpszFileName);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("Error loading %s file (%s)", lpszFileName, res.description());
		return FALSE;
	}

	this->_InitOption();

	pugi::xml_node main = file.child("HarmonySystem");

	for (pugi::xml_node type = main.child("Type"); type; type = type.next_sibling())
	{
		int iType = type.attribute("ID").as_int();

		if (iType <= JEWELOFHARMONY_ITEM_TYPE_NULL || iType > JEWELOFHARMONY_ITEM_TYPE_DEFENSE)
		{
			g_Log.MsgBox("%s - wrong Type ID (%d)", lpszFileName, iType);
			continue;
		}

		for (pugi::xml_node option = type.child("Option"); option; option = option.next_sibling())
		{
			int iOptionIndex = option.attribute("Index").as_int();

			if (iOptionIndex < 0 || iOptionIndex >= MAX_JOH_ITEM_INDEX)
			{
				g_Log.MsgBox("%s (Type ID:%d) - wrong Option Index (%d)", lpszFileName, iType, iOptionIndex);
				continue;
			}

			JEWELOFHARMONY_ITEM_OPTION * p = &this->m_itemOption[iType][iOptionIndex];

			strcpy(p->szOptionName, option.attribute("Name").as_string());
			p->iRandomWeight = option.attribute("RandomWeight").as_int();
			p->iRequireLevel = option.attribute("ReqLevel").as_int();

			int iCount = 0;
			for (pugi::xml_node effect = option.child("Effect"); effect; effect = effect.next_sibling())
			{
				char szTemp[128];

				sprintf(szTemp, "Value%d", iCount);
				p->iItemEffectValue[iCount] = effect.attribute(szTemp).as_int();

				sprintf(szTemp, "ReqMoney%d", iCount);
				p->iZenForRestore[iCount] = effect.attribute(szTemp).as_int();

				iCount++;
			}

			p->bValid = TRUE;
		}
	}

	return TRUE;
}

BOOL CJewelOfHarmonySystem::LoadScriptOfSmelt(LPSTR lpszFileName)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(lpszFileName);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("Error loading %s file (%s)", lpszFileName, res.description());
		return FALSE;
	}

	this->m_mapEnableMixList.clear();

	pugi::xml_node main = file.child("HarmonySystem");

	for (pugi::xml_node section = main.child("Section"); section; section = section.next_sibling())
	{
		int iItemType = section.attribute("ID").as_int();

		for (pugi::xml_node item = section.child("Item"); item; item = item.next_sibling())
		{
			int iItemIndex = item.attribute("Index").as_int();
			int iReqLevel = item.attribute("ReqLevel").as_int();

			int iItemID = ItemGetNumberMake(iItemType, iItemIndex);

			if (iItemID == -1)
			{
				g_Log.MsgBox("ERROR - Wrong Item found (%d) in (%s)", ITEMGET(iItemType, iItemIndex), lpszFileName);
				continue;
			}

			this->m_mapEnableMixList[iItemID] = iReqLevel;
		}
	}

	return TRUE;
}


BOOL CJewelOfHarmonySystem::IsJewelOfHarmonyOriginal(short type)
{
	BOOL bRet = FALSE;

	if ( type == this->JEWEL_OF_HARMONY_ITEMINDEX )
		bRet = TRUE;

	return bRet;
}


BOOL CJewelOfHarmonySystem::IsJewelOfHarmonyPurity(short type)
{
	BOOL bRet = FALSE;

	if ( type == this->JEWEL_OF_HARMONY_PURITY_ITEMINDEX )
		bRet = TRUE;

	return bRet;
}



BOOL CJewelOfHarmonySystem::IsJewelOfHarmonySmeltingItems(short type)
{
	BOOL bRet = FALSE;

	if ( this->_IsJewelOfHarmonySmeltingItemNor(type) == TRUE || this->_IsJewelOfHarmonySmeltingItemExt(type) == TRUE )
		bRet = TRUE;

	return bRet;
}

BOOL CJewelOfHarmonySystem::_IsJewelOfHarmonySmeltingItemNor(short type)
{
	BOOL bRet = FALSE;

	if ( type == this->JEWEL_OF_HARMONY_SMELT_NOR_ITEMINDEX )
		bRet = TRUE;

	return bRet;
}


BOOL CJewelOfHarmonySystem::_IsJewelOfHarmonySmeltingItemExt(short type)
{
	BOOL bRet = FALSE;

	if ( type == this->JEWEL_OF_HARMONY_SMELT_EXT_ITEMINDEX )
		bRet = TRUE;

	return bRet;
}



void CJewelOfHarmonySystem::SetEnableToUsePuritySystem(BOOL bEnable)
{
	this->m_bEnable = bEnable;

	g_Log.Add("[JewelOfHarmony][PuritySystem] Enable %d", bEnable);
}

BOOL CJewelOfHarmonySystem::IsEnableToUsePuritySystem()
{
	return this->m_bEnable;
}


void CJewelOfHarmonySystem::PurityJewelOfHarmony(LPOBJ lpObj)
{
	if (this->m_bSystemPrutiyJewel != TRUE)
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0, 279), lpObj->m_Index, 1);
		return;
	}

	if (this->IsEnableToUsePuritySystem() == FALSE)
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0, 279), lpObj->m_Index, 1);
		g_Log.Add("[JewelOfHarmony][%s][%s] Not Purity Time ", lpObj->AccountID, lpObj->Name);
		return;
	}

	lpObj->ChaosLock = TRUE;
	// Chaos Lock was Enabled
	int iJewelOfHarmonyItemCount = 0;
	int iInvalidItemCount = 0;
	int iChaosMixPrice = 0;
	PMSG_CHAOSMIXRESULT pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof(PMSG_CHAOSMIXRESULT));

	pMsg.Result = CB_ERROR;

	for (int n = 0; n < CHAOS_BOX_SIZE; n++)
	{
		if (lpObj->pChaosBox[n].IsItem() == TRUE)
		{
			if (lpObj->pChaosBox[n].m_Type == this->JEWEL_OF_HARMONY_ITEMINDEX)
			{
				iJewelOfHarmonyItemCount++;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if (iInvalidItemCount > 0 || iJewelOfHarmonyItemCount != 1)
	{
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	iChaosMixPrice = this->m_iZenForPurity;
	int iChaosTaxMoney = iChaosMixPrice * g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / 100;

	if (iChaosTaxMoney < 0)
		iChaosTaxMoney = 0;

	iChaosMixPrice += iChaosTaxMoney;

	if (iChaosMixPrice < 0)
		iChaosMixPrice = 0;

	if (lpObj->m_PlayerData->Money < iChaosMixPrice)
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	lpObj->m_PlayerData->Money -= iChaosMixPrice;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);
	g_MixSystem.LogChaosItem(lpObj, "JewelOfHarmony][Purity");

	g_Log.Add("[JewelOfHarmony][Purity] - Mix Start");

	int iRate = rand() % 100;

	if (iRate < this->m_iRatePuritySuccess)
	{
		int iItemType = this->JEWEL_OF_HARMONY_PURITY_ITEMINDEX;
		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, iItemType, 0, 1, 0, 0, 0, lpObj->m_Index, 0, 0, 0, 0, 0);
		gObjInventoryCommit(lpObj->m_Index);

		g_Log.Add("[JewelOfHarmony][Purity] Purity Success [%s][%s] Rate %d/%d",
			lpObj->AccountID, lpObj->Name, iRate, this->m_iRatePuritySuccess);
	}
	else
	{
		g_MixSystem.ChaosBoxInit(lpObj);
		GSProtocol.GCUserChaosBoxSend(lpObj, 0);
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		g_Log.Add("[JewelOfHarmony][Purity] Purity Fail [%s][%s] Rate %d/%d",
			lpObj->AccountID, lpObj->Name, iRate, this->m_iRatePuritySuccess);
	}
	// Chaos Lock was Disabled
	lpObj->ChaosLock = FALSE;
}


void CJewelOfHarmonySystem::PurityJewelOfHarmony_MultiMix(OBJECTSTRUCT * lpObj, int iMixCount)
{
	if (this->m_bSystemPrutiyJewel != TRUE)
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0, 279), lpObj->m_Index, 1);
		return;
	}

	if (this->IsEnableToUsePuritySystem() == FALSE)
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0, 279), lpObj->m_Index, 1);
		g_Log.Add("[JewelOfHarmony][%s][%s] Not Purity Time ", lpObj->AccountID, lpObj->Name);
		return;
	}

	lpObj->ChaosLock = TRUE;
	// Chaos Lock was Enabled
	int iJewelOfHarmonyItemCount = 0;
	int iInvalidItemCount = 0;
	int iChaosMixPrice = 0;
	PMSG_CHAOSMIXRESULT pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof(PMSG_CHAOSMIXRESULT));

	pMsg.Result = CB_ERROR;

	for (int n = 0; n < CHAOS_BOX_SIZE; n++)
	{
		if (lpObj->pChaosBox[n].IsItem() == TRUE)
		{
			if (lpObj->pChaosBox[n].m_Type == this->JEWEL_OF_HARMONY_ITEMINDEX)
			{
				iJewelOfHarmonyItemCount++;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if (iInvalidItemCount > 0 || iJewelOfHarmonyItemCount != iMixCount)
	{
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	iChaosMixPrice = this->m_iZenForPurity * iMixCount;
	int iChaosTaxMoney = iChaosMixPrice * g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / 100;

	if (iChaosTaxMoney < 0)
		iChaosTaxMoney = 0;

	iChaosMixPrice += iChaosTaxMoney;

	if (iChaosMixPrice < 0)
		iChaosMixPrice = 0;

	if (lpObj->m_PlayerData->Money < iChaosMixPrice)
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	lpObj->m_PlayerData->Money -= iChaosMixPrice;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);
	g_MixSystem.LogChaosItem(lpObj, "JewelOfHarmony][Purity");

	pMsg.Result = CB_MULTIMIX_RESULT;
	pMsg.Pos = iMixCount;
	memset(pMsg.ItemInfo, 0x00, sizeof(pMsg.ItemInfo));

	lpObj->ChaosMassMixCurrItem = 0;
	lpObj->ChaosMassMixSuccessCount = 0;

	g_Log.Add("[JewelOfHarmony][Purity] - Mix Start");

	for (int i = 0; i < iMixCount; i++)
	{
		int iRate = rand() % 100;

		if (iRate < this->m_iRatePuritySuccess)
		{
			BYTE btMixNumber = 1 << (i % 8);
			pMsg.ItemInfo[i / 8] |= btMixNumber;

			lpObj->ChaosMassMixSuccessCount++;

			g_Log.Add("[JewelOfHarmony][Purity] Purity Success (MassID:%d) [%s][%s] Rate %d/%d",
				i, lpObj->AccountID, lpObj->Name, iRate, this->m_iRatePuritySuccess);
		}

		else
		{
			g_Log.Add("[JewelOfHarmony][Purity] Purity Fail (MassID:%d) [%s][%s] Rate %d/%d",
				i, lpObj->AccountID, lpObj->Name, iRate, this->m_iRatePuritySuccess);
		}
	}

	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

	if (lpObj->ChaosMassMixSuccessCount == 0)
	{
		g_MixSystem.ChaosBoxInit(lpObj);
		GSProtocol.GCUserChaosBoxSend(lpObj, 0);

		pMsg.Result = CB_ERROR;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	}

	else
	{
		int iItemType = this->JEWEL_OF_HARMONY_PURITY_ITEMINDEX;
		
		for (int i = 0; i < lpObj->ChaosMassMixSuccessCount; i++)
		{
			ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, iItemType, 0, 1, 0, 0, 0, lpObj->m_Index, 0, 0, 0, 0, 0);
		}

		gObjInventoryCommit(lpObj->m_Index);
	}
}

BYTE CJewelOfHarmonySystem::GetItemStrengthenOption(CItem *pItem)
{
	return ( pItem->m_JewelOfHarmonyOption & 0xF0 ) >> 4;
}

BYTE CJewelOfHarmonySystem::GetItemOptionLevel(CItem *pItem)
{
	return this->_GetItemOptionLevel(pItem);
}

BYTE CJewelOfHarmonySystem::_GetItemOptionLevel(CItem *pItem)
{
	return ( pItem->m_JewelOfHarmonyOption & 0x0F ) ;
}

BOOL CJewelOfHarmonySystem::IsStrengthenByJewelOfHarmony(CItem *pItem)
{
	if ( g_SocketOptionSystem.IsSocketItem(pItem) == true )
		return FALSE;

	if ( this->GetItemStrengthenOption(pItem) > 0  )
		return TRUE;

	return FALSE;
}

BOOL CJewelOfHarmonySystem::IsActive(CItem *pItem)
{
	if ( this->IsStrengthenByJewelOfHarmony(pItem) == FALSE )
		return FALSE;

	int iOptionLevel = this->_GetItemOptionLevel(pItem);

	if ( pItem->m_Level < iOptionLevel )
		return FALSE;

	return TRUE;
}

BYTE CJewelOfHarmonySystem::_GetItemOptionRequireLevel(CItem * pItem)
{
	int iItemType = this->_GetItemType(pItem);
	BYTE iItemOption = this->GetItemStrengthenOption(pItem);

	if ( iItemType == 0 || iItemOption > MAX_JOH_ITEM_OPTION+1 || iItemOption == 0 )
		return 0;

	return this->m_itemOption[iItemType][iItemOption].iRequireLevel;
}



int CJewelOfHarmonySystem::_GetItemType(CItem *pItem)
{
	int iItemType = JEWELOFHARMONY_ITEM_TYPE_NULL;

	if ( (pItem->m_Type >= ITEMGET(0,0) && pItem->m_Type <ITEMGET(4,0)) || (pItem->m_Type >= ITEMGET(4,0) && pItem->m_Type <ITEMGET(4,7)) || (pItem->m_Type >= ITEMGET(4,8) && pItem->m_Type <ITEMGET(4,15)) || (pItem->m_Type >= ITEMGET(4,16) && pItem->m_Type <ITEMGET(5,0)) )
		iItemType =JEWELOFHARMONY_ITEM_TYPE_WEAPON;

	if (pItem->m_Type >= ITEMGET(5,0) && pItem->m_Type <ITEMGET(6,0) )
		iItemType =JEWELOFHARMONY_ITEM_TYPE_STAFF;

	if (pItem->m_Type >= ITEMGET(6,0) && pItem->m_Type <ITEMGET(12,0) )
		iItemType =JEWELOFHARMONY_ITEM_TYPE_DEFENSE;

	return iItemType;
}



int CJewelOfHarmonySystem::_GetSelectRandomOption(CItem * pItem, int iItemType)
{
	if ( iItemType == 0 )
		return iItemType;

	int iItemOption = AT_JEWELOFHARMONY_NOT_STRENGTHEN_ITEM;
	this->m_kRandomPool.InitPool();

	for ( int idx =1;idx<MAX_JOH_ITEM_INDEX;idx++)
	{
		if ( this->m_itemOption[iItemType][idx].bValid == FALSE )
			continue;

		if ( pItem->m_Level < this->m_itemOption[iItemType][idx].iRequireLevel )
			continue;

		if ( iItemOption == AT_JEWELOFHARMONY_STAFF_DECREASE_REQUIRE_STR || iItemOption == AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_STR)
		{
			if ( pItem->m_RequireStrength == 0 )
				continue;
		}

		if ( iItemOption == AT_JEWELOFHARMONY_STAFF_DECREASE_REQUIRE_DEX || iItemOption == AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_DEX)
		{
			if ( pItem->m_RequireDexterity == 0 )
				continue;
		}

		this->m_kRandomPool.AddValue(idx, this->m_itemOption[iItemType][idx].iRandomWeight);
	}

	iItemOption = this->m_kRandomPool.GetRandomValue(eRANDOMPOOL_BY_WEIGHT);
	return iItemOption;
}

BOOL CJewelOfHarmonySystem::StrengthenItemByJewelOfRise(LPOBJ lpObj, int source, int target)
{
	if ( this->m_bSystemStrengthenItem == FALSE )
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,281), lpObj->m_Index, 1);
		return FALSE;
	}

	if ( source < 0 || source > MAIN_INVENTORY_SIZE-1 )
		return FALSE;

	if ( target < 0 || target > MAIN_INVENTORY_SIZE-1 )
		return FALSE;

	if ( lpObj->pInventory[source].IsItem() == FALSE )
		return FALSE;

	if ( lpObj->pInventory[target].IsItem() == FALSE )
		return FALSE;

	CItem * pSource = &lpObj->pInventory[source];
	CItem * pTarget = &lpObj->pInventory[target];

	if (!g_LuckyItemManager.IsLuckyItemEquipment(pTarget->m_Type))
	{
		return false;
	}

	if ( this->IsStrengthenByJewelOfHarmony(pTarget) == TRUE )
	{
		g_Log.Add("[LuckyItem][Strengthen Item] Already Strengthened [%s][%s]",
			lpObj->AccountID, lpObj->Name);

		return FALSE;
	}

	int iItemType = this->_GetItemType(pTarget);

	if ( iItemType == JEWELOFHARMONY_ITEM_TYPE_NULL )
	{
		g_Log.Add("[LuckyItem][Strengthen Item] Strengthen Fail [%s][%s] Name[%s] Type[%d] Serial[%I64d] Invalid ItemType[%d]",
			lpObj->AccountID, lpObj->Name, pTarget->GetName(), pTarget->m_Type,
			pTarget->m_Number, iItemType);
		
		return FALSE;
	}

	int iItemOption = this->_GetSelectRandomOption(pTarget, iItemType);

	if ( iItemOption == AT_JEWELOFHARMONY_NOT_STRENGTHEN_ITEM )
	{
		g_Log.Add("[LuckyItem][Strengthen Item] Strengthen Fail - NOT OPTION [%s][%s] Name[%s] Type[%d] Serial[%I64d] ItemType[%d]",
			lpObj->AccountID, lpObj->Name, pTarget->GetName(), pTarget->m_Type,
			pTarget->m_Number, iItemType);
		
		return FALSE;
	}

	int iItemOptionLevel = this->m_itemOption[iItemType][iItemOption].iRequireLevel;
	int iSuccessRate = rand() % 100;

	if ( iSuccessRate >= this->m_iRateStrengthenSuccess )
	{
		g_Log.Add("[LuckyItem][Strengthen Item] Strengthen Fail [%s][%s] Name[%s] Type[%d] Serial[%I64d]  Rate (%d/%d)",
			lpObj->AccountID, lpObj->Name, pTarget->GetName(), pTarget->m_Type,
			pTarget->m_Number, iSuccessRate, this->m_iRateStrengthenSuccess);
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,274), lpObj->m_Index, 1);
		return TRUE;
	}

	this->_MakeOption(pTarget, iItemOption, iItemOptionLevel);

	g_Log.Add("[LuckyItem][Strengthen Item] Strengthen Success [%s][%s] Name[%s] Type[%d] Serial[%I64d] Rate (%d/%d) Option %d OptionLevel %d",
		lpObj->AccountID, lpObj->Name, pTarget->GetName(), pTarget->m_Type,
		pTarget->m_Number, iSuccessRate, this->m_iRateStrengthenSuccess,
		iItemOption, iItemOptionLevel);

	GSProtocol.GCServerMsgStringSend(Lang.GetText(0,275), lpObj->m_Index, 1);

	gObjMakePreviewCharSet(lpObj->m_Index);

	float levelitemdur = ItemGetDurability(lpObj->pInventory[target].m_Type,
		lpObj->pInventory[target].m_Level, lpObj->pInventory[target].IsExtItem(),
		lpObj->pInventory[target].IsSetItem());

	lpObj->pInventory[target].m_Durability = levelitemdur * lpObj->pInventory[target].m_Durability / lpObj->pInventory[target].m_BaseDurability;

	lpObj->pInventory[target].Convert(lpObj->pInventory[target].m_Type,
		lpObj->pInventory[target].m_Option1, lpObj->pInventory[target].m_Option2,
		lpObj->pInventory[target].m_Option3, lpObj->pInventory[target].m_NewOption,
		lpObj->pInventory[target].m_SetOption, lpObj->pInventory[target].m_ItemOptionEx,
		0, -1, 0, CURRENT_DB_VERSION);

	return TRUE;
}

BOOL CJewelOfHarmonySystem::StrengthenItemByJewelOfHarmony(LPOBJ lpObj, int source, int target)
{
	if ( this->m_bSystemStrengthenItem == FALSE )
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,281), lpObj->m_Index, 1);
		return FALSE;
	}

	if ( source < 0 || source > MAIN_INVENTORY_SIZE-1 )
		return FALSE;

	if ( target < 0 || target > MAIN_INVENTORY_SIZE-1 )
		return FALSE;

	if ( lpObj->pInventory[source].IsItem() == FALSE )
		return FALSE;

	if ( lpObj->pInventory[target].IsItem() == FALSE )
		return FALSE;

	CItem * pSource = &lpObj->pInventory[source];
	CItem * pTarget = &lpObj->pInventory[target];

	if ( this->IsStrengthenByJewelOfHarmony(pTarget) == TRUE )
	{
		g_Log.Add("[JewelOfHarmony][Strengthen Item] Already Strengthened [%s][%s]",
			lpObj->AccountID, lpObj->Name);

		return FALSE;
	}

	if (pTarget->IsSetItem() != false && g_ConfigRead.data.common.AncHarmonyLimit == false )
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,273), lpObj->m_Index, 1);
		g_Log.Add("[JewelOfHarmony][Strengthen Item] SetItem not enable to Strengthened [%s][%s]",
			lpObj->AccountID, lpObj->Name);

		return FALSE;
	}

	if (g_SocketOptionSystem.IsSocketItem(pTarget) )
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,273), lpObj->m_Index, 1);
		g_Log.Add("[JewelOfHarmony][Strengthen Item] SocketItem not enable to Strengthened [%s][%s]",
			lpObj->AccountID, lpObj->Name);

		return FALSE;
	}

	int iItemType = this->_GetItemType(pTarget);

	if ( iItemType == JEWELOFHARMONY_ITEM_TYPE_NULL )
	{
		g_Log.Add("[JewelOfHarmony][Strengthen Item] Strengthen Fail [%s][%s] Name[%s] Type[%d] Serial[%I64d] Invalid ItemType[%d]",
			lpObj->AccountID, lpObj->Name, pTarget->GetName(), pTarget->m_Type,
			pTarget->m_Number, iItemType);
		
		return FALSE;
	}

	int iItemOption = this->_GetSelectRandomOption(pTarget, iItemType);

	if ( iItemOption == AT_JEWELOFHARMONY_NOT_STRENGTHEN_ITEM )
	{
		g_Log.Add("[JewelOfHarmony][Strengthen Item] Strengthen Fail - NOT OPTION [%s][%s] Name[%s] Type[%d] Serial[%I64d] ItemType[%d]",
			lpObj->AccountID, lpObj->Name, pTarget->GetName(), pTarget->m_Type,
			pTarget->m_Number, iItemType);
		
		return FALSE;
	}

	int iItemOptionLevel = this->m_itemOption[iItemType][iItemOption].iRequireLevel;
	int iSuccessRate = rand() % 100;

	if ( iSuccessRate >= this->m_iRateStrengthenSuccess )
	{
		g_Log.Add("[JewelOfHarmony][Strengthen Item] Strengthen Fail [%s][%s] Name[%s] Type[%d] Serial[%I64d]  Rate (%d/%d)",
			lpObj->AccountID, lpObj->Name, pTarget->GetName(), pTarget->m_Type,
			pTarget->m_Number, iSuccessRate, this->m_iRateStrengthenSuccess);
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,274), lpObj->m_Index, 1);
		return TRUE;
	}

	this->_MakeOption(pTarget, iItemOption, iItemOptionLevel);

	g_Log.Add("[JewelOfHarmony][Strengthen Item] Strengthen Success [%s][%s] Name[%s] Type[%d] Serial[%I64d] Rate (%d/%d) Option %d OptionLevel %d",
		lpObj->AccountID, lpObj->Name, pTarget->GetName(), pTarget->m_Type,
		pTarget->m_Number, iSuccessRate, this->m_iRateStrengthenSuccess,
		iItemOption, iItemOptionLevel);

	GSProtocol.GCServerMsgStringSend(Lang.GetText(0,275), lpObj->m_Index, 1);

	gObjMakePreviewCharSet(lpObj->m_Index);

	float levelitemdur = ItemGetDurability(lpObj->pInventory[target].m_Type,
		lpObj->pInventory[target].m_Level, lpObj->pInventory[target].IsExtItem(),
		lpObj->pInventory[target].IsSetItem());

	lpObj->pInventory[target].m_Durability = levelitemdur * lpObj->pInventory[target].m_Durability / lpObj->pInventory[target].m_BaseDurability;

	lpObj->pInventory[target].Convert(lpObj->pInventory[target].m_Type,
		lpObj->pInventory[target].m_Option1, lpObj->pInventory[target].m_Option2,
		lpObj->pInventory[target].m_Option3, lpObj->pInventory[target].m_NewOption,
		lpObj->pInventory[target].m_SetOption, lpObj->pInventory[target].m_ItemOptionEx,
		0, -1, 0, CURRENT_DB_VERSION);

	return TRUE;
}


BOOL CJewelOfHarmonySystem::_MakeOption(CItem *pItem, BYTE btOptionType, BYTE btOptionLevel)
{
	pItem->m_JewelOfHarmonyOption = AT_JEWELOFHARMONY_NOT_STRENGTHEN_ITEM;
	pItem->m_JewelOfHarmonyOption |= btOptionType << 4;
	pItem->m_JewelOfHarmonyOption |= btOptionLevel & 0x0F;

	return TRUE;
}


#pragma warning ( disable : 4101 )
void CJewelOfHarmonySystem::StrengthenItemByMacro(LPOBJ lpObj, BYTE invenrotyTargetPos, BYTE btOptionType,  BYTE btOptionLevel)
{
//#pragma message("Add Here code to make an Artificial JOH Item")
	return;
	
	int iType;
	CItem * pItem;
	char szMsg[256];
	{
		char szMsg[256];
	}
	{
		char szMsg[256];
	}
	{
		char szMsg[256];
	}
	{
		BYTE iValue;
	}
	{
		
		char szMsg[256];
	}
	{
		char szMsg[256];
	}
	{
		char szMsg[256];
	}
	{
		char szMsg	[256];
	}
}
#pragma warning ( default : 4101 )

BYTE CJewelOfHarmonySystem::ShowStrengthenOption(CItem *pItem)
{
	BYTE bResult = -1;

	if ( this->IsStrengthenByJewelOfHarmony(pItem) == FALSE )
		return bResult;

	int type = this->_GetItemType(pItem);
	bResult = this->GetItemStrengthenOption(pItem);
	int iItemOptionLevel = this->_GetItemOptionLevel(pItem);
	return bResult;
}


BYTE CJewelOfHarmonySystem::MakeCharSetData(LPOBJ lpObj)
{
	BYTE btResult = 0;

	if ( this->IsActive(&lpObj->pInventory[0]) == TRUE )
	{
		int iOptionLevel = this->_GetItemOptionLevel(&lpObj->pInventory[0]);

		if ( iOptionLevel > 5 && iOptionLevel < 9 )
			btResult |= 0x40;
		else if ( iOptionLevel > 8 && iOptionLevel < 13 )
			btResult |= 0x80;
		else if ( iOptionLevel > 12 )
			btResult |= 0xC0;
	}

	if ( this->IsActive(&lpObj->pInventory[1]) == TRUE )
	{
		int iOptionLevel = this->_GetItemOptionLevel(&lpObj->pInventory[1]);

		if ( iOptionLevel > 5 && iOptionLevel < 9 )
			btResult |= 0x10;
		else if ( iOptionLevel > 8 && iOptionLevel < 13 )
			btResult |= 0x20;
		else if ( iOptionLevel > 12 )
			btResult |= 0x30;
	}

	return btResult;
}

void CJewelOfHarmonySystem::SetApplyStrengthenItem(LPOBJ lpObj)
{
	JEWELOFHARMONY_ITEM_EFFECT * pItemEffect = &lpObj->m_PlayerData->m_JewelOfHarmonyEffect;

	this->InitEffectValue(pItemEffect);

	int iItemIndex = 0;

	for ( iItemIndex =0;iItemIndex <INVETORY_WEAR_SIZE ; iItemIndex++)
	{
		if ( lpObj->pInventory[iItemIndex].IsItem() != FALSE )
		{
			if ( lpObj->pInventory[iItemIndex].m_IsValidItem !=  false )
			{
				BOOL bResult = this->_CalcItemEffectValue(&lpObj->pInventory[iItemIndex], pItemEffect);
			}
		}
	}

	lpObj->m_AttackDamageMinLeft += pItemEffect->HJOpAddMinAttackDamage;
	lpObj->m_AttackDamageMinLeft += pItemEffect->HJOpAddAttackDamage;
	lpObj->m_AttackDamageMinRight += pItemEffect->HJOpAddMinAttackDamage;
	lpObj->m_AttackDamageMinRight += pItemEffect->HJOpAddAttackDamage;

	lpObj->m_AttackDamageMaxLeft += pItemEffect->HJOpAddMaxAttackDamage;
	lpObj->m_AttackDamageMaxLeft += pItemEffect->HJOpAddAttackDamage;
	lpObj->m_AttackDamageMaxRight += pItemEffect->HJOpAddMaxAttackDamage;
	lpObj->m_AttackDamageMaxRight += pItemEffect->HJOpAddAttackDamage;

	lpObj->m_MagicDamageMin += pItemEffect->HJOpAddMagicPower;
	lpObj->m_MagicDamageMax += pItemEffect->HJOpAddMagicPower;

	lpObj->m_Defense += pItemEffect->HJOpAddDefense * 10 / 20;

	lpObj->AddBP += pItemEffect->HJOpAddMaxAG;
	
	lpObj->AddLife += pItemEffect->HJOpAddMaxHP;

	lpObj->DamageMinus += pItemEffect->HJOpAddDamageDecrease;
}



int CJewelOfHarmonySystem::GetItemEffectValue(CItem * pItem, int iOptionType)
{
	int iItemType = this->_GetItemType(pItem);

	if ( iItemType == JEWELOFHARMONY_ITEM_TYPE_NULL )
		return 0;

	int iItemOptionType = AT_JEWELOFHARMONY_NOT_STRENGTHEN_ITEM;

	iItemOptionType = this->GetItemStrengthenOption(pItem);

	if ( iItemOptionType == AT_JEWELOFHARMONY_NOT_STRENGTHEN_ITEM ||iItemOptionType != iOptionType )
		return 0;

	int iItemOptionLevel = this->_GetItemOptionLevel(pItem);

	if ( iItemOptionLevel >= MAX_JOH_ITEM_OPTION )
		return 0;

	int iItemEffectValue = this->m_itemOption[iItemType][iItemOptionType].iItemEffectValue[iItemOptionLevel];
	return iItemEffectValue;
}


BOOL CJewelOfHarmonySystem::_CalcItemEffectValue(CItem *pItem, JEWELOFHARMONY_ITEM_EFFECT * pItemEffect)
{
	int iItemType = this->_GetItemType(pItem);

	if ( iItemType == JEWELOFHARMONY_ITEM_TYPE_NULL )
		return 0;

	int iItemOptionType = AT_JEWELOFHARMONY_NOT_STRENGTHEN_ITEM;

	iItemOptionType = this->GetItemStrengthenOption(pItem);

	if ( iItemOptionType == AT_JEWELOFHARMONY_NOT_STRENGTHEN_ITEM )
		return 0;

	int iItemOptionLevel = this->_GetItemOptionLevel(pItem);

	if ( iItemOptionLevel >= MAX_JOH_ITEM_OPTION )
		return 0;

	if ( this->IsActive(pItem) == FALSE )
		return FALSE;

	int iItemEffectValue = this->m_itemOption[iItemType][iItemOptionType].iItemEffectValue[iItemOptionLevel];

	BOOL bResult = TRUE;
//#pragma message("Add the LAcking Effects CJewelOfHarmonySystem::_CalcItemEffectValue")
	switch ( iItemType )
	{
		case JEWELOFHARMONY_ITEM_TYPE_WEAPON:
			switch (iItemOptionType)
			{
				case AT_JEWELOFHARMONY_WEAPON_IMPROVE_MINATTACKDAMAGE:
					pItemEffect->HJOpAddMinAttackDamage += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_WEAPON_IMPROVE_MAXATTACKDAMAGE:
					pItemEffect->HJOpAddMaxAttackDamage += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_STR:
					break;
				case AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_DEX:
					break;
				case AT_JEWELOFHARMONY_WEAPON_IMPROVE_ATTACKDAMAGE_BOTH:
					pItemEffect->HJOpAddMinAttackDamage += iItemEffectValue;
					pItemEffect->HJOpAddMaxAttackDamage += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_WEAPON_IMPROVE_CRITICALDAMAGE:
					pItemEffect->HJOpAddCriticalDamage += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_WEAPON_IMPROVE_SKILLDAMAGE:
					pItemEffect->HJOpAddSkillAttack += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_WEAPON_IMPROVE_ATTACKSUCCESRATE_PVP:
					pItemEffect->HJOpAddAttackSuccessRatePVP += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_WEAPON_DECREASE_SD_RATE:
					pItemEffect->HJOpDecreaseSDRate += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_WEAPON_IMPROVE_IGNORE_SD_RATE:
					pItemEffect->HJOpAddIgnoreSDRate += iItemEffectValue;
					break;
				default:
					bResult = FALSE;
			}
			break;

		case JEWELOFHARMONY_ITEM_TYPE_STAFF:
			switch (iItemOptionType)
			{
				case AT_JEWELOFHARMONY_STAFF_IMPROVE_MAGICDAMAGE:
					pItemEffect->HJOpAddMagicPower += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_STAFF_DECREASE_REQUIRE_STR:
					break;
				case AT_JEWELOFHARMONY_STAFF_DECREASE_REQUIRE_DEX:
					break;
				case AT_JEWELOFHARMONY_STAFF_IMPROVE_SKILLDAMAGE:
					pItemEffect->HJOpAddSkillAttack += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_STAFF_IMPROVE_CRITICALDAMAGE:
					pItemEffect->HJOpAddCriticalDamage += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_STAFF_DECREASE_SD_RATE:
					pItemEffect->HJOpDecreaseSDRate += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_STAFF_IMPROVE_ATTACKSUCCESRATE_PVP:
					pItemEffect->HJOpAddAttackSuccessRatePVP += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_STAFF_IMPROVE_IGNORE_SD_RATE:
					pItemEffect->HJOpAddIgnoreSDRate += iItemEffectValue;
					break;
				default:
					bResult = FALSE;
			}
			break;

		case JEWELOFHARMONY_ITEM_TYPE_DEFENSE:
			switch (iItemOptionType)
			{
				case AT_JEWELOFHARMONY_DEFENSE_IMPROVE_DEFENSE:
					pItemEffect->HJOpAddDefense += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_DEFENSE_IMPROVE_MAX_AG:
					pItemEffect->HJOpAddMaxAG += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_DEFENSE_IMPROVE_MAX_HP:
					pItemEffect->HJOpAddMaxHP += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_DEFENSE_IMPROVE_AUTO_REFILL_HP:
					pItemEffect->HJOpAddRefillHP += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_DEFENSE_IMPROVE_AUTO_REFILL_MP:
					pItemEffect->HJOpAddRefillMP += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_DEFENSE_IMPROVE_BLOCKSUCCESRATE_PVP:
					pItemEffect->HJOpAddDefenseSuccessRatePvP += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_DEFENSE_IMPROVE_DAMAGE_DECREASE:
					pItemEffect->HJOpAddDamageDecrease += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_DEFENSE_IMPROVE_SD_RATE:
					pItemEffect->HJOpAddSDRate += iItemEffectValue;
					break;
				default:
					bResult = FALSE;
			}
			break;

		default:
			bResult = FALSE;
	}

	return bResult;
}


void CJewelOfHarmonySystem::InitEffectValue(JEWELOFHARMONY_ITEM_EFFECT * pItemEffect)
{
	pItemEffect->HJOpAddMinAttackDamage = 0;
	pItemEffect->HJOpAddMaxAttackDamage = 0;
	pItemEffect->HJOpRequireStr = 0;
	pItemEffect->HJOpRequireDex = 0;
	pItemEffect->HJOpAddAttackDamage = 0;
	pItemEffect->HJOpAddCriticalDamage = 0;
	pItemEffect->HJOpAddSkillAttack = 0;
	pItemEffect->HJOpAddAttackSuccessRatePVP = 0;
	pItemEffect->HJOpDecreaseSDRate = 0;
	pItemEffect->HJOpAddIgnoreSDRate = 0;
	pItemEffect->HJOpAddMagicPower = 0;
	pItemEffect->HJOpAddDefense = 0;
	pItemEffect->HJOpAddMaxAG = 0;
	pItemEffect->HJOpAddMaxHP = 0;
	pItemEffect->HJOpAddRefillHP = 0;
	pItemEffect->HJOpAddRefillMP = 0;
	pItemEffect->HJOpAddDefenseSuccessRatePvP = 0;
	pItemEffect->HJOpAddDamageDecrease = 0;
	pItemEffect->HJOpAddSDRate = 0;
}



BOOL CJewelOfHarmonySystem::IsEnableToMakeSmeltingStoneItem(CItem * pItem)
{
	if ( this->_GetItemType(pItem) == JEWELOFHARMONY_ITEM_TYPE_NULL )
		return FALSE;

	if ( this->IsStrengthenByJewelOfHarmony(pItem) == TRUE )
		return FALSE;

	if ( pItem->IsSetItem() == TRUE && g_ConfigRead.data.common.AncHarmonyLimit == false )
		return FALSE;

	std::map<int,int>::iterator iter = this->m_mapEnableMixList.find(pItem->m_Type);

	if ( iter != this->m_mapEnableMixList.end() )
	{
		int reqLevel = iter->second;

		if ( pItem->m_Level < reqLevel )
			return FALSE;

	}

	return TRUE;
}

BOOL CJewelOfHarmonySystem::MakeSmeltingStoneItem_MultiMix(OBJECTSTRUCT * lpObj, int iMixCount)
{
	if (this->m_bSystemMixSmeltingStone != TRUE)
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0, 282), lpObj->m_Index, 1);
		return TRUE;
	}

	lpObj->ChaosLock = TRUE;
	int iValidItemCount = 0;
	int iNormalItemCount = 0;
	int iExtItemCount = 0;
	int iInvalidItemCount = 0;
	int iMakeSmeltingStoneMixPrice = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof(pMsg));
	pMsg.Result = CB_ERROR;

	for (int n = 0; n < CHAOS_BOX_SIZE; n++)
	{
		if (lpObj->pChaosBox[n].IsItem() == TRUE)
		{
			if (this->IsEnableToMakeSmeltingStoneItem(&lpObj->pChaosBox[n]) == TRUE)
			{
				iValidItemCount++;

				if (lpObj->pChaosBox[n].IsExtItem() == TRUE)
				{
					iExtItemCount++;
				}

				else
				{
					iNormalItemCount++;
				}
			}

			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if (iValidItemCount != iMixCount ||
		iInvalidItemCount)
	{
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return FALSE;
	}

	if (iNormalItemCount > 0 && iExtItemCount > 0)
	{
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return FALSE;
	}

	int JEWEL_OF_HARMONY_MAKE_SMELTINGSTONE_RATE;
	BOOL bIsItemNormal = TRUE;

	if (iExtItemCount > 0)
	{
		bIsItemNormal = FALSE;
		JEWEL_OF_HARMONY_MAKE_SMELTINGSTONE_RATE = this->m_iRateMixSmeltingStoneExt;
	}
	else
	{
		bIsItemNormal = TRUE;
		JEWEL_OF_HARMONY_MAKE_SMELTINGSTONE_RATE = this->m_iRateMixSmeltingStoneNor;
	}

	iMakeSmeltingStoneMixPrice = this->m_iZenForMixSmeltingStone * iMixCount;
	int iChaosTaxMoney = iMakeSmeltingStoneMixPrice * g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / 100;

	if (iChaosTaxMoney < 0)
		iChaosTaxMoney = 0;

	iMakeSmeltingStoneMixPrice += iChaosTaxMoney;

	if (iMakeSmeltingStoneMixPrice < 0)
		iMakeSmeltingStoneMixPrice = 0;

	if (lpObj->m_PlayerData->Money < iMakeSmeltingStoneMixPrice)
	{
		pMsg.Result = 2;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return FALSE;
	}

	lpObj->m_PlayerData->Money -= iMakeSmeltingStoneMixPrice;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);
	g_MixSystem.LogChaosItem(lpObj, "JewelOfHarmony][Smelt Item Mix");
	g_Log.Add("[JewelOfHarmony][Smelt Item Mix] - Mix Start");

	pMsg.Result = CB_MULTIMIX_RESULT;
	pMsg.Pos = iMixCount;

	memset(pMsg.ItemInfo, 0x00, sizeof(pMsg.ItemInfo));

	lpObj->ChaosMassMixCurrItem = 0;
	lpObj->ChaosMassMixSuccessCount = 0;

	for (int i = 0; i < iMixCount; i++)
	{
		int iRate = rand() % 100;
		if (iRate < JEWEL_OF_HARMONY_MAKE_SMELTINGSTONE_RATE)
		{
			BYTE btMixNumber = 1 << (i % 8);
			pMsg.ItemInfo[i / 8] |= btMixNumber;

			lpObj->ChaosMassMixSuccessCount++;

			g_Log.Add("[JewelOfHarmony][Smelt Item Mix] Smelting Stone Normal[%d] Mass Mix (%d) Success [%s][%s], Money(%d-%d) Rate(%d/%d)",
				bIsItemNormal, i, lpObj->AccountID, lpObj->Name,
				lpObj->m_PlayerData->Money, iMakeSmeltingStoneMixPrice, iRate,
				JEWEL_OF_HARMONY_MAKE_SMELTINGSTONE_RATE);
		}

		else
		{
			g_Log.Add("[JewelOfHarmony][Smelt Item Mix] Smelting Stone Normal[%d] Mass Mix Fail (%d) [%s][%s], Money : %d-%d Rate(%d/%d)",
				bIsItemNormal, i, lpObj->AccountID, lpObj->Name,
				lpObj->m_PlayerData->Money, iMakeSmeltingStoneMixPrice, iRate,
				JEWEL_OF_HARMONY_MAKE_SMELTINGSTONE_RATE);
		}
	}

	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

	if (lpObj->ChaosMassMixSuccessCount == 0)
	{
		g_MixSystem.ChaosBoxInit(lpObj);
		GSProtocol.GCUserChaosBoxSend(lpObj, 0);

		lpObj->ChaosLock = FALSE;
		return FALSE;
	}

	for (int i = 0; i < lpObj->ChaosMassMixSuccessCount; i++)
	{
		int iItemType;

		if (bIsItemNormal == TRUE)
			iItemType = this->JEWEL_OF_HARMONY_SMELT_NOR_ITEMINDEX;
		else
			iItemType = this->JEWEL_OF_HARMONY_SMELT_EXT_ITEMINDEX;

		ItemSerialCreateSend(lpObj->m_Index, 255, 0, 0, iItemType, 0, 1, 0, 0, 0, lpObj->m_Index, 0, 0, 0, 0, 0);
	}

	gObjInventoryCommit(lpObj->m_Index);
	return TRUE;
}

BOOL CJewelOfHarmonySystem::MakeSmeltingStoneItem(LPOBJ lpObj)
{
	if ( this->m_bSystemMixSmeltingStone != TRUE )
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,282), lpObj->m_Index, 1);
		return TRUE;
	}

	lpObj->ChaosLock = TRUE;
	int iValidItemCount = 0;
	int iInvalidItemCount = 0;
	int iMakeSmeltingStoneMixPrice = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof(pMsg));
	pMsg.Result = 0;
	int iItemPos = -1;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( this->IsEnableToMakeSmeltingStoneItem(&lpObj->pChaosBox[n]) == TRUE )
			{
				iValidItemCount++;
				iItemPos = n;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( iValidItemCount != 1 ||
		 iInvalidItemCount ||
		 iItemPos == -1 )
	{
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return FALSE;
	}

	int JEWEL_OF_HARMONY_MAKE_SMELTINGSTONE_RATE;
	BOOL bIsItemNormal = TRUE;

	if ( lpObj->pChaosBox[iItemPos].IsExtItem() == TRUE )
	{
		bIsItemNormal = FALSE;
		JEWEL_OF_HARMONY_MAKE_SMELTINGSTONE_RATE = this->m_iRateMixSmeltingStoneExt;
	}
	else
	{
		bIsItemNormal = TRUE;
		JEWEL_OF_HARMONY_MAKE_SMELTINGSTONE_RATE = this->m_iRateMixSmeltingStoneNor;
	}

	iMakeSmeltingStoneMixPrice = this->m_iZenForMixSmeltingStone;
	int iChaosTaxMoney = iMakeSmeltingStoneMixPrice * g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / 100;

	if ( iChaosTaxMoney < 0 )
		iChaosTaxMoney = 0;

	iMakeSmeltingStoneMixPrice += iChaosTaxMoney;

	if ( iMakeSmeltingStoneMixPrice < 0 )
		iMakeSmeltingStoneMixPrice = 0;

	if ( lpObj->m_PlayerData->Money < iMakeSmeltingStoneMixPrice )
	{
		pMsg.Result = 2;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return FALSE;
	}

	lpObj->m_PlayerData->Money -= iMakeSmeltingStoneMixPrice;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);
	g_MixSystem.LogChaosItem(lpObj, "JewelOfHarmony][Smelt Item Mix");
	g_Log.Add("[JewelOfHarmony][Smelt Item Mix] - Mix Start");

	int iRate = rand() % 100;

	if ( iRate < JEWEL_OF_HARMONY_MAKE_SMELTINGSTONE_RATE )
	{
		int iItemType;

		if ( bIsItemNormal == TRUE )
			iItemType = this->JEWEL_OF_HARMONY_SMELT_NOR_ITEMINDEX;
		else
			iItemType = this->JEWEL_OF_HARMONY_SMELT_EXT_ITEMINDEX;

		ItemSerialCreateSend(lpObj->m_Index, 255, 0, 0, iItemType, 0,
							1, 0, 0, 0, lpObj->m_Index, 0, 0, 0, 0, 0);
		gObjInventoryCommit(lpObj->m_Index);

		g_Log.Add("[JewelOfHarmony][Smelt Item Mix] Smelting Stone Normal[%d] Mix Success [%s][%s], Money(%d-%d) Rate(%d/%d)",
			bIsItemNormal, lpObj->AccountID, lpObj->Name,
			lpObj->m_PlayerData->Money, iMakeSmeltingStoneMixPrice, iRate,
			JEWEL_OF_HARMONY_MAKE_SMELTINGSTONE_RATE);
	}
	else
	{
		g_MixSystem.ChaosBoxInit(lpObj);
		GSProtocol.GCUserChaosBoxSend(lpObj, 0);
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		g_Log.Add("[JewelOfHarmony][Smelt Item Mix] Smelting Stone Normal[%d] Mix Fail [%s][%s], Money : %d-%d Rate(%d/%d)",
			bIsItemNormal, lpObj->AccountID, lpObj->Name,
			lpObj->m_PlayerData->Money, iMakeSmeltingStoneMixPrice, iRate,
			JEWEL_OF_HARMONY_MAKE_SMELTINGSTONE_RATE);
	}

	lpObj->ChaosLock = FALSE;
	return TRUE;
}



BOOL CJewelOfHarmonySystem::SmeltItemBySmeltingStone(LPOBJ lpObj, int source, int target)
{
	if ( this->m_bSystemSmeltingItem == FALSE )
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,280), lpObj->m_Index, 1);
		return FALSE;
	}

	if ( source < 0 || source > MAIN_INVENTORY_SIZE-1 )
		return FALSE;

	if ( target < 0 || target > MAIN_INVENTORY_SIZE-1 )
		return FALSE;

	if ( lpObj->pInventory[source].IsItem() == FALSE )
		return FALSE;

	if ( lpObj->pInventory[target].IsItem() == FALSE )
		return FALSE;

	CItem * pSource = &lpObj->pInventory[source];
	CItem * pTarget = &lpObj->pInventory[target];

	if ( !this->IsStrengthenByJewelOfHarmony(pTarget)  )
	{
		g_Log.Add("[JewelOfHarmony][Smelt Item] Not Strengten Item [%s][%s]",
			lpObj->AccountID, lpObj->Name);

		return FALSE;
	}

	int iItemOptionLevel = this->_GetItemOptionLevel(pTarget);

	if ( iItemOptionLevel >= pTarget->m_Level || iItemOptionLevel == 13 )
	{
		if (pTarget->m_Level >= 13 )
		{
			GSProtocol.GCServerMsgStringSend(Lang.GetText(0,270), lpObj->m_Index, 1);
		}
		else
		{
			GSProtocol.GCServerMsgStringSend(Lang.GetText(0,269), lpObj->m_Index, 1);
		}

		g_Log.Add("[JewelOfHarmony][Smelt Item] Already Have Max OptionLevel [%s][%s] OptionLevel [%d] ItemLevel [%d]",
			lpObj->AccountID, lpObj->Name, iItemOptionLevel, pTarget->m_Level);

		return FALSE;
	}

	if ( this->_GetItemType(pTarget) == JEWELOFHARMONY_ITEM_TYPE_WEAPON )
	{
		if ( this->GetItemStrengthenOption(pTarget) == AT_JEWELOFHARMONY_WEAPON_IMPROVE_MINATTACKDAMAGE )
		{
			int iNextLevel = this->_GetItemOptionLevel(pTarget)+1;
			BYTE iValue = this->m_itemOption[JEWELOFHARMONY_ITEM_TYPE_WEAPON][AT_JEWELOFHARMONY_WEAPON_IMPROVE_MINATTACKDAMAGE].iItemEffectValue[iNextLevel];

			if ( (pTarget->m_DamageMin+iValue) > (pTarget->m_DamageMax-1) )
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,270), lpObj->m_Index, 1);
				return FALSE;
			}
		}
	}

	int iSuccessRate = rand() % 100;
	int iRateSmeltingSuccess = 0;
	BOOL bIsNormalSmeltingStone = this->_IsJewelOfHarmonySmeltingItemNor(pSource->m_Type);

	if ( bIsNormalSmeltingStone == TRUE )
		iRateSmeltingSuccess = this->m_iRateSmeltingSuccessNor;
	else
		iRateSmeltingSuccess = this->m_iRateSmeltingSuccessExt;

	if ( iSuccessRate >= iRateSmeltingSuccess )
	{
		int iItemOptionNewLevel = _GetItemOptionRequireLevel(pTarget);
		pTarget->m_JewelOfHarmonyOption = pTarget->m_JewelOfHarmonyOption & 0xF0;
		pTarget->m_JewelOfHarmonyOption |= iItemOptionNewLevel & 0x0F;
		this->ShowStrengthenOption(pTarget);

		g_Log.Add("[JewelOfHarmony][Smelt Item] Smelt Item Fail by Normal[%d] [%s][%s] Name[%s] Type [%d] Serial [%I64d] Rate(%d/%d) Level(%d->%d)",
			bIsNormalSmeltingStone, lpObj->AccountID, lpObj->Name, pTarget->GetName(), pTarget->m_Type,
			pTarget->m_Number, iSuccessRate, iRateSmeltingSuccess, iItemOptionLevel, iItemOptionNewLevel);
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,276), lpObj->m_Index, 1);
	}
	else
	{
		int iItemOptionNewLevel = iItemOptionLevel+1;
		pTarget->m_JewelOfHarmonyOption = pTarget->m_JewelOfHarmonyOption & 0xF0;
		pTarget->m_JewelOfHarmonyOption |= iItemOptionNewLevel & 0x0F;
		this->ShowStrengthenOption(pTarget);

		g_Log.Add("[JewelOfHarmony][Smelt Item] Smelt Item Success by Normal[%d] [%s][%s] Name[%s] Type [%d] Serial [%I64d] Rate(%d/%d) Level(%d->%d)",
			bIsNormalSmeltingStone, lpObj->AccountID, lpObj->Name, pTarget->GetName(), pTarget->m_Type,
			pTarget->m_Number, iSuccessRate, iRateSmeltingSuccess, iItemOptionLevel, iItemOptionNewLevel & 0x0F);
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,277), lpObj->m_Index, 1);
	}

	return TRUE;
}

int CJewelOfHarmonySystem::_GetZenForRestoreItem(CItem * pItem)
{
	int iItemType = this->_GetItemType(pItem);

	if ( iItemType == JEWELOFHARMONY_ITEM_TYPE_NULL )
		return -1;

	int iItemOptionType = AT_JEWELOFHARMONY_NOT_STRENGTHEN_ITEM;
	iItemOptionType = this->GetItemStrengthenOption(pItem);

	if ( iItemOptionType == AT_JEWELOFHARMONY_NOT_STRENGTHEN_ITEM )
		return -1;

	int iItemOptionLevel = this->_GetItemOptionLevel(pItem);

	if ( iItemOptionLevel >= MAX_JOH_ITEM_OPTION )
		return -1;

	int iZenForRestore = this->m_itemOption[iItemType][iItemOptionType].iZenForRestore[iItemOptionLevel];
	return iZenForRestore;
}

BOOL CJewelOfHarmonySystem::RestoreStrengthenItem(LPOBJ lpObj)
{
	if ( this->m_bSystemRestoreStrengthen != TRUE )
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,283), lpObj->m_Index, 1);
		return TRUE;
	}

	lpObj->ChaosLock = TRUE;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof(pMsg));
	pMsg.Result = 0;
	int iStrengtenItemCount = 0;
	int iInvalidItemCount = 0;
	CItem * pItem = NULL;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( this->IsStrengthenByJewelOfHarmony(&lpObj->pChaosBox[n]) == TRUE )
			{
				iStrengtenItemCount++;
				pItem = &lpObj->pChaosBox[n];
			}
		}
	}

	if ( iStrengtenItemCount != 1 )
	{
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return FALSE;
	}
	
	int iItemOption = this->GetItemStrengthenOption(pItem);
	int iItemOptionLevel = this->_GetItemOptionLevel(pItem);
	int JEWEL_OF_HARMONY_RETORE_NEEDZEN = this->_GetZenForRestoreItem(pItem);

	if ( JEWEL_OF_HARMONY_RETORE_NEEDZEN < 0 ) 
	{
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return FALSE;
	}

	int iChaosTaxMoney = JEWEL_OF_HARMONY_RETORE_NEEDZEN * g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / 100;

	if ( iChaosTaxMoney < 0 )
		iChaosTaxMoney = 0;

	JEWEL_OF_HARMONY_RETORE_NEEDZEN += iChaosTaxMoney;

	if ( JEWEL_OF_HARMONY_RETORE_NEEDZEN < 0 )
		JEWEL_OF_HARMONY_RETORE_NEEDZEN = 0;

	if ( lpObj->m_PlayerData->Money < JEWEL_OF_HARMONY_RETORE_NEEDZEN )
	{
		pMsg.Result = 2;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return FALSE;
	}

	lpObj->m_PlayerData->Money -= JEWEL_OF_HARMONY_RETORE_NEEDZEN;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);
	g_MixSystem.LogChaosItem(lpObj, "JewelOfHarmony][Restore Item");
	g_Log.Add("[JewelOfHarmony][Restore Item] - Restore Start");

	g_Log.Add("[JewelOfHarmony][Restore Item] Restore Strengtened Item [%s][%s] Name[%s] ItemType[%d] Serial[%I64d] OptionType[%d] OptionLevel [%d] Money %d-%d",
		lpObj->AccountID, lpObj->Name, pItem->GetName(), pItem->m_Type,
		pItem->m_Number, iItemOption, iItemOptionLevel, 
		lpObj->m_PlayerData->Money, JEWEL_OF_HARMONY_RETORE_NEEDZEN);

	pItem->m_JewelOfHarmonyOption = 0;
	GSProtocol.GCUserChaosBoxSend(lpObj, 0);
	this->ShowStrengthenOption(pItem);
	lpObj->ChaosLock = FALSE;

	return TRUE;
}

BOOL CJewelOfHarmonySystem::NpcJewelOfHarmony(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( lpObj->m_IfState.use > 0 )
		return TRUE;

	PMSG_TALKRESULT pResult;

	pResult.h.c = 0xC3;
	pResult.h.headcode = 0x30;
	pResult.h.size = sizeof(pResult);
	
	if ( lpNpc->Class == 368 )
	{
		pResult.result = 17;
		pResult.level1 = this->m_iRatePuritySuccess;

		if ( this->m_bSystemPrutiyJewel != TRUE )
		{
			GSProtocol.GCServerMsgStringSend(Lang.GetText(0,279), lpObj->m_Index, 1);
			return TRUE;
		}

		if ( !this->IsEnableToUsePuritySystem() )
		{
			GSProtocol.GCServerMsgStringSend(Lang.GetText(0,279), lpObj->m_Index, 1);
			g_Log.Add("[JewelOfHarmony][%s][%s] Not Purity Time ",
				lpObj->AccountID, lpObj->Name);

			return TRUE;
		}
	}
	else  if ( lpNpc->Class == 369 )
	{
		pResult.result = 18;
		pResult.level1 = this->m_iRateMixSmeltingStoneNor;
		pResult.level2 = this->m_iRateMixSmeltingStoneExt;

		if ( this->m_bSystemMixSmeltingStone != TRUE )
		{
			GSProtocol.ChatTargetSend(lpNpc, Lang.GetText(0,282), lpObj->m_Index);
			return TRUE;
		}
	}
	else if ( lpNpc->Class == 370 )
	{
		pResult.result = 19;

		if ( this->m_bSystemRestoreStrengthen != TRUE )
		{
			GSProtocol.ChatTargetSend(lpNpc, Lang.GetText(0,283), lpObj->m_Index);
			return TRUE;
		}
	}
	else
	{
		g_Log.Add("[JewelOfHarmony][%s][%s] Open Chaos Box Failed NpcType [%d]",
			lpObj->AccountID, lpObj->Name, lpNpc->Class);

		return TRUE;
	}

	if ( bCanChaosBox == TRUE )
	{
		if ( lpObj->m_bPShopOpen == true )
		{
			g_Log.Add("[JewelOfHarmony][%s][%s] is Already Opening PShop, ChaosBox Failed",
				lpObj->AccountID, lpObj->Name);

			GSProtocol.GCServerMsgStringSend(Lang.GetText(0,112), lpObj->m_Index, 1);
			return TRUE;
		}

		lpObj->m_IfState.type = 7;
		lpObj->m_IfState.state = 0;
		lpObj->m_IfState.use = 1;
		lpObj->bIsChaosMixCompleted = false;

		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
		gObjInventoryTrans(lpObj->m_Index);

		g_Log.Add("[JewelOfHarmony][%s][%s] Open Chaos Box",
			lpObj->AccountID, lpObj->Name);

		gObjItemTextSave(lpObj);
		gObjWarehouseTextSave(lpObj);
		GSProtocol.GCAnsCsMapSvrTaxInfo(lpObj->m_Index, 1, g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index));
	}

	return TRUE;
}


BOOL CJewelOfHarmonySystem::IsEnableToTrade(OBJECTSTRUCT * lpObj) 
{
	BOOL bRet = TRUE;

	if(g_ConfigRead.data.common.TradeHarmonyBlock == TRUE)
		return TRUE;

	for ( int n=0;n<TRADE_BOX_SIZE;n++)
	{
		if ( lpObj->Trade[n].IsItem() == TRUE )
		{
			if ( this->IsStrengthenByJewelOfHarmony(&lpObj->Trade[n]) == TRUE )
			{
				bRet = FALSE;
			}
		}
	}

	return bRet;
}



//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

