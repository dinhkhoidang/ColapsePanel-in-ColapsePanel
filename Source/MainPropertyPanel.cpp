/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2015 - ROLI Ltd.

   Permission is granted to use this software under the terms of either:
   a) the GPL v2 (or any later version)
   b) the Affero GPL v3

   Details of these licenses can be found at: www.gnu.org/licenses

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

   ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.juce.com for more information.

  ==============================================================================
*/

#include "MainPropertyPanel.h"

struct MainPropertyPanel::SectionComponent : public Component
{
	SectionComponent(const String& sectionTitle,
		const Array<PropertyComponent*>& newProperties,
		const bool sectionIsOpen,const int _index, const int _titleHeight)
		: Component(sectionTitle),
		index(_index),
		titleHeight(sectionTitle.isNotEmpty() ? _titleHeight : 0),
		isOpen(sectionIsOpen)
	{
		propertyComps.addArray(newProperties);
		/*bordercolor = Settings::getColorFromString(Settings::secondaryPanels.collapsed.borderColor);
		borderThick = Settings::secondaryPanels.border;*/
		bordercolor = Colours::orange;
		borderThick = 1;
		for (int i = propertyComps.size(); --i >= 0;)
		{
			addAndMakeVisible(propertyComps.getUnchecked(i));
			propertyComps.getUnchecked(i)->refresh();
		}
	}

	~SectionComponent()
	{
		propertyComps.clear();
	}

	void paint(Graphics& g) override
	{
		const float cornerSize = 0;// jmin(15.0f, jmin(width, height) * 0.45f);
		const float lineThickness = float(borderThick);
		const float halfThickness = lineThickness * 0.5f;
		g.setColour(bordercolor);
		Path outline;
		outline.addRoundedRectangle(0.5f + halfThickness, 0.5f + halfThickness, this->getWidth() - lineThickness, this->getHeight() - lineThickness,
			cornerSize, cornerSize);
		g.strokePath(outline, PathStrokeType(lineThickness)); // draw border
		if (titleHeight > 0)
			getLookAndFeel().drawPropertyPanelSectionHeader(g, getName(), isOpen, getWidth(), titleHeight);
	}

	void resized() override
	{
		int y = titleHeight;

		for (int i = 0; i < propertyComps.size(); ++i)
		{
			PropertyComponent* const pec = propertyComps.getUnchecked(i);
			pec->setBounds(1, y, getWidth() - 2, pec->getPreferredHeight());
			y = pec->getBottom();
		}
	}

	int getPreferredHeight() const
	{
		
		int y = titleHeight;

		if (isOpen)
			for (int i = propertyComps.size(); --i >= 0;)
				y += propertyComps.getUnchecked(i)->getPreferredHeight();

		return y;
	}

	void setOpen(const bool open)
	{
		if (isOpen != open)
		{
			isOpen = open;
			isOpenState = true;
			for (int i = propertyComps.size(); --i >= 0;)
				propertyComps.getUnchecked(i)->setVisible(open);

			if (MainPropertyPanel* const pp = findParentComponentOfClass<MainPropertyPanel>()) {
				pp->resized();
				
			}
		}
		isOpenState = false;

	}

	void refreshAll() const
	{
		for (int i = propertyComps.size(); --i >= 0;)
			propertyComps.getUnchecked(i)->refresh();
	}

	void mouseUp(const MouseEvent& e) override
	{
		if (e.getMouseDownX() < titleHeight
			&& e.x < titleHeight
			&& e.getNumberOfClicks() != 2)
			mouseDoubleClick(e);
	}

	void mouseDoubleClick(const MouseEvent& e) override
	{

		if (e.y < titleHeight) {
			if (MainPropertyPanel* const pp = findParentComponentOfClass<MainPropertyPanel>()) {
				//pp->setCollapseAll();
				pp->indexActive = index;
				pp->callChangeListeners();
				
			}
			setOpen(!isOpen);	
			
		}
	}
	
	OwnedArray<PropertyComponent> propertyComps;
	const int titleHeight;
	int borderThick;
	bool isOpen;
	bool isOpenState;
	Colour bordercolor;
	MainPropertyPanel parent;
	int index;
	JUCE_DECLARE_NON_COPYABLE(SectionComponent)
};

//==============================================================================
struct MainPropertyPanel::PropertyHolderComponent : public Component
{
	PropertyHolderComponent() {}

	void paint(Graphics&) override {
	}

	void updateLayout(int width)
	{
		int y = 0;

		for (int i = 0; i < sections.size(); ++i)
		{
			SectionComponent* const section = sections.getUnchecked(i);

			section->setBounds(0, y + 10, width, section->getPreferredHeight());// set padding between two item
			y = section->getBottom();
		}

		setSize(width, y);
		repaint();
	}

	void refreshAll() const
	{
		for (int i = 0; i < sections.size(); ++i)
			sections.getUnchecked(i)->refreshAll();
	}

	void insertSection(int indexToInsertAt, SectionComponent* newSection)
	{
		sections.insert(indexToInsertAt, newSection);
		addAndMakeVisible(newSection, 0);
	}

	SectionComponent* getSectionWithNonEmptyName(const int targetIndex) const noexcept
	{
		for (int index = 0, i = 0; i < sections.size(); ++i)
		{
			SectionComponent* const section = sections.getUnchecked(i);

			if (section->getName().isNotEmpty())
				if (index++ == targetIndex)
					return section;
		}

		return nullptr;
	}
	SectionComponent* getSectionActive() const noexcept
	{
		for (auto i = 0; i < sections.size(); ++i)
		{
			SectionComponent* const section = sections.getUnchecked(i);

			if (section->isOpenState)				
				return section;
		}

		return nullptr;
	}
	int getIndexSectionActive() const noexcept
	{	
		for (auto i = 0; i < sections.size(); ++i)
		{
			SectionComponent* const section = sections.getUnchecked(i);

			if (section->isOpenState)
				return section->index;
		}
		return 0;
	}
	void collapseAll() {
		for (auto i = 0; i < sections.size(); ++i)
		{
			SectionComponent* const section = sections.getUnchecked(i);
			section->setOpen(false);
		}
	}
	OwnedArray<SectionComponent> sections;

	JUCE_DECLARE_NON_COPYABLE(PropertyHolderComponent)
};

//==============================================================================
MainPropertyPanel::MainPropertyPanel()
{
	init();
}

MainPropertyPanel::MainPropertyPanel(const String& name) : Component(name)
{
	init();
}

void MainPropertyPanel::init()
{
	messageWhenEmpty = TRANS("(nothing selected)");

	addAndMakeVisible(viewport);
	viewport.setViewedComponent(propertyHolderComponent = new PropertyHolderComponent());
	viewport.setFocusContainer(true);
}

MainPropertyPanel::~MainPropertyPanel()
{
	listeners.clear();
	clear();

}

//==============================================================================
void MainPropertyPanel::paint(Graphics& g)
{
	if (isEmpty())
	{
		g.setColour(Colours::black.withAlpha(0.5f));
		g.setFont(14.0f);
		g.drawText(messageWhenEmpty, getLocalBounds().withHeight(30),
			Justification::centred, true);
	}
}

void MainPropertyPanel::resized()
{
	viewport.setBounds(getLocalBounds());
	updatePropHolderLayout();
}

void MainPropertyPanel::valueChanged(Value &)
{

}

void MainPropertyPanel::callChangeListeners( )
{

	Component::BailOutChecker checker(this);
	listeners.callChecked(checker, &MainPropertyPanelListener::selectedChanged, this, indexActive);  // (can't use Label::Listener due to idiotic VC2005 bug)
}



void MainPropertyPanel::addListener(MainPropertyPanelListener * listener)
{
	listeners.add(listener);
}

void MainPropertyPanel::removeListener(MainPropertyPanelListener * listener)
{
	listeners.remove(listener);
}

//==============================================================================
void MainPropertyPanel::clear()
{
	if (!isEmpty())
	{
		propertyHolderComponent->sections.clear();
		updatePropHolderLayout();
	}
}

bool MainPropertyPanel::isEmpty() const
{
	return propertyHolderComponent->sections.size() == 0;
}

int MainPropertyPanel::getTotalContentHeight() const
{
	return propertyHolderComponent->getHeight();
}


void MainPropertyPanel::addProperties(const Array<PropertyComponent*>& newProperties,const int titleHeight)
{
	if (isEmpty())
		repaint();

	propertyHolderComponent->insertSection(-1, new SectionComponent(String::empty, newProperties, true,-1, titleHeight));
	updatePropHolderLayout();
}

void MainPropertyPanel::addSection(const String& sectionTitle,
	const Array<PropertyComponent*>& newProperties,
	const bool shouldBeOpen,
	const int indexToInsertAt, const int titleHeight)
{
	jassert(sectionTitle.isNotEmpty());

	if (isEmpty())
		repaint();

	propertyHolderComponent->insertSection(indexToInsertAt, new SectionComponent(sectionTitle, newProperties, shouldBeOpen, indexToInsertAt, titleHeight));
	updatePropHolderLayout();
}

void MainPropertyPanel::updatePropHolderLayout() const
{
	const int maxWidth = viewport.getMaximumVisibleWidth();
	propertyHolderComponent->updateLayout(maxWidth);

	const int newMaxWidth = viewport.getMaximumVisibleWidth();
	if (maxWidth != newMaxWidth)
	{
		// need to do this twice because of scrollbars changing the size, etc.
		propertyHolderComponent->updateLayout(newMaxWidth);
	}
}

void MainPropertyPanel::refreshAll() const
{
	propertyHolderComponent->refreshAll();
}


//==============================================================================
StringArray MainPropertyPanel::getSectionNames() const
{
	StringArray s;

	for (int i = 0; i < propertyHolderComponent->sections.size(); ++i)
	{
		SectionComponent* const section = propertyHolderComponent->sections.getUnchecked(i);

		if (section->getName().isNotEmpty())
			s.add(section->getName());
	}

	return s;
}

bool MainPropertyPanel::isSectionOpen(const int sectionIndex) const
{
	if (SectionComponent* s = propertyHolderComponent->getSectionWithNonEmptyName(sectionIndex))
		return s->isOpen;

	return false;
}

void MainPropertyPanel::setSectionOpen(const int sectionIndex, const bool shouldBeOpen)
{
	for (auto i = 0; i < propertyHolderComponent->getNumChildComponents(); i++)
	{
		if (shouldBeOpen == true)
		{
			if (i != sectionIndex)
			{
				if (SectionComponent* s = propertyHolderComponent->getSectionWithNonEmptyName(i))
					s->setOpen(false);
			}
		}
	}
	if (SectionComponent* s = propertyHolderComponent->getSectionWithNonEmptyName(sectionIndex))
		s->setOpen(shouldBeOpen);
}

void MainPropertyPanel::setSectionEnabled(const int sectionIndex, const bool shouldBeEnabled)
{
	if (SectionComponent* s = propertyHolderComponent->getSectionWithNonEmptyName(sectionIndex))
		s->setEnabled(shouldBeEnabled);
}

void MainPropertyPanel::removeSection(int sectionIndex)
{
	if (SectionComponent* s = propertyHolderComponent->getSectionWithNonEmptyName(sectionIndex))
	{
		propertyHolderComponent->sections.removeObject(s);
		updatePropHolderLayout();
	}
}

//==============================================================================
XmlElement* MainPropertyPanel::getOpennessState() const
{
	XmlElement* const xml = new XmlElement("PROPERTYPANELSTATE");

	xml->setAttribute("scrollPos", viewport.getViewPositionY());

	const StringArray sections(getSectionNames());

	for (int i = 0; i < sections.size(); ++i)
	{
		if (sections[i].isNotEmpty())
		{
			XmlElement* const e = xml->createNewChildElement("SECTION");
			e->setAttribute("name", sections[i]);
			e->setAttribute("open", isSectionOpen(i) ? 1 : 0);
		}
	}

	return xml;
}

void MainPropertyPanel::restoreOpennessState(const XmlElement& xml)
{
	if (xml.hasTagName("PROPERTYPANELSTATE"))
	{
		const StringArray sections(getSectionNames());

		forEachXmlChildElementWithTagName(xml, e, "SECTION")
		{
			setSectionOpen(sections.indexOf(e->getStringAttribute("name")),
				e->getBoolAttribute("open"));
		}

		viewport.setViewPosition(viewport.getViewPositionX(),
			xml.getIntAttribute("scrollPos", viewport.getViewPositionY()));
	}
}

//==============================================================================
void MainPropertyPanel::setMessageWhenEmpty(const String& newMessage)
{
	if (messageWhenEmpty != newMessage)
	{
		messageWhenEmpty = newMessage;
		repaint();
	}
}

void MainPropertyPanel::setCollapseAll()
{
	propertyHolderComponent->collapseAll();
}

const String& MainPropertyPanel::getMessageWhenEmpty() const noexcept
{
	return messageWhenEmpty;
}