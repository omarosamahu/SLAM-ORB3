// STL
#include <chrono>
#include <iostream>
#include<algorithm> 
// Opencv
#include <opencv2/opencv.hpp>
// Eigen 


void pose_estimation_2d2d(std::vector<cv::KeyPoint> keypoints_1, std::vector<cv::KeyPoint> keypoints_2,
	std::vector<cv::DMatch> matches,cv::Mat R, cv::Mat T) {
	// Note Convert to eigen 
	//cv::Mat K = (cv::Mat_<double>(3, 3) << 520.9, 0, 325.1, 0, 521.0, 249.7, 0, 0, 1);
	// Convert the matching points to form the vector 
	std::vector<cv::Point2d> points1;
	std::vector<cv::Point2d> points2;
	//
	for (size_t i = 0; i < matches.size(); i++)
	{
		points1.push_back(keypoints_1[matches[i].queryIdx].pt);
		points2.push_back(keypoints_2[matches[i].trainIdx].pt);
	}
	// Calculate Fundamental matrix
	cv::Mat fundamentalMatrix;
	fundamentalMatrix = cv::findFundamentalMat(points1, points2, CV_FM_8POINT);
	std::cout << "Fundamental Matrix is:\n" << fundamentalMatrix << "\n";
	// Calculate Essential Matrix
	cv::Mat essential_matrix;
	cv::Point2d principal_point(325.1, 249.7);
	double focal_length = 521;
	essential_matrix = cv::findEssentialMat(points1, points2, focal_length, principal_point);
	std::cout << "Essential Matrix is:\n" << essential_matrix << "\n";
	// Recover Rotation and translation from essential matrix
	cv::recoverPose(essential_matrix, points1, points2, R, T, focal_length, principal_point);
	std::cout << "R is " << std::endl << R << std::endl;
	std::cout << "T is " << std::endl << T << std::endl;
}
int main() {
	std::cout << "ORB Feature Extractor ...........\n";
	// Load images 
	auto img1 = cv::imread("E:\\SLAM_Book\\projects\\NonLinear_optimization\\src\\FeatureExtraction\\resources\\1.png",cv::IMREAD_COLOR);
	auto img2 = cv::imread("E:\\SLAM_Book\\projects\\NonLinear_optimization\\src\\FeatureExtraction\\resources\\1.png",cv::IMREAD_COLOR);
	// Initialization
	// Define your keypoints for each image
	std::vector<cv::KeyPoint> keypoints_1, keypoints_2;
	cv::Mat descriptor_1, descriptor_2;
	// Define your Feature Detector
	cv::Ptr<cv::FeatureDetector> detector = cv::ORB::create();
	// Define your Desciptor
	cv::Ptr<cv::DescriptorExtractor> descriptor = cv::ORB::create();
	// Define your Matcher
	cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create("BruteForce-Hamming");
	// Calculate Key points of each image
	std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
	detector->detect(img1, keypoints_1);
	detector->detect(img2, keypoints_2);
	// Calculate BRIEF descriptors
	descriptor->compute(img1, keypoints_1, descriptor_1);
	descriptor->compute(img2, keypoints_2, descriptor_2);
	std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();

	auto time_diff = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	std::cout << "Extract ORB cost: " << time_diff.count() << "Sec"<< "\n";

	cv::Mat outimg1;
	cv::drawKeypoints(img1, keypoints_1, outimg1);
	cv::imshow("KeyPoints", outimg1);
	/****************** Matching ********************/
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	std::vector<cv::DMatch> matches;
	matcher->match(descriptor_1, descriptor_2, matches);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	auto time_elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
	std::cout << "Matching time: " << time_elapsed.count() << "Sec"<< "\n";
	/**********************************************/
	auto minmax = std::minmax_element(matches.begin(), matches.end(), [](const cv::DMatch& m1, const cv::DMatch& m2) {
		return m1.distance < m2.distance; });
	
	double minDist = minmax.first->distance;
	double maxDist = minmax.second->distance;
	std::vector<cv::DMatch> goodMatches;
	for (size_t i = 0; i < descriptor_1.rows; i++)
	{
		if (matches[i].distance <= std::max(2 * minDist, 30.0)) {
			goodMatches.push_back(matches[i]);
		}
	}
	/*************************************************************/
	std::cout << "******Pose Estimation******\n";
	t1 = std::chrono::steady_clock::now();
	cv::Mat R, T;
	pose_estimation_2d2d(keypoints_1, keypoints_2, matches,R,T);
	t2 = std::chrono::steady_clock::now();
	auto time_takes= std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
	std::cout << "Pose Estimation time takes: " << time_takes.count() << "\n";
	/*************************************************************/
	/*cv::Mat imgMatch;
	cv::Mat imgGoodMatches;
	
	cv::drawMatches(img1, keypoints_1, img2, keypoints_2, matches, imgMatch);
	cv::drawMatches(img1, keypoints_1, img2, keypoints_2, goodMatches, imgGoodMatches);
	
	cv::imshow("Default Match", imgMatch);
	cv::imshow("Good Match", imgGoodMatches);

	std::cout << "Good Matches size: " << goodMatches.size() << "\n";
	std::cout << "Default Matches size: " << matches.size() << "\n";
	*/
	cv::waitKey(0);
}