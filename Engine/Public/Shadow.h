#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class CShadow final : public CBase
{
public:
	typedef struct tagShadowDesc
	{
		_float4			vEye, vAt;
		_float			fFovy, fNear, fFar;
	}SHADOW_DESC;
private:
	CShadow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CShadow() = default;

public:
	const _float4x4* Get_Light_ViewMatrix() {
		return &m_LightViewMatrix;
	}
	const _float4x4* Get_Light_ProjMatrix() {
		return &m_LightProjMatrix;
	}

public:
	HRESULT Ready_Light_For_Shadow(const SHADOW_DESC& Desc);

private:
	ID3D11Device*					m_pDevice = { nullptr };
	ID3D11DeviceContext*			m_pContext = { nullptr };

private:
	_float4x4						m_LightViewMatrix = {};
	_float4x4						m_LightProjMatrix = {};

public:
	static CShadow* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free();

};

NS_END