#include "SEComplexAnalyzerApp.hpp"
#include "SEComplexAnalyzerAppGUI.hpp"
#include "SAMSON.hpp"
#include <QDir>
#include <QFileInfo>

SEComplexAnalyzerApp::SEComplexAnalyzerApp() {

	setGUI(new SEComplexAnalyzerAppGUI(this));
	getGUI()->loadDefaultSettings();

}

SEComplexAnalyzerApp::~SEComplexAnalyzerApp() {

	getGUI()->saveDefaultSettings();
	delete getGUI();

}

SEComplexAnalyzerAppGUI* SEComplexAnalyzerApp::getGUI() const { return static_cast<SEComplexAnalyzerAppGUI*>(SBDApp::getGUI()); }

void SEComplexAnalyzerApp::analyze() {

	QString folderPath = "D:\\INF473N\\refined-set\\";

	SBList<std::string>* parameters = new SBList<std::string>();
	parameters->push_back("0");
	parameters->push_back("0");
	parameters->push_back("0");
	parameters->push_back("0");
	parameters->push_back("0");
	parameters->push_back("1");
	parameters->push_back("0");
	parameters->push_back("0");

	QDir folder(folderPath);
	QStringList dirnames = folder.entryList(QDir::AllDirs);
	int a = folder.count();
	int b = 0;
	SBIAPosition3 boxmax = SBIAPosition3::empty;


	foreach(QString dir, dirnames) {

		QStringList sdfFiles = QDir(folderPath + dir).entryList(QStringList() << "*.sdf" << "*.SDF", QDir::Files);


		foreach(QString filename, sdfFiles) {
			b++;
			SBPointer<SBFolder> folder = new SBFolder("Folder");
			folder->create();

			SAMSON::disableHolding();
			SAMSON::importFromFile((folderPath + dir +"\\"+ filename).toStdString(), parameters, folder());
			SAMSON::enableHolding();

			// get atoms

			SBNodeIndexer atomIndexer;
			folder->getNodes(atomIndexer, SBNode::IsType(SBNode::Atom));

			SBIAPosition3 boundingBox = SBIAPosition3::empty;

			SB_FOR(SBNode * node, atomIndexer) {

				SBAtom* atom = static_cast<SBAtom*>(node);
				boundingBox.bound(atom->getPosition());

			}
			


			
			if (boundingBox.i[0].diameter() > boxmax.i[0].diameter()) {

				boxmax.i[0]=boundingBox.i[0];

			}

			if (boundingBox.i[1].diameter() > boxmax.i[1].diameter()) {

				boxmax.i[1] = boundingBox.i[1];

			}

			if (boundingBox.i[2].diameter() > boxmax.i[2].diameter()) {

				boxmax.i[2] = boundingBox.i[2];

			}


			std::cout << b << "sur " + a << std::endl;

			// clean up

			folder.deleteReferenceTarget();

		}
	
	}

	std::cout << boxmax.i[0].diameter() << std::endl;
	std::cout << boxmax.i[1].diameter() << std::endl;
	std::cout << boxmax.i[2].diameter() << std::endl;
	std::cout << std::endl;




}
