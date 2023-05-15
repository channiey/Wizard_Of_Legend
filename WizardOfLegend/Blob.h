#pragma once
#include "Enemy.h"
class CBlob_P;
class CBlob :
	public CEnemy
{
public:
	CBlob(const OBJ_TYPE& _eType);
	virtual ~CBlob();

public:
	virtual const bool			Init() override;
	virtual void				Update() override;
	virtual void				LateUpdate() override;
	virtual void				Render(HDC _hDC) override;
	virtual void				Release() override;

public:
	virtual void				OnCollision_Enter(CCollider* _pOtherCol, const float& _fCX, const float& _fCY);
	virtual void				OnCollision_Stay(CCollider* _pOtherCol, const float& _fCX, const float& _fCY);
	virtual void				OnCollision_Exit(CCollider* _pOtherCol, const float& _fCX, const float& _fCY);

	virtual const bool			Init_Animator() override;

public:
	void						Set_State(const BLOB_STATE&	_eState) { m_eState = _eState; }
	virtual void				Set_Active(const bool _bActive) override;

	CObj*						Get_Target() const { return m_pTarget; }

private:
	const bool					Set_Target();					// 플레이어를 찾아온다.
	const bool					Set_Jump(const bool _bJump);	// 점프 시작과 끝에서 필요한 데이터를 세팅한다.
	const float					Calculate_Line_Y();				// 직선에서의 Y포지션을 계산한다 (그림자 포지션 - 직선의 방정식)
	const bool					Fall();							// 점프 도중 피격시 호출되는 함수, 그라운드에 도착했는지 여부를 반환한다
	//const bool				Is_Randing();				// 점프를 시작했다가 착지 했는지

private:
	BLOB_STATE					m_eState;
	LINE						m_tLine;
	float						m_fJumpPower;
	CObj*						m_pTarget;
	float						m_fIdleTime;
	float						m_fJumpTime; // 점프 체공시간
	float						m_fAccTime;

	Graphics					m_Graphics;
	float						m_fmoveX;
	CBlob_P*					m_pProj;
};

