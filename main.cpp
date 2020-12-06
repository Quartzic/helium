#include <iostream>
#include <opencv2/opencv.hpp>
#include <boost/program_options.hpp>
#include <filesystem> // C++17
#include "cascade_model.cpp"

using namespace boost::program_options;
using namespace cv;

namespace po = boost::program_options;
namespace fs = std::filesystem;

int main(int argc, char **argv) {

    std::vector<std::string> argList(argv, argv + argc);
    po::variables_map vm;
    if (argList[1] == "crop-faces") {
        try {
            // Get options
            po::options_description desc("Options for crop-faces");
            desc.add_options()("help", "produce help message")
                    ("input", po::value<std::string>()->required(), "input file")
                    ("output", po::value<std::string>()->required()->default_value("out"), "output folder")
                    ("model", po::value<std::string>()->default_value("internal"),
                     "model file ('internal' to use haarcascade_frontalface_alt2)")
                    ("zoom", po::value<float>()->default_value(2), "zoom factor (0.5 = half size, 2 = double size)")
                    ("min-size", po::value<float>()->default_value(0.05, "0.05"),
                     "minimum face size (0.05 = 5% of image size, 0.5 = 50% of image size)");
            po::store(po::parse_command_line(argc, argv, desc), vm);

            // Return help message
            if (vm.count("help")) {
                std::cout << desc << std::endl;
                return 1;
            }

            // Raise errors about incorrect options
            po::notify(vm);

            // Cast variables from  variable map
            const std::string inputPath = vm["input"].as<std::string>();
            const std::string modelPath = vm["model"].as<std::string>();
            const std::string outputPath = vm["output"].as<std::string>();
            const float zoomLevel = vm["zoom"].as<float>();
            const float minimumSize = vm["min-size"].as<float>();

            // Error if input path is not a file
            if (!fs::is_regular_file(inputPath)) {
                std::cerr << inputPath << " is not a file." << std::endl;
                return -1;
            }

            // Error if output path is not a directory
            if (!fs::is_directory(outputPath)) {
                std::cerr << outputPath << " is not a directory." << std::endl;
                return -1;
            }

            // Load the input image
            cv::Mat inputImage = cv::imread(inputPath);

            // Error if image is empty
            if (inputImage.empty()) {
                std::cerr << inputPath << " couldn't be read" << std::endl;
                return -1;
            }

            // Initialize cascade classifier
            CascadeClassifier cascade;

            if (modelPath == "internal") {
                // Open in-memory model
                cv::FileStorage fs;
                fs.open(std::string(cascade_model), FileStorage::READ | FileStorage::MEMORY);

                // Load model into cascade classifier
                cascade.read(fs.getFirstTopLevelNode());
            } else {
                // Error if model path is not a file
                if (!fs::is_regular_file(modelPath)) {
                    std::cerr << modelPath << " does not exist." << std::endl;
                    return -1;
                }

                // Load model into cascade classifier
                cascade.load(modelPath);
            }

            // The minimum size for detected faces is based on the larger of the image's sides multiplied by a ratio
            const int greatestImageSideLength = max(inputImage.size().height, inputImage.size().width);
            const int minimumFaceLength = (greatestImageSideLength * minimumSize);
            cv::Size minimumFaceSize = cv::Size(minimumFaceLength, minimumFaceLength);

            // Use the cascade model to detect faces as Rects in detectedFaces
            std::vector<Rect> detectedFaces;
            cascade.detectMultiScale(inputImage, detectedFaces, 1.1, 3, 0, minimumFaceSize);

            // Loop over every detected face
            int imageId = 1;
            for (const Rect &faceArea : detectedFaces) {

                // Set the crop rectangle to the detected face before performing zoom transformation
                Rect cropRectangle = faceArea;

                // Apply the zoom level
                cropRectangle.width = faceArea.width * zoomLevel;
                cropRectangle.height = faceArea.height * zoomLevel;
                cropRectangle.x -= (faceArea.width / zoomLevel);
                cropRectangle.y -= (faceArea.height / zoomLevel);

                // If the new crop rectangle would try to read nonexistent pixels, skip
                if ((cropRectangle.x < 0) || (cropRectangle.y < 0) ||
                    ((cropRectangle.x + cropRectangle.width) > inputImage.size().width) ||
                    ((cropRectangle.y + cropRectangle.height) > inputImage.size().height)) {
                    std::cerr << "Face " << faceArea << " could not be saved (zoom too small)" << std::endl;
                    continue;
                }

                // Write a cropped version of the image
                cv::imwrite((outputPath + "/" + std::to_string(imageId) + ".jpg"), inputImage(cropRectangle));

                imageId++;
            }
        }
        catch (std::exception &e) {
            std::cerr << "Error: " << e.what() << "\n";
            return -1;
        }
    } else {
        std::cout << "usage: helium <function>" << std::endl;
        std::cout << "crop-faces: creates cropped images of faces" << std::endl;
        return -1;
    }

    return 0;
}