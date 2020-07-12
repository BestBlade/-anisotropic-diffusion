/*
*����������������������������������������������������������������������������������������������������
*������    ��������ԭ���д����ʵ��PM��������ģ��												   ��
*������    �ߣ�Ĳ����|BestBlade																	   ��
*������    ����1.0																			   	   ��
*��������ʱ�䣺2020.07.12																		   ��
*����������������������������������������������������������������������������������������������������
*/

/*	CSDNѧϰ�ο���ַ
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

void anisotropic_filter(Mat &origin_pic, Mat &result_pic, int iter, double kappa, double lambda, int offset)					/*	��origin_pic������ͼƬ	*/
{																																/*	��result_pic�����ͼƬ	*/
	result_pic = origin_pic.clone();																							/*	��iter����������	*/

	if ((offset == 1) ? (offset == 2) : 1)																						/*	��kappa���ȴ���ϵ��20-100	*/
	{																															/*	��lambda��һ��С��0.25�Ա����ȶ�	*/
		cerr << "ERROR:����offset��Ϊ1��2" << endl;																				/*	��offset��P-M��ɢ�������ͣ�����ˮƽ���ƣ�Ĭ��Ϊһ����ʹ�õ�һ��P-M��ɢ����	*/
	}

	if (iter < 1)
	{
		cerr << "ERROR:iter�������������1" << endl;
	}

	if (origin_pic.empty())
	{
		cerr << "ERROR:ͼƬ����ʧ��" << endl;
	}

	int row = origin_pic.rows;
	int col = origin_pic.cols;

	for (int t = 1; t <= iter; t++)
	{
		Mat during_pic = result_pic;																					/*	ע��˴���ѡ�result_pic��������during_pic��������ɵ�����֮ǰ�����ˡ�origin_pic������һֱֻ�ǵ���һ�ε�Ч��	*/

		double delta_n = { 0 };
		double delta_s = { 0 };
		double delta_e = { 0 };
		double delta_w = { 0 };

		for (int i = 1; i <= row - 2; i++)																				/*	ע��˴�-2��������С�j+1���Ȳ�����-2���ܲ������ַ��Ѱ�����������˰���	*/
		{
			for (int j = 1; j <= col - 2; j++)
			{
				for (int channel = 0; channel <= 2; channel++)
				{
					delta_n = during_pic.at<Vec3b>(i, j - 1)[channel] - during_pic.at<Vec3b>(i, j)[channel];			/*	��ͨ����������(i,j)��Χ�ĸ�������ݶ�	*/
					delta_s = during_pic.at<Vec3b>(i, j + 1)[channel] - during_pic.at<Vec3b>(i, j)[channel];
					delta_e = during_pic.at<Vec3b>(i - 1, j)[channel] - during_pic.at<Vec3b>(i, j)[channel];
					delta_w = during_pic.at<Vec3b>(i + 1, j)[channel] - during_pic.at<Vec3b>(i, j)[channel];

					double cn = { 0 };
					double cs = { 0 };
					double ce = { 0 };
					double cw = { 0 };

					if (offset == 1)																					/*	����ϵ��cN��cS��cW��cE	*/
					{
						cn = exp(-delta_n * delta_n / (kappa * kappa));													/*	����һ	*/
						cs = exp(-delta_s * delta_s / (kappa * kappa));
						ce = exp(-delta_e * delta_e / (kappa * kappa));
						cw = exp(-delta_w * delta_w / (kappa * kappa));

					}
					else if (offset == 2)
					{
						cn = 1 / (1 + (delta_n * delta_n / (kappa * kappa)));											/*	������	*/
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

	anisotropic_filter(origin_pic, result_pic, iter, kappa, lambda, offset);										/*	�Զ����������ģ��ģ������	*/

	imshow("ԭͼ", origin_pic);
	imshow("ƽ�����ͼ", result_pic);

	waitKey(0);
	//return 0;
}