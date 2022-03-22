#include "main.h"
#include "srcMain.h"
#include <vector>

void initialize() {}

void disabled() {}

void competition_initialize() {}

void autonomous() {}

namespace batmobile {
	class Robot {
		private:
			MotorSet binded;
			pros::Controller master{ pros::Controller(CONTROLLER_MASTER) };
			bool tank_control_enabled;

		public:

			Robot(MotorSet motor_set, pros::controller_id_e_t controller_binding) {
				binded = motor_set;
				master = pros::Controller(controller_binding);
				tank_control_enabled = true;
			}

			Robot(std::vector<int> left_motors_ports, std::vector<int> right_motors_ports, pros::controller_id_e_t controller_binding) {
				binded = MotorSet(left_motors_ports, right_motors_ports);
				master = pros::Controller(controller_binding);
				tank_control_enabled = true;
			}

			void tank_control() {
				binded.move_left_wheels(master.get_analog(ANALOG_LEFT_Y));
				binded.move_right_wheels(master.get_analog(ANALOG_RIGHT_Y));
			}

			void arcade_control() {
				int power = master.get_analog(ANALOG_LEFT_Y);
				int turn = master.get_analog(ANALOG_RIGHT_X);
				int left = power + turn;
				int right = power - turn;
				binded.move_left_wheels(left);
				binded.move_right_wheels(right);
			}

			/**
			 * Uses the control scheme as mapped by the control_type.
			 * 1 - tank control
			 * 0 - arcade control
			 */
			void selected_control() {
				tank_control_enabled ? tank_control() : arcade_control();
			}

			void switch_control_type() {
				tank_control_enabled = !tank_control_enabled;
			}

			void set_control_type(int control_type) {
				if (control_type != 0 || control_type != 1) 
					throw std::invalid_argument("Control type must be either 0 or 1! Control type was: " + control_type);
				
				tank_control_enabled = control_type;
			}

			pros::Controller get_controller() {
				return master;
			}

			MotorSet get_motor_set() {
				return binded;
			}

			/**
			 * Gets the current control type.
			 * 0 - Tank Control
			 * 1 - Arcade Control
			 */
			int get_control_type() {
				return tank_control_enabled;
			}
	};

	class ControllerScreen {
		private:
			pros::Controller master{ pros::Controller(CONTROLLER_MASTER) };
			int count = 0;

		public:
			ControllerScreen() {}

			ControllerScreen(pros::controller_id_e_t controller_binding) {
				master = pros::Controller (controller_binding);
			}

			ControllerScreen(pros::Controller controller) {
				master = controller;
			}

			void draw(int row, int column, std::string text) {
				if(row < 0 || row > 2) 
					throw std::invalid_argument("Invalid bounds for row! Must be greater than zero (0) and less than or equal to two (2), was: " + row);
				if(column < 0 || column > 18)
					throw std::invalid_argument("Invalid bounds for row! Must be greater than zero (0) and less than or equal to eighteen (18), was: " + column);

				if (!(count % 25)) {
					master.set_text(row, column, text);
				}
				count++;
			}

			void draw(std::string text) {
				if (!(count % 25)) {
					master.set_text(0, 0, text);
				}
				count++;
			}
	};

	class MotorSet {
		private:
			std::vector<pros::Motor> left_motors;
			std::vector<pros::Motor> right_motors;

		public:
			MotorSet() {}

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
};

void opcontrol() {
	batmobile::MotorSet motors (ports::left_ports, ports::right_ports);
  	batmobile::Robot batmobile (motors, CONTROLLER_MASTER);
	batmobile::ControllerScreen lcd (batmobile.get_controller());


	while (true) {
		batmobile.selected_control();

		if(batmobile.get_controller().get_digital(DIGITAL_Y)) {
			batmobile.switch_control_type();
			lcd.draw("C (Y): " + batmobile.get_control_type() ? "Tank" : "Arcade");
		}

		pros::delay(2);
	}
}