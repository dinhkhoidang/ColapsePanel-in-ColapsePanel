#pragma once
#ifndef __JUCE_HEADER_FD25722B9E65E4C6__
#define __JUCE_HEADER_FD25722B9E65E4C6__

#include "../JuceLibraryCode/JuceHeader.h"

class dtsTableModel :public TableListBoxModel
{
public:
	dtsTableModel();
	~dtsTableModel();
	//==============================================================================
	/** This must return the number of rows currently in the table.

	If the number of rows changes, you must call TableListBox::updateContent() to
	cause it to refresh the list.
	*/
	int getNumRows();

	/** This must draw the background behind one of the rows in the table.

	The graphics context has its origin at the row's top-left, and your method
	should fill the area specified by the width and height parameters.

	Note that the rowNumber value may be greater than the number of rows in your
	list, so be careful that you don't assume it's less than getNumRows().
	*/
	void paintRowBackground(Graphics&,
		int rowNumber,
		int width, int height,
		bool rowIsSelected);

	/** This must draw one of the cells.

	The graphics context's origin will already be set to the top-left of the cell,
	whose size is specified by (width, height).

	Note that the rowNumber value may be greater than the number of rows in your
	list, so be careful that you don't assume it's less than getNumRows().
	*/
	void paintCell(Graphics&,
		int rowNumber,
		int columnId,
		int width, int height,
		bool rowIsSelected) ;

	//==============================================================================
	/** This is used to create or update a custom component to go in a cell.

	Any cell may contain a custom component, or can just be drawn with the paintCell() method
	and handle mouse clicks with cellClicked().

	This method will be called whenever a custom component might need to be updated - e.g.
	when the table is changed, or TableListBox::updateContent() is called.

	If you don't need a custom component for the specified cell, then return nullptr.
	(Bear in mind that even if you're not creating a new component, you may still need to
	delete existingComponentToUpdate if it's non-null).

	If you do want a custom component, and the existingComponentToUpdate is null, then
	this method must create a new component suitable for the cell, and return it.

	If the existingComponentToUpdate is non-null, it will be a pointer to a component previously created
	by this method. In this case, the method must either update it to make sure it's correctly representing
	the given cell (which may be different from the one that the component was created for), or it can
	delete this component and return a new one.
	*/
	Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected,
		Component* existingComponentToUpdate);

	//==============================================================================
	/** This callback is made when the user clicks on one of the cells in the table.

	The mouse event's coordinates will be relative to the entire table row.
	@see cellDoubleClicked, backgroundClicked
	*/
	void cellClicked(int rowNumber, int columnId, const MouseEvent&);

	/** This callback is made when the user clicks on one of the cells in the table.

	The mouse event's coordinates will be relative to the entire table row.
	@see cellClicked, backgroundClicked
	*/
	void cellDoubleClicked(int rowNumber, int columnId, const MouseEvent&);

	/** This can be overridden to react to the user double-clicking on a part of the list where
	there are no rows.

	@see cellClicked
	*/
	void backgroundClicked(const MouseEvent&);

	//==============================================================================
	/** This callback is made when the table's sort order is changed.

	This could be because the user has clicked a column header, or because the
	TableHeaderComponent::setSortColumnId() method was called.

	If you implement this, your method should re-sort the table using the given
	column as the key.
	*/
	void sortOrderChanged(int newSortColumnId, bool isForwards);

	//==============================================================================
	/** Returns the best width for one of the columns.

	If you implement this method, you should measure the width of all the items
	in this column, and return the best size.

	Returning 0 means that the column shouldn't be changed.

	This is used by TableListBox::autoSizeColumn() and TableListBox::autoSizeAllColumns().
	*/
	int getColumnAutoSizeWidth(int columnId);

	/** Returns a tooltip for a particular cell in the table. */
	String getCellTooltip(int rowNumber, int columnId);

	//==============================================================================
	/** Override this to be informed when rows are selected or deselected.
	@see ListBox::selectedRowsChanged()
	*/
	void selectedRowsChanged(int lastRowSelected);

	/** Override this to be informed when the delete key is pressed.
	@see ListBox::deleteKeyPressed()
	*/
	void deleteKeyPressed(int lastRowSelected);

	/** Override this to be informed when the return key is pressed.
	@see ListBox::returnKeyPressed()
	*/
	void returnKeyPressed(int lastRowSelected);

	/** Override this to be informed when the list is scrolled.

	This might be caused by the user moving the scrollbar, or by programmatic changes
	to the list position.
	*/
	void listWasScrolled();

	/** To allow rows from your table to be dragged-and-dropped, implement this method.

	If this returns a non-null variant then when the user drags a row, the table will try to
	find a DragAndDropContainer in its parent hierarchy, and will use it to trigger a
	drag-and-drop operation, using this string as the source description, and the listbox
	itself as the source component.

	@see getDragSourceCustomData, DragAndDropContainer::startDragging
	*/
	//var getDragSourceDescription(const SparseSet<int>& currentlySelectedRows);

	private:
		StringArray arr;
};

#endif   // __JUCE_HEADER_FD25722B9E65E4C6__
