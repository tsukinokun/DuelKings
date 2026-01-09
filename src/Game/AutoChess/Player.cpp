//---------------------------------------------------------------------------
//!	@file	Player.cpp
//! @brief	インゲームシーンのプレイヤークラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "Agent.h"
#include "Player.h"
#include <Game/AutoChess/Piece/Piece.h>
#include "Square.h"
#include <Game/AutoChess/Info/BoardInfo.h>
#include <Game/AutoChess/Info/ShopStandInfo.h>
#include <Game/AutoChess/Info/PieceStandInfo.h>
#include <Game/AutoChess/ChessBoard.h>
#include <Game/AutoChess/PieceStand.h>
#include <Game/AutoChess/Info/PieceInfo.h>
#include <Game/AutoChess/UIObject/UISynergy.h>
#include <Game/AutoChess/system/ImageBuffer.h>
#include <Game/AutoChess/system/UIHitManager.h>
#include <Game/AutoChess/Events/SynergyClickEvent.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool Player::Init()
{
    __super::Init();
    SetName("Player");
    AddGold(1000);    //初期所持金を100に設定
    //---------------------------------------------------------------------------------
    // 更新処理
    //---------------------------------------------------------------------------------
    {
        auto update = [this]() {
            bool has_board_changes = false;    //この呼び出しでボードに変更があったかどうかのフラグ
            //ボードをループ
            if(auto chess_board = Scene::Object::Get<ChessBoard>()) {
                auto squares_ = chess_board->GetSquarePtrArray();
                for(int file = 0; file < 4; ++file) {
                    for(int rank = 0; rank < 8; ++rank) {
                        if(auto square_ = squares_[file][rank].lock()) {
                            //変更されたかをチェック
                            if(square_->IsChanged()) {
                                //変更された場合、ピース情報を更新
                                auto      piece_wp = square_->GetPutPiece();
                                PieceInfo piece_info;
                                if(auto piece = piece_wp.lock()) {
                                    //ピースが置かれている場合、情報を更新
                                    piece_info.SetOwner(dynamic_pointer_cast<Player>(shared_from_this()));
                                    piece_info.SetTypeName(piece->GetNameDefault().data());
                                    piece_info.SetLevel(piece->GetLevel());
                                    selected_piece_info_ = &board_info_.GetSquarePieceRef(file, rank);    //選択されているピース情報を更新
                                }
                                //ない場合は空の状態に変更
                                SetBoardInfo(file, rank, piece_info);
                                //フラグをリセット
                                square_->SetChanged(false);
                                has_board_changes = true;    //変更があったことを記録
                            }
                        }
                    }
                }
            }

            //ボードに変更があった場合の処理
            if(has_board_changes) {
                synergy_system_.UpdateSynergys(board_info_);    //変更された駒があった場合はシナジー情報を更新
                UpdateSynergysUI();                             //シナジーUIを更新
            }

            //ピーススタンドにもかける
            if(auto piece_stand = Scene::Object::Get<PieceStand>()) {
                auto squares_ = piece_stand->GetSquarePtrArray();
                for(size_t index = 0; index < squares_.size(); ++index) {
                    if(auto square_ = squares_[index].lock()) {
                        //変更されたかをチェック
                        if(square_->IsChanged()) {
                            //変更された場合、ピース情報を更新
                            auto      piece_wp = square_->GetPutPiece();
                            PieceInfo piece_info;
                            if(auto piece = piece_wp.lock()) {
                                //ピースが置かれている場合、情報を更新
                                piece_info.SetOwner(dynamic_pointer_cast<Player>(shared_from_this()));
                                piece_info.SetTypeName(piece->GetNameDefault().data());
                                selected_piece_info_ = &stand_info_.GetStandPieceRef(index);    //選択されているピース情報を更新
                            }
                            //ない場合は空の状態に変更
                            stand_info_.SetPieceAt(index, piece_info);
                            //フラグをリセット
                            square_->SetChanged(false);
                        }
                    }
                }
            }
        };
        SetProc("Update", update, ProcTiming::Update, ProcPriority::NORMAL);
    }
    return true;
}

//---------------------------------------------------------------------------------
//!	OnHit時に選択を行うかを返す関数
//---------------------------------------------------------------------------------
bool Player::IsShouldSelectPiece()
{
    return should_select_piece_;
}

//---------------------------------------------------------------------------------
//!	OnHit時にドロップを行うかを返す関数
//---------------------------------------------------------------------------------
bool Player::IsShouldDropPiece()
{
    return should_drop_piece_;
}
//-----------------------------------------------------------
//! ピース制限を強制する関数(オーバーライド)
//-----------------------------------------------------------
void Player::EnforcePieceLimit()
{
    int level              = GetAgentLevel();        // エージェントのレベルを取得
    int placed_piece_count = GetPlacedPieceNum();    // 置かれているピースの数を取得
    if(placed_piece_count <= level) {
        return;    // レベル以内なら何もしない
    }
    // レベルを超えている場合、超過分のピースをスタンドに移動
    int pieces_to_remove = placed_piece_count - level;
    for(int file = 0; file < 4 && pieces_to_remove > 0; ++file) {
        for(int rank = 0; rank < 8 && pieces_to_remove > 0; ++rank) {
            PieceInfo piece = board_info_.GetPieceInfoArray()[file][rank];
            if(piece.GetTypeName() != "") {
                if(!MoveBoardPieceToStand(file, rank)) {
                    //失敗していたら、駒を強制削除
                    board_info_.RemovePiece(file, rank);
                    //インスタンスも削除
                    if(auto board = Scene::Object::Get<ChessBoard>()) {
                        auto squares_ = board->GetSquarePtrArray();
                        if(auto square = squares_[file][rank].lock()) {
                            if(auto piece_instance = square->GetPutPiece().lock()) {
                                square->RemovePiece();
                            }
                        }
                    }
                }
                --pieces_to_remove;
            }
        }
    }
}
//-----------------------------------------------------------
//! ピーススタンドとチェスボードの情報を交換する関数
//-----------------------------------------------------------
void Player::SwapPieceStandAndBoardInfo(size_t piece_stand_index, int board_file, int board_rank)
{
    auto agent = shared_from_this();
    //一時変数にピース情報を保存
    auto stand_piece = stand_info_.GetStandPieces()[piece_stand_index];
    auto board_piece = board_info_.GetPieceInfoArray()[board_file][board_rank];
    // ピーススタンドのピースをチェスボードに移動
    board_info_.AddPiece(board_file, board_rank, stand_piece);
    // チェスボードのピースをピーススタンドに移動
    stand_info_.SetPieceAt(piece_stand_index, board_piece);
    //ボードのインスタンスとスタンドのインスタンスも交換
    if(auto board = Scene::Object::Get<ChessBoard>()) {
        auto board_square_wp = board->GetSquarePtrArray()[board_file][board_rank];
        if(auto piece_stand = Scene::Object::Get<PieceStand>()) {
            auto stand_square_wp = piece_stand->GetSquarePtrArray()[piece_stand_index];
            //チェスボード側のインスタンスを取得
            if(auto board_square = board_square_wp.lock()) {
                //あれば、こちら側からスワップ
                board_square->ExchangePiece(stand_square_wp);
            }
            else if(auto stand_square = stand_square_wp.lock()) {
                //チェスボード側にインスタンスがなければ、スタンド側からスワップ
                stand_square->ExchangePiece(board_square_wp);
            }
        }
    }
}
//-----------------------------------------------------------
//! チェスボードの駒をスタンドに移動する関数
//-----------------------------------------------------------
bool Player::MoveBoardPieceToStand(int board_file, int board_rank)
{
    // チェスボードのピース情報を取得
    auto board_piece = board_info_.GetPieceInfoArray()[board_file][board_rank];
    if(board_piece.GetTypeName() == "") {
        return false;    // 駒が存在しない場合は何もしない
    }
    // ピーススタンドの空いている場所を探す
    for(size_t i = 0; i < stand_info_.GetStandPieces().size(); ++i) {
        auto stand_pieces = stand_info_.GetStandPieces();
        auto stand_piece  = stand_pieces[i];
        if(stand_piece.GetTypeName() == "") {
            // 空いている場所が見つかったら、ピース情報を移動
            stand_info_.SetPieceAt(i, board_piece);
            board_info_.RemovePiece(board_file, board_rank);
            //インスタンスも移動
            if(auto board = Scene::Object::Get<ChessBoard>()) {
                auto board_square_wp = board->GetSquarePtrArray()[board_file][board_rank];
                if(auto piece_stand = Scene::Object::Get<PieceStand>()) {
                    auto stand_square_wp = piece_stand->GetSquarePtrArray()[i];
                    //チェスボード側のインスタンスを取得
                    if(auto board_square = board_square_wp.lock()) {
                        board_square->ExchangePiece(stand_square_wp);
                    }
                }
            }
            // 成功した場合 true を返す
            return true;
        }
    }
    return false;    //空いてる場所がなかった場合は失敗
}

//-----------------------------------------------------------
//! @brief 選択されているピースを取得する関数
//-----------------------------------------------------------
std::shared_ptr<Piece> Player::GetSelectedPiece() const
{
    return selected_piece_;
}

//-----------------------------------------------------------
//! @brief 選択中のピースをクリアする関数
//-----------------------------------------------------------
bool Player::ReleaseSelectedPiece()
{
    //選択されているピースがある場合
    if(selected_piece_) {
        // 選択を解除
        selected_piece_->SetSelect(false);
        // 選択中のピースをクリア
        selected_piece_ = nullptr;
        if(selected_piece_info_) {
            *selected_piece_info_ = PieceInfo();    //選択中のピース情報もクリア
            selected_piece_info_  = nullptr;
        }
        return true;
    }
    return false;
}

//-----------------------------------------------------------
//! @brief ピース購入画面が開いているかどうかのフラグへのポインタを設定する関数
//-----------------------------------------------------------
void Player::SetIsPurchaseOpenFlag(bool* is_purchase_open)
{
    is_purchase_open_ = is_purchase_open;
}

//-----------------------------------------------------------
//! @brief ピースを選択中かを返す関数
//-----------------------------------------------------------
bool Player::IsSelectingPiece() const
{
    if(selected_piece_) {
        return true;
    }
    return false;
}

//-----------------------------------------------------------
//! @brief シナジー情報のUIを更新する関数
//-----------------------------------------------------------
void Player::UpdateSynergysUI()
{
    //シナジー情報を取得
    auto synergys = synergy_system_.GetSynergys();
    //既存のシナジーUIを全て削除
    for(auto& synergy_ui : Scene::Object::GetArray<UISynergy>()) {
        Scene::Object::Release(synergy_ui);
    }
    //TODO: シナジー情報に合わせてUIを更新する処理
    int synergy_index = 0;
    for(auto& synergy : synergys) {
        //IDを取得
        SynergyID synergy_id = synergy.GetID();
        //そのシナジーデータを取得
        auto synergy_data = synergy_system_.GetSynergyData(synergy_id);
        auto synergy_ui   = Scene::Object::Create<UISynergy>();
        synergy_ui->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);              //右上寄せに設定
        synergy_ui->SetTranslate(float3(800.0f + (synergy_index * 100.0f), 200.0f, 0.0f));    //位置を右上あたりに設定
        synergy_ui->SetSynergyImage(ImageBuffer::GetImageHandle(synergy_data->icon_path_));
        synergy_ui->SetSynergyData(synergy_data);
        auto synergy_click_func = [synergy_data, this]() { event_bus_->publish(SynergyClickEvent(synergy_data)); };
        synergy_ui->SetClickFunc(synergy_click_func);
        synergy_ui->SetScaleAxisXYZ(0.7f);
        //シナジーの数を取得
        int synergy_count = synergy.GetSynergyCount();
        synergy_ui->SetSynergyCount(synergy_count);    //シナジーの数を設定
        int next_count = 0;                            // 次のレベルまでの必要数を計算
        for(int i = 0; i < synergy_data->level_thresholds_.size(); i++) {
            //現在のシナジー数が閾値を超えていなければ、次のレベルまでの必要数を代入してループを抜ける
            if(synergy_count < synergy_data->level_thresholds_[i]) {
                next_count = synergy_data->level_thresholds_[i];
                break;
            }
        }
        synergy_ui->SetNextCount(next_count);    //次のレベルまでの必要数を設定
        synergy_index++;
    }
}

//-----------------------------------------------------------
//! @brief イベントバスのポインタを設定する関数
//-----------------------------------------------------------
void Player::SetEventBus(TsukinoEventBus::EventBus* event_bus)
{
    event_bus_ = event_bus;
}

//---------------------------------------------------------------------------------
//!	更新
//---------------------------------------------------------------------------------
void Player::Update()
{
    __super::Update();

    should_select_piece_ = false;    //選択するかをリセット
    //購入画面が開いていない場合のみ選択可能
    if(!*is_purchase_open_) {
        bool is_selecting_piece = false;    //ピース選択を行ったかどうかのフラグ
        //左クリックで選択
        if(IsMouseDown(MOUSE_INPUT_LEFT)) {
            if(UIHitManager::IsMouseHitUIFilter()) {
                return;
            }
            if(auto stand = Scene::Object::Get<PieceStand>()) {
                auto stand_squares_ = stand->GetSquarePtrArray();
                for(int i = 0; i < stand_squares_.size(); i++) {
                    if(auto square = stand_squares_[i].lock()) {
                        if(auto piece = square->GetPutPiece().lock()) {
                            piece->SetSelect(false);
                            if(square->IsRayHit()) {
                                selected_piece_       = piece;
                                PieceInfo* piece_info = &stand_info_.GetStandPieceRef(i);
                                selected_piece_info_  = piece_info;
                                is_selecting_piece    = true;
                            }
                        }
                    }
                }
            }
            if(auto board = Scene::Object::Get<ChessBoard>()) {
                auto board_squares_ = board->GetSquarePtrArray();
                for(int f = 0; f < 4; f++) {
                    for(int r = 0; r < board_squares_[f].size(); r++) {
                        if(auto square = board_squares_[f][r].lock()) {
                            if(auto piece = square->GetPutPiece().lock()) {
                                piece->SetSelect(false);
                                if(square->IsRayHit()) {
                                    selected_piece_       = piece;
                                    PieceInfo* piece_info = &board_info_.GetSquarePieceRef(f, r);
                                    selected_piece_info_  = piece_info;
                                    is_selecting_piece    = true;
                                }
                            }
                        }
                    }
                }
            }
            should_select_piece_ = true;
            //ピースを選択していなければ、選択中のピースをクリア
            if(!is_selecting_piece) {
                if(selected_piece_) {
                    // 選択を解除
                    selected_piece_->SetSelect(false);
                    // 選択中のピースをクリア
                    selected_piece_ = nullptr;
                    if(selected_piece_info_) {
                        selected_piece_info_ = nullptr;
                    }
                }
            }
        }

        //ドロップ操作
        should_drop_piece_ = false;    //ドロップするかをリセット
        if(IsMouseUp(MOUSE_INPUT_LEFT)) {
            if(auto stand = Scene::Object::Get<PieceStand>()) {
                auto stand_squares_ = stand->GetSquarePtrArray();
                for(int i = 0; i < stand_squares_.size(); i++) {
                    if(auto square = stand_squares_[i].lock()) {
                        if(auto piece = square->GetPutPiece().lock()) {
                            if(piece->IsSelect()) {
                                should_drop_piece_ = true;    //ドロップする
                            }
                        }
                    }
                }
            }
            if(auto board = Scene::Object::Get<ChessBoard>()) {
                auto board_squares_ = board->GetSquarePtrArray();
                for(int f = 0; f < 4; f++) {
                    for(int r = 0; r < board_squares_[f].size(); r++) {
                        if(auto square = board_squares_[f][r].lock()) {
                            if(auto piece = square->GetPutPiece().lock()) {
                                should_drop_piece_ = true;    //ドロップする
                            }
                        }
                    }
                }
            }
        }
    }
}
