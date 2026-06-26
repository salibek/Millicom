#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>

#include "Consts.h"
#include <string>

struct NodeData
{
    string name;
    vector<string> content;
};

struct EdgeData
{
    int fromIdx = 0;
    int cellIdx = 0;
    int toIdx = 0;
};

struct Vertex
{
    float x, y = 0.0;
    float dx = 0, dy = 0;
    int w = 130, h;
    NodeData data;
    bool visible = true;
    bool selected = false;
    int parentGroupIdx = -1;
    const int cellH = 25;

    Vertex(NodeData d) : data(d)
    {
        h = data.content.size() * cellH;
    }

    bool isInside(int mx, int my)
    {
        if (!visible) return false;
        return mx >= x && mx <= x + w && my >= y && my <= y + h;
    }
};

struct Group
{
    float x, y = 0.0;
    vector<int> nodeIndices;
    float h = 80;
    float currentW = 80;
    bool selected = false;
    static const int size = 40;
    string name = "";

    bool isInside(int mx, int my) const
    {
        return (mx >= x && mx <= x + currentW && my >= y && my <= y + h);
    }
};

struct LayoutBridge
{
    vector<NodeData> nodes;
    vector<EdgeData> edges;
    map<string, int> idToIdx;

    int GetOrCreateNode(const string& id, const string& title, const vector<string>& rows)
    {
        if (idToIdx.find(id) == idToIdx.end())
        {
            idToIdx[id] = nodes.size();
            nodes.push_back({ title, rows });
        }

        return idToIdx[id];
    }
};

class GraphCanvas : public Fl_Widget
{
private:
    vector<Vertex> vertices;
    vector<EdgeData> edges;
    vector<Group> groups;

    bool isMovingSelection = false;
    int lastMx = 0, lastMy = 0;
    bool isSelecting = false;
    int selX1 = 0, selY1 = 0, selX2 = 0, selY2 = 0;

public:
    GraphCanvas(int x, int y, int w, int h, const vector<NodeData>& nodes, const vector<EdgeData>& connections);

    static void PrepareCanvasData(IC_type graphUk, LayoutBridge& bridge, LoadMnemoToStr& mnemoToStr);

    void ApplyLayout();

    void draw() override;

    int handle(int event) override;

private:
    static void ProcessLoadPoint(LoadPoint& lp, const vector<IC_type>& graph, LayoutBridge& bridge,
        const string& currentNode, int& portCounter, const string& prefix, vector<string>& currentContent,
        LoadMnemoToStr& mnemoToStr);

    bool HasAnySelection();

    void GetReferencePoint(int nodeIdx, bool isStart, int cellIdx, int& outX, int& outY);

    void CreateGroupFromSelected();

    void ReindexGroups();

    void Ungroup(int groupIdx);

    void ShowContextMenu(int mx, int my);
};