#include "dataBaseController.h"


dataBaseController::dataBaseController()
{
}

dataBaseController::dataBaseController(std::string fileLocation, std::string inFileType, std::string filename)
	: fileLocation(fileLocation), fileType(inFileType)
{
	tree = establishConnection(filename);
}


dataBaseController::~dataBaseController()
{
}


ci::XmlTree * dataBaseController::establishConnection(std::string url)
{	
	/* online fileLocation */
	if (fileLocation == "online") 
	{ 
		try
		{
			return new ci::XmlTree(loadUrl(url));
		}
		catch (cinder::StreamExc)
		{
			CI_LOG_I("invalid url");
			return nullptr;
		}
		
	}
	/* local fileLocation */
	else if (fileLocation == "local") 
	{
		try
		{
			return new ci::XmlTree(loadFile(url));
		}
		catch (cinder::StreamExc)
		{
			CI_LOG_I("invalid url");
			return nullptr;
		}
	}
	else
	{
		CI_LOG_I("local or online file location only");
		return nullptr;
	}
	
}
