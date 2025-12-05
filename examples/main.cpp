#include <dft.h>
#include <histogram_equalization.h>
#include <image_inversion.h>
#include <laplacian.h>
#include <marr_hildreth.h>
#include <prewitt.h>
#include <roberts.h>
#include <sobel.h>

#include <active_contour_model.h>

#include <harris_detector.h>


void acm()
{
    Image augustusOctavian = read_image(IMAGE_DATA_PATH("/augustusOctavian.png"));
    segmentation::ActiveContourModel acm;
    acm.initializeCircle(Eigen::Vector2d(400, 400), 100, 100);
    acm.evolveSnake(augustusOctavian);

    const std::vector<Eigen::Vector2d> &snake = acm.snakePoints();

    cv::Mat mat = to_cv_mat(augustusOctavian);

    for (const auto &p: snake)
    {
        cv::circle(mat, cv::Point(p.x(), p.y()), 1, cv::Scalar(0, 0, 255), 1);
    }

    cv::imwrite(IMAGE_OUTPUT_PATH("active_contour_model.png"), mat);
}

void harris()
{
    Image img = read_image(IMAGE_DATA_PATH("/check.png"));
    primitive::HarrisDetector harris;
    cv::Mat mt = harris.detect(img);
    cv::imwrite(IMAGE_OUTPUT_PATH("harris.png"), mt);
}

void harris_opencv()
{
    cv::Mat image = cv::imread(IMAGE_DATA_PATH("/check.png"), cv::IMREAD_COLOR);

    cv::Mat result1;
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    
    cv::Mat harris_response;
    cornerHarris(gray, harris_response, 2, 3, 0.04);
    
    cv::Mat result_opencv = image.clone();
    cv::normalize(harris_response, harris_response, 0, 255, cv::NORM_MINMAX, CV_32FC1,cv:: Mat());
    for (int i = 0; i < harris_response.rows; i++) {
        for (int j = 0; j < harris_response.cols; j++) {
            if ((int)harris_response.at<float>(i, j) > 120) {
                circle(result_opencv, cv::Point(j, i), 3, cv::Scalar(0, 0, 255), 1);
            }
        }
    }

    cv::imwrite(IMAGE_OUTPUT_PATH("harris_opencv.png"), result_opencv);
}

int main()
{
    Image img = read_image(IMAGE_DATA_PATH("/lena.png"));

    write_image(IMAGE_OUTPUT_PATH("reberts.png"), segmentation::reberts(img));
    write_image(IMAGE_OUTPUT_PATH("prewitt.png"), segmentation::prewitt(img));
    write_image(IMAGE_OUTPUT_PATH("sobel.png"), segmentation::sobel(img));
    write_image(IMAGE_OUTPUT_PATH("laplacian_4.png"), segmentation::laplacian(img, 4));
    write_image(IMAGE_OUTPUT_PATH("laplacian_8.png"), segmentation::laplacian(img, 8));
    write_image(IMAGE_OUTPUT_PATH("laplacian_20.png"), segmentation::laplacian(img, 20));
    write_image(IMAGE_OUTPUT_PATH("marrHildreth_20_2_3.png"), segmentation::marrHildreth(img, 20, 2.0, 3));

    harris();
    harris_opencv();
    return 0;
}
