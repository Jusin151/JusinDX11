#include "Shader.h"

CShader::CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent { pDevice, pContext }
{
}

CShader::CShader(const CShader& Prototype)
	: CComponent{ Prototype }
	, m_pEffect { Prototype.m_pEffect }
	, m_InputLayouts { Prototype.m_InputLayouts }
{
	for (auto& pInputLayout : m_InputLayouts)
		Safe_AddRef(pInputLayout);

	Safe_AddRef(m_pEffect);
}

HRESULT CShader::Initialize_Prototype(const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements)
{
	_uint		iShaderFlag = {};

#ifdef _DEBUG
	iShaderFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	iShaderFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif
	if (FAILED(D3DX11CompileEffectFromFile(pShaderFilePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, iShaderFlag, 0, m_pDevice, &m_pEffect, nullptr)))
		return E_FAIL;

	ID3DX11EffectTechnique*		pTechnique = m_pEffect->GetTechniqueByIndex(0);
	if (nullptr == pTechnique)
		return E_FAIL;

	D3DX11_TECHNIQUE_DESC		TechniqueDesc{};

	pTechnique->GetDesc(&TechniqueDesc);

	m_iNumPasses = TechniqueDesc.Passes;

	m_InputLayouts.reserve(m_iNumPasses);

	for (size_t i = 0; i < TechniqueDesc.Passes; i++)
	{
		ID3D11InputLayout*	pInputLayout = { nullptr };
		ID3DX11EffectPass*	pPass = pTechnique->GetPassByIndex(static_cast<uint32_t>(i));

		D3DX11_PASS_DESC		PassDesc{};

		pPass->GetDesc(&PassDesc);

		/* PassDesc.pIAInputSignature : 패스가 어떤 정점을 받는다. */
		/* PassDesc.IAInputSignatureSize : 패스가 받고 있는 정점의 크기 .*/

		if (FAILED(m_pDevice->CreateInputLayout(pElements, iNumElements,
			PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &pInputLayout)))
			return E_FAIL;

		m_InputLayouts.push_back(pInputLayout);
	}	

	return S_OK;
}

HRESULT CShader::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CShader::Begin(_uint iPassIndex)
{
	m_pContext->IASetInputLayout(m_InputLayouts[iPassIndex]);	

	ID3DX11EffectPass* pPass = m_pEffect->GetTechniqueByIndex(0)->GetPassByIndex(iPassIndex);
	if (nullptr == pPass)
		return E_FAIL;

	/* 이 쉐이더에 이 패스로 그린다. */
	/* Apply() 이전에 쉐이더에 전달해야할 모든 데이터들을 다 전달해놔야한다. */
	return pPass->Apply(0, m_pContext);
}

HRESULT CShader::Bind_RawValue(const _char* pConstantName, const void* pData, _uint iLength)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	return pVariable->SetRawValue(pData, 0, iLength);	
}

HRESULT CShader::Bind_Matrix(const _char* pConstantName, const _float4x4* pMatrix)
{
	ID3DX11EffectVariable*		pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
	if (nullptr == pMatrixVariable)
		return E_FAIL;

	return pMatrixVariable->SetMatrix(reinterpret_cast<const _float*>(pMatrix));	
}

HRESULT CShader::Bind_Matrices(const _char* pConstantName, const _float4x4* pMatrix, _uint iNumMatrices)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
	if (nullptr == pMatrixVariable)
		return E_FAIL;

	return pMatrixVariable->SetMatrixArray(reinterpret_cast<const _float*>(pMatrix), 0, iNumMatrices);
}

HRESULT CShader::Bind_SRV(const _char* pConstantName, ID3D11ShaderResourceView* pSRV)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	
	ID3DX11EffectShaderResourceVariable* pShaderResourceVariable = pVariable->AsShaderResource();
	if (nullptr == pShaderResourceVariable)
		return E_FAIL;

	return pShaderResourceVariable->SetResource(pSRV);
}

CShader* CShader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements)
{
	CShader* pInstance = new CShader(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pShaderFilePath, pElements, iNumElements)))
	{
		MSG_BOX("Failed to Created : CShader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CShader::Clone(void* pArg)
{
	CShader* pInstance = new CShader(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CShader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShader::Free()
{
	__super::Free();

	for (auto& pInputLayout : m_InputLayouts)
		Safe_Release(pInputLayout);

	m_InputLayouts.clear();

	Safe_Release(m_pEffect);
}
