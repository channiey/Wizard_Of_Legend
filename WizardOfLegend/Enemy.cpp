#include "stdafx.h"
#include "Enemy.h"
#include "AI.h"

CEnemy::CEnemy(const OBJ_TYPE& _eType)
	: CObj(_eType)
	, m_pAI(nullptr)
	 , m_pDropItem(nullptr)
{
}


CEnemy::~CEnemy()
{
	Release();
}

void CEnemy::Update()
{
	if (nullptr != m_pAI)
		m_pAI->Update();
}

void CEnemy::Render(HDC _hDC)
{
	if (nullptr != m_pAI)
		m_pAI->Render(_hDC);
}

void CEnemy::Release()
{
	
}

const bool CEnemy::Set_AI(AI * _pAI)
{
	if (nullptr == _pAI) return false; 
	
	m_pAI = _pAI; 
	
	m_pAI->Set_Owner(this); 
	
	return true;
}