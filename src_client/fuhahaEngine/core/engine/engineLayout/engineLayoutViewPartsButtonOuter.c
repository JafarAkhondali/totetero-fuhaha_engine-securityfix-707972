#include "../../library.h"
#include "../../engine/engineUtil/engineUtil.h"
#include "../../engine/engineLayout/engineLayout.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct engineLayoutViewPartsButtonOuterImplement{
	struct engineLayoutViewPartsButtonOuter super;
};

// ----------------------------------------------------------------

// タッチ判定
static bool isInner(struct engineLayoutViewPartsButtonOuterImplement *this, double x, double y){
	// 自要素の内側にあることを確認する
	if(!engineLayoutViewUtilPositionTransformIsInner((struct engineLayoutView*)this, x, y)){return false;}
	// 子要素の外側にあることを確認する 孫要素は確認しない
	struct engineLayoutView *temp = this->super.super.family.childrenHead;
	while(temp != NULL){
		if(temp->position.isInner(temp, x, y)){return false;}
		temp = temp->family.next;
	}
	return true;
}

// 初期化
static void init(struct engineLayoutViewPartsButtonOuterImplement *this){
	engineLayoutViewUtilInit((struct engineLayoutView*)this);

	// タッチ設定
	this->super.super.position.isInner = (bool(*)(struct engineLayoutView*, double, double))isInner;
	this->super.super.interact.setting.isTouchable = true;
}

// ----------------------------------------------------------------

// タッチ処理
static bool touch(struct engineLayoutViewPartsButtonOuterImplement *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel){
	bool isActive = false;
	bool isActiveChild = engineLayoutViewUtilChildrenTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive); isActive = isActiveChild || isActive;
	bool isActiveLocal = engineLayoutViewUtilInteractTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive); isActive = isActiveLocal || isActive;
	return isActive;
}

// 計算
static void calc(struct engineLayoutViewPartsButtonOuterImplement *this){
	// 子要素計算
	engineLayoutViewUtilChildrenCalc((struct engineLayoutView*)this);
}

// ----------------------------------------------------------------

// バッファ更新確認
static bool shouldBufferCreate(struct engineLayoutViewPartsButtonOuterImplement *this){
	return false;
}

// バッファ作成
static void bufferCreate(struct engineLayoutViewPartsButtonOuterImplement *this){
}

// 描画
static void draw(struct engineLayoutViewPartsButtonOuterImplement *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	// 子要素描画
	engineLayoutViewUtilChildrenDraw((struct engineLayoutView*)this, mat, color);
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct engineLayoutViewPartsButtonOuterImplement *this){
	// 子要素一時停止
	engineLayoutViewUtilChildrenPause((struct engineLayoutView*)this);
}

// 破棄
static void dispose(struct engineLayoutViewPartsButtonOuterImplement *this){
	// 子要素破棄
	engineLayoutViewUtilChildrenDispose((struct engineLayoutView*)this);

	// 自要素破棄
	engineLayoutViewUtilDispose((struct engineLayoutView*)this);
	engineUtilMemoryInfoFree("engineLayoutViewPartsButtonOuter", this);
}

// ----------------------------------------------------------------

// 外枠ボタン構造体 作成
struct engineLayoutViewPartsButtonOuter *engineLayoutViewPartsButtonOuterCreate(){
	struct engineLayoutViewPartsButtonOuterImplement *this = (struct engineLayoutViewPartsButtonOuterImplement*)engineUtilMemoryInfoCalloc("engineLayoutViewPartsButtonOuter", 1, sizeof(struct engineLayoutViewPartsButtonOuterImplement));
	init(this);

	struct engineLayoutView *view = (struct engineLayoutView*)this;
	view->touch = (bool(*)(struct engineLayoutView*, int, double, double, bool, bool, bool))touch;
	view->calc = (void(*)(struct engineLayoutView*))calc;
	view->draw = (void(*)(struct engineLayoutView*, struct engineMathMatrix44*, struct engineMathVector4*))draw;
	view->pause = (void(*)(struct engineLayoutView*))pause;
	view->dispose = (void(*)(struct engineLayoutView*))dispose;
	view->graphicObject.shouldBufferCreate = (bool(*)(struct engineLayoutView*))shouldBufferCreate;
	view->graphicObject.bufferCreate = (void(*)(struct engineLayoutView*))bufferCreate;
	return (struct engineLayoutViewPartsButtonOuter*)this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
