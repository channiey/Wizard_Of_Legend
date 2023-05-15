#pragma once

template <typename T>
void Safe_Delete(T& _p)
{
	if (_p)
	{
		delete _p;
		_p = nullptr;
	}
}

template <typename T>
void Safe_Delete_Arr(T& _pArr)
{
	if (_pArr)
	{
		delete[] _pArr;
		_pArr = nullptr;
	}
}

template <typename T>
void Safe_Delete_Vector(vector<T>& _vec)
{
	for (size_t i = 0; i < _vec.size(); ++i)
	{
		if (nullptr != _vec[i])
			delete _vec[i];
	}
	_vec.clear();
}

template <typename T1, typename T2>
void Safe_Delete_Map(map<T1, T2>& _map)
{
	typename map<T1, T2>::iterator iter = _map.begin();

	for (; iter != _map.end(); ++iter)
	{
		if (nullptr != iter->second)
			delete iter->second;
	}
	_map.clear();
}