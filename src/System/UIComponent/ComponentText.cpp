//---------------------------------------------------------------------------
//!	@file	ComponentText.cpp
//! @brief	文字列コンポーネント
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#include "ComponentText.h"
#include "ComponentTransformUI.h"

//---------------------------------------------------------------------------
//! @brief	初期化関数
//---------------------------------------------------------------------------
void ComponentText::Init()
{    // 初期化処理
    __super::Init();
    //---------------------------------------------------------------------------
    //  UI描画を登録
    //---------------------------------------------------------------------------
    auto draw_ui = [this]() {
        DxLib::SetFontSize(font_size_);                  //フォントサイズに合わせる
        auto   owner      = GetOwner();                  //オーナーを取得
        float3 adjustment = float3(0.0f, 0.0f, 0.0f);    // 調整値(Alignmentに合わせて)
        if(auto comp_transform = owner->GetComponent<ComponentTransformUI>()) {
            ComponentTransformUI::Alignment alignment = comp_transform->GetAlignment();
            //配置位置(縦)
            float hight = static_cast<float>(GetFontSize());    //フォントサイズを取得(=高さ)
            switch(static_cast<int>(alignment) / 3) {
            case 0:
                adjustment.y = 0.0f;
                break;    // 上寄せ
            case 1:
                adjustment.y = (hight * 0.5f);
                break;    // 中央寄せ
            case 2:
                adjustment.y = hight;
                break;    // 下寄せ
            }
            //配置位置(横)
            float width = static_cast<float>(GetDrawStringWidth(str_.data(), str_.size()));    // 文字列の幅を取得
            switch(static_cast<int>(alignment) % 3) {
            case 0:
                adjustment.x = 0.0f;
                break;    // 左寄せ
            case 1:
                adjustment.x = (width * 0.5f);
                break;    // 中央寄せ
            case 2:
                adjustment.x = width;
                break;    // 右寄せ
            }
        }
        float3 pos = float3(0.0f, 0.0f, 0.0f);
        pos        = owner->GetTranslate() + adjustment;
        DrawString(pos.x, pos.y, str_.data(), text_color_, edge_color_);
        DxLib::SetFontSize(DEFAULT_FONT_SIZE);    //フォントサイズを元に戻す
    };
    SetProc("UIDraw", draw_ui, ProcTiming::UI, static_cast<ProcPriority>(NONE));
}

//---------------------------------------------------------------------------
//! @brief	ImGui
//---------------------------------------------------------------------------
void ComponentText::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode(u8"文字列コンポーネント")) {
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
std::shared_ptr<ComponentText> ComponentText::SetText(const std::string_view& str)
{
    str_ = std::string(str);    // コピーして保持
    return dynamic_pointer_cast<ComponentText>(shared_from_this());
}

//---------------------------------------------------------------------------
//! @brief	文字列色の設定
//---------------------------------------------------------------------------
std::shared_ptr<ComponentText> ComponentText::SetColor(int text_color, int edge_color)
{
    text_color_ = text_color;    // 文字色を設定
    edge_color_ = edge_color;    // 縁取り色を設定
    return dynamic_pointer_cast<ComponentText>(shared_from_this());
}

//---------------------------------------------------------------------------
//! @brief	フォントサイズの設定
//---------------------------------------------------------------------------
std::shared_ptr<ComponentText> ComponentText::SetFontSize(int font_size)
{
    font_size_ = font_size;    // フォントサイズを設定
    return dynamic_pointer_cast<ComponentText>(shared_from_this());
}

CEREAL_REGISTER_TYPE(ComponentText)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentText)
