#include "stdafx.h"
#include "Player.h"
#include "InputMgr.h"
#include "TimeMgr.h"
#include "CameraMgr.h"
#include "MainGame.h"
#include "ResMgr.h"
#include "Component.h"
#include "Transform.h"
#include "Collider.h"
#include "Animator.h"
#include "Animation.h"
#include "Res.h"
#include "Texture.h"
#include "GameMgr.h"
#include "PlayerAnimator.h"
#include "Skill.h"
#include "ExplodingFireball_S.h"
#include "DragonArc_S.h"
#include "WindSlash_S.h"
#include "Rigidbody.h"
#include "AirBurst_S.h"
#include "Transform.h"
#include "Tile.h"
#include "Projectile.h"
#include "TimeMgr.h"
#include "DamageUI.h"
#include "ObjPool.h"
#include "SoundMgr.h"
#include "WaterBomb_S.h"
#include "Item.h"
#include "Gold.h"

CPlayer::CPlayer(const OBJ_TYPE& _eType)
	: CObj(_eType)
	, m_bAttackType(false)
	, m_bPortalAni(true)
	, m_fStepTime(0.f)
{
	ZeroMemory(&m_vMouse, sizeof(VEC2));

	for (int i = 0; i < (UINT)SKILL_KEY_TYPE::TYPEEND; ++i)
		m_SkillList[i] = nullptr;
}


CPlayer::~CPlayer()
{
	Release();
}

const bool CPlayer::Init()
{
	// Component Setting
	Create_Transform();
	Create_Collider();
	Create_Rigidbody();

	m_pTransform->Init();
	m_pTransform->Set_Pos(DF_POS);
	m_pTransform->Set_Scale(VEC2(50, 50));

	m_pCollider->Init();
	m_pCollider->Set_Pos(DF_POS);
	m_pCollider->Set_OffSet(VEC2(0, m_pTransform->Get_Scale().fY * 0.5f));
	m_pCollider->Set_Scale(m_pTransform->Get_Scale());

	m_pAnimator = new CPlayerAnimator(this);
	m_pAnimator->Init();
	m_listComponent.push_back(m_pAnimator);
	Init_Animator();

	m_pAnimator->Play(L"IDLE", true); 

	m_pRigidbody->Init();

	// Skill Setting
	m_SkillList[(UINT)SKILL_KEY_TYPE::Q] = new CExplodingFireball_S(this,	 3.f,		 DF_AD);
	m_SkillList[(UINT)SKILL_KEY_TYPE::Q]->Init();

	m_SkillList[(UINT)SKILL_KEY_TYPE::RBTN] = new CDragonArc_S(		this,	 0.2f,		 DF_AD);
	m_SkillList[(UINT)SKILL_KEY_TYPE::RBTN]->Init();

	m_SkillList[(UINT)SKILL_KEY_TYPE::LBTN] = new CWindSlash_S(		this,	 0.3f,		 DF_AD);
	m_SkillList[(UINT)SKILL_KEY_TYPE::LBTN]->Init();

	m_SkillList[(UINT)SKILL_KEY_TYPE::SPACE] = new CAirBurst_S(		this,	 0.2f,		 DF_AD);
	m_SkillList[(UINT)SKILL_KEY_TYPE::SPACE]->Init();

	m_SkillList[(UINT)SKILL_KEY_TYPE::E] = new CWaterBomb_S(		this,	 5.f,		 DF_AD);
	m_SkillList[(UINT)SKILL_KEY_TYPE::E]->Init();

	// Renser Setting
	m_fRenderMag = 3.5f;

	// Dash Speed Setting
	m_fDashSpeed = 10000.f;

	// Set Gold
	m_iGold = DF_GOLD;

	// Set HP

	m_fMaxHp = 500.f;
	m_fCurHP = m_fMaxHp;
	
	m_bActive = true;
	return true;
}

void CPlayer::Update()
{
	if (!m_bActive) return;

	//cout << m_pTransform->Get_Pos().fX << " " << m_pTransform->Get_Pos().fY << endl;
	//if (m_bPortalAni) return;

	Move();

	Skill();

	Skill_Update();
}

void CPlayer::LateUpdate()
{
	if (!m_bActive) return;
	Component_LateUpdate();
}

void CPlayer::Render(HDC _hDC)
{
	if (!m_bActive) return;
	// Render Shadow

	Graphics g(CMainGame::Get_Inst()->Get_SubDC());
	int iShadowWidth = 70, iShadowHeight = 40;
	VEC2 vPos = CCameraMgr::Get_Inst()->Get_RenderPos(m_pTransform->Get_Pos());
	Rect rc{ INT(vPos.fX - iShadowWidth * 0.5f),
		INT(vPos.fY - iShadowHeight * 0.5f + m_pTransform->Get_Scale().fY),
		INT(iShadowWidth),
		INT(iShadowHeight) };
	SolidBrush brush(Color(120, 10, 10, 10));
	g.FillEllipse(&brush, rc);
	
	// Component Render
	Component_Render(_hDC);

	/*if (!CGameMgr::Get_Inst()->Is_DevMode() || !CGameMgr::Get_Inst()->Is_Dev_Render()) return;*/

	/*VEC2 vMouseWorld = VEC2{ CInputMgr::Get_Inst()->Get_Mouse_Pos().x, CInputMgr::Get_Inst()->Get_Mouse_Pos().y };
	VEC2 vPlayerPos = CCameraMgr::Get_Inst()->Get_RenderPos(m_pTransform->Get_Pos());
	Graphics g(CMainGame::Get_Inst()->Get_SubDC());
	pen(Color(100, 255, 255, 0), 5.f);
	pen.SetEndCap(LineCapArrowAnchor);
	g.DrawLine(&pen, PointF(vPlayerPos.fX, vPlayerPos.fY), PointF(vMouseWorld.fX, vMouseWorld.fY));*/
}

void CPlayer::Release()
{
	for (int i = 0; i < (UINT)SKILL_KEY_TYPE::TYPEEND; ++i)
		if(nullptr != m_SkillList[i])
			Safe_Delete<CSkill*>(m_SkillList[i]);
}

void CPlayer::OnCollision_Enter(CCollider * _pOtherCol, const float& _fCX, const float& _fCY)
{
	// 맞을 경우 
	//m_pAnimator->Play(L"HURT", false);
	//m_pAnimator->Play(L"DEAD", true);

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
		if (OBJ_TYPE::ENEMY == static_cast<CProjectile*>(pOther)->Get_Owner()->Get_ObjType())
		{
			// Set HP
			float fAD = pOther->Get_AD();

			m_fCurHP -= fAD;

			m_pAnimator->Play(L"HURT", false);

			if (m_fCurHP <= 0)
				m_bDead = true;

			// Set Knockback
			float fTheta = pOther->Get_Transform()->Get_Theta();
			VEC2 vKnockBack{ cosf(fTheta), sinf(fTheta) };
			vKnockBack.Normalize();

			if (fabs(vKnockBack.fX) > fabs(vKnockBack.fY))
			{
				m_pTransform->Set_Look(vKnockBack * -1.f);
				m_pRigidbody->Add_Force(vKnockBack * DF_KNOCKBACK_SPEED);
			}
			else
			{
				m_pTransform->Set_Look(vKnockBack * 1.f);
				m_pRigidbody->Add_Force(vKnockBack * DF_KNOCKBACK_SPEED * -1.f);
			}

			// Camera Shake & hurt
			CCameraMgr::Get_Inst()->Shake_Camera(0.3f, 10.f);
			CCameraMgr::Get_Inst()->Hurt_Camera(0.5f);

			// Damage UI
			CDamageUI* pUI = static_cast<CDamageUI*>(CObjPool<CDamageUI>::Get_Obj());
			if (nullptr != pUI)
			{
				pUI->Set_UIType(UI_TYPE::FIXED);
				CGameMgr::Get_Inst()->Add_EnemyToPlayerDamage(pUI->Get_Damage());
				pUI->Reset(m_pTransform->Get_Pos());
			}

			// Sound IceField

			CSoundMgr::Get_Inst()->StopSound(SOUND_PLAYER);
			CSoundMgr::Get_Inst()->PlaySound(L"IceField.wav", SOUND_PLAYER, 1.f);
		}
	}
		break;
	case OBJ_TYPE::ITEM:
	{
		switch (static_cast<CItem*>(pOther)->Get_ItemType())
		{
		case ITEM_TYPE::GOLD:
		{
			m_iGold += static_cast<CGold*>(pOther)->Get_Value();

			// Equip

			CSoundMgr::Get_Inst()->StopSound(SOUND_PLAYER);
			CSoundMgr::Get_Inst()->PlaySound(L"Equip.wav", SOUND_PLAYER, 1.f);
		}
		break;
		case ITEM_TYPE::SKILL:
		{

			CSoundMgr::Get_Inst()->StopSound(SOUND_PLAYER);
			CSoundMgr::Get_Inst()->PlaySound(L"Equip.wav", SOUND_PLAYER, 1.f);

			g_SkillUI = true;
			//g_SkillUI = true;
		}
		break;
		}
	}
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

void CPlayer::OnCollision_Stay(CCollider * _pOtherCol, const float& _fCX, const float& _fCY)
{
	CObj* pOther = _pOtherCol->Get_Owner();

	switch (pOther->Get_ObjType())
	{
	case OBJ_TYPE::PLAYER:
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
		if (TILE_TYPE::WALL == static_cast<CTile*>(_pOtherCol->Get_Owner())->Get_TileType())
		{
			VEC2 vLook = m_pTransform->Get_Look();

			if (0.f < vLook.fX && 0.f < vLook.fY)
			{

			}
			else if (0.f != vLook.fX && 0.f == vLook.fY)
			{
				if (pOther->Get_Transform()->Get_Pos().fX < m_pTransform->Get_Pos().fX)
					m_pTransform->Add_Pos_X(_fCX - 1);
				else
					m_pTransform->Add_Pos_X(-_fCX + 1);
			}
			else if (0.f != vLook.fY && 0.f == vLook.fX)
			{
				if (pOther->Get_Transform()->Get_Pos().fY < m_pTransform->Get_Pos().fY)
					m_pTransform->Add_Pos_Y(_fCY - 1);
				else
					m_pTransform->Add_Pos_Y(-_fCY + 1);
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

void CPlayer::OnCollision_Exit(CCollider * _pOtherCol, const float& _fCX, const float& _fCY)
{
}

void CPlayer::Move()
{
	VEC2 vLook{};

	if (CInputMgr::Get_Inst()->Key_Hold('W') && CInputMgr::Get_Inst()->Key_Hold('D'))
		vLook = VEC2_UP + VEC2_RIGHT;
	else if (CInputMgr::Get_Inst()->Key_Hold('D') && CInputMgr::Get_Inst()->Key_Hold('S'))
		vLook = VEC2_RIGHT + VEC2_DOWN;
	else if (CInputMgr::Get_Inst()->Key_Hold('S') && CInputMgr::Get_Inst()->Key_Hold('A'))
		vLook = VEC2_DOWN + VEC2_LEFT;
	else if (CInputMgr::Get_Inst()->Key_Hold('A') && CInputMgr::Get_Inst()->Key_Hold('W'))
		vLook = VEC2_LEFT + VEC2_UP;
	else if (CInputMgr::Get_Inst()->Key_Hold('W'))
		vLook = VEC2_UP;
	else if (CInputMgr::Get_Inst()->Key_Hold('D'))
		vLook = VEC2_RIGHT;
	else if (CInputMgr::Get_Inst()->Key_Hold('S'))
		vLook = VEC2_DOWN;
	else if (CInputMgr::Get_Inst()->Key_Hold('A'))
		vLook = VEC2_LEFT;
	else
	{
		m_pAnimator->Play(L"IDLE", true);
		return;
	}

	if (m_bLock) return;

	m_fStepTime += DT;

	m_pTransform->Set_Look(vLook.Normalized());
	m_pTransform->Add_Pos(m_pTransform->Get_Look() * m_fSpeed * DT);
	m_pAnimator->Play(L"RUN", true);

	if (m_fStepTime > 0.25f)
	{
		CSoundMgr::Get_Inst()->StopSound(SOUND_PLAYER_MOVE);
		CSoundMgr::Get_Inst()->PlaySound(L"PlayerFootstep.wav", SOUND_PLAYER_MOVE, 0.7f);
		m_fStepTime = 0.f;
	}
}

void CPlayer::Skill()
{
	if (CInputMgr::Get_Inst()->Key_Down(VK_SPACE) && !m_bLock) // 공중 폭발
	{
		CSoundMgr::Get_Inst()->StopSound(SOUND_PLAYER_MOVE);
		CSoundMgr::Get_Inst()->PlaySound(L"AirJet.wav", SOUND_PLAYER_MOVE, 0.4f);

		m_pRigidbody->Add_Force(m_pTransform->Get_Look() * m_fDashSpeed);
		m_pAnimator->Play(L"DASH", false);

		if (nullptr != m_SkillList[(UINT)SKILL_KEY_TYPE::SPACE])
		{
			m_SkillList[(UINT)SKILL_KEY_TYPE::SPACE]->Fire();
			m_SkillList[(UINT)SKILL_KEY_TYPE::SPACE]->Start_Count();
		}
	}
	else if (CInputMgr::Get_Inst()->Key_Down(VK_LBUTTON)) // 바람베기
	{
		/*CSoundMgr::Get_Inst()->StopSound(SOUND_PLAYER);
		CSoundMgr::Get_Inst()->PlaySound(L"IceSwordSwing.wav", SOUND_PLAYER, 1.f);*/

		if (m_bAttackType)
			m_pAnimator->Play(L"ATTACK_A", false);
		else
			m_pAnimator->Play(L"ATTACK_B", false);
		m_bAttackType = !m_bAttackType;

		Set_LookFromMouse();
		m_pRigidbody->Add_Force(m_pTransform->Get_Look() * DF_KNOCKBACK_SPEED); 

		if (nullptr != m_SkillList[(UINT)SKILL_KEY_TYPE::LBTN])
		{
			m_SkillList[(UINT)SKILL_KEY_TYPE::LBTN]->Fire();
			m_SkillList[(UINT)SKILL_KEY_TYPE::LBTN]->Start_Count();
		}
	}
	else if (CInputMgr::Get_Inst()->Key_Down(VK_RBUTTON)) // 용의 불숨
	{
		CSoundMgr::Get_Inst()->StopSound(SOUND_PLAYER);
		CSoundMgr::Get_Inst()->PlaySound(L"StandardHeavySwing.wav", SOUND_PLAYER, 0.7f);

		if (m_bAttackType)
			m_pAnimator->Play(L"ATTACK_A", false);
		else
			m_pAnimator->Play(L"ATTACK_B", false);
		m_bAttackType = !m_bAttackType;

		Set_LookFromMouse();

		if (nullptr != m_SkillList[(UINT)SKILL_KEY_TYPE::RBTN])
		{
			m_SkillList[(UINT)SKILL_KEY_TYPE::RBTN]->Fire();
			m_SkillList[(UINT)SKILL_KEY_TYPE::RBTN]->Start_Count();
		}

	}
	else if (CInputMgr::Get_Inst()->Key_Down('Q')) // 폭발 화염구
	{
		CSoundMgr::Get_Inst()->StopSound(SOUND_PLAYER);
		CSoundMgr::Get_Inst()->PlaySound(L"StandardHeavySwing.wav", SOUND_PLAYER, 0.7f);

		if (m_bAttackType)
			m_pAnimator->Play(L"ATTACK_A", false);
		else
			m_pAnimator->Play(L"ATTACK_B", false);
		m_bAttackType = !m_bAttackType;

		Set_LookFromMouse();


		if (nullptr != m_SkillList[(UINT)SKILL_KEY_TYPE::Q] && !m_SkillList[(UINT)SKILL_KEY_TYPE::SPACE]->Is_Cool())
		{
			m_SkillList[(UINT)SKILL_KEY_TYPE::Q]->Fire();
			m_SkillList[(UINT)SKILL_KEY_TYPE::Q]->Start_Count();
		}

	}
	else if (CInputMgr::Get_Inst()->Key_Down('E')) // 물 폭탄
	{
		CSoundMgr::Get_Inst()->StopSound(SOUND_PLAYER);
		CSoundMgr::Get_Inst()->PlaySound(L"StandardHeavySwing.wav", SOUND_PLAYER, 0.7f);

		if (m_bAttackType)
			m_pAnimator->Play(L"ATTACK_A", false);
		else
			m_pAnimator->Play(L"ATTACK_B", false);
		m_bAttackType = !m_bAttackType;

		Set_LookFromMouse();

		if (nullptr != m_SkillList[(UINT)SKILL_KEY_TYPE::E] && !m_SkillList[(UINT)SKILL_KEY_TYPE::SPACE]->Is_Cool())
		{
			static_cast<CWaterBomb_S*>(m_SkillList[(UINT)SKILL_KEY_TYPE::E])->Fire();
			m_SkillList[(UINT)SKILL_KEY_TYPE::E]->Start_Count();
		}
	}
	/*else if (CInputMgr::Get_Inst()->Key_Down('R'))
	{
	
	}*/
}

void CPlayer::Set_LookFromMouse()
{
	VEC2 vMouseWorld = CCameraMgr::Get_Inst()->Get_RealPos(VEC2{ CInputMgr::Get_Inst()->Get_Mouse_Pos().x, CInputMgr::Get_Inst()->Get_Mouse_Pos().y });

	m_vMouse = vMouseWorld - m_pTransform->Get_Pos();

	if (fabs(m_vMouse.fX) < fabs(m_vMouse.fY))
	{
		if (0.f < m_vMouse.fY)
			m_pTransform->Set_Look(VEC2_DOWN);
		else
			m_pTransform->Set_Look(VEC2_UP);
	}
	else
	{
		if (0.f < m_vMouse.fX)
			m_pTransform->Set_Look(VEC2_RIGHT);
		else
			m_pTransform->Set_Look(VEC2_LEFT);
	}
}

void CPlayer::Skill_Update()
{
	for (auto& iter : m_SkillList)
	{
		if (nullptr != iter)
			iter->Update();
	}
}

const bool CPlayer::Init_Animator()
{
	CTexture* pTex;
	CAnimation* pAnimation;
	float fDuration = 0.f;
	// IDLE
	{
		pAnimation = new CAnimation(L"IDLE", m_pAnimator);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_IDLE_DOWN", L"../Data/Sprite/03_Player/Idle_Down.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, 0.01f);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_IDLE_UP", L"../Data/Sprite/03_Player/Idle_Up.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::UP, 0.01f);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_IDLE_RIGHT", L"../Data/Sprite/03_Player/Idle_Side.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, 0.01f);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_IDLE_LEFT", L"../Data/Sprite/03_Player/Idle_Side.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, 0.01f);

		m_pAnimator->PushBack_Ani(pAnimation);
	}

	// WALK
	{
		pAnimation = new CAnimation(L"RUN", m_pAnimator);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_RUN_DOWN_1", L"../Data/Sprite/03_Player/Walk_Down_1.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_RUN_DOWN_2", L"../Data/Sprite/03_Player/Walk_Down_2.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_RUN_DOWN_3", L"../Data/Sprite/03_Player/Walk_Down_3.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_RUN_DOWN_4", L"../Data/Sprite/03_Player/Walk_Down_4.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_RUN_DOWN_5", L"../Data/Sprite/03_Player/Walk_Down_5.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_RUN_DOWN_6", L"../Data/Sprite/03_Player/Walk_Down_6.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_RUN_UP_1", L"../Data/Sprite/03_Player/Walk_Up_1.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::UP);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_RUN_UP_2", L"../Data/Sprite/03_Player/Walk_Up_2.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::UP);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_RUN_UP_3", L"../Data/Sprite/03_Player/Walk_Up_3.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::UP);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_RUN_UP_4", L"../Data/Sprite/03_Player/Walk_Up_4.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::UP);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_RUN_UP_5", L"../Data/Sprite/03_Player/Walk_Up_5.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::UP);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_RUN_UP_6", L"../Data/Sprite/03_Player/Walk_Up_6.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::UP);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_RUN_RIGHT_1", L"../Data/Sprite/03_Player/Walk_Side_1.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_RUN_RIGHT_2", L"../Data/Sprite/03_Player/Walk_Side_2.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_RUN_RIGHT_3", L"../Data/Sprite/03_Player/Walk_Side_3.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_RUN_RIGHT_4", L"../Data/Sprite/03_Player/Walk_Side_4.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_RUN_LEFT_1", L"../Data/Sprite/03_Player/Walk_Side_1.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_RUN_LEFT_2", L"../Data/Sprite/03_Player/Walk_Side_2.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_RUN_LEFT_3", L"../Data/Sprite/03_Player/Walk_Side_3.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_RUN_LEFT_4", L"../Data/Sprite/03_Player/Walk_Side_4.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT);

		m_pAnimator->PushBack_Ani(pAnimation);
	}

	// ATTACK A 
	{
		pAnimation = new CAnimation(L"ATTACK_A", m_pAnimator);

		// 다운 공격 1
		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_ATTACK_DOWN_A1", L"../Data/Sprite/03_Player/Attack_Down_1.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, 0.5f);

		// 업 공격 1
		/*pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_ATTACK_UP_A1", L"../Data/Sprite/03_Player/Attack_Up_1.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::UP, 0.2f);*/

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_ATTACK_UP_A2", L"../Data/Sprite/03_Player/Attack_Up_4.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::UP, 0.5f);

		// 좌측 공격 1
		/*pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_ATTACK_LEFT_A1", L"../Data/Sprite/03_Player/Attack_Side_1.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, 0.2f);*/

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_ATTACK_LEFT_A2", L"../Data/Sprite/03_Player/Attack_Side_2.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, 0.5f);

		// 우측 공격 1
		/*pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_ATTACK_RIGHT_A1", L"../Data/Sprite/03_Player/Attack_Side_3.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, 0.2f);*/
		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_ATTACK_RIGHT_A2", L"../Data/Sprite/03_Player/Attack_Side_4.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, 0.5f);

		m_pAnimator->PushBack_Ani(pAnimation);
	}

	// ATTACK_B
	{
		pAnimation = new CAnimation(L"ATTACK_B", m_pAnimator);

		// 다운 공격 2
		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_ATTACK_DOWN_B1", L"../Data/Sprite/03_Player/Attack_Down_2.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, 0.5f);

		// 업 공격2
		/*pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_ATTACK_UP_B1", L"../Data/Sprite/03_Player/Attack_Up_1.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::UP, 0.2f);*/

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_ATTACK_UP_B2", L"../Data/Sprite/03_Player/Attack_Up_3.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::UP, 0.5f);

		// 좌측 공격 2
		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_ATTACK_LEFT_B1", L"../Data/Sprite/03_Player/Attack_Side_4.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, 0.5f);

		// 우측 공격 2
		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_ATTACK_RIGHT_B1", L"../Data/Sprite/03_Player/Attack_Side_2.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, 0.5f);

		m_pAnimator->PushBack_Ani(pAnimation);
	}

	// HURT
	{
		pAnimation = new CAnimation(L"HURT", m_pAnimator);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_HURT_DOWN", L"../Data/Sprite/03_Player/Hurt_Down.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, m_fKonckBackTime);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_HURT_UP", L"../Data/Sprite/03_Player/Hurt_Up.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::UP, m_fKonckBackTime);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_HURT_RIGHT", L"../Data/Sprite/03_Player/Hurt_Side.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, m_fKonckBackTime);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_HURT_LEFT", L"../Data/Sprite/03_Player/Hurt_Side.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, m_fKonckBackTime);

		m_pAnimator->PushBack_Ani(pAnimation);
	}

	// DEAD
	/*pAnimation = new CAnimation(L"DEAD", m_pAnimator);

	pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_DEAD_DOWN", L"../Data/Sprite/03_Player/Dead.png");
	pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN);

	pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_DEAD_UP", L"../Data/Sprite/03_Player/Dead.png");
	pAnimation->PushBack_Tex(pTex, DIR_TYPE::UP);

	pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_DEAD_RIGHT", L"../Data/Sprite/03_Player/Dead.png");
	pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT);

	pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_DEAD_LEFT", L"../Data/Sprite/03_Player/Dead.png", true);
	pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT);c

	m_pAnimator->PushBack_Ani(pAnimation);*/

	// DASH
	{
		pAnimation = new CAnimation(L"DASH", m_pAnimator);

		// Down
		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_DASH_DOWN_1", L"../Data/Sprite/03_Player/Dash_Down_1.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, 0.2f);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_DASH_DOWN_2", L"../Data/Sprite/03_Player/Dash_Down_2.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, 0.05f);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_DASH_DOWN_3", L"../Data/Sprite/03_Player/Dash_Down_3.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, 0.05f);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_DASH_DOWN_4", L"../Data/Sprite/03_Player/Dash_Down_4.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, 0.05f);

		// Up
		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_DASH_UP_1", L"../Data/Sprite/03_Player/Dash_Up_1.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::UP, 0.25f);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_DASH_UP_2", L"../Data/Sprite/03_Player/Dash_Up_2.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::UP, 0.05f);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_DASH_UP_3", L"../Data/Sprite/03_Player/Dash_Up_3.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::UP, 0.05f);

		// Right
		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_DASH_RIGHT_1", L"../Data/Sprite/03_Player/Dash_Side_1.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, 0.05f);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_DASH_RIGHT_2", L"../Data/Sprite/03_Player/Dash_Side_2.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, 0.1f);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_DASH_RIGHT_3", L"../Data/Sprite/03_Player/Dash_Side_3.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, 0.05f);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_DASH_RIGHT_4", L"../Data/Sprite/03_Player/Dash_Side_4.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, 0.05f);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_DASH_RIGHT_5", L"../Data/Sprite/03_Player/Dash_Side_5.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, 0.05f);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_DASH_RIGHT_6", L"../Data/Sprite/03_Player/Dash_Side_6.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, 0.05f);

		// Left
		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_DASH_LEFT_1", L"../Data/Sprite/03_Player/Dash_Side_1.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, 0.05f);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_DASH_LEFT_2", L"../Data/Sprite/03_Player/Dash_Side_2.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, 0.1f);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_DASH_LEFT_3", L"../Data/Sprite/03_Player/Dash_Side_3.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, 0.05f);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_DASH_LEFT_4", L"../Data/Sprite/03_Player/Dash_Side_4.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, 0.05f);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_DASH_LEFT_5", L"../Data/Sprite/03_Player/Dash_Side_5.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, 0.05f);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_DASH_LEFT_6", L"../Data/Sprite/03_Player/Dash_Side_6.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, 0.05f);

		m_pAnimator->PushBack_Ani(pAnimation);
	}

	// PORTAL
	//{
	//	pAnimation = new CAnimation(L"PORTAL", m_pAnimator);

	//	fDuration = 0.05f;

	//	pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_PORTAL_DOWN_0", L"../Data/Sprite/08.Effect/Portal/TeleportEffect0.png");
	//	pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

	//	pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_PORTAL_DOWN_1", L"../Data/Sprite/08.Effect/Portal/TeleportEffect1.png");
	//	pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

	//	pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_PORTAL_DOWN_2", L"../Data/Sprite/08.Effect/Portal/TeleportEffect2.png");
	//	pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

	//	pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_PORTAL_DOWN_3", L"../Data/Sprite/08.Effect/Portal/TeleportEffect3.png");
	//	pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

	//	pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_PORTAL_DOWN_4", L"../Data/Sprite/08.Effect/Portal/TeleportEffect4.png");
	//	pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

	//	pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_PORTAL_DOWN_5", L"../Data/Sprite/08.Effect/Portal/TeleportEffect5.png");
	//	pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

	//	pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_PORTAL_DOWN_6", L"../Data/Sprite/08.Effect/Portal/TeleportEffect6.png");
	//	pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

	//	/*pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_PORTAL_DOWN_7", L"../Data/Sprite/08.Effect/Portal/TeleportEffect7.png");
	//	pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

	//	pTex = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_PORTAL_DOWN_8", L"../Data/Sprite/08.Effect/Portal/TeleportEffect8.png");
	//	pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);*/

	//	m_pAnimator->PushBack_Ani(pAnimation);
	//}
	return true;
}
