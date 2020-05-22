#include "SEComplexAnalyzerAppGUI.hpp"
#include "SEComplexAnalyzerApp.hpp"
#include "SAMSON.hpp"
#include "SBGWindow.hpp"


SEComplexAnalyzerAppGUI::SEComplexAnalyzerAppGUI( SEComplexAnalyzerApp* t ) : SBGApp( t ) {

	ui.setupUi( this );

}

SEComplexAnalyzerAppGUI::~SEComplexAnalyzerAppGUI() {

}

SEComplexAnalyzerApp* SEComplexAnalyzerAppGUI::getApp() const { return static_cast<SEComplexAnalyzerApp*>(SBGApp::getApp()); }

void SEComplexAnalyzerAppGUI::loadSettings(SBGSettings* settings) {

	if (settings == NULL) return;
	
	// SAMSON Element generator pro tip: complete this function so your app can save its GUI state from one session to the next

}

void SEComplexAnalyzerAppGUI::saveSettings(SBGSettings* settings) {

	if (settings == NULL) return;

	// SAMSON Element generator pro tip: complete this function so your app can save its GUI state from one session to the next

}

SBCContainerUUID SEComplexAnalyzerAppGUI::getUUID() const { return SBCContainerUUID( "E4B79809-9649-0122-66AA-1FC3123C2BBA" );}

QPixmap SEComplexAnalyzerAppGUI::getLogo() const { 
	
	// SAMSON Element generator pro tip: this icon will be visible in the GUI title bar. 
	// Modify it to better reflect the purpose of your app.

	return QPixmap(QString::fromStdString(SB_ELEMENT_PATH + "/Resource/icons/SEComplexAnalyzerAppIcon.png"));

}

QString SEComplexAnalyzerAppGUI::getName() const { 

	// SAMSON Element generator pro tip: this string will be the GUI title. 
	// Modify this function to have a user-friendly description of your app inside SAMSON

	return "Complex analyzer"; 

}

int SEComplexAnalyzerAppGUI::getFormat() const { 
	
	// SAMSON Element generator pro tip: modify these default settings to configure the window
	//
	// SBGWindow::Savable : let users save and load interface settings (implement loadSettings and saveSettings)
	// SBGWindow::Lockable : let users lock the window on top
	// SBGWindow::Resizable : let users resize the window
	// SBGWindow::Citable : let users obtain citation information (implement getCitation)
	
	return (SBGWindow::Savable | SBGWindow::Lockable | SBGWindow::Resizable | SBGWindow::Citable);

}

QString SEComplexAnalyzerAppGUI::getCitation() const {

	// SAMSON Element generator pro tip: modify this function to add citation information

	return
		"If you use this app in your work, please cite: <br/>"
		"<br/>"
		"[1] <a href=\"https://www.samson-connect.net\">https://www.samson-connect.net</a><br/>";
	
}

void SEComplexAnalyzerAppGUI::onStart() {

	getApp()->analyze();

}

void SEComplexAnalyzerAppGUI::onGetType() {

	getApp()->getAtomType();

}


void SEComplexAnalyzerAppGUI::onPL() {

	getApp()->analyzePL();

}


void SEComplexAnalyzerAppGUI::onPrediction() {

	getApp()->prediction();

	
}