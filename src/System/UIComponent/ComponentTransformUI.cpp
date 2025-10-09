//---------------------------------------------------------------------------
//!	@file	ComponentTransformUI.cpp
//! @brief	文字列コンポーネント
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#include "ComponentTransformUI.h"

//---------------------------------------------------------------------------
//! @brief	初期化関数
//---------------------------------------------------------------------------
void ComponentTransformUI::Init()
{    // 初期化処理
    __super::Init();
}

//---------------------------------------------------------------------------
//! @brief	UI描画
//---------------------------------------------------------------------------
void ComponentTransformUI::LateDraw()
{
    __super::LateDraw();
}

//---------------------------------------------------------------------------
//! @brief	ImGui
//---------------------------------------------------------------------------
void ComponentTransformUI::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode(u8"UIコンポーネント")) {
            // GUI上でオーナーから自分(SampleObjectController)を削除します
            if(ImGui::Button(u8"削除"))
                GetOwner()->RemoveComponent(shared_from_this());

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

//---------------------------------------------------------------------------
//! @brief	描画位置の設定
//---------------------------------------------------------------------------
std::shared_ptr<ComponentTransformUI> ComponentTransformUI::SetAlignment(Alignment alignment)
{
    alignment_ = alignment;    // 配置の設定
    return dynamic_pointer_cast<ComponentTransformUI>(shared_from_this());
}

//---------------------------------------------------------------------------
//! @brief	描画位置の取得
//---------------------------------------------------------------------------
ComponentTransformUI::Alignment ComponentTransformUI::GetAlignment() const
{
    return alignment_;    // 配置の取得
}
CEREAL_REGISTER_TYPE(ComponentTransformUI)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentTransformUI)
