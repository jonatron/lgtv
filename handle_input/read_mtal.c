#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>


// EXTERN UINT16 SIF_ReadMultipleSubAddr(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum, UINT32 u4WordAddr,
// UINT8 *pu1Buf, UINT16 u2ByteCnt); 

//-------------------------------------------------------------------------
/** MTSIF_ReadMultipleSubAddr
 *  read data with word address from SIF module.
 *  @param  ePort     SIF Port ID
 *  @param  u4ClkVal  Demaned Sif Clock in Khz
 *  @param  u1DevAddr   Serial interface device address.
 *  @param  pu1Addr   Pointer to Serial interface word address
 *  @param  u2AddrCnt   Serial interface word address count (up to 3).
 *  @param  pu1Data     Pointer to data
 *  @param  u2DataCnt   Number of byte to read. Max = 256 bytes.
*/


// typedef struct
// {
//     UINT8 u1RequestType;      /* SIF_ASYNC_REQ_READ or  SIF_ASYNC_REQ_WRITE */
//     UINT8 u1CHannelID;      /* SIF bus ID */
//     UINT16 u2ClkDiv;      /* Parameter of divider to divide 27Mhz for SCL */
//     UINT8 u1DevAddr;      /* SIF device address */
//     UINT8 u1WordAddrNum;      /* SIF word address length */
//     UINT32 u4WordAddr;      /* SIF word address. Format XX B2 B1 B0. B2 is sent on SIF bus first */
//     UINT8 *pu1Buf;      /* Pointer to user's buffer */
//     UINT16 u2ByteCnt;      /* Number of byte to transfer. Max = 256 bytes */
//     SIF_ASYNC_NFY_FCT pfnNfy;      /* callback function pointer */
//     VOID *pvNfyData;      /* callback function parameter */
// } SIF_ASYNC_REQ_T;


/*
[MTPERIPHERAL]:MTSIF_ReadMultipleSubAddr
 - ePort = 1
u4ClkVal = 0x17c
u1DevAddr = 0x52
u2AddrCnt = 0x1
u2DataCnt = 0x3
*/


// int DDI_CMNIO_I2C_Read(uchar chNum, uchar transMode, uchar slaveAddr, uint subAddrMode, uchar *subAddr, ushort nBytes, uchar *rxBuf, uint retry)
// {
//     uint  i;
//     int   ret;
//     uchar ucSifId;
//     uint  uiClkVal=100;
//     uint  uiAddr;
//     switch(chNum)
//     {
//     case 1:
//       ucSifId = SIF_BUS_PDWNC_I2C_1;
//       break;
//     case 2:
//       return -1;
//       break;
//     case 3:
//       ucSifId = SIF_BUS_SYSTEM_I2C;
//       break;
//     case 4:
//       ucSifId = SIF_BUS_SECOND_I2C;
//       break;
//     case 5:
//       ucSifId = SIF_BUS_TUNER_I2C;
//       break;
//         default:
//             return -1;
//         break;
//     }

//     for(i=0;i<subAddrMode;i++)
//     {
//         uiAddr = uiAddr << 8;
//         uiAddr |= *(subAddr+i);
//     }


// subaddr = CP_READ_MICOM_INV_ONOFF;

// #define CP_WRITE_INVERT_OR_VAVS_ON    0x36
// #define CP_WRITE_INVERT_OR_VAVS_OFF   0x37
// #define CP_WRITE_PANEL_ONOFF        0x3A
// #define   CP_SLAVRX_DELAYED_INV_OFF   0x3B
// #define CP_READ_RTC_YMDHMS        0x80
// #define CP_READ_RESERVE_TIME        0x87
// #define CP_READ_MICOM_VERSION     0xA1
// #define CP_READ_MICOM_HWOPTION      0xA6
// #define CP_READ_MICOM_INV_ONOFF     0xD0
// #define CP_READ_MICOM_PANEL_ONOFF   0xD1
// #define CP_WRITE_NAND_WRITE_PROTECT   0x54
// #define CP_READ_MICOM_TYPE        0xB4

// from micom_task = 0xc0


typedef unsigned char BYTE;
typedef unsigned char  UCHAR;
typedef unsigned char  UINT8;
typedef unsigned short  UINT16;
typedef unsigned int  UINT32;


int main(void)
{
   void *handle;
   // double (*cosine)(double);
   int (*MTSIF_ReadMultipleSubAddr)(UINT32 ePort, UINT32 u4ClkVal, UINT8 u1DevAddr, UINT8 *pu1Addr,
        UINT16 u2AddrCnt, UINT8 *pu1Data, UINT16 u2DataCnt);
   int (*_DRV_GetMtalDeviceHandle)(void);
   int (*DRV_Init)(void);

   int init_rc;
   int device_handle;

   char *error;
   UINT8 subaddr[5];
   UINT8 data[5];

   handle = dlopen("libmtal.so", RTLD_LAZY);
   if (!handle) {
       fprintf(stderr, "%s\n", dlerror());
       exit(EXIT_FAILURE);
   }

   dlerror();    /* Clear any existing error */

   // cosine = (double (*)(double)) dlsym(handle, "cos");
   DRV_Init = dlsym(handle, "DRV_Init");
   _DRV_GetMtalDeviceHandle = dlsym(handle, "_DRV_GetMtalDeviceHandle");
   MTSIF_ReadMultipleSubAddr = dlsym(handle, "MTSIF_ReadMultipleSubAddr");

   subaddr[0] = 0xc0;

   init_rc = (*DRV_Init)();
   printf("init %d\n", init_rc);

   device_handle = (*_DRV_GetMtalDeviceHandle)();
   printf("device_handle %d\n", device_handle);

   (*MTSIF_ReadMultipleSubAddr)(1, 0x17c, 0x52, subaddr, 0x1, data, 0x3);
   printf("data %d, %d, %d.\n", data[0], data[1], data[2]);

   error = dlerror();
   if (error != NULL) {
       fprintf(stderr, "%s\n", error);
       exit(EXIT_FAILURE);
   }

   // printf("%f\n", (*cosine)(2.0));
   dlclose(handle);
   exit(EXIT_SUCCESS);
}
