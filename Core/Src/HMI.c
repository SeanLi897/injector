#include "HMI.h"
uint8_t dir_display_refresh = 0;
uint8_t refresh_dir = 0;
uint8_t first_display_dir = 1;
uint8_t current_focus_line = 0;
uint8_t last_focus_line = 0;
PagingState paging = {0};
uint8_t focus_key_pressed = 0;

enum HMI_PAGE {Main_page,File_M_page};
enum HMI_PAGE page_location;

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

FRESULT delete_file(const char* path) {
    FRESULT res;

    // 检查文件是否存在
    FILINFO fno;
    if((res = f_stat(path, &fno)) != FR_OK) {
        return res; // 返回错误码：FR_NO_FILE等
    }

    // 执行删除
    if((res = f_unlink(path)) != FR_OK) {
        return res; // 返回错误码：FR_DENIED（写保护）等
    }

    return FR_OK;
}

void On_Delete_Key_Pressed() {
		// 计算实际文件索引（需考虑分页提示符）
		uint16_t actual_index = paging.start_index + current_focus_line;
		if(paging.show_prev_more) actual_index -= 1; // 排除"..."行

		// 构造完整路径
		char full_path[FF_MAX_LFN + 3] = "0:/";
		strcat(full_path, paging.file_list[actual_index]);

		FIL fp;
		if(f_open(&fp, full_path, FA_READ) == FR_OK) {
		    f_close(&fp); // 确保文件未被占用
		}
		// 执行删除
		FRESULT res = delete_file(full_path);

		// 在删除成功后更新缓存前释放旧内存
		if(paging.file_list != NULL) {
		    for(uint16_t i=0; i<paging.file_count; i++)
		        free(paging.file_list[i]);
		    free(paging.file_list);
		}

		// 处理结果
		if(res == FR_OK) {
				// 更新文件列表缓存
				Cache_File_List();
				Refresh_Display();
				// HMI提示
				sprintf(Tx_Buffer, "Main.t0.txt=\"删除成功\"\xff\xff\xff");
		} else {
				sprintf(Tx_Buffer, "Main.t0.txt=\"错误:%d\"\xff\xff\xff", res);
		}
		USART1_Tx_HMIdata((uint8_t*)Tx_Buffer);
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
