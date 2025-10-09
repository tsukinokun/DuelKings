//---------------------------------------------------------------------------
//!	@file	UIObject.h
//! @brief	UIの文字オブジェクト
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include <System/UIComponent/ComponentTransformUI.h>
USING_PTR(UIObject);

class UIObject : public Object
{
public:
    BP_OBJECT_DECL(UIObject, u8"UIオブジェクトの基底クラス")

    //@}
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示

    //描画位置の設定
    //! @param alignment [in] UIの配置位置を設定する
    //! @return	自分のSharedPtr
    std::shared_ptr<ComponentTransformUI> SetAlignment(ComponentTransformUI::Alignment alignment);

private:
    std::weak_ptr<ComponentTransformUI>
        ui_transform_component_;    // 文字コンポーネントへの参照、使う側が毎回GetCompoenentするのも、weak_ptrを保持しておくのもさすがに面倒なので、ここでpublicにして保持しておく
};
