#include "SEComplexAnalyzerApp.hpp"
#include "SEComplexAnalyzerAppGUI.hpp"
#include "SAMSON.hpp"
#include <QDir>
#include <QFileInfo>
#include <QDataStream>

SEComplexAnalyzerApp::SEComplexAnalyzerApp() {

	setGUI(new SEComplexAnalyzerAppGUI(this));
	getGUI()->loadDefaultSettings();

}

SEComplexAnalyzerApp::~SEComplexAnalyzerApp() {

	getGUI()->saveDefaultSettings();
	delete getGUI();

}

SEComplexAnalyzerAppGUI* SEComplexAnalyzerApp::getGUI() const { 
	return static_cast<SEComplexAnalyzerAppGUI*>(SBDApp::getGUI()); 
}

void SEComplexAnalyzerApp::analyze() {

	QString folderPath = "D:\\Modal INF473N\\PDB";

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
			SAMSON::importFromFile((folderPath + dir + "\\" + filename).toStdString(), parameters, folder());
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

				boxmax.i[0] = boundingBox.i[0];

			}

			if (boundingBox.i[1].diameter() > boxmax.i[1].diameter()) {

				boxmax.i[1] = boundingBox.i[1];

			}

			if (boundingBox.i[2].diameter() > boxmax.i[2].diameter()) {

				boxmax.i[2] = boundingBox.i[2];

			}


			std::cout << b << "sur " << a << std::endl;

			// clean up

			folder.deleteReferenceTarget();

		}

	}

	std::cout << boxmax.i[0].diameter() << std::endl;
	std::cout << boxmax.i[1].diameter() << std::endl;
	std::cout << boxmax.i[2].diameter() << std::endl;
	std::cout << std::endl;




}


void SEComplexAnalyzerApp::getAtomType() {


	/*
	1 Hydrogen
	6 Carbon
	7 Nitrogen
	8 Oxygen
	9 Fluorine
	11 Sodium
	12 Magnesium
	15 Phosphorus
	16 Sulfur
	17 Chlorine
	20 Calcium
	25 Manganese
	26 Iron
	27 Cobalt
	28 Nickel
	29 Copper
	30 Zinc
	35 Bromine
	48 Cadmium
	53 Iodine
	80 Mercury
	*/






	//QString folderPath = "D:\\INF473N\\refined-set\\";
	QString folderPath = "D:\\Modal INF473N\\PDB";

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

	std::set<SBElement::Type> atomType;

	foreach(QString dir, dirnames) {

		QStringList sdfFiles = QDir(folderPath + dir).entryList(QStringList() << "*.sdf" << "*.SDF", QDir::Files);
		QStringList pdbFiles = QDir(folderPath + dir).entryList(QStringList() << "*protein.pdb" << "*protein.PDB", QDir::Files);

		QStringList files = sdfFiles + pdbFiles;

		foreach(QString filename, files) {
			b++;
			SBPointer<SBFolder> folder = new SBFolder("Folder");
			folder->create();

			SAMSON::disableHolding();
			SAMSON::importFromFile((folderPath + dir + "\\" + filename).toStdString(), parameters, folder());
			SAMSON::enableHolding();

			// get atoms

			SBNodeIndexer atomIndexer;
			folder->getNodes(atomIndexer, SBNode::IsType(SBNode::Atom));

			SB_FOR(SBNode * node, atomIndexer) {

				SBAtom* atom = static_cast<SBAtom*>(node);
				atomType.insert(atom->getElementType());

			}
			folder.deleteReferenceTarget();
		}


	}

	for (std::set<SBElement::Type>::iterator i = atomType.begin(); i != atomType.end(); i++) {

		std::cout << *i << " " << SAMSON::getElementName(*i) << std::endl;
	}

}

void figureOutInfluence(SBMStructuralModelGrid* grid, const SBPosition3& positionCase, double* values) {
	SBIndexer< SBStructuralParticle*> neighbors;
	grid->getNeighbors(neighbors, positionCase, SBQuantity::angstrom(4 * sqrt(3)));

	SB_FOR(SBStructuralParticle * particle, neighbors) {

		SBAtom* atom = static_cast<SBAtom*>(particle);
		double d = (positionCase - atom->getPosition()).norm().getValue();

		if (atom != NULL) {
			SBElement::Type typeAtom = atom->getElementType();
			double vanDerValsRadius = atom->getVanDerWaalsRadius().getValue();
			double gaussian = 10*std::exp(-(d * d) / (2 * vanDerValsRadius* vanDerValsRadius));
			switch (typeAtom)
			{
			case SBElement::Type::H:
				values[0] += gaussian;
				break;
			case SBElement::Type::C:
				values[1] += gaussian;
				break;
			case SBElement::Type::N:
				values[2] += gaussian;
				break;
			case SBElement::Type::O:
				values[3] += gaussian;
				break;
			case SBElement::Type::S:
				values[4] += gaussian;
				break;
			case SBElement::Type::P:
				values[5] += gaussian;
				break;
			case SBElement::Type::F:
				values[6] += gaussian;
				break;
			case SBElement::Type::Cl:
				values[7] += gaussian;
				break;
			case SBElement::Type::Br:
				values[8] += gaussian;
				break;
			case SBElement::Type::I:
				values[9] += gaussian;
				break;
			case SBElement::Type::Magnesium:
				values[10] += gaussian;
				break;
			case SBElement::Type::Calcium:
				values[11] += gaussian;
				break;
			case SBElement::Type::Manganese:
				values[12] += gaussian;
				break;
			case SBElement::Type::Iron:
				values[13] += gaussian;
				break;
			case SBElement::Type::Cobalt:
				values[14] += gaussian;
				break;
			case SBElement::Type::Nickel:
				values[15] += gaussian;
				break;
			case SBElement::Type::Copper:
				values[16] += gaussian;
				break;
			case SBElement::Type::Zinc:
				values[17] += gaussian;
				break;
			case SBElement::Type::Cadmium:
				values[18] += gaussian;
				break;
			case SBElement::Type::Mercury:
				values[19] += gaussian;
				break;
			}
		}
	}
}


void SEComplexAnalyzerApp::analyzePL() {

	//QString folderPath = "D:\\INF473N\\refined-set\\";
	QString folderPath = "D:\\Modal INF473N\\PDB\\";
	QString dataPath = "D:\\Modal INF473N\\Data\\Data";

	QFile* fileP; // ("D:\\Modal INF473N\\Data\\DataP.dat");
	QFile* filePL; // ("D:\\Modal INF473N\\Data\\DataPL.dat");
	QDataStream* outP;
	QDataStream* outPL;


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
	QStringList dirnames = folder.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);

	const int fileSize = 400;
	int currentSize = 0;
	int numCurrentFile = 1;

	double* vP = new double[20];
	double* vPL = new double[20];

	foreach(QString dir, dirnames) {

		if (currentSize == fileSize) {
			++numCurrentFile;
			currentSize = 0;
		}

		if (currentSize == 0) {
			if (numCurrentFile > 1) {
				fileP->close();
				filePL->close();
				delete(fileP);
				delete(filePL);
				delete(outP);
				delete(outPL);

			}
			fileP = new QFile(dataPath + "P" + QString::number(numCurrentFile) + ".dat");
			filePL = new QFile(dataPath + "PL" + QString::number(numCurrentFile) + ".dat");

			fileP->open(QIODevice::WriteOnly);
			filePL->open(QIODevice::WriteOnly);

			outP = new QDataStream(fileP);
			outPL = new QDataStream(filePL);

		}


		SBPointer<SBFolder> folderP = new SBFolder("FolderP");
		folderP->create();

		SBPointer<SBFolder> folderL = new SBFolder("FolderL");
		folderL->create();

		QStringList sdfFiles = QDir(folderPath + dir).entryList(QStringList() << "*.sdf" << "*.SDF", QDir::Files);
		QStringList pdbFiles = QDir(folderPath + dir).entryList(QStringList() << "*protein.pdb" << "*protein.PDB", QDir::Files);
	
		SAMSON::disableHolding();
		foreach(QString filename, sdfFiles) {
			SAMSON::importFromFile((folderPath + dir + "\\" + filename).toStdString(), parameters, folderL());
		}

		foreach(QString filename, pdbFiles) {
			SAMSON::importFromFile((folderPath + dir + "\\" + filename).toStdString(), parameters, folderP());
		}
		SAMSON::enableHolding();

		SBNodeIndexer atomIndexerP;
		folderP->getNodes(atomIndexerP, SBNode::IsType(SBNode::Atom));

		SBNodeIndexer atomIndexerL;
		folderL->getNodes(atomIndexerL, SBNode::IsType(SBNode::Atom));

		SBNodeIndexer atomIndexerPL = atomIndexerP;
		SB_FOR(SBNode * node, atomIndexerL) {
			atomIndexerPL.addNode(node);
		}


		SBIAPosition3 boundingBox = SBIAPosition3::empty;

		SB_FOR(SBNode * node, atomIndexerL) {
			SBAtom* atom = static_cast<SBAtom*>(node);
			boundingBox.bound(atom->getPosition());
		}

		SBPosition3 center(boundingBox.i[0].center(), boundingBox.i[1].center(), boundingBox.i[2].center());
		//std::cout << center<<std::endl;

		SBMStructuralModelGrid* gridP = createGrid(atomIndexerP);
		SBMStructuralModelGrid* gridPL = createGrid(atomIndexerPL);

		const int gridSize = 28;
		
		for (int x = -0.5* gridSize; x < 0.5 * gridSize; ++x) {
			for (int y = -0.5 * gridSize; y < 0.5 * gridSize; ++y) {
				for (int z = -0.5 * gridSize; z < 0.5 * gridSize; ++z) {
					
					SBIndexer< SBStructuralParticle*> neighbors;
					SBAtom* closestAtom = 0;
					SBQuantity::length dmin(DBL_MAX);

					SBPosition3 positionCase(center.v[0] + SBQuantity::angstrom(2) * x, center.v[1] + SBQuantity::angstrom(2) * y, center.v[2] + SBQuantity::angstrom(2) * z);
					for (int i = 0; i < 20; ++i) {
						vP[i] = 0;
						vPL[i] = 0;
					}

					figureOutInfluence(gridP, positionCase, vP);
					figureOutInfluence(gridPL, positionCase, vPL);

					for (int i = 0; i < 20; ++i) {
						outP->writeRawData(reinterpret_cast<const char*>(&vP[i]), sizeof(vP[i]));
						outPL->writeRawData(reinterpret_cast<const char*>(&vPL[i]), sizeof(vPL[i]));
					}					
				}
			}
		}

		folderP.deleteReferenceTarget();
		folderL.deleteReferenceTarget();

		++currentSize;
	}
	delete[] vP;
	delete[] vPL;
	delete fileP;
	delete filePL;
	delete outP;
	delete outPL;
	fileP->close();
	filePL->close();
}


SBMStructuralModelGrid* SEComplexAnalyzerApp::createGrid(SBNodeIndexer atomIndexer) {


	SBMStructuralModelGrid* grid = new SBMStructuralModelGrid(atomIndexer, SBQuantity::angstrom(2));

	return grid;
}


void SEComplexAnalyzerApp::prediction() {


	SBNodeIndexer nodeIndexer;
	SAMSON::getActiveDocument()->getNodes(nodeIndexer, SBNode::IsType(SBNode::Atom) && SBNode::IsSelected());

	SBMStructuralModelGrid* grid = createGrid(nodeIndexer);
	
	SBPosition3 c;
	c.setZero();

	int i = 0;
	SB_FOR(SBNode * node, nodeIndexer){

		SBAtom* atom = static_cast<SBAtom*>(node);
		SBPosition3 p = atom->getPosition();
		c = c + SBPosition3(p.v[0], p.v[1], p.v[2]);
		i = i + 1;

	}
	c.v[0] /= i;
	c.v[1] /= i;
	c.v[2] /= i;

	std::cout << c << std::endl;
}
