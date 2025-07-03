#pragma once

#include "GameObject.h"

/* 블렌딩이 필요한 객체들은 카메라로 부터의 깊이를 계산하여ㅐ 저장한다. */
/* 깊이를 통한 정렬. */

NS_BEGIN(Engine)

class ENGINE_DLL CBlendObject abstract : public CGameObject
{
protected:
	CBlendObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBlendObject(const CBlendObject& Prototype);
	virtual ~CBlendObject() = default;

public:
	_float Get_Depth() const {
		return m_fDepth;
	}

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	/* 카메라로부터의 깊이를 계산한다.  */
	_float				m_fDepth = {};

protected:
	void Compute_CameraDepth(_fvector vPosition);
	
public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free();
};

NS_END