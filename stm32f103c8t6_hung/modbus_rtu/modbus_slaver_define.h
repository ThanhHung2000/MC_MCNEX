#ifndef _MODBUS_SLAVER_DEFINE_H_
#define _MODBUS_SLAVER_DEFINE_H_

// ====================================  Holding register ===========

#define ADR_REG_HOLDING_APPLICATION_CONFIG 0 //

#define ADR_REG_HOLDING_ENGINE_OIL_PRESSURE_CFG 5
#define ADR_REG_HOLDING_ENGINE_COOLANT_TEMPERATURE_CFG 6
#define ADR_REG_HOLDING_ENGINE_FUEL_LEVEL_CFG 7
#define ADR_REG_HOLDING_ENGINE_OPTION_CFG 8
#define ADR_REG_HOLDING_ENGINE_CRANKING_CFG 9
#define ADR_REG_HOLDING_ENGINE_SPEED_SETTING_CFG 10
#define ADR_REG_HOLDING_ENGINE_PLANT_BATTERY_CFG 11
#define ADR_REG_HOLDING_ENGINE_DC_LOW 12

#define ADR_REG_HOLDING_GENERATOR_CFG 15

#define ADR_REG_HOLDING_IN_ANALOG_A_CFG 20
#define ADR_REG_HOLDING_IN_ANALOG_B_CFG 21
#define ADR_REG_HOLDING_IN_ANALOG_C_CFG 22
#define ADR_REG_HOLDING_IN_ANALOG_D_CFG 23

#define ADR_REG_HOLDING_IN_DIGITAL_CFG 25

#define ADR_REG_HOLDING_MAINS_CFG 30

#define ADR_REG_HOLDING_MAINTENANCE_CFG 35

#define ADR_REG_HOLDING_MODULE_CFG 40

#define ADR_REG_HOLDING_OUTPUT_DIGITAL_CFG 45

#define ADR_REG_HOLDING_TIMER_CFG 50

#define ADR_REG_HOLDING_SCHEDULER_CFG 55


#define ADR_REG_HOLDING_RESET 80 // lệnh reset sau khi cài đặt


#define ADR_REG_HOLDING_UPDATE_REALTIME 100 // cap nhật lại thời gian thực
#define ADR_REG_HOLDING_VERSION 101 // phiên bản phần mềm
#define ADR_REG_HOLDING_TEST_RELAY 105 // lệnh điều khiển test relay
#define ADR_REG_HOLDING_BTN_STOP 110 // lệnh điều khiển tương đương nút stop
#define ADR_REG_HOLDING_BTN_MANUAL 111 // lệnh điềuk khiển tương đương nút manual
#define ADR_REG_HOLDING_BTN_AUTO 112 // lệnh điều khiển tương đương nút auto
#define ADR_REG_HOLDING_BTN_TEST 113
#define ADR_REG_HOLDING_BTN_MENU 114
#define ADR_REG_HOLDING_BTN_START 115
#define ADR_REG_HOLDING_BTN_CLOSE_MAIN 116
#define ADR_REG_HOLDING_BTN_CLOSE_GEN 117

#define ADR_REG_HOLDING_FORMAT_EVENT_LOG 120 // lệnh xóa event logs
#define ADR_REG_HOLDING_TEST_EVENT_LOG 121 // lệnh test ghi event log

#define ADR_REG_HOLDING_RESET_HOUR_RUN 130          // xóa thời gian chạy máy
#define ADR_REG_HOLDING_RESET_NO_OF_START 131       // lệnh xóa số lần đè
#define ADR_REG_HOLDING_RESET_ENERGY_ACTIVE 132     // lệnh xóa tổng năng lượng tiêu thụ
#define ADR_REG_HOLDING_RESET_ENERGY_REACTIVE 133   // lệnh xóa tổng năng lượng phản kháng
#define ADR_REG_HOLDING_RESET_ENERGY_APP 134        // lệnh xóa tổng năng lượng toàn phần

#define ADR_REG_HOLDING_GEN_SECURE 140              // lệnh cài đặt mật khẩu truy cập

#define ADR_REG_HOLDING_SYSTEM_CALI_CONFIG 160      // lệnh lưu thông số cấu hình hệ thống


// =========================== Input Register Command ===================//
#define ADR_REG_INPUT_READ_GENERATOR 0      // đọc thông số máy phát điện
#define ADR_REG_INPUT_READ_MAINS 5          // đọc thông lưới điện
#define ADR_REG_INPUT_SENSOR_IO 10          // đọc thông số sensor io

#define ADR_REG_INPUT_EVENT_LOG_0_15 20     // đọc sự kiện 0->15
#define ADR_REG_INPUT_EVENT_LOG_15_30 21    // đọc sự kiện 15->30
#define ADR_REG_INPUT_EVENT_LOG_30_45 22    // đọc sự kiện 30->45
#define ADR_REG_INPUT_EVENT_LOG_45_60 23
#define ADR_REG_INPUT_EVENT_LOG_60_75 24
#define ADR_REG_INPUT_EVENT_LOG_75_90 25

// =========================== Write Single Coil ===================// 0-15 status on/off
// đăng code tăt bật Led pa3,pa4,pa5,pa6,pa7,pb0 ; tương ứng LEDUV1, LEDUV2, LEDUV3, FAN QUATER
#define ADR_REG_COIL_0                      0
#define ADR_REG_COIL_1                      1
#define ADR_REG_COIL_2                      2
#define ADR_REG_COIL_3                      3
#define ADR_REG_TOTALCOIL                   4
#endif
