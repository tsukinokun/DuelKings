//---------------------------------------------------------------------------
//!	@file	ShopPieceEffectManager.cpp
//! @brief	ショップエフェクト管理クラスの定義の実装
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/Manager/ShopPieceEffectManager.h>
#include <Game/AutoChess/Player.h>
#include <Game/AutoChess/ShopStand.h>
#include <Game/AutoChess/Piece/Piece.h>
#include <system/Component/ComponentEffect.h>
//---------------------------------------------------------------------------
//! @brief 初期化
//---------------------------------------------------------------------------
void ShopPieceEffectManager::Init()
{
    __super::Init();
    //---------------------------------------------------------------------------
    // プレイヤーを取得
    //---------------------------------------------------------------------------
    auto                  player    = Scene::Object::Get<Player>();
    std::weak_ptr<Player> wp_player = player;    // プレイヤーの弱参照(オーバーヘッドを避けたいのでここで一度作る)
    //---------------------------------------------------------------------------
    // 更新処理を入れる
    //---------------------------------------------------------------------------
    auto update_proc = [this, wp_player]() {
        auto player = wp_player.lock();
        // プレイヤーがいなければ処理しない
        if(!player)
            return;
        //---------------------------------------------------------------------------
        // ショップピースエフェクトの更新処理
        //---------------------------------------------------------------------------

        // まず、プレイヤーの一枚でも所持しているピースをテーブルにする
        std::unordered_set<std::string> owned_piece_types;    // 所持しているピースの種類名のセット

        //---------------------------------------------------------------------------
        // ボードを走査
        //---------------------------------------------------------------------------
        for(int f = 0; f < 4; f++) {
            for(int r = 0; r < 8; r++) {
                PieceInfo piece_info = player->GetBoardInfo(f, r);    // ボード上のピース情報を取得
                // ピースの名前を取得して
                if(piece_info.GetTypeName() != "") {
                    // 空でなければセットに追加
                    owned_piece_types.insert(piece_info.GetTypeName());
                }
            }
        }

        //---------------------------------------------------------------------------
        // スタンドを走査
        //---------------------------------------------------------------------------
        for(int i = 0; i < 8; i++) {
            PieceInfo piece_info = player->GetPieceStandInfo(i);    // スタンド上のピース情報を取得
            // ピースの名前を取得して
            if(piece_info.GetTypeName() != "") {
                // 空でなければセットに追加
                owned_piece_types.insert(piece_info.GetTypeName());
            }
        }

        //---------------------------------------------------------------------------
        // ショップピースの名前が被っていれば、エフェクトを有効化、なければ無効化
        //---------------------------------------------------------------------------
        if(auto shop = Scene::Object::Get<ShopStand>()) {
            auto shop_pieces = shop->GetShopPieces();
            int  idx         = 0;    // ピースのインデックス
            for(auto& piece_wp : shop_pieces) {
                if(auto piece = piece_wp.lock()) {
                    std::string effect_name = "ShopPieceEffect" + std::to_string(idx);
                    std::string piece_type  = piece->GetNameDefault().data();    // ピースの種類名を取得
                    // 所持しているピースの種類名セットに含まれているかでエフェクトの有効/無効を切り替え
                    if(owned_piece_types.find(piece_type.data()) != owned_piece_types.end()) {
                        // 所持しているならエフェクトを有効化
                        if(!Scene::Object::Get<Object>(effect_name.data())) {
                            const std::string path   = "data/AutoChess/Effect/DuplicatePieceEffect.efkefc";
                            float3            pos    = piece->GetTranslate();
                            auto              effect = ComponentEffect::Object::Create(path, pos);
                            effect->SetName(effect_name.data());
                        }
                    }
                    else {
                        // 所持していないならエフェクトを無効化
                        if(auto effect = Scene::Object::Get<Object>(effect_name.data())) {
                            Scene::Object::Release(effect);
                        }
                    }
                }
                idx++;    // インデックスを進める
            }
        }
    };
    SetProc("ShopPieceEffectManagerUpdateProc", update_proc, ProcTiming::Update, ProcPriority::NORMAL);
}
