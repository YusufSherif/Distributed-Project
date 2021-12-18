//
// Created by Yusuf Sherif on 16/12/2021.
//

#ifndef PROJECT_2_INCLUDE_IMAGEOPERATIONS_HPP_
#define PROJECT_2_INCLUDE_IMAGEOPERATIONS_HPP_

#include "../libs/CppSteg/include/CppSteg/Steganography.hpp"

inline std::vector<uchar> open_image(const std::string &path) {
	cv::Mat image = cv::imread(path, -1);
	std::vector<uchar> buf;
	cv::imencode(".png", image, buf);
	return buf;
}

inline std::string encode_image(const std::vector<uchar> &img_buf) {
	std::string encoded = base64_encode(img_buf.data(), img_buf.size());
	std::cout << "Encode Size: " << encoded.size() << std::endl;
	return encoded;
}

inline std::vector<uchar> decode_image(const std::string &encoded) {
	std::string dec_jpg = base64_decode(encoded);
	std::cout << "Decoded Size: " << dec_jpg.size() << std::endl;
	std::vector<uchar> data(dec_jpg.begin(), dec_jpg.end());
	return data;
}

inline void save_image(const std::vector<uchar> &img_buf, const std::string &path) {
	std::vector<uchar> data(img_buf.begin(), img_buf.end());
	cv::Mat img = cv::imdecode(cv::Mat(data), -1);
	cv::imwrite(path, img);
}

#endif //PROJECT_2_INCLUDE_IMAGEOPERATIONS_HPP_
