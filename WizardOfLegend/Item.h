#pragma once
#include "Obj.h"

class CTexture;

class CItem :
	public CObj
{
public:
	CItem(const OBJ_TYPE& _eType, const ITEM_TYPE& _eItemType);
	virtual ~CItem();


public:

	const ITEM_TYPE& Get_ItemType() const { return m_eItemType; }

private:
	virtual void	Load_Image() PURE;


protected:
	ITEM_TYPE m_eItemType;

	CTexture* m_pTex;
	Graphics m_Graphics;
};

