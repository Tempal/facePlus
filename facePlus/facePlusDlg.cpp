
// facePlusDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "facePlus.h"
#include "facePlusDlg.h"
#include "afxdialogex.h"
#include "include\curl\curl.h"
#include "json/json.h"
#include "Base64.h"
#include <fstream>
#include<string>
#include <sstream>
#include <iostream>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CfacePlusDlg 对话框



CfacePlusDlg::CfacePlusDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FACEPLUS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CfacePlusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CfacePlusDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN_LEFT, &CfacePlusDlg::OnBnClickedOpenLeft)
	ON_BN_CLICKED(IDC_OPENT_RIGHT, &CfacePlusDlg::OnBnClickedOpentRight)
	ON_BN_CLICKED(IDC_COMPARE, &CfacePlusDlg::OnBnClickedCompare)
	ON_BN_CLICKED(IDC_REQUIRE, &CfacePlusDlg::OnBnClickedRequire)
	ON_BN_CLICKED(IDC_REQUIRE2, &CfacePlusDlg::OnBnClickedRequire2)
END_MESSAGE_MAP()


// CfacePlusDlg 消息处理程序

BOOL CfacePlusDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	std::ifstream myfile;
	myfile.open("./in.txt", std::ios::in);
	if (!myfile)
	{
		MessageBox(_T("请在facePlus.exe文件夹下面新建in.txt文件\r\n并且在in.txt文件写入两行：api_ID、api_key\r\n上面上个信息可以在https://console.bce.baidu.com/ai/申请"));
		return -1;
	}
	else {
		std::string temp;
		getline(myfile, temp);
		SetDlgItemText(IDC_CLIENT_ID, temp.c_str());
		getline(myfile, temp);
		SetDlgItemText(IDC_CLIENT_SECRET, temp.c_str());
	}
	myfile.close();
	CURLcode res = curl_global_init(CURL_GLOBAL_ALL);
	if (CURLE_OK != res)
	{
		MessageBox(_T("初始化出错"));
		return 1;
	}
	get_access_token(sToken,AK,SK);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CfacePlusDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CfacePlusDlg::OnPaint()
{
	showPiture(true);
	showPiture(false);
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CfacePlusDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CfacePlusDlg::showPiture(bool bRight)
{
	if ((bRight == true) && (sRightFilePath.IsEmpty()!=true)) {
		CImage myImage;
		myImage.Load(sRightFilePath);
		CRect rect;
		CWnd *pWnd = GetDlgItem(IDC_PICTURE_RIGHT);  //(这是在此资源创建的类的内部, 若是在外部, 可先通过获得CMainFrame的指针, 再通过pMianFrame->GetDlgItem(IDCk_MY_PIC)来获取)
		CDC *pDC = pWnd->GetDC();
		pWnd->GetClientRect(&rect);
		pDC->SetStretchBltMode(STRETCH_HALFTONE);
		myImage.Draw(pDC->m_hDC, rect);
		ReleaseDC(pDC);
		myImage.Destroy();
	}
	else if ((bRight==false) && (sLeftFilePath.IsEmpty() != true)) {
		CImage myImage;
		myImage.Load(sLeftFilePath);
		CRect rect;
		CWnd *pWnd = GetDlgItem(IDC_PICTURE_LEFT);  //(这是在此资源创建的类的内部, 若是在外部, 可先通过获得CMainFrame的指针, 再通过pMianFrame->GetDlgItem(IDCk_MY_PIC)来获取)
		CDC *pDC = pWnd->GetDC();
		pWnd->GetClientRect(&rect);
		pDC->SetStretchBltMode(STRETCH_HALFTONE);
		myImage.Draw(pDC->m_hDC, rect);
		ReleaseDC(pDC);
		myImage.Destroy();
	}
}

void CfacePlusDlg::OnBnClickedOpenLeft()
{
	// TODO: 在此添加控件通知处理程序代码
	CString filter;
	filter = "所有文件(*.bmp,*.png,*.gif,*.tiiff)|*.bmp;*.png;*.jpg;*.gif;*.tiff| BMP(*.bmp)|*.bmp| JPG(*.jpg)|*.jpg|PNG(*.png)|*.png|GIF(*.gif)|*.gif| TIFF(*.tiff)|*.tiff||";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter, NULL);
	if (dlg.DoModal() == IDOK)
	{
		//打开对话框获取图像信息
		sLeftFilePath = dlg.GetPathName();     //获取文件路径名   如D:\pic\abc.bmp
		showPiture(false);
	}
}


void CfacePlusDlg::OnBnClickedOpentRight()
{
	// TODO: 在此添加控件通知处理程序代码
	CString filter;
	filter = "所有文件(*.bmp,*.png,*.gif,*.tiiff)|*.bmp;*.png;*.jpg;*.gif;*.tiff| BMP(*.bmp)|*.bmp| JPG(*.jpg)|*.jpg|PNG(*.png)|*.png|GIF(*.gif)|*.gif| TIFF(*.tiff)|*.tiff||";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter, NULL);
	if (dlg.DoModal() == IDOK)
	{
		//打开对话框获取图像信息
		sRightFilePath = dlg.GetPathName();     //获取文件路径名   如D:\pic\abc.bmp
		showPiture(true);
	}
}

/**
 * curl发送http请求调用的回调函数,获取百度token，回调函数中对返回的json格式的body进行了解析，解析结果储存在result中
 * @param 参数定义见libcurl库文档
 * @return 返回值定义见libcurl库文档
 */
size_t CfacePlusDlg::callbacktoken(void *ptr, size_t size, size_t nmemb, void *stream) {
	// 获取到的body存放在ptr中，先将其转换为string格式
	std::string s((char *)ptr, size * nmemb);
	// 开始获取json中的access token项目
	Json::Reader reader;
	Json::Value root;
	// 使用boost库解析json
	reader.parse(s, root);
	std::string* access_token_result = static_cast<std::string*>(stream);
	*access_token_result = root["access_token"].asString();
	return size * nmemb;
}

int CfacePlusDlg::get_access_token(std::string &access_token, const std::string &AK, const std::string &SK) {
	CURL *curl;
	CURLcode result_code;
	int error_code = 0;
	curl = curl_easy_init();
	if (curl) {
		std::string url = access_token_url + "&client_id=" + AK + "&client_secret=" + SK;
		curl_easy_setopt(curl, CURLOPT_URL, url.data());
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
		std::string access_token_result;
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &access_token_result);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callbacktoken);
		result_code = curl_easy_perform(curl);
		if (result_code != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(result_code));
			return 1;
		}
		access_token = access_token_result;
		curl_easy_cleanup(curl);
		error_code = 0;
	}
	else {
		fprintf(stderr, "curl_easy_init() failed.");
		error_code = 1;
	}
	return error_code;
}
static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	std::string data((const char*)ptr, (size_t)size * nmemb);

	*((std::stringstream*)stream) << data << std::endl;

	return size * nmemb;
}

void CfacePlusDlg::OnBnClickedCompare()
{
	// TODO: 在此添加控件通知处理程序代码
	std::string str_json;
	faceMatch(str_json, sToken);
	Json::Reader reader;
	Json::Value root;
	reader.parse(str_json, root);
	Json::Value result = root["result"];
	std::string sResult="相似度得分："+ std::to_string(result["score"].asDouble())+"\r\n说明：满分100分，80分以上基本可以判断是同个人";
	SetDlgItemText(IDC_RESULT, sResult.c_str());
	UpdateData();	
}
static std::string faceMatch_result;
/**
 * curl发送http请求调用的回调函数，回调函数中对返回的json格式的body进行了解析，解析结果储存在全局的静态变量当中
 * @param 参数定义见libcurl文档
 * @return 返回值定义见libcurl文档
 */
size_t CfacePlusDlg::callbackface(void *ptr, size_t size, size_t nmemb, void *stream) {
	// 获取到的body存放在ptr中，先将其转换为string格式
	faceMatch_result = std::string((char *)ptr, size * nmemb);
	return size * nmemb;
}

/**
 * 人脸对比
 * @return 调用成功返回0，发生错误返回其他错误码
 */
int CfacePlusDlg::faceMatch(std::string &json_result, const std::string &access_token) {
	if (sLeftFilePath.IsEmpty() || sRightFilePath.IsEmpty()) {
		MessageBox("请输入左右图片");
		return -1;
	}
	std::string sLeft,sRight;
	ReadPhotoFile(sLeftFilePath.GetBuffer(), sLeft);
	ReadPhotoFile(sRightFilePath.GetBuffer(), sRight);
	std::string sRequire = "[{\"image\": \"" + sLeft + "\", \"image_type\": \"BASE64\", \"face_type\": \"LIVE\", \"quality_control\": \"LOW\"},{\"image\": \"" + sRight + "\", \"image_type\": \"BASE64\", \"face_type\": \"LIVE\", \"quality_control\": \"LOW\"}]";
	std::string url = request_face_url + "?access_token=" + access_token;
	CURL *curl = NULL;
	CURLcode result_code;
	int is_success;
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.data());
		curl_easy_setopt(curl, CURLOPT_POST, 1);
		curl_slist *headers = NULL;
		headers = curl_slist_append(headers, "Content-Type:application/json");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, sRequire.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callbackface);//设置回调函数
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, FALSE);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, FALSE);
		result_code = curl_easy_perform(curl);

		if (result_code != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(result_code));
			is_success = 1;
			return is_success;
		}
		json_result = faceMatch_result;
		curl_easy_cleanup(curl);
		is_success = 0;
		}
	else {
		fprintf(stderr, "curl_easy_init() failed.");
		is_success = 1;
	}
	return is_success;
}

static std::string personVerify_result;
/**
 * curl发送http请求调用的回调函数，回调函数中对返回的json格式的body进行了解析，解析结果储存在全局的静态变量当中
 * @param 参数定义见libcurl文档
 * @return 返回值定义见libcurl文档
 */
size_t CfacePlusDlg::callbackidcard(void *ptr, size_t size, size_t nmemb, void *stream) {
	// 获取到的body存放在ptr中，先将其转换为string格式
	personVerify_result = std::string((char *)ptr, size * nmemb);
	return size * nmemb;
}

void CfacePlusDlg::OnBnClickedRequire()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sID, sName;
	GetDlgItemText(IDC_NAME,sName);
	GetDlgItemText(IDC_IDENTIFYCARD, sID);
	std::string sLeft;
	ReadPhotoFile(sLeftFilePath.GetBuffer(), sLeft);
	std::string sRequire = "{\"image\":\""+ sLeft+"\",\"image_type\":\"BASE64\",\"id_card_number\":\""+sID.GetBuffer()+"\",\"name\":\""+sName.GetBuffer()+"\",\"quality_control\":\"LOW\",\"liveness_control\":\"NONE\"}";
	std::string url = request_idcard_url + "?access_token=" + sToken;
	CURL *curl = NULL;
	CURLcode result_code;
	int is_success;
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true); ;
		curl_easy_setopt(curl, CURLOPT_CAINFO,"./cacert.pem");
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
		curl_easy_setopt(curl, CURLOPT_URL, url.data());
		curl_easy_setopt(curl, CURLOPT_POST, 1);
		curl_slist *headers = NULL;
		headers = curl_slist_append(headers, "Content-Type:application/json");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callbackidcard);//设置回调函数
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, sRequire);
		result_code = curl_easy_perform(curl);
		if (result_code != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(result_code));
			is_success = 1;
		}
		SetDlgItemText(IDC_RESULT, personVerify_result.c_str()); 
		UpdateData();
		curl_easy_cleanup(curl);
		is_success = 0;
	}
	else {
		fprintf(stderr, "curl_easy_init() failed.");
		is_success = 1;
	}
}


void CfacePlusDlg::OnBnClickedRequire2()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	std::string sRgiht;
	ReadPhotoFile(sRightFilePath.GetBuffer(), sRgiht);
	std::string sRequire = "{\"image\":\""+ sRgiht +"\",\"image_type\":\"BASE64\",\"face_field\":\"faceshape,facetype,age,beauty,emotion\"}";
	std::string url = request_property_url + "?access_token=" + sToken;
	CURL *curl = NULL;
	CURLcode result_code;
	int is_success;
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, FALSE);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, FALSE);

		curl_easy_setopt(curl, CURLOPT_URL, url.data());
		curl_easy_setopt(curl, CURLOPT_POST, 1);
		curl_slist *headers = NULL;
		headers = curl_slist_append(headers, "Content-Type:application/json");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callbackidcard);//设置回调函数
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, sRequire.c_str());
		result_code = curl_easy_perform(curl);
		if (result_code != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(result_code));
			is_success = 1;
		}
		SetDlgItemText(IDC_RESULT, personVerify_result.c_str());
		UpdateData();
		curl_easy_cleanup(curl);
		is_success = 0;
	}
	else {
		fprintf(stderr, "curl_easy_init() failed.");
		is_success = 1;
	}
}
