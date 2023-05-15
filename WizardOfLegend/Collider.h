#pragma once

#include "Component.h"

class CCollider : public CComponent
{
public:
	CCollider(CObj* _pOwner);
	CCollider(const CCollider& _rhs);
	CCollider& operator = (const CCollider& _rhs) = delete;
	virtual ~CCollider();

public:
	virtual void	Init() override;
	virtual void	LateUpdate() override;
	virtual void	Render(HDC _hDC) override;

public:
	void			OnCollision_Enter(CCollider* _pCollider, const float& _fCX, const float& _fCY);
	void			OnCollision_Stay(CCollider* _pCollider, const float& _fCX, const float& _fCY);
	void			OnCollision_Exit(CCollider* _pCollider, const float& _fCX, const float& _fCY);

public:
	const VEC2&		Get_Pos() const						{ return m_vPos; }
	const VEC2&		Get_OffSet() const					{ return m_vOffset; }
	const VEC2&		Get_Scale() const					{ return m_vScale; }
	const UINT&		Get_ID() const						{ return m_iID; }

	void			Set_Pos(const VEC2& _vPos)			{ m_vPos = _vPos; }
	void			Set_OffSet(const VEC2& _vOffSet)	{ m_vOffset = _vOffSet; }
	void			Set_Scale(const VEC2& _vScale)		{ m_vScale = _vScale; }

	const bool		Is_Collision() const				{ return (0 < m_iCol) ? true : false; }

private:
	VEC2			m_vPos;
	VEC2			m_vOffset;
	VEC2			m_vScale;
	int				m_iCol; // 현재 충돌중인 객체 수
	UINT			m_iID;
	static UINT		g_iNextID;
};

