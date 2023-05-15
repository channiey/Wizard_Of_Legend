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

	void			Fade_In(const float& _fTime = 1.f); // 검 -> 흰
	void			Fade_Out(const float& _fTime = 1.f);

	bool			Is_End_FadeOut() const { return m_bFadeOut; }

public:
	void			Shake_Camera(const float& _fTime, const float& _fIntensity) { m_bShake = true; m_fShakeTime = _fTime; m_fIntensity = _fIntensity;  m_fAcc = 0.f; }
	void			Hurt_Camera(const float& _fTime) { m_bHurt = true; m_fHurtTime = _fTime; }

private:
	void			Apply_Shake();
	void			Apply_Hurt();
	void			Effect();
	void			Limit_Pos(); // 카메라가 맵 사이즈 밖을 비추는 것을 방지한다.

private:
	VEC2			m_vLookAt;	// 카메라가 실제 바라보고 있는 위치의 중점
	CObj*			m_pTarget;	// 카메라가 추적할 오브젝트
	VEC2			m_vDiff;	// 해상도 중심 위치와 카메라  look at간의 차이 값

	bool			m_bShake;		// 카메라 흔들림 여부
	float			m_fIntensity;	// 카메라 흔들림 세기
	float			m_fShakeTime;		// 카메라 흔들림 시간
	float			m_fAcc;

	CUI*			m_pHurtUI;
	float			m_fAccHurt;
	float			m_fHurtTime;
	bool			m_bHurt;



	float			m_fMoveTime; // 타겟을 따라가는 데 걸리는 시간
	float			m_fMoveSpeed; // 타겟을 따라가는 속도
	float			m_fAccMoveTime; // 누적 시간
	VEC2			m_vCurLookAt; // 이전 위치와 현재 위치의 보정위치
	VEC2			m_vPreLookAt; // 카메라가 보는 이전 프레임 위치

	bool			m_bFadeIn;
	bool			m_bFadeOut;
	float			m_fFadevalue;

	Graphics m_Graphics;
};

