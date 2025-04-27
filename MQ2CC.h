#pragma once

#include <mq/Plugin.h>               // Core MacroQuest plugin functionality
#include "eqlib/WindowOverride.h"    // For WindowOverride functionality

// Forward declaration of the custom override class
class CharacterCreationOverride : public WindowOverride<CharacterCreationOverride, CCharacterCreation>
{
public:
	// Called when the window is hooked
	static void OnHooked(CharacterCreationOverride* pWnd);

	// Called when the window is about to be unhooked
	static void OnAboutToUnhook(CharacterCreationOverride* pWnd);

	// Override the PostDraw method
	virtual int PostDraw() override;

	// Override mouse handling
	virtual int HandleLButtonDown(const CXPoint& pos, uint32_t flags) override;

	bool IsCharacterNameValid(CCharacterCreation* pCharacterCreation)
	{
		if (!pCharacterCreation)
		{
			DebugSpewAlways("IsCharacterNameValid: pCharacterCreation is null.");
			return false;
		}

		// Retrieve the character name input field
		CXWnd* pNameInput = pCharacterCreation->GetChildItem("CC_NameInput");
		if (!pNameInput)
		{
			DebugSpewAlways("IsCharacterNameValid: CC_NameInput not found.");
			return false;
		}

		// Get the text from the input field
		CXStr nameText = pNameInput->GetWindowText();
		std::string characterName = nameText.c_str();

		// Check if the name is empty
		if (characterName.empty())
		{
			DebugSpewAlways("IsCharacterNameValid: Character name is empty.");
			return false;
		}

		// Add additional validation logic if needed (e.g., length, special characters)
		DebugSpewAlways("IsCharacterNameValid: Character name is valid: %s", characterName.c_str());
		return true;
	}

	// Override WndNotification to handle notifications
	int CharacterCreationOverride::WndNotification(CXWnd* pWnd, uint32_t Message, void* pData) override
	{
		// Ensure the notification is valid and is a left-click event
		if (pWnd && Message == XWM_LCLICK)
		{
			// Retrieve the ScreenID of the button
			const char* screenID = pWnd->GetXMLData()->ScreenID.c_str();
			if (screenID)
			{
				// Map the ScreenID to a class ID
				int classID = 0;

				if (strcmp(screenID, "CC_Class_Warrior_Button") == 0) classID = 1;
				else if (strcmp(screenID, "CC_Class_Cleric_Button") == 0) classID = 2;
				else if (strcmp(screenID, "CC_Class_Paladin_Button") == 0) classID = 3;
				else if (strcmp(screenID, "CC_Class_Ranger_Button") == 0) classID = 4;
				else if (strcmp(screenID, "CC_Class_Shadow Knight_Button") == 0) classID = 5;
				else if (strcmp(screenID, "CC_Class_Druid_Button") == 0) classID = 6;
				else if (strcmp(screenID, "CC_Class_Monk_Button") == 0) classID = 7;
				else if (strcmp(screenID, "CC_Class_Bard_Button") == 0) classID = 8;
				else if (strcmp(screenID, "CC_Class_Rogue_Button") == 0) classID = 9;
				else if (strcmp(screenID, "CC_Class_Shaman_Button") == 0) classID = 10;
				else if (strcmp(screenID, "CC_Class_Necromancer_Button") == 0) classID = 11;
				else if (strcmp(screenID, "CC_Class_Wizard_Button") == 0) classID = 12;
				else if (strcmp(screenID, "CC_Class_Magician_Button") == 0) classID = 13;
				else if (strcmp(screenID, "CC_Class_Enchanter_Button") == 0) classID = 14;
				else if (strcmp(screenID, "CC_Class_Beastlord_Button") == 0) classID = 15;
				else if (strcmp(screenID, "CC_Class_Berserker_Button") == 0) classID = 16;
				else if (strcmp(screenID, "CC_Class_Runeknight_Button") == 0) classID = 17;

				// If a valid class button was clicked, handle the logic
				if (classID > 0)
				{
					DebugSpewAlways("CharacterCreationOverride: Class button clicked. Class ID = %d", classID);

					// Perform any logic for the selected class
					HandleClassSelection(classID);
				}
			}

			if (screenID && strcmp(screenID, "CC_Create_Button") == 0)
			{
				DebugSpewAlways("CharacterCreationOverride: CC_Create_Button clicked.");

				// Validate the character name
				if (IsCharacterNameValid(pCharacterCreation))
				{
					DebugSpewAlways("CharacterCreationOverride: Character name is valid. Shutting down plugin.");

					// Call ShutdownPlugin to clean up the plugin
					DebugSpewAlways("MQ2CC::Shutting down");

					// Remove the hook from pCharacterCreation
					if (pCharacterCreation)
					{
						CharacterCreationOverride::RemoveHooks(pCharacterCreation);
						DebugSpewAlways("Unhooked from pCharacterCreation");
					}
				}
				else
				{
					DebugSpewAlways("CharacterCreationOverride: Invalid character name. Plugin shutdown aborted.");
				}
			}
		}

		// Call the base class implementation
		return Super::WndNotification(pWnd, Message, pData);
	}

	void CharacterCreationOverride::HandleClassSelection(int classID)
	{
		// Example logic for handling class selection
		DebugSpewAlways("CharacterCreationOverride::HandleClassSelection called for Class ID = %d", classID);

		// Update the player's class or perform other actions
		/*if (pLocalPC && pLocalPC->GetCurrentPcProfile())
		{
			pLocalPC->GetCurrentPcProfile()->Class = classID;
			DebugSpewAlways("CharacterCreationOverride: Updated player class to %d", classID);
		}*/
	}
};

// Declares
std::string GetClassName(int classID);
void GetCurrentSelection();
void CheckClassScreenButtons(CCharacterCreation* pCharacterCreation);


// Function prototypes for plugin lifecycle
PLUGIN_API void InitializePlugin();
PLUGIN_API void ShutdownPlugin();
PLUGIN_API void OnReloadUI();