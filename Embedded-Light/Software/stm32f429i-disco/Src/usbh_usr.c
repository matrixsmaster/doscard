/**
  ******************************************************************************
  * @file    usbh_usr.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   USB Host User Functions
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbh_usr.h"

#ifdef FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#endif

#include <string.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern USB_OTG_CORE_HANDLE            USB_OTG_Core;
extern USBH_HOST                      USB_Host;

/*  Points to the DEVICE_PROP structure of current device */
/*  The purpose of this register is to speed up the execution */

USBH_Usr_cb_TypeDef USBH_USR_cb =
  {
    USBH_USR_Init,
    USBH_USR_DeInit,
    USBH_USR_DeviceAttached,
    USBH_USR_ResetDevice,
    USBH_USR_DeviceDisconnected,
    USBH_USR_OverCurrentDetected,
    USBH_USR_DeviceSpeedDetected,
    USBH_USR_Device_DescAvailable,
    USBH_USR_DeviceAddressAssigned,
    USBH_USR_Configuration_DescAvailable,
    USBH_USR_Manufacturer_String,
    USBH_USR_Product_String,
    USBH_USR_SerialNum_String,
    USBH_USR_EnumerationDone,
    USBH_USR_UserInput,
    USBH_USR_MSC_Application,
    USBH_USR_DeviceNotSupported,
    USBH_USR_UnrecoveredError
  };

uint32_t __IO USB_Host_Application_Ready;

/*--------------- LCD Messages ---------------*/
const uint8_t MSG_HOST_INIT[]        = "[USB] Host Library Initialized";
const uint8_t MSG_DEV_ATTACHED[]     = "[USB] Device Attached ";
const uint8_t MSG_DEV_DISCONNECTED[] = "[USB] Device Disconnected";
const uint8_t MSG_DEV_ENUMERATED[]   = "[USB] Enumeration completed ";
const uint8_t MSG_DEV_HIGHSPEED[]    = "[USB] High speed device detected";
const uint8_t MSG_DEV_FULLSPEED[]    = "[USB] Full speed device detected";
const uint8_t MSG_DEV_LOWSPEED[]     = "[USB] Low speed device detected";
const uint8_t MSG_DEV_ERROR[]        = "[USB] Device fault ";

const uint8_t MSG_MSC_CLASS[]      = "[USB] Mass storage device connected";
const uint8_t MSG_HID_CLASS[]      = "[USB] HID device connected";
const uint8_t MSG_DISK_SIZE[]      = "[USB] Size of the disk in MBytes: ";
const uint8_t MSG_LUN[]            = "[USB] LUN Available in the device:";
const uint8_t MSG_ROOT_CONT[]      = "[USB] Exploring disk flash ...";
const uint8_t MSG_WR_PROTECT[]      = "[USB] The disk is write protected";
const uint8_t MSG_UNREC_ERROR[]     = "[USB] UNRECOVERED ERROR STATE";


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Add the host lib initialization message to the console
  * @param  None
  * @retval None
  */
void USBH_USR_Init(void)
{
	USB_Host_Application_Ready = 0;
	LCD_DisplayStringLine(LCD_LINE_0,(uint8_t*)"Inited!");
}

/**
  * @brief  Add the device attachement message to the console
  * @param  None
  * @retval None
  */
void USBH_USR_DeviceAttached(void)
{
	LCD_DisplayStringLine(LCD_LINE_1,(uint8_t*)"Connected!");
}


/**
  * @brief  Add the unrecovered error message to the console
  * @param  None
  * @retval None
  */
void USBH_USR_UnrecoveredError (void)
{
}


/**
  * @brief Add the device disconnection message to the console and free 
  *        USB associated resources
  * @param  None
  * @retval None
  */
void USBH_USR_DeviceDisconnected (void)
{
  USB_Host_Application_Ready = 0;
//  f_mount(0, NULL);
}
/**
  * @brief  callback of the device reset event
  * @param  None
  * @retval None
  */
void USBH_USR_ResetDevice(void)
{
}


/**
  * @brief  Add the device speed message to the console
  * @param  Device speed
  * @retval None
  */
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
}

/**
  * @brief  Add the USB device vendor and MFC Ids to the console
  * @param  device descriptor
  * @retval None
  */
void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{
	char buf[90],tmp[4];
	uint8_t* dsc = (uint8_t*)DeviceDesc;
	int l = 2;
	buf[0] = 0;
	for (int i = 0; i < USB_DEVICE_DESC_SIZE; i++) {
		snprintf(tmp,sizeof(tmp),"%02X",*(dsc++));
		strcat(buf,tmp);
		if (i && !(i%6)) {
			LCD_DisplayStringLine(LINE(l),(uint8_t*)buf);
			buf[0] = 0;
			l++;
		}
	}
	if (strlen(buf)) LCD_DisplayStringLine(LINE(l),(uint8_t*)buf);
}

/**
  * @brief  Device addressed event callbacak
  * @param  None
  * @retval None
  */
void USBH_USR_DeviceAddressAssigned(void)
{
}

/**
  * @brief  Add the device class description to the console
  * @param  Configuration descriptor
  * @retval None
  */
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
    USBH_InterfaceDesc_TypeDef *itfDesc,
    USBH_EpDesc_TypeDef *epDesc)
{
//	LCD_DisplayStringLine(LCD_LINE_3,(uint8_t*)"Config read!");
}

/**
  * @brief  Add the MFC String to the console
  * @param  Manufacturer String
  * @retval None
  */
void USBH_USR_Manufacturer_String(void *ManufacturerString)
{
}

/**
  * @brief  Add the Product String to the console
  * @param  Product String
  * @retval None
  */
void USBH_USR_Product_String(void *ProductString)
{
}

/**
  * @brief  Add the Srial Number String to the console
  * @param  SerialNum_String
  * @retval None
  */
void USBH_USR_SerialNum_String(void *SerialNumString)
{
}



/**
  * @brief  Enumeration complete event callback
  * @param  None
  * @retval None
  */
void USBH_USR_EnumerationDone(void)
{
}

/**
  * @brief  Device is not supported callback
  * @param  None
  * @retval None
  */
void USBH_USR_DeviceNotSupported(void)
{
}

/**
  * @brief  User Action for application state entry callback
  * @param  None
  * @retval USBH_USR_Status : User response for key button
  */
USBH_USR_Status USBH_USR_UserInput(void)
{
  return USBH_USR_RESP_OK;
}

/**
  * @brief  Over Current Detected on VBUS
  * @param  None
  * @retval Staus
  */
void USBH_USR_OverCurrentDetected (void)
{
}


/**
  * @brief  Mass storage application main handler
  * @param  None
  * @retval Staus
  */
int USBH_USR_MSC_Application(void)
{
  return(0);
}
/**
  * @brief  De-init User state and associated variables
  * @param  None
  * @retval None
  */
void USBH_USR_DeInit(void)
{
  USB_Host_Application_Ready = 0;
}

/**
  * @brief  Handle Modules Background processes in the main task
  * @param  None
  * @retval None
*/
void USBH_USR_BackgroundProcess (void)
{
  if((USB_Host_Application_Ready == 0) || (HCD_IsDeviceConnected(&USB_OTG_Core) == 0))
  {
    USBH_Process(&USB_OTG_Core, &USB_Host);   
  }
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
