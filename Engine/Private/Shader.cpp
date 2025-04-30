#include "Shader.h"

CShader::CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent { pDevice, pContext }
{
}

CShader::CShader(const CShader& Prototype)
	: CComponent{ Prototype }
{
}

HRESULT CShader::Initialize_Prototype(const _tchar* pShaderFilePath)
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

	//ID3DX11EffectPass*	pPass = pTechnique->GetPassByIndex(0);

	//D3DX11_PASS_DESC		PassDesc{};
	//pPass->GetDesc(&PassDesc);

	////


	//m_pDevice->CreateInputLayout(내가 쉐이더에 입력하려하는 정점의 정보, 멤버변수 갯수, 
	//	PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &m_pInputLayout)

	






	return S_OK;
}

HRESULT CShader::Initialize(void* pArg)
{
	return S_OK;
}

CShader* CShader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pShaderFilePath)
{
	CShader* pInstance = new CShader(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pShaderFilePath)))
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


}
