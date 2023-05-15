#pragma once
#include "Projectile.h"
class CTexture;

class CWindSlash_P :
	public CProjectile
{
public:
	CWindSlash_P(const OBJ_TYPE& _eType);
	virtual ~CWindSlash_P();

public:
	virtual const bool	Init() override;
	virtual void		Update() override;
	virtual void		LateUpdate() override;
	virtual void		Render(HDC _hDC) override;
	void				Release();

public:
	virtual void		OnCollision_Enter(CCollider * _pOtherCol, const float& _fCX, const float& _fCY) override;
	virtual void		OnCollision_Stay(CCollider * _pOtherCol, const float& _fCX, const float& _fCY) override;
	virtual void		OnCollision_Exit(CCollider * _pOtherCol, const float& _fCX, const float& _fCY) override;

public:
	virtual void		Return() override;

private:
	virtual const bool Init_Animator() override;

public:
	const bool			Is_On() const { return m_bOn; }

public:
	void				On();
	void				Off();

private:
	bool				m_bOn;
};

