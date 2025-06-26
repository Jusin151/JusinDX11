#include "Monster.h"

#include "GameInstance.h"

#include "Player.h"

CMonster::CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject { pDevice, pContext }
{

}

CMonster::CMonster(const CMonster& Prototype)
	: CGameObject { Prototype }
{

}

HRESULT CMonster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{
	GAMEOBJECT_DESC			Desc{};

	Desc.fRotationPerSec = 0.f;
	Desc.fSpeedPerSec = 0.f;
	lstrcpy(Desc.szName, TEXT("Monster"));

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	/* 1. 서로 다른 애니메이션을 셋팅했음에도 같은 동작이 재생된다. : 뼈가 공유되기때문에. */
	/* 2. 같은 애니메이션을 셋했다면 재생속도가 빨라진다. : */
	m_pModelCom->Set_Animation(3, true);

	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(
		m_pGameInstance->Compute_Random(0.f, 20.f), 
		3.f,
		m_pGameInstance->Compute_Random(0.f, 20.f),
		1.f
	));

	return S_OK;
}

void CMonster::Priority_Update(_float fTimeDelta)
{

}

void CMonster::Update(_float fTimeDelta)
{
	if (true == m_pModelCom->Play_Animation(fTimeDelta))
		int a = 10;

	for (auto& pCollider : m_pColliderCom)
		pCollider->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));

	Intersect_ToPlayer();
}

void CMonster::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CMonster::Render()
{

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;


	_uint		iNumMesh = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMesh; i++)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
			return E_FAIL;

		m_pModelCom->Bind_Bone_Matrices(m_pShaderCom, "g_BoneMatrices", i);

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

#ifdef _DEBUG
	for (auto& pCollider : m_pColliderCom)
		pCollider->Render();
#endif

	return S_OK;
}

HRESULT CMonster::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Fiona"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Collider */
	CBounding_AABB::AABB_DESC	AABBDesc{};
	AABBDesc.vExtents = _float3(0.3f, 0.8f, 0.3f);
	AABBDesc.vCenter = _float3(0.0f, AABBDesc.vExtents.y, 0.f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_Collider_AABB"), reinterpret_cast<CComponent**>(&m_pColliderCom[COLLIDER_AABB]), &AABBDesc)))
		return E_FAIL;

	/* For.Com_Collider */
	CBounding_OBB::OBB_DESC	OBBDesc{};
	OBBDesc.vExtents = _float3(0.3f, 0.3f, 0.3f);
	OBBDesc.vCenter = _float3(0.0f, OBBDesc.vExtents.y, 0.f);
	OBBDesc.vRotation = _float3(XMConvertToRadians(0.f), XMConvertToRadians(45.f), XMConvertToRadians(0.f));

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_OBB"), reinterpret_cast<CComponent**>(&m_pColliderCom[COLLIDER_OBB]), &OBBDesc)))
		return E_FAIL;

	/* For.Com_Collider */
	CBounding_Sphere::SPHERE_DESC	SphereDesc{};
	SphereDesc.fRadius = 0.5f;
	SphereDesc.vCenter = _float3(0.0f, SphereDesc.fRadius, 0.f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider_Sphere"), reinterpret_cast<CComponent**>(&m_pColliderCom[COLLIDER_SPHERE]), &SphereDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	return S_OK;
}

void CMonster::Intersect_ToPlayer()
{
	CCollider*		pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Player"), CPlayer::PART_WEAPON, TEXT("Com_Collider")));
	if (nullptr == pTargetCollider)
		return;

	m_pColliderCom[COLLIDER_OBB]->Intersect(pTargetCollider);
}

CMonster* CMonster::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster* pInstance = new CMonster(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonster::Clone(void* pArg)
{
	CMonster* pInstance = new CMonster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMonster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster::Free()
{
	__super::Free();

	for (auto& pCollider : m_pColliderCom)
		Safe_Release(pCollider);

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
