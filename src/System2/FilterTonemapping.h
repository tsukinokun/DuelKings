//---------------------------------------------------------------------------
//! @file   FilterTonemapping.h
//! @brief  トーンマッピングフィルター
//---------------------------------------------------------------------------
#pragma once

//===========================================================================
//! トーンマッピングフィルタークラス
//===========================================================================
class FilterTonemapping : public Object
{
public:
    BP_OBJECT_DECL(FilterTonemapping, "Tonemapping")

    //! 初期化
    virtual bool Init() override;

private:
    std::shared_ptr<ShaderPs> shader_ps_;    //!< ピクセルシェーダー
};
