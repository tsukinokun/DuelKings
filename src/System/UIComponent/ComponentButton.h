#pragma once
//---------------------------------------------------------------------------
//!	@file	ComponentButton.h
//! @brief	ボタンコンポーネント
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#include <System/Scene.h>
#include <System/Component/Component.h>

USING_PTR(ComponentButton);

class ComponentButton : public Component
{
public:
    //被さったときに表示するステータス
    enum class OverInformation
    {
        NONE       = 0,
        LEFT_CLICK = 1,
    };

public:
    BP_COMPONENT_DECL(ComponentButton, u8"ボタンコンポーネント");
    //! @{
    //	初期化処理
    void Init() override;
    //ImGui
    void GUI() override;

    //---------------------------------------------------------------------------
    //  クリックされているかを返す関数
    //! @return クリックされているか
    //---------------------------------------------------------------------------
    bool IsClick();

    //---------------------------------------------------------------------------
    //  マウスがボタンに触れているかを返す関数
    //! @return マウスがボタンに触れているか
    //---------------------------------------------------------------------------
    bool IsMouseOver();

    //---------------------------------------------------------------------------
    //  マウスがボタンに触れている時に表示させる情報の設定
    //! @param info [in] 表示させる情報
    //! @return 自身のポインタ
    //---------------------------------------------------------------------------
    std::shared_ptr<ComponentButton> SetOverInformation(OverInformation info);

    //---------------------------------------------------------------------------
    //  マウスをクリックした時に行う処理の設定
    //! @param info [in] クリック時の関数
    //! @return 自身のポインタ
    //---------------------------------------------------------------------------
    std::shared_ptr<ComponentButton> SetClickFunc(const std::function<void()>& click_func);

    //! @}
    //--------------------------------------------------------------------
    //! @name Cereal処理
    //--------------------------------------------------------------------
    //@{
private:
    //ボタンに触れた時の処理
    std::function<void()> mouse_over_func_ = []() {};
    //ボタンをクリックしたときの処理
    std::function<void()> click_func_ = []() {};
    //! @brief セーブ
    // @param arc アーカイバ
    // @param ver バージョン
    CEREAL_SAVELOAD(arc, ver) { arc(cereal::make_nvp("Component", cereal::base_class<Component>(this))); }
};

CEREAL_CLASS_VERSION(ComponentButton, 3);
