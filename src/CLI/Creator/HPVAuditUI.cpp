#include "CLI/Creator/HPVAuditUI.h"
#include "CLI/UI/TerminalLayout.h"

#include <iostream>
#include <limits>

#include "Models/Creator/HPVAudit.h"

HPVAuditUI::HPVAuditUI(
    CreatorService& service,
    ReferencesService& referenceService,
    const Auth::Session& session
)
    : creatorService(service),
      referencesService(referenceService),
      currentSession(session)
{
}

void HPVAuditUI::handle(
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

        // ===== INITIAL SCREEN =====

        renderer.displayCompactHeader(
            fullData.creator
        );

        renderer.displayHPVAudits(
            fullData.creator
        );

        UI::title("HPV AUDIT MANAGEMENT");

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

        // =====================================================
        // ADD
        // =====================================================

        if(action == 1)
        {
            clearScreen();

            renderer.displayHPVAudits(
                fullData.creator
            );

            UI::title("ADD HPV AUDIT");

            UI::warning(
                "Leave fields blank to skip."
            );

            CreatorAggregate updateData;

            HPVAudit audit;

            std::string input;

            std::cout
                << "Enter Topic: ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                audit.setTopic(
                    input
                );
            }

            std::cout
                << "Enter Spike Rate: ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                audit.setSpikeRate(
                    input
                );
            }

            updateData.hpvAudits
                .push_back(audit);

            creatorService.updateCreator(
                currentSession,
                creatorId,
                updateData
            );

            UI::success(
                "HPV audit added successfully."
            );

            pause();

            return;
        }

        // =====================================================
        // UPDATE
        // =====================================================

        else if(action == 2)
        {
            clearScreen();

            renderer.displayHPVAudits(
                fullData.creator
            );

            UI::title("UPDATE HPV AUDIT");

            CreatorAggregate updateData;

            HPVAudit audit;

            int id;

            std::cout
                << "Enter HPV Audit ID "
                << "(0 to back): ";

            id = takeChoice();

            if(id == 0)
            {
                return;
            }

            audit.setId(id);

            clearScreen();

            renderer.displayHPVAudits(
                fullData.creator
            );

            UI::title("UPDATE HPV AUDIT FIELD");

            std::cout
                << "1. Topic\n"
                << "2. Spike Rate\n";

            int fieldChoice =
                takeChoice();

            std::string input;

            switch(fieldChoice)
            {
                // ===== TOPIC =====

                case 1:
                {
                    std::cout
                        << "Enter New Topic: ";

                    std::getline(
                        std::cin,
                        input
                    );

                    if(input.empty())
                    {
                        UI::error(
                            "Topic cannot be empty."
                        );

                        pause();

                        continue;
                    }

                    audit.setTopic(
                        input
                    );

                    break;
                }

                // ===== SPIKE RATE =====

                case 2:
                {
                    std::cout
                        << "Enter New Spike Rate: ";

                    std::getline(
                        std::cin,
                        input
                    );

                    if(input.empty())
                    {
                        UI::error(
                            "Spike Rate cannot be empty."
                        );

                        pause();

                        continue;
                    }

                    audit.setSpikeRate(
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

            updateData.hpvAudits
                .push_back(audit);

            creatorService.updateCreator(
                currentSession,
                creatorId,
                updateData
            );

            UI::success(
                "HPV audit updated successfully."
            );

            pause();

            return;
        }

        // =====================================================
        // DELETE
        // =====================================================

        else if(action == 3)
        {
            clearScreen();

            renderer.displayHPVAudits(
                fullData.creator
            );

            UI::title("DELETE HPV AUDIT");

            CreatorAggregate deleteData;

            HPVAudit audit;

            int id;

            std::cout
                << "Enter HPV Audit ID to delete "
                << "(0 to back): ";

            id = takeChoice();

            if(id == 0)
            {
                return;
            }

            audit.setId(id);

            deleteData.hpvAudits
                .push_back(audit);

            creatorService.deletePartial(
                currentSession,
                creatorId,
                deleteData
            );

            UI::success(
                "HPV audit deleted successfully."
            );

            pause();

            return;
        }

        else
        {
            UI::error("Invalid choice.");

            pause();

            continue;
        }

    } while(true);
}