#pragma once

#include "Base.h"

/* 렌더링 파이프라인의 변환을 위한 뷰, 투영행려을 보관한다. */
/* 렌더링 파이프라인의 변환을 위한 뷰, 투영행렬의 역행렬을 계산, 보관한다. */
/* 카메라 위치도 보관한ㄷ,ㅏㅡ ./*/

NS_BEGIN(Engine)

class CPipeLine final : public CBase
{
private:
	CPipeLine();
	virtual ~CPipeLine() = default;

public:
	void Set_Transform(D3DTS eState, _fmatrix TransformMatrix);
	const _float4x4* Get_Transform_Float4x4(D3DTS eState) const;
	_matrix Get_Transform_Matrix(D3DTS eState) const;
	const _float4* Get_CamPosition() const;


public:
	void Update();

private:
	_float4x4				m_TransformationMatrices[ENUM_CLASS(D3DTS::END)] = {};
	_float4x4				m_TransformationMatrixInverse[ENUM_CLASS(D3DTS::END)] = {};
	_float4					m_vCamPosition = {};

public:
	static CPipeLine* Create();
	virtual void Free() override;
};

NS_END