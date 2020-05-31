#include "DxLib.h"

int Key[256];
int heartstate=3;
int bridgestate[3] = { 1,1,1 };
int heartflag = 0; //1のとき減らす

// キーの入力状態を更新する
int gpUpdateKey() {
	char tmpKey[256]; // 現在のキーの入力状態を格納する
	GetHitKeyStateAll(tmpKey); // 全てのキーの入力状態を得る
	for (int i = 0; i<256; i++) {
		if (tmpKey[i] != 0) { // i番のキーコードに対応するキーが押されていたら
			Key[i]++;     // 加算
		}
		else {              // 押されていなければ
			Key[i] = 0;   // 0にする
		}
	}
	return 0;
}

void Move(int chara, int flame, int *start, int *speed,double *x,int y,int num) {
	int rnd;
	static int lr[3] = { 0 };
	if (lr[num] == 0) {
		if ((flame >= *start) && (flame % 10 == 0)) {
			*x += *speed;
		}
		DrawRotaGraph(700 - (int)*x, y, 1.0, 0, chara, TRUE);
		if (700 - *x <= -50) {
			rnd = GetRand(300);
			*start = flame + rnd;
			*x = 0;
			*speed = (flame / 500) + 10;
			lr[num] = GetRand(1);
		}else if( ((700-*x)>=420)&& ((700 - *x) <= 450) &&(bridgestate[num]==0)) {
			heartflag=1;
			*x = 0;
			lr[num] = GetRand(1);
		}
		else if (((700 - *x) >= 205) && ((700 - *x) <= 235) &&(bridgestate[num] == 1)) {
			heartflag = 1;
			*x = 0;
			lr[num] = GetRand(1);
		}
	}
	else {
		if ((flame >= *start) && (flame % 10 == 0)) {
			*x += *speed;
		}
		DrawRotaGraph(-50 + (int)*x, y,1.0,0, chara, TRUE,1);
		if (-50 + *x >= 700) {
			rnd = GetRand(300);
			*start = flame + rnd;
			*x = 0;
			*speed = (flame / 500) + 10;
			lr[num] = GetRand(1);
		}
		else if (((-50 + *x) >= 430) && ((-50 + *x) <= 440) && (bridgestate[num] == 0)) {
			heartflag = 1;
			*x = 0;
			lr[num] = GetRand(1);
		}
		else if (((-50 + *x) >= 215) && ((-50 + *x) <= 225) && (bridgestate[num] == 1)) {
			heartflag = 1;
			*x = 0;
			lr[num] = GetRand(1);
		}
	}
}



int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(TRUE), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK); //ウィンドウモード変更と初期化と裏画面設定
	int back,road,heart,hiyoko1,hiyoko2,hiyoko3,bridge1,bridge2,bridge3;
	back = LoadGraph("image/back.png"); //背景の読み込み
	road = LoadGraph("image/road.png");
	heart= LoadGraph("image/heart.png");
	hiyoko1= LoadGraph("image/hiyoko.png");
	hiyoko2 = LoadGraph("image/hiyoko.png");
	hiyoko3 = LoadGraph("image/hiyoko.png");
	bridge1= LoadGraph("image/bridge.png");
	bridge2 = LoadGraph("image/bridge.png");
	bridge3 = LoadGraph("image/bridge.png");
	double x[3] = { 0 };
	int start[3] = { 0 };
	int flame=0,i;
	int speed[3] = { 2,4,6 };
	int bridgex[3] = { 435,435,435 };
	int heartx = 25;
	int end = 10;
	int score = 0;
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && gpUpdateKey() == 0) {
		clsDx();
		printfDx("%d", flame++);
		DrawGraph(0, 0, back, TRUE);
		DrawRotaGraph(320, 440, 0.8,0,road, TRUE);
		DrawRotaGraph(320, 300, 0.8, 0, road, TRUE);
		DrawRotaGraph(320, 160, 0.8, 0, road, TRUE);
		score = flame / 10;
		DrawFormatString(550, 20, GetColor(0, 0, 0), "SCORE %d",score);
		if (heartstate <= 0) {
			DrawFormatString(230, 240, GetColor(0, 0, 0), "Game Over.Press ESC key.");
			end--;
			if (end == 0) {
				break;
			}
		}
		for (i = 1, heartx = 25; i <= heartstate; i++) {
			
			DrawRotaGraph(heartx, 25, 1.0, 0, heart, TRUE);
			heartx += 50;
		}
		if (heartflag == 1) {
			heartstate--;
			heartflag = 0;
		}
		if (Key[KEY_INPUT_W] >= 1) {
			bridgex[2] = 435;
			bridgestate[2] = 1;
		}
		else if (Key[KEY_INPUT_Q] >= 1) {
			bridgex[2] = 220;
			bridgestate[2] = 0;
		}
		if (Key[KEY_INPUT_S] >= 1) {
			bridgex[1] = 435;
			bridgestate[1] = 1;
		}
		else if (Key[KEY_INPUT_A] >= 1) {
			bridgex[1] = 220;
			bridgestate[1] = 0;
		}
		if (Key[KEY_INPUT_X] >= 1) {
			bridgex[0] = 435;
			bridgestate[0] = 1;
		}
		else if (Key[KEY_INPUT_Z] >= 1) {
			bridgex[0] = 220;
			bridgestate[0] = 0;
		}
		Move(hiyoko1, flame, &start[0], &speed[0], &x[0], 390, 0);
		Move(hiyoko2, flame, &start[1], &speed[1], &x[1], 250, 1);
		Move(hiyoko3, flame, &start[2], &speed[2], &x[2], 110, 2);
		DrawRotaGraph(bridgex[0], 440, 0.8, 0, bridge1, TRUE);
		DrawRotaGraph(bridgex[1], 300, 0.8, 0, bridge2, TRUE);
		DrawRotaGraph(bridgex[2], 160, 0.8, 0, bridge3, TRUE);
	}
	while (1) {
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) {
			break;
		}
	}
	DxLib_End(); // DXライブラリ終了処理
	return 0;
}
//x=640,y=480
//right=435