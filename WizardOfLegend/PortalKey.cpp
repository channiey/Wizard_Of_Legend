#include "stdafx.h"
#include "PortalKey.h"
#include "ResMgr.h"
#include "Texture.h"
#include "Transform.h"
#include "CameraMgr.h"

CPortalKey::CPortalKey(const OBJ_TYPE& _eType, const UI_TYPE& _eUiType)
	: CUI(_eType, _eUiType)
	, m_pTex(nullptr)
{
	ZeroMemory(&m_vImgPos, sizeof(VEC2));
}


CPortalKey::~CPortalKey()
{
}

const bool CPortalKey::Init()
{
	Load_Tex();

	return true;
}

void CPortalKey::Render(HDC _hDC)
{
	if (!m_bActive) return;

	if (nullptr == m_pTex || nullptr == m_pOnwer) return;

	// Render Image
	UINT iWidth = m_pTex->Get_Image()->GetWidth();
	UINT iHeight = m_pTex->Get_Image()->GetHeight();

	float fMag = 2.5f;
	float fYterm = 150.f;

	VEC2 vPos = CCameraMgr::Get_Inst()->Get_RenderPos(m_pOnwer->Get_Transform()->Get_Pos());

	
	m_Graphics.DrawImage(m_pTex->Get_Image(),
		int(vPos.fX - iWidth * fMag * 0.5f),
		int(vPos.fY - iHeight * fMag * 0.5f - fYterm),
		int(iWidth * fMag),
		int(iHeight * fMag));
}

void CPortalKey::Load_Tex()
{
	m_pTex = CResMgr::Get_Inst()->Load_Tex(L"KEY_F", L"../Data/Sprite/08.Effect/Portal/F.png");
}
 