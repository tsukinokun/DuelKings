#pragma once
//---------------------------------------------------------------------------
//!	@file	ComponentImage.h
//! @brief	ボタンコンポーネント
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#include <System/Scene.h>
#include <System/Component/Component.h>

USING_PTR(ComponentButton);

class ComponentButton : public Component
{
public:
    BP_COMPONENT_DECL(ComponentButton, u8"ボタンコンポーネント");
    //! @{
    //	初期化処理
    void Init() override;
    //ImGui
    void GUI() override;
    //---------------------------------------------------------------------------
    //  クリックされているかを返す関数
    //! @return クリックされているかを返す
    //---------------------------------------------------------------------------
    bool IsClick() const;
    //! @}
    //--------------------------------------------------------------------
    //! @name Cereal処理
    //--------------------------------------------------------------------
    //@{

    //! @brief セーブ
    // @param arc アーカイバ
    // @param ver バージョン
    CEREAL_SAVELOAD(arc, ver) { arc(cereal::make_nvp("Component", cereal::base_class<Component>(this))); }
};

CEREAL_CLASS_VERSION(ComponentButton, 3);
