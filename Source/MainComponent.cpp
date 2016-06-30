/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/
#include "dts-table-property-comp.h"
#include "MainComponent.h"

class TextBox : public TextPropertyComponent
{
public:
	TextBox(const String& propertyName)
		: TextPropertyComponent(Value(var(propertyName)), propertyName, 200, false)
	{
		refresh();
	}

	void paint(Graphics &g) override {
		g.setColour(Colours::transparentBlack);
		g.fillAll();
	}

	void resized() override {
		if (Component* const component = getChildComponent(0))
			component->setBounds(22, 0, this->getWidth() - 35, 22);
	}

private:

	//LabelComp1 textEditor;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TextBox)
};

//==============================================================================
MainContentComponent::MainContentComponent()
{
   
	list.add(new PropertyControl(GroupType::kMain, GroupType::kMain, ControlType::kSub, "1"));
	list.add(new PropertyControl(GroupType::kMain, GroupType::kSub1, ControlType::kDials, "2"));
	list.add(new PropertyControl(GroupType::kMain, GroupType::kSub1, ControlType::kRadio, "3"));
	list.add(new PropertyControl(GroupType::kMain, GroupType::kMain, ControlType::kSub, "4"));
	list.add(new PropertyControl(GroupType::kMain, GroupType::kSub1, ControlType::kToggle, "5"));

	addAndMakeVisible(panel = new MainPropertyPanel("ddd"));
	//addAndMakeVisible(tablepro = new dtsTablePropertyComp());
	//addAndMakeVisible(table);
	panel->addSection("Ok good", createMode(1), true);
	panel->addSection("Ok good",createMode(1), true);
	//panel->setBounds(0, 0, 300, 300);
	setSize (600, 400);
}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::selectedChanged(SubPropertyPanel * sub, const int indexSection)
{
	panel->refreshAll();
}

void MainContentComponent::paint (Graphics& g)
{
    //g.fillAll (Colours::white);

    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), Justification::centred, true);
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
	panel->setBounds(this->getBounds());

}

Array<PropertyComponent*> MainContentComponent::createMode(int sectionIdx)
{

	Array<PropertyComponent*> comps;
	Array<PropertyComponent*> compssub;
	Array<PropertyComponent*> compssub1;
	 one = new SubPropertyPanel("Khoi",400);
	// tablepro = new dtsTablePropertyComp();
	compssub.add(new TextBox("Dang"));
	compssub.add(new TextBox("Dinh"));
	compssub.add(new TextBox("Khoi"));
	one->addSection("Sub1", compssub, false,1,25);
	
	//two = new CustomPropertyPanelSub("Khoi");
	compssub1.add(new TextBox("Dang"));
	compssub1.add(new TextBox("Dinh"));
	compssub1.add(table = new PropertyWindow());
	one->addSection("Sub2",compssub1, false,2,25);
	//two->addSection("Sub", compssub1, true);
	//..one->addSection(compssub)
	comps.add(one);
	//comps.add(two);
	return comps;
//Array<PropertyComponent*> comps;
//
//	for (auto i : list)
//	{
//		PropertyComponent* control;
//		switch (i->groupParent)
//		{
//		case GroupType::kMain:
//			
//			break;
//		case GroupType::kSub1:
//			break;
//		case GroupType::kSub2:
//			break;
//		default:
//			break;
//		}
//		switch (i->controlType)
//		{
//		case ControlType::kDials:
//			comps.add(control =new TextBox("Dang"));
//			break;
//		case ControlType::kSub:
//			comps.add(control =new SubPropertyPanel("Khoi", 400));
//
//		default:
//			break;
//		}
//
//		
//	}
	
}
