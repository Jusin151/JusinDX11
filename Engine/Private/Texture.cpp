#include "Texture.h"
#include "Shader.h"

CTexture::CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent { pDevice, pContext }
{
}

CTexture::CTexture(const CTexture& Prototype)
	: CComponent{ Prototype }
	, m_SRVs{ Prototype.m_SRVs }
	, m_iNumTextures { Prototype.m_iNumTextures }
{

	for (auto& pSRV : m_SRVs)
		Safe_AddRef(pSRV);
}

HRESULT CTexture::Initialize_Prototype(const _tchar* pTextureFilePath, _uint iNumTextures)
{
	_tchar			szTexturePath[MAX_PATH] = {};
	_tchar			szExt[MAX_PATH] = {};

	_wsplitpath_s(pTextureFilePath, nullptr, 0, nullptr, 0, nullptr, 0, szExt, MAX_PATH);

	for (size_t i = 0; i < iNumTextures; i++)
	{
		ID3D11ShaderResourceView* pSRV = { nullptr };

		wsprintf(szTexturePath, pTextureFilePath, i);
		
		HRESULT			hr = {};

		if (false == lstrcmp(TEXT(".dds"), szExt))
			hr = CreateDDSTextureFromFile(m_pDevice, szTexturePath, nullptr, &pSRV);

		else if (false == lstrcmp(TEXT(".tga"), szExt))
		{
			MSG_BOX("Failed to TGA");
			hr = E_FAIL;
		}
		else
			hr = CreateWICTextureFromFile(m_pDevice, szTexturePath, nullptr, &pSRV);

		if (FAILED(hr))
			return E_FAIL;

		m_SRVs.push_back(pSRV);
	}

	m_iNumTextures = iNumTextures;

	return S_OK;
}

HRESULT CTexture::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CTexture::Bind_ShaderResource(CShader* pShader, const _char* pConstantName, _uint iTextureIndex)
{
	if (iTextureIndex >= m_iNumTextures)
		return E_FAIL;

	return pShader->Bind_SRV(pConstantName, m_SRVs[iTextureIndex]);	
}

HRESULT CTexture::Bind_ShaderResources(CShader* pShader, const _char* pConstantName)
{
	return pShader->Bind_SRVs(pConstantName, &m_SRVs.front(), m_iNumTextures);	
}

CTexture* CTexture::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pTextureFilePath, _uint iNumTextures)
{
	CTexture* pInstance = new CTexture(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pTextureFilePath, iNumTextures)))
	{
		MSG_BOX("Failed to Created : CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent* CTexture::Clone(void* pArg)
{
	CTexture* pInstance = new CTexture(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;

}

void CTexture::Free()
{
	__super::Free();

	for (auto& pSRV : m_SRVs)
		Safe_Release(pSRV);

	m_SRVs.clear();
}
