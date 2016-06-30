#ifndef __JUCE_HEADER_dtsTablePropertyComp__
#define __JUCE_HEADER_dtsTablePropertyComp__
#include "../JuceLibraryCode/JuceHeader.h"
#include "dts-table-model.h"
class dtsTablePropertyComp: public PropertyComponent
{
public:
	dtsTablePropertyComp();
	~dtsTablePropertyComp();
	void refresh() override;
private:
	ScopedPointer<TableListBox> table;
	//ScopedPointer<dtsTableModel> model;
};
#endif
