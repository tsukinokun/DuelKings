//---------------------------------------------------------------------------
//! @file   FilterColorCorrection.h
//! @brief  色補正フィルター
//---------------------------------------------------------------------------
#pragma once

//===========================================================================
//! 色補正フィルタークラス
//===========================================================================
class FilterColorCorrection : public Object
{
public:
    BP_OBJECT_DECL(FilterColorCorrection, "色補正フィルター")

    //! 初期化
    virtual bool Init() override;

private:
    std::shared_ptr<Texture>  work_texture_;    //!< バックバッファと同じサイズのテクスチャ
    std::shared_ptr<ShaderPs> shader_ps_;       //!< ピクセルシェーダー
};
