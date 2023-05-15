#include "stdafx.h"
#include "WaterBomb_S.h"
#include "WaterBomb_P.h"
#include "ObjPool.h"
#include "TimeMgr.h"
#include "SoundMgr.h"
CWaterBomb_S::CWaterBomb_S(CObj* _pObj, const float& _fCoolTime, const float& _fDamage)
	: CSkill(_pObj, _fCoolTime, _fDamage)
	, m_bAttack(false)
	, m_iFireCurCount(0)
	, m_iFireMaxCount(0)
	, m_fAcc(0.f)
{
}


CWaterBomb_S::~CWaterBomb_S()
{
}

void CWaterBomb_S::Update()
{
	//0.1 ~ 0.3�� �������� 5~6���� ����ź�� �������� �߻�

	CSkill::Update();

	/*if (!m_bAttack) return;


	m_fAcc += DT;

	float fRan = float(rand() % 3 + 1) * 0.1f; 

	if (m_fAcc > fRan)
	{
		m_fAcc = 0.f;
		++m_iFireCurCount;

		Fire();

		if (m_iFireCurCount >= m_iFireMaxCount)
		{
			m_bAttack = false;
		}
	}
*/

}

void CWaterBomb_S::Fire()
{
	if (m_bCool) return;
	if (!m_bActive) return;

	/*CSoundMgr::Get_Inst()->StopSound(SOUND_PLAYER);
	CSoundMgr::Get_Inst()->PlaySound(L"StandardHeavySwing.wav", SOUND_PLAYER, 0.7f);*/
	CWaterBomb_P* pProj = static_cast<CWaterBomb_P*>(CObjPool<CWaterBomb_P>::Get_Obj());

	pProj->Set_Owner(m_pOwner);

	if (nullptr == pProj) return;

	pProj->Reset(pProj);
}

void CWaterBomb_S::Start()
{
	//m_iFireMaxCount = rand() % 2 + 3; // 3~4�� �߻�
	m_iFireMaxCount = 1;

	m_iFireCurCount = 0;
	m_bAttack = true;
}
