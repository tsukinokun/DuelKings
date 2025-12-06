//---------------------------------------------------------------------------
//!	@file	PieceHPDisplayer.cpp
//! @brief	ピースのHP表示機能クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/PieceHPDisplayer.h>
#include <Game/AutoChess/Piece/Piece.h>
#include <Game/AutoChess/UIObject/UIGauge.h>
#include <Game/AutoChess/system/HlslppUseful.h>
//---------------------------------------------------------
//! @brief 初期化
//---------------------------------------------------------
void PieceHPDisplayer::Init()
{
    __super::Init();
    auto owner = dynamic_pointer_cast<Piece>(GetOwnerPtr());
    //---------------------------------------------------------------------------------
    // ピースにHPゲージの描画を追加
    //---------------------------------------------------------------------------------
    auto hp_proc = [owner, this]() {
        std::string hp_ui_name = std::string(owner->GetName()) + "hp_ui_name";
        //最大HP未満であれば描画をする。
        if(owner->GetHP() < (owner->GetMaxHP())) {
            //HP表示
            if(auto hp_ui = Scene::Object::Get<UIGauge>(hp_ui_name)) {
                //ピースのスクリーン座標を取得
                float2 screen_pos = WorldPositionToScreenPosition(owner->GetTranslate());
                //少し上にずらす
                float2 offset  = float2(0.0f, -30.0f);
                screen_pos    += offset;
                hp_ui->SetTranslate(float3(screen_pos.x, screen_pos.y, 0.0f));                              //位置設定
                float rate = static_cast<float>(owner->GetHP()) / static_cast<float>(owner->GetMaxHP());    //割合を計算
                hp_ui->SetGaugeRate(rate);
            }
            else {
                //ゲージの表示
                hp_ui = Scene::Object::Create<UIGauge>();    //HPゲージオブジェクトを生成
                hp_ui->SetGaugeColor(hp_bar_color);
                std::weak_ptr<Piece> owner_wp = owner;           //オーナーの弱参照を取得
                hp_ui->SetAutoReleaseTarget(owner_wp.lock());    //オーナーが消えたら自分も消えるように設定
                //ピースのスクリーン座標を取得
                float2 screen_pos = WorldPositionToScreenPosition(owner->GetTranslate());
                //少し上にずらす
                float2 offset  = float2(0.0f, -30.0f);
                screen_pos    += offset;
                hp_ui->SetTranslate(float3(screen_pos.x, screen_pos.y, 0.0f));    //位置を設定
                hp_ui->SetName(hp_ui_name);                                       //名前を設定
            }
        }
        else {
            if(auto button_ui = Scene::Object::Get<UIGauge>(hp_ui_name)) {
                //非表示
                Scene::Object::Release(button_ui);
            }
        }
    };
    SetProc("hp_proc", hp_proc, ProcTiming::Update, ProcPriority::NONE);
}

//---------------------------------------------------------
//! @brief HPバーの色を設定する関数
//---------------------------------------------------------
std::shared_ptr<PieceHPDisplayer> PieceHPDisplayer::SetHPBarColor(int color)
{
    hp_bar_color = color;
    return std::dynamic_pointer_cast<PieceHPDisplayer>(shared_from_this());
}
