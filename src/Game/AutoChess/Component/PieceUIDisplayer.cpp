//---------------------------------------------------------------------------
//!	@file	PieceUIDisplayer.cpp
//! @brief	ピースのHP表示機能クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/PieceUIDisplayer.h>
#include <Game/AutoChess/Piece/Piece.h>
#include <Game/AutoChess/UIObject/UIGauge.h>
#include <Game/AutoChess/system/HlslppUseful.h>
#include <Game/AutoChess/Component/SkillComponent/ComponentActiveSkill.h>
//---------------------------------------------------------
//! @brief 初期化
//---------------------------------------------------------
void PieceUIDisplayer::Init()
{
    __super::Init();
    auto owner = dynamic_pointer_cast<Piece>(GetOwnerPtr());
    //---------------------------------------------------------------------------------
    // ピースにHPゲージの描画を追加
    //---------------------------------------------------------------------------------
    auto hp_proc = [owner, this]() {
        // HPゲージの更新
        UpdateGauge(owner, "hp_ui", owner->GetHP(), owner->GetMaxHP(), hp_bar_color, int2(50, 10));
        // MPゲージの更新
        if(auto component_active_skill = owner->GetComponent<ComponentActiveSkill>()) {
            UpdateGauge(owner, "mp_ui", component_active_skill->GetMP(), 100, GetColor(0, 128, 128), int2(50, 10), float2(0.0f, -15.0f));
        }
    };
    SetProc("hp_proc", hp_proc, ProcTiming::Update, ProcPriority::NONE);
}

//---------------------------------------------------------
//! @brief HPバーの色を設定する関数
//---------------------------------------------------------
std::shared_ptr<PieceUIDisplayer> PieceUIDisplayer::SetHPBarColor(int color)
{
    hp_bar_color = color;
    return std::dynamic_pointer_cast<PieceUIDisplayer>(shared_from_this());
}

//---------------------------------------------------------------------------
//! @brief スクリーン座標を取得し、指定オフセットを加算する
//---------------------------------------------------------------------------
float2 PieceUIDisplayer::GetScreenPositionWithOffset(const Piece* owner, const float2& offset)
{
    float2 screen_pos = WorldPositionToScreenPosition(owner->GetTranslate());
    return screen_pos + offset;
}

//---------------------------------------------------------------------------
//! @brief ゲージオブジェクトを生成する
//---------------------------------------------------------------------------
std::shared_ptr<UIGauge> PieceUIDisplayer::CreateGauge(const std::string& name, const std::shared_ptr<Piece>& owner, int color, const int2& size)
{
    auto gauge = Scene::Object::Create<UIGauge>();
    gauge->SetGaugeColor(color);
    gauge->SetGaugeSize(size);
    gauge->SetName(name);

    std::weak_ptr<Piece> owner_wp = owner;
    gauge->SetAutoReleaseTarget(owner_wp.lock());

    return gauge;
}

//---------------------------------------------------------------------------
//! @brief 任意のゲージ（HP/MP/スタミナなど）を更新する
//---------------------------------------------------------------------------
void PieceUIDisplayer::UpdateGauge(const std::shared_ptr<Piece>& owner,
                                   const std::string&            gauge_name_suffix,
                                   int                           current_value,
                                   int                           max_value,
                                   int                           gauge_color,
                                   const int2&                   gauge_size,
                                   const float2&                 offset)
{
    // ゲージのUIオブジェクト名を作成
    std::string ui_name = std::string(owner->GetName()) + gauge_name_suffix;
    // 現在値が最大値未満の場合、ゲージを表示・更新
    if(current_value < max_value) {
        // ゲージオブジェクトを取得、存在しない場合は新規作成
        auto ui_gauge = Scene::Object::Get<UIGauge>(ui_name);
        if(!ui_gauge) {
            ui_gauge = CreateGauge(ui_name, owner, gauge_color, gauge_size);
        }
        // ゲージの位置を更新
        float2 screen_pos = GetScreenPositionWithOffset(owner.get(), offset);
        ui_gauge->SetTranslate(float3(screen_pos.x, screen_pos.y, 0.0f));
        // ゲージの割合を更新
        float rate = static_cast<float>(current_value) / static_cast<float>(max_value);
        ui_gauge->SetGaugeRate(rate);
    }
    else {
        // 現在値が最大値以上の場合、ゲージを非表示にする
        if(auto ui_gauge = Scene::Object::Get<UIGauge>(ui_name)) {
            Scene::Object::Release(ui_gauge);
        }
    }
}
