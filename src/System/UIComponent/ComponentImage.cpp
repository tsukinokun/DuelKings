//---------------------------------------------------------------------------
//!	@file	ComponentImage.cpp
//! @brief	画像コンポーネント
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#include "ComponentImage.h"
#include "ComponentTransformUI.h"

//---------------------------------------------------------------------------
//! @brief	初期化関数
//---------------------------------------------------------------------------
void ComponentImage::Init()
{    // 初期化処理
    __super::Init();
}

//---------------------------------------------------------------------------
//! @brief	UI描画
//---------------------------------------------------------------------------
void ComponentImage::LateDraw()
{
    __super::LateDraw();
    auto   owner      = GetOwner();                  //オーナーを取得
    float3 adjustment = float3(0.0f, 0.0f, 0.0f);    // 調整値(Alignmentに合わせて)
    if(auto comp_transform = owner->GetComponent<ComponentTransformUI>()) {
        ComponentTransformUI::Alignment alignment = comp_transform->GetAlignment();
        float                           width     = 0;    // 幅
        float                           hight     = 0;    //高さ
        GetGraphSizeF(image_, &width, &hight);
        //配置位置(縦)
        switch(static_cast<int>(alignment) / 3) {
        case 0:
            adjustment.y = (-hight * 0.5f);
            break;    // 上寄せ
        case 1:
            adjustment.y = 0.0f;
            break;    // 中央寄せ
        case 2:
            adjustment.y = (hight * 0.5f);
            break;    // 下寄せ
        }
        //配置位置(横)
        switch(static_cast<int>(alignment) % 3) {
        case 0:
            adjustment.x = (-width * 0.5f);
            break;    // 左寄せ
        case 1:
            adjustment.x = 0.0f;
            break;    // 中央寄せ
        case 2:
            adjustment.x = (width * 0.5f);
            break;    // 右寄せ
        }
    }
    float3 pos   = float3(0.0f, 0.0f, 0.0f);
    pos          = owner->GetTranslate() + adjustment;
    float3 scale = owner->GetScaleAxisXYZ();
    //サイズは、Transformの平均
    float size = (scale.x + scale.y + scale.z) / 3.0f;    // 平均値をとる
    //角度はx軸の角度から取る
    float angle = owner->GetRotationAxisXYZ().x;    // X軸の角度を取得0
    DrawRotaGraphF(pos.x, pos.y, static_cast<double>(size), static_cast<double>(angle), image_, TRUE);
}

//---------------------------------------------------------------------------
//! @brief	ImGui
//---------------------------------------------------------------------------
void ComponentImage::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode(u8"画像コンポーネント")) {
            if(ImGui::Button(u8"削除"))
                GetOwner()->RemoveComponent(shared_from_this());

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

//---------------------------------------------------------------------------
//! @brief	文字列の設定
//---------------------------------------------------------------------------
std::shared_ptr<ComponentImage> ComponentImage::SetImage(int image)
{
    image_ = image;    // 画像ハンドルを設定
    return dynamic_pointer_cast<ComponentImage>(shared_from_this());
}

CEREAL_REGISTER_TYPE(ComponentImage)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentImage)
