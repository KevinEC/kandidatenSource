#include "dataBaseController.h"


dataBaseController::dataBaseController()
{
}

dataBaseController::dataBaseController(std::string fileLocation, std::string inFileType)
	: fileLocation(fileLocation), fileType(inFileType)
{
}


dataBaseController::~dataBaseController()
{
}

ci::XmlTree * dataBaseController::establishConnection(std::string url)
{	
	if (fileLocation == "online") 
	{ 
		CI_LOG_I("it's online!");
		return new ci::XmlTree(loadUrl(url));
	}

	else if (fileLocation == "local") 
	{
		CI_LOG_I("it's local!");
		return new ci::XmlTree(loadFile(url));
	}
	else
	{
		CI_LOG_I("local or online file location only");
		return nullptr;
	}
	
	/*else 
	{
		CI_LOG_I("invalid url");
		return nullptr;
	}*/

	
}
