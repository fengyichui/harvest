/**
 * [History]
 * Create time: long long ago
 * by lq
 *
 * 20121229: 修改 ReadStdout()函数，ReadFile函数的问题，它的入口参数中“缓冲”和“缓冲大小”一定要匹配，否则偶尔会返回失败(即是应该成功)。
 */




#define BIG_ENDIAN 1
#if BIG_ENDIAN
//大端
#define GET_CHAR(p)  ((p)[0])
#define GET_SHORT(p) (((unsigned short)(unsigned char)((p)[0]) << 8) | (unsigned short)(unsigned char)((p)[1]))
#define GET_LONG(p)  (((unsigned long)(GET_SHORT(p)) << 16) | (unsigned long)(GET_SHORT((p)+2)))

#define GET_CHAR_PP(p)  ((p) += 1, GET_CHAR ((p)-1))
#define GET_SHORT_PP(p) ((p) += 2, GET_SHORT((p)-2))
#define GET_LONG_PP(p)  ((p) += 4, GET_LONG ((p)-4))


#define SET_CHAR(p, n)  do{p[0] = (unsigned char)(n);}while(0)
#define SET_SHORT(p, n) do{(p)[0] = (unsigned char)((n) >> 8);(p)[1] = (unsigned char)(n);}while(0)
#define SET_LONG(p, n)  do{SET_SHORT(p, (n)>>16); SET_SHORT((p)+2, n);}while(0)

#define SET_CHAR_PP(p, n)  do{SET_CHAR(p, n);  (p) += 1;}while(0)
#define SET_SHORT_PP(p, n) do{SET_SHORT(p, n); (p) += 2;}while(0)
#define SET_LONG_PP(p, n)  do{SET_LONG(p, n);  (p) += 4;}while(0)

#else
//小端
#define GET_SHORT(p) (((unsigned short)(unsigned char)((p)[1]) << 8) | (unsigned short)(unsigned char)((p)[0]) )
#define GET_LONG(p) (((unsigned long)(GET_SHORT((p)+2)) << 16) | (unsigned long)(GET_SHORT(p)))
#endif


#define max(x, y) (((int)(x) > (int)(y)) ? (x) : (y))
#define min(x, y) (((int)(x) < (int)(y)) ? (x) : (y))

unsigned char *SeekArrayFirst(const unsigned char *originalArray, int originalArrayLen,
                                 const unsigned char *seekArray,     int seekArrayLen)
{
    int originalArrayIndex, seekArrayIndex;
    int flag;

    if(originalArrayLen <= 0 ||
       seekArrayLen     <= 0 ||
       originalArrayLen <  seekArrayLen)
    {
        return (unsigned char *)NULL;
    }
    for(originalArrayIndex = 0; originalArrayIndex < originalArrayLen - seekArrayLen + 1; originalArrayIndex++)
    {
        if(originalArray[originalArrayIndex] == seekArray[0])
        {
            seekArrayIndex = 1;
            flag = 1;
            while(seekArrayIndex < seekArrayLen)
            {
                if(originalArray[originalArrayIndex + seekArrayIndex] != seekArray[seekArrayIndex])
                {
                    flag = 0;
                    break;
                }
                seekArrayIndex++;
            }
            if(flag == 1)
            {
                return (unsigned char *)&originalArray[originalArrayIndex];
            }
        }
    }
    return (unsigned char *)NULL;
}

static unsigned char XOR_Verify(const unsigned char *pdatas,int length)
{
    unsigned char result = 0;
    int i;
    for(i = 0; i < length; i++)
    {
        result ^= pdatas[i];
    }
    return result;
}

int CharToHex(char asc)
{
    if(asc >= '0' && asc <= '9')
        return (asc - '0');
    else if(asc >= 'A' && asc <= 'F')
        return (asc - 'A' + 10);
    else if(asc >= 'a' && asc <= 'f')
        return (asc - 'a' + 10);
    else
        return -1;
}

char *HexToStr(const unsigned char *phex, int hexLen, char *pstr, int strMaxLen)
{
    int i,j;
    unsigned char bcd_h,bcd_l;
    const unsigned char bcd_asc_table[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

    assert(strMaxLen >= 3 * hexLen);
    assert(NULL != phex);
    assert(NULL != pstr);

    if(strMaxLen < 3 * hexLen)
        return NULL;

    if(NULL == phex || NULL == pstr)
        return NULL;

    for(i=0, j=0; i<hexLen; i++)
    {
        bcd_h = (phex[i] >> 4) & 0x0F;
        bcd_l =  phex[i] & 0x0F;
        pstr[j++] = bcd_asc_table[bcd_h];
        pstr[j++] = bcd_asc_table[bcd_l];
        pstr[j++] = ' ';
    }
    pstr[j-1] = '\0';
    return pstr;
}

//"12 13 aa a0 b c ...ef3" => 12 13 aa a0 0b 0c ef 03
int StrToHex(const char *pstr, unsigned char *phex, int hexMaxLen)
{
    char ichar;
    int  hex_h, hex_l, i;

    assert(NULL != phex);
    assert(NULL != pstr);

    for(i = 0; '\0' != (ichar = *pstr++); )
    {
        if(-1 != (hex_h = CharToHex(ichar)))
        {
            if(++i > hexMaxLen)
                return -1;
            if('\0' != (ichar = *pstr++))
            {
                if(-1 != (hex_l = CharToHex(ichar)))
                    *phex++ = (unsigned char)(hex_h*16 + hex_l);
                else
                    *phex++ = (unsigned char)hex_h;
            }
            else
            {
                *phex++ = (unsigned char)hex_h;
                return i;
            }
        }
    }
    return i;
}

//通过磁盘的名字 查找磁盘的驱动号：返回C D E 失败返回 0
char FindDriveByName(const char *pname)
{
    char localDrive[128] = {0};
    DWORD allDrivelen, lenIndex, driveLen;

    allDrivelen = GetLogicalDriveStrings(sizeof(localDrive),localDrive); //获取计算机的所有的盘符
    for(lenIndex = 0; lenIndex < allDrivelen; lenIndex += driveLen+1)    //遍历所有盘符
    {
        char *pdrive = localDrive + lenIndex;
        if(DRIVE_REMOVABLE == GetDriveType(pdrive))                      //查看当前磁盘类型
        {
            char plabal[128] = {0};
            char buff[128] = {0};
            DWORD volumeSerialNumber;  // address of volume serial number
            DWORD maximumComponentLength;
            DWORD fileSystemFlags;
            GetVolumeInformation(pdrive, plabal, 100, &volumeSerialNumber,
                                 &maximumComponentLength, &fileSystemFlags,
                                 buff, 100);                             //查看磁盘信息
            if(0 == memcmp((void *)plabal, (void *)pname, strlen(pname)))//比较
                return pdrive[0];  //成功
        }
        driveLen = strlen(pdrive);
    }
    return '\0'; //失败
}

//liqiang
//返回值既是：文件大小
long __inline FileSize(FILE *stream)
{
    long curpos, length;
    curpos = ftell(stream);          //ftell相对当前文件的偏移量
    fseek(stream, 0L, SEEK_END);
    length = ftell(stream);
    fseek(stream, curpos, SEEK_SET);
    return length;
}


/**
 ****************************************************************************************
 * @brief decade convert to bcd
 * @param[in]    decade           0 ~ 99
 ****************************************************************************************
 */
static uint8_t dec2bcd(uint8_t decade)
{
    uint8_t bcd;

    bcd = ((decade/10) << 4) + (decade%10);

    return bcd;
}

/**
 ****************************************************************************************
 * @brief bcd convert to decade
 * @param[in]    bcd           0 ~ 99
 ****************************************************************************************
 */
static uint8_t bcd2dec(uint8_t bcd)
{
    uint8_t decade;

    decade = ((bcd >> 4) & 0x0F) * 10 + (bcd & 0x0F);

    return decade;
}


const unsigned long all_bit_table[33] =
{   0,
    0x1,        0x3,        0x7,        0xF,
    0x1F,       0x3F,       0x7F,       0xFF,
    0x1FF,      0x3FF,      0x7FF,      0xFFF,
    0x1FFF,     0x3FFF,     0x7FFF,     0xFFFF,
    0x1FFFF,    0x3FFFF,    0x7FFFF,    0xFFFFF,
    0x1FFFFF,   0x3FFFFF,   0x7FFFFF,   0xFFFFFF,
    0x1FFFFFF,  0x3FFFFFF,  0x7FFFFFF,  0xFFFFFFF,
    0x1FFFFFFF, 0x3FFFFFFF, 0x7FFFFFFF, 0xFFFFFFFF,
};

const unsigned char bit_shift_table[32] =
{
    0x1,        0x2,        0x4,        0x8,
    0x10,       0x20,       0x40,       0x80,
    0x100,      0x200,      0x400,      0x800,
    0x1000,     0x2000,     0x4000,     0x8000,
    0x10000,    0x20000,    0x40000,    0x80000,
    0x100000,   0x200000,   0x400000,   0x800000,
    0x1000000,  0x2000000,  0x4000000,  0x8000000,
    0x10000000, 0x20000000, 0x40000000, 0x80000000,
};

const unsigned char even_parity_table[256] =
{
    0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,
    1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,
    1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,
    0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,
    1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,
    0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,
    0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,
    1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,
    1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,
    0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,
    0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,
    1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,
    0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,
    1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,
    1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,
    0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,
};


#define offsetof(TYPE, MEMBER) ((unsigned int) &((TYPE *)0)->MEMBER)
#define container_of(ptr, type, member) ((type *)((char *)ptr - offsetof(type,member)))

#define is_power_of_2(x)    ((x) != 0 && (((x) & ((x) - 1)) == 0))

//  当event不为0时候退出，结果：result = -1超时， 0事件成立未超时
#define EVENT_TIMEOUT                      0xffffff
#define WAIT_FOR_EVENT(event, result)                           \
do{                                                             \
    int timeout;                                                \
    result = -1;                                                \
    for(timeout=0; timeout<EVENT_TIMEOUT; ++timeout)            \
    {                                                           \
        if(event)                                               \
        {                                                       \
            result = 0;                                         \
            break;                                              \
        }                                                       \
    }                                                           \
}while(0)

#define FIX_DELAY(time_out) do{int i;  for(i = 0; i < (time_out); ++i){__nop();}}while(0)


// #define SWITCH(event_pool) {const unsigned int ___my_switch_event_pool = (event_pool); int ___my_switch_have_case = 0;
// #define CASE(event_mask)  switch((___my_switch_event_pool & (event_mask))){{ case (event_mask)
// #define BREAK ___my_switch_have_case = 1;}}
// #define DEFAULT switch(___my_switch_have_case){ case (0)

#define SWITCH(event_pool)      {const unsigned long ___my_switch_event_pool = (event_pool); int ___my_switch_have_case = 0;
#define CASE(event)             if(___my_switch_event_pool & (event)){{___my_switch_have_case = 1;
#define CASE_ALL(event_mask)    if(___my_switch_event_pool & (event_mask) == (event_mask)){{___my_switch_have_case = 1;
#define CASE_MASK(event, mask)  if(___my_switch_event_pool & (mask) == (event)){{___my_switch_have_case = 1;
#define BREAK                   }}
#define DEFAULT                 if(___my_switch_have_case == 0){

#define ALIGN_SIZE 4
#define ALIGN_TO_UP(x)                 (((unsigned long)(x) + (ALIGN_SIZE - 1ul)) & ~(ALIGN_SIZE - 1ul))
#define ALIGN_TO_DOWN(x)               ((unsigned long)(x) & ~(ALIGN_SIZE - 1ul))

//获取应用程序根目录
CString GetAppPath()
{
    TCHAR modulePath[MAX_PATH];
    GetModuleFileName(NULL, modulePath, MAX_PATH);
    CString strModulePath(modulePath);
    strModulePath = strModulePath.Left(strModulePath.ReverseFind(_T('\\')));
    return strModulePath;
}

/*
 * 强制转换类
 * 可实现 1、UTF8 UTF7 UNICODE ANSI 编码的相互转换
 *        2、hex dec bool值和字符串的相互转换
 * 优点 自动实现内存的申请释放 以一种优美的方式实现数据的转换 一行搞定所有
 * 注意 可以使用CStringA和CStringW() 的构造，可以实现UNICODE和ANSI的互相转换
 */
class _cast
{
#define MAX_BUFF_SIZE 1024
#define D_INIT :pwchar(NULL), pchar(NULL)
protected:
    char    *pchar;   //只有在返回多字符编码的时候才使用
    wchar_t *pwchar;  //存储Unicode编码的数据 中间格式 其他格式的需要转换成该格式

public:
    //类内的类型 有种类内成员的感觉 必须通过 _cast::UTF8_t 访问
    typedef const int*     UTF8_t;
    typedef const long*    UTF7_t;
    typedef const wchar_t* UNICODE_t;
    typedef const char*    ANSI_t;
    typedef const char*    MultiByte_t;

    typedef wchar_t*       _UNICODE_t;
    typedef char*          _ANSI_t;
    typedef char*          _MultiByte_t;

    typedef long           DEC_t;
    typedef unsigned long  HEX_t;
    typedef char           BOOL_t;
public:
    ~_cast(){if(pwchar) delete [] pwchar; if(pchar) delete [] pchar;}

    //输入格式选择 构造 默认宽字符则unicode;多字符则ANSI. 如果其他格式输入 则使用 UTF8_t、UTF7_t、UNICODE_t、ANSI
    _cast(UTF8_t    putf8)    D_INIT {pwchar = Mb2Unicode(CP_UTF8, (const char *)putf8);}
    _cast(UTF7_t    putf7)    D_INIT {pwchar = Mb2Unicode(CP_UTF7, (const char *)putf7);}
    _cast(UNICODE_t punicode) D_INIT {pwchar = new wchar_t [wcslen(punicode) + 1]; wcscpy(pwchar, punicode);}
    _cast(ANSI_t    pansi)    D_INIT {pwchar = Mb2Unicode(CP_ACP, (const char *)pansi);}

    //输出格式选择 函数
    _UNICODE_t   ToUnicode()   {return pwchar;}
    _MultiByte_t ToMbANSI()    {return pchar = Unicode2Mb(CP_ACP,  pwchar);}
    _MultiByte_t ToMbUTF7()    {return pchar = Unicode2Mb(CP_UTF7, pwchar);}
    _MultiByte_t ToMbUTF8()    {return pchar = Unicode2Mb(CP_UTF8, pwchar);}

    //输出格式选择 强制转换 宽字符则unicode;多字符则ANSI. 如果需要其他的多字节编码 则使用上面的函数
    operator _UNICODE_t()      {return pwchar;}
    operator _ANSI_t()         {return pchar = Unicode2Mb(CP_ACP, pwchar);}

    //数据输入 构造 十进制 或 十六进制
    _cast(DEC_t  dec, const wchar_t *format = L"%d")   D_INIT {pwchar = new wchar_t [MAX_BUFF_SIZE]; swprintf(pwchar, format, dec);}
    _cast(HEX_t  hex, const wchar_t *format = L"0x%X") D_INIT {pwchar = new wchar_t [MAX_BUFF_SIZE]; swprintf(pwchar, format, hex);}
    _cast(BOOL_t bol) D_INIT {pwchar = new wchar_t [6]; bol == 0 ? wcscpy(pwchar, L"False") : wcscpy(pwchar, L"True");}
    //数据输出  强制转换
    operator DEC_t ()  {return wcstol(pwchar, NULL, 0);}
    operator HEX_t ()  {return wcstoul(pwchar, NULL, 0);}

public:
    // @ code page:
    // ->CP_UTF7; ->CP_UTF8; ->CP_ACP: ANSI编码，汉语可以认为是GB2312。
    static LPWSTR Mb2Unicode(UINT mbCodePage, LPCSTR lpcszStr){
        DWORD dwMinSize = MultiByteToWideChar (CP_ACP, 0, lpcszStr, -1, NULL, 0);
        LPWSTR lpwszStr = new wchar_t [dwMinSize];
        MultiByteToWideChar (mbCodePage, 0, lpcszStr, -1, lpwszStr, dwMinSize);  //该函数会自动向数据中加'/0'
        return lpwszStr;
    }

    static LPSTR Unicode2Mb(UINT mbCodePage, LPCWSTR lpcwszStr){
        DWORD dwMinSize = WideCharToMultiByte(mbCodePage, 0, lpcwszStr, -1, NULL, 0, NULL, FALSE);
        LPSTR lpszStr = new char [dwMinSize];
        WideCharToMultiByte(mbCodePage, 0, lpcwszStr, -1, lpszStr, dwMinSize, NULL, FALSE);
        return lpszStr;
    }

    static LPSTR Mb2Mb(UINT mbSrcCodePage, UINT mbDstCodePage, LPCSTR lpcszSrcStr){
        LPCWSTR lpcwszStr = Mb2Unicode(mbSrcCodePage, lpcszSrcStr);
        LPSTR lpszStr = Unicode2Mb(mbDstCodePage, lpcwszStr);
        delete lpcwszStr;
        return lpszStr;
    }
};


//遍历标准库的容器的元素 通过迭代器返回
#define FOR_EACH_STL(ior, container, container_type)\
    for(container_type::iterator ior=container.begin(); ior != container.end(); ++ior)
#define FOR_EACH_STL_CONST(ior, container, container_type)\
    for(container_type::const_iterator ior=container.begin(); ior != container.end(); ++ior)

//遍历标准库的容器 通过元素的指针返回 需要外部定义一个指向容器内元素的指针
#define FOR_EACH_STL_PTR(ptr, container, container_type)\
    for(container_type::iterator ior = container.begin(); ior == container.end() ? false : (ptr = &*ior, true); ++ior)
#define FOR_EACH_STL_PTR_CONST(ptr, container, container_type)\
    for(container_type::const_iterator ior = container.begin(); ior == container.end() ? false : (ptr = &*ior, true); ++ior)

//遍历CList， _pdata：数据指针，_list：CList变量
#define FPR_EACH_CLIST(_pdata, _list) \
        for(POSITION pos = (_list).GetHeadPosition(); pos ? ((_pdata)=&(_list).GetNext(pos), TRUE) : FALSE;)


//跳过所有非数字字符，可以和_tcstol配合使用，_tcstol只能跳过空白字符其他字符无法跳过
void skip_nonnum(TCHAR **p)
{
    //一句话，简短不饿
    for(TCHAR ch = **p; !(ch == _T('\0') || (ch >= _T('0') && ch <= _T('9'))); ch = *++(*p));
}

//遍历所有的字符串table，如果有匹配的则返回匹配的table的编号，否则返回-1
#define WALK_INDEX_TABLE(x, table) walk_index_table(x, table, sizeof(table) / sizeof(table[0]))
static inline int walk_index_table(CString find, const TCHAR **table, int dimension)
{
    find.MakeLower();
    for(int i=0; i<dimension; ++i)
        //if(((CString)table[i]).MakeLower().Find(find) != -1)
        if(((CString)table[i]).MakeLower() == find)
            return i;
    return -1;
}


/*
 * 读取控制台应用程序的标准输出数据
 * 因为大多数控制台程序输出的文本都是多字符的 所以该版本为多字符版本
 * location :该控制台程序所在的目录，0系统目录，-1应用程序文件夹内，other自定义目录
 * cmdLine  :命令行，即ipconfig /all 会传入到控制台应用程序的main(int argc, char *argv[])的argv[0] argv[1]...
 * 使用条件:控制台应用程序执行结束集退出
 */
int ReadStdout(char *szCmdLine, char *szResultData, ULONG iMaxlen, long xLocation = 0)
{
    SECURITY_ATTRIBUTES sa;
    HANDLE hRead, hWrite;
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    CStringA cmd;
    ULONG len = 0;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;
    if (!CreatePipe(&hRead,&hWrite,&sa,0)) { //创建管道
        AfxMessageBox(_T("Error[ReadStdout()]: CreatePipe()"));
        return -1;
    }

    si.cb = sizeof(STARTUPINFO);
    GetStartupInfoA(&si);
    si.hStdError = si.hStdOutput = hWrite;         //重定向输出
    si.wShowWindow = SW_HIDE;
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    if(xLocation == 0){ //系统的路径Windows/
        cmd = szCmdLine;
    }
    else if(xLocation == -1){ //当前应用程序文件夹下
        char modulePath[MAX_PATH];
        GetModuleFileNameA(NULL, modulePath, MAX_PATH);
        cmd = modulePath;
        cmd = cmd.Left(cmd.ReverseFind('\\')) + "\\" + szCmdLine;
    }
    else //用户自定义文件夹
        cmd = CString((char *)xLocation) + "\\" + szCmdLine;

    if (!CreateProcessA(NULL, (LPSTR)(LPCSTR)cmd, NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi)) {
        AfxMessageBox(_T("Error[ReadStdout()]: CreateProcess()"));
        CloseHandle(hWrite); CloseHandle(hRead);
        return -1;
    }
    //等待应用程序退出
    WaitForSingleObject(pi.hProcess, INFINITE);

    if(!ReadFile(hRead, szResultData, iMaxlen, &len, NULL)){
        AfxMessageBox(_T("Error[ReadStdout()]: ReadFile()"));
        CloseHandle(hWrite); CloseHandle(hRead);
        return -1;
    }

    szResultData[len] = '\0';
    CloseHandle(hWrite); CloseHandle(hRead);
    return (int)len;
}

//开机自动启动
void AutoStartup(TCHAR *name)
{
    HKEY hKey;
    LPCTSTR lpRun = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
    long lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_WRITE, &hKey);
    if(lRet == ERROR_SUCCESS)
    {
        TCHAR pFileName[MAX_PATH] = {0};
        DWORD dwRet = GetModuleFileName(NULL, pFileName, MAX_PATH);
        lRet = RegSetValueEx(hKey, name, 0, REG_SZ, (BYTE *)pFileName, dwRet * sizeof(TCHAR));
        RegCloseKey(hKey);
        if(lRet != ERROR_SUCCESS)
            AfxMessageBox(_T("系统参数错误,不能随系统启动"));
    }
}


//以下两个函数 等待过程中忽略所有的用户输入(鼠标键盘)
//如果是等待线程则 创建线程时使用
//   1、hThreadDealWith = (HANDLE)_beginthreadex(NULL, 0, ThreadRcvFunc, this, 0, 0);
//   2、pThreadDealWith = AfxBeginThread(ThreadDealWithFunc, (void *)this);
//      pThreadDealWith->m_bAutoDelete = FALSE; //关键点，线程结束后不自动删除目标，否则在等待时候会提示无效句柄，记得手动delete该目标
//在“UI的线程”中等待许多事件变成有信号状态 参数handle_array: HANDLE handle_array[2] = {hThreadDealWith, hThreadRcv};
static inline BOOL UI_WaitForMultipleObjects(HANDLE *handle_array, int n)
{
    DWORD result;
    MSG msg;
    int index;
    while(TRUE)
    {
        result = MsgWaitForMultipleObjects(n, handle_array, FALSE, INFINITE, QS_ALLINPUT);
        if(result == WAIT_FAILED)
        {
            ASSERT(0);//可能是无效句柄
            return FALSE;
        }
        index = result - WAIT_OBJECT_0;
        if(index < n && index >= 0)//handle_array中的有效
        {
            TRACE("[%p/%d] get signal\n", handle_array[index], n);
            if(--n == 0)
                return TRUE;  //结束

            if(n != index)
                handle_array[index] = handle_array[n];//删除该句柄
        }
        else if(index == n)//QS_ALLINPUT中的有效
        {
            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) //进入消息循环
            {
                if (msg.message==WM_QUIT||msg.message==WM_CLOSE)
                    return FALSE;

                //不响应所有的用户输入(鼠标键盘)
                if(!(msg.message >= WM_MOUSEFIRST && msg.message <= WM_MOUSELAST ||
                    msg.message >= WM_KEYFIRST   && msg.message <= WM_KEYLAST))
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }
        }
        else//其他的有效
        {
            TRACE("WAIT_ABANDONED get signal\n");
        }
    }

    return TRUE;
}

//在“UI的线程”中等待某个事件变成有信号状态
static inline BOOL UI_WaitForSingleObject(HANDLE hHandle)
{
    return UI_WaitForMultipleObjects(&hHandle, 1);
}

//获取上次错误的文本描述
CString GetLastErrorMessage(void)
{
    CString strErrorMassage;
    DWORD dwError = GetLastError();

    HLOCAL hlocal = NULL;   // Buffer that gets the error message string

    // Use the default system locale since we look for Windows messages.
    // Note: this MAKELANGID combination has 0 as value
    DWORD systemLocale = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);

    // Get the error code's textual description
    BOOL fOk = FormatMessage(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS |
        FORMAT_MESSAGE_ALLOCATE_BUFFER,
        NULL, dwError, systemLocale,
        (PTSTR) &hlocal, 0, NULL);

    if (!fOk) {
        // Is it a network-related error?
        HMODULE hDll = LoadLibraryEx(TEXT("netmsg.dll"), NULL,
            DONT_RESOLVE_DLL_REFERENCES);

        if (hDll != NULL) {
            fOk = FormatMessage(
                FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS |
                FORMAT_MESSAGE_ALLOCATE_BUFFER,
                hDll, dwError, systemLocale,
                (PTSTR) &hlocal, 0, NULL);
            FreeLibrary(hDll);
        }
    }

    if (fOk && (hlocal != NULL)) {
        strErrorMassage = (PCTSTR) LocalLock(hlocal);
        LocalFree(hlocal);
    } else {
        strErrorMassage = TEXT("Unknown Error.");
    }

    // Reset last error
    SetLastError(dwError);
    return strErrorMassage;
}

void split(const CString &strOriginal, const CString &strSeparator, list<CString> &lstResult)
{
    TCHAR *cstr, *p;
    cstr = new TCHAR[strOriginal.GetLength() + 1];
    _tcscpy(cstr, strOriginal.GetString());
    p = _tcstok(cstr, strSeparator);
    while(p != NULL)
    {
        lstResult.push_back(p);
        p = _tcstok(NULL, strSeparator);
    }
}

void join(const list<CString> &lstOriginal, const CString &strSeparator, CString &strResult)
{
    for(list<CString>::const_iterator itr=lstOriginal.cbegin(); itr != lstOriginal.cend(); ++itr)
    {
        strResult += *itr + strSeparator;
    }
}

BOOL Relative2AbsolutePath(LPCTSTR szRelPath, LPTSTR szAbsPath, DWORD nAbsMaxLen)
{
    TCHAR szOPath[MAX_PATH] = {0};

    if(FALSE == GetCurrentDirectory(MAX_PATH, szOPath))
        return FALSE;

    if(FALSE == SetCurrentDirectory(szRelPath))
        return FALSE;

    if(FALSE == GetCurrentDirectory(nAbsMaxLen, szAbsPath))
        return FALSE;

    if(FALSE == SetCurrentDirectory(szOPath))
        return FALSE;

    return TRUE;
}

//前导0的个数
int clz (uint32_t x)
{
    int n = 0;
    if (x == 0) return 32;
    if ((x & 0xFFFF0000) == 0) { n += 16; x <<= 16; }
    if ((x & 0xFF000000) == 0) { n +=  8; x <<=  8; }
    if ((x & 0xF0000000) == 0) { n +=  4; x <<=  4; }
    if ((x & 0xC0000000) == 0) { n +=  2; x <<=  2; }
    if ((x & 0x80000000) == 0) { n +=  1; }
    return n;
}

//后导0的个数
int ctz (uint32_t x)
{
    int n = 0;
    if (x == 0) return 32;
    if ((x & 0x0000FFFF) == 0) { n += 16; x >>= 16; }
    if ((x & 0x000000FF) == 0) { n +=  8; x >>=  8; }
    if ((x & 0x0000000F) == 0) { n +=  4; x >>=  4; }
    if ((x & 0x00000003) == 0) { n +=  2; x >>=  2; }
    if ((x & 0x00000001) == 0) { n +=  1; }
    return n;
}



