#include "main.h"
#include "srcMain.h"
#include <vector>

void initialize() {}

void disabled() {}

void competition_initialize() {}

void autonomous() {}

class Robot {
	private:
			MotorSet binded;
			pros::Controller master;
			bool tank_control;

			Robot(pros::controller_id_e_t controller_binding): master{ controller_binding } {
				tank_control = true;
			}

	public:
			Robot(MotorSet motor_set, pros::controller_id_e_t controller_binding): Robot(controller_binding) {
				// MotorSet instance
				binded = motor_set;
			}

			Robot(std::vector<int> left_motors_ports, std::vector<int> right_motors_ports, pros::controller_id_e_t controller_binding): Robot(controller_binding) {
				// MotorSet instant
				binded = MotorSet(left_motors_ports, right_motors_ports);
			}

			void tank_control() {
				binded.move_left_wheels(master.get_analog(ANALOG_LEFT_Y));
				binded.move_right_wheels(master.get_analog(ANALOG_RIGHT_Y));

				pros::delay(2);
			}

			void arcade_control() {
				int power = master.get_analog(ANALOG_LEFT_Y);
				int turn = master.get_analog(ANALOG_RIGHT_X);
				int left = power + turn;
				int right = power - turn;
				binded.move_left_wheels(left);
				binded.move_right_wheels(right);
			}

			void switch_control_type() {

			}
};

class MotorSet {
	private:
	 	std::vector<pros::Motor> left_motors;
		std::vector<pros::Motor> right_motors;

	public:
		MotorSet(std::vector<int> left_motors_ports, std::vector<int> right_motors_ports) {
			left_motors = std::vector<pros::Motor> {
				pros::Motor (left_motors_ports[0]),
				pros::Motor (left_motors_ports[1]),
				pros::Motor (left_motors_ports[2]),
			};

			right_motors = std::vector<pros::Motor> {
				pros::Motor (right_motors_ports[0]),
				pros::Motor (right_motors_ports[1]),
				pros::Motor (right_motors_ports[2]),
			};
		}		

		void move_left_wheels(int32_t voltage) {
			for (pros::Motor& motor : left_motors) {
				motor.move(voltage);
			}
		}

		void move_right_wheels(int32_t voltage) {
			for (pros::Motor& motor : left_motors) {
				motor.move(voltage);
			}
		}
};

void opcontrol() {
	MotorSet motors (ports::left_ports, ports::right_ports);
  	Robot robot (motors, CONTROLLER_MASTER);

	while (true) {
		robot.tank_control();

		pros::delay(2);
	}
}