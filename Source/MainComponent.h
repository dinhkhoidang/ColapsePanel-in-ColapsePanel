/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "juce_PropertyPanel.h"
#include "MainPropertyPanel.h"
#include "SubPropertyPanel.h"
#include "dts-table-model.h"
#include "PropertyWindow.h"
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component,public SubPropertyPanel::Listener
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();
	void  selectedChanged(SubPropertyPanel* sub, const int indexSection)override;
    void paint (Graphics&);
    void resized();
	Array<PropertyComponent*> createMode(int sectionIdx);

private:
    //==============================================================================
	ScopedPointer<MainPropertyPanel> panel;
	SubPropertyPanel* one;
	SubPropertyPanel* two;
	PropertyWindow* table;

	enum GroupType {
		kMain,
		kSub1,
		kSub2,
		kSub3
	};
	enum ControlType {
		kDials,
		kRadio,
		kToggle,
		kCombobox,
		kSub
	};
	struct PropertyControl
	{
		GroupType groupParent;
		GroupType groupOrder;
		ControlType controlType;
		String uuid;
		PropertyControl(GroupType _groupParent, GroupType _groupOrder, ControlType _controlType, String _uuid)
			:groupParent(_groupParent), groupOrder(_groupOrder), controlType(_controlType), uuid(_uuid)
		{

		}
	};

	OwnedArray<PropertyControl> list;
	//dtsTablePropertyComp* tablepro;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
