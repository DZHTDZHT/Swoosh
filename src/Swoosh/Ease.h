#pragma once
#include <cmath>

namespace swoosh {
  namespace ease {
    static double pi = 3.14159265358979323846;

    template<typename T>
    static T radians(T degrees) { return static_cast<T>((static_cast<double>(degrees) * pi) / 180.0); }

    template<typename T>
    static T interpolate(T factor, T a, T b) {
      return a + ((b - a) * factor);
    }

    template<typename T>
    static T linear(T delta, T length, T power) {
      T normal = static_cast<T>(1.0 / static_cast<double>(length));

      T x = delta * normal;

      if (x >= 1) {
        x = 1;
      }

      T y = std::pow(x, power);

      return y;
    }

    /*
    y = (1 - abs(2-x*4) + 1)/2

    sharp back and forth, no easing
    */
    template<typename T>
    static T inOut(T delta, T length) {
      T normal = static_cast<T>(1.0 / static_cast<double>(length));

      T x = delta * normal;

      if (x >= 1) {
        x = 1;
      }

      T y = static_cast<T>((1.0 - std::fabs(2.0 - static_cast<double>(x) * 4.0) + 1.0) / 2.0);
      return y;
    }

    template<typename T>
    static T wideParabola(T delta, T length, T power) {
      T normal = static_cast<T>(2.0 / static_cast<double>(length));

      // Convert seconds elapsed to x values of 0 -> 2
      T x = delta * normal;

      // When x = 2, the parabola drops into the negatives
      // prevent that
      if (x >= 2) {
        x = 2;
      }

      // y = 1 - (x ^ 2 - 2x + 1) ^ n
      T poly = static_cast<T>((x*x) - (2.0 * static_cast<double>(x)) + 1.0);

      T y = static_cast<T>(1.0 - static_cast<double>(std::pow(poly, power)));

      return y;
    }

    /*
      y = 3x ^ 2 - 2x ^ 4

      overshoot destination and slide back at the end
    */
    template<typename T>
    static T bezierPopIn(T delta, T length) {
      T normal = static_cast<T>(1.0 / static_cast<double>(length));

      T x = delta * normal;

      if (x >= 1) {
        x = 1;
      }


      double part1 = static_cast<double>(3 * x * x);
      double part2 = static_cast<double>(2 * x * x * x * x);

      T y = static_cast<T>(part1 - part2);

      return y;
    }

    /*
    y = 3(1-x) ^ 2 - 2(1-x) ^ 4

    pop out and then slide out
  */
    template<typename T>
    static T bezierPopOut(T delta, T length) {
      T normal = static_cast<T>(1.0 / static_cast<double>(length));

      T x = delta * normal;

      if (x >= 1) {
        x = 1;
      }

      double x2 = static_cast<T>(1.0 - static_cast<double>(x));
      double part1 = 3 * x2 * x2;
      double part2 = 2 * x2 * x2 * x2 * x2;

      T y = static_cast<T>(part1 - part2);

      return y;
    }

    /*
      y = 1-(sin(x+90)*cos(-2x)
    */
    template<typename T>
    static T sinuoidBounceOut(T delta, T length) {
      T normal = static_cast<T>(3.0 / static_cast<double>(length));

      T x = delta * normal;

      if (x >= 3) {
        x = 3;
      }

      double y = 1.0 - (std::sin(static_cast<double>(x) + 90.0)*std::cos(-2.0 * static_cast<double>(x)));

      // Transform y into canonical [0,1] values
      return static_cast<T>(y/2.0);
    }
  }
}
