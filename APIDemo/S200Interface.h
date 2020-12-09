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
	std::string treeCode; //�����
	std::string cameraIndexCode; //��ص���
	std::string name;  //�������
	int cameraType; //��ص����ͣ�0-ǹ��,1-����,2-����,3-����̨ǹ��,
	std::string cameraTypeName;//��ص�����˵��
	int status; //����״̬��0-�����ߣ�1-����
	std::string url; 
	std::string deviceIndexCode;//�����豸��ţ�ͨ��Ψһʶ����UUID��
	std::string unitIndexCode;//������֯���
}CameraInfo;
typedef struct
{
	std::string indexCode; //��֯���
	std::string name; //��֯����
	std::string treeCode; //������
	std::string parentIndexCode;//����֯���
}DeviceGroup;

extern std::vector <TreeNode> g_TreeNodeList;
extern std::vector<CameraInfo> g_CameraInfoList;
extern std::vector<DeviceGroup> g_DeviceGroupList;
extern bool g_GerScrOK;
extern std::string AnsiToUtf8(LPCSTR Ansi);
extern std::string FindPreviewUrlByCameraindex(std::string cameraIndexCode);