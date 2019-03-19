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

	XmlTree mediatree = tree->getChild("content");
	
	for (XmlTree::Iter iter = mediatree.begin(); iter != mediatree.end(); ++iter) {
		if (iter->hasAttribute("name")) {
			CI_LOG_I("Tag: " + iter->getTag() + " Value: " + iter->getAttributeValue<std::string>("name"));
		}
		
		
	}	
	/*
	for (XmlTree::Iter iter = mediatree.begin(); iter != mediatree.end(); ++iter) {
		CI_LOG_I("Tag: " + iter->getTag() + "  Value: " + iter->getValue());
		for (XmlTree::Iter iter2 = iter->begin(); iter2 != iter->end(); ++iter2) {
			CI_LOG_I("Tag: " + iter2->getTag() + "  Value: " + iter2->getValue());
		}

	}
	*/


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
