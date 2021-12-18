#ifndef CPPSTEG_LEASTBIT_HPP
#define CPPSTEG_LEASTBIT_HPP

#include "Conversions.hpp"
#include <bitset>
#include <exception>
#include <filesystem>
#include <iostream>
#include <map>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <string>
#include <vector>

namespace cppsteg {
namespace leastBit {

inline void manipulateBit(unsigned char &pixel, const char &bit) {
	std::string binConvertedValue = conv::decToBin(pixel);

	// manipulate last bit of binary number
	binConvertedValue[7] = bit;

	// change out color value
	pixel = conv::binToDec(binConvertedValue);
}

inline void encodeText(std::string imagePath, std::string outputPath, std::string input) {
	try {
		cv::Mat img = cv::imread(imagePath, -1);

		if (img.empty()) {
			throw std::invalid_argument("Could not open image");
		}

		// calculate maximum amount of bits to change and check if text fits into it
		int maxImgBits = img.rows * img.cols * img.channels();
		// check how many pixels must be manipulated to fit the text (+ 32 for the header)
		int neededBits = input.size() * 8 + 32;

		if (maxImgBits < neededBits) {
			throw std::length_error("Text does not fit in image");
		}

		int bitCounter = 0;

		// create header for hidden text and append bits of the letters
		std::string binConvertedText = conv::createHeader(input.length());
		binConvertedText += conv::textToBin(input);

		// iterate over image pixels
		for (int i = 0; i < img.rows; i++) {
			// if number of needed pixels is reached stop iterating
			if (bitCounter >= neededBits) {
				break;
			}
			for (int j = 0; j < img.cols; j++) {
				// if number of needed pixels is reached stop iterating
				if (bitCounter >= neededBits) {
					break;
				}
				// fetch RGB values of pixel
				cv::Vec4b &intensity = img.at<cv::Vec4b>(i, j);
				for (int k = 0; k < img.channels(); k++) {
					if (bitCounter >= neededBits) {
						break;
					}
					// convert color value to binary number
					manipulateBit(intensity.val[k], binConvertedText[bitCounter]);
					++bitCounter;
				}
			}
		}

		cv::imwrite(outputPath, img);
	} catch (const std::exception &e) { // reference to the base of a polymorphic object
		std::cout << e.what();          // information from length_error printed
	}
}

inline std::string decodeText(std::string imagePath) {
	cv::Mat img = cv::imread(imagePath, -1);

	if (img.empty()) {
		throw std::invalid_argument("Could not open image");
	}

	int bitCounter = 0;
	int maxConvertedBits = 0;
	int headerCounter = 0;
	std::string header = "";
	std::string bits = "";
	std::string fullText = "";
	int length = 0;

	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			if (maxConvertedBits > length) {
				break;
			}
			// if number of needed pixels is reached stop iterating
			cv::Vec4b &intensity = img.at<cv::Vec4b>(i, j);
			for (int k = 0; k < img.channels(); k++) {
				if (maxConvertedBits > length) {
					break;
				}
				std::string binConvertedValue = conv::decToBin(intensity.val[k]);
				// as long as 37 bits are not reached store all 37 bits in a string

				if (headerCounter == 32) {
					length = conv::decodeHeader(header) * 8;
					// std::cout << "filetype: " << jasdflkasfj["filetype"] << std::endl << "colorSpace: " <<
					// jasdflkasfj["colorSpace"] << std::endl << "length: " << jasdflkasfj["length"] << std::endl;
					++headerCounter;
				}

				if (headerCounter < 32) {
					header += binConvertedValue[7];
					++headerCounter;
				} else {
					// when header bits are fetched extract the normal text
					bits += binConvertedValue[7];
					++maxConvertedBits;
					++bitCounter;
					// if 8 bits were extracted convert them to a string
					if (bitCounter == 8) {
						fullText += conv::binToText(bits);
						bitCounter = 0;
						bits = "";
					}
				}
			}
		}
	}
	return fullText;
}

inline cv::Mat encodeText(const cv::Mat& input_img, std::string input) {
	try {
		cv::Mat output_img;
		input_img.copyTo(output_img);

		if (input_img.empty()) {
			throw std::invalid_argument("Could not open image");
		}

		// calculate maximum amount of bits to change and check if text fits into it
		int maxImgBits = input_img.rows * input_img.cols * input_img.channels();
		// check how many pixels must be manipulated to fit the text (+ 32 for the header)
		int neededBits = input.size() * 8 + 32;

		if (maxImgBits < neededBits) {
			throw std::length_error("Text does not fit in image");
		}

		int bitCounter = 0;

		// create header for hidden text and append bits of the letters
		std::string binConvertedText = conv::createHeader(input.length());
		binConvertedText += conv::textToBin(input);

		// iterate over image pixels
		for (int i = 0; i < input_img.rows; i++) {
			// if number of needed pixels is reached stop iterating
			if (bitCounter >= neededBits) {
				break;
			}
			for (int j = 0; j < input_img.cols; j++) {
				// if number of needed pixels is reached stop iterating
				if (bitCounter >= neededBits) {
					break;
				}
				// fetch RGB values of pixel
				cv::Vec4b& intensity = output_img.at<cv::Vec4b>(i, j);
				for (int k = 0; k < output_img.channels(); k++) {
					if (bitCounter >= neededBits) {
						break;
					}
					// convert color value to binary number
					manipulateBit(intensity.val[k], binConvertedText[bitCounter]);
					++bitCounter;
				}
			}
		}
		return output_img;
	} catch (const std::exception& e) { // reference to the base of a polymorphic object
		std::cout << e.what();          // information from length_error printed
	}
	return cv::Mat();
}

inline std::string decodeText(cv::Mat& img) {

	if (img.empty()) {
		throw std::invalid_argument("Could not open image");
	}

	int bitCounter = 0;
	int maxConvertedBits = 0;
	int headerCounter = 0;
	std::string header = "";
	std::string bits = "";
	std::string fullText = "";
	int length = 0;

	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			if (maxConvertedBits > length) {
				break;
			}
			// if number of needed pixels is reached stop iterating
			cv::Vec4b& intensity = img.at<cv::Vec4b>(i, j);
			for (int k = 0; k < img.channels(); k++) {
				if (maxConvertedBits > length) {
					break;
				}
				std::string binConvertedValue = conv::decToBin(intensity.val[k]);
				// as long as 37 bits are not reached store all 37 bits in a string

				if (headerCounter == 32) {
					length = conv::decodeHeader(header) * 8;
					// std::cout << "filetype: " << jasdflkasfj["filetype"] << std::endl << "colorSpace: " <<
					// jasdflkasfj["colorSpace"] << std::endl << "length: " << jasdflkasfj["length"] << std::endl;
					++headerCounter;
				}

				if (headerCounter < 32) {
					header += binConvertedValue[7];
					++headerCounter;
				} else {
					// when header bits are fetched extract the normal text
					bits += binConvertedValue[7];
					++maxConvertedBits;
					++bitCounter;
					// if 8 bits were extracted convert them to a string
					if (bitCounter == 8) {
						fullText += conv::binToText(bits);
						bitCounter = 0;
						bits = "";
					}
				}
			}
		}
	}
	return fullText;
}

} // namespace cppsteg::leastBit
}

#endif