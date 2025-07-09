#include "Engine_Shader_Defines.hlsli"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_Texture;

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
    
    float4 vRight : TEXCOORD1;
    float4 vUp : TEXCOORD2;
    float4 vLook : TEXCOORD3;
    float4 vTranslation : TEXCOORD4;
    
    float2 vLifeTime : TEXCOORD5;    
};

struct VS_OUT
{
    /* SV_ : ShaderValue약자 */
    /* 내가 해야할 연산은 다 했으니 이제 니(장치)가 알아서 추가적인 연산을 해라. */     
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;    
    float2 vLifeTime : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;
    
    matrix matWV, matWVP;
    
    matrix TransformMatrix = matrix(In.vRight, In.vUp, In.vLook, In.vTranslation);
    
    vector vPosition = mul(vector(In.vPosition, 1.f), TransformMatrix);
    
    /* mul : 모든 행렬의 곱하기를 수행한다. /w연산을 수행하지 않는다. */
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    Out.vPosition = mul(vPosition, matWVP);
    Out.vTexcoord = In.vTexcoord;    
    Out.vLifeTime = In.vLifeTime;
    
    return Out;
}


struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float2 vLifeTime : TEXCOORD1;
};

struct PS_OUT
{
    vector vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;    
    
    Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);           
    
    if(Out.vColor.a < 0.3f)
        discard;
    
    Out.vColor.a = saturate(In.vLifeTime.x - In.vLifeTime.y);
    
    float2 vRG = 1.f - Out.vColor.a + 0.3f;
    
    Out.vColor = float4(vRG.r, vRG.g, Out.vColor.b, Out.vColor.a);
    
    if (In.vLifeTime.y >= In.vLifeTime.x)
        discard;
    
    return Out;
}



technique11 DefaultTechnique
{
  
    pass Default
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        

        VertexShader = compile vs_5_0 VS_MAIN();        
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();      
    }
 
}
