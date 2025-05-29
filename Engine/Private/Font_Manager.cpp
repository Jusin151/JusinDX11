#include "Font_Manager.h"
#include "CustomFont.h"

CFont_Manager::CFont_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
{
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
}

HRESULT CFont_Manager::Initialize()
{
	m_pBatch = new SpriteBatch(m_pContext);

	return S_OK;
}

HRESULT CFont_Manager::Add_Font(const _wstring& strFontTag, const _tchar* pFontFilePath)
{
	if (nullptr != Find_Font(strFontTag))
		return E_FAIL;

	CCustomFont* pFont = CCustomFont::Create(m_pDevice, m_pContext, pFontFilePath);
	if (nullptr == pFont)
		return E_FAIL;

	m_Fonts.emplace(strFontTag, pFont);

	return S_OK;
}

void CFont_Manager::Draw(const _wstring& strFontTag, const _tchar* pText, const _float2& vPosition, _fvector vColor, _float fRotation, const _float2& vOrigin, _float fScale)
{
	CCustomFont* pFont = Find_Font(strFontTag);
	if (nullptr == pFont)
		return;

	m_pBatch->Begin();

	pFont->Draw(m_pBatch, pText, vPosition, vColor, fRotation, vOrigin, fScale);

	m_pBatch->End();
}

CCustomFont* CFont_Manager::Find_Font(const _wstring& strFontTag)
{
	auto	iter = m_Fonts.find(strFontTag);

	if(iter == m_Fonts.end())
		return nullptr;

	return iter->second;
}

CFont_Manager* CFont_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFont_Manager* pInstance = new CFont_Manager(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CFont_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CFont_Manager::Free()
{
	__super::Free();

	for (auto& Pair : m_Fonts)
		Safe_Release(Pair.second);

	m_Fonts.clear();

	Safe_Delete(m_pBatch);

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}
