#pragma once
/*

주의사항

함수 호출시 포인터 타입이 아닌 '비 포인터' 타입을 넘겨주어야 한다.

ex)
	CObjPool<CDamageUI>::Init(OBJ_TYPE::UI, 50);

	CDamageUI* pUI = static_cast<CDamageUI*>(CObjPool<CDamageUI>::Get_Obj());

*/

#include "EventMgr.h"
#include "Obj.h"
#include "ObjMgr.h"

template <typename T>

class CObjPool
{
public:
	CObjPool() {}
	~CObjPool() { Release(); }

public:
	static const bool Init(const OBJ_TYPE& _eType, const int& _iSize)
	{
		for (int i = 0; i < _iSize; ++i)
		{
			CObj* pObj = new T(_eType);
			pObj->Init();
			pObj->Set_Active(false);
			g_ObjQueue.push(pObj);
		}
		return true;
	}

	void Release()
	{
		if (g_ObjQueue.empty()) return;

		int iSize = g_ObjQueue.size();

		for (int i = 0; i < iSize; ++i)
		{
			CObj* pObj = g_ObjQueue.front();

			if(!pObj->Is_Active())
				Safe_Delete<CObj*>(pObj);

			g_ObjQueue.pop();
		}
	}

public:
	static CObj* Get_Obj()
	{
		if (g_ObjQueue.empty()) return nullptr;

		CObj* pObj = g_ObjQueue.front();

		if (nullptr == pObj) return nullptr;

		g_ObjQueue.pop();

		pObj->Set_Active(true);

		CEventMgr::Get_Inst()->Add_Obj(pObj);

		return pObj;
	}

	static const bool Return_Obj(CObj* _pObj)
	{
		if (nullptr == _pObj) return false;

		_pObj->Set_Active(false);

		g_ObjQueue.push(_pObj);

		CEventMgr::Get_Inst()->Remove_Obj(_pObj);

		return true;
	}

private:
	static queue<CObj*> g_ObjQueue;     // 비활성화된 오브젝트들만 갖고있다.
};

template <typename T>
queue<CObj*> CObjPool<T>::g_ObjQueue;
