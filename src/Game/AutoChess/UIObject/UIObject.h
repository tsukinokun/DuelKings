//---------------------------------------------------------------------------
//!	@file	UIObject.h
//! @brief	UIオブジェクトの基底クラス
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
    bool Init() override;    //!< 初期化

    //------------------------------------------------------
    //描画位置の設定
    //! @param alignment [in] UIの配置位置を設定する
    //! @return	自分のSharedPtr
    //------------------------------------------------------
    std::shared_ptr<ComponentTransformUI> SetAlignment(ComponentTransformUI::Alignment alignment);

protected:
    std::weak_ptr<ComponentTransformUI> ui_transform_component_;    //UIのTransformコンポーネント
};
