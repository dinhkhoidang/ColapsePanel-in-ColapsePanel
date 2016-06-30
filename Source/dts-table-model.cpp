#include "dts-table-model.h"



dtsTableModel::dtsTableModel()
{
	arr.add("dang");
	arr.add("dang");
	arr.add("dang");
}


dtsTableModel::~dtsTableModel()
{
}

int dtsTableModel::getNumRows()
{
	return 0;
}

void dtsTableModel::paintRowBackground(Graphics &, int rowNumber, int width, int height, bool rowIsSelected)
{
}

void dtsTableModel::paintCell(Graphics &, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
}

Component * dtsTableModel::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component * existingComponentToUpdate)
{
	return nullptr;
}

void dtsTableModel::cellClicked(int rowNumber, int columnId, const MouseEvent &)
{
}

void dtsTableModel::cellDoubleClicked(int rowNumber, int columnId, const MouseEvent &)
{
}

void dtsTableModel::backgroundClicked(const MouseEvent &)
{
}

void dtsTableModel::sortOrderChanged(int newSortColumnId, bool isForwards)
{
}

int dtsTableModel::getColumnAutoSizeWidth(int columnId)
{
	return 0;
}

String dtsTableModel::getCellTooltip(int rowNumber, int columnId)
{
	return String();
}

void dtsTableModel::selectedRowsChanged(int lastRowSelected)
{
}

void dtsTableModel::deleteKeyPressed(int lastRowSelected)
{
}

void dtsTableModel::returnKeyPressed(int lastRowSelected)
{
}

void dtsTableModel::listWasScrolled()
{
}
