#include "fuhahaEngine.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ページカートリッジ構造体
struct pageCartridgeTest4{
	struct engineCartridgePage super;

	struct enginePrimitiveSphere primitiveSphere;

	struct pageCartridgeTest4BufferCompare{
		int sw;
		int sh;
	} bufferCompare;
	engineGraphicObjectVBOId egoIdVert;
	engineGraphicObjectVBOId egoIdNorm;
	engineGraphicObjectVBOId egoIdTexc;
	engineGraphicObjectIBOId egoIdFace;
	engineGraphicTextureId egoIdTexSystem;

	int step;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct pageCartridgeTest4 *this){
	// 画像読み込み
	this->egoIdTexSystem = engineGraphicTextureCreateLocal("img/system.png", ENGINEGRAPHICTEXTURETYPE_LINEAR);
	// 基本図形初期化
	enginePrimitiveSphereInit(&this->primitiveSphere);

	this->step = 0;
}

// ----------------------------------------------------------------

// 計算
static void calc(struct pageCartridgeTest4 *this){
	this->step++;
}

// ----------------------------------------------------------------

// バッファ作成
static void createBuffer(struct pageCartridgeTest4 *this){
	struct pageCartridgeTest4BufferCompare bufferCompare;
	bufferCompare.sw = global.screen.w;
	bufferCompare.sh = global.screen.h;

	if(memcmp(&this->bufferCompare, &bufferCompare, sizeof(struct pageCartridgeTest4BufferCompare))){
		memcpy(&this->bufferCompare, &bufferCompare, sizeof(struct pageCartridgeTest4BufferCompare));

		// バッファ作成開始
		engineGraphicBufferBegin();

		// 基本図形作成
		enginePrimitiveSphereCreateArray(&this->primitiveSphere);

		// バッファ作成完了
		engineGraphicBufferEnd(&this->egoIdVert, &this->egoIdNorm, NULL, &this->egoIdTexc, &this->egoIdFace);
	}
}

// 描画
static void draw(struct pageCartridgeTest4 *this){
	engineGraphicEngineClearAll();

	// 描画準備
	createBuffer(this);

	// 行列作成
	double distance = 10;
	double lookAtX = 0;
	double lookAtY = 0;
	double lookAtZ = 0;
	double angleCos = ENGINEMATH_COS045;
	double angleSin = ENGINEMATH_SIN045;
	double rotateCos = engineMathCos(ENGINEMATH_DEG2RAD * this->step);
	double rotateSin = engineMathSin(ENGINEMATH_DEG2RAD * this->step);
	double lookEyeX = lookAtX + distance * angleCos * rotateSin;
	double lookEyeY = lookAtY + distance * angleSin;
	double lookEyeZ = lookAtZ + distance * angleCos * rotateCos;
	double lookUpX = 0.0;
	double lookUpY = 1.0;
	double lookUpZ = 0.0;
	struct engineMathMatrix44 tempMat1;
	struct engineMathMatrix44 tempMat2;
	struct engineMathMatrix44 tempMat3;
	engineMathMat4Perspective(&tempMat1, ENGINEMATH_DEG2RAD * 30, (double)global.window.w / global.window.h, 1.0, 1000.0);
	engineMathMat4LookAt(&tempMat2, lookEyeX, lookEyeY, lookEyeZ, lookAtX, lookAtY, lookAtZ, lookUpX, lookUpY, lookUpZ);
	engineMathMat4Multiply(&tempMat2, &tempMat1, &tempMat2);
	engineMathMat4Ortho(&tempMat1, -global.screen.offset.x, global.screen.w + global.screen.offset.x, global.screen.h + global.screen.offset.y, -global.screen.offset.y, -1, 1);

	// モードとバッファ設定
	engineGraphicEngineSetDrawMode(ENGINEGRAPHICENGINEMODEDRAW_PHONG);
	engineGraphicEngineBindVertVBO(this->egoIdVert);
	engineGraphicEngineBindNormVBO(this->egoIdNorm);
	engineGraphicEngineBindTexcVBO(this->egoIdTexc);
	engineGraphicEngineBindFaceIBO(this->egoIdFace);
	engineGraphicEngineBindTexture(this->egoIdTexSystem);

	// 球描画
	engineMathMat4Copy(&tempMat3, &tempMat2);
	engineMathMat4Translate(&tempMat3, 0, 0, 1);
	engineGraphicEngineSetMatrix(&tempMat3);
	engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, 1.0);
	enginePrimitiveSphereDraw(&this->primitiveSphere);

	engineGraphicEngineFlush();
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct pageCartridgeTest4 *this){
}

// 破棄
static void dispose(struct pageCartridgeTest4 *this){
	engineGraphicObjectVBODispose(this->egoIdVert);
	engineGraphicObjectVBODispose(this->egoIdNorm);
	engineGraphicObjectVBODispose(this->egoIdTexc);
	engineGraphicObjectIBODispose(this->egoIdFace);
	engineGraphicTextureDispose(this->egoIdTexSystem);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// ページカートリッジ作成
static struct engineCartridgePage *pageTest4Create(){
	struct pageCartridgeTest4 *this = (struct pageCartridgeTest4*)engineUtilMemoryCalloc(1, sizeof(struct pageCartridgeTest4));

	struct engineCartridgePage *cartridge = (struct engineCartridgePage*)this;
	cartridge->isExist = true;
	cartridge->init = (void(*)(struct engineCartridgePage*))init;
	cartridge->calc = (void(*)(struct engineCartridgePage*))calc;
	cartridge->draw = (void(*)(struct engineCartridgePage*))draw;
	cartridge->pause = (void(*)(struct engineCartridgePage*))pause;
	cartridge->dispose = (void(*)(struct engineCartridgePage*))dispose;
	return cartridge;
}

// ページ状態初期化
void pageTest4PushPage(){
	engineCartridgePageManagerPush(pageTest4Create());
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
