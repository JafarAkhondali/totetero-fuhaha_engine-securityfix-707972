#include "../../library.h"
#include "../../engine/engineMath/engineMath.h"
#include "../../engine/engineUtil/engineUtil.h"
#include "../../engine/engineGraphic/engineGraphic.h"
#include "../../engine/engineLayout/engineLayout.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct engineLayoutViewPartsRootImplement{
	struct engineLayoutViewPartsRoot super;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct engineLayoutViewPartsRootImplement *this){
	// レイアウト初期化
	engineLayoutViewUtilFamilyInit((struct engineLayoutView*)this);
	engineLayoutViewUtilPositionInit((struct engineLayoutView*)this);
	// レイアウト設定
	engineLayoutViewUtilPositionSetLtRtTpBm((struct engineLayoutView*)this, 0, 0, 0, 0);
}

// ----------------------------------------------------------------

// タッチ処理
static bool touch(struct engineLayoutViewPartsRootImplement *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel){
	bool isActive = false;
	isActive = engineLayoutViewUtilChildrenTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive) || isActive;
	isActive = engineLayoutViewUtilInteractTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive) || isActive;
	return isActive;
}

// 計算
static void calc(struct engineLayoutViewPartsRootImplement *this){
	// 子要素計算
	engineLayoutViewUtilChildrenCalc((struct engineLayoutView*)this);
}

// ----------------------------------------------------------------

// 描画
static void draw(struct engineLayoutViewPartsRootImplement *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	engineLayoutViewUtilPositionModeSetDraw();

	// ルートなので引数の行列を無視して正射影行列作成
	struct engineMathMatrix44 tempMat1;
	engineMathMat4Ortho(&tempMat1, -global.screen.offset.x, global.screen.w + global.screen.offset.x, global.screen.h + global.screen.offset.y, -global.screen.offset.y, -1, 1);
	mat = &tempMat1;

	// ルートなので引数の色を無視して白色作成
	struct engineMathVector4 tempColor1;
	engineMathVec4Set(&tempColor1, 1.0, 1.0, 1.0, 1.0);
	color = &tempColor1;

	// 描画モード設定
	engineGraphicEngineSetDrawMode(ENGINEGRAPHICENGINEMODEDRAW_2D_ALPHA_NORMAL);
	engineGraphicStencilStackMaskRead();

	// 子要素描画
	engineLayoutViewUtilChildrenDraw((struct engineLayoutView*)this, mat, color);

	engineLayoutViewUtilPositionModeSetCalc();
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct engineLayoutViewPartsRootImplement *this){
	// 子要素一時停止
	engineLayoutViewUtilChildrenPause((struct engineLayoutView*)this);
}

// 破棄
static void dispose(struct engineLayoutViewPartsRootImplement *this){
	// 子要素破棄
	engineLayoutViewUtilChildrenDispose((struct engineLayoutView*)this);

	// 自要素破棄
	engineLayoutViewUtilPositionDispose((struct engineLayoutView*)this);
	engineLayoutViewUtilFamilyDispose((struct engineLayoutView*)this);
	engineUtilMemoryInfoFree("engineLayoutViewPartsRoot", this);
}

// ----------------------------------------------------------------

// ルート構造体 作成
struct engineLayoutViewPartsRoot *engineLayoutViewPartsRootCreate(){
	struct engineLayoutViewPartsRootImplement *this = (struct engineLayoutViewPartsRootImplement*)engineUtilMemoryInfoCalloc("engineLayoutViewPartsRoot", 1, sizeof(struct engineLayoutViewPartsRootImplement));
	init(this);

	struct engineLayoutView *view = (struct engineLayoutView*)this;
	view->touch = (bool(*)(struct engineLayoutView*, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel))touch;
	view->calc = (void(*)(struct engineLayoutView*))calc;
	view->draw = (void(*)(struct engineLayoutView*, struct engineMathMatrix44*, struct engineMathVector4*))draw;
	view->pause = (void(*)(struct engineLayoutView*))pause;
	view->dispose = (void(*)(struct engineLayoutView*))dispose;
	return (struct engineLayoutViewPartsRoot*)this;
}

// ----------------------------------------------------------------

// ルート構造体 運用
void engineLayoutViewPartsRootTouch(struct engineLayoutViewPartsRoot *this, bool isCancel){engineLayoutViewUtilInteractTouchRoot((struct engineLayoutView*)this, isCancel);}
void engineLayoutViewPartsRootCalc(struct engineLayoutViewPartsRoot *this){engineLayoutViewCalc((struct engineLayoutView*)this);}
void engineLayoutViewPartsRootDraw(struct engineLayoutViewPartsRoot *this){engineLayoutViewDraw((struct engineLayoutView*)this, NULL, NULL);}
void engineLayoutViewPartsRootPause(struct engineLayoutViewPartsRoot *this){engineLayoutViewPause((struct engineLayoutView*)this);}
void engineLayoutViewPartsRootDispose(struct engineLayoutViewPartsRoot *this){engineLayoutViewDispose((struct engineLayoutView*)this);}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
