//---------------------------------------------------------------------------
//! @file   Shadowmap.h
//! @brief  シャドウマップ
//---------------------------------------------------------------------------
#pragma once

//===========================================================================
//! シャドウマップ管理クラス
//===========================================================================
class Shadowmap : public Object
{
public:
    BP_OBJECT_DECL(Shadowmap, "シャドウマップ")

    //! 初期化
    virtual bool Init() override;

    //! 更新後処理
    virtual void PostUpdate() override;

    matrix getLightViewMatrix() const { return mat_light_view_; }
    matrix getLightProjMatrix() const { return mat_light_proj_; }

private:
    static constexpr s32 RESOLUTION = 2048 * 4;    //!< シャドウマップ解像度

    std::shared_ptr<Texture> texture_color_;    //!< シャドウ用カラー (DxLibの制約で必要)
    std::shared_ptr<Texture> texture_depth_;    //!< シャドウ用デプス

    float3 center_position_ = float3(0.0f, 0.0f, 0.0f);                //!< シャドウの中心ワールド座標
    float3 light_dir_       = normalize(float3(1.0f, 1.0f, -1.0f));    //!< 光源がある方向
    float  light_height_    = 512.0f;                                  //!< 光源の高さ

    matrix mat_light_view_ = matrix::identity();    //!< 光源用ビュー行列
    matrix mat_light_proj_ = matrix::identity();    //!< 光源用投影行列
};
