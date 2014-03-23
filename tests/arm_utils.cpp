#include "CppUTest/TestHarness.h"
#include <cmath>

extern "C" {
#include "../arm_utils.h"
}

TEST_GROUP(ArmUtilsTestGroup)
{
};

TEST(ArmUtilsTestGroup, CoordinateRobotToArm)
{
    point_t target = {100,100};
    vect2_cart offset_xy = {0,100};
    float offset_angle = M_PI/2.;
    point_t result;

    result = arm_coordinate_robot2arm(target, offset_xy, offset_angle);
    DOUBLES_EQUAL(0., result.x, 1e-2);
    DOUBLES_EQUAL(-100., result.y, 1e-2);
}

TEST(ArmUtilsTestGroup, CoordinateTableToRobot)
{
    point_t target = {100,200};
    point_t robot_pos = {100, 100};
    float robot_a_rad = M_PI/2;
    point_t result;

    result = arm_coordinate_table2robot(target, robot_pos, robot_a_rad);

    DOUBLES_EQUAL(100., result.x, 1e-2);
    DOUBLES_EQUAL(0., result.y, 1e-2);

}

TEST(ArmUtilsTestGroup, CoordinateTableToRobot45Deg)
{
    point_t target = {100,100};
    point_t robot_pos = {0, 0};
    float robot_a_rad = M_PI/4;
    point_t result;

    result = arm_coordinate_table2robot(target, robot_pos, robot_a_rad);

    DOUBLES_EQUAL(sqrt(2)*100., result.x, 1e-2);
    DOUBLES_EQUAL(0., result.y, 1e-2);
}
