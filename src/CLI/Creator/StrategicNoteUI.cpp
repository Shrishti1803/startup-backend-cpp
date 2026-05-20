// StrategicNoteUI.cpp

#include "CLI/Creator/StrategicNoteUI.h"
#include "CLI/UI/TerminalLayout.h"

#include <iostream>
#include <limits>

#include "Models/Creator/StrategicNote.h"

StrategicNoteUI::StrategicNoteUI(
    CreatorService& service,
    ReferencesService& referenceService,
    const Auth::Session& session
)
    : creatorService(service),
      referencesService(referenceService),
      currentSession(session)
{
}

void StrategicNoteUI::handle(
    int creatorId
)
{
    int action;

    do
    {
        clearScreen();

        CreatorAggregate fullData =
            creatorService.getCreatorById(
                currentSession,
                creatorId
            );

        CreatorRenderUI renderer(
            referencesService,
            currentSession
        );

        // =========================================
        // INITIAL SCREEN
        // =========================================

        renderer.displayCompactHeader(
            fullData.creator
        );

        renderer.displayStrategicNotes(
            fullData.creator
        );

        // =========================================
        // ACTIONS
        // =========================================

        UI::title("STRATEGIC NOTE MANAGEMENT");

        std::cout
            << "1. Add\n"
            << "2. Update\n"
            << "3. Delete\n"
            << "4. Back\n";

        action = takeChoice();

        if(action == 4)
        {
            return;
        }

        /* =====================================================
           ADD
        ===================================================== */

        if(action == 1)
        {
            clearScreen();

            renderer.displayStrategicNotes(
                fullData.creator
            );

            UI::title("ADD STRATEGIC NOTE");

            UI::warning(
                "Description is optional."
            );

            CreatorAggregate updateData;

            StrategicNote note;

            std::string input;

            std::cout
                << "Enter Description: ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                note.setDescription(
                    input
                );
            }

            std::cout
                << "Enter Author Name: ";

            std::getline(
                std::cin,
                input
            );

            if(input.empty())
            {
                UI::error(
                    "Author Name cannot be empty."
                );

                pause();

                continue;
            }

            note.setAuthor(
                input
            );

            updateData.strategicNotes
                .push_back(note);

            creatorService.updateCreator(
                currentSession,
                creatorId,
                updateData
            );

            UI::success(
                "Strategic note added successfully."
            );

            pause();

            return;
        }

        /* =====================================================
           UPDATE
        ===================================================== */

        else if(action == 2)
        {
            clearScreen();

            renderer.displayStrategicNotes(
                fullData.creator
            );

            UI::title("UPDATE STRATEGIC NOTE");

            CreatorAggregate updateData;

            StrategicNote note;

            int id;

            std::cout
                << "Enter Strategic Note ID "
                << "(0 to back): ";

            id = takeChoice();

            if(id == 0)
            {
                return;
            }

            note.setId(id);

            clearScreen();

            renderer.displayStrategicNotes(
                fullData.creator
            );

            UI::title("UPDATE NOTE FIELD");

            std::cout
                << "1. Description\n"
                << "2. Author Name\n";

            int fieldChoice =
                takeChoice();

            std::string input;

            switch(fieldChoice)
            {
                // ===== DESCRIPTION =====

                case 1:
                {
                    std::cout
                        << "Enter New Description: ";

                    std::getline(
                        std::cin,
                        input
                    );

                    if(input.empty())
                    {
                        UI::error(
                            "Description cannot be empty."
                        );

                        pause();

                        continue;
                    }

                    note.setDescription(
                        input
                    );

                    break;
                }

                // ===== AUTHOR =====

                case 2:
                {
                    std::cout
                        << "Enter New Author Name: ";

                    std::getline(
                        std::cin,
                        input
                    );

                    if(input.empty())
                    {
                        UI::error(
                            "Author Name cannot be empty."
                        );

                        pause();

                        continue;
                    }

                    note.setAuthor(
                        input
                    );

                    break;
                }

                default:
                {
                    UI::error(
                        "Invalid choice."
                    );

                    pause();

                    continue;
                }
            }

            updateData.strategicNotes
                .push_back(note);

            creatorService.updateCreator(
                currentSession,
                creatorId,
                updateData
            );

            UI::success(
                "Strategic note updated successfully."
            );

            pause();

            return;
        }

        /* =====================================================
           DELETE
        ===================================================== */

        else if(action == 3)
        {
            clearScreen();

            renderer.displayStrategicNotes(
                fullData.creator
            );

            UI::title("DELETE STRATEGIC NOTE");

            CreatorAggregate deleteData;

            StrategicNote note;

            int id;

            std::cout
                << "Enter Strategic Note ID to delete "
                << "(0 to back): ";

            id = takeChoice();

            if(id == 0)
            {
                return;
            }

            note.setId(id);

            deleteData.strategicNotes
                .push_back(note);

            creatorService.deletePartial(
                currentSession,
                creatorId,
                deleteData
            );

            UI::success(
                "Strategic note deleted successfully."
            );

            pause();

            return;
        }

        else
        {
            UI::error(
                "Invalid choice."
            );

            pause();

            continue;
        }

    } while(true);
}