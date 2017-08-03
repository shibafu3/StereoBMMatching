#ifdef _DEBUG
//Debug���[�h�̏ꍇ
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_world300d.lib")            // opencv_core
#else
//Release���[�h�̏ꍇ
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
	//���E�摜���O���[�X�P�[���œǂݍ���
	//stereoBM�̏ꍇ�̓O���[�X�P�[�������ł��Ȃ��B
	cv::Mat image_l;
	cv::Mat image_r;
#pragma omp parallel sections
{
#pragma omp section
	image_l = cv::imread("C:\\Users\\0133752\\Desktop\\ambush_5_left.jpg", 0);
#pragma omp section
	image_r = cv::imread("C:\\Users\\0133752\\Desktop\\ambush_5_right.jpg", 0);
}
	//�e��p�����[�^
	int numDisparities = 48;
	int blockSize = 21;

	//�X�e���IBM�̃C���X�^���X��
	//StereoBM��create���\�b�h�Ń|�C���^���擾
	//���̍ۂɊe��p�����[�^�������ɓ���
	//StereoBM::create(
	//	numDisparities = 0 -- - int�A0�̓f�t�H���g�l�ł͂Ȃ��A�f�t�H���g�̈ӁB���߂鎋���̍ő�l��16�̔{���Ŏw�肷��A�f�t�H���g�l��48�ȏ�Ɛ��肳���
	//	blockSize = 21 ------int�ASum of Absolute Differences���v�Z����E�B���h�E�̃T�C�Y�A��ł��邱�ƁA�����ɂ�3�`11�Ƃ��邪�A5�ȏ�łȂ��ƃG���[�ɂȂ�A�傫���ƍL�͈͂ŕ��ω�����A�������ƍׂ����Ȃ邪�A�m�C�Y�Ɏキ�Ȃ�B
	//	�j
	cv::Ptr<cv::StereoBM> sbm = cv::StereoBM::create(
		numDisparities,
		blockSize
		);

	//set���\�b�h���g���Ă��ݒ�ł���B
	//�Ȃ���SGBM�̕ϐ����ݒ�ł��邵�A���f�������H
	sbm->setNumDisparities(48);
	sbm->setBlockSize(21);
	sbm->setSpeckleWindowSize(100);
	sbm->setSpeckleRange(1);

	//���E�摜����[�x�}�b�v���쐬
	cv::Mat disparity;    //((cv::MatSize)leftImg.size, CV_16S);
	sbm->compute(image_l, image_r, disparity);

	//�[�x�}�b�v�����o�I�ɕ�����悤�Ƀs�N�Z���l��ϊ�
	cv::Mat disparity_map;
	double min, max;
	//�[�x�}�b�v�̍ŏ��l�ƍő�l�����߂�
	cv::minMaxLoc(disparity, &min, &max);
	//CV_8UC1�ɕϊ��A�ő僌���W��0�`255�ɂ���
	disparity.convertTo(disparity_map, CV_8UC1, 255.0 / (max - min), -255.0 * min / (max - min));
	cv::imshow("result", disparity_map);
	cv::waitKey(0);

	return 0;
}