#include "engine/engine.h"
#include "gamePluginUtil.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// コールバック構造体
struct gamePluginUtilCallback{
	struct gamePluginUtilCallback *next;
	int callbackId;
	void *param;
	void *callback;
};

static struct{
	// 揮発性一時バッファ
	struct{
		void *buff;
		size_t size;
	} temporary;
	// コールバック
	struct{
		int callbackIdCount;
		struct gamePluginUtilCallback *list;
		struct gamePluginUtilCallback *pool;
	} callback;
} localGlobal = {0};

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 揮発性一時バッファ 返値領域は解放禁止
void *corePluginUtilTemporaryBuffer(size_t size){
	if(localGlobal.temporary.size < size){
		if(localGlobal.temporary.size > 0){free(localGlobal.temporary.buff);}
		localGlobal.temporary.size = size;
		localGlobal.temporary.buff = malloc(localGlobal.temporary.size);
	}
	return localGlobal.temporary.buff;
}

// ----------------------------------------------------------------

// コールバック関数の登録 ひな形
int corePluginUtilCallbackSet(void *param, void *callback){
	struct gamePluginUtilCallback *new = NULL;

	if(localGlobal.callback.pool != NULL){
		// プールから要素をとってくる
		new = localGlobal.callback.pool;
		localGlobal.callback.pool = new->next;
	}else{
		// 要素を新しく作成
		new = (struct gamePluginUtilCallback*)calloc(1, sizeof(struct gamePluginUtilCallback));
	}

	// 要素の設定
	new->next = NULL;
	new->callbackId = ++localGlobal.callback.callbackIdCount;
	new->param = param;
	new->callback = callback;

	// 要素をリストに追加
	if(localGlobal.callback.list != NULL){
		struct gamePluginUtilCallback *temp = localGlobal.callback.list;
		while(temp->next != NULL){temp = temp->next;}
		temp->next = new;
	}else{
		localGlobal.callback.list = new;
	}

	return new->callbackId;
}

// コールバック関数の取得と解放 ひな形
void *corePluginUtilCallbackGet(int callbackId, void **param){
	struct gamePluginUtilCallback *prev = NULL;
	struct gamePluginUtilCallback *temp = localGlobal.callback.list;
	while(temp != NULL){
		if(temp->callbackId == callbackId){
			// リストから要素を外す
			struct gamePluginUtilCallback *use = temp;
			temp = temp->next;
			if(prev == NULL){localGlobal.callback.list = temp;}
			else{prev->next = temp;}
			// コールバック取得
			void *callback = use->callback;
			if(param != NULL){*param = use->param;}
			// 解放の代わりに要素のプール
			use->callbackId = 0;
			use->param = NULL;
			use->callback = NULL;
			use->next = localGlobal.callback.pool;
			localGlobal.callback.pool = use;

			return callback;
		}else{
			prev = temp;
			temp = temp->next;
		}
	}
	return NULL;
}

//// コールバック関数の全解放
//void corePluginUtilCallbackDispose(){
//	struct gamePluginUtilCallback *tempList = localGlobal.callback.list;
//	while(tempList != NULL){
//		struct gamePluginUtilCallback *dispose = tempList;
//		tempList = tempList->next;
//		// 要素の除去
//		free(dispose->param);
//		free(dispose);
//	}
//	localGlobal.callback.list = NULL;
//
//	struct gamePluginUtilCallback *tempPool = localGlobal.callback.pool;
//	while(tempPool != NULL){
//		struct gamePluginUtilCallback *dispose = tempPool;
//		tempPool = tempPool->next;
//		// 要素の除去
//		free(dispose);
//	}
//	localGlobal.callback.pool = NULL;
//}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// url取得 返値文字列は揮発性バッファで解放禁止
char *gamePluginUtilUrlGet(){
	char *value = "http://totetero.com/cgi-bin/php";
	char *buff = (char*)corePluginUtilTemporaryBuffer(strlen(value) + 1);
	strcpy(buff, value);
	return buff;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

