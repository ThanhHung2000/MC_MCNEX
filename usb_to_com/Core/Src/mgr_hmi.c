/*
 * mgr_hmi.c
 *
 *  Created on: Jan 25, 2026
 *      Author: Admin
 */
#include "motor.h"
#include "RS232.h"
#include "main.h"
#include "delay.h"
#include  "Common.h"
#include "mgr_hmi.h"
#include "drive.h"

#define STATUS_IDLE_OXIS     0x00U
#define STATUS_JOGGING_OXIS  0x01U
#define STATUS_STEP_OXIS     0x02U
#define STATUS_STOP_OXIS     0x03U
#define NUM_BUTTON_HOLD      0x06U
uint8_t Emergency = 0;
uint8_t Glass_group = 0;
#define HMI_STATE_BUTTON    0x01U
typedef struct {
    uint8_t last_state;    // Trạng thái Coil ở chu kỳ 1ms trước
    uint32_t press_timer;  // Bộ đếm thời gian (ms)
    uint8_t is_jogging;    // Cờ báo hiệu đã chuyển sang chế độ chạy liên tục
} HMI_Button_t;

HMI_Button_t hmi_btns[NUM_BUTTON_HOLD]; // Quản lý 6 nút

void Handle_Set(uint8_t data);

void Handle_Left(uint8_t data);
void Handle_Right(uint8_t data);
void Handle_In(uint8_t data);
void Handle_Out(uint8_t data);
void Handle_Up(uint8_t data);
void Handle_Down(uint8_t data);

void Save_Glass_1(uint8_t data);
void Save_Glass_2(uint8_t data);
void Save_Glass_3(uint8_t data);
void Save_Glass_All(uint8_t data);
void Save_Cover_1(uint8_t data);
void Save_Cover_2(uint8_t data);
void Save_Cover_3(uint8_t data);
void Save_Cover_All(uint8_t data);


void Reset(uint8_t data);

typedef void (*ActionHandler_t)(uint8_t );
typedef struct {
    uint16_t bitMask;
    ActionHandler_t handler;
} Map_t;

Map_t motorMoveTable[16] = {
    { 1 << 0, Handle_Left  },
    { 1 << 1, Handle_Right },
    { 1 << 2, Handle_In    },
    { 1 << 3, Handle_Out   },
    { 1 << 4, Handle_Up    },
	{ 1 << 5, Handle_Down  },
	{ 1 << 6, Handle_Set   },
	{ 1 << 7, Handle_Home  },

	{ 1 << 8, Save_Glass_1  },
	{ 1 << 9, Save_Glass_2  },
	{ 1 << 10, Save_Glass_3 },
	{ 1 << 11, Save_Cover_1  },
	{ 1 << 12, Save_Cover_2  },
	{ 1 << 13, Save_Cover_3  },
	{ 1 << 14, Save_Glass_All  },
	{ 1 << 15, Save_Cover_All  },
};

Map_t worker_controlTable[]={
	{ 1 << 3, Reset },
};
uint16_t FindActiveBit(uint8_t *data,uint8_t numbyte)
{
    for (uint8_t byteIdx = 0; byteIdx < numbyte; ++byteIdx)
    {
        uint8_t val = data[byteIdx];
        if (val) // chỉ xử lý nếu có ít nhất 1 bit được set
        {
        	int8_t fisrtbit = __builtin_ffs(val)-1;
        	return (byteIdx * 8 + fisrtbit);
        }
    }
    return 0xffff; // không có bit nào = 1
}
// dữ liệu đã lưu ở Coils_Database[1]; 0-5: trái 0x, phải Ox, lên 0x, xuống 0y, lên 0z, xuống 0z

void Task_scan_HMI()
{
	uint8_t current_state=0x00U;
	for(int i=0x00U; i< NUM_BUTTON_HOLD;i++)
	{
		current_state= ( (Control_motor->all) &(1<<i))==0x00U ? 0x00U : 0x1U;
		if (current_state == 0x1U)
		{
			// --- TRƯỜNG HỢP ĐANG NHẤN ---
			hmi_btns[i].press_timer++; // Mỗi lần gọi là 1ms, cứ thế cộng dồn lên

			// Nếu giữ đủ 500ms và chưa Jog thì kích hoạt Jog
			if (hmi_btns[i].press_timer >= 500 && hmi_btns[i].is_jogging == 0)
			{
				//MC_Jog(axis, direction, 10000, 5000);
				hmi_btns[i].is_jogging = 1;
				motorMoveTable[i].handler(STATUS_JOGGING_OXIS);

			}
		}
		else
		{
			// --- TRƯỜNG HỢP NHẢ TAY (current_state == 0) ---
			if (hmi_btns[i].last_state == 1)
			{ // Chỉ xử lý khi vừa mới nhả tay (Sườn xuống)

				if (hmi_btns[i].press_timer < 500) {
					// Nhấn nhả nhanh: Nhích 100 xung
					//MC_MoveRelative(axis, direction * 100, 5000, 2000);
					motorMoveTable[i].handler(STATUS_STEP_OXIS);
				}
				else {
					// Nhả sau khi đã Jog: Dừng trục
					//MC_Stop(axis, 5000);
					motorMoveTable[i].handler(STATUS_STOP_OXIS);
				}
				// Reset các biến để chuẩn bị cho lần nhấn sau
				hmi_btns[i].press_timer = 0;
				hmi_btns[i].is_jogging = 0;
			}
		}
		// Lưu trạng thái để bắt sườn xuống ở chu kỳ 1ms sau
		hmi_btns[i].last_state = current_state;
	}
}
void Task_Run_HMI(void)
{
	if(Emergency!=0) return;
	if(Tab->bits.Home == 1 && Home_Lamp->bits.Run == 0)
	{

	}
	else if (Tab->bits.Engine == 1)
	{
		uint16_t current = ((Save_Tray -> all) << 8) | (Control_motor->all);
		int8_t fisrtbit = __builtin_ffs(current)-1;
		if(fisrtbit >= NUM_BUTTON_HOLD)
		{
			motorMoveTable[fisrtbit].handler(1);
		}
	}
}
void Choose_glass_group(uint8_t num){
	if(num == 0) return;
//	Home_Lamp-> all &= ~(0x0fu << 2);
//	Home_Lamp-> all |= (num << 2);
//	Glass_group = __builtin_ffs(num)-1;
//	ClearBit(Lamp_glass_empty, 0, 196);
//	*Lamp_glass_select = 0;
//	for(uint16_t i = 0; i<196;i++){
//		if(Glass[Glass_group*196+i].State != OK){
//			SetBit(Lamp_glass_empty, i, 1);
//		}
//	}
}
// Nut sang trai Ox -> xa gốc tọa độ
void Handle_Left(uint8_t data)
{
	MC_MoveHandle(AXIT_X_ROBOT,data,0x01);
}
void Handle_Right(uint8_t data)
{
	MC_MoveHandle(AXIT_X_ROBOT,data, -1u);
}
void Handle_In(uint8_t data)
{
	MC_MoveHandle(AXIT_Y_ROBOT,data, -1u);
}
void Handle_Out(uint8_t data)
{
	MC_MoveHandle(AXIT_Y_ROBOT,data, 0x01);
}
void Handle_Up(uint8_t data)
{
	MC_MoveHandle(AXIT_Z_ROBOT,data, -1u);
}
void Handle_Down(uint8_t data)
{
	MC_MoveHandle(AXIT_Z_ROBOT,data, 0x01);
}

void Save_Glass_1(uint8_t data)
{

}
void Save_Glass_2(uint8_t data)
{

}
void Save_Glass_3(uint8_t data)
{

}
void Save_Glass_All(uint8_t data)
{

}
void Save_Cover_1(uint8_t data)
{

}
void Save_Cover_2(uint8_t data)
{

}
void Save_Cover_3(uint8_t data)
{

}
void Save_Cover_All(uint8_t data)
{

}
void Handle_Set(uint8_t data)
{
	MC_MoveLinear(Holding_Registers_Database[0],Holding_Registers_Database[1],Holding_Registers_Database[2],10000);
}
void Handle_Home(uint8_t data)
{

}
