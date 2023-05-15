#pragma once

class CTexture;
class CAnimator;

typedef struct myFrame // 일단 구조체 없이 하되, 애니메이션별 별도의 시간값 세팅이 필요한 경우 이것을 사용하기로 한다.
{
	CTexture* pTex;
	float fDuration; // 한 프레임, 즉 한 애니메이션에서 얼마나 머물지에 대한 시간값

}ANIFRAME;

class CAnimation
{
public:
	CAnimation(const wstring& _strName, CAnimator* _pAnimator);
	~CAnimation();

public:
	void				LateUpdate();
	void				Render(HDC _hDC);
	void				Release();

public:
	const bool			PushBack_Tex(CTexture* const _pTex, const DIR_TYPE& _eDir, const float& _fDuration = 0.1f);

	const wstring&		Get_Name() const											{ return m_strName; }

	void				Set_Name(const wstring& _strName)							{ m_strName = _strName; }

	const bool			Is_Finish()	const											{ return m_bFinish; }
	void				Init_Frame()												{ m_bFinish = false; m_iCurTex = 0; m_fAccTime = 0.f; }
	void				Init_Time()													{ m_iCurTex = 0; m_fAccTime = 0.f; }

private:
	void				Set_Dir();

private:
	wstring				m_strName;
	vector<ANIFRAME>	m_vecTex[(UINT)DIR_TYPE::TYPEEND]; // 하나의 애니메이션은 상, 하, 좌, 우 4개 방향 애니메이션을 벡터 배열로 갖는다.
	DIR_TYPE			m_eCurDir;
	DIR_TYPE			m_ePreDir;
	int					m_iCurTex;
	bool				m_bFinish;
	float				m_fAccTime;
	CAnimator*			m_pAnimator;
	VEC2				m_vOffSet;
	Graphics			m_Graphics;
};

