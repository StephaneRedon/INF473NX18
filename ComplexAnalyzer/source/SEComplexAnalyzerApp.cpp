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
	QString folderPath = "D:\\pdb\\";

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


void SEComplexAnalyzerApp::analyzePL() {

	//QString folderPath = "D:\\INF473N\\refined-set\\";
	QString folderPath = "D:\\pdb\\";

	QFile fileP("D:\\INF473N\\Donnees\\DataP.dat");
	QFile filePL("D:\\INF473N\\Donnees\\DataPL.dat");
	fileP.open(QIODevice::WriteOnly);
	filePL.open(QIODevice::WriteOnly);

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


	foreach(QString dir, dirnames) {

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
		std::cout << center<<std::endl;

		SBMStructuralModelGrid* gridP = createGrid(atomIndexerP);
		SBMStructuralModelGrid* gridPL = createGrid(atomIndexerPL);

		QByteArray vP;
		QByteArray vPL;
	

		for (int x = 0; x < 100; x = x++) {
			for (int y = 0; y < 100; y = y++) {
				for (int z = 0; z < 100; z++) {
					
					SBIndexer< SBStructuralParticle*> neighbors;
					SBAtom* closestAtom = 0;
					SBQuantity::length dmin(DBL_MAX);

					SBPosition3 positionCase(center.v[0] + SBQuantity::angstrom(0.5) * (x - 50), center.v[1] + SBQuantity::angstrom(0.5) * (y - 50), center.v[2] + SBQuantity::angstrom(0.5) * (z - 50));

					gridP->getNeighbors(neighbors, positionCase,SBQuantity::angstrom(0.5*sqrt(3)));
					SB_FOR(SBStructuralParticle * particle, neighbors) {

						SBAtom* atom = static_cast<SBAtom*>(particle);
						SBQuantity::length d = (positionCase - atom->getPosition()).norm();

						if (d < dmin) {
							closestAtom = atom;
							dmin = d;
						}

					}
					if (closestAtom == 0) {
						vP.push_back((char) 0);
					}
					else {
						SBElement::Type typeAtom = closestAtom->getElementType();

						if (typeAtom == SBElement::Type::H) {
							vP.push_back((char)1);
						}
						else if (typeAtom == SBElement::Type::C) {
							vP.push_back((char)2);
						}
						else if (typeAtom == SBElement::Type::N) {
							vP.push_back((char)3);
						}
						else if (typeAtom == SBElement::Type::O) {
							vP.push_back((char)4);
						}
						else if (typeAtom == SBElement::Type::S) {
							vP.push_back((char)5);
						}
						else if (typeAtom == SBElement::Type::P) {
							vP.push_back((char)6);
						}
						else if (typeAtom == SBElement::Type::F) {
							vP.push_back((char)7);
						}
						else if (typeAtom == SBElement::Type::Cl) {
							vP.push_back((char)8);
						}
						else if (typeAtom == SBElement::Type::Br) {
							vP.push_back((char)9);
						}
						else if (typeAtom == SBElement::Type::I) {
							vP.push_back((char)10);
						}
						else if (typeAtom == SBElement::Type::Magnesium) {
							vP.push_back((char)11);
						}
						else if (typeAtom == SBElement::Type::Calcium) {
							vP.push_back((char)12);
						}
						else if (typeAtom == SBElement::Type::Manganese) {
							vP.push_back((char)13);
						}
						else if (typeAtom == SBElement::Type::Iron) {
							vP.push_back((char)14);
						}
						else if (typeAtom == SBElement::Type::Cobalt) {
							vP.push_back((char)15);
						}
						else if (typeAtom == SBElement::Type::Nickel) {
							vP.push_back((char)16);
						}
						else if (typeAtom == SBElement::Type::Copper) {
							vP.push_back((char)17);
						}
						else if (typeAtom == SBElement::Type::Zinc) {
							vP.push_back((char)18);
						}
						else if (typeAtom == SBElement::Type::Cadmium) {
							vP.push_back((char)19);
						}
						else if (typeAtom == SBElement::Type::Mercury) {
							vP.push_back((char)20);
						}

						else {
							vP.push_back((char)0);
						}
					}

					neighbors = SBIndexer< SBStructuralParticle*>();

					gridPL->getNeighbors(neighbors, positionCase, SBQuantity::angstrom(0.5 * sqrt(3)));
					dmin = SBQuantity::length(DBL_MAX);

					SB_FOR(SBStructuralParticle * particle, neighbors) {

						SBAtom* atom = static_cast<SBAtom*>(particle);
						SBQuantity::length d = (positionCase - atom->getPosition()).norm();

						if (d < dmin) {
							closestAtom = atom;
							dmin = d;
						}

					}
					if (closestAtom==0) {
						vPL.push_back((char)0);
					}
					else {

						SBElement::Type typeAtom = closestAtom->getElementType();
						if (typeAtom == SBElement::Type::H) {
							vPL.push_back((char)1);
						}
						else if (typeAtom == SBElement::Type::C) {
							vPL.push_back((char)2);
						}
						else if (typeAtom == SBElement::Type::N) {
							vPL.push_back((char)3);
						}
						else if (typeAtom == SBElement::Type::O) {
							vPL.push_back((char)4);
						}
						else if (typeAtom == SBElement::Type::S) {
							vPL.push_back((char)5);
						}
						else if (typeAtom == SBElement::Type::P) {
							vPL.push_back((char)6);
						}
						else if (typeAtom == SBElement::Type::F) {
							vPL.push_back((char)7);
						}
						else if (typeAtom == SBElement::Type::Cl) {
							vPL.push_back((char)8);
						}
						else if (typeAtom == SBElement::Type::Br) {
							vPL.push_back((char)9);
						}
						else if (typeAtom == SBElement::Type::I) {
							vPL.push_back((char)10);
						}
						else if (typeAtom == SBElement::Type::Magnesium) {
							vPL.push_back((char)11);
						}
						else if (typeAtom == SBElement::Type::Calcium) {
							vPL.push_back((char)12);
						}
						else if (typeAtom == SBElement::Type::Manganese) {
							vPL.push_back((char)13);
						}
						else if (typeAtom == SBElement::Type::Iron) {
							vPL.push_back((char)14);
						}
						else if (typeAtom == SBElement::Type::Cobalt) {
							vPL.push_back((char)15);
						}
						else if (typeAtom == SBElement::Type::Nickel) {
							vPL.push_back((char)16);
						}
						else if (typeAtom == SBElement::Type::Copper) {
							vPL.push_back((char)17);
						}
						else if (typeAtom == SBElement::Type::Zinc) {
							vPL.push_back((char)18);
						}
						else if (typeAtom == SBElement::Type::Cadmium) {
							vPL.push_back((char)19);
						}
						else if (typeAtom == SBElement::Type::Mercury) {
							vPL.push_back((char)20);
						}

						else {
							vPL.push_back((char)0);
						}




					}


					

					
				}
			}
		}

		
		
		
		
		fileP.write(vP);
		filePL.write(vPL);
		
		



		folderP.deleteReferenceTarget();
		folderL.deleteReferenceTarget();


	}
	fileP.close();
	filePL.close();
}



SBMStructuralModelGrid* SEComplexAnalyzerApp::createGrid(SBNodeIndexer atomIndexer) {


	SBMStructuralModelGrid* grid = new SBMStructuralModelGrid(atomIndexer, SBQuantity::angstrom(0.5));

	return grid;
}


