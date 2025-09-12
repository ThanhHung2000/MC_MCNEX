#include "main.h"
//#include "modbus_master_rtu.h"
#include "modbus_slaver_comp.h"
#include "modbus_common.h"
//#include "modbus_master_handle.h"

#define ENABLE_MODBUS_MASTER


uint8_t modeModbusWork=MODBUS_SLAVE;

//uint8_t modeModbusWork = MODBUS_MASTER;

void ModbusComon_SetModeWork(uint8_t mode)
{
    modeModbusWork = mode;

    if(mode==MODBUS_SLAVE || mode == MODBUS_MASTER)
    {
        modeModbusWork=mode;
    }
    else
    {
        // không xử lí
    }
}



void ModbusCommon_Init(void)
{
//    ModbusMasterRTU_Begin();

    ModbusSlaverComp_Init();
//    ModbusMaster_RegisterSlaver();
}



void ModbusCommon_Run(void)
{

        ModbusSlaverComp_Run();

}
