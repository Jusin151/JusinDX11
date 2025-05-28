#include "Level_GamePlay.h"
#include "GameInstance.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
		: CLevel { pDevice, pContext }
{

}

HRESULT CLevel_GamePlay::Initialize()
{
	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{
	int a = 10;
}

HRESULT CLevel_GamePlay::Render()
{
	SetWindowText(g_hWnd, TEXT("게임플레이 레벨입니다."));

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Lights()
{
	LIGHT_DESC			LightDesc{};

	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);	
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Terrain"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	for (size_t i = 0; i < 10; i++)
	{
		if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_ForkLift"),
			ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Camera_Free"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Monster(const _wstring strLayerTag)
{
	for (size_t i = 0; i < 10; i++)
	{
		if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Monster"),
			ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
			return E_FAIL;
	}
	

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

CLevel_GamePlay* CLevel_GamePlay::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_GamePlay* pInstance = new CLevel_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_GamePlay::Free()
{
	__super::Free();

}
