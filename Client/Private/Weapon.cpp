#include "Weapon.h"

#include "GameInstance.h"

CWeapon::CWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject{ pDevice, pContext }
{

}

CWeapon::CWeapon(const CWeapon& Prototype)
	: CPartObject{ Prototype }
{

}

HRESULT CWeapon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWeapon::Initialize(void* pArg)
{
	WEAPON_DESC* pDesc = static_cast<WEAPON_DESC*>(pArg);

	m_pSocketMatrix = pDesc->pSocketMatrix;
	m_pParentState = pDesc->pParentState;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(0.1f, 0.1f, 0.1f);
	m_pTransformCom->Rotation(0.f, XMConvertToRadians(90.f), 0.f);
	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(0.8f, 0.f, 0.f, 1.f));

	return S_OK;
}

void CWeapon::Priority_Update(_float fTimeDelta)
{

}

void CWeapon::Update(_float fTimeDelta)
{
	
}

void CWeapon::Late_Update(_float fTimeDelta)
{
	_matrix		BoneMatrix = XMLoadFloat4x4(m_pSocketMatrix);

	for (size_t i = 0; i < 3; i++)
		BoneMatrix.r[i] = XMVector3Normalize(BoneMatrix.r[i]);


	XMStoreFloat4x4(&m_CombinedWorldMatrix,
		XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) * BoneMatrix * XMLoadFloat4x4(m_pParentMatrix)
	);

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CWeapon::Render()
{

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;


	_uint		iNumMesh = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMesh; i++)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
			return E_FAIL;		

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CWeapon::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_ForkLift"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeapon::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_Light(0);

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

CWeapon* CWeapon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWeapon* pInstance = new CWeapon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWeapon::Clone(void* pArg)
{
	CWeapon* pInstance = new CWeapon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMonster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
