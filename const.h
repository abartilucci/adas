#ifndef CONST_H
#define CONST_H

const char *NORMAL_MODE = "NORMALE";
const char *ARTIFICIAL_MODE = "ARTIFICIALE";

const char *U_PATH_P = "./park /dev/urandom";
const char *U_BIN_P = "./park ../data/urandomARTIFICIALE.binary";

const char *U_PATH_C = "./sur_cameras /dev/urandom";
const char *U_BIN_C = "./sur_cameras ../data/urandomARTIFICIALE.binary";

const char *R_PATH = " ../../../dev/random";
const char *R_BIN = "../data/randomARTIFICIALE.binary";


const char * PIPE_INPUT = "/tmp/input_pipe";
const char * PIPE_OUTPUT = "/tmp/output_pipe";
const char * PIPE_STEER = "/tmp/steer_pipe";
const char * PIPE_THROTTLE = "/tmp/throttle_pipe";
const char * PIPE_BRAKE = "/tmp/brake_pipe";
const char * PIPE_FRONT_W_CAMERA = "/tmp/front_w_camera_pipe";
const char * PIPE_PARK = "/tmp/park_pipe";
const char * PIPE_CAMERAS = "/tmp/cameras";

const char *INIZIO_PID = "../data/inizio_pid.data";
const char *STEER_PID = "../data/steer_pid.data";
const char *BRAKE_PID = "../data/brake_pid.data";
const char *CAMERA_PID = "../data/camera_pid.data";
const char *THROTTLE_PID = "../data/throttle_pid.data";
const char *HMI_OUTPUT_PID = "../data/hmi_output_pid.data";

const char *INPUT_DATA = "../data/frontCamera.data"; // un punto per ogni sovradirectory
const char *ECU_LOG = "../log/ECU.log";
const char *CAMERA_LOG = "../log/camera.log";
const char *THROTTLE_LOG = "../log/throttle.log";
const char *BRAKE_LOG = "../log/brake.log";
const char *STEER_LOG = "../log/steer.log";
const char *PARK_LOG = "../log/assist.log";
const char *SUR_CAMERAS_LOG = "../log/cameras.log";

#endif
