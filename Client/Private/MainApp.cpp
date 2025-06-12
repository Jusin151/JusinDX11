#include "MainApp.h"
#include "GameInstance.h"
#include "Level_Loading.h"

CMainApp::CMainApp()
	: m_pGameInstance { CGameInstance::Get_Instance() }
{
	// D3D11_RASTERIZER_DESC
	// D3D11_DEPTH_STENCIL_DESC
	//D3D11_RASTERIZER_DESC RSDesc{};
	//RSDesc.CullMode = D3D11_CULL_BACK;

		

	//ID3D11RasterizerState* pRSState = { nullptr };
	//m_pDevice->CreateRasterizerState(&RSDesc, &pRSState);

	//m_pContext->RSSetState(pRSState);

	//m_pContext->OMSetBlendState();

	//m_pContext->OMSetDepthStencilState();

	//D3D11_BLEND_DESC


	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	ENGINE_DESC			EngineDesc{};

	EngineDesc.hInstance = g_hInst;
	EngineDesc.hWnd = g_hWnd;
	EngineDesc.iWinSizeX = g_iWinSizeX;
	EngineDesc.iWinSizeY = g_iWinSizeY;
	EngineDesc.isWindowed = true;
	EngineDesc.iNumLevels = static_cast<_uint>(LEVEL::END);

	if (FAILED(m_pGameInstance->Initialize_Engine(EngineDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;

	if (FAILED(Ready_Gara()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Component()))
		return E_FAIL;

	if (FAILED(Start_Level(LEVEL::LOGO)))
		return E_FAIL;

    return S_OK;
}

void CMainApp::Update(_float fTimeDelta)
{
	m_pGameInstance->Update_Engine(fTimeDelta);
}

HRESULT CMainApp::Render()
{
	m_pGameInstance->Begin_Draw();

	m_pGameInstance->Draw();

	m_pGameInstance->Draw_Font(TEXT("Font_151"), TEXT("배가 고파요"), _float2(0.f, 0.f), XMVectorSet(1.f, 0.f, 0.f, 1.f));

	m_pGameInstance->End_Draw();

    return S_OK;
}

HRESULT CMainApp::Ready_Gara()
{
	ID3D11Texture2D* pTexture2D = { nullptr };

	D3D11_TEXTURE2D_DESC		TextureDesc{};
	TextureDesc.Width = 256;
	TextureDesc.Height = 256;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.Usage = D3D11_USAGE_STAGING;	
	TextureDesc.BindFlags = 0;		
	TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	TextureDesc.MiscFlags = 0;

	_uint* pPixel = new _uint[256 * 256];

	for (size_t i = 0; i < 256; i++)
	{
		for (size_t j = 0; j < 256; j++)
		{
			_uint iIndex = i * 256 + j;

			/* a b g r */
			pPixel[iIndex] = D3DCOLOR_ARGB(255, 0, 0, 0);/*0xff000000*//*0b00000000000000000000000011111111*/;

		}

	}

	/*m_pContext->CopyResource(동적, 정적)*/
	

	D3D11_SUBRESOURCE_DATA		InitialDesc{};
	InitialDesc.pSysMem = pPixel;
	InitialDesc.SysMemPitch = 256 * 4;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, &InitialDesc, &pTexture2D)))
		return E_FAIL;

	D3D11_MAPPED_SUBRESOURCE		SubResource{};

	m_pContext->Map(pTexture2D, 0, D3D11_MAP_READ_WRITE, 0, &SubResource);

	_uint* pPixels = static_cast<_uint*>(SubResource.pData);

	for (size_t i = 0; i < 256; i++)
	{
		for (size_t j = 0; j < 256; j++)
		{
			_uint iIndex = i * 256 + j;

			if(j < 128)
			/* a b g r */
				pPixels[iIndex] = D3DCOLOR_ARGB(255, 0, 0, 0);/*0xff000000*//*0b00000000000000000000000011111111*/
			else
				pPixels[iIndex] = D3DCOLOR_ARGB(255, 255, 255, 255);/*0xff000000*//*0b00000000000000000000000011111111*/;


		}

	}

	m_pContext->Unmap(pTexture2D, 0);

	/*m_pVB->Lock(0, 0, void**, 0);

	m_pVB->unlock();

	m_pTexture->LockRect();*/

	



	if (FAILED(DirectX::SaveDDSTextureToFile(m_pContext, pTexture2D, TEXT("../Bin/Resources/Textures/Terrain/Mask.dds"))))
		return E_FAIL;

	Safe_Delete_Array(pPixel);

	Safe_Release(pTexture2D);

	/*MakeSpriteFont "배찌체" /FontSize:20 /FastPack /CharacterRegion:0x0020-0x00FF /CharacterRegion:0x3131-0x3163 /CharacterRegion:0xAC00-0xD800 /DefaultCharacter:0xAC00 151ex.spritefont */
	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_151"), TEXT("../Bin/Resources/Fonts/151ex.spritefont"))))
		return E_FAIL;

	_float3		vPoints[3] = {};

	_ulong		dwByte = {};
	HANDLE		hFile = CreateFile(TEXT("../Bin/DataFiles/Navigation.dat"), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	vPoints[0] = _float3(0.0f, 2.f, 10.f);
	vPoints[1] = _float3(10.f, 0.f, 0.f);
	vPoints[2] = _float3(0.f, 5.f, 0.f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	vPoints[0] = _float3(0.0f, 2.f, 10.f);
	vPoints[1] = _float3(10.f, 0.f, 10.f);
	vPoints[2] = _float3(10.f, 0.f, 0.f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	vPoints[0] = _float3(0.0f, 0.f, 20.f);
	vPoints[1] = _float3(10.f, 0.f, 10.f);
	vPoints[2] = _float3(0.f, 2.f, 10.f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);


	vPoints[0] = _float3(10.f, 0.f, 10.f);
	vPoints[1] = _float3(20.f, 0.f, 0.f);
	vPoints[2] = _float3(10.f, 0.f, 0.f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component()
{
	/* For.Prototype_Component_Collider_AABB */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext, COLLIDER::AABB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pContext, COLLIDER::OBB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_Sphere */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_Sphere"),
		CCollider::Create(m_pDevice, m_pContext, COLLIDER::SPHERE))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPosTex */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPosTex */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Start_Level(LEVEL eStartLevel)
{
	if (FAILED(m_pGameInstance->Change_Level(static_cast<_uint>(LEVEL::LOADING), CLevel_Loading::Create(m_pDevice, m_pContext, eStartLevel))))
		return E_FAIL;

	return S_OK;
}

CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
    __super::Free();

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	m_pGameInstance->Release_Engine();

	Safe_Release(m_pGameInstance);

	
}
