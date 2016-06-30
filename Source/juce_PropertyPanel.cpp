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
#include "juce_PropertyPanel.h"

struct myPropertyPanel::SectionComponent : public Component
{
	SectionComponent(const String& sectionTitle,
		const Array<PropertyComponent*>& newProperties,
		const bool sectionIsOpen)
		: Component(sectionTitle),
		titleHeight(sectionTitle.isNotEmpty() ? 22 : 0),
		isOpen(sectionIsOpen)
	{
		propertyComps.addArray(newProperties);

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
		const float lineThickness = 10 * 0.1f;
		const float halfThickness = lineThickness * 0.5f;
		g.setColour(Colours::red);
		Path outline;
		outline.addRoundedRectangle(0.5f + halfThickness, 0.5f + halfThickness, this->getWidth() - lineThickness, this->getHeight() - lineThickness,
			cornerSize, cornerSize);
		g.strokePath(outline, PathStrokeType(lineThickness));
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

			for (int i = propertyComps.size(); --i >= 0;)
				propertyComps.getUnchecked(i)->setVisible(open);

			if (PropertyPanel* const pp = findParentComponentOfClass<PropertyPanel>())
				pp->resized();
		}
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
			setOpen(!isOpen);
		}
	}

	OwnedArray<PropertyComponent> propertyComps;
	const int titleHeight;
	bool isOpen;

	JUCE_DECLARE_NON_COPYABLE(SectionComponent)
};

//==============================================================================
struct myPropertyPanel::PropertyHolderComponent : public Component
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

			section->setBounds(0, y + 5, width, section->getPreferredHeight());
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

	OwnedArray<SectionComponent> sections;

	JUCE_DECLARE_NON_COPYABLE(PropertyHolderComponent)
};

//==============================================================================
myPropertyPanel::myPropertyPanel() : PropertyComponent("dd", 200)
{
	init();
}

myPropertyPanel::myPropertyPanel(const String& name) : PropertyComponent(name,200)
{
	init();
}

void myPropertyPanel::init()
{
	messageWhenEmpty = TRANS("(nothing selected)");

	addAndMakeVisible(viewport);
	viewport.setViewedComponent(propertyHolderComponent = new PropertyHolderComponent());
	viewport.setFocusContainer(true);
}

myPropertyPanel::~myPropertyPanel()
{
	clear();
}

//==============================================================================
void myPropertyPanel::paint(Graphics& g)
{
	if (isEmpty())
	{
		g.setColour(Colours::black.withAlpha(0.5f));
		g.setFont(14.0f);
		g.drawText(messageWhenEmpty, getLocalBounds().withHeight(30),
			Justification::centred, true);
	}
}

void myPropertyPanel::resized()
{
	viewport.setBounds(getLocalBounds());
	updatePropHolderLayout();
}

//==============================================================================
void myPropertyPanel::clear()
{
	if (!isEmpty())
	{
		propertyHolderComponent->sections.clear();
		updatePropHolderLayout();
	}
}

bool myPropertyPanel::isEmpty() const
{
	return propertyHolderComponent->sections.size() == 0;
}

int myPropertyPanel::getTotalContentHeight() const
{
	return propertyHolderComponent->getHeight();
}

void myPropertyPanel::addProperties(const Array<PropertyComponent*>& newProperties)
{
	if (isEmpty())
		repaint();

	propertyHolderComponent->insertSection(-1, new SectionComponent(String::empty, newProperties, true));
	updatePropHolderLayout();
}

void myPropertyPanel::addSection(const String& sectionTitle,
	const Array<PropertyComponent*>& newProperties,
	const bool shouldBeOpen,
	const int indexToInsertAt)
{
	jassert(sectionTitle.isNotEmpty());

	if (isEmpty())
		repaint();

	propertyHolderComponent->insertSection(indexToInsertAt, new SectionComponent(sectionTitle, newProperties, shouldBeOpen));
	updatePropHolderLayout();
}

void myPropertyPanel::updatePropHolderLayout() const
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

void myPropertyPanel::refreshAll() const
{
	propertyHolderComponent->refreshAll();
}

void myPropertyPanel::refresh()
{
	refreshAll();
}

//==============================================================================
StringArray myPropertyPanel::getSectionNames() const
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

bool myPropertyPanel::isSectionOpen(const int sectionIndex) const
{
	if (SectionComponent* s = propertyHolderComponent->getSectionWithNonEmptyName(sectionIndex))
		return s->isOpen;

	return false;
}

void myPropertyPanel::setSectionOpen(const int sectionIndex, const bool shouldBeOpen)
{
	for (size_t i = 0; i < propertyHolderComponent->getNumChildComponents(); i++)
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

void myPropertyPanel::setSectionEnabled(const int sectionIndex, const bool shouldBeEnabled)
{
	if (SectionComponent* s = propertyHolderComponent->getSectionWithNonEmptyName(sectionIndex))
		s->setEnabled(shouldBeEnabled);
}

void myPropertyPanel::removeSection(int sectionIndex)
{
	if (SectionComponent* s = propertyHolderComponent->getSectionWithNonEmptyName(sectionIndex))
	{
		propertyHolderComponent->sections.removeObject(s);
		updatePropHolderLayout();
	}
}

//==============================================================================
XmlElement* myPropertyPanel::getOpennessState() const
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

void myPropertyPanel::restoreOpennessState(const XmlElement& xml)
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
void myPropertyPanel::setMessageWhenEmpty(const String& newMessage)
{
	if (messageWhenEmpty != newMessage)
	{
		messageWhenEmpty = newMessage;
		repaint();
	}
}

const String& myPropertyPanel::getMessageWhenEmpty() const noexcept
{
	return messageWhenEmpty;
}