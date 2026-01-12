//---------------------------------------------------------------------------
//!	@file	ComponentGauge.cpp
//! @brief	画像コンポーネント
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#include "ComponentGauge.h"
#include "ComponentTransformUI.h"

//---------------------------------------------------------------------------
//! @brief	初期化関数
//---------------------------------------------------------------------------
void ComponentGauge::Init()
{    // 初期化処理
    __super::Init();
    //---------------------------------------------------------------------------
    //  UI描画を登録
    //---------------------------------------------------------------------------
    auto draw_ui = [this]() {
        //---------------------------------------------------------------------------
        // 線形補間でsmooth_rate_の値を更新
        //-----------------------------------------------------------------------
        elapsed_time_ += GetDeltaTime();               // 経過時間を更新
        float t        = elapsed_time_ / duration_;    // 経過時間を割合に変換
        if(t > 1.0f)
            t = 1.0f;                                                                         // 最大値を1.0fに制限
        smooth_rate_ = hlslpp::lerp(float1(duration_start_rate_), float1(gauge_rate_), t);    // 線形補間で割合を更新

        auto   owner      = GetOwner();         //オーナーを取得
        float3 adjustment = GetAdjustment();    // 調整値
        float3 pos        = float3(0.0f, 0.0f, 0.0f);
        pos               = owner->GetTranslate() + adjustment;
        float3 scale      = owner->GetScaleAxisXYZ();
        //---------------------------------------------------------------------------
        //	ゲージの描画
        //---------------------------------------------------------------------------
        // 滑らかに変化する部分
        {
            int x1 = static_cast<int>(pos.x - (gauge_size_.x * 0.5f));
            int y1 = static_cast<int>(pos.y - (gauge_size_.y * 0.5f));
            int x2 = static_cast<int>(pos.x - (gauge_size_.x * 0.5f) + (gauge_size_.x * smooth_rate_));    //右端は割合で変化
            int y2 = static_cast<int>(pos.y + (gauge_size_.y * 0.5f));
            DxLib::DrawFillBox(x1, y1, x2, y2, smooth_color_);
        }
        // ゲージ本体
        {
            int x1 = static_cast<int>(pos.x - (gauge_size_.x * 0.5f));
            int y1 = static_cast<int>(pos.y - (gauge_size_.y * 0.5f));
            int x2 = static_cast<int>(pos.x - (gauge_size_.x * 0.5f) + (gauge_size_.x * gauge_rate_));    //右端は割合で変化
            int y2 = static_cast<int>(pos.y + (gauge_size_.y * 0.5f));
            DxLib::DrawFillBox(x1, y1, x2, y2, color_);
        }
        // 枠
        {
            int x1 = static_cast<int>(pos.x - (gauge_size_.x * 0.5f));
            int y1 = static_cast<int>(pos.y - (gauge_size_.y * 0.5f));
            int x2 = static_cast<int>(pos.x + (gauge_size_.x * 0.5f));
            int y2 = static_cast<int>(pos.y + (gauge_size_.y * 0.5f));
            DxLib::DrawLineBox(x1, y1, x2, y2, GetColor(0, 0, 0));
        }
    };
    SetProc("UIDraw", draw_ui, ProcTiming::UI, static_cast<ProcPriority>(NORMAL));
}

//---------------------------------------------------------------------------
//! @brief	ImGui
//---------------------------------------------------------------------------
void ComponentGauge::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode(u8"ゲージコンポーネント")) {
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
void ComponentGauge::Construct(ObjectPtr owner)
{
    __super::Construct(owner);
    Init();
}

//---------------------------------------------------------------------------
//! @brief ゲージの割合を設定する関数
//---------------------------------------------------------------------------
std::shared_ptr<ComponentGauge> ComponentGauge::SetGaugeSize(const int2& size)
{
    gauge_size_ = size;    // ゲージのサイズを設定
    return dynamic_pointer_cast<ComponentGauge>(shared_from_this());
}

//---------------------------------------------------------------------------
//! @brief 滑らかに変化する時間を設定する関数
//---------------------------------------------------------------------------
std::shared_ptr<ComponentGauge> ComponentGauge::SetDuration(float duration)
{
    duration_ = duration;    // 滑らかに変化する時間を設定
    return dynamic_pointer_cast<ComponentGauge>(shared_from_this());
}

//---------------------------------------------------------------------------
//! @brief ゲージの割合を設定する関数
//---------------------------------------------------------------------------
std::shared_ptr<ComponentGauge> ComponentGauge::SetGaugeRate(float rate)
{
    gauge_rate_ = rate;    // ゲージの割合を設定
    //割合は0.0f~1.0fの範囲内に収める
    gauge_rate_ = std::clamp(gauge_rate_, 0.0f, 1.0f);
    //セットしたタイミングで滑らかに変化するために必要な処理をここに書く
    duration_start_rate_ = smooth_rate_;    // 滑らかに変化する開始時の割合を現在の割合に設定
    elapsed_time_        = 0.0f;            // 経過時間をリセット
    return dynamic_pointer_cast<ComponentGauge>(shared_from_this());
}

//---------------------------------------------------------------------------
//! @brief ゲージの色を設定する関数
//---------------------------------------------------------------------------
std::shared_ptr<ComponentGauge> ComponentGauge::SetGaugeColor(int color, int smooth_color)
{
    color_        = color;    // ゲージの色を設定
    smooth_color_ = smooth_color;
    return dynamic_pointer_cast<ComponentGauge>(shared_from_this());
}

//---------------------------------------------------------------------------
//! @brief  ゲージ座標の補正値を取得する関数
//---------------------------------------------------------------------------
float3 ComponentGauge::GetAdjustment() const
{
    auto   owner      = GetOwner();                  //オーナーを取得
    float3 adjustment = float3(0.0f, 0.0f, 0.0f);    // 調整値(Alignmentに合わせて)
    if(auto comp_transform = owner->GetComponent<ComponentTransformUI>()) {
        ComponentTransformUI::Alignment alignment = comp_transform->GetAlignment();
        //配置位置(縦)
        switch(static_cast<int>(alignment) / 3) {
        case 0:
            adjustment.y = (-gauge_size_.y * 0.5f);
            break;    // 上寄せ
        case 1:
            adjustment.y = 0.0f;
            break;    // 中央寄せ
        case 2:
            adjustment.y = (gauge_size_.y * 0.5f);
            break;    // 下寄せ
        }
        //配置位置(横)
        switch(static_cast<int>(alignment) % 3) {
        case 0:
            adjustment.x = (-gauge_size_.x * 0.5f);
            break;    // 左寄せ
        case 1:
            adjustment.x = 0.0f;
            break;    // 中央寄せ
        case 2:
            adjustment.x = (gauge_size_.x * 0.5f);
            break;    // 右寄せ
        }
    }
    return adjustment;
}

CEREAL_REGISTER_TYPE(ComponentGauge)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentGauge)
