//---------------------------------------------------------------------------
//!	@file	PieceStand.cpp
//! @brief	インゲームシーンのピース置き場
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "PieceStand.h"
#include <Game/AutoChess/Piece/Piece.h>
#include "Square.h"
#include <Game/AutoChess/system/GameConst.h>
#include <Game/AutoChess/Player.h>
#include <Game/AutoChess/ChessBoard.h>
#include <Game/AutoChess/UIObject/UIButton.h>
#include <Game/AutoChess/UIObject/UIGauge.h>
#include <Game/AutoChess/system/HlslppUseful.h>
#include <Game/AutoChess/system/ImageBuffer.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool PieceStand::Init()
{
    __super::Init();
    SetTranslate(float3(0.0f, 0.0f, -6.0f));
    SetName("PieceStand");
    CreateSquare();
    //---------------------------------------------------------------------------------
    //  更新処理を追加
    //---------------------------------------------------------------------------------
    auto update = [this]() {
        //ピースが変更されたかを確認
        for(int i = 0; i < STAND_SQUARE_MAX_; i++) {
            if(auto square = squares_[i].lock()) {
                if(square->IsChanged()) {
                    if(auto player = Scene::Object::Get<Player>()) {
                        PieceInfo piece_info;    //空のピース情報
                        //マスに中身があれば
                        auto piece_wp = square->GetPutPiece();
                        if(auto piece = piece_wp.lock()) {
                            piece_info.SetTypeName(piece->GetNameDefault().data());
                            piece_info.SetOwner(player);
                        }
                        player->SetPieceStandInfo(i, piece_info);
                    }
                }
            }
        }
    };
    SetProc("Update", update, ProcTiming::Update, ProcPriority::NONE);
    return true;
}

//---------------------------------------------------------------------------------
//!	描画
//---------------------------------------------------------------------------------
void PieceStand::Draw()
{
    __super::Draw();
    for(int s = 0; s < STAND_SQUARE_MAX_; s++) {
        int color = GetColor(0, 255, 0);
        //ファイルとランクの合計値が偶数なら白に
        if((s % 2) == 0) {
            color = GetColor(0, 255, 255);
        }
        float3 curr_translate = GetTranslate();    //現在のポジションを取得
        float  x              = (s * SQUARE_SIZE) - STAND_SQUARE_HALF_ * (SQUARE_SIZE);
        float3 p1             = float3(x + -SQUARE_HALF, -0.1f, -SQUARE_HALF) + curr_translate;
        float3 p2             = float3(x + SQUARE_HALF, 0.1f, SQUARE_HALF) + curr_translate;
        DrawCube3D(cast(p1), cast(p2), color, color, TRUE);
    }
}

//---------------------------------------------------------------------------------
//!	終了
//---------------------------------------------------------------------------------
void PieceStand::Exit()
{
    __super::Exit();
}

//---------------------------------------------------------------------------------
//!	GUI表示
//---------------------------------------------------------------------------------
void PieceStand::GUI()
{
    __super::GUI();
}

//---------------------------------------------------------------------------------
//!	ピースの初期化
//---------------------------------------------------------------------------------
void PieceStand::AddPiece(std::shared_ptr<Piece> piece)
{
    //バッファを確認
    for(int i = 0; i < squares_.size(); i++) {
        //ヌルポインタなら
        if(auto square = squares_[i].lock()) {
            if(square->GetPutPiece().expired()) {
                piece->SetTranslate(float3((i * SQUARE_SIZE) - STAND_SQUARE_HALF_ * (SQUARE_SIZE), 0.5f, 0.0f) + GetTranslate());    //位置を設定
                //---------------------------------------------------------------------------------
                // ピースに、レベルアップを可能にする処理を追加
                //---------------------------------------------------------------------------------
                auto level_up_proc = [piece]() {
                    int piece_num = 0;    //同じピースの数をカウントする変数、自身も探索に含まれるので0スタート
                    //---------------------------------------------------------------------------------
                    // ピーススタンドを探索したい
                    //---------------------------------------------------------------------------------
                    if(auto piece_stand = Scene::Object::Get<PieceStand>()) {
                        auto square_ptrs = piece_stand->GetSquarePtrArray();
                        for(int j = 0; j < square_ptrs.size(); j++) {
                            if(auto square = square_ptrs[j].lock()) {
                                auto put_piece_wp = square->GetPutPiece();
                                if(auto put_piece = put_piece_wp.lock()) {
                                    //同じピース名で同じレベルならカウントアップ
                                    if(put_piece->GetNameDefault() == piece->GetNameDefault()) {
                                        if(put_piece->GetLevel() == piece->GetLevel()) {
                                            piece_num++;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    //---------------------------------------------------------------------------------
                    // 次はチェスボードを探索
                    //---------------------------------------------------------------------------------
                    if(auto chess_board = Scene::Object::Get<ChessBoard>()) {
                        auto board_square_ptrs = chess_board->GetSquarePtrArray();
                        for(int f = 0; f < board_square_ptrs.size(); f++) {
                            for(int r = 0; r < board_square_ptrs[f].size(); r++) {
                                if(auto board_square = board_square_ptrs[f][r].lock()) {
                                    auto put_piece_wp = board_square->GetPutPiece();
                                    if(auto put_piece = put_piece_wp.lock()) {
                                        //アップデートをしていなかったらカウントしない
                                        if(put_piece->GetStatus(Object::StatusBit::NoUpdate)) {
                                            continue;
                                        }
                                        //同じピース名で同じレベルならカウントアップ
                                        if(put_piece->GetNameDefault() == piece->GetNameDefault()) {
                                            if(put_piece->GetLevel() == piece->GetLevel()) {
                                                piece_num++;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    //3つ以上あったらレベルアップボタンを生成
                    //マウスが触れていたら
                    std::string button_ui_name = std::string(piece->GetName()) + "level_up_name";
                    if(piece_num >= LEVEL_UP_REQUIRED_PIECE_COUNT) {
                        //左マウスクリックを促す
                        if(auto button_ui = Scene::Object::Get<UIButton>(button_ui_name)) {
                            //ピースのスクリーン座標を取得
                            float2 screen_pos = WorldPositionToScreenPosition(piece->GetTranslate());
                            //少し上にずらす
                            float2 offset  = float2(0.0f, -30.0f);
                            screen_pos    += offset;
                            button_ui->SetTranslate(float3(screen_pos.x, screen_pos.y, 0.0f));    //位置設定
                        }
                        else {
                            button_ui = Scene::Object::Create<UIButton>();    //左クリックアニメーションオブジェクトを生成
                            //ピースのスクリーン座標を取得
                            float2 screen_pos = WorldPositionToScreenPosition(piece->GetTranslate());
                            //少し上にずらす
                            float2 offset  = float2(0.0f, -30.0f);
                            screen_pos    += offset;
                            button_ui->SetTranslate(float3(screen_pos.x, screen_pos.y, 0.0f));    //位置を設定
                            button_ui->SetImage(ImageBuffer::GetImageHandle("deff"));             //アニメーションステータス設定
                            button_ui->SetName(button_ui_name);                                   //名前設定
                            //自身のポインタを格納
                            std::weak_ptr<Piece> weak_piece  = piece;    // 自分の弱参照を取得
                            auto                 update_proc = [button_ui, weak_piece]() {
                                //---------------------------------------------------------------------------------
                                // ピースが生きているかを確認
                                //---------------------------------------------------------------------------------
                                auto piece = weak_piece.lock();
                                if(!piece) {
                                    Scene::Object::Release(button_ui);    // ピースがもうないなら画像も消す
                                    return;                               // ここで終了
                                }
                                else if(!piece->GetStatus(Object::StatusBit::Alive)) {
                                    Scene::Object::Release(button_ui);    // ピースがもうないなら画像も消す
                                    return;                               // ここで終了
                                }
                                //---------------------------------------------------------------------------------
                                // 描画状態を親に合わせる
                                //---------------------------------------------------------------------------------
                                button_ui->SetStatus(Object::StatusBit::NoDraw, piece->GetStatus(Object::StatusBit::NoDraw));

                                //---------------------------------------------------------------------------------
                                //ピースのワールド空間スクリーン空間に変換したい
                                //---------------------------------------------------------------------------------
                                float2 pixel_position = WorldPositionToScreenPosition(piece->GetTranslate());
                                button_ui->SetTranslate(float3(pixel_position.xy, 0.0f));
                            };
                            button_ui->SetProc("update", update_proc, ProcTiming::Update, ProcPriority::HIGH);
                            //---------------------------------------------------------------------------------
                            // ボタンクリック時の処理を設定
                            //---------------------------------------------------------------------------------
                            auto click_func = [piece, button_ui_name, weak_piece]() {
                                //---------------------------------------------------------------------------------
                                // 同種のピースを2つ削除する処理
                                //---------------------------------------------------------------------------------
                                int pieces_to_remove = LEVEL_UP_REQUIRED_PIECE_COUNT - 1;    //削除するピースの数はレベルアップに必要な数-1
                                int removed_count    = 0;                                    //削除したピースの数
                                //ピーススタンドを探索したい
                                if(auto piece_stand = Scene::Object::Get<PieceStand>()) {
                                    //中身を見る
                                    auto square_ptrs = piece_stand->GetSquarePtrArray();
                                    for(int j = 0; j < square_ptrs.size(); j++) {
                                        if(removed_count >= pieces_to_remove) {
                                            break;
                                        }
                                        if(auto square = square_ptrs[j].lock()) {
                                            auto put_piece_wp = square->GetPutPiece();
                                            if(auto put_piece = put_piece_wp.lock()) {
                                                //同じポインタならcontinue
                                                if(put_piece == weak_piece.lock()) {
                                                    continue;
                                                }
                                                //同じピース名で同じレベルならリリースしてカウントアップ
                                                if(put_piece->GetNameDefault() == piece->GetNameDefault()) {
                                                    if(put_piece->GetLevel() == piece->GetLevel()) {
                                                        square->RemovePiece();    //マスからピースを削除
                                                        if(auto player = Scene::Object::Get<Player>()) {
                                                            player->SetPieceStandInfo(j, PieceInfo());    //ピース情報を空にする
                                                        }
                                                        removed_count++;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                                //次はチェスボードを探索
                                if(auto chess_board = Scene::Object::Get<ChessBoard>()) {
                                    auto board_square_ptrs = chess_board->GetSquarePtrArray();
                                    for(int f = 0; f < board_square_ptrs.size(); f++) {
                                        if(removed_count >= pieces_to_remove) {
                                            break;
                                        }
                                        for(int r = 0; r < board_square_ptrs[f].size(); r++) {
                                            if(removed_count >= pieces_to_remove) {
                                                break;
                                            }
                                            if(auto board_square = board_square_ptrs[f][r].lock()) {
                                                auto put_piece_wp = board_square->GetPutPiece();
                                                if(auto put_piece = put_piece_wp.lock()) {
                                                    //アップデートをしていなかったらcontinue
                                                    if(put_piece->GetStatus(Object::StatusBit::NoUpdate)) {
                                                        continue;
                                                    }
                                                    //同じポインタならcontinue
                                                    if(put_piece == weak_piece.lock()) {
                                                        continue;
                                                    }
                                                    //同じピース名ならリリースしてカウントアップ
                                                    if(put_piece->GetNameDefault() == piece->GetNameDefault()) {
                                                        if(put_piece->GetLevel() == piece->GetLevel()) {
                                                            board_square->RemovePiece();    //マスからピースを削除
                                                            if(auto player = Scene::Object::Get<Player>()) {
                                                                player->RemoveBoardInfo(f, r);    //ボード情報を空にする
                                                            }
                                                            removed_count++;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                                //レベルアップ処理
                                piece->LevelUp();
                                //---------------------------------------------------------------------------------
                                // マスを探索して、自分が置かれているマスを探し、そこに更新されたことを伝える
                                //---------------------------------------------------------------------------------
                                //ピーススタンド
                                if(auto piece_stand = Scene::Object::Get<PieceStand>()) {
                                    auto square_ptrs = piece_stand->GetSquarePtrArray();
                                    for(int j = 0; j < square_ptrs.size(); j++) {
                                        if(auto square = square_ptrs[j].lock()) {
                                            auto put_piece_wp = square->GetPutPiece();
                                            if(auto put_piece = put_piece_wp.lock()) {
                                                //同じポインタなら更新を伝える
                                                if(put_piece == weak_piece.lock()) {
                                                    square->SetChanged(true);    //更新されたことを伝える
                                                }
                                            }
                                        }
                                    }
                                }
                                //チェスボード
                                if(auto chess_board = Scene::Object::Get<ChessBoard>()) {
                                    auto board_square_ptrs = chess_board->GetSquarePtrArray();
                                    for(int f = 0; f < board_square_ptrs.size(); f++) {
                                        for(int r = 0; r < board_square_ptrs[f].size(); r++) {
                                            if(auto board_square = board_square_ptrs[f][r].lock()) {
                                                auto put_piece_wp = board_square->GetPutPiece();
                                                if(auto put_piece = put_piece_wp.lock()) {
                                                    //同じポインタなら更新を伝える
                                                    if(put_piece == weak_piece.lock()) {
                                                        board_square->SetChanged(true);    //更新されたことを伝える
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            };
                            button_ui->SetClickFunc(click_func);
                        }
                    }
                    else {
                        //非表示
                        if(auto button_ui = Scene::Object::Get<UIButton>(button_ui_name)) {
                            Scene::Object::Release(button_ui);
                        }
                    }
                };
                piece->SetProc("level_up_proc", level_up_proc, ProcTiming::Update, ProcPriority::NONE);
                square->SetPutPiece(piece);    //バッファにポインタを登録
                //一度生成したらリターンする
                return;
            }
        }
    }
}

//---------------------------------------------------------------------------------
//!	オーナーを設定
//---------------------------------------------------------------------------------
void PieceStand::SetOwner(std::weak_ptr<Object> owner)
{
    owner_ = owner;
}

//---------------------------------------------------------------------------------
//!	マスの生成
//---------------------------------------------------------------------------------
void PieceStand::CreateSquare()
{
    for(int i = 0; i < STAND_SQUARE_MAX_; i++) {
        auto square = Scene::Object::Create<Square>();
        square->SetOwner(owner_);
        square->SetTranslate(float3((i * SQUARE_SIZE) - STAND_SQUARE_HALF_ * (SQUARE_SIZE), 0.0f, 0.0f) + GetTranslate());
        squares_[i] = square;
    }
}

//---------------------------------------------------------------------------------
//!	マスのウィークポインタを取得
//---------------------------------------------------------------------------------
std::array<std::weak_ptr<Square>, 8> PieceStand::GetSquarePtrArray()
{
    return squares_;
}
//---------------------------------------------------------------------------
// ピースのスタンドが満タンかどうかを取得する関数
//! @retval 満タンの場合 true、そうでない場合 false
//---------------------------------------------------------------------------
bool PieceStand::IsFull() const
{
    for(int i = 0; i < squares_.size(); i++) {
        if(auto square = squares_[i].lock()) {
            if(square->GetPutPiece().expired()) {
                return false;    //空きマスがあったら false を返す
            }
        }
    }
    return true;    //全てのマスが埋まっていたら true を返す
}
