#include "dataBaseController.h"


dataBaseController::dataBaseController()
{
}


dataBaseController::dataBaseController(std::string fileLocation, std::string inFileType, std::string url)
	: fileLocation(fileLocation), fileType(inFileType)
{
	if (fileType == "xml")
	{
		tree = establishConnection(url);
	}
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


void dataBaseController::extractCategories(std::vector<std::string> & categories)
{
	XmlTree branch = tree->getChild("content");
	//categories = new std::vector <std::string>;

	for (XmlTree::Iter iter = branch.begin(); iter != branch.end(); ++iter) 
	{
		if (iter->hasAttribute("name")) 
		{
			categories.push_back(iter->getAttributeValue<std::string>("name"));
		}
	}

}
