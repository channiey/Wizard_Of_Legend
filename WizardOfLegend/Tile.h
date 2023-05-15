#pragma once
#include "Obj.h"

class CTexture;

class CTile : public CObj
{
public:
	explicit CTile(const OBJ_TYPE& _eType);
	virtual ~CTile();

public:
	virtual const bool		Init() override;
	virtual void			Update() override;
	virtual void			LateUpdate() override;
	virtual void			Render(HDC _hDC) override;
	void					Release();

public:
	virtual void			OnCollision_Enter(CCollider * _pOtherCol, const float& _fCX, const float& _fCY) override;
	virtual void			OnCollision_Stay(CCollider * _pOtherCol, const float& _fCX, const float& _fCY) override;
	virtual void			OnCollision_Exit(CCollider * _pOtherCol, const float& _fCX, const float& _fCY) override;

public:
	void					Set_TileType(const TILE_TYPE& _eType)	{ m_eTileType = _eType; }
	//void					Set_Index(const int& _iIndex) { m_iIndex = _iIndex; }
	
	//const int&				Get_Index() const { return m_iIndex; }
	const TILE_TYPE&		Get_TileType() const					{ return m_eTileType; }

	//void					Calculate_Index() {}


private:
	TILE_TYPE				m_eTileType;
	Graphics				m_Graphics;
	//int						m_iIndex;
};

