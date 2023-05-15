#include "stdafx.h"
#include "PortalEffect.h"
#include "Transform.h"
#include "Animator.h"
#include "Animation.h"
#include "ResMgr.h"
#include "Texture.h"
#include "ObjMgr.h"
#include "CameraMgr.h"
#include "SoundMgr.h"2
/*

Enter

1. 플레이어 없는 씬 화면 1초 렌더링

2. 이펙트 시작

3. 플레이어 등장


Exit

1. 이펙트 시작

2. 플레이어 사라짐

*/

CPortalEffect::CPortalEffect(const OBJ_TYPE& _eType, CObj* _pOwner)
	:CEffect(_eType, _pOwner)
{
}


CPortalEffect::~CPortalEffect()
{
}

const bool CPortalEffect::Init()
{

	Create_Transform();
	m_pTransform->Init();

	m_pAnimator = new CAnimator(this);
	m_pAnimator->Init();
	m_listComponent.push_back(m_pAnimator);

	Init_Animator();

	m_fRenderMag = 3.5f;

	return true;
}

void CPortalEffect::Update()
{
}

void CPortalEffect::LateUpdate()
{
	Component_LateUpdate();
}

void CPortalEffect::Render(HDC _hDC)
{
	Component_Render(_hDC);
}

const bool CPortalEffect::Init_Animator()
{
	CTexture* pTex;
	CAnimation* pAnimation;
	float fDuration = 0.f;

	pAnimation = new CAnimation(L"PORTAL", m_pAnimator);

	fDuration = 0.05f;

	pTex = CResMgr::Get_Inst()->Load_Tex(L"PORTAL_DOWN_0", L"../Data/Sprite/08.Effect/Portal/TeleportEffect0.png");
	pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

	pTex = CResMgr::Get_Inst()->Load_Tex(L"PORTAL_DOWN_1", L"../Data/Sprite/08.Effect/Portal/TeleportEffect1.png");
	pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

	pTex = CResMgr::Get_Inst()->Load_Tex(L"PORTAL_DOWN_2", L"../Data/Sprite/08.Effect/Portal/TeleportEffect2.png");
	pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

	pTex = CResMgr::Get_Inst()->Load_Tex(L"PORTAL_DOWN_3", L"../Data/Sprite/08.Effect/Portal/TeleportEffect3.png");
	pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

	pTex = CResMgr::Get_Inst()->Load_Tex(L"PORTAL_DOWN_4", L"../Data/Sprite/08.Effect/Portal/TeleportEffect4.png");
	pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

	pTex = CResMgr::Get_Inst()->Load_Tex(L"PORTAL_DOWN_5", L"../Data/Sprite/08.Effect/Portal/TeleportEffect5.png");
	pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

	pTex = CResMgr::Get_Inst()->Load_Tex(L"PORTAL_DOWN_6", L"../Data/Sprite/08.Effect/Portal/TeleportEffect6.png");
	pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

	pTex = CResMgr::Get_Inst()->Load_Tex(L"PORTAL_DOWN_7", L"../Data/Sprite/08.Effect/Portal/TeleportEffect7.png");
	pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

	pTex = CResMgr::Get_Inst()->Load_Tex(L"PORTAL_DOWN_8", L"../Data/Sprite/08.Effect/Portal/TeleportEffect8.png");
	pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

	m_pAnimator->PushBack_Ani(pAnimation);
	return true;
}

void CPortalEffect::On(const VEC2& _vPos)
{
	CCameraMgr::Get_Inst()->Shake_Camera(1.f, 10.f);
	CSoundMgr::Get_Inst()->StopSound(SOUND_PORTAL);
	CSoundMgr::Get_Inst()->PlaySound(L"ExitPortal.wav", SOUND_PORTAL, 1.f);

	m_pAnimator->Play(L"PORTAL", false);

	m_pTransform->Set_Pos(_vPos);
}
