#pragma once
#include "Item.h"
class CSkillItem :
	public CItem
{
public:
	CSkillItem(const OBJ_TYPE& _eType, const ITEM_TYPE& _eItemType);
	virtual ~CSkillItem();

public:
	virtual const bool Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC _hDC) override;

public:

public:
	virtual void				OnCollision_Enter(CCollider* _pOtherCol, const float& _fCX, const float& _fCY);
	virtual void				OnCollision_Stay(CCollider* _pOtherCol, const float& _fCX, const float& _fCY);
	virtual void				OnCollision_Exit(CCollider* _pOtherCol, const float& _fCX, const float& _fCY);

	virtual void Load_Image() override;

};

