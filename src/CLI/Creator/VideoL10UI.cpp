// VideoL10UI.cpp

#include "CLI/Creator/VideoL10UI.h"
#include "CLI/UI/TerminalLayout.h"

#include <iostream>
#include <limits>

#include "Models/Creator/VideoL10.h"

VideoL10UI::VideoL10UI(
    CreatorService& service,
    ReferencesService& referenceService,
    const Auth::Session& session
)
    : creatorService(service),
      referencesService(referenceService),
      currentSession(session)
{
}

void VideoL10UI::handle(
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

        renderer.displayVideos(
            fullData.creator
        );

        // =========================================
        // ACTIONS
        // =========================================

        UI::title("VIDEO PERFORMANCE MANAGEMENT");

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

            renderer.displayVideos(
                fullData.creator
            );

            UI::title("ADD VIDEO PERFORMANCE");

            UI::warning(
                "Leave fields blank to skip."
            );

            CreatorAggregate updateData;

            VideoL10 vid;

            std::string input;

            std::cout
                << "Enter Consistency: ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                vid.setConsistency(
                    input
                );
            }

            std::cout
                << "Enter Avg Views: ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                vid.setAvgViews(
                    input
                );
            }

            std::cout
                << "Enter Avg ER: ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                vid.setAvgER(
                    input
                );
            }

            std::cout
                << "Enter Sharability: ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                vid.setSharability(
                    input
                );
            }

            std::cout
                << "Enter Velocity Trend: ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                vid.setVelocityTrend(
                    input
                );
            }

            updateData.videos
                .push_back(vid);

            creatorService.updateCreator(
                currentSession,
                creatorId,
                updateData
            );

            UI::success(
                "Video performance record added successfully."
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

            renderer.displayVideos(
                fullData.creator
            );

            UI::title("UPDATE VIDEO PERFORMANCE");

            CreatorAggregate updateData;

            VideoL10 vid;

            int id;

            std::cout
                << "Enter Video Record ID "
                << "(0 to back): ";

            id = takeChoice();

            if(id == 0)
            {
                return;
            }

            vid.setId(id);

            clearScreen();

            renderer.displayVideos(
                fullData.creator
            );

            UI::title("UPDATE VIDEO FIELD");

            std::cout
                << "1. Consistency\n"
                << "2. Avg Views\n"
                << "3. Avg ER\n"
                << "4. Sharability\n"
                << "5. Velocity Trend\n";

            int fieldChoice =
                takeChoice();

            std::string input;

            switch(fieldChoice)
            {
                case 1:
                {
                    std::cout
                        << "Enter New Consistency: ";

                    std::getline(
                        std::cin,
                        input
                    );

                    if(input.empty())
                    {
                        UI::error(
                            "Consistency cannot be empty."
                        );

                        pause();

                        continue;
                    }

                    vid.setConsistency(
                        input
                    );

                    break;
                }

                case 2:
                {
                    std::cout
                        << "Enter New Avg Views: ";

                    std::getline(
                        std::cin,
                        input
                    );

                    if(input.empty())
                    {
                        UI::error(
                            "Avg Views cannot be empty."
                        );

                        pause();

                        continue;
                    }

                    vid.setAvgViews(
                        input
                    );

                    break;
                }

                case 3:
                {
                    std::cout
                        << "Enter New Avg ER: ";

                    std::getline(
                        std::cin,
                        input
                    );

                    if(input.empty())
                    {
                        UI::error(
                            "Avg ER cannot be empty."
                        );

                        pause();

                        continue;
                    }

                    vid.setAvgER(
                        input
                    );

                    break;
                }

                case 4:
                {
                    std::cout
                        << "Enter New Sharability: ";

                    std::getline(
                        std::cin,
                        input
                    );

                    if(input.empty())
                    {
                        UI::error(
                            "Sharability cannot be empty."
                        );

                        pause();

                        continue;
                    }

                    vid.setSharability(
                        input
                    );

                    break;
                }

                case 5:
                {
                    std::cout
                        << "Enter New Velocity Trend: ";

                    std::getline(
                        std::cin,
                        input
                    );

                    if(input.empty())
                    {
                        UI::error(
                            "Velocity Trend cannot be empty."
                        );

                        pause();

                        continue;
                    }

                    vid.setVelocityTrend(
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

            updateData.videos
                .push_back(vid);

            creatorService.updateCreator(
                currentSession,
                creatorId,
                updateData
            );

            UI::success(
                "Video performance updated successfully."
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

            renderer.displayVideos(
                fullData.creator
            );

            UI::title("DELETE VIDEO PERFORMANCE");

            CreatorAggregate deleteData;

            VideoL10 vid;

            int id;

            std::cout
                << "Enter Video Record ID to delete "
                << "(0 to back): ";

            id = takeChoice();

            if(id == 0)
            {
                return;
            }

            vid.setId(id);

            deleteData.videos
                .push_back(vid);

            creatorService.deletePartial(
                currentSession,
                creatorId,
                deleteData
            );

            UI::success(
                "Video performance deleted successfully."
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