//----------------------------------------------------------------------------
//!	@file	ps_model.fx
//!	@brief	MV1モデルピクセルシェーダー
//----------------------------------------------------------------------------
#include "dxlib_ps.h"

// 頂点シェーダーの出力
struct VS_OUTPUT_MODEL
{
    float4 position_ : SV_Position; //!< 座標       (スクリーン空間)
    float4 curr_position_ : CURR_POSITION; //!< 現在の座標 (スクリーン空間)
    float3 world_position_ : WORLD_POSITION; //!< ワールド座標
    float3 normal_ : NORMAL0; //!< 法線
    float4 diffuse_ : COLOR0; //!< Diffuseカラー
    float2 uv0_ : TEXCOORD0; //!< テクスチャ座標
    float4 prev_position_ : PREV_POSITION; //!< 1フレーム前の座標 (スクリーン空間) ※末尾に追加されているため注意
};

typedef VS_OUTPUT_MODEL PS_INPUT_MODEL;

//---------------------------------------------------
//  定数バッファ
// CPUから受け渡される値。ReadOnly。
//---------------------------------------------------
cbuffer CameraInfo : register(b10)
{
    matrix mat_view_; //!<ビュー行列
    matrix mat_proj_; //!<投影行列
    float3 eye_position_; //!<カメラ位置
};


//----------------------------------------------------------------------------
// メイン関数
//----------------------------------------------------------------------------
PS_OUTPUT main(PS_INPUT_MODEL input)
{
    PS_OUTPUT output;

    float2 uv = input.uv0_;
    float3 N = normalize(input.normal_); // 法線
    
    float3 V = normalize(eye_position_ - input.world_position_); // 視線ベクトル

	//------------------------------------------------------------
	// テクスチャカラーを読み込み
	//------------------------------------------------------------
    float4 textureColor = DiffuseTexture.Sample(DiffuseSampler, uv);

    // アルファテスト
    if (textureColor.a < 0.5)
        discard;

    output.color0_ = textureColor; // * input.diffuse_;
    
    float3 L = normalize(float3(1, 1, -1)); //光源のある方向
    float3 lightColor = float3(1, 1, 1); //ライトの色
    
    float3 H = normalize(L + V); //ハーフベクトル
    
    float NdotH = saturate(dot(N, H)); //法線とハーフベクトルの内積
    
    //------------------------------------------------------------
    // 拡散反射光 Diffuse
    // Lambert model
    //------------------------------------------------------------
    float diffuse = saturate(dot(N, L)); //拡散反射光(ランバートモデル)
    float3 diffuseTerm = lightColor * textureColor.rgb * diffuse; //Diffuse項
    //--------------------------------------------
    // 鏡面反射光 Specular
    // Blinn-Phong model
    //--------------------------------------------
    float shininess = 1000; //テカり具合
    float specular = pow(NdotH, shininess);
    float3 specularTerm = lightColor * specular;
    
    //--------------------------------------------
    // 環境光 Ambient
    //--------------------------------------------
    float3 ambient = float3(0.5f, 0.5f, 0.5f);
    float3 ambientTerm = ambient * textureColor.rgb;
    
    output.color0_ = float4(diffuseTerm + specularTerm + ambientTerm, 1);
    
    //output.color0_ = float4(N, 1);

    
	// 出力パラメータを返す
    return output;
}
