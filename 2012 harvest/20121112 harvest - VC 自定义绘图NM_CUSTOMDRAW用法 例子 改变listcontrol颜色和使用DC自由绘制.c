void CConfigNVDS::OnNMCustomdrawLcConfigNvds(NMHDR *pNMHDR, LRESULT *pResult)
{
	//此处强制把指针解释成为：LPNMLVCUSTOMDRAW(list view类型).使用其他控件则可解释为其他类型
	LPNMLVCUSTOMDRAW lpLVCustomDraw = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);
	int item    = lpLVCustomDraw->nmcd.dwItemSpec;
	int subItem = lpLVCustomDraw->iSubItem;
	int tag;
	switch(lpLVCustomDraw->nmcd.dwDrawStage)
	{
		//case CDDS_ITEMPREPAINT:
		case CDDS_ITEMPREPAINT | CDDS_SUBITEM: //筛选绘制前且绘制subitem时， 绘制前只能做该颜色等简单操作
			tag = _tcstol(m_list.GetItemText(item, NVDS_TAG_NUM), NULL, 0);
				
			if (tag <= NVDS_SYS_TAIL_TAG && tag >= NVDS_SYS_BASE_TAG)
			{
				lpLVCustomDraw->clrText = RGB(0,0,255); // 更改文本颜色
				lpLVCustomDraw->clrTextBk = CLR_DEFAULT; // 更改背景颜色
			}
			else 
			{
				lpLVCustomDraw->clrText = CLR_DEFAULT;
				lpLVCustomDraw->clrTextBk = CLR_DEFAULT;
			}
			break;
		case CDDS_ITEMPOSTPAINT | CDDS_SUBITEM: //筛选绘制后且绘制subitem时，绘制后则想做什么就做什么
			if(subItem == 0)
			{
				CDC* pDC = CDC::FromHandle(lpLVCustomDraw->nmcd.hdc); //获取DC 重新绘制，不用释放DC
				CString strItemText = _T("  ") + m_list.GetItemText(item, subItem);
		
				CRect rcItem;
				m_list.GetSubItemRect(item, subItem, LVIR_LABEL, rcItem);
				rcItem.left -= 5;
				rcItem.bottom -= 1;
				
				//绘制背景
				pDC->FillSolidRect(&rcItem, ::GetSysColor(COLOR_3DFACE)); //填充背景
				pDC->Draw3dRect(&rcItem, ::GetSysColor(COLOR_3DHIGHLIGHT), ::GetSysColor(COLOR_3DSHADOW));//设置3D样式

				//绘制文字和文字颜色和文字背景模式
				pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
				pDC->SetBkMode(TRANSPARENT);//不会在文字重绘时替换背景
				pDC->TextOut(rcItem.left, rcItem.top, strItemText);
			}
			break;
		default: 
			break;    
	}

	*pResult = 0;                      //默认值， 如果最终返回为该值，则该消息window不会再发送
	*pResult |= CDRF_NOTIFYITEMDRAW;   //要求绘制item前，发送此消息
	*pResult |= CDRF_NOTIFYSUBITEMDRAW;//要求绘制subitem前，发送此消息
	*pResult |= CDRF_NOTIFYPOSTPAINT;  //绘制结束后，发送此消息
}