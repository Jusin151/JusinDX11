#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class CTarget_Manager final : public CBase
{
private:
	CTarget_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTarget_Manager() = default;

public:
	HRESULT Initialize();
	HRESULT Add_RenderTarget(const _wstring& strTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor);
	HRESULT Add_MRT(const _wstring& strMRTTag, const _wstring& strTargetTag);

	/*특정 타겟들을 장치에 동시(최대8개)에 바인딩한다. */
	HRESULT Begin_MRT(const _wstring& strMRTTag);
	HRESULT End_MRT();
	HRESULT Bind_ShaderResource(const _wstring& strTargetTag, class CShader* pShader, const _char* pContantName);


#ifdef _DEBUG
public:
	HRESULT Ready_Debug(const _wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_Debug(const _wstring& strMRTTag, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);

#endif

private:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };

	ID3D11RenderTargetView* m_pBackBuffer = { nullptr };
	ID3D11DepthStencilView* m_pOriginalDSV = { nullptr };

private:
	map<const _wstring, class CRenderTarget*>		m_RenderTargets;
	map<const _wstring, list<class CRenderTarget*>>	m_MRTs;

private:
	class CRenderTarget* Find_RenderTarget(const _wstring& strTargetTag);
	list<class CRenderTarget*>* Find_MRT(const _wstring& strMRTTag);

public:
	static CTarget_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

NS_END