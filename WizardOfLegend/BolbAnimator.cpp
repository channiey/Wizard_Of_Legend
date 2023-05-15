#include "stdafx.h"
#include "BolbAnimator.h"
#include "Blob.h"
#include "Rigidbody.h"
#include "Animation.h"
#include "EventMgr.h"
#include "SoundMgr.h"

CBolbAnimator::CBolbAnimator(CObj* _pOwner)
	: CAnimator(_pOwner)
{
}


CBolbAnimator::~CBolbAnimator()
{
}

void CBolbAnimator::LateUpdate()
{
	if (!m_bActive) return;

	if (nullptr != m_pCurAni)
	{
		m_pCurAni->LateUpdate();

		if (m_bRepeat && m_pCurAni->Is_Finish())
			m_pCurAni->Init_Frame();
		else if (!m_bRepeat && m_pCurAni->Is_Finish())
		{
			if (m_pCurAni->Get_Name() == L"SPAWN")
			{
				static_cast<CBlob*>(m_pOwner)->Set_State(BLOB_STATE::IDLE);
				Play(L"IDLE", true);
			}
			if (m_pCurAni->Get_Name() == L"HURT")
			{
				static_cast<CBlob*>(m_pOwner)->Set_State(BLOB_STATE::IDLE);
			}
			else if (m_pCurAni->Get_Name() == L"DEAD")
			{
				m_pOwner->Set_Dead();
				CEventMgr::Get_Inst()->Delete_Obj(m_pOwner);
			}
		}
	}
}

void CBolbAnimator::Play(const wstring & _strName, const bool & _bRepeat)
{
	CAnimation* pAni = Find_Ani(_strName);

	if (nullptr == pAni) return;

	m_pCurAni = pAni;
	m_bRepeat = _bRepeat;

	if (m_pCurAni->Is_Finish())
		m_pCurAni->Init_Frame();
}
