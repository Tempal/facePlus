
// facePlusDlg.h: 头文件
//

#pragma once
#include<string>

// CfacePlusDlg 对话框
class CfacePlusDlg : public CDialogEx
{
// 构造
public:
	CfacePlusDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FACEPLUS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
public:
	CString sLeftFilePath;
	CString sRightFilePath;

	//显示右边的图片：1为显示右边图片，0为显示左边图片
	void showPiture(bool bRight);
/**
 * 用以获取access_token的函数，使用时需要先在百度云控制台申请相应功能的应用，获得对应的API Key和Secret Key
 * @param access_token 获取得到的access token，调用函数时需传入该参数
 * @param AK 应用的API key
 * @param SK 应用的Secret key
 * @return 返回0代表获取access token成功，其他返回值代表获取失败
 */
	int get_access_token(std::string &access_token, const std::string &AK, const std::string &SK);
	const std::string access_token_url = "https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials";
	std::string sToken;
	const std::string request_face_url = "https://aip.baidubce.com/rest/2.0/face/v3/match";
	const std::string request_idcard_url = "https://aip.baidubce.com/rest/2.0/face/v3/person/verify";
	const std::string request_property_url = "https://aip.baidubce.com/rest/2.0/face/v3/detect";
	/**
	 * curl发送http请求调用的回调函数,获取百度token，回调函数中对返回的json格式的body进行了解析，解析结果储存在result中
	 * @param 参数定义见libcurl库文档
	 * @return 返回值定义见libcurl库文档
	 */
	static size_t callbacktoken(void *ptr, size_t size, size_t nmemb, void *stream);
	/**
	 * 人脸对比
	 * @return 调用成功返回0，发生错误返回其他错误码
	 */
	int faceMatch(std::string &json_result, const std::string &access_token);
	//人脸识别回写函数
	static size_t callbackface(void *ptr, size_t size, size_t nmemb, void *stream);
	/**
	 * curl发送http请求调用的回调函数，回调函数中对返回的json格式的body进行了解析，解析结果储存在全局的静态变量当中
	 * @param 参数定义见libcurl文档
	 * @return 返回值定义见libcurl文档
	 */
	static size_t callbackidcard(void *ptr, size_t size, size_t nmemb, void *stream);
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpenLeft();
	afx_msg void OnBnClickedOpentRight();
	afx_msg void OnBnClickedCompare();
	afx_msg void OnBnClickedRequire();
	afx_msg void OnBnClickedRequire2();
};
