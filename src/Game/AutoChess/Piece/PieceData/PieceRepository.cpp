//---------------------------------------------------------------------------
//!	@file	PieceRepository.cpp
//! @brief	ピースのマスターデータ情報のリポジトリクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "PieceRepository.h"
//------------------------------------------------------
//! @brief JSONファイルから駒定義（PieceData）を読み込む
//------------------------------------------------------
bool PieceRepository::LoadFromJson(const std::string& path)
{
    std::ifstream file(path);    // ファイルを開く
    if(!file.is_open())
        return false;    // 開けなければ失敗

    try {
        cereal::JSONInputArchive archive(file);    // cerealアーカイブを作成
        archive(CEREAL_NVP(piece_datas_));         // pieces_ にデシリアライズ
        return true;                               // 成功
    }
    catch(const std::exception& e) {
        // 読み込み中に例外が発生した場合はエラーメッセージを出力
        std::cerr << "Failed to load PieceData: " << e.what() << std::endl;
        return false;    // 失敗
    }
}
