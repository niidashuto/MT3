#include "DxLib.h"
#include "Vector3.h"

//関数プロトタイプ宣言
void DrawAxis3D(const float length);


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// ウィンドウ横幅
	const int WIN_WIDTH = 1024;
	// ウィンドウ縦幅
	const int WIN_HEIGHT = 576;
	// ウィンドウモードに設定
	ChangeWindowMode(TRUE);
	// 画面サイズの最大サイズ、カラービット数を設定(モニターの解像度に合わせる)
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);
	// 画面の背景色を設定する
	SetBackgroundColor(0, 0, 64);
	// DXlibの初期化
	if (DxLib_Init() == -1)  return -1;
	// (ダブルバッファ)描画先グラフィック領域は裏面を指定
	SetDrawScreen(DX_SCREEN_BACK);
	SetUseZBuffer3D(TRUE);//Zバッファを有効にする
	SetWriteZBuffer3D(TRUE);//Zバッファへの書き込みを有効にする

	//クリップ面
	SetCameraNearFar(1.0f, 1000.0f);//カメラの有効範囲を設定
	SetCameraScreenCenter(WIN_WIDTH / 2.0f, WIN_HEIGHT / 2.0f);//画面の中心をカメラの中心に合わせる
	SetCameraPositionAndTargetAndUpVec(
		VGet(0.0f,0.0f,-120.0f),//カメラの位置
		VGet(0.0f, 0.0f, 0.0f),//カメラの注視点
		VGet(0.0f, 1.0f, 0.0f)//カメラの上の向き
	);

	//時間計測に必要なデータ
	long long startCount = 0;
	long long nowCount = 0;
	long long elapsedCount = 0;

	//補間で使うデータ
	//start -> endを5[s]で完了させる
	Vector3 start(-100.0f, 0.0f, 0.0f);//スタート地点
	Vector3 end(+100.0f, 0.0f, 0.0f);//エンド地点
	float maxTime = 5.0f;
	float timeRate;

	//球の位置
	Vector3 position;

	//実行前に　カウンタ値を取得
	startCount = GetNowHiPerformanceCount(); //long long int型  64bit int

	// ゲームループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		
		
		//---------  ここからプログラムを記述  ----------//

		// 更新処理

		//[R]でリセット
		if (CheckHitKey(KEY_INPUT_R))
		{
			startCount = GetNowHiPerformanceCount();
		}
		//経過時間(elapsedTime[s])の計算
		nowCount = GetNowHiPerformanceCount();
		elapsedCount = nowCount - startCount;
		float elapsedTime = static_cast<float>(elapsedCount) / 1'000'000.0f;

		//スタート地点　　:start
		//エンド地点　　　:end
		//経過時間　　　　:elapsedTime[s]
		//移動完了の率(経過時間/全体時間):timeRate(%)

		timeRate = min(elapsedTime / maxTime, 1.0f);

		position = lerp(start, end, timeRate);

		//position = easeIn(start,end,timeRate);
		//position = easeOut(start,end,timeRate);
		//position = easeInOut(start,end,timeRate);

		// 描画処理--------------------------------------
		// 画面クリア
		ClearDrawScreen();  //画面を消去
		DrawAxis3D(500.0f); //xyz軸の描画

		//球の描画
		DrawSphere3D(position, 5.0f, 32, GetColor(255, 0, 0), GetColor(255, 255, 255), TRUE);

		//
		DrawFormatString(0, 0, GetColor(255, 255, 255), "position(%5.1f,%5.1f,%5.1f)", position.x, position.y, position.z);
		DrawFormatString(0, 20, GetColor(255, 255, 255), "%7.3f [s]", elapsedTime);
		DrawFormatString(0, 40, GetColor(255, 255, 255), "[R] : Restart");

		//DrawKeyOperation();//キー操作の描画

		//---------  ここまでにプログラムを記述  ---------//
		// (ダブルバッファ)裏面
		ScreenFlip();

		
	}
	// Dxライブラリ終了処理
	DxLib_End();

	// 正常終了
	return 0;
}

//xyzの描画
void DrawAxis3D(const float length)
{
	//軸の線の描画
	DrawLine3D(VGet(-length, 0, 0), VGet(+length, 0, 0), GetColor(255, 0, 0));//x軸
	DrawLine3D(VGet(0, -length, 0), VGet(0, +length, 0), GetColor(0, 255, 0));//y軸
	DrawLine3D(VGet(0, 0, -length), VGet(0, 0, +length), GetColor(0, 0, 255));//z軸
	
}

