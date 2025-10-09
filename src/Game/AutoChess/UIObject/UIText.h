//---------------------------------------------------------------------------
//!	@file	UIText.h
//! @brief	UIの文字オブジェクト
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "UIObject.h"
class ComponentText;    //ポインタとして使用するための前方宣言
USING_PTR(UIText);

class UIText : public UIObject
{
public:
    BP_OBJECT_DECL(UIText, u8"UIの文字オブジェクト")

    //@}
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示

    //文字列の設定
    //! @param str [in] 描画する文字列の設定
    std::shared_ptr<UIText> SetText(const std::string_view& str);

    //文字色の設定
    //! @param		text_color [in] 描画する文字色の設定
    //! @param		edge_color [in] 描画する文字のふち色の設定
    //! @details	第二引数はデフォルト引数で黒に設定されています。
    std::shared_ptr<UIText> SetColor(int text_color, int edge_color = 0);

    //フォントサイズの設定
    //! @param font_size [in] フォントサイズの設定
    //! @retval 自身のポインタ
    std::shared_ptr<UIText> SetFontSize(int font_size);

private:
    std::weak_ptr<ComponentText>
        text_component_;    // 文字コンポーネントへの参照、使う側が毎回GetCompoenentするのも、weak_ptrを保持しておくのもさすがに面倒なので、ここでpublicにして保持しておく
};
