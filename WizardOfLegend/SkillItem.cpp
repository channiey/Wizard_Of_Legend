#include "stdafx.h"
#include "SkillItem.h"
#include "Transform.h"
#include "Collider.h"
#include "Texture.h"
#include "ResMgr.h"
#include "EventMgr.h"
#include "CameraMgr.h"
#include "MainGame.h"

CSkillItem::CSkillItem(const OBJ_TYPE& _eType, const ITEM_TYPE& _eItemType)
	: CItem(_eType, _eItemType)
{
}


CSkillItem::~CSkillItem()
{
}

const bool CSkillItem::Init()
{
	Create_Transform();
	m_pTransform->Init();
	m_pTransform->Set_Pos(VEC2_ZERO);
	m_pTransform->Set_Scale(VEC2{ 100, 100 });

	Create_Collider();
	m_pCollider->Init();
	m_pCollider->Set_Pos(m_pTransform->Get_Pos());
	m_pCollider->Set_Scale(m_pTransform->Get_Scale());

	Load_Image();

	m_bActive = false;

	return true;
}

void CSkillItem::Update()
{
}

void CSkillItem::LateUpdate()
{
	if (!m_bActive) return;







	Component_LateUpdate();
}

void CSkillItem::Render(HDC _hDC)
{
	if (!m_bActive) return;


	//Graphics g(CMainGame::Get_Inst()->Get_SubDC());
	int iShadowWidth = 70, iShadowHeight = 50;
	VEC2 vPos = CCameraMgr::Get_Inst()->Get_RenderPos(m_pTransform->Get_Pos());
	Rect rc{ INT(vPos.fX - iShadowWidth * 0.5f),
		INT(vPos.fY + 40),
		INT(iShadowWidth),
		INT(iShadowHeight) };
	SolidBrush brush(Color(100, 10, 10, 10));
	m_Graphics.FillRectangle(&brush, rc);



	// Render Image
	UINT iWidth = m_pTex->Get_Image()->GetWidth();
	UINT iHeight = m_pTex->Get_Image()->GetHeight();

	vPos = CCameraMgr::Get_Inst()->Get_RenderPos(m_pTransform->Get_Pos());

	float fMag = 4.f;

	m_Graphics.DrawImage(m_pTex->Get_Image(),
		int(vPos.fX - (iWidth * fMag * 0.5f)),
		int(vPos.fY - (iHeight * fMag * 0.5f)),
		int(iWidth * fMag),
		int(iHeight * fMag));
}

void CSkillItem::OnCollision_Enter(CCollider * _pOtherCol, const float & _fCX, const float & _fCY)
{
}

void CSkillItem::OnCollision_Stay(CCollider * _pOtherCol, const float & _fCX, const float & _fCY)
{
}

void CSkillItem::OnCollision_Exit(CCollider * _pOtherCol, const float & _fCX, const float & _fCY)
{
	if (_pOtherCol->Get_Owner()->Get_ObjType() == OBJ_TYPE::PLAYER)
	{
		CEventMgr::Get_Inst()->Delete_Obj(this);
	}
}

void CSkillItem::Load_Image()
{
	m_pTex = CResMgr::Get_Inst()->Load_Tex(L"SKILL_WATER_BOMB", L"../Data/Sprite/99_UI/skill/Skills #13665224.png");
}
