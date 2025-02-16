// Feb 14 2025
// Unfinished
// 
// gcc -g3 -o catan catan.c -lGLEW -lglfw -lGL -lX11 -lm
//

// Catan Gameplay Instructions
// ===============================================================
// press a control button to initiate various game functions such as passing control to next player, 
// or placing a road on the board if player has enough resource cards available.
// Controls: 'q' to cancel any action
//           'n' to forfeit players' turn for next player in queue
//           'r' to initiate placing a road for the current player
//           't' to initiate placing a town for the current player
//           'c' to initiate placing a city for the current player
//           'd' to initiate buying a dev card for current player
//           'u' to initiate using of a dev card for current player
//           'a' to initiate a trade between two players or with bank


#include "catan.h"
// #include "artificialCatanPlayer.h"


//
// callbacks
//
static void error_callback(int error, const char* description) {

    fprintf(stderr, "%d : %s\n", error, description);

}


static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {

    return;

}


static void mouse_callback(GLFWwindow* window, int button, int action, int mods) {

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {

        int whichPlayer = myCatanBoard.gameTurnStructure[myCatanBoard.turnStructureIndex] - 1;

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        printf("mouse clicked: (x: %f, y: %f)\n", xpos, ypos);

        double buttonX;
        double buttonY;
        bool withinXBoundary;
        bool withinYBoundary;

        int i = 0;
        for (; i < NUM_SETTLEMENT_BUTTONS; ++i) {

            buttonX = TOMOUSECOORDS('x', true, myCatanBoard.boardSettlementButtons[2 * i]);
            buttonY = TOMOUSECOORDS('y', true, myCatanBoard.boardSettlementButtons[2 * i + 1]);
            withinXBoundary = ((int)(double)floor(xpos) >= (int)(double)floor(buttonX) - SMALL_BUTTON_WIDTH) && ((int)(double)floor(xpos) <= (int)(double)floor(buttonX) + SMALL_BUTTON_WIDTH);
            withinYBoundary = ((int)(double)floor(ypos) >= (int)(double)floor(buttonY) - SMALL_BUTTON_HEIGHT) && ((int)(double)floor(ypos) <= (int)(double)floor(buttonY) + SMALL_BUTTON_HEIGHT);

            if (withinXBoundary && withinYBoundary) {

                myCatanBoard.players[whichPlayer].whichSettlementButtonPressed = i;

                printf("settlement button pressed at: x:%f y:%f, button #%d\n", xpos, ypos, i);

                break;

            }

        }

        int j = 0;
        for (; j < NUM_ROAD_BUTTONS; ++j) {

            buttonX = TOMOUSECOORDS('x', true, myCatanBoard.boardRoadButtons[2 * j]);
            buttonY = TOMOUSECOORDS('y', true, myCatanBoard.boardRoadButtons[2 * j + 1]);
            withinXBoundary = ((int)(double)floor(xpos) >= (int)(double)floor(buttonX) - SMALL_BUTTON_WIDTH) && ((int)(double)floor(xpos) <= (int)(double)floor(buttonX) + SMALL_BUTTON_WIDTH);
            withinYBoundary = ((int)(double)floor(ypos) >= (int)(double)floor(buttonY) - SMALL_BUTTON_HEIGHT) && ((int)(double)floor(ypos) <= (int)(double)floor(buttonY) + SMALL_BUTTON_HEIGHT);

            if (withinXBoundary && withinYBoundary) {

                myCatanBoard.players[whichPlayer].whichRoadButtonPressed = j;

                printf("road button pressed at: x:%f y:%f, button #%d\n", xpos, ypos, j);

                break;

            }

        }


        if (i == NUM_SETTLEMENT_BUTTONS) {

            printf("no settlement button pressed\n");

        }
        if (j == NUM_ROAD_BUTTONS) {

            printf("no road button pressed\n");

        }


    }

    return;

}


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (!myCatanBoard.isPregame) {

        bool reset = false;
        int whichPlayer = myCatanBoard.gameTurnStructure[myCatanBoard.turnStructureIndex];

        // next turn
        if (action == GLFW_PRESS && key == GLFW_KEY_N && myCatanBoard.click == false) {

            myCatanBoard.click = true;
            myCatanBoard.players[whichPlayer].gameConds[NEXT_TURN_COND] = true;
            reset = true;

        // place a road
        } else if (action == GLFW_PRESS && key == GLFW_KEY_R && myCatanBoard.click == false) {

            myCatanBoard.click = true;
            myCatanBoard.players[whichPlayer].gameConds[PLACE_ROAD_COND] = true;
            reset = true;

        // press t for town placement
        } else if (action == GLFW_PRESS && key == GLFW_KEY_T && myCatanBoard.click == false) {

            myCatanBoard.click = true;
            myCatanBoard.players[whichPlayer].gameConds[PLACE_TOWN_COND] = true;
            reset = true;

        // place a city
        } else if (action == GLFW_PRESS && key == GLFW_KEY_C && myCatanBoard.click == false) {

            myCatanBoard.click = true;
            myCatanBoard.players[whichPlayer].gameConds[PLACE_CITY_COND] = true;
            reset = true;

        // purchase a developement card
        } else if (action == GLFW_PRESS && key == GLFW_KEY_D && myCatanBoard.click == false) {

            printf("unimplemented\n\n");
            reset = true;

        // use a developement card
        } else if (action == GLFW_PRESS && key == GLFW_KEY_U && myCatanBoard.click == false) {

            printf("unimplemented\n\n");
            reset = true;

        // initiate trade
        } else if (action == GLFW_PRESS && key == GLFW_KEY_A && myCatanBoard.click == false) {

            printf("unimplemented\n\n");
            reset = true;

        }

        if (reset) {

            printf("press 'q' to quit, 'y' to confirm, 'n' to deny action\n\n");
            action = -1;
            key = -1;
            reset = false;

        }

        while ((action != GLFW_PRESS && key != GLFW_KEY_Q) || (action != GLFW_PRESS && key != GLFW_KEY_Y) || (action != GLFW_PRESS && key != GLFW_KEY_N)) {

            glfwPollEvents();

        }

        if (action == GLFW_PRESS && key == GLFW_KEY_Y) {
        
            myCatanBoard.confirmClick = true;

        } else if (action == GLFW_PRESS && key == GLFW_KEY_N) {
        
            myCatanBoard.confirmClick = false;

        } else if (action == GLFW_PRESS && key == GLFW_KEY_Q) {

            return;

        }

        if (myCatanBoard.click && myCatanBoard.confirmClick) {

            for (int i = 0; i < TOTAL_GAMEPLAY_CONDITIONS; ++i) {

                if (myCatanBoard.players[whichPlayer].gameConds[i]) {

                    switch (i) {

                        case NEXT_TURN_COND:    // N

                            ++myCatanBoard.turn;
                            myCatanBoard.turnStructureIndex = myCatanBoard.turn % 3;
                            printf("next turn: %s's turn\n\n", playerColors[whichPlayer]);
                            myCatanBoard.players[whichPlayer].gameConds[i] = false;
                            break;

                        case PLACE_ROAD_COND:   // R

                            if (myCatanBoard.players[whichPlayer].playerRoadsInHand) {    // && hasResourcesInHand(ROAD)

                                printf("press a road button on screen to place\npress q to cancel road placement\n\n");
                                while (action != GLFW_PRESS && key != GLFW_KEY_Q) {
                                    
                                    glfwPollEvents();

                                    if (myCatanBoard.players[whichPlayer].whichRoadButtonPressed != DUMMY_VALUE) {

                                        myCatanBoard.click = true;
                                        myCatanBoard.players[whichPlayer].gameConds[i] = false;
                                        myCatanBoard.players[whichPlayer].playerRoadLocations[myCatanBoard.players[whichPlayer].playerRoadsOnBoard] = myCatanBoard.players[whichPlayer].whichRoadButtonPressed;
                                        --myCatanBoard.players[whichPlayer].playerRoadsInHand;     // use current player's playerPiecesInHand
                                        ++myCatanBoard.players[whichPlayer].playerRoadsOnBoard;
                                        /*

                                        takeRCardsPlayerHand(ROAD);  // use game's resource card deck
                                        placeRCardsGameDeck(ROAD);   // use game's resource card deck

                                        */

                                    }

                                }

                                myCatanBoard.players[whichPlayer].whichRoadButtonPressed = DUMMY_VALUE;
                                myCatanBoard.players[whichPlayer].gameConds[i] = false;
                                printf("road #%d placed for %s player at road button location #%d\n\n", myCatanBoard.players[whichPlayer].playerTownsOnBoard, playerColors[whichPlayer], myCatanBoard.players[whichPlayer].whichRoadButtonPressed);

                            }
                            break;

                        case PLACE_TOWN_COND:   // T
                        
                            if (myCatanBoard.players[whichPlayer].playerTownsInHand) {    // && hasResourcesInHand(TOWN)

                                printf("press a settlement button on screen to place\npress q to cancel town placement\n\n");
                                while (action != GLFW_PRESS && key != GLFW_KEY_Q) {
                                    
                                    glfwPollEvents();

                                    if (myCatanBoard.players[whichPlayer].whichSettlementButtonPressed != DUMMY_VALUE) {

                                        myCatanBoard.click = true;
                                        myCatanBoard.players[whichPlayer].gameConds[i] = false; 
                                        myCatanBoard.players[whichPlayer].playerTownLocations[myCatanBoard.players[whichPlayer].playerTownsOnBoard] = myCatanBoard.players[whichPlayer].whichSettlementButtonPressed;
                                        --myCatanBoard.players[whichPlayer].playerTownsInHand;
                                        ++myCatanBoard.players[whichPlayer].playerTownsOnBoard;
                                        /*

                                        takeRCardsPlayerHand(TOWN);  // use game's resource card deck
                                        placeRCardsGameDeck(TOWN);   // use game's resource card deck

                                        */

                                    }

                                }

                                myCatanBoard.players[whichPlayer].whichSettlementButtonPressed = DUMMY_VALUE;
                                myCatanBoard.players[whichPlayer].gameConds[i] = false;
                                printf("town #%d placed for %s player at settlement button location #%d\n\n", myCatanBoard.players[whichPlayer].playerTownsOnBoard, playerColors[whichPlayer], myCatanBoard.players[whichPlayer].whichSettlementButtonPressed);

                            }
                            break;

                        case PLACE_CITY_COND:   // C

                            if (myCatanBoard.players[whichPlayer].playerCitiesInHand) {    // && hasResourcesInHand(CITY)

                                printf("press a road button on screen to place\npress q to cancel road placement\n\n");
                                while (action != GLFW_PRESS && key != GLFW_KEY_Q) {

                                    glfwPollEvents();

                                    if (myCatanBoard.players[whichPlayer].whichSettlementButtonPressed != DUMMY_VALUE) {

                                        myCatanBoard.click = true;
                                        myCatanBoard.players[whichPlayer].gameConds[i] = false;
                                        myCatanBoard.players[whichPlayer].playerCityLocations[myCatanBoard.players[whichPlayer].playerCitiesOnBoard] = myCatanBoard.players[whichPlayer].whichSettlementButtonPressed;
                                        --myCatanBoard.players[whichPlayer].playerCitiesInHand;
                                        ++myCatanBoard.players[whichPlayer].playerCitiesOnBoard;
                                        --myCatanBoard.players[whichPlayer].playerTownsOnBoard;
                                        ++myCatanBoard.players[whichPlayer].playerTownsInHand;
                                        /*

                                        takeRCardsPlayerHand(CITY);  // use game's resource card deck
                                        placeRCardsGameDeck(CITY);   // use game's resource card deck

                                        */

                                    }

                                }

                                myCatanBoard.players[whichPlayer].whichSettlementButtonPressed = DUMMY_VALUE;
                                myCatanBoard.players[whichPlayer].gameConds[i] = false;
                                printf("road #%d placed for %s player at road button location #%d\n\n", myCatanBoard.players[whichPlayer].playerTownsOnBoard, playerColors[whichPlayer], myCatanBoard.players[whichPlayer].whichSettlementButtonPressed);

                            }
                            break;

                        case BUY_DCARD_COND:  // D

                            // TODO
                            /*
                            if (hasResourcesInHand(GENERIC_DEV_CARD)) {

                                popDev();
                                ++myCatanBoard.players[myCatanBoard.gameTurnStructure[myCatanBoard.turnStructureIndex]].playerDevCardCount;
                                takeRCardsPlayerHand(TOWN);  // use game's resource card deck
                                placeRCardsGameDeck(TOWN);   // use game's resource card deck
                                myCatanBoard.players[myCatanBoard.gameTurnStructure[myCatanBoard.turnStructureIndex]].gameConds[i] = false;
                                printf("%s player bought their #%d development card.\n\n", playerColor[myCatanBoard.players[myCatanBoard.gameTurnStructure[myCatanBoard.turnStructureIndex]]], myCatanBoard.players[myCatanBoard.gameTurnStructure[myCatanBoard.turnStructureIndex]].playerDevCardCount);

                            }
                            */
                            break;

                        case USE_DCARD_COND:   // U

                            // TODO
                            break;

                        case INIT_TRADE_COND:  // A

                            // TODO
                            break;

                        // etc...

                    }

                }

            }

        }

        myCatanBoard.click = false;
        myCatanBoard.confirmClick = false;

    }

    return;

}


//
// roads: is the placed road's location unique to any road currently on the board?
// towns: is the placed town's location unique to any town or city currently on the board?
// cities: is the placed city's location unique to any town or city currently on the board?
//
static bool isLocationUnique(int whichPlayer, enum pieceEnum whichPiece) {

    bool isUnique = true;

    if (whichPiece == ROAD) {

        for (int i = 0; i < 3; ++i) {

            for (int j = 0; j < NUM_ROADS_PER_PLAYER; ++j) {

                if (myCatanBoard.players[i].playerRoadLocations[j] == DUMMY_VALUE) {

                    break;

                }

                isUnique = (myCatanBoard.players[i].playerRoadLocations[j] == myCatanBoard.players[whichPlayer].whichRoadButtonPressed ? false : true);
                if (!isUnique) {

                    return false;

                }

            }

        }

    } else if (whichPiece == TOWN) {

        for (int i = 0; i < 3; ++i) {

            for (int j = 0; j < NUM_TOWNS_PER_PLAYER; ++j) {

                if (myCatanBoard.players[i].playerTownLocations[j] == DUMMY_VALUE) {

                    break;

                }

                isUnique = (myCatanBoard.players[i].playerTownLocations[j] == myCatanBoard.players[whichPlayer].whichSettlementButtonPressed ? false : true);
                if (!isUnique) {

                    return false;

                }

            }

        }

    } else if (whichPiece == CITY) {

        for (int i = 0; i < 3; ++i) {

            for (int j = 0; j < NUM_CITIES_PER_PLAYER; ++j) {

                if (myCatanBoard.players[i].playerCityLocations[j] == DUMMY_VALUE) {

                    break;

                }

                isUnique = (myCatanBoard.players[i].playerCityLocations[j] == myCatanBoard.players[whichPlayer].whichSettlementButtonPressed ? false : true);
                if (!isUnique) {

                    return false;

                }

            }

        }

    }

    return isUnique;

}


//
// is the road adjacent to another road
//
static bool isRoadAdjacentToSettlement(int whichPlayer, enum pieceEnum whichPiece) {

    bool isAdjacentToSettlement = false;

    if (myCatanBoard.players[whichPlayer].whichRoadButtonPressed != DUMMY_VALUE) {

        int *localTownOrCityArray = (whichPiece == TOWN ? myCatanBoard.players[whichPlayer].playerTownLocations : myCatanBoard.players[whichPlayer].playerCityLocations); 

        for (int i = 0; i < NUM_TOWNS_PER_PLAYER; ++i) {

            if (localTownOrCityArray[i] == DUMMY_VALUE) {

                break;

            }

            // check both settlement spots adjacent to the player's road placement against each of player's town spots
            isAdjacentToSettlement = (myCatanBoard.roadToSettlementAdjacencyArray[myCatanBoard.players[whichPlayer].whichRoadButtonPressed][0] == localTownOrCityArray[i] ? true : false);
            if (isAdjacentToSettlement) {

                return isAdjacentToSettlement;

            }
            isAdjacentToSettlement = (myCatanBoard.roadToSettlementAdjacencyArray[myCatanBoard.players[whichPlayer].whichRoadButtonPressed][1] == localTownOrCityArray[i] ? true : false);
            if (isAdjacentToSettlement) {

                return isAdjacentToSettlement;

            }

        }

    }

    return isAdjacentToSettlement;

}


//
// is the road placement adjacent to a road
//
static bool isRoadAdjacentToRoad(int whichPlayer) {

    bool isAdjacentToRoad = false;

    if (myCatanBoard.players[whichPlayer].whichRoadButtonPressed != DUMMY_VALUE) {

        for (int i = 0; i < NUM_ROADS_PER_PLAYER; ++i) {

            // check all four road spots adjacent to the player's road placement against each of player's road spots
            isAdjacentToRoad = (myCatanBoard.roadToRoadAdjacencyArray[myCatanBoard.players[whichPlayer].whichRoadButtonPressed][0] == myCatanBoard.players[whichPlayer].playerRoadLocations[i] ? true : false);
            if (isAdjacentToRoad) {

                return isAdjacentToRoad;

            }
            isAdjacentToRoad = (myCatanBoard.roadToRoadAdjacencyArray[myCatanBoard.players[whichPlayer].whichRoadButtonPressed][1] == myCatanBoard.players[whichPlayer].playerRoadLocations[i] ? true : false);
            if (isAdjacentToRoad) {

                return isAdjacentToRoad;

            }        
            isAdjacentToRoad = (myCatanBoard.roadToRoadAdjacencyArray[myCatanBoard.players[whichPlayer].whichRoadButtonPressed][2] == myCatanBoard.players[whichPlayer].playerRoadLocations[i] ? true : false);
            if (isAdjacentToRoad) {

                return isAdjacentToRoad;

            }
            isAdjacentToRoad = (myCatanBoard.roadToRoadAdjacencyArray[myCatanBoard.players[whichPlayer].whichRoadButtonPressed][3] == myCatanBoard.players[whichPlayer].playerRoadLocations[i] ? true : false);
            if (isAdjacentToRoad) {

                return isAdjacentToRoad;

            }

        }

    }

    return isAdjacentToRoad;

}


//
// is settlement adjacent to any town? is settlement adjacent to any city?
//
static bool isSettlementAdjacentToSettlement(int whichPlayer, enum pieceEnum whichPiece) {

    bool isAdjacentToSettlement = false;

    if (myCatanBoard.players[whichPlayer].whichSettlementButtonPressed != DUMMY_VALUE) {

        int *localTownOrCityArray = NULL;
        int t1 = DUMMY_VALUE;
        int t2 = DUMMY_VALUE;
        int t3 = DUMMY_VALUE;

        // check each player's towns or cities array
        for (int i = 0; i < 3; ++i) {

            localTownOrCityArray = (whichPiece == TOWN ? myCatanBoard.players[i].playerTownLocations : myCatanBoard.players[i].playerCityLocations);

            // check a player's towns or cities array
            for (int k = 0; k < (whichPiece == TOWN ? NUM_TOWNS_PER_PLAYER : NUM_CITIES_PER_PLAYER); ++k) {

                if (localTownOrCityArray[k] == DUMMY_VALUE) {

                    break;

                }

                t1 = myCatanBoard.settlementToSettlementAdjacencyArray[localTownOrCityArray[k]][0];
                t2 = myCatanBoard.settlementToSettlementAdjacencyArray[localTownOrCityArray[k]][1];
                t3 = myCatanBoard.settlementToSettlementAdjacencyArray[localTownOrCityArray[k]][2];

                isAdjacentToSettlement = (t1 == myCatanBoard.players[whichPlayer].whichSettlementButtonPressed ? true : false);
                if (isAdjacentToSettlement) {

                    return isAdjacentToSettlement;

                }
                isAdjacentToSettlement = (t2 == myCatanBoard.players[whichPlayer].whichSettlementButtonPressed ? true : false);
                if (isAdjacentToSettlement) {

                    return isAdjacentToSettlement;

                }
                isAdjacentToSettlement = (t3 == myCatanBoard.players[whichPlayer].whichSettlementButtonPressed ? true : false);
                if (isAdjacentToSettlement) {

                    return isAdjacentToSettlement;

                }

            }

        }

    }

    return isAdjacentToSettlement;

}


//
// 
//
static bool processPreGamePlayerActions(GLFWwindow* window) {

    int whichPlayer = myCatanBoard.gameTurnStructure[myCatanBoard.turnStructureIndex] - 1;
    bool ret = true;
    bool unique = false;
    bool adjacent = false;

    if (myCatanBoard.players[whichPlayer].gameConds[PLACE_ROAD_COND]) {

        while (myCatanBoard.players[whichPlayer].gameConds[PLACE_ROAD_COND]) {

            glfwPollEvents();

            if (glfwWindowShouldClose(window)) {

                ret = false;
                break;

            }

            if (myCatanBoard.players[whichPlayer].whichRoadButtonPressed != DUMMY_VALUE) {

                unique = isLocationUnique(whichPlayer, ROAD);
                adjacent = isRoadAdjacentToSettlement(whichPlayer, TOWN);

                if (unique && adjacent) {

                    myCatanBoard.players[whichPlayer].playerRoadLocations[myCatanBoard.players[whichPlayer].playerRoadsOnBoard] = myCatanBoard.players[whichPlayer].whichRoadButtonPressed;
                    myCatanBoard.players[whichPlayer].gameConds[PLACE_ROAD_COND] = false;
                    ++myCatanBoard.players[whichPlayer].playerRoadsOnBoard;
                    --myCatanBoard.players[whichPlayer].playerRoadsInHand;
                    break;

                }

            }

        }

    } else if (myCatanBoard.players[whichPlayer].gameConds[PLACE_TOWN_COND]) {

        while (myCatanBoard.players[whichPlayer].gameConds[PLACE_TOWN_COND]) {

            glfwPollEvents();

            if (glfwWindowShouldClose(window)) {

                ret = false;
                break;

            }

            if (myCatanBoard.players[whichPlayer].whichSettlementButtonPressed != DUMMY_VALUE) {

                unique = isLocationUnique(whichPlayer, TOWN);
                adjacent = isSettlementAdjacentToSettlement(whichPlayer, TOWN);

                if (unique && !adjacent) {

                    myCatanBoard.players[whichPlayer].playerTownLocations[myCatanBoard.players[whichPlayer].playerTownsOnBoard] = myCatanBoard.players[whichPlayer].whichSettlementButtonPressed;
                    myCatanBoard.players[whichPlayer].gameConds[PLACE_TOWN_COND] = false;
                    ++myCatanBoard.players[whichPlayer].playerTownsOnBoard;
                    --myCatanBoard.players[whichPlayer].playerTownsInHand;
                    break;

                }

            }

        }

    }

    return ret;

}


//
// One hexagon
//
static void setHexagon(GLfloat* verts) {

    int cnt = 0;
    for (int i = 0; i < HEXAGON_SIDES; ++i) {

        switch (i) {

            case 5:

                verts[cnt++]        = (cos(TORADIANS((60 * i + 30) % 360)) * HEXAGON_EDGE_RADIUS);
                verts[cnt++]        = (sin(TORADIANS((60 * i + 30) % 360)) * HEXAGON_EDGE_RADIUS);
                verts[cnt++]        = verts[0];
                verts[cnt++]        = verts[1];
                break;

            default:

                verts[cnt++]        = (cos(TORADIANS((60 * i + 30) % 360)) * HEXAGON_EDGE_RADIUS);
                verts[cnt++]        = (sin(TORADIANS((60 * i + 30) % 360)) * HEXAGON_EDGE_RADIUS);
                verts[cnt++]        = (cos(TORADIANS((60 * i + 90) % 360)) * HEXAGON_EDGE_RADIUS);
                verts[cnt++]        = (sin(TORADIANS((60 * i + 90) % 360)) * HEXAGON_EDGE_RADIUS);
                break;

        }
    }

    return;

}


//
// board geometry
//
static int setBoardVertices() {

    GLfloat prevHexVertices[FLOATS_PER_HEXAGON];
    GLfloat xOffset = 0.0f;
    GLfloat yOffset = 0.0f;
    GLuint countOfVertices = 0;
    GLuint cnt;

    // get pixel x and y coord
    setHexagon(prevHexVertices);

    for (int i = 0; i < HEXAGONS_TO_DRAW; ++i) {

        // ignore setting middle hexagon offsets
        if (i > 0 && i < VERTICES_PER_HEXAGON) {

            // (HEXAGON_SIDE_RADIUS * 2) because each inner ring hexagon is 150 pixels away from center of middle hexagon
            xOffset = cos(TORADIANS((i * 60 - 60) % 360)) * HEXAGON_SIDE_RADIUS * 2;
            yOffset = sin(TORADIANS((i * 60 - 60) % 360)) * HEXAGON_SIDE_RADIUS * 2;

        // 0-6 inner, 7-18 outter
        } else if (i >= VERTICES_PER_HEXAGON && i < HEXAGONS_TO_DRAW && (i % 2 == 1)) {

            // 3 edge radius lengths is radius of even outer ring circle
            xOffset = (cos(TORADIANS((i * 30 - 210) % 360))) * HEXAGON_SIDE_RADIUS * 4;
            yOffset = (sin(TORADIANS((i * 30 - 210) % 360))) * HEXAGON_SIDE_RADIUS * 4;

        } else if (i >= VERTICES_PER_HEXAGON && i < HEXAGONS_TO_DRAW && (i % 2 == 0)) {

            // 4 side radius lengths is radius of odd outer ring circle
            xOffset = (cos(TORADIANS((i * 30 - 210) % 360))) * HEXAGON_EDGE_RADIUS * 3;
            yOffset = (sin(TORADIANS((i * 30 - 210) % 360))) * HEXAGON_EDGE_RADIUS * 3;

        }

        // offset everything in prevHexVertices
        int j = 0;
        for (; j < HEXAGON_SIDES; ++j) {
        
            myCatanBoard.vertices[countOfVertices + (j * 4)]         = (prevHexVertices[(j * 4)] + xOffset) / WIDTH;
            myCatanBoard.vertices[countOfVertices + (j * 4) + 1]     = (prevHexVertices[(j * 4) + 1] + yOffset) / HEIGHT;
            myCatanBoard.vertices[countOfVertices + (j * 4) + 2]     = (prevHexVertices[(j * 4) + 2] + xOffset) / WIDTH;
            myCatanBoard.vertices[countOfVertices + (j * 4) + 3]     = (prevHexVertices[(j * 4) + 3] + yOffset) / HEIGHT;

        }

        countOfVertices += (HEXAGON_SIDES * 4); // 24

    }
    
    return countOfVertices;

}


//
// settlement (town/city) buttons
// 
static void setSettlementButtons() {

    int skipStop = 0;
    int k = 0, p = 0, q = 0;
    bool cond1 = false, cond2 = false, cond3 = false;
    int i = 0;

    for (int j = 0; j < NUM_SETTLEMENT_BUTTONS; j += SETTLEMENT_BUTTONS_WRITE[i++]) {

        skipStop = (i == 0 ? 0 : (HEXAGON_SIDES - SETTLEMENT_BUTTONS_WRITE[i] + SETTLEMENT_BUTTONS_LOC[i]) % HEXAGON_SIDES);

        for (k = 0, p = 0; (k + p) < HEXAGON_SIDES; ) {

            cond1 = (SETTLEMENT_BUTTONS_LOC[i] < skipStop ? true : false);
            cond2 = ((k + p) >= SETTLEMENT_BUTTONS_LOC[i] ? true : false);
            cond3 = ((k + p) < skipStop ? true : false);

            if ((!cond1 && (cond2 || cond3)) || (cond1 && (cond2 && cond3))) {

                ++k;
                continue;

            }

            myCatanBoard.boardSettlementButtons[2 * (j + p)]         = TOPIXELS('w', myCatanBoard.vertices[4 * (q + k + p)]);           // x
            myCatanBoard.boardSettlementButtons[2 * (j + p) + 1]     = TOPIXELS('h', myCatanBoard.vertices[4 * (q + k + p) + 1]);       // y

            ++p;

        }

        q += HEXAGON_SIDES;

    }

    return;

}


//
// roads buttons
// 
static void setRoadButtons() {

    int skipStop = 0;
    int skipCount = 0, writeCount = 0, roadCnt = 0, vertCnt = 0;
    bool cond1 = false, cond2 = false, cond3 = false;
    int i = 0;
    GLfloat X1 = 0.0f, X2 = 0.0f, Y1 = 0.0f, Y2 = 0.0f;

    for (int j = 0; j < NUM_ROAD_BUTTONS; j += ROADS_BUTTONS_WRITE[i++]) {

        skipStop = (i == 0 ? 0 : (HEXAGON_SIDES - ROADS_BUTTONS_WRITE[i] + ROADS_BUTTONS_LOC[i]) % HEXAGON_SIDES);

        skipCount = 0, writeCount = 0;

        for (; (skipCount + writeCount) < HEXAGON_SIDES; ) {

            cond1 = (ROADS_BUTTONS_LOC[i] < skipStop ? true : false);                   // does hexagon sides to skip start late in hexagon and end early in hexagon or not?
            cond2 = ((skipCount + writeCount) >= ROADS_BUTTONS_LOC[i] ? true : false);  // started skipping
            cond3 = ((skipCount + writeCount) < skipStop ? true : false);               // still skipping

            if ((!cond1 && (cond2 || cond3)) || (cond1 && (cond2 && cond3))) {

                vertCnt += 4;
                skipCount += 1;
                continue;

            }

            X1 = TOPIXELS('w', myCatanBoard.vertices[vertCnt]);
            ++vertCnt;
            X2 = TOPIXELS('w', myCatanBoard.vertices[vertCnt + 1]);

            Y1 = TOPIXELS('h', myCatanBoard.vertices[vertCnt]);
            ++vertCnt;
            Y2 = TOPIXELS('h', myCatanBoard.vertices[vertCnt + 1]);

            myCatanBoard.boardRoadButtons[roadCnt++]         = (X1 + X2) / 2;        // x
            myCatanBoard.boardRoadButtons[roadCnt++]         = (Y1 + Y2) / 2;        // y

            vertCnt += 2;

            ++writeCount;

        }

    }

    return;

}


//
// dev cards setup -- dev cards are a stack
//
static void setupDevCards() {

    for (int i = 0; i < 14; ++i) {

        myCatanBoard.myDevCardDeck[i]      = KNIGHTS;

    }

    for (int i = 0; i < 5; ++i) {

        myCatanBoard.myDevCardDeck[14 + i] = VPS;

    }

    myCatanBoard.myDevCardDeck[14 + 5]     = ROADS;
    myCatanBoard.myDevCardDeck[14 + 5 + 1] = ROADS;
    myCatanBoard.myDevCardDeck[14 + 5 + 2] = PLENTY;
    myCatanBoard.myDevCardDeck[14 + 5 + 3] = PLENTY;
    myCatanBoard.myDevCardDeck[14 + 5 + 4] = MONOPOLY;
    myCatanBoard.myDevCardDeck[14 + 5 + 5] = MONOPOLY;
    
    // shuffle just the dCard positions in the array
    enum dCardEnum tmp;
    int index;
    srand(time(NULL));
    for (int i = 0; i < 200; ++i) {

        index = rand() % 25;
        tmp = myCatanBoard.myDevCardDeck[i % 25];
        myCatanBoard.myDevCardDeck[i % 25] = myCatanBoard.myDevCardDeck[index];
        myCatanBoard.myDevCardDeck[index] = tmp;

    }

    return;

}


//
// resource cards setup
// 0 - 18 BRICK, 19 - 37 WOOD, 20 - 56 SHEEP, 21 - 75 WHEAT, 76 - 94 ORE
//
static void setupResourceCards() {

    for (int i = 0; i < 5; ++i) {

        for (int j = 0; j < 19; ++j) {

            switch (i) {

                case 0:

                    myCatanBoard.myResourceCardDeck[(i * 19) + j]    = BRICK;
                    break; 

                case 1:

                    myCatanBoard.myResourceCardDeck[(i * 19) + j]    = WOOD;
                    break;

                case 2:

                    myCatanBoard.myResourceCardDeck[(i * 19) + j]    = SHEEP;
                    break;

                case 3: 

                    myCatanBoard.myResourceCardDeck[(i * 19) + j]    = WHEAT;
                    break;

                case 4:

                    myCatanBoard.myResourceCardDeck[(i * 19) + j]    = ORE;
                    break;

            }

        }

    }

    // no shuffle

    return;

}


//
// popDev for obtaining dev cards from bank
//
static void popDev(enum dCardEnum* ddeck, enum dCardEnum dcard) {

    int indexToRemove = 0;
    for (int i = 0; i < 25; ++i) {

        if (ddeck[i] == GENERIC_DEV_CARD) {

            dcard = ddeck[i - 1];
            indexToRemove = i - 1;

        } else if (ddeck[i] != GENERIC_DEV_CARD) {

            dcard = ddeck[i];
            indexToRemove = i;

        }

    }

    ddeck[indexToRemove] = GENERIC_DEV_CARD;

    return;

}


//
// popRes for obtaining resource cards from bank
//
static void popRes(enum rCardEnum* rdeck, enum rCardEnum rcard) {

    int indexToRemove = 0;
    int startIndex = (rcard == BRICK ? 0 : (rcard == WOOD ? 19 : (rcard == SHEEP ? 38 : (rcard == WHEAT ? 57 : (rcard == ORE ? 76 : 100)))));
    
    if (startIndex == 100) {

        return;

    }

    for (int i = 0; i < 19; ++i) {

        if (rdeck[i] == RES_NONE) {

            rcard = rdeck[i - 1];
            indexToRemove = i - 1;

        } else if (rdeck[i] != RES_NONE) {

            rcard = rdeck[i];
            indexToRemove = i;

        }

    }

    rdeck[indexToRemove] = RES_NONE;

    return;

}


//
// push resource card
//
static void pushRes(enum rCardEnum* rdeck, enum rCardEnum rcard) {

    int startIndex = (rcard == BRICK ? 0 : (rcard == WOOD ? 19 : (rcard == SHEEP ? 38 : (rcard == WHEAT ? 57 : (rcard == ORE ? 76 : 100)))));
    for (int i = 0; i < 19; ++i) {

        if (rdeck[startIndex + i] == RES_NONE) {

            rdeck[startIndex + i] = rcard;

        }

    }

    return;

}


//
// setup 3 players
//
static void setupPlayers() {

    myCatanBoard.players[0].id = PLAYERONE;
    myCatanBoard.players[1].id = PLAYERTWO;
    myCatanBoard.players[2].id = PLAYERTHREE;

    myCatanBoard.players[0].currentRoll = 0;
    myCatanBoard.players[1].currentRoll = 0;
    myCatanBoard.players[2].currentRoll = 0;

    myCatanBoard.players[0].playerTurnStructure = 0;
    myCatanBoard.players[1].playerTurnStructure = 0;
    myCatanBoard.players[2].playerTurnStructure = 0;

    myCatanBoard.turnStructureIndex = 0;

    for (int i = 0; i < 3; ++i) {

        myCatanBoard.gameTurnStructure[i] = 0;

    }

    for (int i = 0; i < TOTAL_DEV_CARD_COUNT; ++i) {

        myCatanBoard.players[0].playerDevCards[i] = GENERIC_DEV_CARD;
        myCatanBoard.players[1].playerDevCards[i] = GENERIC_DEV_CARD;
        myCatanBoard.players[2].playerDevCards[i] = GENERIC_DEV_CARD;

    }

    for (int i = 0; i < TOTAL_RESOURCE_CARD_COUNT; ++i) {

        myCatanBoard.players[0].playerResourceCards[i] = RES_NONE;
        myCatanBoard.players[1].playerResourceCards[i] = RES_NONE;
        myCatanBoard.players[2].playerResourceCards[i] = RES_NONE;

    }

    for (int i = 0; i < TOTAL_GAMEPLAY_CONDITIONS; ++i) {

        myCatanBoard.players[0].gameConds[i] = false;
        myCatanBoard.players[1].gameConds[i] = false;
        myCatanBoard.players[2].gameConds[i] = false;

    }

    myCatanBoard.players[0].whichRoadButtonPressed = DUMMY_VALUE;
    myCatanBoard.players[1].whichRoadButtonPressed = DUMMY_VALUE;
    myCatanBoard.players[2].whichRoadButtonPressed = DUMMY_VALUE;

    myCatanBoard.players[0].whichSettlementButtonPressed = DUMMY_VALUE;
    myCatanBoard.players[1].whichSettlementButtonPressed = DUMMY_VALUE;
    myCatanBoard.players[2].whichSettlementButtonPressed = DUMMY_VALUE;

    for (int i = 0; i < 3; ++i) {

        myCatanBoard.players[i].playerTownsOnBoard = 0;
        myCatanBoard.players[i].playerCitiesOnBoard = 0;
        myCatanBoard.players[i].playerRoadsOnBoard = 0;

        myCatanBoard.players[i].playerRoadsInHand = NUM_ROADS_PER_PLAYER;
        myCatanBoard.players[i].playerTownsInHand = NUM_TOWNS_PER_PLAYER;
        myCatanBoard.players[i].playerCitiesInHand = NUM_CITIES_PER_PLAYER;
        myCatanBoard.players[i].playerDevCardCount = 0;

        for (int j = 0; j < TOTAL_GAMEPLAY_CONDITIONS; ++j) {

            myCatanBoard.players[i].gameConds[j] = 0;

        }

        for (int j = 0; j < NUM_ROADS_PER_PLAYER; ++j) {

            myCatanBoard.players[i].playerRoadLocations[j] = DUMMY_VALUE;

        }

        for (int j = 0; j < NUM_TOWNS_PER_PLAYER; ++j) {

            myCatanBoard.players[i].playerTownLocations[j] = DUMMY_VALUE;            

        }

        for (int j = 0; j < NUM_CITIES_PER_PLAYER; ++j) {

            myCatanBoard.players[i].playerCityLocations[j] = DUMMY_VALUE;

        }

    }

    // 72 road buttons total (72 actual) 
    int localRoadToSettlementAdjacencyArray[NUM_ROAD_BUTTONS][2] = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 0}, {6, 7}, {7, 0}, {5, 8}, {8, 9}, {9, 6}, {10, 11}, {11, 12}, {12, 1}, {7, 10}, {12, 13}, {13, 14}, {14, 15}, {15, 2},    \
    {15, 16}, {16, 17}, {17, 18}, {18, 3}, {18, 19}, {19, 20}, {20, 21}, {21, 4}, {21, 22}, {22, 23}, {23, 8}, {24, 25}, {25, 6}, {9, 26}, {26, 27}, {27, 24}, {28, 29}, {29, 10}, {25, 28},                   \
    {30, 31}, {31, 32}, {32, 11}, {29, 30}, {32, 33}, {33, 34}, {34, 13}, {34, 35}, {35, 36}, {36, 37}, {37, 14}, {37, 38}, {38, 39}, {39, 16}, {39, 40}, {40, 41}, {41, 42}, {42, 17},                        \
    {42, 43}, {43, 44}, {44, 19}, {44, 45}, {45, 46}, {46, 47}, {47, 20}, {47, 48}, {48, 49}, {49, 22}, {50, 23}, {49, 51}, {51, 52}, {52, 50}, {50, 53}, {53, 26}};

    for (int i = 0; i < NUM_ROAD_BUTTONS; ++i) {

        myCatanBoard.roadToSettlementAdjacencyArray[i][0] = localRoadToSettlementAdjacencyArray[i][0];
        myCatanBoard.roadToSettlementAdjacencyArray[i][1] = localRoadToSettlementAdjacencyArray[i][1];        

    }

    // 54 buttons total (53 actual FIX ME)
    int localSettlementtoSettlementAdjacencyArray[NUM_SETTLEMENT_BUTTONS][3] = {{7, 1, 5}, {12, 2, 0}, {15, 3, 1}, {18, 4, 2}, {21, 5, 3}, {8, 0, 4}, {25, 7, 9}, {10, 0, 6}, {23, 9, 5}, {26, 6, 8}, {29, 11, 7}, {32, 12, 10},          \
    {13, 1, 11}, {34, 14, 12}, {37, 15, 13}, {16, 2, 14}, {39, 17, 15}, {42, 18, 16}, {19, 3, 17}, {44, 20, 18}, {47, 21, 19}, {22, 4, 20}, {49, 23, 21}, {50, 8, 22}, {DUMMY_VALUE, 25, 27},           \
    {28, 6, 24}, {53, 27, 9}, {DUMMY_VALUE, 24, 26}, {DUMMY_VALUE, 29, 25}, {30, 10, 28}, {DUMMY_VALUE, 31, 29}, {DUMMY_VALUE, 32, 30}, {33, 11, 31}, {DUMMY_VALUE, 34, 32}, {DUMMY_VALUE, 36, 34},     \
    {DUMMY_VALUE, 37, 35}, {36, 38, 14}, {DUMMY_VALUE, 39, 37}, {38, 40, 16}, {DUMMY_VALUE, 41, 39}, {DUMMY_VALUE, 42, 40}, {43, 17, 41}, {DUMMY_VALUE, 44, 42}, {45, 19, 43}, {DUMMY_VALUE, 46, 44},   \
    {DUMMY_VALUE, 47, 45}, {48, 20, 46}, {DUMMY_VALUE, 49, 47}, {51, 22, 48}, {DUMMY_VALUE, 52, 49}, {DUMMY_VALUE, 50, 51}, {53, 23, 52}, {DUMMY_VALUE, 26, 50}};

    for (int i = 0; i < NUM_SETTLEMENT_BUTTONS; ++i) {

        myCatanBoard.settlementToSettlementAdjacencyArray[i][0] = localSettlementtoSettlementAdjacencyArray[i][0];
        myCatanBoard.settlementToSettlementAdjacencyArray[i][1] = localSettlementtoSettlementAdjacencyArray[i][1];
        myCatanBoard.settlementToSettlementAdjacencyArray[i][2] = localSettlementtoSettlementAdjacencyArray[i][2];

    }

    // looks right
    int localRoadToRoadAdjacencyArray[NUM_ROAD_BUTTONS][4] = {{13, 1, 5, 7}, {18, 2, 0, 13}, {18, 22, 3, 1}, {2, 22, 26, 4}, {3, 26, 8, 5}, {0, 4, 8, 7}, {14, 10, 7, 31}, {0, 5, 6, 14},                   \
    {5, 4, 29, 9}, {8, 29, 32, 10}, {6, 9, 32, 31}, {40, 12, 14, 36}, {15, 13, 11, 40}, {15, 1, 0, 12}, {11, 7, 6, 36}, {16, 13, 12, 44}, {48, 17, 15, 44}, {48, 19, 18, 16}, {17, 19, 2, 1},               \
    {17, 51, 20, 18}, {51, 55, 21, 19}, {20, 55, 23, 22}, {21, 23, 3, 2}, {21, 58, 24, 22}, {23, 58, 62, 25}, {24, 62, 27, 26}, {3, 25, 27, 4}, {26, 25, 65, 28}, {27, 65, 66, 29},                         \
    {8, 28, 66, 9}, {37, 31, 34, DUMMY_VALUE}, {6, 10, 30, 37}, {10, 9, 71, 33}, {32, 71, DUMMY_VALUE, 34}, {30, 33, DUMMY_VALUE, DUMMY_VALUE}, {41, 36, 37, DUMMY_VALUE}, {11, 14, 35, 41},                \
    {35, 31, DUMMY_VALUE, DUMMY_VALUE}, {39, 41, DUMMY_VALUE, DUMMY_VALUE}, {42, 40, 38, DUMMY_VALUE}, {42, 12, 11, 39}, {38, 36, 35, DUMMY_VALUE}, {DUMMY_VALUE, 43, 40, 39}, {45, 44, 42, DUMMY_VALUE},   \
    {45, 16, 15, 43}, {DUMMY_VALUE, 46, 44, 43}, {DUMMY_VALUE, 47, 45, DUMMY_VALUE}, {DUMMY_VALUE, 49, 48, 46}, {47, 49, 17, 16}, {DUMMY_VALUE, 50, 48, 47}, {DUMMY_VALUE, 52, 51, 49},                     \
    {50, 52, 20, 19}, {DUMMY_VALUE, 53, 51, 50}, {DUMMY_VALUE, DUMMY_VALUE, 54, 52}, {53, DUMMY_VALUE, 56, 55}, {54, 56, 21, 20}, {54, DUMMY_VALUE, 57, 55}, {56, DUMMY_VALUE, 59, 58},                     \
    {57, 59, 24, 23}, {57, DUMMY_VALUE, 60, 58}, {59, DUMMY_VALUE, DUMMY_VALUE, 61}, {60, DUMMY_VALUE, 63, 62}, {24, 61, 63, 25}, {62, 61, DUMMY_VALUE, 64}, {63, DUMMY_VALUE, 67, 65},                     \
    {27, 64, 67, 28}, {29, 28, 69, 70}, {65, 64, DUMMY_VALUE, 68}, {67, DUMMY_VALUE, DUMMY_VALUE, 69}, {66, 68, DUMMY_VALUE, 70}, {66, 69, DUMMY_VALUE, 71}, {32, 70, DUMMY_VALUE, 33}};

    for (int i = 0; i < NUM_SETTLEMENT_BUTTONS; ++i) {

        myCatanBoard.roadToRoadAdjacencyArray[i][0] = localRoadToRoadAdjacencyArray[i][0];
        myCatanBoard.roadToRoadAdjacencyArray[i][1] = localRoadToRoadAdjacencyArray[i][1];
        myCatanBoard.roadToRoadAdjacencyArray[i][2] = localRoadToRoadAdjacencyArray[i][2];
        myCatanBoard.roadToRoadAdjacencyArray[i][3] = localRoadToRoadAdjacencyArray[i][3];

    }

    myCatanBoard.isPregame = true;

    myCatanBoard.click = false;
    myCatanBoard.confirmClick = false;

    return;

}

//
//
//
static int setBoardHexagons() {

    GLfloat offsetx = 0.0f;
    GLfloat offsety = 0.0f;
    
    int i = 0;
    int p = 0;
    int q = 0;
    int hexFanIndex = i * HEXAGON_SIDES * 3 * 2;
    int verticesIndex = i * 24;

    // set the hexFan array with fan values for drawing colors for the hexagons
    for (; i < HEXAGONS_TO_DRAW; ++i) {

        // ignore setting middle hexagon offsets
        if (i > 0 && i < VERTICES_PER_HEXAGON) {

            // (HEXAGON_SIDE_RADIUS * 2) because each inner ring hexagon is 150 pixels away from center of middle hexagon
            offsetx = cos(TORADIANS((i * 60 - 60) % 360)) * HEXAGON_SIDE_RADIUS * 2;
            offsety = sin(TORADIANS((i * 60 - 60) % 360)) * HEXAGON_SIDE_RADIUS * 2;

        // 0-6 inner, 7-18 outter
        } else if (i >= VERTICES_PER_HEXAGON && i < HEXAGONS_TO_DRAW && (i % 2 == 1)) {

            // 3 edge radius lengths is radius of even outer ring circle
            offsetx = (cos(TORADIANS((i * 30 - 210) % 360))) * HEXAGON_SIDE_RADIUS * 4;
            offsety = (sin(TORADIANS((i * 30 - 210) % 360))) * HEXAGON_SIDE_RADIUS * 4;

        } else if (i >= VERTICES_PER_HEXAGON && i < HEXAGONS_TO_DRAW && (i % 2 == 0)) {

            // 4 side radius lengths is radius of odd outer ring circle
            offsetx = (cos(TORADIANS((i * 30 - 210) % 360))) * HEXAGON_EDGE_RADIUS * 3;
            offsety = (sin(TORADIANS((i * 30 - 210) % 360))) * HEXAGON_EDGE_RADIUS * 3;

        }

        for (int j = 0; j < HEXAGON_SIDES; ++j) {

            myCatanBoard.hexagons.hexFan[hexFanIndex + p++] = offsetx / WIDTH;
            myCatanBoard.hexagons.hexFan[hexFanIndex + p++] = offsety / HEIGHT;

            myCatanBoard.hexagons.hexFan[hexFanIndex + p++] = myCatanBoard.vertices[verticesIndex + q++];
            myCatanBoard.hexagons.hexFan[hexFanIndex + p++] = myCatanBoard.vertices[verticesIndex + q++];

            myCatanBoard.hexagons.hexFan[hexFanIndex + p++] = myCatanBoard.vertices[verticesIndex + q++];
            myCatanBoard.hexagons.hexFan[hexFanIndex + p++] = myCatanBoard.vertices[verticesIndex + q++];

        }

    }

    myCatanBoard.hexagons.hexTypes[0] = BRICK;
    myCatanBoard.hexagons.hexTypes[1] = BRICK;
    myCatanBoard.hexagons.hexTypes[2] = BRICK;
    myCatanBoard.hexagons.hexTypes[3] = BRICK;
    myCatanBoard.hexagons.hexTypes[4] = WOOD;
    myCatanBoard.hexagons.hexTypes[5] = WOOD;
    myCatanBoard.hexagons.hexTypes[6] = WOOD;
    myCatanBoard.hexagons.hexTypes[7] = WOOD;
    myCatanBoard.hexagons.hexTypes[8] = SHEEP;
    myCatanBoard.hexagons.hexTypes[9] = SHEEP;
    myCatanBoard.hexagons.hexTypes[10] = SHEEP;
    myCatanBoard.hexagons.hexTypes[11] = SHEEP;
    myCatanBoard.hexagons.hexTypes[12] = WHEAT;
    myCatanBoard.hexagons.hexTypes[13] = WHEAT;
    myCatanBoard.hexagons.hexTypes[14] = WHEAT;
    myCatanBoard.hexagons.hexTypes[15] = ORE;
    myCatanBoard.hexagons.hexTypes[16] = ORE;
    myCatanBoard.hexagons.hexTypes[17] = ORE;
    myCatanBoard.hexagons.hexTypes[18] = RES_NONE;

    myCatanBoard.hexagons.hexDiceValues[0] = 0;
    myCatanBoard.hexagons.hexDiceValues[1] = 2;
    myCatanBoard.hexagons.hexDiceValues[2] = 3;
    myCatanBoard.hexagons.hexDiceValues[3] = 3;
    myCatanBoard.hexagons.hexDiceValues[4] = 4;
    myCatanBoard.hexagons.hexDiceValues[5] = 4;
    myCatanBoard.hexagons.hexDiceValues[6] = 5;
    myCatanBoard.hexagons.hexDiceValues[7] = 5;
    myCatanBoard.hexagons.hexDiceValues[8] = 6;
    myCatanBoard.hexagons.hexDiceValues[9] = 6;
    myCatanBoard.hexagons.hexDiceValues[10] = 8;
    myCatanBoard.hexagons.hexDiceValues[11] = 8;
    myCatanBoard.hexagons.hexDiceValues[12] = 9;
    myCatanBoard.hexagons.hexDiceValues[13] = 9;
    myCatanBoard.hexagons.hexDiceValues[14] = 10;
    myCatanBoard.hexagons.hexDiceValues[15] = 10;
    myCatanBoard.hexagons.hexDiceValues[16] = 11;
    myCatanBoard.hexagons.hexDiceValues[17] = 11;
    myCatanBoard.hexagons.hexDiceValues[18] = 12;

    srand(time(NULL));

    enum rCardEnum tmp;
    int tmp2;
    int index;
    int random;
    for (int i = 0; i < HEXAGONS_TO_DRAW * 4; ++i) {

        random = rand();

        index = random % HEXAGONS_TO_DRAW;
        tmp = myCatanBoard.hexagons.hexTypes[i % HEXAGONS_TO_DRAW];
        myCatanBoard.hexagons.hexTypes[i % HEXAGONS_TO_DRAW] = myCatanBoard.hexagons.hexTypes[index];
        myCatanBoard.hexagons.hexTypes[index] = tmp;

        index = random % HEXAGONS_TO_DRAW;
        tmp2 = myCatanBoard.hexagons.hexDiceValues[i % HEXAGONS_TO_DRAW];
        myCatanBoard.hexagons.hexDiceValues[i % HEXAGONS_TO_DRAW] = myCatanBoard.hexagons.hexDiceValues[index];
        myCatanBoard.hexagons.hexDiceValues[index] = tmp2;

    }  

    // grab middle points for drawing the numbers associated with each hexagon


    // using a library, grab some bitmaps of numbers and place into hexagons->numberImages

    return (hexFanIndex + p);

}


int main(int argc, char* argv[]) {

    // Set error callback
    glfwSetErrorCallback(error_callback);

    // Initialize GLFW
    if (!glfwInit()) {
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Hexagon", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // mouse callback
    glfwSetMouseButtonCallback(window, mouse_callback);

    // key callback
    glfwSetKeyCallback(window, key_callback);

    // Initialize GLEW
    //glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printf("GLEW initialization failed!\n");
        return -1;
    }

    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Vendor: %s\n", glGetString(GL_VENDOR));
    if (GLEW_ARB_vertex_shader) {
        printf("ARB vertex shader is supported.\n");
    }

    GLuint width = WIDTH;
    GLuint height = HEIGHT;

    glfwGetFramebufferSize(window, &width, &height);
    CHECK_GL_ERROR();
    glViewport(0, 0, width, height);
    CHECK_GL_ERROR();

    for (int i = 0; i < 4; ++i) {
        myCatanBoard.playerPieceCosts.town[i] = townCost[i];
    }
    for (int i = 0; i < 5; ++i) {
        myCatanBoard.playerPieceCosts.city[i] = cityCost[i];
    }
    for (int i = 0; i < 2; ++i) {
        myCatanBoard.playerPieceCosts.road[i] = roadCost[i];
    }
    for (int i = 0; i < 3; ++i) {
        myCatanBoard.playerPieceCosts.devcard[i] = devCardCost[i];
    }


    //
    // SETUP
    //
    int numVerts = setBoardVertices();
    setSettlementButtons();
    setRoadButtons();
    int numFans = setBoardHexagons();
    setupDevCards();
    setupResourceCards();
    setupPlayers();


    // compile vertex shader & fragment shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    if (glIsProgram(shaderProgram) == GL_TRUE) {
        printf("shader is a recognized program\n");
    } else {
        printf("shader is not a recognized program\n");
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLuint colorVAO, geometryVAO;
    GLuint colorVBO, geometryVBO;

    glGenVertexArrays(1, &colorVAO);
    glGenBuffers(1, &colorVBO);
    glGenVertexArrays(1, &geometryVAO);
    glGenBuffers(1, &geometryVBO);
    CHECK_GL_ERROR();

    glBindVertexArray(geometryVAO);
    glBindBuffer(GL_ARRAY_BUFFER, geometryVBO);
    CHECK_GL_ERROR();

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numVerts, myCatanBoard.vertices, GL_STATIC_DRAW);
    CHECK_GL_ERROR();

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
    CHECK_GL_ERROR();
    glEnableVertexAttribArray(0);
    CHECK_GL_ERROR();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    CHECK_GL_ERROR();

    glBindVertexArray(colorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    CHECK_GL_ERROR();

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numFans, myCatanBoard.hexagons.hexFan, GL_STATIC_DRAW);
    CHECK_GL_ERROR();

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
    CHECK_GL_ERROR();
    glEnableVertexAttribArray(0);
    CHECK_GL_ERROR();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    CHECK_GL_ERROR();    

    GLint colorLocation = glGetUniformLocation(shaderProgram, "uColor");

    if (colorLocation == -1) {

        printf("Error: Uniform location for 'uColor' not found.\n");

    }

    glUseProgram(shaderProgram);
    CHECK_GL_ERROR();

    for (int i = 0; i < HEXAGONS_TO_DRAW; ++i) {

        printf("hexTypes i: %d, val: %d\n", i, myCatanBoard.hexagons.hexTypes[i]);

    }

    for (int i = 0; i < HEXAGONS_TO_DRAW; ++i) {

        printf("hexDiceValues i: %d, val: %d\n", i, myCatanBoard.hexagons.hexDiceValues[i]);

    }


    GLdouble xpos = 0.0f;
    GLdouble ypos = 0.0f;
    bool turnDone = false, decideTurnStructureUnfinished = true;
    myCatanBoard.turn = 0;
    enum gameplayConditionsEnum gameCondsIndex;
    int whichPlayer = 0;
    srand(time(NULL));

    // Main loop
    while (!glfwWindowShouldClose(window)) {

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);
        CHECK_GL_ERROR();

        glBindVertexArray(colorVAO);

        for (int i = 0; i < HEXAGONS_TO_DRAW; ++i) {

            switch (myCatanBoard.hexagons.hexTypes[i]) {

                case BRICK:

                    glUniform4fv(colorLocation, 1, brick_color);
                    break;

                case WOOD:

                    glUniform4fv(colorLocation, 1, wood_color);
                    break;

                case SHEEP:

                    glUniform4fv(colorLocation, 1, sheep_color);
                    break;

                case WHEAT:

                    glUniform4fv(colorLocation, 1, wheat_color);
                    break;

                case ORE:

                    glUniform4fv(colorLocation, 1, ore_color);
                    break;

                case RES_NONE:

                    glUniform4fv(colorLocation, 1, sand_color);
                    break;

            }

            glDrawArrays(GL_TRIANGLE_FAN, FAN_VERTICES_INCREMENT * i, FAN_VERTICES_INCREMENT);

        }

        glBindVertexArray(0);
        CHECK_GL_ERROR();

        // geometry
        glBindVertexArray(geometryVAO);
        glUniform4fv(colorLocation, 1, geometry_color);
        glDrawArrays(GL_LINES, 0, GEOMETRY_POINTS_TO_DRAW);
        glBindVertexArray(0);
        CHECK_GL_ERROR();

        // Swap buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();

        //
        // pre game logic starts here
        //

        // decide turn structure for this game -- NOT WORKING
        if (decideTurnStructureUnfinished == true) {
            
            bool reroll[3] = {true, true, true};
            int playerTurnStructure = 0;
            while (decideTurnStructureUnfinished) {

                // roll
                if (reroll[0]) {
                    
                    myCatanBoard.players[0].currentRoll = DICEROLL() + DICEROLL();
                
                } else if (reroll[1]) {

                    myCatanBoard.players[1].currentRoll = DICEROLL() + DICEROLL();

                } else if (reroll[2]) {

                    myCatanBoard.players[2].currentRoll = DICEROLL() + DICEROLL();

                }

                // sort
                reroll[0] = false;
                reroll[1] = false;
                reroll[2] = false;                
                for (int i = 0; i < 3; ++i) {

                    if (myCatanBoard.players[i].currentRoll > myCatanBoard.players[(i + 1) % 3].currentRoll && myCatanBoard.players[i].currentRoll > myCatanBoard.players[(i + 2) % 3].currentRoll) {

                        playerTurnStructure = 1;

                    } else if (myCatanBoard.players[i].currentRoll > myCatanBoard.players[(i + 1) % 3].currentRoll || myCatanBoard.players[i].currentRoll > myCatanBoard.players[(i + 2) % 3].currentRoll) {

                        playerTurnStructure = 2;

                    } else if (myCatanBoard.players[i].currentRoll < myCatanBoard.players[(i + 1) % 3].currentRoll && myCatanBoard.players[i].currentRoll < myCatanBoard.players[(i + 2) % 3].currentRoll) {

                        playerTurnStructure = 3;

                    } else if (myCatanBoard.players[i].currentRoll == myCatanBoard.players[(i + 1) % 3].currentRoll) {

                        reroll[i] = true;

                    }

                    myCatanBoard.gameTurnStructure[i] = playerTurnStructure;

                }

                if (!reroll[0] && !reroll[1] && !reroll[2]) {

                    decideTurnStructureUnfinished = false;

                }

            }

            printf("playerTurnStructure: %s : %d, %s : %d, %s : %d\n\n", playerColors[0], myCatanBoard.gameTurnStructure[0], playerColors[1], myCatanBoard.gameTurnStructure[1], playerColors[2], myCatanBoard.gameTurnStructure[2]);

            // preGameReturnValue: false == returned normally, true == window needs to close
            bool preGameReturnValue = false;
            // pre game sequence: setup towns and roads -- WORKS
            for (int i = 0; i < 3; ++i) {

                whichPlayer = myCatanBoard.gameTurnStructure[i] - 1;

                printf("%s player's turn.\n\n", playerColors[whichPlayer]);

                //
                // players place town 1
                //
                printf("%s player : place first town\n\n", playerColors[whichPlayer]);
                
                myCatanBoard.players[whichPlayer].gameConds[PLACE_TOWN_COND] = true;
                preGameReturnValue = processPreGamePlayerActions(window);
                if (!preGameReturnValue) {

                    decideTurnStructureUnfinished = false;
                    break;

                }
                myCatanBoard.players[whichPlayer].whichRoadButtonPressed = DUMMY_VALUE;
                myCatanBoard.players[whichPlayer].whichSettlementButtonPressed = DUMMY_VALUE;

                char road1[8], road2[8], town1[8], town2[8];
                sprintf(town1, "%d", (myCatanBoard.players[whichPlayer].playerTownLocations[0]));                
                printf("player #%d | road location 1: %s road location 2: %s, town location 1: %s town location 2: %s\n\n", whichPlayer, (myCatanBoard.players[whichPlayer].playerRoadLocations[0] == DUMMY_VALUE ? locationUnset : road1), (myCatanBoard.players[whichPlayer].playerRoadLocations[1] == DUMMY_VALUE ? locationUnset : road2), (myCatanBoard.players[whichPlayer].playerTownLocations[0] == DUMMY_VALUE ? locationUnset : town1), (myCatanBoard.players[whichPlayer].playerTownLocations[1] == DUMMY_VALUE ? locationUnset : town2));

                //
                // players place road 1
                //
                printf("%s player : place first road\n\n", playerColors[whichPlayer]);
                
                myCatanBoard.players[whichPlayer].gameConds[PLACE_ROAD_COND] = true;
                preGameReturnValue = processPreGamePlayerActions(window);
                if (!preGameReturnValue) {

                    decideTurnStructureUnfinished = false;
                    break;

                }
                myCatanBoard.players[whichPlayer].whichRoadButtonPressed = DUMMY_VALUE;
                myCatanBoard.players[whichPlayer].whichSettlementButtonPressed = DUMMY_VALUE;

                sprintf(road1, "%d", (myCatanBoard.players[whichPlayer].playerRoadLocations[0]));
                printf("player #%d | road location 1: %s road location 2: %s, town location 1: %s town location 2: %s\n\n", whichPlayer, (myCatanBoard.players[whichPlayer].playerRoadLocations[0] == DUMMY_VALUE ? locationUnset : road1), (myCatanBoard.players[whichPlayer].playerRoadLocations[1] == DUMMY_VALUE ? locationUnset : road2), (myCatanBoard.players[whichPlayer].playerTownLocations[0] == DUMMY_VALUE ? locationUnset : town1), (myCatanBoard.players[whichPlayer].playerTownLocations[1] == DUMMY_VALUE ? locationUnset : town2));

                //
                // players place town 2
                //
                printf("%s player : place second town\n\n", playerColors[whichPlayer]);
                
                myCatanBoard.players[whichPlayer].gameConds[PLACE_TOWN_COND] = true;
                preGameReturnValue = processPreGamePlayerActions(window);
                if (!preGameReturnValue) {

                    decideTurnStructureUnfinished = false;
                    break;

                }
                myCatanBoard.players[whichPlayer].whichRoadButtonPressed = DUMMY_VALUE;
                myCatanBoard.players[whichPlayer].whichSettlementButtonPressed = DUMMY_VALUE;

                sprintf(town2, "%d", (myCatanBoard.players[whichPlayer].playerTownLocations[1]));
                printf("player #%d | road location 1: %s road location 2: %s, town location 1: %s town location 2: %s\n\n", whichPlayer, (myCatanBoard.players[whichPlayer].playerRoadLocations[0] == DUMMY_VALUE ? locationUnset : road1), (myCatanBoard.players[whichPlayer].playerRoadLocations[1] == DUMMY_VALUE ? locationUnset : road2), (myCatanBoard.players[whichPlayer].playerTownLocations[0] == DUMMY_VALUE ? locationUnset : town1), (myCatanBoard.players[whichPlayer].playerTownLocations[1] == DUMMY_VALUE ? locationUnset : town2));

                //
                // players place road 2
                //
                printf("%s player : place second road\n\n", playerColors[whichPlayer]);

                myCatanBoard.players[whichPlayer].gameConds[PLACE_ROAD_COND] = true;                    
                preGameReturnValue = processPreGamePlayerActions(window);
                if (!preGameReturnValue) {

                    decideTurnStructureUnfinished = false;
                    break;

                }
                myCatanBoard.players[whichPlayer].whichRoadButtonPressed = DUMMY_VALUE;
                myCatanBoard.players[whichPlayer].whichSettlementButtonPressed = DUMMY_VALUE;

                sprintf(road2, "%d", (myCatanBoard.players[whichPlayer].playerRoadLocations[1]));
                printf("player #%d | road location 1: %s road location 2: %s, town location 1: %s town location 2: %s\n\n", whichPlayer, (myCatanBoard.players[whichPlayer].playerRoadLocations[0] == DUMMY_VALUE ? locationUnset : road1), (myCatanBoard.players[whichPlayer].playerRoadLocations[1] == DUMMY_VALUE ? locationUnset : road2), (myCatanBoard.players[whichPlayer].playerTownLocations[0] == DUMMY_VALUE ? locationUnset : town1), (myCatanBoard.players[whichPlayer].playerTownLocations[1] == DUMMY_VALUE ? locationUnset : town2));

                ++myCatanBoard.turn;
                myCatanBoard.turnStructureIndex = myCatanBoard.turn % 3;
                myCatanBoard.isPregame = false;

            }

        }

        //
        // game play begins here
        //
        // Controls: 'q' to cancel any action except 'n'
        //           'n' to forfeit players' turn for next player in queue
        //           'r' to initiate placing a road for the current player
        //           't' to initiate placing a town for the current player
        //           'c' to initiate placing a city for the current player
        //           'd' to initiate buying a dev card for current player
        //           'u' to initiate using of a dev card for current player
        //           'a' to initiate a trade between two players or with bank

    }

    // Clean up and exit
    glDeleteVertexArrays(1, &colorVAO);
    glDeleteBuffers(1, &colorVBO);
    glDeleteVertexArrays(1, &geometryVAO);
    glDeleteBuffers(1, &geometryVBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;

}