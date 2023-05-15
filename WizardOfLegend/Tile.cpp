#include "stdafx.h"
#include "Tile.h"
#include "Component.h"
#include "Transform.h"
#include "Collider.h"
#include "MainGame.h"
#include "CameraMgr.h"
#include "Texture.h"
#include "GameMgr.h"


CTile::CTile(const OBJ_TYPE& _eType)
	: CObj(_eType)
	, m_eTileType(TILE_TYPE::WALL)
	, m_Graphics(CMainGame::Get_Inst()->Get_SubDC())
	//, m_iIndex(0)
{
}


CTile::~CTile()
{
	Release();
}

const bool CTile::Init()
{
	Create_Transform();
	Create_Collider();

	m_pTransform->Init();
	m_pTransform->Set_Pos(VEC2{});
	m_pTransform->Set_Scale(VEC2{});

	m_pCollider->Init();
	m_pCollider->Set_Pos(VEC2{});
	m_pCollider->Set_Scale(VEC2{});

	m_bActive = true;
	return true;
}

void CTile::Update()
{
	return;
	//if (!m_bActive) return;
}

void CTile::LateUpdate()
{
	return;
	//if (!m_bActive) return;
	//return;
	Component_LateUpdate();
}

void CTile::Render(HDC _hDC)
{
	//if (!m_bActive) return;

	return;

	Component_Render(_hDC);

	//if (!CGameMgr::Get_Inst()->Is_DevMode() || !CGameMgr::Get_Inst()->Is_Dev_Render()) return;
	


	VEC2 vPos = CCameraMgr::Get_Inst()->Get_RenderPos(m_pTransform->Get_Pos());
	VEC2 vScale = m_pTransform->Get_Scale();

	Pen pen(Color(255, 0, 255, 0), 3);

	switch (m_eTileType)
	{
	/*case TILE_TYPE::GROUND:
	{
		pen.SetColor(Color(255, 0, 0, 0));
	}
		break;*/
	case TILE_TYPE::WALL:
	{
		pen.SetColor(Color(255, 0, 255, 0));
	}
		break;
	case TILE_TYPE::DEADZONE:
	{
		pen.SetColor(Color(255, 255, 0, 0));
	}
		break;
	case TILE_TYPE::TYPEEND:
		break;
	default:
		break;
	}

	m_Graphics.DrawRectangle(&pen, 
		vPos.fX - (vScale.fX * 0.5f),
		vPos.fY - (vScale.fY * 0.5f),
		vScale.fX,
		vScale.fY);
}

void CTile::Release()
{
}

void CTile::OnCollision_Enter(CCollider * _pOtherCol, const float& _fCX, const float& _fCY)
{
}

void CTile::OnCollision_Stay(CCollider * _pOtherCol, const float& _fCX, const float& _fCY)
{
}

void CTile::OnCollision_Exit(CCollider * _pOtherCol, const float& _fCX, const float& _fCY)
{
}

