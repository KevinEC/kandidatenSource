#include "dataBaseController.h"

// default constructor
dataBaseController::dataBaseController()
{
}

// constructor taking in parameters to connect to data base
dataBaseController::dataBaseController(std::string fileLocation, std::string inFileType, std::string url)
	: fileLocation(fileLocation), fileType(inFileType)
{
	if (fileType == "xml")
	{
		tree = establishConnection(url);
	}
}

// destructor
dataBaseController::~dataBaseController()
{

}

//instantiate XmlTree from local or online source file
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

// store all categories in vector
void dataBaseController::extractCategories(std::vector<std::string> & categories)
{
	XmlTree branch = tree->getChild("content");

	for (XmlTree::Iter iter = branch.begin(); iter != branch.end(); ++iter) 
	{
		if (iter->hasAttribute("name")) 
		{
			categories.push_back(iter->getAttributeValue<std::string>("name"));
		}
	}

}

// store swedish title in first slot and english title in second slot of a pair
void dataBaseController::extractTitles(std::vector< std::pair<std::string, std::string> > & titles)
{
	XmlTree headertree = tree->getChild("content");
	std::pair<std::string, std::string> temp;
	
	for (XmlTree::Iter iter2 = headertree.begin(); iter2 != headertree.end(); ++iter2) { // loop through all media-tags
		if (iter2->hasAttribute("scale_exp")) {
			for (XmlTree::Iter iter3 = iter2->begin(); iter3 != iter2->end(); ++iter3) { // loop through inside media-tag
				if (iter3->hasChild("header")) {
					if (iter3->getTag() == "se") { //swedish
						temp.first = (iter3->getChild("header").getValue());
					}
					else { //english 
						temp.second = (iter3->getChild("header").getValue());
					}
				}
			}
			titles.push_back(temp);
			temp.first.clear();
			temp.second.clear();
		}
	}
}

// store swedish body text in first slot and english body text in second slot of a pair
void dataBaseController::extractBodies(std::vector< std::pair<std::string, std::string> > & bodyText)
{
	XmlTree headertree = tree->getChild("content");
	std::pair<std::string, std::string> temp;

	for (XmlTree::Iter iter2 = headertree.begin(); iter2 != headertree.end(); ++iter2) { // loop through all media-tags
		if (iter2->hasAttribute("scale_exp")) {
			for (XmlTree::Iter iter3 = iter2->begin(); iter3 != iter2->end(); ++iter3) { // loop through inside media-tag
				if (iter3->hasChild("text")) {
					if (iter3->getTag() == "se") { //swedish
						temp.first = (iter3->getChild("text").getValue());
					}
					else { //english 
						temp.second = (iter3->getChild("text").getValue());
					}
				}
			}
			bodyText.push_back(temp);
			temp.first.clear();
			temp.second.clear();
		}
	}
}

// store paths to images in vector
void dataBaseController::extractImgPaths(std::vector<std::string> & imgPath)
{
	XmlTree headertree = tree->getChild("content");

	for (XmlTree::Iter iter2 = headertree.begin(); iter2 != headertree.end(); ++iter2) { // loop through all media-tags
		if (iter2->hasAttribute("scale_exp")) {
			imgPath.push_back("http://www.student.itn.liu.se/~chrad171/databas/databas" + iter2->getAttributeValue<std::string>("path"));
		}
	}
}

// store each cards categories in vector
void dataBaseController::extractCardCats(std::vector<std::vector<std::string> > & cardCategory)
{
	XmlTree headertree = tree->getChild("content");
	std::vector<std::string> tempvec;

	for (XmlTree::Iter iter2 = headertree.begin("media"); iter2 != headertree.end(); ++iter2) { // loop through all media-tags

		for (XmlTree::Iter iter3 = iter2->begin("Category"); iter3 != iter2->end(); ++iter3) {
			tempvec.push_back(iter3->getAttributeValue<std::string>("name"));
		}
		
		cardCategory.push_back(tempvec);
		tempvec.clear();
		
		}
	}


