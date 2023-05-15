#include "stdafx.h"
#include "IceLance_P.h"
#include "Transform.h"
#include "Collider.h"
#include "Animator.h"
#include "ResMgr.h"
#include "ExplodingFireballAnimator.h"
#include "Tile.h"
#include "TimeMgr.h"
#include "EventMgr.h"
#include "Texture.h"
#include "Animation.h"
#include "ObjPool.h"
#include "SnowParticle.h"
#include "EventMgr.h"

CIceLance_P::CIceLance_P(const OBJ_TYPE& _eType)
	: CProjectile(_eType)
{
}


CIceLance_P::~CIceLance_P()
{
}

const bool CIceLance_P::Init()
{
	// Component Setting
	Create_Transform();
	Create_Collider();

	m_pTransform->Init();
	//m_pTransform->Set_Pos(m_pOwner->Get_Transform()->Get_Pos());
	m_pTransform->Set_Scale(VEC2(100.f, 100.f));

	m_pCollider->Init();
	//m_pCollider->Set_Pos(m_pTransform->Get_Pos());
	m_pCollider->Set_Scale(m_pTransform->Get_Scale());

	m_pAnimator = new CExplodingFireballAnimator(this);
	m_pAnimator->Init();
	m_pAnimator->Set_Use_Dir(false);
	m_listComponent.push_back(m_pAnimator);
	Init_Animator();

	m_pAnimator->Play(L"FLY", true);

	// Speed Setting
	m_fSpeed = 1200.f;

	// Render Setting
	m_fRenderMag = 3.f;

	return true;
}

void CIceLance_P::Update()
{
	if (!m_bActive) return;
	if (Out_Of_Screen())
		Explode();

	if (m_bMove)
		Move();

	if (Create_Particle())
	{
		CSnowParticle* pProj = static_cast<CSnowParticle*>(CObjPool<CSnowParticle>::Get_Obj());
		if (nullptr == pProj) return;
		pProj->Reset(this);
		CEventMgr::Get_Inst()->Add_Obj(pProj);
	}
}

void CIceLance_P::LateUpdate()
{
	if (!m_bActive) return;
	Component_LateUpdate();

}

void CIceLance_P::Render(HDC _hDC)
{
	if (!m_bActive) return;
	Component_Render(_hDC);

}

void CIceLance_P::Release()
{
}

void CIceLance_P::OnCollision_Enter(CCollider * _pOtherCol, const float & _fCX, const float & _fCY)
{
	switch (_pOtherCol->Get_Owner()->Get_ObjType())
	{
	case OBJ_TYPE::PLAYER:
	{
		//Explode();
	}
	break;
	case OBJ_TYPE::ENEMY:
	{

	}
	break;
	case OBJ_TYPE::NPC:
		break;
	case OBJ_TYPE::PROJECTILE:
		break;
	case OBJ_TYPE::ITEM:
		break;
	case OBJ_TYPE::TILE:
	{
		/*if (TILE_TYPE::WALL == static_cast<CTile*>(_pOtherCol->Get_Owner())->Get_TileType())
		Explode();*/
	}
	break;
	case OBJ_TYPE::EFFECT:
		break;
	case OBJ_TYPE::TYPEEND:
		break;
	default:
		break;
	}
}

void CIceLance_P::OnCollision_Stay(CCollider * _pOtherCol, const float & _fCX, const float & _fCY)
{
}

void CIceLance_P::OnCollision_Exit(CCollider * _pOtherCol, const float & _fCX, const float & _fCY)
{
}

void CIceLance_P::Reset(CObj * _pOwner, const VEC2 & _vPos, const float & _fDamage, const float & _fTheta)
{
	m_pOwner = _pOwner;
	m_pTransform->Set_Pos(_vPos);
	m_pCollider->Set_Pos(m_pTransform->Get_Pos());
	m_fAD = _fDamage;
	m_pTransform->Set_Theta(_fTheta);
	m_bMove = true;

	m_pAnimator->Play(L"FLY", true);
}

void CIceLance_P::Return()
{
	CObjPool<CIceLance_P>::Return_Obj(this);

}

const bool CIceLance_P::Init_Animator()
{
	CTexture* pTex;
	CAnimation* pAnimation;

	pAnimation = new CAnimation(L"FLY", m_pAnimator);

	pTex = CResMgr::Get_Inst()->Load_Tex(L"CIceLance", L"../Data/Sprite/04_Enemy/0_IceBoss/IceLanceLarge.png");
	pTex->Get_Image()->RotateFlip(Rotate90FlipNone);
	pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN);

	m_pAnimator->PushBack_Ani(pAnimation);

	return true;
}

void CIceLance_P::Explode()
{
	Return();
}

void CIceLance_P::Move()
{
	float fTheta = m_pTransform->Get_Theta();

	m_pTransform->Add_Pos_X(m_fSpeed * cosf(fTheta) * DT);
	m_pTransform->Add_Pos_Y(-m_fSpeed * sinf(fTheta) * DT);
}
