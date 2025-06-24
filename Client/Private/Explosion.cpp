#include "Explosion.h"

#include "GameInstance.h"

CExplosion::CExplosion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject{ pDevice, pContext }
{

}

CExplosion::CExplosion(const CExplosion& Prototype)
	: CPartObject{ Prototype }
{

}

HRESULT CExplosion::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CExplosion::Initialize(void* pArg)
{
	EXPLOSION_DESC* pDesc = static_cast<EXPLOSION_DESC*>(pArg);

	m_pSocketMatrix = pDesc->pSocketMatrix;
	m_pParentState = pDesc->pParentState;


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CExplosion::Priority_Update(_float fTimeDelta)
{

}

void CExplosion::Update(_float fTimeDelta)
{
	
	m_pVIBufferCom->Spread(fTimeDelta);

	_matrix		BoneMatrix = XMLoadFloat4x4(m_pSocketMatrix);

	for (size_t i = 0; i < 3; i++)
		BoneMatrix.r[i] = XMVector3Normalize(BoneMatrix.r[i]);


	XMStoreFloat4x4(&m_CombinedWorldMatrix,
		XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) * BoneMatrix * XMLoadFloat4x4(m_pParentMatrix)
	);

}

void CExplosion::Late_Update(_float fTimeDelta)
{
	/* WeightBlend */	
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this); 
}

HRESULT CExplosion::Render()
{

	
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;



	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CExplosion::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxPosInstance"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Explosion"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Snow"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CExplosion* CExplosion::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CExplosion* pInstance = new CExplosion(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CExplosion::Clone(void* pArg)
{
	CExplosion* pInstance = new CExplosion(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CExplosion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CExplosion::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
