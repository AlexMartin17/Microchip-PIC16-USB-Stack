/*
 * File:   Usb.h
 * Author: Szymon Roslowski
 *
 * Created on 13 October 2014, 17:45
 *
 * Firmware framework for USB I/O on PIC 16F1455 (and siblings)
 *
 * Based On
 *
 * Firmware framework for USB I/O on PIC 18F2455 (and siblings)
 * Copyright (C) 2005 Alexander Enzmann
 * adapted to MCC18 by Alberto Maccioni on 1/8/09
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111 USA
 * or see <http://www.gnu.org/licenses/>
 */

#ifndef USB_H
#define	USB_H

// Macros
#define PTR16(x) ((unsigned int)(((unsigned long)x) & 0xFFFF))
#define LSB(x) (x & 0xFF)
#define MSB(x) ((x & 0xFF00) >> 8)
#define ClearUsbInterruptFlag(x)        UIR &= ~(x)
#define IsUsbReady ((DeviceState == 0x05) && (UCONbits.SUSPND==0))
#define UsbInterrupt PIR2bits.USBIF
#define VIDL LSB(VendorId)  // Vendor Id Low Byte (LSB)
#define VIDH MSB(VendorId)  // Vendor Id High Byte (MSB)
#define PIDH MSB(ProductId) // Product Id High Byte (MSB)
#define PIDL LSB(ProductId) // Product Id Low Byte (LSB)
#define RELH MSB(ReleaseNo) // Release Number High Byte (MSB)
#define RELL LSB(ReleaseNo) // Release Number Low Byte (LSB)
#define INTF InterfaceCount // Total Count of Interfaces
#define IHID HidInterfaceNumber
#define E0SZ Endpoint0BufferSize
#define CONFIG_HEADER_SIZE      0x09 // Configuration descriptor header size (see UsbDescriptors.h) - Pretty much always 9 :)

#include <GenericTypeDefs.h>

// Structures
typedef struct _BufferInfo
{
    char Size;
    char *Buffer;
} BufferInfo;

// Includes
#include "UsbDescriptors.h"

// Global Variables
char DeviceState;    // Visible device states (from USB 2.0, chap 9.1.1)

// USB Functions
void InitializeUSB(void);
void EnableUSBModule(void);
void HIDSend(char InterfaceNo);
void ProcessUSBTransactions(void);
void ReArmInterface(char InterfaceNo);
char IsUsbDataAvaialble(char InterfaceNo);

#endif	/* USB_H */

