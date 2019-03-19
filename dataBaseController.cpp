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


void dataBaseController::extractTitles(std::vector<std::string> & titles)
{
	XmlTree headertree = tree->getChild("content");

	for (XmlTree::Iter iter2 = headertree.begin(); iter2 != headertree.end(); ++iter2) { // loop through all media-tags
		for (XmlTree::Iter iter3 = iter2->begin(); iter3 != iter2->end(); ++iter3) { // loop through inside media-tag
			if (iter3->hasChild("header")) {
				titles.push_back(iter3->getChild("header").getValue());
			}
		}
	}
}

void dataBaseController::extractBodies(std::vector<std::string> & bodyText)
{
	XmlTree headertree = tree->getChild("content");

	for (XmlTree::Iter iter2 = headertree.begin(); iter2 != headertree.end(); ++iter2) { // loop through all media-tags
		for (XmlTree::Iter iter3 = iter2->begin(); iter3 != iter2->end(); ++iter3) { // loop through inside media-tag
			if (iter3->hasChild("text")) {
				bodyText.push_back(iter3->getChild("text").getValue());
			}
		}
	}
}

void dataBaseController::extractImgPaths(std::vector<std::string> & imgPath)
{
	XmlTree headertree = tree->getChild("content");

	for (XmlTree::Iter iter2 = headertree.begin(); iter2 != headertree.end(); ++iter2) { // loop through all media-tags
		imgPath.push_back(iter2->getAttributeValue<std::string>("path"));
	}
}

void dataBaseController::extractCardCats(std::vector<std::string> & cardCategory)
{
	XmlTree headertree = tree->getChild("content");

	for (XmlTree::Iter iter2 = headertree.begin(); iter2 != headertree.end(); ++iter2) { // loop through all media-tags
		if (iter2->hasChild("category")) {
			cardCategory.push_back(iter2->getChild("category").getAttributeValue<std::string>("name"));
		}
	}

}


