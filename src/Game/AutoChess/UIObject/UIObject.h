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
    std::shared_ptr<UIObject> SetAlignment(ComponentTransformUI::Alignment alignment);

    //------------------------------------------------------
    // あるオブジェクトが破棄されたら、自身も破棄する処理を登録する
    //! @param target_obj 破棄を監視するオブジェクト
    //! @return 自分のSharedPtr
    //------------------------------------------------------
    std::shared_ptr<UIObject> SetAutoReleaseTarget(const std::weak_ptr<Object>& target_obj);

protected:
    std::weak_ptr<ComponentTransformUI> ui_transform_component_;     //UIのTransformコンポーネント
    std::weak_ptr<Object>               target_obj_;                 //破棄を監視するオブジェクト
    bool                                is_auto_release_ = false;    //自動解放が有効かどうか
};
