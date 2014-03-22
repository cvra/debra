#ifndef _ARM_CINEMATICS_H_
#define _ARM_CINEMATICS_H_

#include <vect_base.h>

typedef enum {
    SHOULDER_FRONT,
    SHOULDER_BACK,
} shoulder_mode_t;

/** Computes the possible positions for the elbow.
 *
 * @param [in] x,y The coordinates of the hand.
 * @param [in] l1,l2 The length of the two parts of the arm.
 * @param [out] p1, p2 The possible positions.
 * @returns The number of possible positons (0, 1 or 2).
 */
int compute_possible_elbow_positions(point_t target, float l1, float l2, point_t *p1, point_t *p2);

/**Inverts the shoulder mode depending on wheter the arm is facing the
 * left or right side of the robot. This is needed as "forward" and "backward"
 * are in robot frame while the transformations should be applied in arm frame.
 */
shoulder_mode_t mode_for_orientation(shoulder_mode_t mode, float arm_angle_offset);

/** Choose the correct elbow solution to avoid colision with the robot and to
 * respect the given mode.
 * @note The mode is for angle_offset > 0. Use mode_for_orientation to change it
 * if needed.
 */
point_t choose_shoulder_solution(point_t target, point_t elbow1, point_t elbow2, shoulder_mode_t mode);

point_t arm_forward_cinematics(float alpha, float beta, float length[2]);

float compute_shoulder_angle(point_t elbow, point_t hand);
float compute_elbow_angle(point_t elbow, point_t hand);

#endif
