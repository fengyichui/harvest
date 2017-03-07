sizeof与strlen的区别（C语言本身与运行时库函数之间的区别）
sizeof操作符--编译阶段
运行时库函数，如strlen--链接阶段


请你百度vc 双缓存 闪烁
其实主要原因就是InvalidateRect会使用默认的颜色（通常是白色）擦除图像背景，然后重新绘制，所以会感觉到闪烁（就是擦得那一下）。双缓存的原理就是先将下一幅图片绘制到一个内存dc上，绘制好了 直接贴到原来的位置，因为没有擦背景，所以不会闪。




Invalidate、UpdateWindow、RedrawWindow的区别

void Invalidate( BOOL bErase = TRUE )
Invalidate()是强制系统进行重画，但是不一定就马上进行重画。因为Invalidate()只是令窗口客户区变为无效。强制系统调用WM_PAINT，而这个消息只是通过PostMessage将该消息放入消息队列。由于WM_PAINT消息的优先级很低，它需要等消息队列中的其它消息发送完后才能被处理。

BOOL InvalidateRect( LPCRECT lpRect, BOOL bErase = TRUE );
InvalidateRect()机制上跟Invalidate相同，但可以指定重绘的区域。一般同UpdateWIndow一起使用，使无效区域立即重绘。

void UpdateWindow( )
UpdateWindow( )的作用是使窗口立即重绘，函数直接通过SendMessage发送WM_PAINT到目标窗口，从而导致窗口立即重绘。在发送之前判断GetUpdateRect(hWnd,NULL,TRUE)看有无可绘制的客户区域，如果没有，则不发送WM_PAINT。

BOOL RedrawWindow(...)
RedrawWindow()则是具有Invalidate()和UpdateWindow()的双特性。声明窗口的状态为无效，并立即更新窗口，立即调用WM_PAINT消息处理。




VC无闪烁刷屏技术的实现（双缓存）  


在实现绘图的过程中，显示的图形总是会闪烁，笔者曾经被这个问题折磨了好久，通过向高手请教，搜索资料，问题基本解决，现将文档整理出来以供大家参考.

1.显示的图形为什么会闪烁

　　我们的绘图过程大多放在OnDraw或者OnPaint函数中，OnDraw在进行屏幕显示时是由 OnPaint进行调用的。当窗口由于任何原因需要重绘时，总是先用背景色将显示区清除，然后才调用OnPaint，而背景色往往与绘图内容反差很大，这样在短时间内背景色与显示图形的交替出现，使得显示窗口看起来在闪。如果将背景刷设置成NULL，这样无论怎样重绘图形都不会闪了。当然，这样做会使得窗口的显示乱成一团，因为重绘时没有背景色对原来绘制的图形进行清除，而又叠加上了新的图形。有的人会说，闪烁是因为绘图的速度太慢或者显示的图形太复杂造成的，其实这样说并不对，绘图的显示速度对闪烁的影响不是根本性的。例如在OnDraw(CDC *pDC)中这样写：

pDC->MoveTo(0,0);

pDC->LineTo(100,100);

　　这个绘图过程应该是非常简单、非常快了吧，但是拉动窗口变化时还是会看见闪烁。其实从道理上讲，画图的过程越复杂越慢闪烁应该越少，因为绘图用的时间与用背景清除屏幕所花的时间的比例越大人对闪烁的感觉会越不明显。比如：清楚屏幕时间为1s绘图时间也是为1s，这样在10s内的连续重画中就要闪烁5次；如果清楚屏幕时间为1s不变，而绘图时间为9s，这样10s内的连续重画只会闪烁一次。这个也可以试验，在OnDraw(CDC *pDC)中这样写：

for(int i=0;i<100000;i++)

{

pDC->MoveTo(0,i);

pDC->LineTo(1000,i);

}

　　程序有点极端，但是能说明问题。

　　说到这里可能又有人要说了，为什么一个简单图形看起来没有复杂图形那么闪呢？这是因为复杂图形占的面积大，重画时造成的反差比较大，所以感觉上要闪得厉害一些，但是闪烁频率要低。那为什么动画的重画频率高，而看起来却不闪？这里，我就要再次强调了，闪烁是什么？闪烁就是反差，反差越大，闪烁越厉害。因为动画的连续两个帧之间的差异很小所以看起来不闪。如果不信，可以在动画的每一帧中间加一张纯白的帧，不闪才怪呢。

2、如何避免闪烁

　　在知道图形显示闪烁的原因之后，对症下药就好办了。首先当然是去掉MFC提供的背景绘制过程了。实现的方法很多:

　　* 可以在窗口形成时给窗口的注册类的背景刷付NULL

　　* 也可以在形成以后修改背景

static CBrush brush(RGB(255,0,0));

SetClassLong(this->m_hWnd,GCL_HBRBACKGROUND,(LONG)(HBRUSH)brush);

　　* 要简单也可以重载OnEraseBkgnd(CDC* pDC)直接返回TRUE

　　这样背景没有了，结果图形显示的确不闪了，但是显示也象前面所说的一样，变得一团乱。怎么办？这就要用到双缓存的方法了。双缓冲就是除了在屏幕上有图形进行显示以外，在内存中也有图形在绘制。我们可以把要显示的图形先在内存中绘制好，然后再一次性的将内存中的图形按照一个点一个点地覆盖到屏幕上去（这个过程非常快，因为是非常规整的内存拷贝）。这样在内存中绘图时，随便用什么反差大的背景色进行清除都不会闪，因为看不见。当贴到屏幕上时，因为内存中最终的图形与屏幕显示图形差别很小（如果没有运动，当然就没有差别），这样看起来就不会闪。

3、如何实现双缓冲

　　首先给出实现的程序，然后再解释，同样是在OnDraw(CDC *pDC)中：
	{
		//首先定义一个显示设备对象
		CDC MemDC; 
		//定义一个位图对象
		CBitmap MemBitmap;
		//随后建立与屏幕显示兼容的内存显示设备
		MemDC.CreateCompatibleDC(NULL);
		//这时还不能绘图，因为没有地方画
		//下面建立一个与屏幕显示兼容的位图，至于位图的大小嘛，可以用窗口的大小
		MemBitmap.CreateCompatibleBitmap(pDC,nWidth,nHeight);
		//将位图选入到内存显示设备中
		//只有选入了位图的内存显示设备才有地方绘图，画到指定的位图上
		CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);
		//先用背景色将位图清除干净，这里我用的是白色作为背景
		//你也可以用自己应该用的颜色
		MemDC.FillSolidRect(0,0,nWidth,nHeight,RGB(255,255,255));
		//绘图
		MemDC.MoveTo(……);
		MemDC.LineTo(……);
		//将内存中的图拷贝到屏幕上进行显示
		pDC->BitBlt(0,0,nWidth,nHeight,&MemDC,0,0,SRCCOPY);
		//绘图完成后的清理
		MemBitmap.DeleteObject();
		MemDC.DeleteDC();
	}
　　请参阅注释。

4、如何提高绘图的效率

　　实际上，在OnDraw(CDC *pDC)中绘制的图并不是所有都显示了的，例如：你在OnDraw中画了两个矩形，在一次重绘中虽然两个矩形的绘制函数都有执行，但是很有可能只有一个显示了，这是因为MFC本身为了提高重绘的效率设置了裁剪区。裁剪区的作用就是：只有在这个区内的绘图过程才会真正有效，在区外的是无效的，即使在区外执行了绘图函数也是不会显示的。因为多数情况下窗口重绘的产生大多是因为窗口部分被遮挡或者窗口有滚动发生，改变的区域并不是整个图形而只有一小部分，这一部分需要改变的就是pDC中的裁剪区了。因为显示（往内存或者显存都叫显示）比绘图过程的计算要费时得多，有了裁剪区后显示的就只是应该显示的部分，大大提高了显示效率。但是这个裁剪区是MFC设置的，它已经为我们提高了显示效率，在进行复杂图形的绘制时如何进一步提高效率呢？那就只有去掉在裁剪区外的绘图过程了。可以先用pDC->GetClipBox()得到裁剪区，然后在绘图时判断你的图形是否在这个区内，如果在就画，不在就不画。

　　如果你的绘图过程不复杂，这样做可能对你的绘图效率不会有提高。

　　具体程序大家可看以下源代码：

5、无闪烁背景图的绘制代码实现

/////////////// 无闪烁背景图绘制 //////////
//// 程序设计: icemen (温冰) 树爱兵
//////////////////////////////////////////////
BOOL CStrucView::OnEraseBkgnd(CDC* pDC)
{ 
	int nWidth;
	int nHeight;
	//CView::OnEraseBkgnd(pDC);
	CStrucDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CRect rect;
	GetWindowRect(&rect);
	nWidth = rect.Width();
	nHeight= rect.Height();
	CDC MemDC;
	CBitmap MemBitmap;
	MemDC.CreateCompatibleDC (NULL);
	MemBitmap.CreateCompatibleBitmap(pDC,nWidth,nHeight);
	CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);
	MemDC.FillSolidRect(0,0,nWidth,nHeight,RGB(255,255,255));
	///////////////////////////////////////////////////////
	//// 以上为画背景色 //////////////
	//// 以下为画背景图 //////////////
	///////////////////////////////////////////////////////
	GetClientRect(rect);
	BITMAP bm;
	CDC dcMem;
	VERIFY(m_bmp.GetObject(sizeof(bm),(LPVOID)&bm));
	dcMem.CreateCompatibleDC(pDC);
	CBitmap *pOldBMP =( CBitmap *)dcMem.SelectObject(&m_bmp);
	MemDC.BitBlt( (rect.right - bm.bmWidth)/2,
	(rect.bottom - bm.bmHeight)/2,
	bm.bmWidth,bm.bmHeight,&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldBMP);
	///////////////////////////////////////////////////////
	//// 以上为画背景图 //////////////
	///////////////////////////////////////////////////////
	pDC->BitBlt(0,0,nWidth,nHeight,&MemDC,0,0,SRCCOPY);
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();
	return TRUE;
}

/////////////// 无闪烁背景图绘制 //////////

/////////////////////////////////////////////

　　注:程序中m_bmp 为相应 的 StrucView.h中定义,为位图资源类

protected:

CBitmap m_bmp;

　　并应加上下面段,当然,你可在任何时候加入loadBitmap 子例程,也可加入文件资源,那由得你了!

CStrucView::CStrucView()

{

VERIFY(m_bmp.LoadBitmap(IDB_BITMAP3));

}