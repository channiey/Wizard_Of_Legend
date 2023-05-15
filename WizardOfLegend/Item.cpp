#include "stdafx.h"
#include "Item.h"
#include "MainGame.h"

CItem::CItem(const OBJ_TYPE& _eType, const ITEM_TYPE& _eItemType)
	: CObj(_eType)
	, m_eItemType(_eItemType)
	, m_Graphics(CMainGame::Get_Inst()->Get_SubDC())
{
}


CItem::~CItem()
{
}
