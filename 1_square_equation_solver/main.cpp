#include <iostream>
#include <cassert>
#include <cmath>

//#define TEST

//constants
#define INF_ROOTS (-1)
#define EPSILON 0.0000000001

//font color
#define RESET   "\033[0m"
#define RED     "\033[1;31m"

//asserts
#define ASSERT_EQ(x, y) assert((x) == (y));
#define ASSERT_TRUE(x) assert((x));
#define ASSERT_NOT_EQ(x, y) assert((x) != (y));
#define ASSERT_EQ_PAIR(a, b, x, y) assert (((a) == (x) && (b) == (y)) || ((a) == (y) && (b) == (x)));

/*!
 * solves a linear equation of the form ax + b = 0
*/

int solve_linear(double a, double b, double* x)
{
    ASSERT_TRUE(std::isfinite(a));
    ASSERT_TRUE(std::isfinite(b));

    ASSERT_NOT_EQ(x, nullptr);

    //equals a == 0 && b == 0
    if (abs(a) < EPSILON && abs(b) < EPSILON)
    {
        return INF_ROOTS;
    }

    //equals a == 0
    if (abs(a) < EPSILON && b != 0) {
        return 0;
    }
    *x = -b / a;
    return 1;
}


/*!
 * solves a square equation of the form ax^2 + bx + c = 0
*/

int solve_square(double a, double b, double c,
                 double* x1, double* x2)
{
    ASSERT_TRUE(std::isfinite(a));
    ASSERT_TRUE(std::isfinite(b));
    ASSERT_TRUE(std::isfinite(c));

    ASSERT_NOT_EQ(x1, nullptr);
    ASSERT_NOT_EQ(x2, nullptr);
    ASSERT_NOT_EQ(x1, x2);

    //equals a == 0
    if (abs(a) < EPSILON)
    {
        int roots_quantity = solve_linear(b, c, x1);
        return roots_quantity;
    }

    double d = b*b - 4*a*c;

    //equals d == 0
    if (abs(d) < EPSILON)
    {
        *x1 = *x2 = -b / (2*a);
        return 1;
    }

    if (d < 0)
    {
        return 0;
    }

    *x1 = (-b + std::sqrt(d)) / (2*a);
    *x2 = (-b - std::sqrt(d)) / (2*a);
    return 2;
}

#ifndef TEST

int main(int argc, char *argv[])
{
    if (argc > 1) {
        if (static_cast<std::string>(argv[1]) == "-help") {
            printf("--------------------------------------\n");
            printf("for all questions t.me/nikitamikhaylov\n");
            printf("--------------------------------------\n");
            return 0;
        }
    }

    printf("hey! it's a square equation solver.\n");
    printf("please, enter a, b, c coefficients:\n");

    double a = 0, b = 0, c = 0;
    int operations_count = scanf("%lf %lf %lf", &a, &b, &c);

    if (operations_count != 3) {
        printf("%swarning :: %s", RED, RESET);
        printf("uncorrected input");
        return 1;
    }

    double x1 = 0, x2 = 0;
    int roots_quantity = solve_square(a, b, c, &x1, &x2);
    switch (roots_quantity)
    {
        case 0:
        {
            printf("no roots\n");
            break;
        }
        case 1:
        {
            printf("one root : %lf \n", x1);
            break;
        }
        case 2:
        {
            printf("two roots : %lf %lf \n", x1, x2);
            break;
        }
        case INF_ROOTS:
        {
            printf("any number \n");
            break;
        }
        default:
        {
            printf("%s error :: %s", RED, RESET);
            printf("roots quantity = %d", roots_quantity);
            return 1;
        }
    }
    return 0;
}

#endif

#ifdef TEST

int main() {
    double x1 = 0, x2 = 0;
    int roots_quantity;

    roots_quantity= solve_square(1, -4, 3, &x1, &x2);
    ASSERT_EQ(roots_quantity, 2);
    ASSERT_EQ_PAIR(x1, x2, 1, 3);
    printf("Test passed! \n");

    roots_quantity = solve_square(0, 0, 0, &x1, &x2);
    ASSERT_EQ(roots_quantity, INF_ROOTS);
    printf("Test passed! \n");

    roots_quantity = solve_square(1000000, 0, -0.000001, &x1, &x2);
    ASSERT_EQ(roots_quantity, 2);
    printf("Test passed! \n");

    roots_quantity = solve_square(0, 0, 1, &x1, &x2);
    ASSERT_EQ(roots_quantity, 0);
    printf("Test passed! \n");

    roots_quantity = solve_square(1, 0, 0, &x1, &x2);
    ASSERT_EQ(roots_quantity, 1);
    ASSERT_EQ_PAIR(x1, x2, 0, 0);
    printf("Test passed! \n");

    printf("All tests passed! \n");
}

#endif