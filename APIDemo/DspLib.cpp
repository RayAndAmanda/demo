#include "stdafx.h"
#include "microdef.h"

static UINT name168=0;
static UINT conname=0;
static UINT rdip=0;
static sockaddr_in rdaddr;
static SOCKET rdskt=NULL;

static SOCKET srcsktstream=NULL;
static int rdspd=1;
static HWND svrWnd;
CArray<IPvs,IPvs&> arrIPvs;
CArray<NET_VIDEO_DATA*,NET_VIDEO_DATA*> m_VideoData;

static char yuvfmt=0;
char GetSigVic();
int LogAndOpen(NET_VIDEO_DATA *netvid);
void CloseSig(NET_VIDEO_DATA *netvid);
void ChangeYuntai(NET_VIDEO_DATA *netvid,int yuntaipara,int StartStop);
void PlatformLogin(IPvs &ipvs);
void GetSrcInfo(IPvs &ipvs);
void PlayBackSig(NET_VIDEO_DATA *netvid,int act,SYSTEMTIME &sts,SYSTEMTIME &ste);

static char bVic=0;

int	 WINAPI SplitString(const char* pData,const char* chFlag,CStringArray* strArray)
{
	char* str=NULL;
	int len=0;
	int nNum=0;
	len=strlen(pData);
	char* pbuffer=new char[len+1];
	memcpy(pbuffer,pData,len+1);
	str=strtok(pbuffer,chFlag);
	while(str!=NULL)
	{
		strArray->Add(CString(str));
		str=strtok(NULL,chFlag);
		nNum++;
	}
	delete[] pbuffer;
	return nNum;
}

void SdSrcInfo2Cli(int MsgID ,char *msg)
{
	if(srcsktstream==NULL) return;
	char* sckSendbuffer;
	int nLen=strlen(msg)+1;
	sckSendbuffer=new char[nLen+sizeof(UINT)];
	memcpy(sckSendbuffer,&nLen,sizeof(UINT));
	memcpy(sckSendbuffer+sizeof(UINT),&MsgID,1);
	memcpy(sckSendbuffer+sizeof(UINT)+1,msg,strlen(msg));
	send(srcsktstream,sckSendbuffer,nLen+sizeof(UINT),NULL);
	delete sckSendbuffer;
}

int GetIpsIndex(char* devicename)
{
	for(int i=0;i<arrIPvs.GetCount();i++)
	{
		if(strcmp(arrIPvs[i].devicename,devicename)==0) return i;
	}
	return -1;
}
void SendBody(SOCKET sndskt, sockaddr_in addr,char *buf,int len,int pktlen)
{
	int pos=2;
	int index=1;
	int leftdata = len-2;
	while(1)
	{
		USHORT *sendbuf = (USHORT *)&buf[pos-2];
		sendbuf[0] = index;
		if(leftdata>pktlen)
		{
			sendto(sndskt,(char*)sendbuf,pktlen+2,0,(const sockaddr*)&addr,sizeof(sockaddr_in));
		}
		else
		{
			sendto(sndskt,(char*)sendbuf,leftdata+2,0,(const sockaddr*)&addr,sizeof(sockaddr_in));
			break;
		}
		index++;
		pos+=pktlen;
		leftdata-=pktlen;
	}
}
void GetRdPos(DSP_VIDEO_DATA *dspvid)
{
	if((dspvid->RdWidth==0)||(dspvid->RdHeight==0)) return;
	printf("RD %d %d %d %d\r\n",dspvid->width,dspvid->RdWidth,dspvid->height,dspvid->RdHeight);
	DOUBLE hrate = (DOUBLE)(dspvid->width)/dspvid->RdWidth;
	DOUBLE vrate = (DOUBLE)(dspvid->height)/dspvid->RdHeight;
	DOUBLE tmppos = 0;
	int i;
	for(i=0;i<dspvid->RdWidth/2;i++)			//以U数据为准
	{
		dspvid->rdtablehbuf[i] = tmppos;
		tmppos += hrate;
		if(tmppos>=dspvid->width/2)
		{
			tmppos = dspvid->width/2-1;
		}
	} 
	tmppos = 0;
	for(i=0;i<dspvid->RdHeight/2;i++)
	{
		dspvid->rdtablevbuf[i] = tmppos;
		tmppos += vrate;
		if(tmppos>=(dspvid->height/2))
		{
			tmppos=dspvid->height/2-1;
		}
	}
}

#define UDPPKT 1022
#define BIGPKT 65000
void senddata(NET_VIDEO_DATA *userpara,ULONG *pBufYuv,int width,int height,ULONG *ulLineSize)
{
	DSP_VIDEO_DATA *netvid = (DSP_VIDEO_DATA *)(userpara->otherpara);
//	printf("netvid 0x%x %d %s %d\r\n",netvid->hShareMem, netvid->WinId,userpara->strIP,netvid->clinum);
	//int pinterval = iFrame - netvid->lastpts;
	//netvid->lastpts = iFrame;
	//int curtick = GetTickCount();
	//int tinterval = curtick-netvid->lasttick;
	//if(pinterval>tinterval)
	//{
	//	if(netvid->framenum%100)
	//		Sleep(pinterval-tinterval-1);
	//	else
	//		Sleep(pinterval-tinterval);
	//}
	//netvid->lasttick = GetTickCount();
	if((netvid->width!=width)||(netvid->height!=height))
	{
		netvid->width = width;
		netvid->height = height;
		if(netvid->clinum!=1)
		{
			int winw = netvid->rcWinReal.right-netvid->rcWinReal.left;
			int winh = netvid->rcWinReal.bottom-netvid->rcWinReal.top;
			for(int i=0;i<netvid->clinum;i++)
			{
				netvid->m_arrClientInfo[i].top = netvid->m_arrClientInfo[i].left = netvid->m_arrClientInfo[i].cwidth = netvid->m_arrClientInfo[i].cheight=0;
				int left = (netvid->m_arrClientInfo[i].rcWinReal.left-netvid->rcWinReal.left)*width/winw;
				if(left<0) break;
				int right = (netvid->m_arrClientInfo[i].rcWinReal.right-netvid->rcWinReal.left)*width/winw;
				if(right<0) break;
				int top = (netvid->m_arrClientInfo[i].rcWinReal.top-netvid->rcWinReal.top)*height/winh;
				if(top<0) break;
				netvid->m_arrClientInfo[i].top = top;
				int	bottom = (netvid->m_arrClientInfo[i].rcWinReal.bottom-netvid->rcWinReal.top)*height/winh;
				if(bottom<0) break;
				left = left/4;
				netvid->m_arrClientInfo[i].left = left = left*4;
				right = right/4;
				right = right*4;
				top = top/4;
				netvid->m_arrClientInfo[i].top = top = top*4;
				bottom = bottom/4;
				bottom = bottom*4;
				int cwidth = right-left;
				if(left+cwidth>width) break;
				netvid->m_arrClientInfo[i].cwidth = cwidth;
				int cheight = bottom-top;
				if(top+cheight>height) break;
				netvid->m_arrClientInfo[i].cheight = cheight;
			}
		}
		 GetRdPos(netvid);
	}
	char *pYBuf = (char*)(pBufYuv[0]);
	char *pUBuf = (char*)(pBufYuv[1]);
	char *pVBuf = (char*)(pBufYuv[2]);
////////////////////////////////////////////////////////////回显////////////////////////////////////////////////////////////
	if((netvid->RdWidth !=0)&&(netvid->framenum%rdspd==0))
	{
//		printf("begin rd\r\n");
		int ylen,ulen,vlen;
		if(ulLineSize==NULL)
		{
			ylen = width;
			ulen = vlen = width/2;
		}
		else
		{
			ylen = ulLineSize[0];
			ulen = ulLineSize[1];
			vlen = ulLineSize[2];
		}
		for(int a=0;a<netvid->RdHeight/2;a++)
		{
//			printf("rd v %d\r\n",netvid->rdtablevbuf[a]);
			netvid->rdbuf[2] = a>>8;
			netvid->rdbuf[3] = a&0xff;
			int scnt = 4;
			unsigned char *DecodeBufVtmp = (BYTE*)pYBuf + netvid->rdtablevbuf[a]*ylen*2;
			unsigned char *DecodeBuftmp;
			//第一行 Y
			for(int b=0;b<netvid->RdWidth/2;b++)
			{
				DecodeBuftmp=DecodeBufVtmp +netvid->rdtablehbuf[b]*2;
				netvid->rdbuf[scnt++] = *DecodeBuftmp;				//每次取两个像素，2个字节
				DecodeBuftmp++;
				netvid->rdbuf[scnt++] = *DecodeBuftmp;				//每次取两个像素，2个字节
			}
			//第二行 Y
			DecodeBufVtmp = DecodeBufVtmp+ylen;
			for(int b=0;b<netvid->RdWidth/2;b++)
			{
				DecodeBuftmp=DecodeBufVtmp +netvid->rdtablehbuf[b]*2;
				netvid->rdbuf[scnt++] = *DecodeBuftmp;				//每次取两个像素，2个字节
				DecodeBuftmp++;
				netvid->rdbuf[scnt++] = *DecodeBuftmp;				//每次取两个像素，2个字节
			}
			if(yuvfmt)
				DecodeBufVtmp = (BYTE*)pUBuf + netvid->rdtablevbuf[a]*ulen;
			else
				DecodeBufVtmp = (BYTE*)pVBuf + netvid->rdtablevbuf[a]*vlen;
			for(int b=0;b<netvid->RdWidth/2;b++)
			{
				DecodeBuftmp=DecodeBufVtmp +netvid->rdtablehbuf[b];
				netvid->rdbuf[scnt++] = *DecodeBuftmp;				
			}
			if(yuvfmt)
				DecodeBufVtmp = (BYTE*)pVBuf + netvid->rdtablevbuf[a]*vlen;
			else
				DecodeBufVtmp = (BYTE*)pUBuf + netvid->rdtablevbuf[a]*ulen;
			for(int b=0;b<netvid->RdWidth/2;b++)
			{
				DecodeBuftmp=DecodeBufVtmp +netvid->rdtablehbuf[b];
				netvid->rdbuf[scnt++] = *DecodeBuftmp;				
			}
			sendto(rdskt,(const char *)(netvid->rdbuf),scnt,0,(SOCKADDR *)&rdaddr,sizeof(sockaddr_in));
		}
	}
	netvid->framenum++;
////////////////////////////////////////////////////////////回显////////////////////////////////////////////////////////////

	int leftdata;
	sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port=htons(netvid->WinId);

	USHORT tmpbuf[20]={0};
	tmpbuf[5] = bVic;
//	memcpy(&tmpbuf[6],&iFrame,8);
	if(netvid->clinum==1)
	{
		tmpbuf[1] = width;
		tmpbuf[2] = height;
		tmpbuf[3] = BIGPKT;
		addr.sin_addr.s_addr=netvid->m_arrClientInfo[0].ulIP;
		leftdata = width*height*3/2;
		if(ulLineSize==NULL)
		{
//			printf("b netvid->clinum==1 0x%x 0x%x 0x%d\r\n",netvid->pcMap,pYBuf,leftdata);
//			tmpbuf[4] = *(USHORT*)pYBuf;
//			int tm = GetTickCount();
//			SendBody(netvid->sndskt,addr,pYBuf,leftdata,BIGPKT);
			memcpy(netvid->pcMap,pYBuf,leftdata);
//			printf("e netvid->clinum==1 0x%x\r\n",netvid->pcMap);
//			TRACE("%d\r\n",GetTickCount()-tm);
		}
		else
		{
			char *sptr=pYBuf;
//			BYTE *tptr = netvid->sepbuf;
			BYTE *tptr = netvid->pcMap;
			for(int i=0;i<height;i++)
			{
				memcpy(tptr,sptr,width);
				sptr += ulLineSize[0];
				tptr += width;
			}
			sptr=pUBuf;
			for(int i=0;i<height/2;i++)
			{
				memcpy(tptr,sptr,width/2);
				sptr += ulLineSize[1];
				tptr += width/2;
			}
			sptr=pVBuf;
			for(int i=0;i<height/2;i++)
			{
				memcpy(tptr,sptr,width/2);
				sptr += ulLineSize[2];
				tptr += width/2;
			}
//			tmpbuf[4] = *(USHORT*)(netvid->sepbuf);
//			SendBody(netvid->sndskt,addr,(char*)(netvid->sepbuf),leftdata,BIGPKT);
		}
		sendto(netvid->sndskt,(char*)tmpbuf,12,0,(const sockaddr*)&addr,sizeof(sockaddr_in));
	}
	else
	{
		tmpbuf[3] = UDPPKT;
		int ylen,ulen,vlen;
		if(ulLineSize==NULL)
		{
			ylen = width;
			ulen = vlen = width/2;
		}
		else
		{
			ylen = ulLineSize[0];
			ulen = ulLineSize[1];
			vlen = ulLineSize[2];
		}
		for(int i=netvid->clinum-1;i>=0;i--)
		{
			int left = netvid->m_arrClientInfo[i].left;
			int top = netvid->m_arrClientInfo[i].top;
			int cwidth = netvid->m_arrClientInfo[i].cwidth;
			int cheight = netvid->m_arrClientInfo[i].cheight;
			char *sbuf = pYBuf+top*ylen+left;
			BYTE *tbuf;
			if(i==0)
				tbuf = netvid->pcMap;
			else
				tbuf = netvid->sepbuf;
			for(int j=0;j<cheight;j++)
			{
				memcpy(tbuf,sbuf,cwidth);
				tbuf+=cwidth;
				sbuf+=ylen;
			}
			sbuf = pUBuf+top*ulen/2+left/2;
			for(int j=0;j<cheight/2;j++)
			{
				memcpy(tbuf,sbuf,cwidth/2);
				tbuf+=cwidth/2;
				sbuf+=ulen;
			}
			sbuf = pVBuf+top*vlen/2+left/2;
			for(int j=0;j<cheight/2;j++)
			{
				memcpy(tbuf,sbuf,cwidth/2);
				tbuf+=cwidth/2;
				sbuf+=vlen;
			}
			netvid->FirstData[i] = *(USHORT*)(netvid->sepbuf);
			addr.sin_addr.s_addr=netvid->m_arrClientInfo[i].ulIP;
			leftdata = cwidth*cheight*3/2;
//			if(leftdata<0)	break;
			if(i!=0)
				SendBody(netvid->sndskt,addr,(char*)(netvid->sepbuf),leftdata,UDPPKT);
		}
		for(int i=netvid->clinum-1;i>=0;i--)
		{
			tmpbuf[1] = netvid->m_arrClientInfo[i].cwidth;
			tmpbuf[2] = netvid->m_arrClientInfo[i].cheight;
			tmpbuf[4] = netvid->FirstData[i];
			addr.sin_addr.s_addr=netvid->m_arrClientInfo[i].ulIP;
			sendto(netvid->sndskt,(char*)tmpbuf,12,0,(const sockaddr*)&addr,sizeof(sockaddr_in));
		}
	}
//	delete sepbuf;

//	printf("tm %d\r\n",tm1-tm);
}
void CopyDataCheck(HWND hWnd,COPYDATASTRUCT *tmpstr)
{
	char *para = (char*)(tmpstr->lpData);
	int offset=0;
	switch(tmpstr->dwData)
	{
		case EXE_RESET_IPS:
			arrIPvs.RemoveAll();
			break;
		case EXE_SVR_INI:
		{
			int ptr=0;
			memcpy(&svrWnd,para,4);
			ptr+=4;
			memcpy(&conname,para+ptr,4);
			ptr+=4;
			memcpy(&name168,para+ptr,4);
			ptr+=4;
			memcpy(&rdip,para+ptr,4);
			ptr+=4;
			int rdport;
			memcpy(&rdport,para+ptr,4);
			ptr+=4;
			if(rdskt==NULL)
			{
				rdskt = socket(AF_INET,SOCK_DGRAM,0);
				sockaddr_in addr;
				addr.sin_family=AF_INET;
				addr.sin_addr.s_addr=name168;
				addr.sin_port=htons(rdport);
				bind(rdskt,(sockaddr*)&addr,sizeof(sockaddr_in));
				rdaddr.sin_family=AF_INET;
				rdaddr.sin_addr.s_addr=rdip;
				rdaddr.sin_port=htons(60001);
			}
		}
		break;
		case EXE_SET_IPS:
		{
			IPvs ipvs;
			memcpy(&ipvs,para+offset,sizeof(IPvs));
			yuvfmt = ipvs.fmt;
			PlatformLogin(ipvs);
			bVic = GetSigVic();
			arrIPvs.Add(ipvs);
		}
		break;
		case EXE_SET_SKT:
		{
			DSP_VIDEO_DATA *dspvid = new DSP_VIDEO_DATA;
			memset(dspvid,0,sizeof(DSP_VIDEO_DATA));
			NET_VIDEO_DATA *netvid = new NET_VIDEO_DATA;
			memset(netvid,0,sizeof(NET_VIDEO_DATA));
			netvid->otherpara = dspvid;
			dspvid->sepbuf = new BYTE[2048*1536*3/2];
			memcpy(netvid->strIP,para+offset,128);
			offset+=128;
			memcpy(&(netvid->iChanIndex),para+offset,4);
			offset+=4;
			memcpy(&(netvid->netport),para+offset,4);
			offset+=4;
			memcpy(netvid->devicename,para+offset,32);
			offset+=32;
			memcpy(&(dspvid->WinId),para+offset,4);
			offset+=4;	
			memcpy(&(dspvid->rcWinReal),para+offset,sizeof(RECT));
			offset+=sizeof(RECT);	
			memcpy(&(dspvid->setrcOffset),para+offset,sizeof(RECT));
			offset+=sizeof(RECT);	
			memcpy(&(dspvid->clinum),para+offset,1);					//clinum
			offset+=1;	
			for(int j=0;j<dspvid->clinum;j++)
			{
				memcpy(&(dspvid->m_arrClientInfo[j].ulIP),para+offset,4);  //
				offset+=4;	
				memcpy(&(dspvid->m_arrClientInfo[j].rcWinReal),para+offset,sizeof(RECT)); //m_arrClientInfo[j].rcWinReal
				offset+=sizeof(RECT);	
			}
			memcpy(&(dspvid->RealBack),para+offset,1);					//clinum
			offset+=1;	
			sockaddr_in addr;
			addr.sin_family=AF_INET;
			addr.sin_addr.s_addr=dspvid->m_arrClientInfo[0].ulIP;
			dspvid->sndskt = socket(AF_INET,SOCK_DGRAM,0);
			for(int i=9999;i>8192;i--)
			{
				addr.sin_port=htons(i);
				int ret = bind(dspvid->sndskt,(sockaddr*)&addr,sizeof(sockaddr_in));
				if(ret != SOCKET_ERROR) break;
			}
			int msec=10000000;
			setsockopt(dspvid->sndskt, SOL_SOCKET, SO_SNDBUF, (char *)&msec, sizeof(msec));
			dspvid->rdbuf[0] = (BYTE)(dspvid->WinId&0xff);
			dspvid->rdbuf[1] = (BYTE)(dspvid->WinId>>8);

//			CString sharestr;
//			sharestr.Format(L"%d_%s",dspvid->WinId,netvid->strIP);
			char msharestr[255]={0};
//			WideCharToMultiByte(CP_ACP,0,sharestr.GetBuffer(),sharestr.GetLength(),msharestr,255,NULL,NULL);
			sprintf(msharestr,"%d_%s",dspvid->WinId,netvid->strIP);
			dspvid->hShareMem = OpenFileMappingA(FILE_MAP_ALL_ACCESS,FALSE,msharestr);
			printf("EXE_SET_SKT 0x%s 0x%x\r\n",msharestr,dspvid->hShareMem);
			dspvid->pcMap = (LPBYTE)MapViewOfFile(dspvid->hShareMem, FILE_MAP_ALL_ACCESS,0, 0, 0);

			m_VideoData.Add(netvid);
			if(dspvid->RealBack==0)
			{
				int winw = dspvid->rcWinReal.right-dspvid->rcWinReal.left;
				int winh = dspvid->rcWinReal.bottom-dspvid->rcWinReal.top;
				if((winw>962)&&(winh>542)) netvid->bVic = 0;
				else netvid->bVic = 1;
				int result = LogAndOpen(netvid);
			}
		}
		break;
		case EXE_CLS_SIG:
		{
			UINT SigId;
			memcpy(&SigId,para+offset,4);
			offset+=4;
			for(int i=0;i<m_VideoData.GetCount();i++)
			{
				DSP_VIDEO_DATA *dspvid = (DSP_VIDEO_DATA *)(m_VideoData[i]->otherpara);
				if(dspvid->WinId==SigId)
				{
					if(dspvid->RealBack==0)
						CloseSig(m_VideoData[i]);
					closesocket(dspvid->sndskt);
					delete dspvid->sepbuf;
					UnmapViewOfFile(dspvid->pcMap);
					CloseHandle(dspvid->hShareMem);
					delete dspvid;
					delete m_VideoData[i];
					m_VideoData.RemoveAt(i);
					break;
				}
			}
		}
		break;
		case EXE_REOPEN_SIG:
		{
			UINT SigId;
			memcpy(&SigId,para+offset,4);
			offset+=4;
			for(int i=0;i<m_VideoData.GetCount();i++)
			{
				DSP_VIDEO_DATA *dspvid = (DSP_VIDEO_DATA *)(m_VideoData[i]->otherpara);
				if(dspvid->WinId==SigId)
				{
					CloseSig(m_VideoData[i]);
					Sleep(1000);
					LogAndOpen(m_VideoData[i]);
					break;
				}
			}
		}
		break;
		case EXE_GET_PROCESSOR_SIGNAL:
		{
			char devicename[33]={0};
			memcpy(devicename,para+offset,32);
			offset+=32;
			for(int i=0;i<arrIPvs.GetCount();i++)
			{
				if(strcmp(arrIPvs[i].devicename,devicename)==0)
				{
					UINT cliport;
					memcpy(&cliport,para+offset,4);
					offset+=4;
					UINT srcip;
					memcpy(&srcip,para+offset,4);
					offset+=4;

					srcsktstream = socket(AF_INET,SOCK_STREAM,0);
					SOCKADDR_IN addrSrv;
					addrSrv.sin_family = AF_INET;
					addrSrv.sin_port = htons(cliport); 
					addrSrv.sin_addr.S_un.S_addr = srcip;
					connect(srcsktstream,(SOCKADDR *)&addrSrv,sizeof(SOCKADDR));
					GetSrcInfo(arrIPvs[i]);
					break;
					//do not close socket, socket will be disconnected by host. close immediatly maybe cause data lose
				}
			}
		}
		break;
		case EXE_SET_RD:
		{
			UINT SigId;
			memcpy(&SigId,para+offset,4);
			offset+=4;
			for(int i=0;i<m_VideoData.GetCount();i++)
			{
				DSP_VIDEO_DATA *dspvid = (DSP_VIDEO_DATA *)(m_VideoData[i]->otherpara);
				if(dspvid->WinId==SigId)
				{
					memcpy(&(dspvid->RdWidth),para+offset,4);
					offset+=4;
					memcpy(&(dspvid->RdHeight),para+offset,4);
					offset+=4;
					memcpy(&rdip,para+offset,4);
					offset+=4;
					rdaddr.sin_addr.s_addr=rdip;
					GetRdPos(dspvid);
					break;
				}
			}
		}
		break;
		case EXE_SET_RDSPD:
		break;
		case EXE_PRE_SIG:
		{
			//DSP_VIDEO_DATA *dspvid = new DSP_VIDEO_DATA;
			//memset(dspvid,0,sizeof(DSP_VIDEO_DATA));
			//NET_VIDEO_DATA *netvid = new NET_VIDEO_DATA;
			//memset(netvid,0,sizeof(NET_VIDEO_DATA));
			//netvid->otherpara = dspvid;
			//memcpy(netvid->strIP,para+offset,128);
			//offset+=128;
			//memcpy(&(netvid->iChanIndex),para+offset,4);
			//offset+=4;
			//memcpy(&(netvid->netport),para+offset,4);
			//offset+=4;
			//memcpy(netvid->devicename,para+offset,32);
			//offset+=32;
			//memcpy(&(dspvid->WinId),para+offset,4);
			//offset+=4;	
			//memcpy(&(dspvid->rcWinReal),para+offset,sizeof(RECT));
			//offset+=sizeof(RECT);	
			//memcpy(&(dspvid->setrcOffset),para+offset,sizeof(RECT));
			//offset+=sizeof(RECT);	
			//memcpy(&(dspvid->clinum),para+offset,1);					//clinum
			//offset+=1;	
			//for(int j=0;j<dspvid->clinum;j++)
			//{
			//	memcpy(&(dspvid->m_arrClientInfo[j].ulIP),para+offset,4);  //
			//	offset+=4;	
			//	memcpy(&(dspvid->m_arrClientInfo[j].rcWinReal),para+offset,sizeof(RECT)); //m_arrClientInfo[j].rcWinReal
			//	offset+=sizeof(RECT);	
			//}
			//sockaddr_in addr;
			//addr.sin_family=AF_INET;
			//addr.sin_addr.s_addr=dspvid->m_arrClientInfo[0].ulIP;
			//dspvid->sndskt = socket(AF_INET,SOCK_DGRAM,0);
			//for(int i=9999;i>8192;i--)
			//{
			//	addr.sin_port=htons(i);
			//	int ret = bind(dspvid->sndskt,(sockaddr*)&addr,sizeof(sockaddr_in));
			//	if(ret != SOCKET_ERROR) break;
			//}
			//int msec=10000000;
			//setsockopt(dspvid->sndskt, SOL_SOCKET, SO_SNDBUF, (char *)&msec, sizeof(msec));
			//dspvid->rdbuf[0] = (BYTE)(dspvid->WinId&0xff);
			//dspvid->rdbuf[1] = (BYTE)(dspvid->WinId>>8);
			//m_VideoData.Add(netvid);
			//int result = LogAndOpen(netvid);
		}
		break;
		case EXE_YUNTAI_SIG:
		{
			UINT SigId;
			memcpy(&SigId,para+offset,4);
			offset+=4;
			for(int i=0;i<m_VideoData.GetCount();i++)
			{
				DSP_VIDEO_DATA *dspvid = (DSP_VIDEO_DATA *)(m_VideoData[i]->otherpara);
				if(dspvid->WinId==SigId)
				{
					int yuntaipara;
					memcpy(&yuntaipara,para+offset,4);
					offset+=4;
					int StartStop;
					memcpy(&StartStop,para+offset,4);
					offset+=4;
					ChangeYuntai(m_VideoData[i],yuntaipara,StartStop);
					break;
				}
			}
		}
		break;
		case EXE_PLAYBACK_SIG:
		{
			UINT SigId;
			memcpy(&SigId,para+offset,4);
			offset+=4;
			for(int i=0;i<m_VideoData.GetCount();i++)
			{
				DSP_VIDEO_DATA *dspvid = (DSP_VIDEO_DATA *)(m_VideoData[i]->otherpara);
				if(dspvid->WinId==SigId)
				{
					int act;
					memcpy(&act,para+offset,4);
					offset+=4;
					int paralen;
					memcpy(&paralen,para+offset,4);
					offset+=4;
					SYSTEMTIME sts,ste;
					if(paralen)
					{
						char datatime[1000]={0};
						memcpy(datatime,para+offset,paralen);
						CStringArray strArray;
						SplitString(datatime,"|",&strArray);
						sts.wYear = atoi(strArray[0]);
						sts.wMonth = atoi(strArray[1]);
						sts.wDay = atoi(strArray[2]);
						sts.wHour = atoi(strArray[3]);
						sts.wMinute = atoi(strArray[4]);
						sts.wSecond = atoi(strArray[5]);

						ste.wYear = atoi(strArray[6]);
						ste.wMonth = atoi(strArray[7]);
						ste.wDay = atoi(strArray[8]);
						ste.wHour = atoi(strArray[9]);
						ste.wMinute = atoi(strArray[10]);
						ste.wSecond = atoi(strArray[11]);
					}

					PlayBackSig(m_VideoData[i],act,sts,ste);
				}
			}
		}
		break;
	}
}