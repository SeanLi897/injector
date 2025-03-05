#include "HMI.h"
uint8_t dir_display_refresh = 0;
uint8_t refresh_dir = 0;
uint8_t first_display_dir = 1;
uint8_t current_focus_line = 0;
uint8_t last_focus_line = 0;
PagingState paging = {0};
uint8_t focus_key_pressed = 0;

void HMI_init(void){
  refresh_bat_vlt();

	sprintf(Tx_Buffer,"Main.n0.val=0\xff\xff\xff");
	USART1_Tx_HMIdata((uint8_t*)Tx_Buffer);
	memset(Tx_Buffer,'\0',sizeof(Tx_Buffer));

	sprintf(Tx_Buffer,"Main.t3.txt=\"0.0\"\xff\xff\xff");
	USART1_Tx_HMIdata((uint8_t*)Tx_Buffer);
	memset(Tx_Buffer,'\0',sizeof(Tx_Buffer));

	sprintf(Tx_Buffer,"Main.t0.txt=\"启动中...\"\xff\xff\xff");
	USART1_Tx_HMIdata((uint8_t*)Tx_Buffer);
	memset(Tx_Buffer,'\0',sizeof(Tx_Buffer));

	sprintf(Tx_Buffer,"Main.t8.txt=\"\"\xff\xff\xff");
	USART1_Tx_HMIdata((uint8_t*)Tx_Buffer);
	memset(Tx_Buffer,'\0',sizeof(Tx_Buffer));

	sprintf(Tx_Buffer,"page Main\xff\xff\xff");
	USART1_Tx_HMIdata((uint8_t*)Tx_Buffer);
}

void Send_To_HMI(uint8_t index,uint8_t* name) {
    /* 构造HMI指令并发送 */
    sprintf(Tx_Buffer, "File_M.t%d.txt=\"%s\"\xff\xff\xff", index, name);
    USART1_Tx_HMIdata((uint8_t*)Tx_Buffer);
}

void Clear_HMI_Item(uint8_t index) {
    /* 清空指定位置的显示 */
    sprintf(Tx_Buffer, "File_M.t%d.txt=\"\"\xff\xff\xff", index);
    USART1_Tx_HMIdata((uint8_t*)Tx_Buffer);
}

//void Display_File_List(void){
//  FATFS fs;
//  DIR dir;
//  FILINFO fno;
//  FRESULT res;
//
//  uint8_t item_count = 0;
//  char sixth_filename[FF_MAX_LFN] = {0}; // 存储第六个文件名
//  uint8_t overflow_flag = 0;             // 溢出标志
//
//  /* 打开根目录 */
//  res = f_opendir(&dir, "/");
//  if (res != FR_OK) {
//      printf("打开目录失败！错误码：%d\n", res);
//      return;
//  }
//
//  // 遍历目录
//     for (;;) {
//         res = f_readdir(&dir, &fno);
//         if (res != FR_OK || fno.fname[0] == 0) break;
//
//         // 跳过目录
//         if (fno.fattrib & AM_DIR) continue;
//
//         // 前5个正常显示
//         if (item_count < 5) {
//             Send_To_HMI(item_count, fno.fname);
//             item_count++;
//         }
//         // 捕获第六个文件名
//         else if (item_count == 5) {
//             strncpy(sixth_filename, fno.fname, sizeof(sixth_filename)-1);
//             item_count++;
//         }
//         // 检测超过6个文件
//         else {
//             overflow_flag = 1;
//             break;
//         }
//     }
//
//     // 处理第六个显示位
//     if (item_count > 5) {
//         overflow_flag ? Send_To_HMI(5, "...") :  Send_To_HMI(5, sixth_filename);
//     } else {
//         Clear_HMI_Item(5);
//     }
//
//     // 清空未使用的显示位
//     for (uint8_t i = item_count; i < 5; i++) {
//         Clear_HMI_Item(i);
//     }
//
//     // 关闭目录
//     f_closedir(&dir);
//     dir_display_refresh = 1;
//}

void Refresh_Display() {
    // 计算显示标志
    paging.show_prev_more = (paging.start_index > 0);
    paging.show_next_more = (paging.start_index + MAX_DISPLAY_ITEMS < paging.file_count);

    // 清空显示
    for(uint8_t i=0; i<MAX_DISPLAY_ITEMS; i++)
        Clear_HMI_Item(i);

    uint8_t display_pos = 0;

    // 显示前页提示
    if(paging.show_prev_more) {
        Send_To_HMI(display_pos++, "...");
    }

    // 显示当前页文件
    for(uint8_t i=0; i < MAX_DISPLAY_ITEMS - paging.show_prev_more - paging.show_next_more; i++) {
        uint16_t actual_index = paging.start_index + i;
        if(actual_index < paging.file_count) {
            Send_To_HMI(display_pos++, paging.file_list[actual_index]);
        }
    }

    // 显示后页提示
    if(paging.show_next_more) {
        Send_To_HMI(MAX_DISPLAY_ITEMS-1, "...");
    }
}

void Cache_File_List() {
//    FATFS fs;
    DIR dir;
    FILINFO fno;
    uint16_t count = 0;

    // 释放旧内存
    if(paging.file_list != NULL) {
        for(uint16_t i=0; i<paging.file_count; i++)
            free(paging.file_list[i]);
        free(paging.file_list);
    }

    // 第一次遍历统计数量
//    f_mount(&fs, "0:", 1);
    f_opendir(&dir, "/");
    while(f_readdir(&dir, &fno) == FR_OK && fno.fname[0]) {
        if(!(fno.fattrib & AM_DIR))
            count++;
    }
    f_closedir(&dir);

    // 分配内存
    paging.file_list = malloc(count * sizeof(char*));
    paging.file_count = 0;

    // 第二次遍历存储文件名
    f_opendir(&dir, "/");
    while(f_readdir(&dir, &fno) == FR_OK && fno.fname[0]) {
        if(!(fno.fattrib & AM_DIR)) {
            paging.file_list[paging.file_count] = malloc(FF_MAX_LFN);
            strcpy(paging.file_list[paging.file_count], fno.fname);
            paging.file_count++;
        }
    }
    f_closedir(&dir);
//    f_mount(NULL, "0:", 0);

    // 计算分页
    paging.total_pages = (paging.file_count + MAX_DISPLAY_ITEMS-1) / MAX_DISPLAY_ITEMS;
    paging.start_index = 0;
    paging.current_page = 1;
}

void On_Key_Pressed() {
		// 在翻页操作前范围检查
		if(paging.start_index < 0) paging.start_index = 0;
		if(paging.start_index >= paging.file_count)
				paging.start_index = paging.file_count - MAX_DISPLAY_ITEMS;

    // 处理前页翻页
    if(current_focus_line == 0 && paging.show_prev_more) {
        paging.start_index = (paging.start_index >= (MAX_DISPLAY_ITEMS-1)) ?
                            paging.start_index - (MAX_DISPLAY_ITEMS-1) : 0;
        paging.current_page--;
        Refresh_Display();
    }
    // 处理后页翻页
    else if(current_focus_line == (MAX_DISPLAY_ITEMS-1) && paging.show_next_more) {
        if(paging.start_index + MAX_DISPLAY_ITEMS < paging.file_count) {
            paging.start_index += (MAX_DISPLAY_ITEMS-1);
            paging.current_page++;
            Refresh_Display();
        }
    }
}

void scroll_focus_line(void){
	sprintf(Tx_Buffer,"File_M.t%d.bco=65535\xff\xff\xff",last_focus_line);
	USART1_Tx_HMIdata((uint8_t*)Tx_Buffer);
	sprintf(Tx_Buffer,"File_M.t%d.pco=0\xff\xff\xff",last_focus_line);
	USART1_Tx_HMIdata((uint8_t*)Tx_Buffer);

	sprintf(Tx_Buffer,"File_M.t%d.bco=825\xff\xff\xff",current_focus_line);
	USART1_Tx_HMIdata((uint8_t*)Tx_Buffer);
	sprintf(Tx_Buffer,"File_M.t%d.pco=65535\xff\xff\xff",current_focus_line);
	USART1_Tx_HMIdata((uint8_t*)Tx_Buffer);
}
