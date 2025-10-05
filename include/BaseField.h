#pragma once
#include <array>
#include <vector>
#include <cmath>

#include <fstream>
#include <iostream>
// #include "FieldContext.h"


/**
 * @class BaseField
 * @brief Represents a 2D grid field
 *
 * The BaseField class defines a rectangular grid in 2D space, specified either by the number of grid points
 * and coordinate bounds, or by a uniform grid size. It is intended for use in simulations that require
 * evaluating quantities (e.g., potential) at discrete points in space.
 *
 * @param Nx Number of grid points along the x-axis.
 * @param Ny Number of grid points along the y-axis.
 * @param x_min Minimum x-coordinate of the grid.
 * @param x_max Maximum x-coordinate of the grid.
 * @param y_min Minimum y-coordinate of the grid.
 * @param y_max Maximum y-coordinate of the grid.
 * @param N Number of grid points along each axis (for square grids).
 * @param size Physical size of the grid (for square grids).
 */
class BaseField {
    public:

        BaseField(int Nx, int Ny, double x_min, double x_max, double y_min, double y_max);
        BaseField(int N, double size);

        virtual ~BaseField() = default;    
        
        inline double getYCoord(int j) const { return m_y_min + j * m_dy; };
        inline double getXCoord(int i) const { return m_x_min + i * m_dx; };

        /**
         * @brief Returns the value of the field at a given point (x, y).
         * 
         * @param i Index of the point along the x-axis.
         * @param j Index of the point along the y-axis.
         * @return The value of the field at (x, y).
         */
        double valueAt(int i, int j) const;
        std::array<double, 2> gradientAt(double x, double y) const;

        // returns copy of underlying domain matrix
        std::vector<double> getDomain() const { return m_domain; }

        void exportCSV(const std::string& filename, int frame=0) const;

        void printInfo() const;

    protected:
        // Add member variables if needed
        const int m_Nx, m_Ny;
        const double m_x_min, m_x_max, m_y_min, m_y_max;
        const double m_dx, m_dy;
        std::vector<double> m_domain;

        template <typename T>
        static constexpr const T& clamp(const T& v, const T& lo, const T& hi) {
            return (v < lo) ? lo : (hi < v) ? hi: v;
        }


};