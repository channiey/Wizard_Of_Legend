#include "stdafx.h"
#include "GameMgr.h"

CGameMgr* CGameMgr::m_pInst = nullptr;

CGameMgr::CGameMgr()
	: m_bDevMode(false)
	, m_bFinish(false)
	, m_bDevText(false)
	, m_bDebRender(false)
	, m_iFPS(10)
	, m_iAmount_Damage_Sufferd(0)
	, m_iAmount_Damage_Inflected(0)
	, m_iKill(0)
{
}


CGameMgr::~CGameMgr()
{
}
