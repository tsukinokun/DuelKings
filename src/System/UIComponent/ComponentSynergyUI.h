#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>

USING_PTR(ComponentSynergyUI);

class ComponentSynergyUI : public Component
{
public:
public:
    BP_COMPONENT_DECL(ComponentSynergyUI, u8"シナジーUIのコンポーネント");
    //! @{
    //	初期化処理
    void Init() override;

    //ImGui
    void GUI() override;

    //--------------------------------------------------------------------
    // シナジー画像の設定関数
    //! @param image_id 画像ID
    //! @retval このコンポーネントのshared_ptr
    //--------------------------------------------------------------------
    std::shared_ptr<ComponentSynergyUI> SetSynergyImage(int image_handle);

    //! @}
private:
    //--------------------------------------------------------------------
    //  画像座標の補正値を取得する関数
    //! @retval 画像座標の補正値
    //--------------------------------------------------------------------
    float3 GetAdjustment() const;

private:
    int         synergy_image_ = -1;                         //!< シナジー画像ID
    int         max_level_     = 0;                          //!< シナジー最大値
    int         level_         = 0;                          //!< シナジーレベル
    int         next_count_    = 0;                          //!< 次のレベルまでの必要数
    int         synergy_count_ = 0;                          //!< シナジーの数
    std::string font_name_     = "Book Antiqua";             //フォントの名前
    int         font_size_     = DEFAULT_FONT_SIZE;          //フォントサイズ
    int         text_color_    = GetColor(255, 255, 255);    //文字の色
    int         edge_color_    = 0;                          //文字の縁の色
    int         edge_size_     = 1;                          //ふちの幅
    //--------------------------------------------------------------------
    //! @name Cereal処理
    //--------------------------------------------------------------------
    //@{

    //! @brief セーブ
    // @param arc アーカイバ
    // @param ver バージョン
    CEREAL_SAVELOAD(arc, ver) { arc(cereal::make_nvp("Component", cereal::base_class<Component>(this))); }
};

CEREAL_CLASS_VERSION(ComponentSynergyUI, 3);
