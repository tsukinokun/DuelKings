//---------------------------------------------------------------------------
//!	@file	Piece.cpp
//! @brief	インゲームシーンのピースクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "Piece.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
#include <Game/AutoChess/UIObject/UIImage.h>
#include <Game/AutoChess/system/ImageBuffer.h>
#include <Game/AutoChess/system/HlslppUseful.h>
#include <System/Component/ComponentEffect.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool Piece::Init()
{
    __super::Init();
    SetTranslate(float3(0.0f, 0.5f, 0.0f));    // 初期位置を設定
    SetName("Piece");
    //---------------------------------------------------------------------------------
    // ピースのステータス情報を設定(Pieceの基底クラスなので、とりあえず参照しても大丈夫なように値を入れて置きます。)
    //---------------------------------------------------------------------------------
    status_ = PieceStatus::Create().HP(100).AttackPower(5).AttackRange(1.0f).MoveSpeed(1.0f).Build();

    //---------------------------------------------------------------------------------
    // レベル表示画像の追加
    //---------------------------------------------------------------------------------
    {
        auto level_image = Scene::Object::Create<UIImage>();    //レベル表示用の画像オブジェクトを生成
        level_image->SetName("PieceLevelImage");
        level_image->SetImage(ImageBuffer::GetImageHandle("level1_star"));                          //レベル1の画像を設定
        level_image->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);                   //中央に表示
        level_image->SetScaleAxisXYZ(0.1f);                                                         //画像を小さくする
        std::weak_ptr<Piece> weak_piece  = std::dynamic_pointer_cast<Piece>(shared_from_this());    // 自分の弱参照を取得
        auto                 update_proc = [level_image, this, weak_piece]() {
            //---------------------------------------------------------------------------------
            // ピースが生きているかを確認
            //---------------------------------------------------------------------------------
            auto piece = weak_piece.lock();
            if(!piece) {
                Scene::Object::Release(level_image);    // ピースがもうないなら画像も消す
                return;                                 // ここで終了
            }
            else if(!piece->GetStatus(Object::StatusBit::Alive)) {
                Scene::Object::Release(level_image);    // ピースがもうないなら画像も消す
                return;                                 // ここで終了
            }
            //---------------------------------------------------------------------------------
            // 描画状態を親に合わせる
            //---------------------------------------------------------------------------------
            level_image->SetStatus(Object::StatusBit::NoDraw, GetStatus(Object::StatusBit::NoDraw));

            //---------------------------------------------------------------------------------
            //ピースのワールド空間スクリーン空間に変換したい
            //---------------------------------------------------------------------------------
            float2 pixel_position = WorldPositionToScreenPosition(GetTranslate());
            level_image->SetTranslate(float3(pixel_position.xy, 0.0f));
        };
        level_image->SetProc("update", update_proc);
        level_ui_ = level_image;    // レベル表示画像をメンバ変数に保存
    }
    //---------------------------------------------------------------------------------
    // 死亡したならピースをリリースする
    //---------------------------------------------------------------------------------
    auto death_proc = [this]() {
        if(status_.IsDead()) {
            // 死亡エフェクトを再生
            const std::string path = "data/AutoChess/Effect/PieceDeath.efkefc";
            //const std::string path   = "data/AutoChess/Effect/Poison/Poison.efkefc";
            float3 pos    = GetTranslate();
            auto   effect = ComponentEffect::Object::Create(path, pos);
            effect->SetTranslate(pos);
            Scene::Object::Release(dynamic_pointer_cast<Piece>(shared_from_this()));    // ピースをリリースする
        }
    };
    SetProc("death_check", death_proc, ProcTiming::Update, ProcPriority::LOW);
    return true;
}

//---------------------------------------------------------------------------------
//!	選択状態の設定
//---------------------------------------------------------------------------------
void Piece::SetSelect(bool is_selected)
{
    is_selected_ = is_selected;
}

//---------------------------------------------------------------------------------
//!	選択状態の取得
//---------------------------------------------------------------------------------
bool Piece::IsSelect()
{
    return is_selected_;
}
//----------------------------------------------------------
//! 所有者エージェントの設定
//----------------------------------------------------------
void Piece::SetOwner(const std::shared_ptr<Agent>& owner)
{
    owner_ = owner;
}

//----------------------------------------------------------
// 所有者エージェントの取得
//! @retval 所有者エージェント
//----------------------------------------------------------
std::shared_ptr<Agent> Piece::GetOwner() const
{
    return owner_.lock();
}

//----------------------------------------------------------
//! HPの取得
//----------------------------------------------------------
int Piece::GetHP() const
{
    return status_.GetHP();
}

//----------------------------------------------------------
// 最大HPの取得
//----------------------------------------------------------
int Piece::GetMaxHP() const
{
    return status_.GetMaxHP();
}

//----------------------------------------------------------
//! 攻撃力の取得
//----------------------------------------------------------
int Piece::GetAttackPower() const
{
    return status_.GetAttackPower();
}

//----------------------------------------------------------
//! 射程距離の取得
//----------------------------------------------------------
float Piece::GetAttackRange() const
{
    return status_.GetAttackRange();
}

//----------------------------------------------------------
//! 攻撃間隔の取得
//----------------------------------------------------------
float Piece::GetAttackInterval() const
{
    return status_.GetAttackInterval();
}

//----------------------------------------------------------
//! 移動速度の取得
//----------------------------------------------------------
float Piece::GetMoveSpeed() const
{
    return status_.GetMoveSpeed();
}

//----------------------------------------------------------
// ピースのレベルの取得
//----------------------------------------------------------
int Piece::GetLevel() const
{
    return status_.GetLevel();
}

//----------------------------------------------------------
// ピースレベルを設定する関数
//! @param level [in] ピースレベル
//----------------------------------------------------------
void Piece::SetLevel(int level)
{
    status_.SetLevel(level);
    //ステータスをマスターデータから再適用
    ApplyStatsFromMaster();
    // レベル表示用UI画像の更新
    if(auto level_ui = level_ui_.lock()) {
        // レベルに応じた画像名を生成
        std::string image_name = "level" + std::to_string(status_.GetLevel()) + "_star";
        level_ui->SetImage(ImageBuffer::GetImageHandle(image_name));
    }
}

//----------------------------------------------------------
//! ダメージを受ける
//----------------------------------------------------------
void Piece::TakeDamage(int amount)
{
    status_.ApplyDamage(amount);
}

//----------------------------------------------------------
// 駒をレベルアップさせる関数
//----------------------------------------------------------
void Piece::LevelUp()
{
    status_.LevelUp();
    //ステータスをマスターデータから再適用
    ApplyStatsFromMaster();
    // レベル表示用UI画像の更新
    if(auto level_ui = level_ui_.lock()) {
        // レベルに応じた画像名を生成
        std::string image_name = "level" + std::to_string(status_.GetLevel()) + "_star";
        level_ui->SetImage(ImageBuffer::GetImageHandle(image_name));
    }
}

//----------------------------------------------------------
//! 参照するマスターデータを設定する関数
//----------------------------------------------------------
void Piece::SetMasterData(const PieceData* master)
{
    master_ = master;
}

//----------------------------------------------------------
// マスターデータを参照して、ピースステータスを更新する関数
//----------------------------------------------------------
void Piece::ApplyStatsFromMaster()
{
    int level = status_.GetLevel();
    int index = level - 1;
    status_   = PieceStatus::Create()
                  .Level(level)
                  .HP(master_->levels_[index].hp_)
                  .AttackPower(master_->levels_[index].attack_)
                  .AttackInterval(master_->attack_interval_)
                  .AttackRange(master_->attack_range_)
                  .MoveSpeed(1.0f)
                  .Build();
}
