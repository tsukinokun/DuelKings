//---------------------------------------------------------------------------
//!	@file	ComponentImage.cpp
//! @brief	画像コンポーネント
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#include "ComponentImage.h"
#include "ComponentTransformUI.h"
#include <Game/AutoChess/system/HlslppUseful.h>
#include <Game/AutoChess/system/ImageBuffer.h>
//---------------------------------------------------------------------------
//! @brief	初期化関数
//---------------------------------------------------------------------------
void ComponentImage::Init()
{    // 初期化処理
    __super::Init();
    //---------------------------------------------------------------------------
    //  UI描画を登録
    //---------------------------------------------------------------------------
    auto draw_ui = [this]() {
        auto   owner      = GetOwner();         //オーナーを取得
        float3 adjustment = GetAdjustment();    // 調整値
        float3 pos        = float3(0.0f, 0.0f, 0.0f);
        pos               = owner->GetTranslate() + adjustment;
        float3 scale      = owner->GetScaleAxisXYZ();
        //サイズは、Transformの平均
        float size = (scale.x + scale.y + scale.z) / 3.0f;    // 平均値をとる
        //角度はx軸の角度から取る
        float angle = owner->GetRotationAxisXYZ().x;    // X軸の角度を取得0
        // 透明度の設定
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);
        // 画像の描画
        DrawRotaGraphF(pos.x, pos.y, static_cast<double>(size), static_cast<double>(angle), image_, TRUE);
        //透明度を元に戻す
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
    };
    SetProc("UIDraw", draw_ui, ProcTiming::UI, static_cast<ProcPriority>(NORMAL));
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
//! @brief コンストラクト関数
//---------------------------------------------------------------------------
void ComponentImage::Construct(ObjectPtr owner)
{
    __super::Construct(owner);
    Init();
}

//---------------------------------------------------------------------------
//! @brief	画像の設定
//---------------------------------------------------------------------------
std::shared_ptr<ComponentImage> ComponentImage::SetImage(int image)
{
    image_ = image;    // 画像ハンドルを設定
    return dynamic_pointer_cast<ComponentImage>(shared_from_this());
}

//---------------------------------------------------------------------------
//! @brief  マウスがボタンに触れているかを返す関数
//---------------------------------------------------------------------------
bool ComponentImage::IsMouseOver()
{
    // とりあえずオーナーを取得
    auto owner = GetOwner();
    //マウス座標を取得
    float2 mouse_pos = GetMouseFloat2();
    //オーナー(UI)座標を取得
    float3 translate = owner->GetTranslate() + GetAdjustment();
    float2 ui_pos    = float2(translate.x, translate.y);
    //UIサイズを取得
    float2 ui_size = float2(0.0f, 0.0f);      //とりあえず宣言
    ui_size        = GetScreenImageSize();    //サイズ取得
    if(CheckBoxPointHit(ui_pos, ui_size, mouse_pos)) {
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
//画像ハンドルの取得
//---------------------------------------------------------------------------
int ComponentImage::GetImageHandle()
{
    return image_;
}
//---------------------------------------------------------------------------
//! @brief	画像のサイズを取得する関数
//---------------------------------------------------------------------------
float2 ComponentImage::GetImageSize() const
{
    float width = 0;                          // 幅
    float hight = 0;                          //高さ
    GetGraphSizeF(image_, &width, &hight);    // 画像のサイズを取得
    return float2(width, hight);              // 画像のサイズをfloat2で返す
}
//---------------------------------------------------------------------------
//! @brief	画面にうつる画像のサイズを取得する関数
//---------------------------------------------------------------------------
float2 ComponentImage::GetScreenImageSize()
{
    float width = 0;                          // 幅
    float hight = 0;                          //高さ
    GetGraphSizeF(image_, &width, &hight);    // 画像のサイズを取得
    auto   owner = GetOwner();                //オーナーを取得
    float3 scale = owner->GetScaleAxisXYZ();
    //サイズは、Transformの平均
    float size  = (scale.x + scale.y + scale.z) / 3.0f;    // 平均値をとる
    width      *= size;                                    // 幅にサイズをかける
    hight      *= size;                                    // 高さにサイズをかける
    return float2(width, hight);                           // 画像のサイズをfloat2で返す
}
//---------------------------------------------------------------------------
//! @brief  画像座標の補正値を取得する関数
//! @retval 画像座標の補正値
//---------------------------------------------------------------------------
float3 ComponentImage::GetAdjustment() const
{
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
    return adjustment;
}

//---------------------------------------------------------------------------
//! @brief  画像の透明度を設定する関数
//---------------------------------------------------------------------------
std::shared_ptr<ComponentImage> ComponentImage::SetAlpha(int alpha)
{
    alpha_ = alpha;
    return dynamic_pointer_cast<ComponentImage>(shared_from_this());
}

//---------------------------------------------------------------------------
//! @brief この関数がフィルターとして機能するかどうかを設定する関数
//---------------------------------------------------------------------------
std::shared_ptr<ComponentImage> ComponentImage::SetIsFilter(bool is_filter)
{
    is_filter_ = is_filter;
    return dynamic_pointer_cast<ComponentImage>(shared_from_this());
}

//---------------------------------------------------------------------------
//! @brief この関数がフィルターとして機能するかどうかを取得する関数
//---------------------------------------------------------------------------
bool ComponentImage::GetIsFilter() const
{
    return is_filter_;
}

CEREAL_REGISTER_TYPE(ComponentImage)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentImage)
