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
        level_image->SetImage(ImageBuffer::GetImageHandle("level1_star"));           //レベル1の画像を設定
        level_image->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);    //中央に表示
        level_image->SetScaleAxisXYZ(0.3f);                                          //画像を小さくする
        auto update_proc = [level_image, this]() {
            //ピースのワールド空間スクリーン空間に変換したい
            if(auto camera = Scene::GetCurrentCamera().lock()) {
                float3 world_position   = GetTranslate();
                matrix view_matrix      = camera->GetViewMatrix();          //ビュー行列
                matrix proj_matrix      = camera->GetProjectionMatrix();    //投影行列
                matrix view_proj_matrix = mul(view_matrix, proj_matrix);
                float4 screen_position  = mul(float4(world_position, 1.0f), view_proj_matrix);
                screen_position.xyz     = screen_position.xyz / screen_position.w;

                // スクリーン座標(-1～+1)→UV座標(0～1)
                float2 uv = screen_position.xy * float2(0.5f, -0.5f) + 0.5f;

                float2 pixel_position = uv * float2(WINDOW_W, WINDOW_H);

                level_image->SetTranslate(float3(pixel_position.xy, 0.0f));
            }
        };
        level_image->SetProc("update", update_proc);
    }
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
//! ダメージを受ける
//----------------------------------------------------------
void Piece::TakeDamage(int amount)
{
    status_.ApplyDamage(amount);
}
