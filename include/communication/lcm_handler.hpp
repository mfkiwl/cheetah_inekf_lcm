#pragma once

//STL
#include <iostream>
#include <fstream>
#include <memory>
#include <stdlib.h>
#include <limits.h>
// Utility types
#include "utils/cheetah_data_t.hpp"
#include "sensor_msgs/Imu.h"
#include "yaml-cpp/yaml.h"

// ROS related
#include <ros/ros.h>
// #include "kinematics_handler.hpp"

// LCM related
#include <lcm/lcm-cpp.hpp>
// #include "lcm-types/cheetah_inekf_lcm/imu_t.hpp"
// #include "lcm-types/cheetah_inekf_lcm/legcontrol_t.hpp"
// #include "lcm-types/cheetah_inekf_lcm/contact_t.hpp"

#include "lcm-types/cheetah_inekf_lcm/microstrain_lcmt.hpp"
#include "lcm-types/cheetah_inekf_lcm/leg_control_data_lcmt.hpp"
#include "lcm-types/cheetah_inekf_lcm/contact_t.hpp"
#include "lcm-types/cheetah_inekf_lcm/synced_proprioceptive_lcmt.hpp"

// Threading
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/circular_buffer.hpp>

namespace cheetah_inekf_lcm {

class lcm_handler {
    public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    lcm_handler(lcm::LCM* lcm, ros::NodeHandle* n, cheetah_lcm_data_t* cheetah_buffer, boost::mutex* cdata_mtx);
    
    ~lcm_handler();

    void synced_msgs_lcm_callback(const lcm::ReceiveBuffer* rbuf,
                               const std::string& channel_name,
                               const synced_proprioceptive_lcmt* msg);
    
    void receiveLegControlMsg(const lcm::ReceiveBuffer *rbuf,
                                        const std::string &chan,
                                        const leg_control_data_lcmt *msg);

    void receiveLegControlMsg_Fast(const lcm::ReceiveBuffer *rbuf,
                                        const std::string &chan,
                                        const leg_control_data_lcmt *msg);

    void receiveMicrostrainMsg(const lcm::ReceiveBuffer *rbuf,
                                           const std::string &chan,
                                           const microstrain_lcmt *msg);

    void receiveMicrostrainMsg_Fast(const lcm::ReceiveBuffer *rbuf,
                                            const std::string &chan,
                                            const microstrain_lcmt *msg);         

    void receiveContactMsg(const lcm::ReceiveBuffer *rbuf,
                                                const std::string &chan,
                                                const contact_t *msg);                                                       
  private:
    lcm::LCM* lcm_;
    ros::NodeHandle* nh_;
    // ros::Publisher imu_publisher_;
    // ros::Publisher joint_state_publisher_;
    // ros::Publisher contact_publisher_;
    // ros::Publisher kinematics_publisher_;

    boost::mutex* cdata_mtx_;

    Eigen::Matrix<double,12,12> cov_encoders_;
    Eigen::Matrix<double,6,6> cov_prior_;

    uint64_t seq_imu_data_;
    uint64_t seq_joint_state_;
    uint64_t seq_contact_;

    cheetah_lcm_data_t* cheetah_buffer_;

    //Debugging
    bool debug_enabled_;
    std::ofstream kinematics_debug_;

    int64_t start_time_; //<! the starting time of the interface
    YAML::Node config_; //<! load interface config file
    
    /// Dimensions for sensor input:
    // leg_control_data:
    int q_dim;
    int qd_dim;
    int p_dim;
    int v_dim;
    int tau_est_dim;
    // microstrain:
    int acc_dim;
    int omega_dim;
    int quat_dim;
    int rpy_dim;
};

} // namespace mini_cheetah