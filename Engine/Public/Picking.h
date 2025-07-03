#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class CPicking final : public CBase
{
private:
	CPicking(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CPicking() = default;

public:
	HRESULT Initialize();
	void Update();

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	ID3D11Texture2D*		m_pTexture = { nullptr };

public:
	static CPicking* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

NS_END