/*
 * version.c
 *
 *  Created on: Apr 1, 2025
 *      Author: Chuc
 */

#include"version.h"


__attribute__((section(".version_data")))
uint8_t version_data[4]={0x21,0x48,0x78,0x92};
