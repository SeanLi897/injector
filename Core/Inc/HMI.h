#ifndef INC_HMI_H_
#define INC_HMI_H_

#include "usart.h"
#include "battery_show.h"
#include "SD_APP.h"
#include "ff.h"


#define MAX_DISPLAY_ITEMS 6		//每页最多显示6行

// 分页控制结构体
typedef struct {
    uint16_t file_count;
    uint16_t start_index;
    uint8_t current_page;
    uint8_t total_pages;
    char** file_list;
    uint8_t show_prev_more; // 新增：是否显示前页更多
    uint8_t show_next_more; // 新增：是否显示后页更多
} PagingState;

//// 使用LRU缓存最近访问的目录页
//#define CACHE_PAGES 3
//struct {
//    uint16_t start;
//    char* files[MAX_DISPLAY_ITEMS];
//} page_cache[CACHE_PAGES];

extern uint8_t dir_display_refresh;
extern uint8_t refresh_dir;
extern uint8_t first_display_dir;
extern uint8_t current_focus_line;// 当前焦点行0-5
extern uint8_t last_focus_line;
extern uint8_t focus_key_pressed;

void HMI_init(void);
//void Display_File_List(void);
void scroll_focus_line(void);
void Refresh_Display();
void Cache_File_List();
void On_Key_Pressed();

#endif /* INC_HMI_H_ */
