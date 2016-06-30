#include "dts-table-property-comp.h"



dtsTablePropertyComp::dtsTablePropertyComp(): PropertyComponent("Table",200)
{
//	model = new dtsTableModel();
	addAndMakeVisible(table = new TableListBox("table"));
	
	table->getHeader().addColumn("A", 1, 1, 100, -1, TableHeaderComponent::notSortable);
	table->getHeader().addColumn("B", 2, 1, 40, -1, TableHeaderComponent::notSortable);
}


dtsTablePropertyComp::~dtsTablePropertyComp()
{
}

void dtsTablePropertyComp::refresh()
{
}
