#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class CTimer final : public CBase
{
private:
	CTimer();
	virtual ~CTimer() = default;

public:
	_float Get_TimeDelta() {	
		return m_fTimeDelta;	}

public:
	HRESULT Initialize();
	void Update();

private:
	LARGE_INTEGER			m_CurTime{};
	LARGE_INTEGER			m_OldTime{};
	LARGE_INTEGER			m_OriginTime{};
	LARGE_INTEGER			m_CpuTick{};

	_float					m_fTimeDelta{};

public:
	static CTimer* Create();
	virtual void Free();
};

NS_END