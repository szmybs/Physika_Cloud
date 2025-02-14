#pragma once
#include "global.h"
#include "Utility/Vector.h"
#include "Utility/Color.h"
#include "Utility/perlin.h"
#include <vector>
#include <algorithm> 
#include <string>
#include <fstream>
#include <cmath>

enum SatDataType { IR1, IR2, IR3, IR4, VIS, CLC, CTT };
enum DrawType { LongLat, SunZA, Seg, TopSurface, BottomSurface, Thick, EffRadius, Extinction };
struct Date
{
	int year;
	int month;
	int day;

	int hour;
	int minute;
	int second;
	Date()
	{
		year = 2013;
		month = 7;
		day = 10;
		setHMS();
	}
	Date(int year, int month, int day)
	{
		this->year = year;
		this->month = month;
		this->day = day;
		setHMS();
	}
	Date(int year, int month, int day, int hour, int minute, int second)
	{
		this->year = year;
		this->month = month;
		this->day = day;
		setHMS(hour, minute, second);
		usingHMS = true;
	}
	bool IsUsingHMS() const
	{
		return usingHMS;
	}
	private:
		bool usingHMS = false;
		void setHMS(int hour = 0, int minute = 0, int second = 0)
		{
			this->hour = hour;
			this->minute = minute;
			this->second = second;
		}
};

//9210??????????
//9210??????????FY2C?��?????????????FY2C?��?????????????????????????????
//????????????????1????????��?512??*518?��???512*518???????????6???????(?????????)??????512??????????????????????��??????
//??????????????
//????????????????????????11??1212Char??12SAT96?????213??142Int16??????????????315??162Int16?????????????417??182Int16?????????????519??202Int16???????????621??222Int16???????723??242Int16??????��????825??262Int16?????????��????927??282Int16??????1029??302Int16??????1131??388Char??8?????????1239??402Int16??????????????
//???????????????
//????????????????????????141??488A??8??????249??502I??2?????351??522I??2??????453??542I??2??????555??562I??2??????657??582I??2??????759??602I??2?????861??622I??2?????963??642I??2??????1065??662I??2?????1167??682I??2??????????????1269??702I??2?????????????1371??722I??2??????1473??742I??2??????��????��??1575??762I??2??????��????��??1677??782I??2??????��????????1779??802I??2??????��????????1881??822I??2??????��??1983??842I??2?????????2085??862I??2?????��??1???????????2187??882I??2?????��??22289??902I??2?????????2391??922I??2??????????2493??942I??2?????????????2595??962I??2????????????2697??982I??2?????????�A??2799??1002I??2????????�A??28101??1022I??2??��????�A??29103??1042I??2????
///*?????????*/
typedef struct AwxFileFirstHeader
{
	char strFileName[12]; //????? ??1??
	short int iByteOrder; //???????????? ??2??
	short int iFirstHeaderLength; //????????????? ??3??
	short int iSecondHeaderLength; //????????????? ??4??
	short int iFillSectionLength; //???????????
	short int iRecoderLength; //??????? ??5??
	short int iRecordsOfHeader; //??????��???? ??5A??
	short int iRecordsOfData; //?????????��????
	short int iTypeOfProduct; //?????? ??6??
	short int iTypeOfCompress; //?????? ??7??
	char strVersion[8]; //????????? ??8??
	short int iFlagOfQuality; //?????????????? ??9??
}AwxFileFirstHeader;

/*????????????????????????????????*/
typedef struct AwxFileGeoSatelliteSecondHeader
{
	char strSatelliteName[8]; //??????
	short int iYear; //?????
	short int iMonth; //??????
	short int iDay; //??????
	short int iHour; //??????
	short int iMinute; //??????
	short int iChannel; //?????
	short int iFlagOfProjection; //?????
	short int iWidthOfImage; //??????
	short int iHeightOfImage; //?????
	short int iScanLineNumberOfImageTopLeft; //??????????????
	short int iPixelNumberOfImageTopLeft; //?????????????
	short int iSampleRatio; //??????
	short int iLatitudeOfNorth; //??????��????��??
	short int iLatitudeOfSouth; //??????��????��??
	short int iLongitudeOfWest; //??????��????????
	short int iLongitudeOfEast; //??????��????????
	short int iCenterLatitudeOfProjection;//??????��?? ???100
	short int iCenterLongitudeOfProjection;//????????? ???100
	short int iStandardLatitude1;//?????��??1???????????
	short int iStandardLatitude2; //?????��??2
	short int iHorizontalResolution;//????????? ?????100
	short int iVerticalResolution; //?????????? ?????100
	short int iOverlapFlagGeoGrid; //?????????????
	short int iOverlapValueGeoGrid;//????????????
	short int iDataLengthOfColorTable; //?????????�A??
	short int iDataLengthOfCalibration; //????????�A??
	short int iDataLengthOfGeolocation; //??��????�A??
	short int iReserved; //????
}AwxFileGeoSatelliteSecondHeader;


class SatDataCloud
{
public:
	float* ir1Data;
	float* ir2Data;
	float* ir3Data;
	float* ir4Data;
	float* visData;
	float* clcData;
	float* cttData;

	float* irReflectanceData;//ir4->reflectance

	int* pixelTypeList; //0-ground, 1-cloud, 2-thin cloud(thin cirrus)
	float* cthList; //cloud top height of a day

	float* thinCloudTList;
	float* vis_thick_data;
	float* ir4_thick_data;
	float* efficientRadius_data;
	float* geo_thick_data;
	float* extinctionPlane;

	Vector2 dataRange[7];

	int WIDTH;
	int HEIGHT;

	Date SatDate;

	string satelliteName;
public:
	SatDataCloud(void);
	void Init();
	void Run(const vector<string>& typName, const string& savePath, const string& saveName, int height, int width);
	void Run(Date date, string satStr, string savePath, string saveName, int height, int width);

	// my add to generate XML file used in vtk.js
	void GenerateVolumeFile(const string& savePath, int startFrame, int endFrame);
	// bool WriteVTI(int length, int width, int height, const std::vector<float>& data, std::string& path);
	// void ExportCloudXMLVolume(const std::vector<float> data, const std::string& savePath);

	//position information
	float center_theta;
	float center_phi;

	//longitude latitude, altitude
	float* longitudeLatitudeTable;
	float* altitudeTable;
	void CreateLongLatTable(const char* longLatFile);
	void CreateLongLatTable();
	void DrawLongLatTable();
	void CreateAltitudeTable();

	//the azimuth angle and the zenith angle of the satellite
	float* satZenith_mat_E;
	float* satAzimuth_mat_E;

	float* satZenith_mat_F;
	float* satAzimuth_mat_F;

	void CreateSatZenithAzimuthIfo();

	//the azimuth angle and the zenith angle of the sun
	float* sunZenithAzimuth_mat;
	int CreateSunZenithAzimuthIfo(Date date);
	void CreateSunZenithAzimuthIfoFromFile(const char* filename);
	void DrawSunZenithAzimuth(int curFrame);

	//read satellite image data
	void IntepImgData(float* img_data, int img_width, int img_height, float* pData, int width, int height);
	void IntepImgData(int nframe, SatDataType channel, float* img_data, int img_width, int img_height, float* pData, int width, int height);

	bool ReadSingleSatData(char* filename, float* pData, SatDataType channel, int nframe);
	void ReadSatData(CString satStr, Date date, SatDataType channel);
	void ReadSatData(const string& satStr, SatDataType channel);
	void DrawSatData(SatDataType channel, int nframe);

	//ground temperature
	float* ground_temperature_mat; //ir1
	float* ground_temperature_mat_ir2; //ir2
	void CreateGroundTemperatureTable(CString satStr, Date startDate, SatDataType channel);
	void CreateGroundTemperatureTable(SatDataType channel, int difference);
	void CreateGroundTemperatureTable(SatDataType channel);
	bool ReadFixedTimeAwxData(CString satStr, float* pData, Date date, int hour, int minute, SatDataType channel, int nframe);
	void DrawGroundTemperature(int nframe);

	//pixel type
	void CloudGroundSegment(); //cloud-ground labeling
	void Classification(); //cirrus-water-ice labeling
	void LabelCirrus(int nframe); //cirrus detection
	void SegmentSatDataKMeans(int nframe); //ice-water labeling
	void SegmentSatDataIRWV(int nframe);
	void DrawPixelType(int nframe);

	//cloud top height
	void CreateCloudTopHeight();
	bool FindaAndb(int nframe, int i, int j, float& a, float& b);
	void DrawCloudTopSurface(int nframe);
	void ComputeTriangleNormal(double normal[3], double PA[3], double PB[3], double PC[3]);

	//ir4 to reflectance
	float PlanckFunction(int channel, float T);
	void IR4Temperature2Reflectance();
	void ModifyReflectance();

	//cloud properties of the ice cloud and the water cloud.
	void ComputeCloudProperties_MEA();
	void DrawEfficientRadius(int nframe);

	//geometric thickness
	void ComputeGeoThick();
	void DrawGeoThick(int nframe);
	void DrawCloudBottomSurface(int nframe);
	void DrawExt(int nframe);

	//output to file
	void GenerateExtinctionFieldFile(int startFrame, int endFrame, int Z_Res);
	void GenerateCloudParticlesFile(string savePath, string saveName, int startFrame, int endFrame);

	float Dis2Boundary(float samplingpoint[3], float* tempCTH, float* temCBH, int X_RES, int Y_RES);

	void GenerateCloudFieldIfoFile(int startFrame, int endFrame);

	void GenerateIfoForDobashi();

	//output file for earth rendering
	void GenerateCloudFiledFileEarth(int startFrame, int endFrame, int X_Res, int Y_Res, int Z_Res);
	Vector3 MatMult(float Mat[9], Vector3 vec);

	bool FindPosInLgLatTable(float lg, float lat, int& y, int& x);

public:
	void TimeChannel2FileName(CString satStr, char filename[256], Date date, int hour, int minute, SatDataType channel);

	float PhaseFunction(Vector3 v1, Vector3 v2);

	float MetaballFunction(float r, float R);

	float InterPolateCTHField(float samplePoint[2], float* cthList, int nframe);
	float InterPolateGeoThickField(float samplePoint[2], float* geo_thick_data, int nframe);
	float InterPolateExtinctionPlane(float samplePoint[2], float* extinctionPlane, int nframe);

	bool isProbabilityGreater(double threahold);
	//draw each type
	void Draw(DrawType type);
	void DrawEarth();
	void DrawSimulationCube();

	//cloud particles data file
	int puffNumber;
	vector<Vector3> puffPosVec;
	vector<float> puffSizeVec;
	vector<Color4> puffColorVec;
	vector <float> puffExtVec;

	//output cloud model: cloudxx.dat
	void ExportCloudModel(char* cloudfile);

	void ExportCloudFieldIfo(int nframe, char* cloudfile);
	bool FindSlope(int nframe, int i, int j, float& slope, float* pdata_ir, float* pData_wv);
	void RenderFrame(SatDataType channel, int n_frame);
	void RenderFrame(DrawType dtype);

	void PrintRunIfo(string info);

	Perlin* perlin;
public:
	~SatDataCloud(void);
};