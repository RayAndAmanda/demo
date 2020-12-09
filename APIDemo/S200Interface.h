#pragma once 

void GetSrcS200();
BOOL StartRealPlayS200(const char* path, void* netid);
typedef struct
{
	std::string treeCode;
	std::string treeName;

}TreeNode;
typedef struct
{
	std::string treeCode; //树编号
	std::string cameraIndexCode; //监控点编号
	std::string name;  //监控名称
	int cameraType; //监控点类型（0-枪机,1-半球,2-快球,3-带云台枪机,
	std::string cameraTypeName;//监控点类型说明
	int status; //在线状态（0-不在线，1-在线
	std::string url; 
	std::string deviceIndexCode;//所属设备编号（通用唯一识别码UUID）
	std::string unitIndexCode;//所属组织编号
}CameraInfo;
typedef struct
{
	std::string indexCode; //组织编号
	std::string name; //组织名称
	std::string treeCode; //树编码
	std::string parentIndexCode;//父组织编号
}DeviceGroup;

extern std::vector <TreeNode> g_TreeNodeList;
extern std::vector<CameraInfo> g_CameraInfoList;
extern std::vector<DeviceGroup> g_DeviceGroupList;
extern bool g_GerScrOK;
extern std::string AnsiToUtf8(LPCSTR Ansi);
extern std::string FindPreviewUrlByCameraindex(std::string cameraIndexCode);