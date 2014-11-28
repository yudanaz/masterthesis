#include "camerasystemconfig.h"

CameraSystemConfig::CameraSystemConfig()
{
	QList<qint16> empty;
	initialize(empty);
}

CameraSystemConfig::CameraSystemConfig(QList<qint16> bands)
{
	initialize(bands);
}

CameraSystemConfig::CameraSystemConfig(const CameraSystemConfig &aConfig)
{
	ringlightInterface = aConfig.ringlightInterface;
	wavebands = aConfig.wavebands;
	autoCameraSearch = aConfig.autoCameraSearch;
	IPadress = aConfig.IPadress;
	frequency = aConfig.frequency;
	integrationTime = aConfig.integrationTime;
	bufferSize = aConfig.bufferSize;
}

CameraSystemConfig& CameraSystemConfig::operator =(const CameraSystemConfig& aConfig)
{
	if (this != &aConfig )
	{
		ringlightInterface = aConfig.ringlightInterface;
		wavebands = aConfig.wavebands;
		autoCameraSearch = aConfig.autoCameraSearch;
		IPadress = aConfig.IPadress;
		frequency = aConfig.frequency;
		integrationTime = aConfig.integrationTime;
		bufferSize = aConfig.bufferSize;
	}
	return *this;
}

CameraSystemConfig::~CameraSystemConfig()
{
	;
}

void CameraSystemConfig::initialize(QList<qint16> bands)
{
	quint8 i;

#ifdef __linux__
	ringlightInterface = "/dev/ttyUSB0";
#endif
#ifdef _WIN32
	ringlightInterface = "COM3";
#endif
	frequency = maxFPS = 30;
	integrationTime = 10.0;

	//if no list of available wavebands was given, use default:
	if(bands.count() == 0)
	{
		bands << 935 << 1060 << 1300 << 1550;
	}

	//read waveband information from flashlight and set up wavebands structure
	wavebands.totalNumber = bands.count();
	wavebands.usedNumber = wavebands.totalNumber;
	for (i = 0; i < wavebands.totalNumber; i++)
	{
		wavebands.config.insert(bands.at(i), true);
	}

	autoCameraSearch = true;

	bufferSize = 3;
}
