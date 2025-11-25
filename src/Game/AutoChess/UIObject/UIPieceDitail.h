//---------------------------------------------------------------------------
//!	@file	UIPieceDitail.h
//! @brief	UIの駒情報オブジェクト
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "UIObject.h"
class ComponentText;    //ポインタとして使用するための前方宣言
USING_PTR(UIPieceDitail);

class UIPieceDitail : public UIObject
{
public:
    BP_OBJECT_DECL(UIPieceDitail, u8"UIの駒情報オブジェクト")

    //@}
    bool Init() override;    //!< 初期化

    //フォントの変更
    //! @param	font_name [in] 使用したいフォントの論理名
    //! @retval 自身のポインタ
    std::shared_ptr<UIPieceDitail> SetFontName(const std::string& font_name);

    //文字列の設定
    //! @param str [in] 描画する文字列の設定
    std::shared_ptr<UIPieceDitail> SetText(const std::string_view& str);

    //文字色の設定
    //! @param		text_color [in] 描画する文字色の設定
    //! @param		edge_color [in] 描画する文字のふち色の設定
    //! @details	第二引数はデフォルト引数で黒に設定されています。
    std::shared_ptr<UIPieceDitail> SetColor(int text_color, int edge_color = 0);

    //フォントサイズの設定
    //! @param font_size [in] フォントサイズの設定
    //! @retval 自身のポインタ
    std::shared_ptr<UIPieceDitail> SetFontSize(int font_size);

    //エッジサイズの設定
    //! @param	edge_size [in] エッジサイズ
    //! @retval 自身のポインタ
    std::shared_ptr<UIPieceDitail> SetEdgeSize(int edge_size);

private:
    std::weak_ptr<ComponentText>
        text_component_;    // 文字コンポーネントへの参照、使う側が毎回GetCompoenentするのも、weak_ptrを保持しておくのもさすがに面倒なので、ここでpublicにして保持しておく
};
