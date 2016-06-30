/*
  ==============================================================================

    PropertyWindow.h
    Created: 15 Jun 2016 1:21:30pm
    Author:  tuan.truong

  ==============================================================================
*/

#ifndef PROPERTYWINDOW_H_INCLUDED
#define PROPERTYWINDOW_H_INCLUDED
#include <string>  //for std::string

using std::string;
using std::vector;
#include "../JuceLibraryCode/JuceHeader.h"
#include "Model.h"
//==============================================================================
/*
*/
class PropertyWindow    : public PropertyComponent,
	public TableListBoxModel, public Button::Listener
{
public:
	vector<Person*> data;
	//vector<String> data;
    PropertyWindow();
    ~PropertyWindow();
	void refresh() override;
    void paint (Graphics&);
    void resized();
	int getNumRows();
	void paintCell(Graphics &,
		int 	rowNumber,
		int 	columnId,
		int 	width,
		int 	height,
		bool 	rowIsSelected
	);
	void paintRowBackground(Graphics &,
		int 	rowNumber,
		int 	width,
		int 	height,
		bool 	rowIsSelected
	);
	int getCombo(const int rowNumber) const;
	void setCombo(const int rowNumber, const int newRating);
	Component * refreshComponentForCell(int rowNumber, int columnId, bool, Component * existingComponentToUpdate);
	String getText(const int columnNumber, const int rowNumber) const;
	void setText(const int columnNumber, const int rowNumber, const String & newText);
	void buttonClicked(Button *);
	bool  sortList(Person & p1, Person & p2);

private:
	TextButton saveButton;
	TableListBox table;
	//ScopedPointer<XmlElement> demoData;   // This is the XML document loaded from the embedded file "demo table data.xml"
	int numRows;            // The number of rows of data we've got
	class EditableTextCustomComponent : public Label
	{
	public:
		EditableTextCustomComponent(PropertyWindow& td) : owner(td)
		{
			// double click to edit the label text; single click handled below
			setEditable(false, true, false);
			setColour(textColourId, Colours::black);
		}

		void mouseDown(const MouseEvent& event) override
		{
			// single click on the label should simply select the row
			owner.table.selectRowsBasedOnModifierKeys(row, event.mods, false);

			Label::mouseDown(event);
		}

		void textWasEdited() override
		{
			owner.setText(columnId, row, getText());
		}

		// Our demo code will call this when we may need to update our contents
		void setRowAndColumn(const int newRow, const int newColumn)
		{
			row = newRow;
			columnId = newColumn;
			setText(owner.getText(columnId, row), dontSendNotification);
		}

	private:
		PropertyWindow& owner;
		int row, columnId;
	};
	class ListComboBox : public Component,
		private ComboBoxListener
	{
	public:
		ListComboBox(PropertyWindow& td) : owner(td)
		{
			// just put a combo box inside this component
			addAndMakeVisible(comboBox);
			comboBox.addItem("Single", 1);
			comboBox.addItem("Double", 2);
			comboBox.addItem("Triple", 3);
			comboBox.setSelectedId(3);
			// when the combo is changed, we'll get a callback.
			comboBox.addListener(this);
			comboBox.setWantsKeyboardFocus(false);
		}

		void resized() override
		{
			comboBox.setBoundsInset(BorderSize<int>(2));
		}

		// Our demo code will call this when we may need to update our contents
		void setRowAndColumn(int newRow, int newColumn)
		{
			row = newRow;
			columnId = newColumn;
			//comboBox.setSelectedId(owner.getCombo(row), dontSendNotification);
		}

		void comboBoxChanged(ComboBox*) override
		{
		//	owner.setCombo(row, comboBox.getSelectedId());
		}

	private:
		
		PropertyWindow& owner;
		ComboBox comboBox;
		int row, columnId;
	};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PropertyWindow)
};


#endif  // PROPERTYWINDOW_H_INCLUDED
