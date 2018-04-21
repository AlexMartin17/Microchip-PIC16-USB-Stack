/*
 * File:   UsbDescriptors.h
 * Author: Szymon Roslowski
 *
 * Created on 13 October 2014, 18:44
 *
 * Device and configuration descriptors.  These are used as the
 * host enumerates the device and discovers what class of device
 * it is and what interfaces it supports.
 */

#ifndef USBDESCRIPTORS_H
#define	USBDESCRIPTORS_H

#include <GenericTypeDefs.h>

// Vendor and Product Information
#define VendorId    0x04D8
#define ProductId   0x01A5
#define ReleaseNo   0x0001

// Definitions
#define InterfaceCount          0x01 // One Interface - Just Generic HID
#define StringDescriptorCount   0x03 // Three string descriptors - See Bottom of this file
#define Endpoint0BufferSize     0x08 // Endpoint 0 Buffer Size
#define HidDescriptorSize       0x20 // Size Of HID Descriptor
// HID
#define HidReportByteCount      0x08 // Hid Report Size, also size of Buffers etc. ( Memory usage can go over the roof if not careful with this value)
#define HidInterfaceNumber      0x00 // Interface For our HID

// Strings
#define SMAN 0x01   // Manufacturer Name String Index
#define SPRD 0x02   // Product Name String Index
#define SSER 0x00   // Serial Number String Index
#define SCON 0x00   // Configuration String Index

// Actual USB Data Buffers
volatile char HIDRxBuffer[HidReportByteCount];
volatile char HIDTxBuffer[HidReportByteCount];

BufferInfo Buffers[(InterfaceCount * 2)] =
{
    { HidReportByteCount, (char*)&HIDTxBuffer },
    { HidReportByteCount, (char*)&HIDRxBuffer }
};

/***********************/
/* Descriptors         */
/***********************/

// Device Descriptor
const char DeviceDescriptor[]=
{
    0x12,   // Size of this descriptor in bytes
    0x01,   // DEVICE descriptor type
    0x00,   // USB Spec Release Number in BCD format LSB
    0x02,   // USB Spec Release Number in BCD format MSB
    0x00,   // Class Code
    0x00,   // Subclass code
    0x00,   // Protocol code
    E0SZ,   // Max packet size for EP0 (8)
    VIDL,   // Vendor ID LSB
    VIDH,   // Vendor ID MSB
    PIDL,   // Product ID: Custom HID device demo LSB
    PIDH,   // Product ID: Custom HID device demo MSB
    RELL,   // Device release number in BCD format LSB
    RELH,   // Device release number in BCD format MSB
    SMAN,   // Manufacturer string index
    SPRD,   // Product string index
    SSER,   // Device serial number string index
    0x01    // Number of possible configurations
};

// ...Stuck these here to keep the number of files to minimum
#define HRBC HidReportByteCount
typedef struct _configStruct
{
    char configHeader[CONFIG_HEADER_SIZE];
    char HIDDescriptor[HidDescriptorSize];
} ConfigStruct;

// Configuration descriptor
const ConfigStruct ConfigurationDescriptor =
{
    {
        // Configuration descriptor
    0x09,   // Size of this descriptor in bytes
    0x02,   // CONFIGURATION descriptor type
    0x29,   // Total length of data for this cfg LSB
    0x00,   // Total length of data for this cfg MSB
    INTF,   // Number of interfaces in this cfg
    0x01,   // Index value of this configuration
    SCON,   // Configuration string index
    0xA0,   // Attributes
    0x32,   // Max power consumption (50 mA)
    },
    {
        // Generic HID Interface descriptor
    0x09,   // Size of this descriptor in bytes
    0x04,   // INTERFACE descriptor type
    IHID,   // Interface Number
    0x00,   // Alternate Setting Number
    0x02,   // Number of endpoints in this interface
    0x03,   // Class code (HID)
    0x00,   // Subclass code
    0x00,   // Protocol code 0-none, 1-Keyboard, 2- Mouse
    0x00,   // Interface string index

        // Generic Hid Class-Specific descriptor
    0x09,   // Size of this descriptor in bytes
    0x21,   // HID descriptor type
    0x11,   // HID Spec Release Number in BCD format (1.11) LSB
    0x01,   // HID Spec Release Number in BCD format (1.11) MSB
    0x00,   // Country Code (0x00 for Not supported)
    0x01,   // Number of class descriptors
    0x22,   // Report descriptor type
    0x2F,   // Report Size LSB  (47 bytes)
    0x00,   // Report Size MSB

    	// Generic HID Endpoint 1 In
    0x07,   // Size of this descriptor in bytes
    0x05,   // ENDPOINT descriptor type
    0x81,   // Endpoint Address
    0x03,   // Attributes (Interrupt)
    HRBC,   // Max Packet Size LSB
    0x00,   // Max Packet Size MSB
    0x01,   // Interval (1 millisecond)

    	// Generic HID Endpoint 1 Out
    0x07,   // Size of this descriptor in bytes
    0x05,   // ENDPOINT descriptor type
    0x01,   // Endpoint Address
    0x03,   // Attributes (Interrupt)
    HRBC,   // Max Packet Size LSB
    0x00,   // Max Packet Size MSB
    0x01   // Interval (1 millisecond)
    }
};

// Report For Generic HID
const char HIDReport[] = {
    0x06, 0xa0, 0xff,  // USAGE_PAGE (Vendor Defined Page 1)
    0x09, 0x01,        // USAGE (Vendor Usage 1)
    0xa1, 0x01,        // COLLECTION (Application)

	// The Input report
	0x09, 0x01,     	// Usage ID - vendor defined
	0x15, 0x00,     	// Logical Minimum (0)
	0x26, 0xFF, 0x00,       // Logical Maximum (255)
	0x75, 0x08,     	// Report Size (8 bits)
	0x95, HRBC,     	// Report Count
	0x81, 0x02,     	// Input (Data, Variable, Absolute)

	// The Output report
	0x09, 0x01,     	// Usage ID - vendor defined
	0x15, 0x00,     	// Logical Minimum (0)
	0x26, 0xFF, 0x00,       // Logical Maximum (255)
	0x75, 0x08,     	// Report Size (8 bits)
	0x95, HRBC,     	// Report Count
	0x91, 0x02,      	// Output (Data, Variable, Absolute)

	// The Feature report
	0x09, 0x02,     	// Usage ID - vendor defined
	0x15, 0x00,     	// Logical Minimum (0)
	0x26, 0xFF, 0x00,       // Logical Maximum (255)
	0x75, 0x08,     	// Report Size (8 bits)
	0x95, HRBC,     	// Report Count
	0xB1, 0x02,      	// Feature (Data, Variable, Absolute)

    0xc0               // END_COLLECTION
};


const struct{char bLength;char bDscType;char string[1];}StringDescriptor0={sizeof(StringDescriptor0),0x03,{0x0409}};

const struct{char bLength;char bDscType;char string[11];}StringDescriptor1={sizeof(StringDescriptor1),0x03,
{'D','I','Y',' ','D','e','v','i','c','e','s'}};

const struct{char bLength;char bDscType;char string[26];}StringDescriptor2={sizeof(StringDescriptor2),0x03,
{'1','6','F','1','4','5','5',' ','G','e','n','e','r','i','c',' ','H','I','D',' ','D','e','v','i','c','e'}};

//Array of string descriptors
const char *const StringDescriptorPointers[StringDescriptorCount]=
{
    (const char *const)&StringDescriptor0,
    (const char *const)&StringDescriptor1,
    (const char *const)&StringDescriptor2
};

#endif	/* USBDESCRIPTORS_H */

