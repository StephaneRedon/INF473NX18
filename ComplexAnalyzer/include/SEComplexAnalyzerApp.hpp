#pragma once 

#include "SBDApp.hpp"
#include "SEComplexAnalyzerAppGUI.hpp"
#include "SBMStructuralModelGrid.hpp"


/// This class implements the functionality of the app

// SAMSON Element generator pro tip: add non-GUI functionality in this class. The GUI functionality should go in the SEComplexAnalyzerAppGUI class

class SEComplexAnalyzerApp : public SBDApp {

	SB_CLASS

public :

	/// \name Constructors and destructors
	//@{

	SEComplexAnalyzerApp();																													///< Constructs an app
	virtual ~SEComplexAnalyzerApp();																											///< Destructs the app

	//@}

	/// \name GUI
	//@{

	SEComplexAnalyzerAppGUI*											getGUI() const;															///< Returns a pointer to the GUI of the app

	//@}

	void																analyze();
	void																getAtomType();
	void																analyzePL();
	SBMStructuralModelGrid*                                createGrid(SBNodeIndexer atomIndexer);
};


SB_REGISTER_TARGET_TYPE(SEComplexAnalyzerApp, "SEComplexAnalyzerApp", "ED88E0A4-E5FB-9C88-F7B4-71922CF96258");
SB_DECLARE_BASE_TYPE(SEComplexAnalyzerApp, SBDApp);
