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

	QString folderPath = "C:\\Users\\StephaneRedon\\Desktop\\INF473N\\PDB\\1a1e\\";
	QDir folder(folderPath);

	QStringList pdbFiles = folder.entryList(QStringList() << "*.pdb" << "*.PDB", QDir::Files);
	QStringList sdfFiles = folder.entryList(QStringList() << "*.sdf" << "*.SDF", QDir::Files);

	SBList<std::string>* parameters = new SBList<std::string>();
	parameters->push_back("0");
	parameters->push_back("0");
	parameters->push_back("0");
	parameters->push_back("0");
	parameters->push_back("0");
	parameters->push_back("1");
	parameters->push_back("0");
	parameters->push_back("0");

	foreach(QString filename, sdfFiles) {

		SBPointer<SBFolder> folder = new SBFolder("Folder");
		folder->create();

		SAMSON::disableHolding();
		SAMSON::importFromFile((folderPath + filename).toStdString(), parameters, folder());
		SAMSON::enableHolding();

		// get atoms

		SBNodeIndexer atomIndexer;
		folder->getNodes(atomIndexer, SBNode::IsType(SBNode::Atom));

		SBIAPosition3 boundingBox = SBIAPosition3::empty;

		SB_FOR(SBNode * node, atomIndexer) {

			SBAtom* atom = static_cast<SBAtom*>(node);
			boundingBox.bound(atom->getPosition());

		}

		std::cout << boundingBox.i[0].diameter() << std::endl;
		std::cout << boundingBox.i[1].diameter() << std::endl;
		std::cout << boundingBox.i[2].diameter() << std::endl;
		std::cout << std::endl;

		// clean up

		folder.deleteReferenceTarget();

	}

}
