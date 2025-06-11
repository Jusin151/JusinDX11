#include "Player.h"

#include "GameInstance.h"
#include "Body_Player.h"
#include "Weapon.h"

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CContainerObject{ pDevice, pContext }
{

}

CPlayer::CPlayer(const CPlayer& Prototype)
	: CContainerObject{ Prototype }
{

}

HRESULT CPlayer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
	CONTAINEROBJECT_DESC	Desc{};

	Desc.fRotationPerSec = XMConvertToRadians(180.0f);
	Desc.fSpeedPerSec = 5.0f;
	Desc.iNumPartObjects = PART_END;
	lstrcpy(Desc.szName, TEXT("Player"));

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;	

	m_pTransformCom->Set_State(Engine::STATE::POSITION,
		XMVectorSet(0.f, 3.f, 0.f, 1.f));

	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

}

void CPlayer::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	if (GetKeyState(VK_LEFT) & 0x8000)
	{

		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * -1.f);
	}

	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta);
	}

	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		if (m_iState & STATE_IDLE)
			m_iState ^= STATE_IDLE;

		m_iState |= STATE_WALK;

		m_pTransformCom->Go_Backward(fTimeDelta);
	}


	if (GetKeyState(VK_UP) < 0)
	{
		if (m_iState & STATE_IDLE)
			m_iState ^= STATE_IDLE;

		m_iState |= STATE_WALK;
		m_pTransformCom->Go_Straight(fTimeDelta);
	}
	else
		m_iState = STATE_IDLE;

}

void CPlayer::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);

	
}

HRESULT CPlayer::Render()
{

	m_pNavigationCom->Render();


	return S_OK;
}

HRESULT CPlayer::Ready_PartObjects()
{
	/* For.Body */
	CBody_Player::BODY_PLAYER_DESC BodyDesc{};

	BodyDesc.pParentState = &m_iState;
	BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();

	if (FAILED(__super::Add_PartObject(PART_BODY, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Player"), &BodyDesc)))
		return E_FAIL;

	/* For.Weapon */
	CWeapon::WEAPON_DESC	WeaponDesc{};

	WeaponDesc.pSocketMatrix = dynamic_cast<CBody_Player*>(m_PartObjects[PART_BODY])->Get_SocketMatrix("SWORD");
	WeaponDesc.pParentState = &m_iState;
	WeaponDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();

	if (FAILED(__super::Add_PartObject(PART_WEAPON, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Weapon"), &WeaponDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CPlayer::Ready_Components()
{
	/* For.Com_Navigation */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom))))
		return E_FAIL;

	return S_OK;
}

CPlayer* CPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer* pInstance = new CPlayer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	Safe_Release(m_pNavigationCom);
}
