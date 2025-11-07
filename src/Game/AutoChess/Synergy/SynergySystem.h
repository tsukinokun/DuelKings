//---------------------------------------------------------------------------
//!	@file	SynergySystem.h
//! @brief	シナジーのシステム関数群
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
////------------------------------------------------------
////! @brief シナジーの段階を評価する関数
////! @param synergy 判定対象のシナジー定義（段階条件を含む）
////! @param board 現在の盤面状態（駒の配置・種類など）
////! @return 発動段階（0 = 未発動）
////------------------------------------------------------
//int EvaluateSynergyLevel(const Synergy& synergy, const BoardState& board) {
//    int count = board.CountSynergyMembers(synergy.GetID());    // 対象駒の数を取得
//    int level = 0;
//
//    // 段階条件テーブルに基づいて段階を判定
//    for(int threshold : synergy.GetLevelThresholds()) {
//        if(count >= threshold)
//            ++level;
//        else
//            break;
//    }
//    return level;
//}
//
////------------------------------------------------------
////! @brief 現在の盤面における発動中シナジー一覧を取得
////! @param board 現在の盤面状態
////! @param repo シナジー定義リポジトリ（全シナジー一覧）
////! @return 発動中の段階付きシナジー一覧
////------------------------------------------------------
//std::vector<Synergy> GetActiveSynergies(const BoardState& board, const SynergyRepository& repo) {
//    std::vector<Synergy> result;
//
//    for(const auto& synergy : repo.GetAllSynergies()) {
//        int level = EvaluateSynergyLevel(synergy, board);
//        if(level > 0) {
//            result.push_back(synergy.WithLevel(level));    // 段階付きインスタンスを生成
//        }
//    }
//    return result;
//}
//
////------------------------------------------------------
////! @brief UI表示用のシナジーデータを生成
////! @param synergies 発動中のシナジー一覧
////! @return UI表示用データ（名前・説明・段階など）
////------------------------------------------------------
//std::vector<SynergyDisplayData> GetSynergyDisplayData(const std::vector<Synergy>& synergies) {
//    std::vector<SynergyDisplayData> result;
//
//    for(const auto& synergy : synergies) {
//        result.push_back(synergy.ToDisplayData());    // 表示用DTOに変換
//    }
//    return result;
//}
