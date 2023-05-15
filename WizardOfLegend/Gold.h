#pragma once
#include "Item.h"

class CGold :
	public CItem
{
public:
	CGold(const OBJ_TYPE& _eType, const ITEM_TYPE& _eItemType);
	virtual ~CGold();

public:
	virtual const bool Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC _hDC) override;

public:
	const int&	Get_Value() const { return m_iValue; }

public:
	virtual void				OnCollision_Enter(CCollider* _pOtherCol, const float& _fCX, const float& _fCY);
		virtual void				OnCollision_Stay(CCollider* _pOtherCol, const float& _fCX, const float& _fCY);
		virtual void				OnCollision_Exit(CCollider* _pOtherCol, const float& _fCX, const float& _fCY);

private:

	int m_iValue;
	

	// Inherited via CItem
	virtual void Load_Image() override;

};

