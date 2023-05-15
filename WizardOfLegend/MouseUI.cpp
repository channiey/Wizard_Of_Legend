#include "stdafx.h"
#include "MouseUI.h"
#include "ResMgr.h"
#include "InputMgr.h"
#include "CameraMgr.h"
#include "Texture.h"

CMouseUI::CMouseUI(const OBJ_TYPE& _eType, const UI_TYPE& _eUiType)
	: CUI(_eType, _eUiType)
	, m_pTex(nullptr)
{
}


CMouseUI::~CMouseUI()
{
}

const bool CMouseUI::Init()
{
	Load_Tex();
	return true;
}

void CMouseUI::Update()
{
}

void CMouseUI::LateUpdate()
{
}

void CMouseUI::Render(HDC _hDC)
{
	VEC2 vPos = VEC2{ CInputMgr::Get_Inst()->Get_Mouse_Pos().x, CInputMgr::Get_Inst()->Get_Mouse_Pos().y };
	
	UINT iWidth = m_pTex->Get_Image()->GetWidth();
	UINT iHeight = m_pTex->Get_Image()->GetHeight();

	float fMag = 2.5f;

	m_Graphics.DrawImage(m_pTex->Get_Image(),
		int(vPos.fX - (iWidth * fMag * 0.5f)),
		int(vPos.fY - (iHeight * fMag * 0.5f)),
		int(iWidth * fMag),
		int(iHeight * fMag));
}

void CMouseUI::Release()
{
}

void CMouseUI::Load_Tex()
{
	m_pTex = CResMgr::Get_Inst()->Load_Tex(L"MOUSE_UI", L"../Data/Sprite/99_UI/MouseCursor0.png");
}
