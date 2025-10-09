#pragma once
//---------------------------------------------------------------------------
//!	@file	UIKeyBoard.h
//! @brief	UIのUIの画像オブジェクト
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "UIObject.h"
class ComponentKeyBoardUI;    //ポインタとして使用するための前方宣言
USING_PTR(UIKeyBoard);

class UIKeyBoard : public UIObject
{
public:
    BP_OBJECT_DECL(UIKeyBoard, u8"UIのキーボードオブジェクト")

    //@}
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示

    //サイズの設定
    //! @param size [in] サイズの設定
    //! @retval 自身のポインタ
    std::shared_ptr<UIKeyBoard> SetSize(const float2& size);

    //キーボードのテキスト設定
    //! @param text [in]キーボードのテキスト
    //! @retval 自身のポインタ
    std::shared_ptr<UIKeyBoard> SetText(const std::string& text);

    //フレームのずらしを設定
    //! @param offset [in] フレームのずらし
    //! @retval 自身のポインタ
    std::shared_ptr<UIKeyBoard> SetFrameOffset(int offset);

private:
    std::weak_ptr<ComponentKeyBoardUI>
        keyboard_component_;    // 文字コンポーネントへの参照、使う側が毎回GetCompoenentするのも、weak_ptrを保持しておくのもさすがに面倒なので、ここでpublicにして保持しておく
};
