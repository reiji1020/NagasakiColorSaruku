#include <iostream>
#include <windows.h>
#include <shellapi.h>

#include <opencv2/opencv.hpp>
#include <opencv2/opencv_lib.hpp>
#include <opencv2/highgui/highgui.hpp>

#pragma comment(lib, "shell32.lib")

using namespace cv;
using namespace std;

int main(void) {

	uchar hue, sat, val;
	bool browser_flag = false;
	Mat src_video(Size(640,480),CV_8UC1,Scalar::all(255));
	Mat smooth_video(Size(640, 480), CV_8UC1, Scalar::all(255));
	Mat dst_img(Size(640, 480), CV_8UC1, Scalar::all(0));
	Mat hsv_video(Size(640, 480), CV_8UC1, Scalar::all(255));
	Mat frame(Size(640, 480), CV_8UC1, Scalar::all(255));
	VideoCapture capture(0);

	// カメラが使えない場合はプログラムを止める
	if (!capture.isOpened())
		return -1;

	// ウィンドウを作成する
	char windowName[] = "カメラでさるくマップを撮影してね!";
	namedWindow(windowName, CV_WINDOW_AUTOSIZE);
	namedWindow("認識結果", CV_WINDOW_AUTOSIZE);

	// 何かキーが押下されるまで、ループをくり返す
	while (cvWaitKey(1) == -1)
	{
		dst_img = Scalar::all(0);
		// カメラから1フレーム取得する
		capture >> frame;
		src_video = frame;
		imshow(windowName,src_video);

		// HSV表色系へ色情報を変換
		// 先にノイズを消しておく
		medianBlur(src_video, smooth_video, 5);
		cvtColor(smooth_video, hsv_video, CV_BGR2HSV);
		//imshow(hsv_window, hsv_video);

		// H,S,Vの要素に分割する
		for(int y = 0; y < hsv_video.rows; y++) {
			for (int x = 0; x < hsv_video.cols; x++) {
				hue = hsv_video.at<Vec3b>(y, x)[0];
				sat = hsv_video.at<Vec3b>(y, x)[1];
				val = hsv_video.at<Vec3b>(y, x)[2];
				// 居留地マップの検出
				if ((hue < 35 && hue > 20) && sat > 127) {
					dst_img.at<uchar>(y, x) = 255;
					browser_flag = true;
				}
				else {
					dst_img.at<uchar>(y, x) = 0;
				}
			}
		}
		imshow("認識結果",dst_img);
	}
	if (browser_flag = true)
		ShellExecute(0, 0, L"http://www.saruku.info/", 0, 0, SW_SHOW);
	destroyAllWindows();
	return 0;
}