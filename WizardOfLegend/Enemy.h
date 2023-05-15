#pragma once
#include "Obj.h"

class AI;
class CItem;
class CEnemy : public CObj
{
public:
	CEnemy(const OBJ_TYPE& _eType);
	virtual ~CEnemy();
	
public:
	virtual void	Update() override;
	virtual void	Render(HDC _hDC) override;
	virtual void	Release() override;

public:
	const bool		Set_AI(AI* _pAI);
	
protected:
	AI* m_pAI;

	CItem*						m_pDropItem;
};

