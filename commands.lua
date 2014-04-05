-- This file is run before each TCP shell is handled to a user and can be
-- used to define new commands.

SPEED_NORMAL_D = 800
SPEED_CALAGE_D = 100

SPEED_NORMAL_A = 4.85
SPEED_CALAGE_A = 1.

-- Standard flags to be used with wait_traj_end
TRAJ_FLAGS_SHORT_DISTANCE = END_TRAJ + END_ERROR + END_TIMER + END_BLOCKING
TRAJ_FLAGS_STD = TRAJ_FLAGS_SHORT_DISTANCE + END_OBSTACLE

function pos()
    x,y,a = position_get()
    print("("..x..";"..y..";"..a..")")
end

function wait_traj_end(why)
    repeat
        val = test_traj_end(why)
    until val ~= 0
    print("Got "..val)
    return val
end

function calage_test()
    trajectory_set_speed(SPEED_CALAGE_D, SPEED_CALAGE_A)
    mode("distance")
    forward(-2000)
    wait_traj_end(END_BLOCKING)
    
    val = rs_get_angle()

    bd_reset()

    mode("all")
--    trajectory_set_speed(SPEED_NORMAL_D, SPEED_NORMAL_A)
    forward(100)
    wait_traj_end(END_TRAJ)

    mode("distance")
    trajectory_set_speed(SPEED_CALAGE_D, SPEED_CALAGE_A)
    forward(-2000)
    wait_traj_end(END_BLOCKING)

    delta = val-rs_get_angle()

    print("Delta = "..delta)
end

function calage()
    bd_set_threshold("distance", 6000)
    bd_set_threshold("distance", 6000)
    trajectory_set_speed(SPEED_CALAGE_D, SPEED_CALAGE_A)
    mode("distance")
    forward(-2000)
    wait_traj_end(END_BLOCKING)
    mode("all")
end



function calibrate_wheels(count)
    trajectory_set_acc(160, 1.94)
    calage()
    

    start_angle = rs_get_angle()
    start_distance = rs_get_distance()

    forward(50)
    wait_traj_end(END_TRAJ)

    while count > 0 do
        forward(1150)
        wait_traj_end(END_TRAJ)

        turn(180)
        wait_traj_end(END_TRAJ)

        forward(1150)
        wait_traj_end(END_TRAJ)

        turn(-180)
        wait_traj_end(END_TRAJ)
        count = count-1
    end

    forward(-25)
    wait_traj_end(TRAJ_FLAGS_STD)

    calage()

    delta_a = start_angle - rs_get_angle()
    delta_d = start_distance - rs_get_distance()

    factor = delta_a / delta_d

    print("Correction factor : "..factor)
    rs_set_factor(factor)
end

function shoulder_pid(p, i, d)
    if p == nil or i == nil or d == nil then
        return
    end
    pid_set_gains(right_shoulder_pid, p, i, d)
    pid_set_gains(left_shoulder_pid, p, i, d)
end

function z_axis_pid(p, i, d)
    if p == nil or i == nil or d == nil then
        return
    end
    pid_set_gains(right_z_axis_pid, p, i, d)
    pid_set_gains(left_z_axis_pid, p, i, d)
end

function elbow_pid(p, i, d)
    if p == nil or i == nil or d == nil then
        return
    end
    pid_set_gains(right_elbow_pid, p, i, d)
    pid_set_gains(left_elbow_pid, p, i, d)
end

function hand_pid(p, i, d)
    if p == nil or i == nil or d == nil then
        return
    end
    pid_set_gains(right_hand_pid, p, i, d)
    pid_set_gains(left_hand_pid, p, i, d)
end


-- Finally greet the user if running in interactive mode
function greet()
    print("Wilkommen bei ReichOS 1.0 !")
    print("Die offizielle Scriptsprache des vierten Reichs !")
    print("---------------------------------------------------------------------------")
end

if __conn then
    greet()
end

