#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>

USING_PTR(ComponentKeyBoardUI);

class ComponentKeyBoardUI : public Component
{
public:
    BP_COMPONENT_DECL(ComponentKeyBoardUI, u8"キーボードUIコンポーネント");
    //! @{
    //	初期化処理
    void Init() override;

    //ImGui
    void GUI() override;

    //サイズの設定
    //! @param size [in] サイズの設定
    //! @retval 自身のポインタ
    std::shared_ptr<ComponentKeyBoardUI> SetSize(const float2& size);

    //キーボードのテキスト設定
    //! @param text [in]キーボードのテキスト
    //! @retval 自身のポインタ
    std::shared_ptr<ComponentKeyBoardUI> SetText(const std::string& text);

    //フレームのずらしを設定
    //! @param offset [in] フレームのずらし
    //! @retval 自身のポインタ
    std::shared_ptr<ComponentKeyBoardUI> SetFrameOffset(int offset);
    //! @}

private:
    std::string  text_             = "A";                     //キーボードの文字
    float2       size_             = float2(50.0f, 50.0f);    //キーボードUIのサイズ
    const float2 foundation_offset = float2(10.0f, 10.0f);    //キーボードUIの土台のオフセット
    int          frame_count       = 0;
    int          frame_offset      = 0;    //フレームのずらし
    //--------------------------------------------------------------------
    //! @name Cereal処理
    //--------------------------------------------------------------------
    //@{

    //! @brief セーブ
    // @param arc アーカイバ
    // @param ver バージョン
    CEREAL_SAVELOAD(arc, ver) { arc(cereal::make_nvp("Component", cereal::base_class<Component>(this))); }
};

CEREAL_CLASS_VERSION(ComponentKeyBoardUI, 3);
