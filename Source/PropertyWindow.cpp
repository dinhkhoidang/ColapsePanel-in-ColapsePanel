/*
  ==============================================================================

    PropertyWindow.cpp
    Created: 15 Jun 2016 1:21:30pm
    Author:  tuan.truong

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "PropertyWindow.h"


//==============================================================================
PropertyWindow::PropertyWindow():PropertyComponent("table",400)
{
	addAndMakeVisible(table);
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
	//data.push_back(new Person());
	addAndMakeVisible(saveButton);
	data.push_back(new Person("Enable", "On", "On, Off (On)", "Description"));
	data.push_back(new Person("Percent_Property", "10%", "0-100% (15%)", "Description"));
	data.push_back(new Person("Decibel_Property", "10 dB", "-18 - + dB(0 dB)", "Description"));
	data.push_back(new Person("Herzt_Property", "200 Hz","20 - 2000 Hz (30 Hz)", "Description"));
	data.push_back(new Person("Enum_property","Triple", "Single, Double, Triple (Double)", "Description"));
	
	table.getHeader().addColumn("A", 1, 1, 100, -1, TableHeaderComponent::notSortable);
	table.getHeader().addColumn("B", 2, 1, 40, -1, TableHeaderComponent::notSortable);
	table.getHeader().addColumn("C", 3, 1, 100, -1, TableHeaderComponent::notSortable);
	table.getHeader().addColumn(String::empty, 4, 1, 100, -1, TableHeaderComponent::notSortable);
	table.getHeader().addColumn(String::empty, 5, 1, 100, -1, TableHeaderComponent::notSortable);
	table.getHeader().setStretchToFitActive(true);
	table.getHeader().setPopupMenuActive(true);
	//table.getHeader().setSize(600, 1);
	//table.getHeader().setVisible(false);
	table.setModel(this);
	table.setColour(ListBox::outlineColourId, Colours::grey);
	table.setOutlineThickness(1);
	table.setRowHeight(40);
	TableHeaderComponent a;
	//setSize(600,500);
}

PropertyWindow::~PropertyWindow()
{
}

void PropertyWindow::refresh()
{
	repaint();
}

void PropertyWindow::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (Colours::white);   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::lightblue);
    g.setFont (14.0f);
    g.drawText ("PropertyWindow", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text

}

void PropertyWindow::resized()
{ 
	table.setBoundsInset (BorderSize<int> (12));
	table.setBounds(this->getBounds().reduced(20));
    // This method is where you should set the bounds of any child
    // components that your component contains..
	//saveButton.setBounds(150,350,20,20);
}

int PropertyWindow::getNumRows()
{
	return data.size();
	
}

void PropertyWindow::paintCell(Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
	
	for (int i = 0;i <= data.size();i++) {
		if (rowNumber == i) {
			if(columnId==1){
			g.drawFittedText(data[i]->getName(), 0, 0, width, height, Justification::centredLeft, 1);
			}
			else if (columnId == 2) {
			g.drawFittedText(data[i]->getBirth(), 0, 0, width, height, Justification::centredLeft, 1);
			}
			else if (columnId == 3) {
				g.drawFittedText(data[i]->getDeath(), 0, 0, width, height, Justification::centredLeft, 1);
			}
			else {
			
				g.drawFittedText(data[i]->getInfo(), 0, 0, width, height, Justification::centredLeft, 1);
			}

		}
	
	}
		g.setColour(Colours::black.withAlpha(0.2f));
	g.fillRect(width - 1, 0, 1, height);
}

void PropertyWindow::paintRowBackground(Graphics &g, int rowNumber, int width, int height, bool rowIsSelected)
{
	if (rowIsSelected)
		g.fillAll(Colours::orange);//Colours::lightblue
	else if (rowNumber % 2)
		g.fillAll(Colours::grey);//Colour(0xffeeeeee)
}

int PropertyWindow::getCombo(const int rowNumber) const
{
	return 1;
}

void PropertyWindow::setCombo(const int rowNumber, const int newRating)
{
}

Component * PropertyWindow::refreshComponentForCell(int rowNumber, int columnId, bool, Component * existingComponentToUpdate)
{
	if (columnId == 1 || columnId == 7) // The ID and Length columns do not have a custom component
	{
		jassert(existingComponentToUpdate == nullptr);
		return nullptr;
	}

	if (columnId == 2 && rowNumber == 4) // For the ratings column, we return the custom combobox component
	{
		ListComboBox* listBox = static_cast<ListComboBox*> (existingComponentToUpdate);

		// If an existing component is being passed-in for updating, we'll re-use it, but
		// if not, we'll have to create one.
		if (listBox == nullptr)
			listBox = new ListComboBox(*this);

		listBox->setRowAndColumn(rowNumber, columnId);
		return listBox;
	}

	// The other columns are editable text columns, for which we use the custom Label component
	EditableTextCustomComponent* textLabel = static_cast<EditableTextCustomComponent*> (existingComponentToUpdate);

	// same as above...
	if (textLabel == nullptr)
		textLabel = new EditableTextCustomComponent(*this);

	textLabel->setRowAndColumn(rowNumber, columnId);
	return textLabel;
}
String PropertyWindow::getText(const int columnNumber, const int rowNumber) const
{
	string s;
	if (columnNumber == 1) {
		s = data[rowNumber]->getName();
	}
	else if (columnNumber == 2) {
		
		s = data[rowNumber]->getBirth();
	}
	else if (columnNumber == 3) {
	
		s = data[rowNumber]->getDeath();

	}
	else  {
		s = data[rowNumber]->getInfo();

	}
	return s;
}

void PropertyWindow::setText(const int columnNumber, const int rowNumber, const String & newText)
{
	//const String& columnName = table.getHeader().getColumnName(columnNumber);
	string  n = newText.toStdString();
	if (columnNumber == 1) {
		data[rowNumber]->setName(n);
	}
	else if (columnNumber == 2) {

		data[rowNumber]->setBirth(n);
	}
	else if (columnNumber == 3) {

		data[rowNumber]->setDeath(n);

	}
	else {
		data[rowNumber]->setInfo(n);

	}

}

void PropertyWindow::buttonClicked(Button * buttonSaveClicked)
{
	//std::sort(data.begin(), data.end(), &sortList);
	

}

bool PropertyWindow::sortList(Person & p1, Person & p2)
{
	//if (p1.getName() != p2.getName())
	//	return p1.getName() < p2.getName();
	return true;
}
