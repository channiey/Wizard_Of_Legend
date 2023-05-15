#pragma once

class CObj;
class CUI;

class CCameraMgr
{
	SINGLETON(CCameraMgr)

private:
	CCameraMgr();
	virtual ~CCameraMgr();

public:
	void			Init();
	void			Update();
	void			Render(HDC _hDC);
private:
	void			Calculate_Diff		();
	void			Move_WithArrowKey	();

public:
	const bool		Set_Target(CObj* _pObj); 
	void			Set_LookAt(const VEC2& _vPos, const float& _fMoveTime);			//{ m_vLookAt = _vPos; }

	const VEC2		Get_RenderPos		(const VEC2& _vPos)			{ return _vPos - m_vDiff; }
	const VEC2		Get_RealPos			(const VEC2& _vPos)			{ return _vPos + m_vDiff; }
	const VEC2		Get_LookAt			()const						{ return m_vLookAt; }
	CObj*			Get_Target() const { return m_pTarget; }

	const bool		Is_Shaking() const { return m_bShake; }
	void			Stop_Shaking()		{ m_bShake =  false; }

	void			Fade_In(const float& _fTime = 1.f); // �� -> ��
	void			Fade_Out(const float& _fTime = 1.f);

	bool			Is_End_FadeOut() const { return m_bFadeOut; }

public:
	void			Shake_Camera(const float& _fTime, const float& _fIntensity) { m_bShake = true; m_fShakeTime = _fTime; m_fIntensity = _fIntensity;  m_fAcc = 0.f; }
	void			Hurt_Camera(const float& _fTime) { m_bHurt = true; m_fHurtTime = _fTime; }

private:
	void			Apply_Shake();
	void			Apply_Hurt();
	void			Effect();
	void			Limit_Pos(); // ī�޶� �� ������ ���� ���ߴ� ���� �����Ѵ�.

private:
	VEC2			m_vLookAt;	// ī�޶� ���� �ٶ󺸰� �ִ� ��ġ�� ����
	CObj*			m_pTarget;	// ī�޶� ������ ������Ʈ
	VEC2			m_vDiff;	// �ػ� �߽� ��ġ�� ī�޶�  look at���� ���� ��

	bool			m_bShake;		// ī�޶� ��鸲 ����
	float			m_fIntensity;	// ī�޶� ��鸲 ����
	float			m_fShakeTime;		// ī�޶� ��鸲 �ð�
	float			m_fAcc;

	CUI*			m_pHurtUI;
	float			m_fAccHurt;
	float			m_fHurtTime;
	bool			m_bHurt;



	float			m_fMoveTime; // Ÿ���� ���󰡴� �� �ɸ��� �ð�
	float			m_fMoveSpeed; // Ÿ���� ���󰡴� �ӵ�
	float			m_fAccMoveTime; // ���� �ð�
	VEC2			m_vCurLookAt; // ���� ��ġ�� ���� ��ġ�� ������ġ
	VEC2			m_vPreLookAt; // ī�޶� ���� ���� ������ ��ġ

	bool			m_bFadeIn;
	bool			m_bFadeOut;
	float			m_fFadevalue;

	Graphics m_Graphics;
};

