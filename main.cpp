#include <Novice.h>
#include <cmath>
#include<math.h>

const char kWindowTitle[] = "LE2C_07_sasnami_sousi";

struct Vector3 {
	float x;
	float y;
	float z;
};

struct Matrix4x4 {
	float m[4][4];
};

//x軸回転行列
Matrix4x4 MakeRoteXMatrix(float radian) {
	Matrix4x4 result = {
		1,0,0,0,
		0,cosf(radian),sinf(radian),0,
		0,-sinf(radian),cosf(radian),0,
		0,0,0,1,
	};
	return result;
}

//Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 result = {
		cosf(radian),0,-sinf(radian),0,
		0,1,0,0,
		sinf(radian),0,cosf(radian),0,
		0,0,0,1,
	};
	return result;
}

//Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 result = {
		cosf(radian),sinf(radian),0,0,
		-sinf(radian),cosf(radian),0,0,
		0,0,1,0,
		0,0,0,1,
	};
	return result;
}

//平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3 &translate) {
	Matrix4x4 result = {
	  1, 0, 0, 0,
	  0, 1, 0, 0,
	  0, 0, 1, 0,
	 translate.x, translate.y, translate.z, 1
	};
	return result;
}

//積
Matrix4x4 Multiply(const Matrix4x4 &m1, const Matrix4x4 &m2) {
	Matrix4x4 result = {};
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	return result;
}

//アフィン変換行列
Matrix4x4 MakeAffineMatrox(const Vector3 &scale, const Vector3 &rotate, const Vector3 &translate) {
	Matrix4x4 result;

	// スケーリング行列
	Matrix4x4 scaleMat = {
		scale.x, 0,       0,       0,
		0,       scale.y, 0,       0,
		0,       0,       scale.z, 0,
		0,       0,       0,       1
	};

	Matrix4x4 rot = Multiply(Multiply(MakeRoteXMatrix(rotate.x), MakeRotateYMatrix(rotate.y)), MakeRotateZMatrix(rotate.z));
	result = Multiply(Multiply(scaleMat, rot), MakeTranslateMatrix(translate));

	return result;
}

static const int kRowHeight = 20;
static const int kColumnWidth = 60;
void MatrixScreenPrintf(int x, int y, const Matrix4x4 &matrix, const char *label) {
	Novice::ScreenPrintf(x, y - kRowHeight, "%s", label); // ラベル表示（上に）
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(x + column * kColumnWidth, y + row * kRowHeight, "%6.02f", matrix.m[row][column]);
		}
	}
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		Vector3 scale{ 1.2f,0.79f,-2.1f };
		Vector3 rotate{ 0.4f,1.43f,-0.8f };
		Vector3 translate{ 2.7f,-4.15f,1.57f };
		Matrix4x4 worldMatrix = MakeAffineMatrox(scale, rotate, translate);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///


		MatrixScreenPrintf(0, 20, worldMatrix, "worldMatrix");

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
