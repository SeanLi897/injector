#include "SD_APP.h"

// 增加写入超时
#define SD_WRITE_TIMEOUT 10000  // 10秒
int SDCard_states = 0;
uint16_t csv_crt_line_No = 0;

FRESULT f_write_retry(FIL* fp, const void* buff, UINT btw) {
  UINT bw;
  FRESULT res;
  uint32_t start = HAL_GetTick();

  do {
    res = f_write(fp, buff, btw, &bw);

    if(res != FR_OK) break;
    if(HAL_GetTick() - start > SD_WRITE_TIMEOUT) {
      res = FR_TIMEOUT;
      break;
    }
  } while(bw != btw);

  return res;
}

int SDCard_InsertCheck(void){
	uint8_t SD_Check_retry = 0;
	int SDCard_states = 0;

  if(SD_INSERT){
  	int ret = 0;
  	do
		{
		ret = SD_FS_Init();
		}while(ret != 0 && SD_Check_retry++ >5);

  	if(ret == 0){
  		SDCard_states = 0; //正常
  		printf("SDCard Init success!\r\n");
  	}
  	else{
  		SDCard_states = -1;//初始化失败
  		printf("SDCard Init fail!\r\n");
  	}
  }
  else{
  	SDCard_states = -2;//未插入SD卡
  	printf("SDCard is not insert!\r\n");
  }
  return SDCard_states;
}

uint8_t SD_FS_Init(void)
{
    static FATFS sd_fs;
    uint32_t ret = 0;
    MKFS_PARM opt = {0};
    opt.fmt = FM_FAT;
    static uint8_t work_buf[512] = {0};
    ret = f_mount(&sd_fs, DISK_SD, 1);

    if(ret != FR_OK)
    {
        switch(ret)
        {
            case FR_NO_FILESYSTEM:
            {
                ret = f_mkfs(DISK_SD, &opt, work_buf, sizeof(work_buf));
            }break;
            case FR_MKFS_ABORTED:
            {
                ret = f_mkfs(DISK_SD, &opt, work_buf, sizeof(work_buf));
            }break;
        }
    }
    if(ret != 0)
    {
      printf("sd fs f_mkfs fail(%d)\r\n", ret);
    }
    else
    {
    	printf("sd fs f_mkfs success!\r\n");
    }
    return ret;
}

FRESULT check_file_exists(const char* path) {
    FILINFO fno;
    return f_stat(path, &fno);
}

//#define USER_FILE_NAME "0:/20250224.txt"

//uint8_t demo_user_fs(void)
//{
//    FIL fp;
//    uint32_t real_write_num = 0;
//    FRESULT res;
//
//    char USER_FILE_NAME[15] = {0};
//    char file_name_buf[15] = {0};
//    Format_FileName_Date(file_name_buf, GPS_RMC_Data.time, GPS_RMC_Data.date);
//    snprintf(USER_FILE_NAME,15,"0:/%s.txt",file_name_buf);
//		printf("%s\r\n",USER_FILE_NAME);
//
//    res = check_file_exists(USER_FILE_NAME);
//
//    if (res == FR_OK) {
//        printf("文件存在\r\n");
//    } else if (res == FR_NO_FILE) {
//        printf("文件不存在\r\n");
//    } else {
//        printf("错误码: %d\r\n", res);  // 其他错误（如硬件问题）
//    }
//
//    FRESULT ret = f_open(&fp, USER_FILE_NAME, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
//    if(ret != FR_OK)
//    {
//    	printf("f_open fail: %s\r\n", USER_FILE_NAME);
//      return 1;
//    }
//
//    uint8_t *test_w_b = "fs test write  gps data!";
//    ret = f_write_retry(&fp, test_w_b, strlen(test_w_b));
//
//    if(ret != FR_OK)
//    {
//    	printf("f_open fail:%d\r\n", ret );
//      return 1;
//    }
//
//    ret = f_close(&fp);
//    if(ret != FR_OK)
//    {
//    	printf("f_close fail:%d\r\n", ret);
//      return 1;
//    }
//
//    res = check_file_exists(USER_FILE_NAME);
//
//    if (res == FR_OK) {
//        printf("文件存在\r\n");
//    } else if (res == FR_NO_FILE) {
//        printf("文件不存在\r\n");
//    } else {
//        printf("错误码: %d\r\n", res);  // 其他错误（如硬件问题）
//    }
//
//    uint32_t real_read_num = 0;
//    uint8_t test_r_b[100] = {0};
//    ret = f_open(&fp, USER_FILE_NAME, FA_READ);
//    if(ret != FR_OK)
//    {
//    	printf("f_open fail:%s\r\n", USER_FILE_NAME);
//      return 1;
//    }
//
//    ret = f_read(&fp, test_r_b, 100, &real_read_num);
//    if(ret != FR_OK)
//    {
//    	printf("f_read fail:%d\r\n", ret);
//      return 1;
//    }
//    printf("read data:%s\r\n", test_r_b);
//
//    ret = f_close(&fp);
//    if(ret != FR_OK)
//    {
//    	printf("f_close fail:%d\r\n", ret);
//      return 1;
//    }
////    else{
////    	printf("fs test read data f_close success!\r\n");
////    }
//    return 0;
//}

//#define FileName "0:/ABC.txt"
//void SD_TEST(void){
//	FIL fnew;
//	FIL fs;
//	uint32_t fnum = 0;
////----------------------f_mount()----------------------------
////	res_flash = f_mount(&fs,DISK_SD,1);                        // 挂载文件系统
////	printf("res_flash:%d\r\n",res_flash);
////
////	if(res_flash == FR_NO_FILESYSTEM)                       // 检测是否存在文件系统
////		{
////			res_flash = f_mkfs(DISK_SD,NULL,buffer,4096);          // 创建文件系统
////			if(res_flash == FR_OK)                              // 判断是否创建成功
////			{
////				printf("FATFS has been mkf.\r\n");
////				res_flash = f_mount(NULL,DISK_SD,0);               // 卸载文件系统
////				res_flash = f_mount(&fs,DISK_SD,1);                // 重新挂载文件系统
////			}
////			else                                                // 创建失败
////			{
////				printf("FATFS mkf filed.\r\n");
////				printf("res_flash:%d\r\n",res_flash);
////				while(1)                                        // 死循环
////				{
////				}
////			}
////		}
////	else if(res_flash !=FR_OK)                              // 挂载失败
////		{
////			printf("mount ERROR.\r\n");
////			while(1)                                            // 死循环
////			{
////			}
////		}
////	else                                                    // 挂载成功
////		{
////			printf("mount OK.\r\n");
////		}
////-----------------------f_open()----------------------------
//	FRESULT res_flash = f_open(&fnew,FileName,FA_CREATE_ALWAYS | FA_WRITE);  // 创建文件
//	printf("res_flash:%d\r\n",res_flash);
//
//	if(res_flash == FR_OK)                                  // 判断是否创建成功
//		{
//			printf("File open is OK.\r\n");
//		}
////-----------------------f_write()----------------------------
//	res_flash = f_write_retry(&fnew,"ABCDEFG",7);           // 写入数据
//	if(res_flash == FR_OK)                                  // 判断是否写入成功
//		{
//			printf("File write is OK.\r\n");
//		}
//	else                                                    // 写入失败
//		{
//			printf("res_flash:%d\r\n",res_flash);
//		}
////------------------------f_close()--------------------------------------
//	f_close(&fnew);                                         // 关闭文件
//
//	if(res_flash == FR_OK)                                  // 判断是否关闭成功
//		{
//			printf("File close is OK.\r\n");
//		}
//	else                                                    // 关闭失败
//		{
//			printf("res_flash:%d\r\n",res_flash);
//		}
////-------------------------f_unmount()----------------------------
////	res_flash = f_unmount(DISK_SD);                            // 卸载文件系统
////	printf("f_unmount-res_flash:%d\r\n",res_flash);
////
////	res_flash = f_mount(&fnew,DISK_SD,1);                        // 重新挂载文件系统
////	printf("f_mount-res_flash:%d\r\n",res_flash);                       // 判断是否重新挂载成功
////--------------------------f_open()-------------------------------------
//	res_flash = f_open(&fs,FileName,FA_OPEN_EXISTING | FA_READ);   // 打开文件
//	if(res_flash == FR_OK)                                  // 判断是否打开成功
//		{
//			printf("File open is OK.\r\n");
//		}
//	else                                                    // 打开失败
//		{
//		printf("res_flash:%d\r\n",res_flash);
//		}
////--------------------------f_read()-----------------------------------------
//	res_flash = f_read(&fs,FileName,7,&fnum);                    // 读取文件内容
//	if(res_flash == FR_OK)                                  // 判断是否读取成功
//		{
//			printf("File read is OK.\r\n");
//		}
//	else                                                    // 读取失败
//		{
//			printf("res_flash:%d\r\n",res_flash);
//		}
//	f_close(&fs);                                         // 关闭文件
////---------------------------f_unmount()-----------------------------------------------
////	res_flash = f_unmount(DISK_SD);                            // 卸载文件系统
////	printf("res_flash:%d\r\n",res_flash);
////
////	if(res_flash == FR_OK)                                  // 判断是否卸载成功
////		{
////			printf("unmount OK.\r\n");
////		}
//}

int CSV_sheet_Init(void){

	if(SDCard_states == 0){
    FIL fp;
    FRESULT res;
    uint16_t File_Num = 1;

    char USER_FILE_NAME[20] = {0};
    char file_name_buf[20] = {0};

    if(GPS_GGA_Data.fix_status == 1){
    	Format_FileName_Date(file_name_buf, GPS_RMC_Data.time, GPS_RMC_Data.date);
    }
    else{
    	sprintf(file_name_buf,"20250101");
    }
    snprintf(USER_FILE_NAME,20,"0:/%s%03d.csv", file_name_buf, File_Num);

    res = check_file_exists(USER_FILE_NAME);

    if (res == FR_OK) {//文件存在
    	AD24C02_Read2Byte(CSV_LAST_LINE_ADDRESS, &csv_crt_line_No);
    	return 0;
    }
    else if (res == FR_NO_FILE) {//文件不存在
        FRESULT ret = f_open(&fp, USER_FILE_NAME, FA_WRITE | FA_OPEN_APPEND);
        if(ret != FR_OK)
        {
        	printf("f_open fail: %s\r\n", USER_FILE_NAME);
          return 1;
        }

    		char header[] = {0xE5,0xBA,0x8F,0xE5,0x8F,0xB7,0x2C,0xE6,0x97,0xA5,0xE6
    				,0x9C,0x9F,0x2C,0xE6,0x97,0xB6,0xE9,0x97,0xB4,0x2C,0xE6,0xA0,0x91
						,0xE6,0x9C,0xA8,0xE7,0xBC,0x96,0xE5,0x8F,0xB7,0x2C,0xE6,0xB3,0xA8
						,0xE5,0xB0,0x84,0xE9,0x87,0x8F,0x2F,0x6D,0x4C,0x2C,0xE7,0xBB,0x8F
						,0xE5,0xBA,0xA6,0x2C,0xE7,0xBA,0xAC,0xE5,0xBA,0xA6,0x2C,0xE6,0xB5
						,0xB7,0xE6,0x8B,0x94,0x2F,0x6D,0x2C,0xE7,0xB4,0xAF,0xE8,0xAE,0xA1
						,0xE6,0xB3,0xA8,0xE8,0x8D,0xAF,0xE9,0x87,0x8F,0x2F,0x6D,0x4C,0x2C
						,0xE7,0xB4,0xAF,0xE8,0xAE,0xA1,0xE6,0xB3,0xA8,0xE8,0x8D,0xAF,0xE6,0x95,0xB0,0x0D,0X0A};
        ret = f_write_retry(&fp, header, strlen(header));

        f_sync(&fp);

        if(ret != FR_OK)
        {
        	printf("f_open fail:%d\r\n", ret );
          return 1;
        }

        ret = f_close(&fp);
        if(ret != FR_OK)
        {
        	printf("f_close fail:%d\r\n", ret);
          return 1;
        }

        csv_crt_line_No = 0;
        AD24C02_Write2Byte(CSV_LAST_LINE_ADDRESS, &csv_crt_line_No);
        return 0;

    } else {
        printf("错误码: %d\r\n", res);  // 其他错误（如硬件问题）
        return 1;
    }
	}
	else
	{
		return -1;//没发现SDCard
	}
}

int SDCard_Write_log(GPS_Data GGA_Result_Data, GPS_Data RMC_Result_Data){
	if(SDCard_states == 0){
    FIL fp;
    FRESULT res;
    uint16_t File_Num = 1;
    char USER_FILE_NAME[20] = {0};
    char file_name_buf[20] = {0};
    SD_LogData LogBuf;

    if(GGA_Result_Data.fix_status == 1){    //按日期格式化文件名
    	Format_FileName_Date(file_name_buf, GPS_RMC_Data.time, GPS_RMC_Data.date);
    }
    else{
    	sprintf(file_name_buf,"20250101");
    }
    snprintf(USER_FILE_NAME,20,"0:/%s%03d.csv", file_name_buf, File_Num);

    res = check_file_exists(USER_FILE_NAME);    //检查文件是否存在
    FRESULT ret = f_open(&fp, USER_FILE_NAME, FA_WRITE | FA_OPEN_APPEND);    //打开文件，若文件不存在则创建新文件，并定位到文件末尾
    if(ret != FR_OK)
    {
    	printf("f_open fail: %s\r\n", USER_FILE_NAME);
      return 1;
    }

    switch(res){
    case FR_NO_FILE://文件不存在，先向文件中写入列名称：序号,日期,时间,树木编号,注射剂量,经度,纬度,海拔,累计剂量,累计数量
    		char header[] = {0xE5,0xBA,0x8F,0xE5,0x8F,0xB7,0x2C,0xE6,0x97,0xA5,0xE6,0x9C,0x9F,
    				0x2C,0xE6,0x97,0xB6,0xE9,0x97,0xB4,0x2C,0xE6,0xA0,0x91,0xE6,0x9C,0xA8,0xE7,
						0xBC,0x96,0xE5,0x8F,0xB7,0x2C,0xE6,0xB3,0xA8,0xE5,0xB0,0x84,0xE9,0x87,0x8F,
						0x2F,0x6D,0x4C,0x2C,0xE7,0xBB,0x8F,0xE5,0xBA,0xA6,0x2C,0xE7,0xBA,0xAC,0xE5,
						0xBA,0xA6,0x2C,0xE6,0xB5,0xB7,0xE6,0x8B,0x94,0x2F,0x6D,0x2C,0xE7,0xB4,0xAF,
						0xE8,0xAE,0xA1,0xE6,0xB3,0xA8,0xE8,0x8D,0xAF,0xE9,0x87,0x8F,0x2F,0x6D,0x4C,
						0x2C,0xE7,0xB4,0xAF,0xE8,0xAE,0xA1,0xE6,0xB3,0xA8,0xE8,0x8D,0xAF,0xE6,0x95,0xB0,0x0D,0X0A};
        ret = f_write_retry(&fp, header, strlen(header));
        f_sync(&fp);
        csv_crt_line_No = 0;
        AD24C02_Write2Byte(CSV_LAST_LINE_ADDRESS, &csv_crt_line_No);
//        break;
    case FR_OK://文件存在
    		char write_bf[50];
//				if(csv_crt_line_No != 0){
//					AD24C02_Read2Byte(CSV_LAST_LINE_ADDRESS, &csv_crt_line_No);//读取最后一次保存的行号
//				}
				LogBuf.No = csv_crt_line_No++;
				Format_GPS_Date(LogBuf.Date,RMC_Result_Data.time,RMC_Result_Data.date);
				Format_GPS_Time(LogBuf.Time,RMC_Result_Data.time);
				LogBuf.TreeSN = current_TreeNo;
				LogBuf.TreeDose = InjectTimes*2;
				FormatLongitude(LogBuf.Lng, GGA_Result_Data.longitude, GGA_Result_Data.lon_dir);
				FormatLatitude(LogBuf.Lat, GGA_Result_Data.latitude, GGA_Result_Data.lat_dir);
				FormatAltitude(LogBuf.Alt, GGA_Result_Data.altitude);
				LogBuf.CumDose = total_inject_Dosage;
				LogBuf.CumQuty = total_Times;

				AD24C02_Write2Byte(CSV_LAST_LINE_ADDRESS, &csv_crt_line_No);
				printf("\r\n");

				printf("%04d,",LogBuf.No);//第1列：序号 0001
				sprintf(write_bf,"%4d,",LogBuf.No);
				ret = f_write_retry(&fp, write_bf, strlen(write_bf));
				if (ret != FR_OK) {
				    printf("Write failed: %d\n", ret);
				    f_close(&fp);
				    return 1;
				}
				memset(write_bf,'\0',sizeof(write_bf));

				if(GGA_Result_Data.fix_status == 1){
					printf("%s,",LogBuf.Date);//第2列：日期 2025/02/25
					sprintf(write_bf,"%s,",LogBuf.Date);//第2列：日期 2025/02/25
				}
				else{
					printf("2025/01/01,");//第2列：日期 2025/01/01
					sprintf(write_bf,"2025/01/01,");//第2列：日期 2025/01/01
				}
				ret = f_write_retry(&fp, write_bf, strlen(write_bf));
				if (ret != FR_OK) {
				    printf("Write failed: %d\n", ret);
				    f_close(&fp);
				    return 1;
				}
				memset(write_bf,'\0',sizeof(write_bf));

				if(GGA_Result_Data.fix_status == 1){
					printf("%s,",LogBuf.Time);//第3列：时间 18:21:21
					sprintf(write_bf,"%s,",LogBuf.Time);//第3列：时间 18:21:21
				}
				else{
					printf("12:00:00,");//第3列：时间 12:00:00
					sprintf(write_bf,"12:00:00,");//第3列：时间 12:00:00
				}
				ret = f_write_retry(&fp, write_bf, strlen(write_bf));
				if (ret != FR_OK) {
				    printf("Write failed: %d\n", ret);
				    f_close(&fp);
				    return 1;
				}
				memset(write_bf,'\0',sizeof(write_bf));

				printf("%04d,",LogBuf.TreeSN);//第4列：序号 0001
				sprintf(write_bf,"%4d,",LogBuf.TreeSN);
				ret = f_write_retry(&fp, write_bf, strlen(write_bf));
				if (ret != FR_OK) {
				    printf("Write failed: %d\n", ret);
				    f_close(&fp);
				    return 1;
				}
				memset(write_bf,'\0',sizeof(write_bf));

				printf("%d,",LogBuf.TreeDose);//第5列：注射剂量 30
				sprintf(write_bf,"%d,",LogBuf.TreeDose);
				ret = f_write_retry(&fp, write_bf, strlen(write_bf));
				if (ret != FR_OK) {
				    printf("Write failed: %d\n", ret);
				    f_close(&fp);
				    return 1;
				}
				memset(write_bf,'\0',sizeof(write_bf));

				if(GGA_Result_Data.fix_status == 1){
					printf("%s,",LogBuf.Lng);//第6列：经度 E118°48'
					sprintf(write_bf,"%s,",LogBuf.Lng);//第6列：经度 E118°48'
				}
				else{
					printf("E118°48',");//第6列：经度 E118°48'
					sprintf(write_bf,"E118°48',");//第6列：经度 E118°48
				}
				ret = f_write_retry(&fp, write_bf, strlen(write_bf));
				if (ret != FR_OK) {
				    printf("Write failed: %d\n", ret);
				    f_close(&fp);
				    return 1;
				}
				memset(write_bf,'\0',sizeof(write_bf));

				if(GGA_Result_Data.fix_status == 1){
					printf("%s,",LogBuf.Lat);//第7列：纬度 N31°50'
					sprintf(write_bf,"%s,",LogBuf.Lat);//第7列：纬度 N31°50'
				}
				else{
					printf("N31°50',");//第7列：纬度 N31°50'
					sprintf(write_bf,"N31°50',");//第7列：纬度 N31°50'
				}
				ret = f_write_retry(&fp, write_bf, strlen(write_bf));
				if (ret != FR_OK) {
				    printf("Write failed: %d\n", ret);
				    f_close(&fp);
				    return 1;
				}
				memset(write_bf,'\0',sizeof(write_bf));

				if(GGA_Result_Data.fix_status == 1){
					printf("%s,",LogBuf.Alt);//第8列：海拔 88.9
					sprintf(write_bf,"%s,",LogBuf.Alt);//第8列：海拔 88.9
				}
				else{
					printf("88.9,");//第8列：海拔 88.9
					sprintf(write_bf,"88.9,");//第8列：海拔 88.9
				}
				ret = f_write_retry(&fp, write_bf, strlen(write_bf));
				if (ret != FR_OK) {
				    printf("Write failed: %d\n", ret);
				    f_close(&fp);
				    return 1;
				}
				memset(write_bf,'\0',sizeof(write_bf));

				printf("%d,",LogBuf.CumDose);//第9列，累计剂量 892
				sprintf(write_bf,"%d,",LogBuf.CumDose);
				ret = f_write_retry(&fp, write_bf, strlen(write_bf));
				if (ret != FR_OK) {
				    printf("Write failed: %d\n", ret);
				    f_close(&fp);
				    return 1;
				}
				memset(write_bf,'\0',sizeof(write_bf));

				printf("%d\r\n",LogBuf.CumQuty);//第10列，累计数量 112
				sprintf(write_bf,"%d\r\n",LogBuf.CumQuty);
				ret = f_write_retry(&fp, write_bf, strlen(write_bf));
				if (ret != FR_OK) {
				    printf("Write failed: %d\n", ret);
				    f_close(&fp);
				    return 1;
				}
				memset(write_bf,'\0',sizeof(write_bf));

				f_sync(&fp);

        ret = f_close(&fp);
        if(ret != FR_OK)
        {
        	printf("f_close fail:%d\r\n", ret);
          return 1;
        }else{
        	sprintf(Tx_Buffer,"Main.t0.txt=\"SDCard write success！\"\xff\xff\xff");
        	USART1_Tx_HMIdata((uint8_t*)Tx_Buffer);
        }
				break;
    default:
				printf("错误码: %d\r\n", res);  // 其他错误（如硬件问题）
				break;
    }

	}
	else
	{
		return -1;//没发现SDCard
	}
}


