#pragma once

class CTexture;
class CAnimator;

typedef struct myFrame // �ϴ� ����ü ���� �ϵ�, �ִϸ��̼Ǻ� ������ �ð��� ������ �ʿ��� ��� �̰��� ����ϱ�� �Ѵ�.
{
	CTexture* pTex;
	float fDuration; // �� ������, �� �� �ִϸ��̼ǿ��� �󸶳� �ӹ����� ���� �ð���

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
	vector<ANIFRAME>	m_vecTex[(UINT)DIR_TYPE::TYPEEND]; // �ϳ��� �ִϸ��̼��� ��, ��, ��, �� 4�� ���� �ִϸ��̼��� ���� �迭�� ���´�.
	DIR_TYPE			m_eCurDir;
	DIR_TYPE			m_ePreDir;
	int					m_iCurTex;
	bool				m_bFinish;
	float				m_fAccTime;
	CAnimator*			m_pAnimator;
	VEC2				m_vOffSet;
	Graphics			m_Graphics;
};

