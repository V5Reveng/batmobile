#ifndef SRC_MAIN
#define SRC_MAIN

namespace batmobile {
    class MotorSet;
    class Robot;
    class ControllerScreen;
}

namespace config {
  constexpr int max_speed_v = -85;
  constexpr double closed_position = 0.0;
  constexpr double closed_threshold = 20.0;
  constexpr auto MOTOR_MAX_VOLTAGE = 127;

  constexpr double open_position = 0.0;

  constexpr auto conversion = 200.0/127.0;
}

namespace ports {
    namespace individual {
        // Change these depending on ports
        constexpr int left_ports_top = 20;
        constexpr int left_ports_bottom = 17;

        constexpr int right_ports_top = 19;
        constexpr int right_ports_bottom = 18;

        // Add arm ports depending on amount of ports
        // Add mobile goal ports depending on amount of parts 


        constexpr int messi = 16;
    }

    namespace ind_ports = ports::individual;

    std::vector<int> left_ports = std::vector<int> {ind_ports::left_ports_top, ind_ports::left_ports_bottom};
    std::vector<int> right_ports = std::vector<int> {ind_ports::right_ports_top, ind_ports::right_ports_bottom};

    std::vector<int> arm_ports = std::vector<int> {};
    std::vector<int> mobile_goal_ports = std::vector<int> {};
}

namespace ind_ports = ports::ind_ports;

#endif