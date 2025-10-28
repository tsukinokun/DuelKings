//---------------------------------------------------------------------------
//!	@file	Square.cpp
//! @brief	インゲームシーンのマス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "Square.h"
#include <Game/AutoChess/Piece/Piece.h>
#include "Agent.h"
#include "Player.h"
#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentCollisionModel.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool Square::Init()
{
    __super::Init();
    SetName("Square");
    SetScaleAxisXYZ(float3(0.05f, 0.05f, 0.05f));
    AddComponent<ComponentModel>("data/AutoChess/Square.mv1");
    AddComponent<ComponentCollisionModel>()->AttachToModel();
    return true;
}

//---------------------------------------------------------------------------------
//!	更新
//---------------------------------------------------------------------------------
void Square::Update()
{
    __super::Update();
    is_changed_ = false;
    is_ray_hit_ = false;
}

//---------------------------------------------------------------------------------
//!	当たり時処理
//---------------------------------------------------------------------------------
void Square::OnHit(const ComponentCollision::HitInfo& hitInfo)
{
    __super::OnHit(hitInfo);
    auto hit_owner_ = hitInfo.hit_collision_->GetOwner();
    if(hit_owner_->GetNameDefault() == "MouseRay") {
        is_ray_hit_ = true;
        if(auto player = Scene::Object::Get<Player>()) {
            //選択
            if(player->IsShouldSelectPiece()) {
                if(auto piece = piece_.lock()) {
                    piece->SetSelect(true);
                }
            }
            //ドロップ
            if(player->IsShouldDropPiece()) {
                for(auto square : Scene::Object::GetArray<Square>()) {
                    //ピースを取得
                    if(auto piece = square->GetPutPiece().lock()) {
                        //選択中なら
                        if(piece->IsSelect()) {
                            float3 pos = GetTranslate();    //現在のポジションを取得
                            pos.y      = 0.5f;              //y座標を0.5にする
                            piece->SetTranslate(pos);       //ピースの位置を設定
                            //ピースのポインタを取得
                            if(auto tmp = piece_.lock()) {
                                float3 other_pos = square->GetTranslate();    //置くピースの位置を取得
                                tmp->SetTranslate(float3(
                                    other_pos.x,
                                    0.5f,
                                    other_pos
                                        .z));    //置くピースの位置を初期化                                                                                              //ピースを交換
                                piece_ = piece;    //置くピースに設定
                                square->SetPutPiece(tmp);
                            }
                            else {
                                piece_ = piece;    //置くピースに設定
                                square->ResetPutPiece();
                            }
                            //お互いに変更されたことを設定
                            SetChanged();
                            square->SetChanged();    //選択を解除
                        }
                    }
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------
//!	マウスから出る光線にあたっているかを返す
//---------------------------------------------------------------------------------
bool Square::IsRayHit()
{
    return is_ray_hit_;
}

//---------------------------------------------------------------------------------
//!	オーナーを設定
//---------------------------------------------------------------------------------
void Square::SetOwner(std::weak_ptr<Object> owner)
{
    owner_ = owner;
}

//---------------------------------------------------------------------------------
//!	ピースを置く
//---------------------------------------------------------------------------------
void Square::SetPutPiece(std::weak_ptr<Piece> piece)
{
    piece_ = piece;
}

//---------------------------------------------------------------------------------
//!	置いてあるピースのポインタを取得
//---------------------------------------------------------------------------------
std::weak_ptr<Piece> Square::GetPutPiece()
{
    return piece_;
}
//----------------------------------------------------------
// 別のマスとピースを交換する関数
//! @param piece [in,out] 移動させるマスのウィークポインタ
//----------------------------------------------------------
void Square::ExchangePiece(std::weak_ptr<Square> other_square_wp)
{
    //移動させるマスのピースを取得
    if(auto other_square = other_square_wp.lock()) {
        //場所を適切な位置に移動
        if(auto piece = piece_.lock()) {
            float3 other_transform = other_square->GetTranslate();
            piece->SetTranslate(float3(other_transform.x, 0.5f, other_transform.z));
        }
        auto temp_piece = other_square->GetPutPiece();    //移動させるマスのピースを一時保存
        other_square->SetPutPiece(piece_);                //移動させるマスに自分のピースを設定
        piece_ = temp_piece;                              //自分に移動させるマスのピースを設定
        //とってきたピースも適切な位置へ変更
        if(auto piece = piece_.lock()) {
            float3 transform = GetTranslate();
            piece->SetTranslate(float3(transform.x, 0.5f, transform.z));
        }
        //お互いに変更されたことを設定
        SetChanged();
        other_square->SetChanged();
    }
}

//----------------------------------------------------------
// ピースを削除する関数
//----------------------------------------------------------
void Square::RemovePiece()
{
    //ピースが存在するなら
    if(auto piece = piece_.lock()) {
        piece->SetSelect(false);    //選択を解除
        piece->Exit();              //ピースを終了
        piece_.reset();             //ピースのポインタをリセット
        SetChanged();               //状態が変化したことを記録
    }
}
