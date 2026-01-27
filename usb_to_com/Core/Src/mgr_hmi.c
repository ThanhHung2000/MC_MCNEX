/*
 * mgr_hmi.c
 *
 *  Created on: Jan 25, 2026
 *      Author: Admin
 */

#include "RS232.h"
#include "main.h"
#include "delay.h"
#include  "Common.h"
#include "mgr_hmi.h"
#include "drive.h"
#include "modbusSlave.h"

int8_t fisrtbit=0x00U;
uint8_t Emergency = 0;
uint8_t Glass_group = 0;
uint8_t save=0x00U;
#define HMI_STATE_BUTTON    0x01U
typedef struct {
    uint8_t last_state;    // Trạng thái Coil ở chu kỳ 1ms trước
    uint32_t press_timer;  // Bộ đếm thời gian (ms)
    uint8_t is_jogging;    // Cờ báo hiệu đã chuyển sang chế độ chạy liên tục
} HMI_Button_t;

HMI_Button_t hmi_btns[NUM_BUTTON_HOLD]; // Quản lý 6 nút

void Handle_Set(void);
void Handle_Home(void);
void Handle_Left(uint8_t data);
void Handle_Right(uint8_t data);
void Handle_In(uint8_t data);
void Handle_Out(uint8_t data);
void Handle_Up(uint8_t data);
void Handle_Down(uint8_t data);
void Pick_handle1(void);
void Release_handle1(void);
void Pick_handle2(void);
void Release_handle2(void);
void Save_1(void);
void Save_2(void);
void Save_3(void);
void Setmove_point(void);

void Save_Glass_1(void);
void Save_Glass_2(void);
void Save_Glass_3(void);
void Save_Glass_All(void);
void Save_Cover_1(void);
void Save_Cover_2(void);
void Save_Cover_3(void);
void Save_Cover_All(void);

void Save_Tray_1(void);
void Save_Tray_2(void);
void Save_Tray_3(void);
void Save_Tray_All(void);



void Reset(void);

typedef void (*ActionHandler_t)(void);
typedef void (*ActionHandler_button)(uint8_t);
typedef struct {
    uint32_t bitMask;
    ActionHandler_t handler;
} Map_t;

typedef struct {
    ActionHandler_button handler;
} Map_button_t;

Map_button_t Handel_buttonTable[6] = {
	{ Handle_Left  },
	{ Handle_Right  },
	{ Handle_In  },
	{ Handle_Out  },
	{ Handle_Up },
	{ Handle_Down  }
};

Map_t motorMoveTable[19] = {

	{ 1 << 0, Handle_Set      },
	{ 1 << 1, Handle_Home     },
	{ 1 << 2, Pick_handle1    },
	{ 1 << 3, Release_handle1 },
	{ 1 << 4, Pick_handle2    },
	{ 1 << 5, Release_handle2 },
	{ 1 << 6, Save_1          },
	{ 1 << 7, Save_2          },
	{ 1 << 8, Save_3          },
	{ 1 << 9, Setmove_point   },
	{ 1 << 10, Save_Glass_1  },
	{ 1 << 11, Save_Glass_2  },
	{ 1 << 12, Save_Glass_3 },
	{ 1 << 13, Save_Cover_1  },
	{ 1 << 14, Save_Cover_2  },
	{ 1 << 15, Save_Cover_3  },
	{ 1 << 16, Save_Tray_1  },
	{ 1 << 17, Save_Tray_2  },
	{ 1 << 18,Save_Tray_3  },
};
void Init_hmi(void)
{
	Tab->bits.Motor=0x01U;
}
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
			if (hmi_btns[i].press_timer >= 1000 && hmi_btns[i].is_jogging == 0)
			{
				//MC_Jog(axis, direction, 10000, 5000);
				hmi_btns[i].is_jogging = 1;
				Handel_buttonTable[i].handler(STATUS_JOGGING_OXIS);
			}
		}
		else
		{
			// --- TRƯỜNG HỢP NHẢ TAY (current_state == 0) ---
			if (hmi_btns[i].last_state == 1)
			{ // Chỉ xử lý khi vừa mới nhả tay (Sườn xuống)

				if (hmi_btns[i].press_timer < 1000) {
					// Nhấn nhả nhanh: Nhích 100 xung
					//MC_MoveRelative(axis, direction * 100, 5000, 2000);
					Handel_buttonTable[i].handler(STATUS_STEP_OXIS);
				}
				else {
					// Nhả sau khi đã Jog: Dừng trục
					//MC_Stop(axis, 5000);
					Handel_buttonTable[i].handler(STATUS_STOP_OXIS);
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
	if(Tab->bits.Main == 1)// && Motor_Lamp->bits.Run == 0)
	{

	}
	else if (Tab->bits.Motor == 1)
	{
		uint32_t current = ((Save_Tray -> all) << 16)|(Worker_Control->all<< 8)| (Control_motor->all);
		fisrtbit = __builtin_ffs(current)-7;
		if(fisrtbit >= 0)
		{
			motorMoveTable[fisrtbit].handler();
		}
		Task_scan_HMI();
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
	MC_MoveHandle(AXIT_X_ROBOT,data,0x00);
}
void Handle_Right(uint8_t data)
{
	MC_MoveHandle(AXIT_X_ROBOT,data,0x01u);
}
void Handle_In(uint8_t data)
{
	MC_MoveHandle(AXIT_Y_ROBOT,data, 0x00u);
}
void Handle_Out(uint8_t data)
{
	MC_MoveHandle(AXIT_Y_ROBOT,data, 0x01u);
}
void Handle_Up(uint8_t data)
{
	MC_MoveHandle(AXIT_Z_ROBOT,data, 0x00u);
}
void Handle_Down(uint8_t data)
{
	MC_MoveHandle(AXIT_Z_ROBOT,data, 0x01);
}
void Pick_handle1(void)
{

}
void Release_handle1(void)
{

}
void Pick_handle2(void)
{

}
void Release_handle2(void)
{

}
void Save_1(void)
{
	switch(save)
	{
		case 0x01U :
		{
			Copy_Holding_Registers(0x00U,0x06);
			// BẬT LED LAMP LÊN BẰNG SET BIT
			Motor_Lamp->bits.ruber1_1=0x01U;
		}
		break;
		case 0x02U :
		{
			Copy_Holding_Registers(0x00U,0x08);
			Motor_Lamp->bits.ruber1_2=0x01U;
		}
		break;
		case 0x03U :
		{
			Copy_Holding_Registers(0x00U,0x0a);
			Motor_Lamp->bits.ruber1_3=0x01U;
		}
		break;
	}
	save=0x00;
	Reset_Tray(0x03);

}
void Save_2(void)
{
	switch(save)
	{
		case 0x04U :
		{
			Copy_Holding_Registers(0x00U,12);
			Motor_Lamp->bits.tray1_1=0x01U;
		}
		break;
		case 0x05U :
		{
			Copy_Holding_Registers(0x00U,14);
			Motor_Lamp->bits.tray1_2=0x01U;
		}
		break;
		case 0x06U :
		{
			Copy_Holding_Registers(0x00U,16);
			Motor_Lamp->bits.tray1_3=0x01U;
		}
		break;
	}
	save=0x00;
	Reset_Tray(0x03);
}
void Save_3(void)
{
	switch(save)
	{
		case 0x07U :
		{
			Copy_Holding_Registers(0x00U,18);
			Motor_Lamp->bits.tray2_1=0x01U;
			Reset_Tray(0x03);
		}
		break;
		case 0x08U :
		{
			Copy_Holding_Registers(0x00U,20U);
			Motor_Lamp->bits.tray2_2=0x01U;
			Reset_Tray(0x03);
		}
		break;
		case 0x09U :
		{
			Copy_Holding_Registers(0x00U,22U);
			Motor_Lamp->bits.tray2_3=0x01U;

			Reset_Tray(0x04);
		}
		break;
	}
	save=0x00;

}
void Setmove_point(void)
{
	switch(save)
	{
		case 0x01:
		{
			MC_MoveLinear(Get_Holding_Registers(6),Get_Holding_Registers(7),Holding_Registers_Database[2],10000);
			Reset_Tray(0x03);
		}
		break;
		case 0x02:
		{
			MC_MoveLinear(Get_Holding_Registers(8),Get_Holding_Registers(9),Holding_Registers_Database[2],10000);
			Reset_Tray(0x03);
		}
		break;
		case 0x03:
		{
			MC_MoveLinear(Get_Holding_Registers(10),Get_Holding_Registers(11),Holding_Registers_Database[2],10000);
			Reset_Tray(0x03);
		}
		break;
		case 0x04:
		{
			MC_MoveLinear(Get_Holding_Registers(12),Get_Holding_Registers(13),Holding_Registers_Database[2],10000);
			Reset_Tray(0x03);
		}
		break;
		case 0x05:
		{
			MC_MoveLinear(Get_Holding_Registers(14),Get_Holding_Registers(15),Holding_Registers_Database[2],10000);
			Reset_Tray(0x03);
		}
		break;
		case 0x06:
		{
			MC_MoveLinear(Get_Holding_Registers(16),Get_Holding_Registers(17),Holding_Registers_Database[2],10000);
			Reset_Tray(0x03);
		}
		break;
		case 0x07:
		{
			MC_MoveLinear(Get_Holding_Registers(18),Get_Holding_Registers(19),Holding_Registers_Database[2],10000);
			Reset_Tray(0x03);
		}
		break;
		case 0x08:
		{
			MC_MoveLinear(Get_Holding_Registers(20),Get_Holding_Registers(21),Holding_Registers_Database[2],10000);
			Reset_Tray(0x03);
		}
		break;
		case 0x09:
		{
			MC_MoveLinear(Get_Holding_Registers(22),Get_Holding_Registers(23),Holding_Registers_Database[2],10000);
			Reset_Tray(0x04);
		}
		break;
		default:
		break;
	}
	Reset_Tray(0x02);
	save=0x00U;
	Motor_Lamp->bits.led_move=0x01U;

}
void Save_Glass_1(void)
{
	// Lưu giá trị glass_1 7 và 8
	save=0x01;
}
void Save_Glass_2(void)
{
	// Lưu giá trị glass_2 vào flash và trên biến
	save=0x02;
}
void Save_Glass_3(void)
{
	// Lưu giá trị glass_3 vào flash và trên biến
	save=0x03;
}


void Save_Glass_All(void)
{
	// Lưu giá trị glass_2 vào flash và trên biến

}
void Save_Cover_1(void)
{
	// Lưu giá trị glass_2 vào flash và trên biến
	save=0x04;
}
void Save_Cover_2(void)
{
	// Lưu giá trị glass_2 vào flash và trên biến
	save=0x05;
}
void Save_Cover_3(void)
{
	// Lưu giá trị glass_2 vào flash và trên biến
	save=0x06;
}
void Save_Cover_All(void)
{
	// Lưu giá trị glass_2 vào flash và trên biến
}
void Save_Tray_1(void)
{
	save=0x07U;
}
void Save_Tray_2(void)
{

	save=0x08U;
}
void Save_Tray_3(void)
{

	save=0x09U;
}
void Handle_Set(void)
{
	MC_MoveLinear(Holding_Registers_Database[0],Holding_Registers_Database[1],Holding_Registers_Database[2],10000);

}
void Handle_Home(void)
{
	// đi về hôm với với quang đường tối đa là max hoặc chạm cảm biến rồi dừng

	// đi xa home 1 đoạn 10cm

	// lùi lại home với tần số 1Hz, để chạm home thì đó set current_pos =0x00U;
}
void  Reset(void)
{
}
