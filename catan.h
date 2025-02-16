//
// includes
//
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// need an image library for numbers, roads, houses, cities

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>


// 
// macros
// 
#define HEXAGON_SIDES                   6
#define VERTICES_PER_HEXAGON            (HEXAGON_SIDES + 1)
#define HEXAGONS_TO_DRAW                19
#define FLOATS_PER_HEXAGON              ((HEXAGON_SIDES + HEXAGON_SIDES) * 2)
#define GEOMETRY_POINTS_TO_DRAW         ((HEXAGON_SIDES + HEXAGON_SIDES) * HEXAGONS_TO_DRAW)
#define FAN_VERTICES_INCREMENT          (HEXAGON_SIDES * 3)
#define WIDTH                           800
#define HEIGHT                          600
#define HEXAGON_SIDE_RADIUS             75
#define HEXAGON_EDGE_RADIUS             86.60254030
#define NUM_SETTLEMENT_BUTTONS          54
#define NUM_ROAD_BUTTONS                72
#define SMALL_BUTTON_WIDTH              10
#define SMALL_BUTTON_HEIGHT             10
#define MED_BUTTON_WIDTH                20
#define MED_BUTTON_HEIGHT               20
#define TOTAL_DEV_CARD_COUNT            25
#define TOTAL_RESOURCE_CARD_COUNT       95
#define NUM_PIECES_PER_PLAYER           25  // 5 towns, 5 cities (might be 4), 15 roads 
#define NUM_ROADS_PER_PLAYER            15
#define NUM_TOWNS_PER_PLAYER            5
#define NUM_CITIES_PER_PLAYER           5
#define HEX_VALUE_RADIUS                20
#define DUMMY_VALUE                     252
#define TOTAL_GAMEPLAY_CONDITIONS       7   // so far

#define TORADIANS(degrees)                      (degrees * (M_PI / 180))
// f is either 'w' or 'h'
#define TOPIXELS(f, normal_value)               (f == 'w' ? normal_value * WIDTH : (f == 'h' ? normal_value * HEIGHT : 0.0f))
#define TONORMAL(f, pixel_value)                (f == 'w' ? pixel_value / WIDTH : (f == 'h' ? pixel_value / HEIGHT : 0.0f))
#define VIEWPORT_SHRINK(vert_coords)            (vert_coords / 2)
// assumes pixel values -- f is either 'x' or 'y'
#define TOMOUSECOORDS(f, shrink, vert_coords)   (f == 'x' ? (shrink == true ? VIEWPORT_SHRINK(vert_coords) + (WIDTH / 2) : vert_coords + (WIDTH / 2)) : (f == 'y' ? (shrink == true ? (HEIGHT / 2) - (VIEWPORT_SHRINK(vert_coords)) : ((HEIGHT / 2) - vert_coords)) : 0.0f))
#define TOVERTCOORDS(f, mouse_coords)           (f == 'x' ? (mouse_coords - (WIDTH / 2)) : (f == 'y' ? (mouse_coords - (HEIGHT / 2)) : 0.0f))
#define DICEROLL()                              (rand() % 6 + 1)

// 
// sources/data
//
const char* locationUnset                           = "unset";
const char* playerColors[3]                         = {"blue", "green", "red"};

const char* knightDesc                              = "move the robber once\n";
const char* vpsDesc                                 = "one victory point, play anytime\n";
const char* roadsDesc                               = "build two roads\n";
const char* plentyDesc                              = "grab two resources from bank\n";
const char* monopolyDesc                            = "declare one resource type, all play must give up that type to the player\n";

const char* gl_invalid_enum                         = "An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.\n";
const char* gl_invalid_value                        = "A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.\n";
const char* gl_invalid_operation                    = "The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.\n";
const char* gl_invalid_frame_buffer_operation       = "The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.\n";
const char* gl_out_of_memory                        = "There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.\n";
const char* gl_stack_underflow                      = "An attempt has been made to perform an operation that would cause an internal stack to underflow.\n";
const char* gl_stack_overflow                       = "An attempt has been made to perform an operation that would cause an internal stack to overflow.\n";

const int SETTLEMENT_BUTTONS_WRITE[19]              = {6, 4, 3, 3, 3, 3, 2, 4, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 1}; // total new buttons for this hex
const int SETTLEMENT_BUTTONS_LOC[19]                = {DUMMY_VALUE, 2, 3, 4, 5, 0, 0, 2, 2, 3, 3, 4, 4, 5, 5, 0, 0, 1, 0}; // start skipping at

const int ROADS_BUTTONS_WRITE[19]                   = {6, 5, 4, 4, 4, 4, 3, 5, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 2};  // total roads-clickable locations on each hexagon
const int ROADS_BUTTONS_LOC[19]                     = {DUMMY_VALUE, 2, 3, 4, 5, 0, 0, 2, 2, 3, 3, 4, 4, 5, 5, 0, 0, 1, 0};  // where to start sequential skipping of hexagon sides

const GLfloat brick_color[4]                        = {1.0f, 0.0f, 0.0f, 1.0f};          // red
const GLfloat wood_color[4]                         = {0.545f, 0.271f, 0.075f, 1.0f};    // brown
const GLfloat sheep_color[4]                        = {0.0f, 1.0f, 0.0f, 1.0f};          // green
const GLfloat wheat_color[4]                        = {1.0f, 1.0f, 0.0f, 1.0f};          // yellow
const GLfloat ore_color[4]                          = {0.5f, 0.5f, 0.5f, 1.0f};          // grey
const GLfloat sand_color[4]                         = {0.5f, 0.0f, 0.5f, 1.0f};          // purple
const GLfloat geometry_color[4]                     = {1.0f, 1.0f, 1.0f, 1.0f};          // white

const GLchar* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec2 aPos;
void main() {
   gl_Position = vec4(aPos, 0.0, 1.0);
}
)";

const GLchar* fragmentShaderSource = R"(
#version 330 core
uniform vec4 uColor;
out vec4 FragColor;
void main() {
   FragColor = uColor;
}
)";

//
// Error detection macro
//
#define CHECK_GL_ERROR()                                        \
    do {                                                        \
        GLenum err;                                             \
        while ((err = glGetError()) != GL_NO_ERROR) {           \
            switch (err) {                                      \
                case GL_INVALID_ENUM:                           \
                    printf("OpenGL Error [File: %s, Line: %d]: #%d %s\n", __FILE__, __LINE__, err, gl_invalid_enum);                        \
                    break;                                                                                                                  \
                case GL_INVALID_VALUE:                                                                                                      \
                    printf("OpenGL Error [File: %s, Line: %d]: #%d %s\n", __FILE__, __LINE__, err, gl_invalid_value);                       \
                    break;                                                                                                                  \
                case GL_INVALID_OPERATION:                                                                                                  \
                    printf("OpenGL Error [File: %s, Line: %d]: #%d %s\n", __FILE__, __LINE__, err, gl_invalid_operation);                   \
                    break;                                                                                                                  \
                case GL_INVALID_FRAMEBUFFER_OPERATION:                                                                                      \
                    printf("OpenGL Error [File: %s, Line: %d]: #%d %s\n", __FILE__, __LINE__, err, gl_invalid_frame_buffer_operation);      \
                    break;                                                                                                                  \
                case GL_OUT_OF_MEMORY:                                                                                                      \
                    printf("OpenGL Error [File: %s, Line: %d]: #%d %s\n", __FILE__, __LINE__, err, gl_out_of_memory);                       \
                    break;                                                                                                                  \
                case GL_STACK_UNDERFLOW:                                                                                                    \
                    printf("OpenGL Error [File: %s, Line: %d]: #%d %s\n", __FILE__, __LINE__, err, gl_stack_underflow);                     \
                    break;                                                                                                                  \
                case GL_STACK_OVERFLOW:                                                                                                     \
                    printf("OpenGL Error [File: %s, Line: %d]: #%d %s\n", __FILE__, __LINE__, err, gl_stack_overflow);                      \
                    break;                                                                                                                  \
            }                                                                                                                               \
        }                                                       \
    } while (0)                                                 \



// 
// enums
//
enum dCardEnum { KNIGHTS = 0, VPS = 1, ROADS = 2, PLENTY = 3, MONOPOLY = 4, GENERIC_DEV_CARD = 5 };

enum rCardEnum { BRICK = 6, WOOD = 7, SHEEP = 8, WHEAT = 9, ORE = 10, RES_NONE = 11 };

enum playerEnum { PLAYERONE = 12, PLAYERTWO = 13, PLAYERTHREE = 14 };

enum pieceEnum { TOWN = 15, CITY = 16, ROAD = 17, PIECE_NONE = 18 };

enum vertEnum { GEOMETRY = 19, COLOR = 20, HEX_VALUE_CIRCLES = 21 };

enum gameplayConditionsEnum { NEXT_TURN_COND = 0, PLACE_TOWN_COND = 1, PLACE_ROAD_COND = 2, PLACE_CITY_COND = 3, BUY_DCARD_COND = 4, USE_DCARD_COND = 5, INIT_TRADE_COND = 6, NONE_COND = 7 };


//
// 
//
const enum rCardEnum townCost[4]      = { BRICK, WOOD, SHEEP, WHEAT };
const enum rCardEnum cityCost[5]      = { WHEAT, WHEAT, ORE, ORE, ORE };
const enum rCardEnum roadCost[2]      = { BRICK, WOOD };
const enum rCardEnum devCardCost[3]   = { SHEEP, WHEAT, ORE };


// 
// structures
//
struct board {

    // not useful to the artificial player
    GLfloat vertices[GEOMETRY_POINTS_TO_DRAW * 2];                  // raw vertices for board geometry
    GLfloat boardSettlementButtons[NUM_SETTLEMENT_BUTTONS * 2];     // coordinates on intersection points for the lines that board geometry forms
    GLfloat boardRoadButtons[NUM_ROAD_BUTTONS * 2];                 // coordinates on the lines that board geometry forms 
    enum dCardEnum myDevCardDeck[TOTAL_DEV_CARD_COUNT];             // board developement card deck
    enum rCardEnum myResourceCardDeck[TOTAL_RESOURCE_CARD_COUNT];   // board resource card deck
    int turnStructureIndex;                                         // used to calculate gameTurnStructure index once instead of in multiple places throughout game
    bool click, confirmClick;                                       // used for keyboard callback function for a detected key click and subsequent comfirmation click
    bool isPregame;

    // useful to the artificial player
    int roadToRoadAdjacencyArray[NUM_ROAD_BUTTONS][4];                          // space for road locations adjacent to any road
    int settlementToSettlementAdjacencyArray[NUM_SETTLEMENT_BUTTONS][3];        // space for settlement locations adjacent to any settlement
    int roadToSettlementAdjacencyArray[NUM_ROAD_BUTTONS][2];                    // space for settlement locations adjacent to any road
    int gameTurnStructure[3];                                                   // the overall player turn structure created in main    
    int turn;                                                                   // raw number of turns taken by all players

    struct playerInfo {

        // not useful to the artificial player
        enum playerEnum id;                                         // probably useless (TODO: DELETE?)
        int whichRoadButtonPressed, whichSettlementButtonPressed;   // the actual road/settlement button pressed by a player (TODO: CREATE A NUM->VERT MAPPING FOR THIS TO DISPLAY IN MAIN)

        // useful to the artificial player
        // this player goes when?
        int playerTurnStructure;                                    // the place the player has in gameTurnStructure (TODO: MAKE LOCAL VAR?)
        // what is/was the roll of each player?
        int currentRoll;                                            // the players' current roll
        // what are the locations of each players' roads
        int playerRoadLocations[NUM_ROADS_PER_PLAYER];              // locations of roads
        // what are the locations of each players' towns
        int playerTownLocations[NUM_TOWNS_PER_PLAYER];              // locations of towns
        // what are the locations of each players' cities
        int playerCityLocations[NUM_CITIES_PER_PLAYER];             // locations of cities
        // what dev cards does each player have
        enum dCardEnum playerDevCards[TOTAL_DEV_CARD_COUNT];             // developement cards in players' hand currently
        // what resource cards does each player have
        enum rCardEnum playerResourceCards[TOTAL_RESOURCE_CARD_COUNT];   // resource cards in players' hand currently
        // how many towns/cities/roads are left in each players' hand/territory, how many dev cards does each player have
        int playerTownsInHand, playerCitiesInHand, playerRoadsInHand, playerDevCardCount; // number of towns/cities/roads/devcards a player has on board or in hand        
        int playerTownsOnBoard, playerCitiesOnBoard, playerRoadsOnBoard;    
        // what is each player thinking/doing on their turn?
        bool gameConds[TOTAL_GAMEPLAY_CONDITIONS];                  // conditions that control the flow of a player's turn

    } players[3];

    struct hexagonsInfo {

        // not useful to the artificial player
        GLfloat hexFan[HEXAGON_SIDES * HEXAGONS_TO_DRAW * 3 * 2];   // vertices for color fans, hexTypes dictate colors
        GLfloat middlePoints[HEXAGONS_TO_DRAW * 2];                 // describe where on board to place images of numbers
                                                                    // something to hold images of numbers
        // useful to the artificial player
        enum rCardEnum hexTypes[HEXAGONS_TO_DRAW];                  // resource types for each hexagon
        int hexDiceValues[HEXAGONS_TO_DRAW];                        // random dice values

    } hexagons;

    struct pieceCosts {

        enum rCardEnum town[4];
        enum rCardEnum city[5];
        enum rCardEnum road[2];
        enum rCardEnum devcard[3];

    } playerPieceCosts;

} myCatanBoard;


//
// prototypes -- forward declarations
//
static bool isLocationUnique(int whichPlayer, enum pieceEnum whichPiece);
static bool isRoadAdjacentToSettlement(int whichPlayer, enum pieceEnum whichPiece);
static bool isRoadAdjacentToRoad(int whichPlayer);
static bool isSettlementAdjacentToSettlement(int whichPlayer, enum pieceEnum whichPiece);
static bool processPreGamePlayerActions(GLFWwindow* window);
static void setHexagon(GLfloat* verts);
static int setBoardVertices();
static void setSettlementButtons();
static void setRoadButtons();
static void setupDevCards();
static void setupResourceCards();
static void popDev(enum dCardEnum* ddeck, enum dCardEnum dcard);
static void popRes(enum rCardEnum* rdeck, enum rCardEnum rcard);
static void pushRes(enum rCardEnum* rdeck, enum rCardEnum rcard);
static void setupPlayers();
static int setBoardHexagons();