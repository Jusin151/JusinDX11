#pragma once

#include "Bounding.h"

NS_BEGIN(Engine)

class CBounding_AABB final : public CBounding
{
public:
	typedef struct tagBoundingBoxDesc : public CBounding::BOUNDING_DESC
	{
		_float3		vExtents;
	}AABB_DESC;
private:
	CBounding_AABB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBounding_AABB() = default;

public:
	HRESULT Initialize(const CBounding::BOUNDING_DESC* pDesc);

private:
	/* 채워지는 초기정보는 로컬상에 상태대로 셋팅한다. */
	BoundingBox* m_pOriginalDesc = {nullptr};
	BoundingBox* m_pDesc = { nullptr };

public:
	static CBounding_AABB* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CBounding::BOUNDING_DESC* pDesc);
	virtual void Free() override;

};

NS_END