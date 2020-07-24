/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.44                          *
*        Compiled Nov 10 2017, 08:53:57                              *
*        (c) 2017 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
// USER END

#include "EditDLG.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_FRAMEWIN_0 (GUI_ID_USER + 0x00)
#define ID_EDIT_0     (GUI_ID_USER + 0x01)
#define ID_EDIT_1     (GUI_ID_USER + 0x02)
#define ID_EDIT_2     (GUI_ID_USER + 0x03)
#define ID_EDIT_3     (GUI_ID_USER + 0x04)
#define ID_TEXT_0     (GUI_ID_USER + 0x05)

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
	{ FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 480, 854, 0, 0x0, 0 },
  { EDIT_CreateIndirect, "Edit0", ID_EDIT_0, 40, 80, 360, 50, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit1", ID_EDIT_1, 40, 150, 140, 50, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit2", ID_EDIT_2, 260, 150, 140, 50, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit3", ID_EDIT_3, 40, 220, 360, 50, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "", ID_TEXT_0, 120, 290, 370, 50, 0, 0x64, 0 },
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/**
  * @brief 对话框回调函数
  * @note 无
  * @param pMsg：消息指针
  * @retval 无
  */
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  char    EditBuff[30] ={0};
  
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    /* Edit0初始化 */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
    EDIT_SetText(hItem, "STemWIN@EmbedFire STM32F407");
    EDIT_SetFont(hItem, GUI_FONT_24_ASCII);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
	  EDIT_EnableBlink(hItem, 500, 1);
    /* Edit1初始化 */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_1);
    EDIT_SetFloatMode(hItem, 3.1415926, 0.0, 10.0, 7, GUI_EDIT_NORMAL);
    EDIT_SetFont(hItem, GUI_FONT_24_ASCII);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
	  EDIT_EnableBlink(hItem, 500, 1);
    /* Edit2初始化 */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_2);
    EDIT_SetMaxLen(hItem, 8);
    EDIT_SetHexMode(hItem, 232425, 0, 4294967295);
    EDIT_SetFont(hItem, GUI_FONT_24_ASCII);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
	  EDIT_EnableBlink(hItem, 500, 1);
    /* Edit3初始化 */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_3);
    EDIT_SetMaxLen(hItem, 28);
    EDIT_SetBinMode(hItem, 123456789, 0, 268435455);
    EDIT_SetFont(hItem, GUI_FONT_24_ASCII);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
	  EDIT_EnableBlink(hItem, 500, 1);
    /* 初始化Text0 */
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
		TEXT_SetFont(hItem, GUI_FONT_COMIC24B_ASCII);
		TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_EDIT_0: // Notifications sent by 'Edit0'
      switch(NCode) {
				case WM_NOTIFICATION_CLICKED:
          hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
          EDIT_GetText(hItem, EditBuff, 40);
          hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
          TEXT_SetText(hItem, EditBuff);
					break;
				case WM_NOTIFICATION_RELEASED:
					break;
				case WM_NOTIFICATION_VALUE_CHANGED:
					break;
      }
      break;
    case ID_EDIT_1: // Notifications sent by 'Edit1'
      switch(NCode) {
				case WM_NOTIFICATION_CLICKED:
          hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_1);
          EDIT_GetText(hItem, EditBuff, 40);
          hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
          TEXT_SetText(hItem, EditBuff);
					break;
				case WM_NOTIFICATION_RELEASED:
					break;
				case WM_NOTIFICATION_VALUE_CHANGED:
					break;
      }
      break;
    case ID_EDIT_2: // Notifications sent by 'Edit2'
      switch(NCode) {
				case WM_NOTIFICATION_CLICKED:
          hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_2);
          EDIT_GetText(hItem, EditBuff, 40);
          hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
          TEXT_SetText(hItem, EditBuff);
					break;
				case WM_NOTIFICATION_RELEASED:
					break;
				case WM_NOTIFICATION_VALUE_CHANGED:
					break;
      }
      break;
    case ID_EDIT_3: // Notifications sent by 'Edit3'
      switch(NCode) {
				case WM_NOTIFICATION_CLICKED:
          hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_3);
          EDIT_GetText(hItem, EditBuff, 40);
          hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
          TEXT_SetText(hItem, EditBuff);
					break;
				case WM_NOTIFICATION_RELEASED:
					break;
				case WM_NOTIFICATION_VALUE_CHANGED:
					break;
      }
      break;
	  }
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
 /**
  * @brief 以对话框方式间接创建控件
  * @note 无
  * @param 无
  * @retval hWin：资源表中第一个控件的句柄
  */
WM_HWIN CreateFramewin(void);
WM_HWIN CreateFramewin(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

/**
  * @brief GUI主任务
  * @note 无
  * @param 无
  * @retval 无
  */
void MainTask(void)
{
	CreateFramewin();

	while (1)
	{
		GUI_Delay(20);
	}
}

/*************************** End of file ****************************/
