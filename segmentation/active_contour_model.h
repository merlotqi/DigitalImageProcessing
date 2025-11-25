#pragma once

#include <Eigen/Dense>
#include <image.h>

namespace segmentation {


class ActiveContourModel
{
    int max_iterations_;
    double alpha_;
    double beta_;
    double gamma_;
    double kappa_;

    std::vector<Eigen::Vector2d> snake_points_;

public:
    ActiveContourModel(double alpha = 0.01, double beta = 0.1, double gamma = 0.1, double kappa = 0.1, int max_iterations = 1000);

    void initializeCircle(const Eigen::Vector2d &center, double radius, int num_points);

    void initializeRectangle(const Eigen::Vector2d &top_left, const Eigen::Vector2d &bottom_right, int points_per_side);

    void computeImageGradient(const Image &image, Image &grad_x, Image &grad_y, Image &grad_mag);

    Eigen::Vector2d computeExternalEnergy(const Image &grad_mag, const Image &grad_x, const Image &grad_y, const Eigen::Vector2d &point);

    Eigen::MatrixXd buildStiffnessMatrix(int n);

    void evolveSnake(const Image &image);

    void drawSnake(Image &image) const;

    const std::vector<Eigen::Vector2d> &snakePoints() const { return snake_points_; }

};

}// namespace segmentation
