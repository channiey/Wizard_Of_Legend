#include "stdafx.h"
#include "ExplodingFireball_P.h"
#include "Transform.h"
#include "Collider.h"
#include "ExplodingFireballAnimator.h"
#include "Texture.h"
#include "ResMgr.h"
#include "Animation.h"
#include "TimeMgr.h"
#include "Obj.h"
#include "Tile.h"
#include "ObjPool.h"
#include "GameMgr.h"
#include "SoundMgr.h"
#include "FireParticle.h"
#include "EventMgr.h"

ExplodingFireball_P::ExplodingFireball_P(const OBJ_TYPE& _eType)
	: CProjectile(_eType)
	, m_fMoveRenderMag(3.5f)
	, m_fBombRenderMag(4.5f)
{
}

ExplodingFireball_P::~ExplodingFireball_P()
{
}

const bool ExplodingFireball_P::Init()
{
	// Component Setting
	Create_Transform();
	Create_Collider();
	
	m_pTransform->Init();
	//m_pTransform->Set_Pos(m_pOwner->Get_Transform()->Get_Pos());
	m_pTransform->Set_Scale(VEC2(50.f, 50.f));

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
	m_fSpeed = 1500.f;

	// Render Setting
	m_fRenderMag = m_fMoveRenderMag;

	return true;
}

void ExplodingFireball_P::Update()
{
	if (!m_bActive) return;

	/*if (Out_Of_Screen())
		Explode();*/

	if(m_bMove)
		Move();

	if (Create_Particle())
	{
		CFireParticle* pProj = static_cast<CFireParticle*>(CObjPool<CFireParticle>::Get_Obj());
		if (nullptr == pProj) return;
		pProj->Reset(this);
		CEventMgr::Get_Inst()->Add_Obj(pProj);
	}
}

void ExplodingFireball_P::LateUpdate()
{
	if (!m_bActive) return;
	Component_LateUpdate();
}

void ExplodingFireball_P::Render(HDC _hDC)
{
	if (!m_bActive) return;
	Component_Render(_hDC);
}

void ExplodingFireball_P::Release()
{
}

void ExplodingFireball_P::OnCollision_Enter(CCollider * _pOtherCol, const float& _fCX, const float& _fCY)
{
	switch (_pOtherCol->Get_Owner()->Get_ObjType())
	{
	case OBJ_TYPE::PLAYER:
		break;
	case OBJ_TYPE::ENEMY:
	{
		Explode();

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
		if (TILE_TYPE::WALL == static_cast<CTile*>(_pOtherCol->Get_Owner())->Get_TileType())
			Explode();
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

void ExplodingFireball_P::OnCollision_Stay(CCollider * _pOtherCol, const float& _fCX, const float& _fCY)
{
}

void ExplodingFireball_P::OnCollision_Exit(CCollider * _pOtherCol, const float& _fCX, const float& _fCY)
{
}

const bool ExplodingFireball_P::Init_Animator()
{
	CTexture* pTex;
	CAnimation* pAnimation;
	float fDuration = 0.f;

	// FLY
	{
		pAnimation = new CAnimation(L"FLY", m_pAnimator);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"FIRE_BALL_FLY_1", L"../Data/Sprite/06.Projectile/Fire/Fireball_1.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN);

		m_pAnimator->PushBack_Ani(pAnimation);
	}
	// BOOMB
	{

		fDuration = 0.04f;

		pAnimation = new CAnimation(L"BOOMB", m_pAnimator);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"FIRE_BOOMB_1", L"../Data/Sprite/06.Projectile/Fire/FireExplosion0.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"FIRE_BOOMB_2", L"../Data/Sprite/06.Projectile/Fire/FireExplosion1.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"FIRE_BOOMB_3", L"../Data/Sprite/06.Projectile/Fire/FireExplosion2.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"FIRE_BOOMB_4", L"../Data/Sprite/06.Projectile/Fire/FireExplosion3.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"FIRE_BOOMB_5", L"../Data/Sprite/06.Projectile/Fire/FireExplosion4.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"FIRE_BOOMB_6", L"../Data/Sprite/06.Projectile/Fire/FireExplosion5.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"FIRE_BOOMB_7", L"../Data/Sprite/06.Projectile/Fire/FireExplosion6.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

		m_pAnimator->PushBack_Ani(pAnimation);
	}
	return true;
}

void ExplodingFireball_P::Explode()
{
	m_pAnimator->Play(L"BOOMB", false);
	m_fRenderMag = m_fBombRenderMag;
	m_bMove = false;

	CSoundMgr::Get_Inst()->StopSound(SOUND_PLAYER);
	CSoundMgr::Get_Inst()->PlaySound(L"DelugeBomb.wav", SOUND_PLAYER, 0.7f);

	//Return(); 애니메이터가 폭발 애니메이션 종료된 후 반환시킨다.

	// 세타로 인한 폭발 애니메이션 회전을 막는다.
	m_pTransform->Set_Theta(0.f);
}

void ExplodingFireball_P::Move()
{
	float fTheta = m_pTransform->Get_Theta();

	float fDT = DT;
	if (CGameMgr::Get_Inst()->Get_FPS() != 10)
	{
		fDT = 0.016f;
	}

	m_pTransform->Add_Pos_X(m_fSpeed * cosf(fTheta) * fDT);
	m_pTransform->Add_Pos_Y(-m_fSpeed * sinf(fTheta) * fDT);
}

void ExplodingFireball_P::Reset(CObj * _pOwner, const VEC2 & _vPos, const float & _fDamage, const float & _fTheta)
{
	m_pOwner = _pOwner;
	m_pTransform->Set_Pos(_vPos);
	m_pCollider->Set_Pos(m_pTransform->Get_Pos());
	m_fAD = _fDamage;
	m_pTransform->Set_Theta(_fTheta);
	m_bMove = true;

	m_fRenderMag = m_fMoveRenderMag;

	m_pAnimator->Play(L"FLY", true);
}

void ExplodingFireball_P::Return()
{
	CObjPool<ExplodingFireball_P>::Return_Obj(this);
}
