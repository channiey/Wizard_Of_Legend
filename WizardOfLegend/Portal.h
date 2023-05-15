#pragma once
#include "Obj.h"

class CEffect;
class CTexture;

class CPortal :
	public CObj
{
public:
	CPortal(const OBJ_TYPE& _eType);
	virtual ~CPortal();

	// Inherited via CObj
	virtual const bool			Init() override;
	virtual void				Update() override;
	virtual void				LateUpdate() override;
	virtual void				Render(HDC _hDC) override;

public:
	virtual void				OnCollision_Enter(CCollider* _pOtherCol, const float& _fCX, const float& _fCY) override;
	virtual void				OnCollision_Stay(CCollider* _pOtherCol, const float& _fCX, const float& _fCY) override;
	virtual void				OnCollision_Exit(CCollider* _pOtherCol, const float& _fCX, const float& _fCY) override;

public:
	void						Effect(const VEC2& _vPos);

private:
	CObj*				m_pKeyUI;
	CEffect*			m_pEffect;

	CTexture*			m_pTexPortal;
	CTexture*			m_pTexPortalDrawing;

	Graphics			m_Graphics;

};

