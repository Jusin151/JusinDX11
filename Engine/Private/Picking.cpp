#include "Picking.h"
#include "GameInstance.h"


CPicking::CPicking(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
	, m_pGameInstance { CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

}

HRESULT CPicking::Initialize(HWND hWnd)
{
	m_hWnd = hWnd;
	_uint				iNumViewports = { 1 };
	D3D11_VIEWPORT		ViewportDesc{};

	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);


	D3D11_TEXTURE2D_DESC		TextureDesc{};
	TextureDesc.Width = ViewportDesc.Width;
	TextureDesc.Height = ViewportDesc.Height;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.Usage = D3D11_USAGE_STAGING;
	TextureDesc.BindFlags = 0;
	TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &m_pTexture)))
		return E_FAIL;

	m_pWorldPostions = new _float4[TextureDesc.Width * TextureDesc.Height];
	ZeroMemory(m_pWorldPostions, sizeof(_float4) * TextureDesc.Width * TextureDesc.Height);

	m_iWidth = TextureDesc.Width;
	m_iHeight = TextureDesc.Height;

	return S_OK;
}

void CPicking::Update()
{
	if (FAILED(m_pGameInstance->Copy_RT_Resource(TEXT("Target_PickPos"), m_pTexture)))
		return;

	D3D11_MAPPED_SUBRESOURCE		SubResource{};

	m_pContext->Map(m_pTexture, 0, D3D11_MAP_READ, 0, &SubResource);

	memcpy(m_pWorldPostions, SubResource.pData, sizeof(_float4) * m_iWidth * m_iHeight);

	m_pContext->Unmap(m_pTexture, 0);	
}

_bool CPicking::Picking(_float4* pOut)
{
	POINT			ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);

	_uint			iIndex = ptMouse.y * m_iWidth + ptMouse.x;

	// *pOut = m_pWorldPostions[iIndex].w > 0.f ? m_pWorldPostions[iIndex] : *pOut;

	*pOut = m_pWorldPostions[iIndex];

	return static_cast<_bool>(m_pWorldPostions[iIndex].w);	
}

CPicking* CPicking::Create(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPicking* pInstance = new CPicking(pDevice, pContext);

	if (FAILED(pInstance->Initialize(hWnd)))
	{
		MSG_BOX("Failed to Created : CPicking");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPicking::Free()
{
	__super::Free();

	Safe_Delete_Array(m_pWorldPostions);

	Safe_Release(m_pGameInstance);

	Safe_Release(m_pTexture);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
