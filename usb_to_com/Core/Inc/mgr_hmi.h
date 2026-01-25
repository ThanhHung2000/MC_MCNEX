/*
 * mgr_hmi.h
 *
 *  Created on: Jan 25, 2026
 *      Author: Admin
 */

#ifndef INC_MGR_HMI_H_
#define INC_MGR_HMI_H_

void Task_scan_HMI(void);
void Task_Run_HMI(void);
void Handle_Home(uint8_t data);
void Choose_glass_group(uint8_t num);

#endif /* INC_MGR_HMI_H_ */
