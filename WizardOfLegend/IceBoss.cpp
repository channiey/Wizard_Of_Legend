#include "stdafx.h"
#include "IceBoss.h"
#include "AI.h"
#include "CameraMgr.h"
#include "Transform.h"
#include "Collider.h"
#include "IcebossAnimator.h"
#include "Texture.h"
#include "Animation.h"
#include "ResMgr.h"
#include "IdleState_IceBoss.h"
#include "AttackState1_IceBoss.h"
#include "AttackState2_IceBoss.h"
#include "AttackState3_IceBoss.h"
#include "Tile.h"
#include "Projectile.h"
#include "Rigidbody.h"
#include "TimeMgr.h"
#include "MainGame.h"
#include "DamageUI.h"
#include "ObjPool.h"
#include "BossStatUI.h"
#include "GameMgr.h"
#include "SoundMgr.h"

CIceBoss::CIceBoss(const OBJ_TYPE& _eType)
	: CEnemy(_eType)
	, m_bFirstAttacked(false)
	, m_bSecondAttacked(false)
	, m_fAcc(0.f)
	, m_fLimit(0.5f)
	, m_pStatUI(nullptr)
	, m_bSpawn(false)
	, m_fSpawnReadyTime(0.f)
	, m_bUpdate(false)
	, m_bSlow(false)
	, m_bDeadCycle(false)
	, m_bDeadCycleTime(0.f)
	, m_bStartDeadAni(false)
	, m_bBoom(false)
{
}


CIceBoss::~CIceBoss()
{
	Release();
}

const bool CIceBoss::Init()
{
	// Set Component
	Create_Transform();
	Create_Collider();
	Create_Rigidbody();

	m_pTransform->Init();
	m_pTransform->Set_Pos(DF_POS);
	m_pTransform->Set_Scale(VEC2(50, 50));

	m_pCollider->Init();
	m_pCollider->Set_Pos(DF_POS);
	m_pCollider->Set_OffSet(VEC2(0, m_pTransform->Get_Scale().fY * 0.5f));
	m_pCollider->Set_Scale(VEC2(100, 100));

	m_pAnimator = new CIcebossAnimator(this);
	m_pAnimator->Init();
	m_listComponent.push_back(m_pAnimator);
	Init_Animator();
	//m_pAnimator->Play(L"SPAWN", false);

	m_pRigidbody->Init();


	// Set AI
	m_pAI = new AI(this);
	m_pAI->Add_State(new CIdleState_IceBoss);
	m_pAI->Add_State(new CAttackState1_IceBoss);
	m_pAI->Add_State(new CAttackState2_IceBoss);
	m_pAI->Add_State(new CAttackState3_IceBoss);

	m_pAI->Set_CurState(L"IDLE");
	//m_pAI->Get_CurState()->Enter();

	// Renser Setting
	m_fRenderMag = 4.5f;

	// Speed
	m_fSpeed = DF_SPEED * 0.2f;

	// HP
	m_fMaxHp = 150.f;
	m_fCurHP = m_fMaxHp;

	// Set UI
	m_pStatUI = new CBossStatUI(OBJ_TYPE::UI, UI_TYPE::FIXED, this);
	m_pStatUI->Init();
	m_pStatUI->Set_Active(false);

	return true;
}

void CIceBoss::Update()
{			

	if (m_bDeadCycle && !m_bStartDeadAni) // 움직임 멈추고 카메라 셰이킹 시작
	{
		m_bDeadCycleTime += DT;

		if (m_bDeadCycleTime > 2.f) // 콜리전 엔터시 카메라 셰이킹 시간도 바꿔야 한다.
		{
			m_pAnimator->Play(L"DEAD", false);
			m_bStartDeadAni = true;
			m_fRenderMag = 12.f;
			m_bDeadCycleTime = 0.f;
		}

		return;
	}

	if (m_bStartDeadAni && !m_bBoom) // 번쩍후 터질 때 셰이킹
	{
		m_bDeadCycleTime += DT;

		if (m_bDeadCycleTime > 0.8f)
		{
			CCameraMgr::Get_Inst()->Shake_Camera(0.6f, 80.f);
			m_bBoom = true;
			CSoundMgr::Get_Inst()->StopSound(SOUND_BOSS);
			CSoundMgr::Get_Inst()->PlaySound(L"PlayerDead.wav", SOUND_BOSS, 1.f);
			//CCameraMgr::Get_Inst()->Fade_Out();
		}
	}
	if (m_bSlow) return;

	if (!m_bActive || !m_bSpawn) return;

	if (m_bSpawn && !m_bUpdate) // 첫 스폰후 플레이어에게로 카메라 무빙 전까지 대기시간
	{
		m_fSpawnReadyTime += DT;

		if (m_fSpawnReadyTime > 2.f)
			m_bUpdate = true;
		else
			return;
	}

	// (넉백 후 일정시간 도과 후 벨로시티 초기화를 위해 설정
	if (m_bFirstAttacked) // 첫 번째 타격에 맞음
	{
		m_fAcc += DT;

		if (m_fAcc >= m_fLimit) // 제한시간이 끝난 후
		{
			if (!m_bSecondAttacked) // 두 번째 타격이 없었다
			{
				m_bFirstAttacked = false;
				Get_Rigidbody()->Zero_Velocity();
			}
			else // 두번째 타격이 있었다.
			{
				m_bSecondAttacked = false;
			}
			m_fAcc = 0.f;
		}
	}

	if (m_bDeadCycle) return;
	CEnemy::Update();
}

void CIceBoss::LateUpdate()
{
	if (!m_bActive) return;

	Component_LateUpdate();
}

void CIceBoss::Render(HDC _hDC)
{
	if (!m_bActive) return;

	if (m_bSpawn && !m_bDeadCycle)
	{
		// Render Shadow
		Graphics g(CMainGame::Get_Inst()->Get_SubDC());;
		int iShadowWidth = 100, iShadowHeight = 80;
		VEC2 vPos = CCameraMgr::Get_Inst()->Get_RenderPos(m_pTransform->Get_Pos());
		Rect rc{ INT(vPos.fX - iShadowWidth * 0.5f),
			INT(vPos.fY - iShadowHeight * 0.5f + m_pTransform->Get_Scale().fY * 2.f),
			INT(iShadowWidth),
			INT(iShadowHeight) };
		SolidBrush brush(Color(120, 10, 10, 10));
		g.FillEllipse(&brush, rc);
	}

	// Render Component
	Component_Render(_hDC);
	 
	if (m_bBoom)
	{
		return;
	}
	// Render State (Line)
	if(!m_bDeadCycle)
		CEnemy::Render(_hDC);
}

void CIceBoss::Release()
{
	Safe_Delete<AI*>(m_pAI);
}

void CIceBoss::OnCollision_Enter(CCollider * _pOtherCol, const float & _fCX, const float & _fCY)
{
	if (!m_bActive) return;
	CObj* pOther = _pOtherCol->Get_Owner();

	switch (pOther->Get_ObjType())
	{
	case OBJ_TYPE::PLAYER:
		break;
	case OBJ_TYPE::ENEMY:
		break;
	case OBJ_TYPE::NPC:
		break;
	case OBJ_TYPE::PROJECTILE:
	{
		if (OBJ_TYPE::PLAYER == static_cast<CProjectile*>(pOther)->Get_Owner()->Get_ObjType())
		{
			// (넉백 후 일정시간 도과 후 벨로시티 초기화를 위해 설정
			if (!m_bFirstAttacked) // 첫번째 타격
			{
				m_bFirstAttacked = true;
				/*CSoundMgr::Get_Inst()->StopSound(SOUND_BOSS);
				CSoundMgr::Get_Inst()->PlaySound(L"IceBossHurt0.wav", SOUND_BOSS, 1.f);*/
			}
			else
				if (!m_bSecondAttacked) // 두번째 타격
					m_bSecondAttacked = true;
	
			// Set HP
			float fAD = pOther->Get_AD();

			m_fCurHP -= fAD;

			if (m_fCurHP <= 0.f)
			{

				CCameraMgr::Get_Inst()->Shake_Camera(2.f, 10.f);
				m_pTransform->Set_Look(VEC2_DOWN);

			/*	CCameraMgr::Get_Inst()->Set_LookAt(CCameraMgr::Get_Inst()->Get_RealPos(m_pTransform->Get_Pos()), 0.5f);
				CCameraMgr::Get_Inst()->Set_Target(this);*/

				m_bDeadCycle = true;

				m_pRigidbody->Set_Active(false);
				m_pCollider->Set_Active(false);

				m_fCurHP = 0.f;

				CSoundMgr::Get_Inst()->StopSound(SOUND_BOSS);
				CSoundMgr::Get_Inst()->PlaySound(L"BossDead.wav", SOUND_BOSS, 1.f);

				CGameMgr::Get_Inst()->Plus_Kill();
				return;
			}

			// Set Knockback
			float fTheta = pOther->Get_Transform()->Get_Theta();

			if (fTheta == 0.f)
			{
				VEC2 vLook = pOther->Get_Transform()->Get_Look();
				if (fabs(vLook.fX) > fabs(vLook.fY))
				{
					m_pRigidbody->Zero_Velocity();
					m_pRigidbody->Add_Force(vLook * DF_KNOCKBACK_SPEED);
				}
				else
				{
					m_pRigidbody->Zero_Velocity();
					m_pRigidbody->Add_Force(vLook * DF_KNOCKBACK_SPEED * -1.f);
				}
			}
			else
			{
				VEC2 vKnockBack{ cosf(fTheta), sinf(fTheta) };
				vKnockBack.Normalize();
				if (fabs(vKnockBack.fX) > fabs(vKnockBack.fY))
				{
					m_pRigidbody->Zero_Velocity();
					m_pRigidbody->Add_Force(vKnockBack * DF_KNOCKBACK_SPEED);
				}
				else
				{
					m_pRigidbody->Zero_Velocity();
					m_pRigidbody->Add_Force(vKnockBack * DF_KNOCKBACK_SPEED * -1.f);
				}
			}
		

			// Damage UI
			CDamageUI* pUI = static_cast<CDamageUI*>(CObjPool<CDamageUI>::Get_Obj());
			if (nullptr != pUI)
			{
				pUI->Set_UIType(UI_TYPE::FIXED);
				CGameMgr::Get_Inst()->Add_PlayerToEnemyDamage(pUI->Get_Damage());
				pUI->Reset(m_pTransform->Get_Pos());
			}


			CSoundMgr::Get_Inst()->StopSound(SOUND_BOSS);
			CSoundMgr::Get_Inst()->PlaySound(L"IceBossHurt0.wav", SOUND_BOSS, 0.7f);
			
		}
	}
	break;
	case OBJ_TYPE::ITEM:
		break;
	case OBJ_TYPE::TILE:
	{
		if (TILE_TYPE::WALL == static_cast<CTile*>(_pOtherCol->Get_Owner())->Get_TileType())
		{
			VEC2 vLook = m_pTransform->Get_Look();

			if (0.f < vLook.fX && 0.f < vLook.fY)
			{

			}
			else if (0.f != vLook.fX && 0.f == vLook.fY)
			{
				if (pOther->Get_Transform()->Get_Pos().fX < m_pTransform->Get_Pos().fX)
					m_pTransform->Add_Pos_X(_fCX);
				else
					m_pTransform->Add_Pos_X(-_fCX);
			}
			else if (0.f != vLook.fY && 0.f == vLook.fX)
			{
				if (pOther->Get_Transform()->Get_Pos().fY < m_pTransform->Get_Pos().fY)
					m_pTransform->Add_Pos_Y(_fCY);
				else
					m_pTransform->Add_Pos_Y(-_fCY);
			}
		}
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

void CIceBoss::OnCollision_Stay(CCollider * _pOtherCol, const float & _fCX, const float & _fCY)
{
	if (!m_bActive) return;
}

void CIceBoss::OnCollision_Exit(CCollider * _pOtherCol, const float & _fCX, const float & _fCY)
{
	if (!m_bActive) return;
}

const bool CIceBoss::Init_Animator()
{
	CTexture* pTex;
	CAnimation* pAnimation;

	// IDLE
	{
		pAnimation = new CAnimation(L"IDLE", m_pAnimator);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_IDLE0", L"../Data/Sprite/04_Enemy/0_IceBoss/IceQueenIdle0.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_IDLE1", L"../Data/Sprite/04_Enemy/0_IceBoss/IceQueenIdle1.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_IDLE2", L"../Data/Sprite/04_Enemy/0_IceBoss/IceQueenIdle2.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_IDLE3", L"../Data/Sprite/04_Enemy/0_IceBoss/IceQueenIdle3.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_IDLE4", L"../Data/Sprite/04_Enemy/0_IceBoss/IceQueenIdle4.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_IDLE5", L"../Data/Sprite/04_Enemy/0_IceBoss/IceQueenIdle5.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN);

		m_pAnimator->PushBack_Ani(pAnimation);
	}

	// SLASH_CHARGE
	{
		pAnimation = new CAnimation(L"SLASH_CHARGE", m_pAnimator);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_SLASH_CHARGE0", L"../Data/Sprite/04_Enemy/0_IceBoss/IceQueenSlash0.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, 0.3f);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_SLASH_CHARGE1", L"../Data/Sprite/04_Enemy/0_IceBoss/IceQueenSlash1.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, 0.3f);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_SLASH_CHARGE2", L"../Data/Sprite/04_Enemy/0_IceBoss/IceQueenSlash2.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, 0.3f);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_SLASH_CHARGE3", L"../Data/Sprite/04_Enemy/0_IceBoss/IceQueenSlash3.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, 0.3f);

		m_pAnimator->PushBack_Ani(pAnimation);
	}

	// SLASH
	{
		pAnimation = new CAnimation(L"SLASH", m_pAnimator);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_SLASH0", L"../Data/Sprite/04_Enemy/0_IceBoss/IceQueenSlash4.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, 0.7f);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_SLASH1", L"../Data/Sprite/04_Enemy/0_IceBoss/IceQueenSlash5.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, 0.7f);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_SLASH2", L"../Data/Sprite/04_Enemy/0_IceBoss/IceQueenSlash6.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, 0.7f);

		m_pAnimator->PushBack_Ani(pAnimation);
	}

	// HANDUP_CHARGE
	{
		pAnimation = new CAnimation(L"HANDUP_CHARGE", m_pAnimator);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_HANDUP_0", L"../Data/Sprite/04_Enemy/0_IceBoss/IceQueen_2.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_HANDUP_1", L"../Data/Sprite/04_Enemy/0_IceBoss/IceQueen_3.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, 2.f);

		m_pAnimator->PushBack_Ani(pAnimation);
	}

	// HANDUP
	{
		pAnimation = new CAnimation(L"HANDUP", m_pAnimator);
		
		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_HANDUP_0", L"../Data/Sprite/04_Enemy/0_IceBoss/IceQueen_4.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, 2.f);

		m_pAnimator->PushBack_Ani(pAnimation);
	}

	// SPAWN
	{
		float fTime = 0.07f;

		pAnimation = new CAnimation(L"SPAWN", m_pAnimator);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_SPAWN_0", L"../Data/Sprite/04_Enemy/0_IceBoss/AirBossSpawn0.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_SPAWN_1", L"../Data/Sprite/04_Enemy/0_IceBoss/AirBossSpawn1.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_SPAWN_2", L"../Data/Sprite/04_Enemy/0_IceBoss/AirBossSpawn2.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_SPAWN_3", L"../Data/Sprite/04_Enemy/0_IceBoss/AirBossSpawn3.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_SPAWN_4", L"../Data/Sprite/04_Enemy/0_IceBoss/AirBossSpawn4.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_SPAWN_5", L"../Data/Sprite/04_Enemy/0_IceBoss/AirBossSpawn5.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_SPAWN_6", L"../Data/Sprite/04_Enemy/0_IceBoss/AirBossSpawn6.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_SPAWN_7", L"../Data/Sprite/04_Enemy/0_IceBoss/AirBossSpawn7.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_SPAWN_8", L"../Data/Sprite/04_Enemy/0_IceBoss/AirBossSpawn8.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_SPAWN_9", L"../Data/Sprite/04_Enemy/0_IceBoss/AirBossSpawn9.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_SPAWN_10", L"../Data/Sprite/04_Enemy/0_IceBoss/AirBossSpawn10.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);

		/*pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_SPAWN_11", L"../Data/Sprite/04_Enemy/0_IceBoss/AirBossSpawn11.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_SPAWN_12", L"../Data/Sprite/04_Enemy/0_IceBoss/AirBossSpawn12.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_SPAWN_13", L"../Data/Sprite/04_Enemy/0_IceBoss/AirBossSpawn13.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_SPAWN_14", L"../Data/Sprite/04_Enemy/0_IceBoss/AirBossSpawn14.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);*/

		m_pAnimator->PushBack_Ani(pAnimation);
	}

	// DEAD
	{
		float fTime = 0.05f;

		pAnimation = new CAnimation(L"DEAD", m_pAnimator);


		// 삐까뻔쩍
		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_DEAD_0", L"../Data/Sprite/04_Enemy/0_IceBoss/SlashHitSpark_0.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_DEAD_1", L"../Data/Sprite/04_Enemy/0_IceBoss/SlashHitSpark_1.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_DEAD_2", L"../Data/Sprite/04_Enemy/0_IceBoss/SlashHitSpark_2.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_DEAD_3", L"../Data/Sprite/04_Enemy/0_IceBoss/SlashHitSpark_3.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_DEAD_4", L"../Data/Sprite/04_Enemy/0_IceBoss/SlashHitSpark_4.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_DEAD_5", L"../Data/Sprite/04_Enemy/0_IceBoss/SlashHitSpark_5.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_DEAD_6", L"../Data/Sprite/04_Enemy/0_IceBoss/SlashHitSpark_6.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);

		fTime = 0.07f;
		// 폭발 전조 빛 1
		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_DEAD_7", L"../Data/Sprite/04_Enemy/0_IceBoss/ExplosionCircle0.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_DEAD_8", L"../Data/Sprite/04_Enemy/0_IceBoss/ExplosionCircle1.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);

		// 폭발 전조 링
		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_DEAD_9", L"../Data/Sprite/04_Enemy/0_IceBoss/ExplosionRing0.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_DEAD_10", L"../Data/Sprite/04_Enemy/0_IceBoss/ExplosionRing1.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_DEAD_11", L"../Data/Sprite/04_Enemy/0_IceBoss/ExplosionRing2.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_DEAD_12", L"../Data/Sprite/04_Enemy/0_IceBoss/ExplosionRing3.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_DEAD_13", L"../Data/Sprite/04_Enemy/0_IceBoss/ExplosionRing4.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_DEAD_14", L"../Data/Sprite/04_Enemy/0_IceBoss/ExplosionRing5.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);



		fTime = 0.07f;

		// 메인 폭발
		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_DEAD_15", L"../Data/Sprite/04_Enemy/0_IceBoss/Explosion0.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_DEAD_16", L"../Data/Sprite/04_Enemy/0_IceBoss/Explosion1.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_DEAD_17", L"../Data/Sprite/04_Enemy/0_IceBoss/Explosion2.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_DEAD_18", L"../Data/Sprite/04_Enemy/0_IceBoss/Explosion3.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_DEAD_19", L"../Data/Sprite/04_Enemy/0_IceBoss/Explosion4.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_DEAD_20", L"../Data/Sprite/04_Enemy/0_IceBoss/Explosion5.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_DEAD_21", L"../Data/Sprite/04_Enemy/0_IceBoss/Explosion6.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_DEAD_22", L"../Data/Sprite/04_Enemy/0_IceBoss/Explosion7.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_DEAD_23", L"../Data/Sprite/04_Enemy/0_IceBoss/Explosion8.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_DEAD_24", L"../Data/Sprite/04_Enemy/0_IceBoss/Explosion9.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_BOSS_DEAD_25", L"../Data/Sprite/04_Enemy/0_IceBoss/Explosion10.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fTime);

		m_pAnimator->PushBack_Ani(pAnimation);
	}

	return true;
}

void CIceBoss::Set_Active(const bool _bActive)
{
	m_bActive = _bActive;

	if (m_bActive)
	{
		m_pAI->Get_CurState()->Enter();

		if (nullptr != m_pStatUI && !m_pStatUI->Is_Active())
		{
		/*	m_pStatUI->Set_Active(true);
			CEventMgr::Get_Inst()->Add_Obj(m_pStatUI);*/
			m_pAnimator->Play(L"SPAWN", false);

			CSoundMgr::Get_Inst()->StopSound(SOUND_BOSS);
			CSoundMgr::Get_Inst()->PlaySound(L"WindWindup.wav", SOUND_BOSS, 1.f);
		}

		//CCameraMgr::Get_Inst()->Shake_Camera(1.f, 15.f);
	}
}

void CIceBoss::Set_UI_Active(const bool _bActive)
{
	if (_bActive)
	{
		if (nullptr != m_pStatUI && !m_pStatUI->Is_Active())
		{
			m_pStatUI->Set_Active(true);
			CEventMgr::Get_Inst()->Add_Obj(m_pStatUI);
		}
		//CCameraMgr::Get_Inst()->Shake_Camera(1.f, 15.f);
	}
}
