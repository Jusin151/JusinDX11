
sampler DefaultSampler = sampler_state
{
    filter = min_mag_mip_linear;
    AddressU = wrap;
    AddressV = wrap;
};


sampler PointSampler = sampler_state
{
    filter = min_mag_mip_point;
    AddressU = wrap;
    AddressV = wrap;
};

sampler LinearClampSampler = sampler_state
{
    filter = min_mag_mip_linear;
    AddressU = clamp;
    AddressV = clamp;
};

sampler MirrorSampler = sampler_state
{
    filter = min_mag_mip_linear;
    AddressU = mirror;
    AddressV = mirror;
};

RasterizerState RS_Default
{
    FillMode = Solid;
    CullMode = back;
};

RasterizerState RS_Cull_Front
{
    FillMode = Solid;
    CullMode = front;
};


DepthStencilState DSS_Default
{
    DepthEnable = true;
    DepthWriteMask = all;
    DepthFunc = less_equal;
};

DepthStencilState DSS_None
{
    DepthEnable = false;
    DepthWriteMask = zero;    
};

BlendState BS_Default
{
    BlendEnable[0] = false;

};

BlendState BS_AlphaBlend
{
    BlendEnable[0] = true;
    SrcBlend = Src_Alpha;
    DestBlend = Inv_Src_Alpha;
    BlendOp = Add;
};
BlendState BS_OneBlend
{
    BlendEnable[0] = true;
    BlendEnable[1] = true;
    SrcBlend = one;
    DestBlend = one;
    BlendOp = Add;
};

