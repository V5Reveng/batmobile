#include "main.h"
#include "srcMain.h"
#include <vector>
#include <algorithm>

enum class batmobile::ControlType
{
	Tank,
	Arcade,
};

std::string &config::get_control_scheme(const batmobile::ControlType &type)
{
	std::string cts = "";
	switch (type)
	{
	case batmobile::ControlType::Tank:
		cts = "Tank";
		break;
	case batmobile::ControlType::Arcade:
		cts = "Arcade";
		break;
	default:
		cts = "[ERR] ICTS";
		break;
	}

	return cts;
}

namespace batmobile
{
	class MotorSet
	{
	private:
		std::vector<int> left_motors;
		std::vector<int> right_motors;
		std::vector<int> arm_motors;

		std::vector<int> mobile_goal_motors;

	public:
		MotorSet() {}

		MotorSet(std::vector<int> &left_motors_ports,
				 std::vector<int> &right_motors_ports, std::vector<int> &arm_motor_ports, std::vector<int> &mobile_goal_motors_ports)
		{
			left_motors = left_motors_ports;
			right_motors = right_motors_ports;
			arm_motors = arm_motor_ports;
			mobile_goal_motors = mobile_goal_motors_ports;
		}

		void move_left_wheels(int32_t velocity)
		{
			for (int &port_num : left_motors)
			{
				// Uses C lib
				pros::c::motor_move_velocity(port_num, -velocity * config::conversion);
			}
		}

		void move_right_wheels(int32_t velocity)
		{
			for (int &port_num : right_motors)
			{
				pros::c::motor_move_velocity(port_num, velocity * config::conversion);
			}
		}

		void move_lift(int32_t velocity)
		{
			for (int &port_num : arm_motors)
			{
				pros::c::motor_move_velocity(port_num, velocity);
			}
		}

		// Taken from Matt Fellenz
		void move_mobile_goal_lift(double angle, int32_t velocity)
		{
			angle = std::clamp(angle, config::closed_position, config::open_position);

			for (int &port_num : mobile_goal_motors)
			{
				pros::c::motor_move_absolute(port_num, angle, velocity);
			}
		}

		double mobile_goal_position() const
		{
			return pros::c::motor_get_position(mobile_goal_motors[0]);
		}
	};
	class Robot
	{
	private:
		MotorSet binded;
		pros::Controller master{pros::Controller(CONTROLLER_MASTER)};

	public:
		ControlType control_type = ControlType::Tank;

		Robot(MotorSet &motor_set, pros::controller_id_e_t controller_binding)
		{
			binded = motor_set;
			master = pros::Controller(controller_binding);
		}

		Robot(std::vector<int> &left_motors_ports, std::vector<int> &right_motors_ports, std::vector<int> &arm_motor_ports,
			  std::vector<int> &mobile_goal_ports, pros::controller_id_e_t controller_binding)
		{
			binded = MotorSet(left_motors_ports, right_motors_ports, arm_motor_ports, mobile_goal_ports);
			master = pros::Controller(controller_binding);
		}

		void lift_control(int32_t velocity)
		{
			if (master.get_digital(DIGITAL_L1))
			{
				binded.move_lift(velocity);
			}
			else if (master.get_digital(DIGITAL_L2))
			{
				binded.move_lift(-velocity);
			}
			else
			{
				binded.move_lift(0);
			}
		}

		void mobile_goal_control(int32_t velocity)
		{
			static double saved_position = binded.mobile_goal_position();

			if (master.get_digital(DIGITAL_R1))
			{
				binded.move_mobile_goal_lift(saved_position, velocity);
				saved_position = binded.mobile_goal_position();
			}
			else if (master.get_digital(DIGITAL_R2))
			{
				binded.move_mobile_goal_lift(saved_position, -velocity);
				saved_position = binded.mobile_goal_position();
			}
			else
			{
				if (saved_position > config::closed_threshold)
				{
					binded.move_mobile_goal_lift(saved_position, config::MOTOR_MAX_VOLTAGE);
				}
				else
				{
					binded.move_mobile_goal_lift(saved_position, 0);
				}
			}
		}

		/* This method of movement is now deprecated, as any outreach event
  requiring the kicker (the messi) is now over */
		void kick_control()
		{
			if (master.get_digital(DIGITAL_R1))
			{
				pros::c::motor_move(ind_ports::messi, config::max_speed_v);
			}
			else
			{
				pros::c::motor_move(ind_ports::messi, 0);
			}
		}

		void tank_control()
		{
			binded.move_left_wheels(master.get_analog(ANALOG_LEFT_Y));
			binded.move_right_wheels(master.get_analog(ANALOG_RIGHT_Y));
		}

		void arcade_control()
		{
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
		void selected_control()
		{
			switch (control_type)
			{
			case ControlType::Arcade:
				return arcade_control();
			case ControlType::Tank:
				return tank_control();
			}
		}

		void switch_control_type()
		{
			switch (control_type)
			{
			case ControlType::Arcade:
				control_type = ControlType::Tank;
				break;
			case ControlType::Tank:
				control_type = ControlType::Arcade;
				break;
			}
		}

		pros::Controller &get_controller() { return master; }

		MotorSet &get_motor_set() { return binded; }

		batmobile::ControlType &get_control_type() { return control_type; }
	};

	class ControllerScreen
	{
	private:
		pros::Controller master{pros::Controller(CONTROLLER_MASTER)};
		int count = 0;

	public:
		ControllerScreen() {}

		ControllerScreen(pros::controller_id_e_t controller_binding)
			: master{controller_binding} {}

		ControllerScreen(pros::Controller controller) : master(controller) {}

		void draw(int row, int column, std::string &text)
		{
			master.set_text(row, column, text);
			pros::delay(50);
		}

		void draw(std::string &text)
		{
			master.set_text(0, 0, text);
			pros::delay(50);
		}
	};
}; // namespace batmobile

batmobile::MotorSet motors(ports::left_ports, ports::right_ports, ports::arm_ports, ports::mobile_goal_ports);
batmobile::Robot bat(motors, CONTROLLER_MASTER);
batmobile::ControllerScreen lcd(bat.get_controller());

void display()
{
	lcd.draw("D(Y): " + config::get_control_scheme(bat.get_control_type()));
}

void initialize()
{
	pros::Task display_task(display);
}

void disabled() {}

void competition_initialize() {}

void autonomous() {}

void opcontrol()
{
	while (true)
	{
		bat.selected_control();
		bat.kick_control();
		bat.mobile_goal_control(100);
		bat.lift_control(100);

		if (bat.get_controller().get_digital(DIGITAL_Y))
		{
			bat.switch_control_type();
		}

		pros::delay(2);
	}
}
