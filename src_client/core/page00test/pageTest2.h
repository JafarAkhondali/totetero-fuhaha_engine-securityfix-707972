#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ページ状態構造体
struct pageTest2Status{
	// 射影行列
	struct engineMathMatrix44 matOrtho;
	// テクスチャ
	engineGraphicObjectTexId egoIdTexSystem;
};

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// サンプルポップアップ処理
struct engineCartridgePopup *popupTest2SampleCreate(struct pageTest2Status *stat);
int popupTest2SampleResult(struct engineCartridgePopup *that);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
