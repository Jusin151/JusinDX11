#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class CPicking final : public CBase
{
private:
	CPicking(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CPicking() = default;

public:
	HRESULT Initialize(HWND hWnd);
	void Update();
	_bool Picking(_float4* pOut);

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	ID3D11Texture2D*		m_pTexture = { nullptr };
	class CGameInstance*	m_pGameInstance = { nullptr };
	HWND					m_hWnd = {};
	_uint					m_iWidth{}, m_iHeight{};

	_float4* m_pWorldPostions = { nullptr };

public:
	static CPicking* Create(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

NS_END