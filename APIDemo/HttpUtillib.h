#pragma once
#include "string"
#include "map"
#include "list"
using std::string;
using std::map;
using std::list;

// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 HTTPUTILLIB_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// HTTPUTILLIB_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。


#ifdef HTTPUTILLIB_EXPORTS
#define HTTPUTILLIB_API extern __declspec(dllexport)
#else
#define HTTPUTILLIB_API __declspec(dllimport)
#endif



#define STDCALL __stdcall

	/**
	* HTTP GET
	*
	* @param url                  http://host+path+query
	* @param headers              Http头
	* @param appKey               APP KEY
	* @param appSecret            APP密钥
	* @param timeout              超时时间（秒）
	* @param signHeaderPrefixList 自定义参与签名Header前缀
	* @return 调用结果
	*/
	
	HTTPUTILLIB_API string STDCALL HttpGet(string url, map<string, string> headers, string appKey, string appSecret, int timeout, list<string> signHeaderPrefixList);

	/**
	* Http POST 字符串
	*
	* @param url                  http://host+path+query
	* @param headers              Http头
	* @param body                 字符串请求体
	* @param appKey               APP KEY
	* @param appSecret            APP密钥
	* @param timeout              超时时间（秒）
	* @param signHeaderPrefixList 自定义参与签名Header前缀
	* @return 调用结果
	*/
	HTTPUTILLIB_API string STDCALL HttpPost(string url, map<string, string> headers, string body, string appKey, string appSecret, int timeout, list<string> signHeaderPrefixList);

	/**
	* Http POST 字节数组
	*
	* @param url                  http://host+path+query
	* @param headers              Http头
	* @param body                 字节数组
	* @param bodySize             字节数组的大小
	* @param appKey               APP KEY
	* @param appSecret            APP密钥
	* @param timeout              超时时间（秒）
	* @param signHeaderPrefixList 自定义参与签名Header前缀
	* @return 调用结果
	*/
	HTTPUTILLIB_API string STDCALL HttpPost(string url, map<string, string> headers, const char *body, unsigned int bodySize, string appKey, string appSecret, int timeout, list<string> signHeaderPrefixList);

	/**
	* Http POST 表单
	*
	* @param url                  http://host+path+query
	* @param headers              Http头
	* @param bodys                表单参数
	* @param appKey               APP KEY
	* @param appSecret            APP密钥
	* @param timeout              超时时间（秒）
	* @param signHeaderPrefixList 自定义参与签名Header前缀
	* @return 调用结果
	*/
	HTTPUTILLIB_API string STDCALL HttpPost(string url, map<string, string> headers, map<string, string> bodys, string appKey, string appSecret, int timeout, list<string> signHeaderPrefixList);

	/**
	* Http PUT
	*
	* @param url                  http://host+path+query
	* @param headers              Http头
	* @param body                 待上传数据
	* @param bodySize             数据大小
	* @param appKey               APP KEY
	* @param appSecret            APP密钥
	* @param timeout              超时时间（秒）
	* @param signHeaderPrefixList 自定义参与签名Header前缀
	* @return 调用结果
	*/
	HTTPUTILLIB_API string STDCALL HttpPut(string url, map<string, string> headers, const char *body, unsigned int bodySize, string appKey, string appSecret, int timeout, list<string> signHeaderPrefixList);

	/**
	* Http DELETE
	*
	* @param url                  http://host+path+query
	* @param headers              Http头
	* @param appKey               APP KEY
	* @param appSecret            APP密钥
	* @param timeout              超时时间（秒）
	* @param signHeaderPrefixList 自定义参与签名Header前缀
	* @return 调用结果
	*/
	HTTPUTILLIB_API string STDCALL HttpDelete(string url, map<string, string> headers, string appKey, string appSecret, int timeout, list<string> signHeaderPrefixList);

	/**
	* 初始化基础Header
	*
	* @param headers              Http头
	* @param appKey               APP KEY
	* @param appSecret            APP密钥
	* @param method               Http方法
	* @param requestAddress       http://host+path+query
	* @param formParam            表单参数
	* @param signHeaderPrefixList 自定义参与签名Header前缀
	* @return 基础Header
	*/
	HTTPUTILLIB_API map<string, string> STDCALL initialBasicHeader(map<string, string> &headers, string appKey, string appSecret, string method, string requestAddress, map<string, string> formParam, list<string> signHeaderPrefixList);



