#define _CRT_SECURE_NO_WARNINGS

#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "Economy.h"
#include "UserAgent.h"
#include "StatsTracker.h"

enum Tab { TAB_MARKET, TAB_INVENTORY, TAB_GRAPHS };

int main() {
    InitWindow(1000, 700, "Economy Simulator");
    SetTargetFPS(60);

    Font uiFont = LoadFont("C:/Windows/Fonts/times.ttf");
    GuiSetFont(uiFont);

    std::vector<std::string> itemNames = {
        "Wool","Linen","Fish","Beef","Chicken",
        "Pickaxe","Axe","Hammer","Knife","Wood","Stone","Iron"
    };
    std::vector<double> initialPrices = { 2.0,3.0,1.5,3.0,2.5,15.0,12.0,10.0,5.0,1.0,1.0,4.0 };
    std::vector<double> productionCaps = { 2,2,3,3,3,0.5,0.5,0.5,1,4,4,3 };

    std::mt19937_64 rng(42);
    Economy econ(10, itemNames, initialPrices, productionCaps, rng);
    UserAgent player(200.0, productionCaps);
    StatsTracker tracker(itemNames.size());

    int turn = 0;
    Tab currentTab = TAB_MARKET;
    //Turn intervals
    double turnInterval = 1.0; //2X speed
    double lastTurnTime = 0.0;
    bool isPaused = false;

    int graphItemIndex = 0;  //Selected item for graph
    float topMargin = 20.0f, tabY = 130.0f, headerY = 160.0f, contentStartY = headerY + 30.0f, rowHeight = 40.0f;

    while (!WindowShouldClose()) {
        double currentTime = GetTime();

		// Turn System for Advancment
        if (!isPaused && currentTime - lastTurnTime >= turnInterval) {
            econ.step(true, &player);
            tracker.record(econ.collect_stats());
            turn++;
            lastTurnTime = currentTime;
        }

        EconomyStats stats = econ.collect_stats();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        //Header
        char turnText[64]; sprintf_s(turnText, sizeof(turnText), "Turn: %d", turn);
        char moneyText[64]; sprintf_s(moneyText, sizeof(moneyText), "Money: %.2f", player.get_money());
        DrawTextEx(uiFont, "Economy Simulator", { 20, topMargin }, 30, 2.0f, DARKBLUE);
        DrawTextEx(uiFont, turnText, { 20, topMargin + 40 }, 20, 1.5f, BLACK);
        DrawTextEx(uiFont, moneyText, { 20, topMargin + 70 }, 20, 1.5f, DARKGREEN);

        //Tabs
        if (GuiButton({ 20, tabY, 100, 30 }, "Market")) currentTab = TAB_MARKET;
        if (GuiButton({ 140, tabY, 100, 30 }, "Inventory")) currentTab = TAB_INVENTORY;
        if (GuiButton({ 260, tabY, 100, 30 }, "Graphs")) currentTab = TAB_GRAPHS;

        //Speed/Pause
        if (GuiButton({ 800, topMargin, 50, 30 }, "1X")) turnInterval = 2.0;
        if (GuiButton({ 860, topMargin, 50, 30 }, "2X")) turnInterval = 1.0;
        if (GuiButton({ 920, topMargin, 50, 30 }, "3X")) turnInterval = 0.5;
        if (GuiButton({ 800, topMargin + 40, 120, 30 }, isPaused ? "Resume" : "Pause")) isPaused = !isPaused;

        //Market Tab
        if (currentTab == TAB_MARKET) {
            DrawTextEx(uiFont, "Market Prices & Trading", { 20, headerY }, 20, 1.5f, DARKGRAY);

            for (size_t i = 0; i < itemNames.size(); i++) {
                float y = contentStartY + i * rowHeight;

                char priceText[64]; sprintf_s(priceText, sizeof(priceText), "Price: %.2f", stats.prices[i]);
                char invText[64]; sprintf_s(invText, sizeof(invText), "Inv: %d", player.get_inventory(i));
                DrawTextEx(uiFont, itemNames[i].c_str(), { 20, y }, 20, 1.5f, BLACK);
                DrawTextEx(uiFont, priceText, { 200, y }, 20, 1.5f, BLUE);
                DrawTextEx(uiFont, invText, { 400, y }, 20, 1.5f, DARKGREEN);

                if (GuiButton({ 550, y, 60, 25 }, "Buy") && player.get_money() >= stats.prices[i]) {
                    player.add_item(i, 1); player.add_money(-stats.prices[i]);
                }
                if (GuiButton({ 620, y, 60, 25 }, "Sell") && player.get_inventory(i) > 0) {
                    player.remove_item(i, 1); player.add_money(stats.prices[i]);
                }
            }
        }

        //Inventory Tab
        if (currentTab == TAB_INVENTORY) {
            DrawTextEx(uiFont, "Player Inventory", { 20, headerY }, 20, 1.5f, DARKGRAY);
            for (size_t i = 0; i < itemNames.size(); i++) {
                float y = contentStartY + i * rowHeight;
                char invText[64]; sprintf_s(invText, sizeof(invText), "You own: %d", player.get_inventory(i));
                DrawTextEx(uiFont, itemNames[i].c_str(), { 20, y }, 20, 1.5f, BLACK);
                DrawTextEx(uiFont, invText, { 200, y }, 20, 1.5f, DARKGREEN);
            }
        }

        //Graph Tab
        if (currentTab == TAB_GRAPHS) {
			//Graph Item Selector
            char comboText[512] = "";
            for (size_t i = 0; i < itemNames.size(); i++) {
                char buf[64]; sprintf_s(buf, sizeof(buf), "%s", itemNames[i].c_str());
                strcat_s(comboText, sizeof(comboText), buf);
                if (i != itemNames.size() - 1) strcat_s(comboText, sizeof(comboText), ";");
            }
            GuiComboBox({ 20, headerY, 300, 30 }, comboText, &graphItemIndex);

            //Graph Title
            char graphTitle[128];
            sprintf_s(graphTitle, sizeof(graphTitle), "Price History (%s)", itemNames[graphItemIndex].c_str());
            DrawTextEx(uiFont, graphTitle, { 340, headerY }, 20, 1.5f, DARKGRAY);

            //Graph
            Rectangle graphRect = { 50, headerY + 50, 900, 400 };
            DrawRectangleLinesEx(graphRect, 2, GRAY);

            size_t historySize = tracker.getHistorySize(graphItemIndex);
            if (historySize > 1) {
                float maxPrice = tracker.getMaxPrice(graphItemIndex);
                float minPrice = tracker.getMinPrice(graphItemIndex);

                //Y-axis & labels
                int numYLines = 5;
                for (int y = 0; y <= numYLines; y++) {
                    float yPos = graphRect.y + graphRect.height - y * graphRect.height / numYLines;
                    DrawLine(graphRect.x, yPos, graphRect.x + graphRect.width, yPos, LIGHTGRAY);
                    float priceVal = minPrice + y * (maxPrice - minPrice) / numYLines;
                    char yLabel[32]; sprintf_s(yLabel, sizeof(yLabel), "%.2f", priceVal);
                    DrawTextEx(uiFont, yLabel, { graphRect.x - 40, yPos - 10 }, 15, 1.2f, BLACK);
                }

                //X-axis & labels
                int numXLines = 10;
                for (int x = 0; x <= numXLines; x++) {
                    int turnIndex = x * (historySize - 1) / numXLines;
                    float xPos = graphRect.x + turnIndex * graphRect.width / (historySize - 1);
                    DrawLine(xPos, graphRect.y, xPos, graphRect.y + graphRect.height, LIGHTGRAY);
                    char xLabel[16]; sprintf_s(xLabel, sizeof(xLabel), "%d", turnIndex);
                    DrawTextEx(uiFont, xLabel, { xPos - 10, graphRect.y + graphRect.height + 5 }, 15, 1.2f, BLACK);
                }

                //Draws Price on Graph
                for (size_t i = 1; i < historySize; i++) {
                    float prevX = graphRect.x + (i - 1) * graphRect.width / (historySize - 1);
                    float prevY = graphRect.y + graphRect.height -
                        (tracker.getPrice(graphItemIndex, i - 1) - minPrice) * graphRect.height / (maxPrice - minPrice);
                    float currX = graphRect.x + i * graphRect.width / (historySize - 1);
                    float currY = graphRect.y + graphRect.height -
                        (tracker.getPrice(graphItemIndex, i) - minPrice) * graphRect.height / (maxPrice - minPrice);
                    DrawLine(prevX, prevY, currX, currY, RED);
                }
            }
        }

        EndDrawing();
    }

    UnloadFont(uiFont);
    CloseWindow();
    return 0;
}
