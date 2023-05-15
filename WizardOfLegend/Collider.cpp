#include "stdafx.h"
#include "Collider.h"
#include "Obj.h"
#include "Transform.h"
#include "CameraMgr.h"
#include "GameMgr.h"
UINT CCollider::g_iNextID = 0;

CCollider::CCollider(CObj * _pOwner)
	: CComponent(_pOwner)
	, m_iID(g_iNextID++)
	, m_iCol(0)
{
	ZeroMemory(&m_vOffset, sizeof(VEC2));
	ZeroMemory(&m_vPos, sizeof(VEC2));
	ZeroMemory(&m_vScale, sizeof(VEC2));

	m_eType = COMPONENT_TYPE::COLLIDER;
}

CCollider::CCollider(const CCollider & _rhs)
	: CComponent(nullptr)
	, m_vOffset(_rhs.m_vOffset)
	, m_vScale(_rhs.m_vScale)
	, m_iID(g_iNextID++)
	, m_iCol(_rhs.m_iCol)
{
}

CCollider::~CCollider()
{

}

void CCollider::Init()
{
}

void CCollider::LateUpdate() 
{
	if (!m_bActive) return;

	m_vPos = m_pOwner->Get_Transform()->Get_Pos() + m_vOffset;
}

void CCollider::Render(HDC _hDC)
{
	if (!m_bActive) return;

	
	if (!CGameMgr::Get_Inst()->Is_DevMode() || !CGameMgr::Get_Inst()->Is_Dev_Render()) return;

	m_vPos = CCameraMgr::Get_Inst()->Get_RenderPos(m_vPos);
	Graphics graphics(_hDC);
	Pen pen(Color(255, 0, 255, 0), 3);

	if (m_iCol)
		pen.SetColor(Color(255, 255, 0, 0));

	graphics.DrawRectangle(&pen,	m_vPos.fX - (m_vScale.fX * 0.5f),
									m_vPos.fY - (m_vScale.fY * 0.5f),
									m_vScale.fX,
									m_vScale.fY);
}

void CCollider::OnCollision_Enter(CCollider * _pCollider, const float& _fCX, const float& _fCY)
{
	++m_iCol;
	m_pOwner->OnCollision_Enter(_pCollider, _fCX, _fCY);
}

void CCollider::OnCollision_Stay(CCollider * _pCollider, const float& _fCX, const float& _fCY)
{
	m_pOwner->OnCollision_Stay(_pCollider, _fCX, _fCY);
}

void CCollider::OnCollision_Exit(CCollider * _pCollider, const float& _fCX, const float& _fCY)
{
	--m_iCol;
	m_pOwner->OnCollision_Exit(_pCollider, _fCX, _fCY);
}
