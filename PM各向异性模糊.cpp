/*
*┌────────────────────────────────────────────────┐
*│　描    述：根据原理编写程序，实现PM各向异性模糊												   │
*│　作    者：牟峙桦|BestBlade																	   │
*│　版    本：1.0																			   	   │
*│　创建时间：2020.07.12																		   │
*└────────────────────────────────────────────────┘
*/

/*	CSDN学习参考地址
*	https://blog.csdn.net/bluecol/article/details/46690985
*/
#include <iostream>
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include "opencv2/imgproc/imgproc.hpp"
#include <stdio.h>
#include <math.h>

using namespace std;
using namespace cv;

void anisotropic_filter(Mat &origin_pic, Mat &result_pic, int iter, double kappa, double lambda, int offset)					/*	『origin_pic』输入图片	*/
{																																/*	『result_pic』输出图片	*/
	result_pic = origin_pic.clone();																							/*	『iter』迭代次数	*/

	if ((offset == 1) ? (offset == 2) : 1)																						/*	『kappa』热传导系数20-100	*/
	{																															/*	『lambda』一般小于0.25以保持稳定	*/
		cerr << "ERROR:参数offset需为1或2" << endl;																				/*	『offset』P-M扩散方程类型，由于水平限制，默认为一，即使用第一版P-M扩散方程	*/
	}

	if (iter < 1)
	{
		cerr << "ERROR:iter迭代次数需大于1" << endl;
	}

	if (origin_pic.empty())
	{
		cerr << "ERROR:图片载入失败" << endl;
	}

	int row = origin_pic.rows;
	int col = origin_pic.cols;

	for (int t = 1; t <= iter; t++)
	{
		Mat during_pic = result_pic;																					/*	注意此处须把『result_pic』赋给『during_pic』才能完成迭代，之前赋成了『origin_pic』，就一直只是迭代一次的效果	*/

		double delta_n = { 0 };
		double delta_s = { 0 };
		double delta_e = { 0 };
		double delta_w = { 0 };

		for (int i = 1; i <= row - 2; i++)																				/*	注意此处-2，因后面有『j+1』等操作，-2才能不溢出有址可寻，这里出错改了半天	*/
		{
			for (int j = 1; j <= col - 2; j++)
			{
				for (int channel = 0; channel <= 2; channel++)
				{
					delta_n = during_pic.at<Vec3b>(i, j - 1)[channel] - during_pic.at<Vec3b>(i, j)[channel];			/*	分通道计算像素(i,j)周围四个方向的梯度	*/
					delta_s = during_pic.at<Vec3b>(i, j + 1)[channel] - during_pic.at<Vec3b>(i, j)[channel];
					delta_e = during_pic.at<Vec3b>(i - 1, j)[channel] - during_pic.at<Vec3b>(i, j)[channel];
					delta_w = during_pic.at<Vec3b>(i + 1, j)[channel] - during_pic.at<Vec3b>(i, j)[channel];

					double cn = { 0 };
					double cs = { 0 };
					double ce = { 0 };
					double cw = { 0 };

					if (offset == 1)																					/*	计算系数cN，cS，cW，cE	*/
					{
						cn = exp(-delta_n * delta_n / (kappa * kappa));													/*	方法一	*/
						cs = exp(-delta_s * delta_s / (kappa * kappa));
						ce = exp(-delta_e * delta_e / (kappa * kappa));
						cw = exp(-delta_w * delta_w / (kappa * kappa));

					}
					else if (offset == 2)
					{
						cn = 1 / (1 + (delta_n * delta_n / (kappa * kappa)));											/*	方法二	*/
						cs = 1 / (1 + (delta_s * delta_s / (kappa * kappa)));
						ce = 1 / (1 + (delta_e * delta_e / (kappa * kappa)));
						cw = 1 / (1 + (delta_w * delta_w / (kappa * kappa)));
					}

					result_pic.at<Vec3b>(i, j)[channel] = during_pic.at<Vec3b>(i, j)[channel] + lambda * (cn * delta_n + cs * delta_s + ce * delta_e + cw * delta_w);
				}

			}
		}
	}
}

int main()
{
	Mat origin_pic = imread("C:/Users/Chrysanthemum/Desktop/20180611103332757.jpg");
	Mat result_pic;
	int iter = 7;
	double kappa = 20;
	double lambda = 0.25;
	int offset = 1;

	anisotropic_filter(origin_pic, result_pic, iter, kappa, lambda, offset);										/*	自定义各向异性模糊模糊函数	*/

	imshow("原图", origin_pic);
	imshow("平滑结果图", result_pic);

	waitKey(0);
	//return 0;
}