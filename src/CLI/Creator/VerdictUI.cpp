// VerdictUI.cpp

#include "CLI/Creator/VerdictUI.h"
#include "CLI/UI/TerminalLayout.h"

#include <iostream>
#include <limits>

#include "Models/Creator/Verdict.h"

VerdictUI::VerdictUI(
    CreatorService& service,
    ReferencesService& referenceService,
    const Auth::Session& session
)
    : creatorService(service),
      referencesService(referenceService),
      currentSession(session)
{
}

void VerdictUI::handle(
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

        renderer.displayVerdict(
            fullData.creator
        );

        UI::title("VERDICT MANAGEMENT");

        std::cout
            << "1. Add / Replace\n"
            << "2. Update Single Field\n"
            << "3. Clear Field\n"
            << "4. Delete Entire Verdict\n"
            << "5. Back\n";

        action = takeChoice();

        if(action == 5)
        {
            return;
        }

        std::string input;

        // =====================================================
        // ADD / REPLACE
        // =====================================================

        if(action == 1)
        {
            clearScreen();

            renderer.displayVerdict(
                fullData.creator
            );

            UI::title("ADD / REPLACE VERDICT");

            UI::warning(
                "Leave fields blank to skip."
            );

            CreatorAggregate updateData;

            Verdict verdict;

            std::cout
                << "Enter Strengths: ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                verdict.setStrengths(
                    input
                );
            }

            std::cout
                << "Enter Weaknesses: ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                verdict.setWeaknesses(
                    input
                );
            }

            std::cout
                << "Enter Growth Potential: ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                verdict.setGrowthPotential(
                    input
                );
            }

            std::cout
                << "Enter Final Verdict: ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                verdict.setFinalVerdict(
                    input
                );
            }

            updateData.verdict = verdict;

            creatorService.updateCreator(
                currentSession,
                creatorId,
                updateData
            );

            UI::success(
                "Verdict updated successfully."
            );

            pause();

            return;
        }

        // =====================================================
        // UPDATE SINGLE FIELD
        // =====================================================

        else if(action == 2)
        {
            clearScreen();

            renderer.displayVerdict(
                fullData.creator
            );

            UI::title("UPDATE VERDICT FIELD");

            CreatorAggregate updateData;

            Verdict verdict;

            std::cout
                << "1. Strengths\n"
                << "2. Weaknesses\n"
                << "3. Growth Potential\n"
                << "4. Final Verdict\n";

            int field =
                takeChoice();

            std::cout
                << "Enter New Value: ";

            std::getline(
                std::cin,
                input
            );

            if(input.empty())
            {
                UI::error(
                    "Field value cannot be empty."
                );

                pause();

                continue;
            }

            if(field == 1)
            {
                verdict.setStrengths(
                    input
                );
            }

            else if(field == 2)
            {
                verdict.setWeaknesses(
                    input
                );
            }

            else if(field == 3)
            {
                verdict.setGrowthPotential(
                    input
                );
            }

            else if(field == 4)
            {
                verdict.setFinalVerdict(
                    input
                );
            }

            else
            {
                UI::error(
                    "Invalid choice."
                );

                pause();

                continue;
            }

            updateData.verdict = verdict;

            creatorService.updateCreator(
                currentSession,
                creatorId,
                updateData
            );

            UI::success(
                "Verdict updated successfully."
            );

            pause();

            return;
        }

        // =====================================================
        // CLEAR FIELD
        // =====================================================

        else if(action == 3)
        {
            clearScreen();

            renderer.displayVerdict(
                fullData.creator
            );

            UI::title("CLEAR VERDICT FIELD");

            CreatorAggregate updateData;

            Verdict verdict;

            std::cout
                << "1. Strengths\n"
                << "2. Weaknesses\n"
                << "3. Growth Potential\n"
                << "4. Final Verdict\n";

            int field =
                takeChoice();

            if(field == 1)
            {
                verdict.setStrengths(
                    std::nullopt
                );

                updateData.clearFields.insert(
                    "verdict.strengths"
                );
            }

            else if(field == 2)
            {
                verdict.setWeaknesses(
                    std::nullopt
                );

                updateData.clearFields.insert(
                    "verdict.weaknesses"
                );
            }

            else if(field == 3)
            {
                verdict.setGrowthPotential(
                    std::nullopt
                );

                updateData.clearFields.insert(
                    "verdict.growth_potential"
                );
            }

            else if(field == 4)
            {
                verdict.setFinalVerdict(
                    std::nullopt
                );

                updateData.clearFields.insert(
                    "verdict.final_verdict"
                );
            }

            else
            {
                UI::error(
                    "Invalid choice."
                );

                pause();

                continue;
            }

            updateData.verdict = verdict;

            creatorService.updateCreator(
                currentSession,
                creatorId,
                updateData
            );

            UI::success(
                "Verdict updated successfully."
            );

            pause();

            return;
        }

        // =====================================================
        // DELETE ENTIRE VERDICT
        // =====================================================

        else if(action == 4)
        {
            clearScreen();

            renderer.displayVerdict(
                fullData.creator
            );

            UI::title("DELETE ENTIRE VERDICT");

            UI::warning(
                "This will remove the full verdict."
            );

            CreatorAggregate deleteData;

            deleteData.verdict = Verdict();

            creatorService.deletePartial(
                currentSession,
                creatorId,
                deleteData
            );

            UI::success(
                "Verdict deleted successfully."
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