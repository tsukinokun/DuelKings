#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>

USING_PTR(ComponentText);

class ComponentText : public Component
{
public:
    BP_COMPONENT_DECL(ComponentText, u8"文字列コンポーネント");
    //! @{
    //	初期化処理
    void Init() override;

    //ImGui
    void GUI() override;

    //フォントの変更
    //! @param	font_name [in] 使用したいフォントの論理名
    //! @retval 自身のポインタ
    std::shared_ptr<ComponentText> SetFontName(const std::string& font_name);

    //文字列の設定
    //! @param str [in] 描画する文字列の設定
    //! @retval 自身のポインタ
    std::shared_ptr<ComponentText> SetText(const std::string_view& str);

    //文字色の設定
    //! @param		text_color [in] 描画する文字色の設定
    //! @param		edge_color [in] 描画する文字のふち色の設定
    //! @details	第二引数はデフォルト引数で黒に設定されています。
    //! @retval 自身のポインタ

    std::shared_ptr<ComponentText> SetColor(int text_color, int edge_color = 0);

    //フォントサイズの設定
    //! @param font_size [in] フォントサイズの設定
    //! @retval 自身のポインタ
    std::shared_ptr<ComponentText> SetFontSize(int font_size);

    //エッジサイズの設定
    //! @param	edge_size [in] エッジサイズ
    //! @retval 自身のポインタ
    std::shared_ptr<ComponentText> SetEdgeSize(int edge_size);

    //! @}

private:
    std::string font_name_  = "百鬼夜行 R";         //フォントの名前
    int         font_size_  = DEFAULT_FONT_SIZE;    //フォントサイズ
    int         text_color_ = 0;                    //文字の色
    int         edge_color_ = 0;                    //文字の縁の色
    int         edge_size_  = 1;                    //ふちの幅
    std::string str_        = "";                   //描画する文字列

    //--------------------------------------------------------------------
    //! @name Cereal処理
    //--------------------------------------------------------------------
    //@{

    //! @brief セーブ
    // @param arc アーカイバ
    // @param ver バージョン
    CEREAL_SAVELOAD(arc, ver) { arc(cereal::make_nvp("Component", cereal::base_class<Component>(this))); }
};

CEREAL_CLASS_VERSION(ComponentText, 3);
