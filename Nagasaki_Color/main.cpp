#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/opencv_lib.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int main(void) {

	uchar hue, sat, val;
	Mat src_video;
	Mat smooth_video;
	Mat dst_img;
	Mat hsv_video;
	Mat frame;
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
		hsv_img = Scalar(0, 0, 0);
		// カメラから1フレーム取得する
		capture >> frame;
		src_video = frame;
		dst_img = Mat(Size(src_video.cols, src_video.rows), CV_8UC1);
		imshow(windowName,src_video);

		// HSV表色系へ色情報を変換
		// 先にノイズを消しておく
		medianBlur(src_video, smooth_video, 7);
		cvtColor(smooth_video, dst_video, CV_BGR2HSV);
		//imshow(hsv_window, hsv_video);

		// H,S,Vの要素に分割する
		for(int y = 0; y < hsv_video.rows; y++) {
			for (int x = 0; x < hsv_video.cols; x++) {
				hue = hsv_video.at<Vec3b>(y, x)[0];
				sat = hsv_video.at<Vec3b>(y, x)[1];
				val = hsv_video.at<Vec3b>(y, x)[2];
				// 居留地マップの検出
				if ((hue < 35 && hue > 20) && sat > 100) {
					dst_img.at<uchar>(y, x) = 255;
				}
				else {
					dst_img.at<uchar>(y, x) = 0;
				}
			}
		}
		imshow("認識結果",dst_img);
	}
	destroyAllWindows();
	return 0;
}
