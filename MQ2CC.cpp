// MQ2CC.cpp : Defines the entry point for the DLL application.
//

// PLUGIN_API is only to be used for callbacks.  All existing callbacks at this time
// are shown below. Remove the ones your plugin does not use.  Always use Initialize
// and Shutdown for setup and cleanup.

#include <mq/Plugin.h>
#include "eqlib/WindowOverride.h"
#include "MQ2CC.h" // Include the header file

PreSetup("MQ2CC");
PLUGIN_VERSION(0.1);

// Implement OnHooked
void CharacterCreationOverride::OnHooked(CharacterCreationOverride* pWnd)
{
	DebugSpewAlways("CharacterCreationOverride::OnHooked called");

	// SYNC the character creation window with the current player profile
	CheckClassScreenButtons(pCharacterCreation);
	// Add any custom logic here
	GetCurrentSelection();
}

// Implement OnAboutToUnhook
void CharacterCreationOverride::OnAboutToUnhook(CharacterCreationOverride* pWnd)
{
	DebugSpewAlways("CharacterCreationOverride::OnAboutToUnhook called");
	// Add any cleanup logic here
}

// PostDraw implementation
int CharacterCreationOverride::PostDraw()
{
	return Super::PostDraw();
}

void CheckClassScreenButtons(CCharacterCreation* pCharacterCreation)
{
	if (!pCharacterCreation)
	{
		DebugSpewAlways("CheckClassScreenButtons: pCharacterCreation is null.");
		return;
	}

	// Retrieve the CC_ClassScreen container
	CXWnd* pClassScreen = pCharacterCreation->GetChildItem("CC_ClassScreen");
	if (!pClassScreen)
	{
		DebugSpewAlways("CheckClassScreenButtons: CC_ClassScreen not found.");
		return;
	}

	// Check for CC_Runeknight_Overlay_Button
	CXWnd* pRuneknightButton = pClassScreen->GetChildItem("CC_Class_Runeknight_Overlay_Button");
	if (pRuneknightButton)
	{
		if (pRuneknightButton->IsVisible())
		{
			DebugSpewAlways("CheckClassScreenButtons: CC_Class_Runeknight_Overlay_Button is visible. Disabling visibility.");
			pRuneknightButton->SetVisible(false);
		}
		else
		{
			DebugSpewAlways("CheckClassScreenButtons: CC_Class_Runeknight_Overlay_Button is already disabled.");
		}
	}
	else
	{
		DebugSpewAlways("CheckClassScreenButtons: CC_Class_Runeknight_Overlay_Button not found.");
	}

	// Store all buttons in an array
	std::vector<CButtonWnd*> classButtons;
	for (CXWnd* pChild = pClassScreen->GetFirstChildWnd(); pChild != nullptr; pChild = pChild->GetNextSiblingWnd())
	{
		// Check if the child has XML data and its ScreenID contains "Button"
		if (pChild->GetXMLData() && pChild->GetXMLData()->ScreenID.find("Button") != std::string::npos)
		{
			// Safely cast to CButtonWnd
			CButtonWnd* pButton = static_cast<CButtonWnd*>(pChild);
			classButtons.push_back(pButton);
		}
	}

	// Check which button has bChecked set to true
	CButtonWnd* pCheckedButton = nullptr;
	for (CButtonWnd* pButton : classButtons)
	{
		if (pButton->Checked) // Access the Checked property
		{
			pCheckedButton = pButton;
			break;
		}
	}

	if (!pCheckedButton)
	{
		DebugSpewAlways("CheckClassScreenButtons: No button is checked.");
		return;
	}

	// Retrieve the Screen ID of the checked button
	std::string screenID = pCheckedButton->GetXMLData()->ScreenID.c_str(); // Access ScreenID via GetXMLData
	DebugSpewAlways("CheckClassScreenButtons: Checked button Screen ID = %s", screenID.c_str());

	// Compare the Screen ID with buttonName results from the switch case
	/*if (pLocalPC && pLocalPC->GetCurrentPcProfile())
	{
		int classID = pLocalPC->GetCurrentPcProfile()->GetClass();
		std::string buttonName;

		switch (classID)
		{
		case 1:  buttonName = "CC_Class_Warrior_Button"; break;
		case 2:  buttonName = "CC_Class_Cleric_Button"; break;
		case 3:  buttonName = "CC_Class_Paladin_Button"; break;
		case 4:  buttonName = "CC_Class_Ranger_Button"; break;
		case 5:  buttonName = "CC_Class_Shadow Knight_Button"; break;
		case 6:  buttonName = "CC_Class_Druid_Button"; break;
		case 7:  buttonName = "CC_Class_Monk_Button"; break;
		case 8:  buttonName = "CC_Class_Bard_Button"; break;
		case 9:  buttonName = "CC_Class_Rogue_Button"; break;
		case 10: buttonName = "CC_Class_Shaman_Button"; break;
		case 11: buttonName = "CC_Class_Necromancer_Button"; break;
		case 12: buttonName = "CC_Class_Wizard_Button"; break;
		case 13: buttonName = "CC_Class_Magician_Button"; break;
		case 14: buttonName = "CC_Class_Enchanter_Button"; break;
		case 15: buttonName = "CC_Class_Beastlord_Button"; break;
		case 16: buttonName = "CC_Class_Berserker_Button"; break;
		case 17: buttonName = "CC_Class_Runeknight_Button"; break;
		default: buttonName = "Unknown"; break;
		}

		if (screenID == buttonName)
		{
			DebugSpewAlways("CheckClassScreenButtons: Screen ID matches buttonName (%s).", buttonName.c_str());
			DebugSpewAlways("CharacterCreationOverride::classID = %s", GetClassName(classID).c_str());
		}
		else
		{
			DebugSpewAlways("CheckClassScreenButtons: Screen ID does not match buttonName (%s).", buttonName.c_str());

			// Map the Screen ID to a class ID
			int newClassID = 0;
			if (screenID == "CC_Class_Warrior_Button") newClassID = 1;
			else if (screenID == "CC_Class_Cleric_Button") newClassID = 2;
			else if (screenID == "CC_Class_Paladin_Button") newClassID = 3;
			else if (screenID == "CC_Class_Ranger_Button") newClassID = 4;
			else if (screenID == "CC_Class_Shadow Knight_Button") newClassID = 5;
			else if (screenID == "CC_Class_Druid_Button") newClassID = 6;
			else if (screenID == "CC_Class_Monk_Button") newClassID = 7;
			else if (screenID == "CC_Class_Bard_Button") newClassID = 8;
			else if (screenID == "CC_Class_Rogue_Button") newClassID = 9;
			else if (screenID == "CC_Class_Shaman_Button") newClassID = 10;
			else if (screenID == "CC_Class_Necromancer_Button") newClassID = 11;
			else if (screenID == "CC_Class_Wizard_Button") newClassID = 12;
			else if (screenID == "CC_Class_Magician_Button") newClassID = 13;
			else if (screenID == "CC_Class_Enchanter_Button") newClassID = 14;
			else if (screenID == "CC_Class_Beastlord_Button") newClassID = 15;
			else if (screenID == "CC_Class_Berserker_Button") newClassID = 16;
			else if (screenID == "CC_Class_Runeknight_Button") newClassID = 17;

			if (newClassID > 0)
			{
				// Update the player's class ID
				pLocalPC->GetCurrentPcProfile()->Class = newClassID;
				DebugSpewAlways("CheckClassScreenButtons: Updated class ID to %d based on Screen ID.", newClassID);
			}
			else
			{
				DebugSpewAlways("CheckClassScreenButtons: No matching class ID for Screen ID = %s", screenID.c_str());
			}
		}
	}*/
}

// Converts classID to class name string
std::string GetClassName(int classID)
{
	switch (classID)
	{
	case 1:  return "Warrior";
	case 2:  return "Cleric";
	case 3:  return "Paladin";
	case 4:  return "Ranger";
	case 5:  return "Shadowknight";
	case 6:  return "Druid";
	case 7:  return "Monk";
	case 8:  return "Bard";
	case 9:  return "Rogue";
	case 10: return "Shaman";
	case 11: return "Necromancer";
	case 12: return "Wizard";
	case 13: return "Magician";
	case 14: return "Enchanter";
	case 15: return "Beastlord";
	case 16: return "Berserker";
	case 17: return "Runeknight";
	default: return "Unknown";
	}
}

void GetPlayerClass()
{
	if (!pLocalPC)
	{
		WriteChatf("GetPlayerClass: pLocalPC is null.");
		return;
	}

	// Retrieve the current player profile
	auto* pProfile = pLocalPC->ProfileManager.GetCurrentProfile();
	if (!pProfile)
	{
		WriteChatf("GetPlayerClass: Failed to retrieve player profile.");
		return;
	}

	// Retrieve the class ID
	int classID = pProfile->GetClass();
	std::string className = GetClassName(classID);

	// Log the class information
	WriteChatf("GetPlayerClass: Class ID = %d, Class Name = %s", classID, className.c_str());
}

void GetCurrentSelection()
{
	if (!pCharacterCreation)
	{
		WriteChatf("GetCurrentSelection: pCharacterCreation is null.");
		return;
	}

	// Retrieve the main container for buttons (e.g., CC_ClassScreen)
	CXWnd* pClassScreen = pCharacterCreation->GetChildItem("CC_ClassScreen");
	if (!pClassScreen)
	{
		WriteChatf("GetCurrentSelection: CC_ClassScreen not found.");
		return;
	}

	WriteChatf("GetCurrentSelection: Iterating through child items in CC_ClassScreen.");

	// Iterate through all child windows of the container
	for (CXWnd* pChild = pClassScreen->GetFirstChildWnd(); pChild != nullptr; pChild = pChild->GetNextSiblingWnd())
	{
		if (pChild->GetXMLData() && pChild->GetXMLData()->ScreenID.find("Button") != std::string::npos)
		{
			bool isChecked = *reinterpret_cast<bool*>(reinterpret_cast<uint8_t*>(pChild) + offsetof(CButtonWnd, Checked));
			if (isChecked)
			{
				const char* screenID = pChild->GetXMLData() ? pChild->GetXMLData()->ScreenID.c_str() : "(Unknown)";
				const char* windowText = pChild->GetTooltip().c_str();

				WriteChatf("GetCurrentSelection: Button Checked - ScreenID: %s, Text: %s",
					screenID ? screenID : "(null)",
					windowText ? windowText : "(null)");
				GetPlayerClass();
			}
		}
		else
		{
			WriteChatf("GetCurrentSelection: Non-button item: %s", pChild->GetXMLName().c_str());
		}
	}
}

// HandleLButtonDown implementation
int CharacterCreationOverride::HandleLButtonDown(const CXPoint& pos, uint32_t flags)
{
	WriteChatf("CharacterCreationOverride::HandleLButtonDown called at position (%d, %d)", pos.x, pos.y);

	// Convert the cursor position to local or world coordinates if needed
	CXPoint localPos = pos; // Adjust this based on your coordinate system

	// Perform logic based on the cursor position
	if (pCharacterCreation)
	{
		CXWnd* pClassScreen = pCharacterCreation->GetChildItem("CC_ClassScreen");
		if (pClassScreen && pClassScreen->GetScreenRect().ContainsPoint(localPos))
		{
			WriteChatf("Cursor is over the CC_ClassScreen.");
			// Add additional logic here
		}
	}

	// Call the base class implementation
	return Super::HandleLButtonDown(pos, flags);
}

/**
 * Avoid Globals if at all possible, since they persist throughout your program.
 * But if you must have them, here is the place to put them.
 */
// bool ShowMQ2CCWindow = true;

/**
 * @fn InitializePlugin
 *
 * This is called once on plugin initialization and can be considered the startup
 * routine for the plugin.
 */
PLUGIN_API void InitializePlugin()
{
	DebugSpewAlways("MQ2CC::Initializing version %s");

	// Check if pCharacterCreation is valid
	if (pCharacterCreation)
	{
		CharacterCreationOverride::InstallHooks(pCharacterCreation);
		DebugSpewAlways("Hooked into pCharacterCreation");
	}
}

/**
 * @fn ShutdownPlugin
 *
 * This is called once when the plugin has been asked to shutdown.  The plugin has
 * not actually shut down until this completes.
 */
PLUGIN_API void ShutdownPlugin()
{
	DebugSpewAlways("MQ2CC::Shutting down");

	// Remove the hook from pCharacterCreation
	if (pCharacterCreation)
	{
		CharacterCreationOverride::RemoveHooks(pCharacterCreation);
		DebugSpewAlways("Unhooked from pCharacterCreation");
	}
}

/**
 * @fn OnCleanUI
 *
 * This is called once just before the shutdown of the UI system and each time the
 * game requests that the UI be cleaned.  Most commonly this happens when a
 * /loadskin command is issued, but it also occurs when reaching the character
 * select screen and when first entering the game.
 *
 * One purpose of this function is to allow you to destroy any custom windows that
 * you have created and cleanup any UI items that need to be removed.
 */
PLUGIN_API void OnCleanUI()
{
	// DebugSpewAlways("MQ2CC::OnCleanUI()");
}

/**
 * @fn OnReloadUI
 *
 * This is called once just after the UI system is loaded. Most commonly this
 * happens when a /loadskin command is issued, but it also occurs when first
 * entering the game.
 *
 * One purpose of this function is to allow you to recreate any custom windows
 * that you have setup.
 */
PLUGIN_API void OnReloadUI()
{
	DebugSpewAlways("MQ2CC::OnReloadUI");

	// Reapply the hook after the UI is reloaded
	if (pCharacterCreation)
	{
		CharacterCreationOverride::InstallHooks(pCharacterCreation);
		DebugSpewAlways("Re-hooked into pCharacterCreation after UI reload");
	}
}

/**
 * @fn OnDrawHUD
 *
 * This is called each time the Heads Up Display (HUD) is drawn.  The HUD is
 * responsible for the net status and packet loss bar.
 *
 * Note that this is not called at all if the HUD is not shown (default F11 to
 * toggle).
 *
 * Because the net status is updated frequently, it is recommended to have a
 * timer or counter at the start of this call to limit the amount of times the
 * code in this section is executed.
 */
PLUGIN_API void OnDrawHUD()
{
/*
	static std::chrono::steady_clock::time_point DrawHUDTimer = std::chrono::steady_clock::now();
	// Run only after timer is up
	if (std::chrono::steady_clock::now() > DrawHUDTimer)
	{
		// Wait half a second before running again
		DrawHUDTimer = std::chrono::steady_clock::now() + std::chrono::milliseconds(500);
		DebugSpewAlways("MQ2CC::OnDrawHUD()");
	}
*/
}

/**
 * @fn SetGameState
 *
 * This is called when the GameState changes.  It is also called once after the
 * plugin is initialized.
 *
 * For a list of known GameState values, see the constants that begin with
 * GAMESTATE_.  The most commonly used of these is GAMESTATE_INGAME.
 *
 * When zoning, this is called once after @ref OnBeginZone @ref OnRemoveSpawn
 * and @ref OnRemoveGroundItem are all done and then called once again after
 * @ref OnEndZone and @ref OnAddSpawn are done but prior to @ref OnAddGroundItem
 * and @ref OnZoned
 *
 * @param GameState int - The value of GameState at the time of the call
 */
PLUGIN_API void SetGameState(int GameState)
{
	// DebugSpewAlways("MQ2CC::SetGameState(%d)", GameState);
}


/**
 * @fn OnPulse
 *
 * This is called each time MQ2 goes through its heartbeat (pulse) function.
 *
 * Because this happens very frequently, it is recommended to have a timer or
 * counter at the start of this call to limit the amount of times the code in
 * this section is executed.
 */
PLUGIN_API void OnPulse()
{
/*
	static std::chrono::steady_clock::time_point PulseTimer = std::chrono::steady_clock::now();
	// Run only after timer is up
	if (std::chrono::steady_clock::now() > PulseTimer)
	{
		// Wait 5 seconds before running again
		PulseTimer = std::chrono::steady_clock::now() + std::chrono::seconds(5);
		DebugSpewAlways("MQ2CC::OnPulse()");
	}
*/
}

/**
 * @fn OnWriteChatColor
 *
 * This is called each time WriteChatColor is called (whether by MQ2Main or by any
 * plugin).  This can be considered the "when outputting text from MQ" callback.
 *
 * This ignores filters on display, so if they are needed either implement them in
 * this section or see @ref OnIncomingChat where filters are already handled.
 *
 * If CEverQuest::dsp_chat is not called, and events are required, they'll need to
 * be implemented here as well.  Otherwise, see @ref OnIncomingChat where that is
 * already handled.
 *
 * For a list of Color values, see the constants for USERCOLOR_.  The default is
 * USERCOLOR_DEFAULT.
 *
 * @param Line const char* - The line that was passed to WriteChatColor
 * @param Color int - The type of chat text this is to be sent as
 * @param Filter int - (default 0)
 */
PLUGIN_API void OnWriteChatColor(const char* Line, int Color, int Filter)
{
	// DebugSpewAlways("MQ2CC::OnWriteChatColor(%s, %d, %d)", Line, Color, Filter);
}

/**
 * @fn OnIncomingChat
 *
 * This is called each time a line of chat is shown.  It occurs after MQ filters
 * and chat events have been handled.  If you need to know when MQ2 has sent chat,
 * consider using @ref OnWriteChatColor instead.
 *
 * For a list of Color values, see the constants for USERCOLOR_. The default is
 * USERCOLOR_DEFAULT.
 *
 * @param Line const char* - The line of text that was shown
 * @param Color int - The type of chat text this was sent as
 *
 * @return bool - Whether to filter this chat from display
 */
PLUGIN_API bool OnIncomingChat(const char* Line, DWORD Color)
{
	// DebugSpewAlways("MQ2CC::OnIncomingChat(%s, %d)", Line, Color);
	return false;
}

/**
 * @fn OnAddSpawn
 *
 * This is called each time a spawn is added to a zone (ie, something spawns). It is
 * also called for each existing spawn when a plugin first initializes.
 *
 * When zoning, this is called for all spawns in the zone after @ref OnEndZone is
 * called and before @ref OnZoned is called.
 *
 * @param pNewSpawn PSPAWNINFO - The spawn that was added
 */
PLUGIN_API void OnAddSpawn(PSPAWNINFO pNewSpawn)
{
	// DebugSpewAlways("MQ2CC::OnAddSpawn(%s)", pNewSpawn->Name);
}

/**
 * @fn OnRemoveSpawn
 *
 * This is called each time a spawn is removed from a zone (ie, something despawns
 * or is killed).  It is NOT called when a plugin shuts down.
 *
 * When zoning, this is called for all spawns in the zone after @ref OnBeginZone is
 * called.
 *
 * @param pSpawn PSPAWNINFO - The spawn that was removed
 */
PLUGIN_API void OnRemoveSpawn(PSPAWNINFO pSpawn)
{
	// DebugSpewAlways("MQ2CC::OnRemoveSpawn(%s)", pSpawn->Name);
}

/**
 * @fn OnAddGroundItem
 *
 * This is called each time a ground item is added to a zone (ie, something spawns).
 * It is also called for each existing ground item when a plugin first initializes.
 *
 * When zoning, this is called for all ground items in the zone after @ref OnEndZone
 * is called and before @ref OnZoned is called.
 *
 * @param pNewGroundItem PGROUNDITEM - The ground item that was added
 */
PLUGIN_API void OnAddGroundItem(PGROUNDITEM pNewGroundItem)
{
	// DebugSpewAlways("MQ2CC::OnAddGroundItem(%d)", pNewGroundItem->DropID);
}

/**
 * @fn OnRemoveGroundItem
 *
 * This is called each time a ground item is removed from a zone (ie, something
 * despawns or is picked up).  It is NOT called when a plugin shuts down.
 *
 * When zoning, this is called for all ground items in the zone after
 * @ref OnBeginZone is called.
 *
 * @param pGroundItem PGROUNDITEM - The ground item that was removed
 */
PLUGIN_API void OnRemoveGroundItem(PGROUNDITEM pGroundItem)
{
	// DebugSpewAlways("MQ2CC::OnRemoveGroundItem(%d)", pGroundItem->DropID);
}

/**
 * @fn OnBeginZone
 *
 * This is called just after entering a zone line and as the loading screen appears.
 */
PLUGIN_API void OnBeginZone()
{
	// DebugSpewAlways("MQ2CC::OnBeginZone()");
}

/**
 * @fn OnEndZone
 *
 * This is called just after the loading screen, but prior to the zone being fully
 * loaded.
 *
 * This should occur before @ref OnAddSpawn and @ref OnAddGroundItem are called. It
 * always occurs before @ref OnZoned is called.
 */
PLUGIN_API void OnEndZone()
{
	// DebugSpewAlways("MQ2CC::OnEndZone()");
}

/**
 * @fn OnZoned
 *
 * This is called after entering a new zone and the zone is considered "loaded."
 *
 * It occurs after @ref OnEndZone @ref OnAddSpawn and @ref OnAddGroundItem have
 * been called.
 */
PLUGIN_API void OnZoned()
{
	// DebugSpewAlways("MQ2CC::OnZoned()");
}

/**
 * @fn OnUpdateImGui
 *
 * This is called each time that the ImGui Overlay is rendered. Use this to render
 * and update plugin specific widgets.
 *
 * Because this happens extremely frequently, it is recommended to move any actual
 * work to a separate call and use this only for updating the display.
 */
PLUGIN_API void OnUpdateImGui()
{
/*
	if (GetGameState() == GAMESTATE_INGAME)
	{
		if (ShowMQ2CCWindow)
		{
			if (ImGui::Begin("MQ2CC", &ShowMQ2CCWindow, ImGuiWindowFlags_MenuBar))
			{
				if (ImGui::BeginMenuBar())
				{
					ImGui::Text("MQ2CC is loaded!");
					ImGui::EndMenuBar();
				}
			}
			ImGui::End();
		}
	}
*/
}

/**
 * @fn OnMacroStart
 *
 * This is called each time a macro starts (ex: /mac somemacro.mac), prior to
 * launching the macro.
 *
 * @param Name const char* - The name of the macro that was launched
 */
PLUGIN_API void OnMacroStart(const char* Name)
{
	// DebugSpewAlways("MQ2CC::OnMacroStart(%s)", Name);
}

/**
 * @fn OnMacroStop
 *
 * This is called each time a macro stops (ex: /endmac), after the macro has ended.
 *
 * @param Name const char* - The name of the macro that was stopped.
 */
PLUGIN_API void OnMacroStop(const char* Name)
{
	// DebugSpewAlways("MQ2CC::OnMacroStop(%s)", Name);
}

/**
 * @fn OnLoadPlugin
 *
 * This is called each time a plugin is loaded (ex: /plugin someplugin), after the
 * plugin has been loaded and any associated -AutoExec.cfg file has been launched.
 * This means it will be executed after the plugin's @ref InitializePlugin callback.
 *
 * This is also called when THIS plugin is loaded, but initialization tasks should
 * still be done in @ref InitializePlugin.
 *
 * @param Name const char* - The name of the plugin that was loaded
 */
PLUGIN_API void OnLoadPlugin(const char* Name)
{
	// DebugSpewAlways("MQ2CC::OnLoadPlugin(%s)", Name);
}

/**
 * @fn OnUnloadPlugin
 *
 * This is called each time a plugin is unloaded (ex: /plugin someplugin unload),
 * just prior to the plugin unloading.  This means it will be executed prior to that
 * plugin's @ref ShutdownPlugin callback.
 *
 * This is also called when THIS plugin is unloaded, but shutdown tasks should still
 * be done in @ref ShutdownPlugin.
 *
 * @param Name const char* - The name of the plugin that is to be unloaded
 */
PLUGIN_API void OnUnloadPlugin(const char* Name)
{
	// DebugSpewAlways("MQ2CC::OnUnloadPlugin(%s)", Name);
}