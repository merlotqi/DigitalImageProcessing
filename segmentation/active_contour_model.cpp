#include "active_contour_model.h"

#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace segmentation {

ActiveContourModel::ActiveContourModel(double alpha, double beta, double gamma, double kappa, int max_iterations)
    : alpha_(alpha), beta_(beta), gamma_(gamma), kappa_(kappa), max_iterations_(max_iterations)
{
}

void ActiveContourModel::initializeCircle(const Eigen::Vector2d &center, double radius, int num_points)
{
    snake_points_.clear();
    for (int i = 0; i < num_points; i++)
    {
        double theta = 2.0 * M_PI * i / num_points;
        double x = center.x() + radius * cos(theta);
        double y = center.y() + radius * sin(theta);
        snake_points_.push_back(Eigen::Vector2d(x, y));
    }
}

void ActiveContourModel::initializeRectangle(const Eigen::Vector2d &top_left, const Eigen::Vector2d &bottom_right,
                                             int points_per_side)
{
    snake_points_.clear();

    const double width = bottom_right.x() - top_left.x();
    const double height = bottom_right.y() - top_left.y();

    for (int i = 0; i < points_per_side; ++i)
    {
        double x = top_left.x() + i * width / points_per_side;
        double y = top_left.y();
        snake_points_.push_back(Eigen::Vector2d(x, y));
    }

    for (int i = 0; i < points_per_side; ++i)
    {
        double x = bottom_right.x();
        double y = top_left.y() + i * height / points_per_side;
        snake_points_.push_back(Eigen::Vector2d(x, y));
    }

    for (int i = 0; i < points_per_side; ++i)
    {
        double x = bottom_right.x() - i * width / points_per_side;
        double y = bottom_right.y();
        snake_points_.push_back(Eigen::Vector2d(x, y));
    }

    for (int i = 0; i < points_per_side; ++i)
    {
        double x = top_left.x();
        double y = bottom_right.y() - i * height / points_per_side;
        snake_points_.push_back(Eigen::Vector2d(x, y));
    }
}

void ActiveContourModel::computeImageGradient(const Image &image, Image &grad_x, Image &grad_y, Image &grad_mag)
{
    const int row = image.row();
    const int col = image.column();

    grad_x.resize(row, col);
    grad_y.resize(row, col);
    grad_mag.resize(row, col);

    // clang-format off
    
    Eigen::Matrix3d sobel_x;
    sobel_x << -1, 0, 1,
               -2, 0, 2,
               -1, 0, 1;

    Eigen::Matrix3d sobel_y;
    sobel_y << -1, -2, -1,
                0,  0,  0,
                1,  2,  1;

    // clang-format on

    for (int i = 1; i < row - 1; ++i)
    {
        for (int j = 1; j < col - 1; ++j)
        {
            double gx = 0.0;
            double gy = 0.0;
            for (int m = -1; m <= 1; ++m)
            {
                for (int n = -1; n <= 1; ++n)
                {
                    int row_idx = i + m;
                    int col_idx = j + n;
                    double pixel_val = static_cast<double>(image(row_idx, col_idx));

                    gx += pixel_val * sobel_x(m + 1, n + 1);
                    gy += pixel_val * sobel_y(m + 1, n + 1);
                }
            }

            grad_x(i, j) = static_cast<unsigned char>(std::abs(gx) / 8.0);
            grad_y(i, j) = static_cast<unsigned char>(std::abs(gy) / 8.0);
            grad_mag(i, j) = static_cast<unsigned char>(std::sqrt(gx * gx + gy * gy) / 8.0);
        }
    }
}

Eigen::Vector2d ActiveContourModel::computeExternalEnergy(const Image &grad_mag, const Image &grad_x,
                                                          const Image &grad_y, const Eigen::Vector2d &point)
{
    int x = static_cast<int>(std::round(point.x()));
    int y = static_cast<int>(std::round(point.y()));

    x = std::max(0, std::min(grad_mag.column() - 1, x));
    y = std::max(0, std::min(grad_mag.row() - 1, y));

    double fx = static_cast<double>(grad_x(y, x));
    double fy = static_cast<double>(grad_y(y, x));

    double magnitude = std::sqrt(fx * fx + fy * fy);
    if (magnitude > 1e-6)
    {
        fx /= magnitude;
        fy /= magnitude;
    }

    return Eigen::Vector2d(fx, fy);
}

Eigen::MatrixXd ActiveContourModel::buildStiffnessMatrix(int n)
{
    Eigen::MatrixXd A = Eigen::MatrixXd::Zero(n, n);

    for (int i = 0; i < n; ++i)
    {
        A(i, i) = 2 * alpha_ + 6 * beta_;
        A(i, (i + 1) % n) = -alpha_ - 4 * beta_;
        A(i, (i + 2) % n) = beta_;
        A(i, (i - 1 + n) % n) = -alpha_ - 4 * beta_;
        A(i, (i - 2 + n) % n) = beta_;
    }

    return A;
}

void ActiveContourModel::evolveSnake(const Image &image)
{
    int n = snake_points_.size();
    if (n < 3)
        return;

    Image grad_x, grad_y, grad_magnitude;
    computeImageGradient(image, grad_x, grad_y, grad_magnitude);

    Eigen::MatrixXd A = buildStiffnessMatrix(n);

    Eigen::MatrixXd system_matrix = Eigen::MatrixXd::Identity(n, n) + gamma_ * A;

    Eigen::FullPivLU<Eigen::MatrixXd> solver(system_matrix);

    for (int iter = 0; iter < max_iterations_; ++iter)
    {
        std::vector<Eigen::Vector2d> new_points(n);
        double max_move = 0.0;

        Eigen::VectorXd fx(n), fy(n);
        for (int i = 0; i < n; ++i)
        {
            Eigen::Vector2d external_force = computeExternalEnergy(grad_magnitude, grad_x, grad_y, snake_points_[i]);

            fx(i) = snake_points_[i].x() + gamma_ * external_force.x();
            fy(i) = snake_points_[i].y() + gamma_ * external_force.y();
        }

        Eigen::VectorXd new_x = solver.solve(fx);
        Eigen::VectorXd new_y = solver.solve(fy);

        for (int i = 0; i < n; ++i)
        {
            new_points[i] = Eigen::Vector2d(new_x(i), new_y(i));
            double move_distance = (new_points[i] - snake_points_[i]).norm();
            max_move = std::max(max_move, move_distance);
        }

        snake_points_ = new_points;

        if (max_move < kappa_)
        {
            break;
        }
    }
}

void ActiveContourModel::drawSnake(Image &image) const {}

}// namespace segmentation