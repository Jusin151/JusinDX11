#include "Shadow.h"

CShadow::CShadow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CShadow::Ready_Light_For_Shadow(const SHADOW_DESC& Desc)
{
	

	return S_OK;
}

CShadow* CShadow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new CShadow(pDevice, pContext);
}

void CShadow::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
