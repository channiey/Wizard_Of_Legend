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
	const bool					Set_Target();					// �÷��̾ ã�ƿ´�.
	const bool					Set_Jump(const bool _bJump);	// ���� ���۰� ������ �ʿ��� �����͸� �����Ѵ�.
	const float					Calculate_Line_Y();				// ���������� Y�������� ����Ѵ� (�׸��� ������ - ������ ������)
	const bool					Fall();							// ���� ���� �ǰݽ� ȣ��Ǵ� �Լ�, �׶��忡 �����ߴ��� ���θ� ��ȯ�Ѵ�
	//const bool				Is_Randing();				// ������ �����ߴٰ� ���� �ߴ���

private:
	BLOB_STATE					m_eState;
	LINE						m_tLine;
	float						m_fJumpPower;
	CObj*						m_pTarget;
	float						m_fIdleTime;
	float						m_fJumpTime; // ���� ü���ð�
	float						m_fAccTime;

	Graphics					m_Graphics;
	float						m_fmoveX;
	CBlob_P*					m_pProj;
};

