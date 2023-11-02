#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


/*测试 itoa(): 整型转成字符串*/
/*测试 atoi(): 字符串转成整型*/



typedef struct
{
   uint32_t CREL;         /*!< FDCAN Core Release register,                                     Address offset: 0x000 */
   uint32_t ENDN;         /*!< FDCAN Endian register,                                           Address offset: 0x004 */
    uint32_t RESERVED1;    /*!< Reserved,                                                                        0x008 */
    uint32_t DBTP;         /*!< FDCAN Data Bit Timing & Prescaler register,                      Address offset: 0x00C */
    uint32_t TEST;         /*!< FDCAN Test register,                                             Address offset: 0x010 */
    uint32_t RWD;          /*!< FDCAN RAM Watchdog register,                                     Address offset: 0x014 */
    uint32_t CCCR;         /*!< FDCAN CC Control register,                                       Address offset: 0x018 */
    uint32_t NBTP;         /*!< FDCAN Nominal Bit Timing & Prescaler register,                   Address offset: 0x01C */
    uint32_t TSCC;         /*!< FDCAN Timestamp Counter Configuration register,                  Address offset: 0x020 */
    uint32_t TSCV;         /*!< FDCAN Timestamp Counter Value register,                          Address offset: 0x024 */
    uint32_t TOCC;         /*!< FDCAN Timeout Counter Configuration register,                    Address offset: 0x028 */
    uint32_t TOCV;         /*!< FDCAN Timeout Counter Value register,                            Address offset: 0x02C */
    uint32_t RESERVED2[4]; /*!< Reserved,                                                                0x030 - 0x03C */
    uint32_t ECR;          /*!< FDCAN Error Counter register,                                    Address offset: 0x040 */
    uint32_t PSR;          /*!< FDCAN Protocol Status register,                                  Address offset: 0x044 */
    uint32_t TDCR;         /*!< FDCAN Transmitter Delay Compensation register,                   Address offset: 0x048 */
    uint32_t RESERVED3;    /*!< Reserved,                                                                        0x04C */
    uint32_t IR;           /*!< FDCAN Interrupt register,                                        Address offset: 0x050 */
    uint32_t IE;           /*!< FDCAN Interrupt Enable register,                                 Address offset: 0x054 */
    uint32_t ILS;          /*!< FDCAN Interrupt Line Select register,                            Address offset: 0x058 */
    uint32_t ILE;          /*!< FDCAN Interrupt Line Enable register,                            Address offset: 0x05C */
    uint32_t RESERVED4[8]; /*!< Reserved,                                                                0x060 - 0x07C */
    uint32_t GFC;          /*!< FDCAN Global Filter Configuration register,                      Address offset: 0x080 */
    uint32_t SIDFC;        /*!< FDCAN Standard ID Filter Configuration register,                 Address offset: 0x084 */
    uint32_t XIDFC;        /*!< FDCAN Extended ID Filter Configuration register,                 Address offset: 0x088 */
    uint32_t RESERVED5;    /*!< Reserved,                                                                        0x08C */
    uint32_t XIDAM;        /*!< FDCAN Extended ID AND Mask register,                             Address offset: 0x090 */
    uint32_t HPMS;         /*!< FDCAN High Priority Message Status register,                     Address offset: 0x094 */
    uint32_t NDAT1;        /*!< FDCAN New Data 1 register,                                       Address offset: 0x098 */
    uint32_t NDAT2;        /*!< FDCAN New Data 2 register,                                       Address offset: 0x09C */
    uint32_t RXF0C;        /*!< FDCAN Rx FIFO 0 Configuration register,                          Address offset: 0x0A0 */
    uint32_t RXF0S;        /*!< FDCAN Rx FIFO 0 Status register,                                 Address offset: 0x0A4 */
    uint32_t RXF0A;        /*!< FDCAN Rx FIFO 0 Acknowledge register,                            Address offset: 0x0A8 */
    uint32_t RXBC;         /*!< FDCAN Rx Buffer Configuration register,                          Address offset: 0x0AC */
    uint32_t RXF1C;        /*!< FDCAN Rx FIFO 1 Configuration register,                          Address offset: 0x0B0 */
    uint32_t RXF1S;        /*!< FDCAN Rx FIFO 1 Status register,                                 Address offset: 0x0B4 */
    uint32_t RXF1A;        /*!< FDCAN Rx FIFO 1 Acknowledge register,                            Address offset: 0x0B8 */
    uint32_t RXESC;        /*!< FDCAN Rx Buffer/FIFO Element Size Configuration register,        Address offset: 0x0BC */
    uint32_t TXBC;         /*!< FDCAN Tx Buffer Configuration register,                          Address offset: 0x0C0 */
    uint32_t TXFQS;        /*!< FDCAN Tx FIFO/Queue Status register,                             Address offset: 0x0C4 */
    uint32_t TXESC;        /*!< FDCAN Tx Buffer Element Size Configuration register,             Address offset: 0x0C8 */
    uint32_t TXBRP;        /*!< FDCAN Tx Buffer Request Pending register,                        Address offset: 0x0CC */
    uint32_t TXBAR;        /*!< FDCAN Tx Buffer Add Request register,                            Address offset: 0x0D0 */
    uint32_t TXBCR;        /*!< FDCAN Tx Buffer Cancellation Request register,                   Address offset: 0x0D4 */
    uint32_t TXBTO;        /*!< FDCAN Tx Buffer Transmission Occurred register,                  Address offset: 0x0D8 */
    uint32_t TXBCF;        /*!< FDCAN Tx Buffer Cancellation Finished register,                  Address offset: 0x0DC */
    uint32_t TXBTIE;       /*!< FDCAN Tx Buffer Transmission Interrupt Enable register,          Address offset: 0x0E0 */
    uint32_t TXBCIE;       /*!< FDCAN Tx Buffer Cancellation Finished Interrupt Enable register, Address offset: 0x0E4 */
    uint32_t RESERVED6[2]; /*!< Reserved,                                                                0x0E8 - 0x0EC */
    uint32_t TXEFC;        /*!< FDCAN Tx Event FIFO Configuration register,                      Address offset: 0x0F0 */
    uint32_t TXEFS;        /*!< FDCAN Tx Event FIFO Status register,                             Address offset: 0x0F4 */
    uint32_t TXEFA;        /*!< FDCAN Tx Event FIFO Acknowledge register,                        Address offset: 0x0F8 */
    uint32_t RESERVED7;    /*!< Reserved,                                                                        0x0FC */
} FDCAN_GlobalTypeDef;

#define PERIPH_BASE               ((uint32_t)0x40000000)                        //地址 ：取名为PERIPH_BASE
#define D2_APB1PERIPH_BASE        PERIPH_BASE
#define D2_APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000)
#define D2_AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000)
#define D2_AHB2PERIPH_BASE       (PERIPH_BASE + 0x08020000)
#define FDCAN1_BASE           (D2_APB1PERIPH_BASE + 0xA000)                      //
#define PERIPH_BASE1           ((FDCAN_GlobalTypeDef *))
#define FDCAN1              ((FDCAN_GlobalTypeDef *) FDCAN1_BASE)              //PDCAN1 = (FDCAN_GlobalTypeDef *) FDCAN1_BASE ;


int main(){

	int a=291;
	char b[16]={0};

	itoa(a,b,16);
	printf("b=0x%s\n",b);
	

	printf("FDCAN1_BASE的地址为：%p\n",FDCAN1_BASE);
	printf("FDCAN1的地址为：%p\n",FDCAN1);
	

	

}