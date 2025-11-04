//---------------------------------------------------------------------------
//!	@file	UIObject.cpp
//! @brief	UIオブジェクトのベースクラス
//! @author	山﨑愛
//---------------------------------------------------------------------------
#include "UIObject.h"
#include <System/UIComponent/ComponentTransformUI.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool UIObject::Init()
{
    __super::Init();
    //---------------------------------------------------------------------------------
    //	UI基本機能コンポーネントの追加
    //---------------------------------------------------------------------------------
    auto ui_transform_comp  = AddComponent<ComponentTransformUI>();    // 文字列機能コンポーネントを追加
    ui_transform_component_ = ui_transform_comp;                       // weak_ptrとして保持
    return true;
}

//---------------------------------------------------------------------------------
//!	描画位置の設定
//---------------------------------------------------------------------------------
std::shared_ptr<UIObject> UIObject::SetAlignment(ComponentTransformUI::Alignment alignment)
{
    if(auto ui_transform_comp = ui_transform_component_.lock()) {
        ui_transform_comp->SetAlignment(alignment);    // UIの配置位置を設定
        return std::dynamic_pointer_cast<UIObject>(shared_from_this());
    }
    return nullptr;    // 失敗した場合はnullptrを返す
}

//------------------------------------------------------
//! あるオブジェクトが破棄されたら、自身も破棄する処理を登録する
//------------------------------------------------------
std::shared_ptr<UIObject> UIObject::SetAutoReleaseTarget(const std::weak_ptr<Object>& target_obj)
{
    //自動解放の処理を登録(ラムダ式)
    auto release_proc = [target_obj, this]() {
        // 監視対象オブジェクトが消滅しているか確認
        if(target_obj.expired()) {
            // 自身を消去
            Scene::Object::Release(shared_from_this());
        }
    };
    SetProc("auto_release_p@roc", release_proc, ProcTiming::Update, ProcPriority::HIGH);
    return std::dynamic_pointer_cast<UIObject>(shared_from_this());
}
