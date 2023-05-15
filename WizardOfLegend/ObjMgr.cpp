#include "stdafx.h"
#include "ObjMgr.h"
#include "Obj.h"
#include "CameraMgr.h"
#include "Transform.h"

CObjMgr* CObjMgr::m_pInst = nullptr;

CObjMgr::CObjMgr()
{
}


CObjMgr::~CObjMgr()
{
	Release();
}

const bool CObjMgr::Init()
{
	return true;
}

void CObjMgr::Update()
{
	for (int i = 0; i < (UINT)OBJ_TYPE::TYPEEND; ++i)
	{
		for (list<CObj*>::iterator iter = m_ObjList[i].begin(); iter != m_ObjList[i].end(); ++iter)
		{
			if (*iter == nullptr || !((*iter)->Is_Active()) || (*iter)->Is_Dead())
				continue;
			else
				(*iter)->Update();
		}
	}
}

void CObjMgr::LateUpdate()
{
	for (int i = 0; i < (UINT)OBJ_TYPE::TYPEEND; ++i)
	{
		for (list<CObj*>::iterator iter = m_ObjList[i].begin(); iter != m_ObjList[i].end(); ++iter)
		{
			if (*iter == nullptr || !((*iter)->Is_Active()) || (*iter)->Is_Dead())
				continue;
			else
			{
				(*iter)->LateUpdate();

				RENDER_ORDER eOrder = (*iter)->Get_RenderOrder();

				if((*iter)->Get_ObjType() != OBJ_TYPE::TILE) // 타일은 개수 많고 렌더 안하므로 넣지 않음
					m_RenderSort[(UINT)eOrder].push_back(*iter);
			}
		}
	}
}

void CObjMgr::Render(HDC _hDC)
{
	for (int i = 0; i < (UINT)RENDER_ORDER::TYPEEND; ++i)
	{
		m_RenderSort[i].sort([&](CObj* pDest, CObj* pSrc)->bool
		{
			if (nullptr == pDest->Get_Transform() || nullptr == pSrc->Get_Transform())
				return false;

			return pDest->Get_Transform()->Get_Pos().fY < pSrc->Get_Transform()->Get_Pos().fY;
		});

		for (auto& iter : m_RenderSort[i])
		{ 
			if (iter == nullptr || (iter)->Is_Dead()) // || !((iter)->Is_Active()))
				continue;
			else
			{
				if (!Out_Of_Screen(iter)) // 화면 안에 있는 오브젝트만 렌더링
					(iter)->Render(_hDC);
			}
		}

		m_RenderSort[i].clear();
	}
}

void CObjMgr::Release()
{
	for (int i = 0; i < (UINT)OBJ_TYPE::TYPEEND; ++i)
	{
		for (auto& iter : m_ObjList[i])
		{
			Safe_Delete<CObj*>(iter);
		}
	}
}

list<CObj*>* CObjMgr::Get_ObjList(const OBJ_TYPE & _eType)
{
	return &m_ObjList[(UINT)_eType];
}

CObj * CObjMgr::Get_Player()
{
	if (m_ObjList[(UINT)OBJ_TYPE::PLAYER].empty()) return nullptr;

	return m_ObjList[(UINT)OBJ_TYPE::PLAYER].front();
}

const bool CObjMgr::Add_Obj(const OBJ_TYPE & _eType, CObj * const _pObj)
{

	if (OBJ_TYPE::TYPEEND <= _eType || nullptr == _pObj) return false;

	m_ObjList[(UINT)_eType].push_back(_pObj);
	return true;
}

const bool CObjMgr::Add_Obj(CObj * const _pObj)
{
	if (nullptr == _pObj) return false;

	m_ObjList[(UINT)_pObj->Get_ObjType()].push_back(_pObj);

	return true;
}

const bool CObjMgr::Delete_Obj(CObj * _pObj)
{
	if (nullptr == _pObj) return false;

	OBJ_TYPE eType = _pObj->Get_ObjType();

	if (OBJ_TYPE::TYPEEND <= eType)  return false;

	for (auto& iter = m_ObjList[(UINT)eType].begin(); iter != m_ObjList[(UINT)eType].end();)
	{
		if (*iter == _pObj)
		{
			_pObj->Release();
			Safe_Delete<CObj*>(_pObj);
			iter = m_ObjList[(UINT)eType].erase(iter);
			return true;
		}
		else
			++iter;
	}

	return false;
}

const bool CObjMgr::Remove_Obj(CObj * _pObj)
{
	if (nullptr == _pObj) return false;

	OBJ_TYPE eType = _pObj->Get_ObjType();

	if(OBJ_TYPE::TYPEEND <= eType) return false;

	for (auto& iter = m_ObjList[(UINT)eType].begin(); iter != m_ObjList[(UINT)eType].end();)
	{
		if (*iter == _pObj)
		{
			//_pObj->Release();
			//Safe_Delete<CObj*>(_pObj);
			iter = m_ObjList[(UINT)eType].erase(iter);
			return true;
		}
		else
			++iter;
	}

	return false;
}

const bool CObjMgr::Out_Of_Screen(CObj* _pObj)
{
	VEC2 vCenterPos = CCameraMgr::Get_Inst()->Get_LookAt();

	if (nullptr == _pObj->Get_Transform()) return false; // UI

	VEC2 vProjPos = _pObj->Get_Transform()->Get_Pos();
	VEC2 vProjScale = _pObj->Get_Transform()->Get_Scale();

	if ((vProjPos.fX - (vProjScale.fX * 0.5f) + 300 < vCenterPos.fX - (WINCX * 0.5f) || vCenterPos.fX + (WINCX * 0.5f) + 300 < vProjPos.fX + (vProjScale.fX * 0.5f))
		|| (vProjPos.fY - (vProjScale.fY * 0.5f) + 300 < vCenterPos.fY - (WINCY * 0.5f) || vCenterPos.fY + (WINCY * 0.5f) + 300 < vProjPos.fY + (vProjScale.fY * 0.5f)))
	{
		return true;
	}
	else
		return false;
}
