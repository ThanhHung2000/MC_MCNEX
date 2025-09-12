/*
 * tp_cfg.h
 *
 *  Created on: Apr 23, 2025
 *      Author: R&D
 */

#ifndef TP_CFG_H_
#define TP_CFG_H_

uint8_t * DescGetBuffer(uint16_t dataLenght);
#define __ApplTpRxGetBuffer(tpChannel, datLen) (DescGetBuffer(datLen))



#endif /* TP_CFG_H_ */
