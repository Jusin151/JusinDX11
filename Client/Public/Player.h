#pragma once

#include "Client_Defines.h"
#include "ContainerObject.h"

NS_BEGIN(Engine)
NS_END

NS_BEGIN(Client)

class CPlayer final : public CContainerObject
{
public:
	enum PART { PART_BODY, PART_WEAPON, PART_EFFECT, PART_END };
	enum STATE { 		
		STATE_IDLE		= 0x00000001,
		STATE_WALK		= 0x00000002, 
		STATE_ATTACK	= 0x00000004,
	};

private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& Prototype);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_PartObjects();
	HRESULT Ready_Components();


private:
	_uint			m_iState = {};

public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END