#ifdef _DEBUG
//Debugモードの場合
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_world300d.lib")            // opencv_core
#else
//Releaseモードの場合
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_world300.lib")

#pragma comment(lib, "C:\\Program Files\\Anaconda3\\libs\\python35.lib")
#endif

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>


using namespace std;

int main(){
	//左右画像をグレースケールで読み込み
	//stereoBMの場合はグレースケールしかできない。
	cv::Mat image_l;
	cv::Mat image_r;
#pragma omp parallel sections
{
#pragma omp section
	image_l = cv::imread("C:\\Users\\0133752\\Desktop\\ambush_5_left.jpg", 0);
#pragma omp section
	image_r = cv::imread("C:\\Users\\0133752\\Desktop\\ambush_5_right.jpg", 0);
}
	//各種パラメータ
	int numDisparities = 48;
	int blockSize = 21;

	//ステレオBMのインスタンス化
	//StereoBMのcreateメソッドでポインタを取得
	//その際に各種パラメータを引数に入力
	//StereoBM::create(
	//	numDisparities = 0 -- - int、0はデフォルト値ではなく、デフォルトの意。求める視差の最大値を16の倍数で指定する、デフォルト値は48以上と推定される
	//	blockSize = 21 ------int、Sum of Absolute Differencesを計算するウィンドウのサイズ、奇数であること、資料には3～11とあるが、5以上でないとエラーになる、大きいと広範囲で平均化され、小さいと細かくなるが、ノイズに弱くなる。
	//	）
	cv::Ptr<cv::StereoBM> sbm = cv::StereoBM::create(
		numDisparities,
		blockSize
		);

	//setメソッドを使っても設定できる。
	//なぜかSGBMの変数も設定できるし、反映もされる？
	sbm->setNumDisparities(48);
	sbm->setBlockSize(21);
	sbm->setSpeckleWindowSize(100);
	sbm->setSpeckleRange(1);

	//左右画像から深度マップを作成
	cv::Mat disparity;    //((cv::MatSize)leftImg.size, CV_16S);
	sbm->compute(image_l, image_r, disparity);

	//深度マップを視覚的に分かるようにピクセル値を変換
	cv::Mat disparity_map;
	double min, max;
	//深度マップの最小値と最大値を求める
	cv::minMaxLoc(disparity, &min, &max);
	//CV_8UC1に変換、最大レンジを0～255にする
	disparity.convertTo(disparity_map, CV_8UC1, 255.0 / (max - min), -255.0 * min / (max - min));
	cv::imshow("result", disparity_map);
	cv::waitKey(0);

	return 0;
}