#include "stdafx.h"
#include "Animation.h"
#include "Texture.h"
#include "MainGame.h"
#include "Animator.h"
#include "Obj.h"
#include "Transform.h"
#include "CameraMgr.h"
#include "TimeMgr.h"

CAnimation::CAnimation(const wstring& _strName, CAnimator* _pAnimator)
	: m_strName(_strName)
	, m_Graphics(CMainGame::Get_Inst()->Get_SubDC())
	, m_bFinish(false)
	, m_iCurTex(0)
	, m_fAccTime(0.f)
	, m_pAnimator(_pAnimator)
	, m_eCurDir(DIR_TYPE::DOWN)
	, m_ePreDir(DIR_TYPE::DOWN)
{
	ZeroMemory(&m_vOffSet, sizeof(VEC2));
}

CAnimation::~CAnimation()
{
	Release();
}

void CAnimation::LateUpdate()
{
	Set_Dir();

	if (m_bFinish)
		return;

	m_fAccTime += DT;

	if (m_vecTex[(UINT)m_eCurDir].empty()) return;

	if (m_vecTex[(UINT)m_eCurDir][m_iCurTex].fDuration < m_fAccTime)
	{
		++m_iCurTex;

		if (int(m_vecTex[(UINT)m_eCurDir].size()) <= m_iCurTex)
		{
			m_iCurTex = -1;
			m_bFinish = true;
			return;
		}
		m_fAccTime = m_fAccTime - m_vecTex[(UINT)m_eCurDir][m_iCurTex].fDuration;
	}
}

void CAnimation::Render(HDC _hDC)
{
	if (m_bFinish)
		return;

	VEC2 vPos = m_pAnimator->Get_Owner()->Get_Transform()->Get_Pos() + m_vOffSet;

	vPos = CCameraMgr::Get_Inst()->Get_RenderPos(vPos);

	float m_fMag = m_pAnimator->Get_Owner()->Get_RenderMag();

	if (m_vecTex[(UINT)m_eCurDir].empty() || nullptr == m_vecTex[(UINT)m_eCurDir][m_iCurTex].pTex->Get_Image())
		return;

	UINT iWidth = m_vecTex[(UINT)m_eCurDir][m_iCurTex].pTex->Get_Image()->GetWidth();
	UINT iHeight = m_vecTex[(UINT)m_eCurDir][m_iCurTex].pTex->Get_Image()->GetHeight();
	
	//if (OBJ_TYPE::EFFECT == m_pAnimator->Get_Owner()->Get_ObjType()) // 포탈 이펙트 특혜
	//{
	//	vPos.fY -= iHeight^;
	//	cout << "처리!\n";
	//}

	// 세타 값에 따른 이미지 회전 수행
	if (0.f != m_pAnimator->Get_Owner()->Get_Transform()->Get_Theta())
	{
		float fAngle = m_pAnimator->Get_Owner()->Get_Transform()->Get_Theta() * 180.f / PI;
		//m_Graphics.RotateTransform(fAngle);

		// 상하 회전 (화염 드래곤)
		if ((90.f <= fAngle && fAngle <= 270.f) && !m_vecTex[(UINT)m_eCurDir][m_iCurTex].pTex->Is_RotationUpDown())
		{
			m_vecTex[(UINT)m_eCurDir][m_iCurTex].pTex->Get_Image()->RotateFlip(Rotate180FlipX);
			m_vecTex[(UINT)m_eCurDir][m_iCurTex].pTex->Set_RotationUpDown(true);
		}
		else if (( 90.f > fAngle || fAngle > 270.f) && m_vecTex[(UINT)m_eCurDir][m_iCurTex].pTex->Is_RotationUpDown())
		{
			m_vecTex[(UINT)m_eCurDir][m_iCurTex].pTex->Get_Image()->RotateFlip(Rotate180FlipX);
			m_vecTex[(UINT)m_eCurDir][m_iCurTex].pTex->Set_RotationUpDown(false);
		}

		Matrix mat;
		mat.RotateAt(-fAngle, PointF(vPos.fX, vPos.fY));
		m_Graphics.SetTransform(&mat);
	}

	m_Graphics.DrawImage(m_vecTex[(UINT)m_eCurDir][m_iCurTex].pTex->Get_Image(),
						int(vPos.fX - (iWidth * m_fMag * 0.5f)),
						int(vPos.fY - (iHeight * m_fMag * 0.5f)),
						int(iWidth * m_fMag),
						int(iHeight* m_fMag));
}

void CAnimation::Release()
{
	// 텍스쳐는 리소스 매니저에서 일괄 삭제한다.
}

const bool CAnimation::PushBack_Tex(CTexture * const _pTex, const DIR_TYPE & _eDir, const float& _fDuration)
{
	if (nullptr == _pTex) return false; 

	ANIFRAME frame{};

	frame.pTex = _pTex;
	frame.fDuration = _fDuration;

	m_vecTex[(UINT)_eDir].push_back(frame); return true;
}

void CAnimation::Set_Dir()
{
	if (!m_pAnimator->Is_Use_Dir()) return;


	VEC2 vLook = m_pAnimator->Get_Owner()->Get_Transform()->Get_Look();

	if (fabs(vLook.fX) < fabs(vLook.fY))
	{
		if (0.f < vLook.fY)
			m_eCurDir = DIR_TYPE::DOWN;		
		else
			m_eCurDir = DIR_TYPE::UP;
	}
	else
	{
		if (0.f < vLook.fX)
			m_eCurDir = DIR_TYPE::RIGHT;
		else
			m_eCurDir = DIR_TYPE::LEFT;
	}

	if (m_ePreDir != m_eCurDir)
	{
		Init_Frame();
		m_ePreDir = m_eCurDir;
	}
}
