//---------------------------------------------------------------------------
//!	@file	UIButton.h
//! @brief	UIのボタンオブジェクト
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "UIObject.h"
//ポインタとして使用するための前方宣言
class ComponentImage;
class ComponentButton;
USING_PTR(UIButton);

class UIButton : public UIObject
{
public:
    BP_OBJECT_DECL(UIButton, u8"UIのボタンオブジェクト")

    //@}
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示

    //画像の設定
    //! @param image [in] 描画する画像ハンドル
    //! @retval 自身のポインタ
    std::shared_ptr<UIButton> SetImage(int image);

    // ボタンがクリックされたかを返す関数
    //! @retval クリックされたか
    bool IsClick() const;

private:
    std::weak_ptr<ComponentImage>
        image_component_;    // 文字コンポーネントへの参照、使う側が毎回GetCompoenentするのも、weak_ptrを保持しておくのもさすがに面倒なので、ここでpublicにして保持しておく
    std::weak_ptr<ComponentButton>
        button_component_;    // ボタンコンポーネントへの参照、使う側が毎回GetCompoenentするのも、weak_ptrを保持しておくのもさすがに面倒なので、ここでpublicにして保持しておく
};
