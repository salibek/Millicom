#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Tree.H>
#include <FL/Fl_PNG_Image.H>

#include "GraphCanvas.h"
#include "GraphDisplay.h"


GraphCanvas::GraphCanvas(int x, int y, int w, int h, const vector<NodeData>& nodes, const vector<EdgeData>& connections)
    : Fl_Widget(x, y, w, h), edges(connections)
{
    for (const auto& n : nodes) vertices.emplace_back(n);
    ApplyLayout();
}


void GraphCanvas::ProcessLoadPoint(
    LoadPoint& lp,
    const vector<IC_type>& graph,
    LayoutBridge& bridge,
    const string& currentNode,
    int& portCounter,
    const string& prefix,
    vector<string>& currentContent,
    LoadMnemoToStr& mnemoToStr)
{
    int currentPortIdx = portCounter++;
    string label = prefix + (lp.isConst() ? " # " : " = ");

    if (lp.isIC())
    {
        IC_type ic = lp.IC();
        auto it = find(graph.begin(), graph.end(), ic);
        string target = (it != graph.end()) ? "IC_" + to_string(distance(graph.begin(), it)) : "unknown";

        currentContent.push_back(label + target);

        int fromIdx = bridge.idToIdx[currentNode];
        int toIdx = bridge.GetOrCreateNode(target, target, { target });
        bridge.edges.push_back({ fromIdx, currentPortIdx + 1, toIdx });

        return;
    }

    if (lp.getType() >> 1 == DLoadArray)
    {
        string arrayNodeId = currentNode + "_arr_" + to_string(currentPortIdx);

        int arrayIdxInBridge = bridge.nodes.size();
        bridge.idToIdx[arrayNodeId] = arrayIdxInBridge;
        bridge.nodes.push_back({ "Array", {"Array"} });

        LoadVect_type arr = lp.LoadVect();
        int innerPort = 0;
        vector<string> arrayRows = { "Array" };

        for (int i = 0; i < (int)arr->size(); ++i)
        {
            string childPrefix = "i " + to_string(i);
            ProcessLoadPoint((*arr)[i], graph, bridge, arrayNodeId, innerPort, childPrefix, arrayRows, mnemoToStr);
        }

        bridge.nodes[arrayIdxInBridge].content = arrayRows;
        currentContent.push_back(label + "Array");
        int fromIdx = bridge.idToIdx[currentNode];
        bridge.edges.push_back({ fromIdx, currentPortIdx + 1, arrayIdxInBridge });

        return;
    }

    currentContent.push_back(label + mnemoToStr.LoadConv(lp));
}

void GraphCanvas::PrepareCanvasData(IC_type graphUk, LayoutBridge& bridge, LoadMnemoToStr& mnemoToStr)
{
    GraphStats graphStats;
    ICVect graph = GraphDisplay::TraverseGraph(graphUk, graphStats);

    for (int i = 0; i < (int)graph.size(); ++i)
    {
        string nodeName = "IC_" + to_string(i);
        bridge.GetOrCreateNode(nodeName, nodeName, { nodeName });
    }

    for (int i = 0; i < (int)graph.size(); ++i)
    {
        string nodeName = "IC_" + to_string(i);
        int nodeIdx = bridge.idToIdx[nodeName];

        vector<string> content = { nodeName };
        int portCounter = 0;

        for (auto& ip : *graph[i])
        {
            string prefix = mnemoToStr.AtrConv(ip.atr);
            ProcessLoadPoint(ip.Load, graph, bridge, nodeName, portCounter, prefix, content, mnemoToStr);
        }

        bridge.nodes[nodeIdx].content = content;
    }
}

// метод для расположения вершин, использует силовой алгоритм Fruchterman-Reingold
void GraphCanvas::ApplyLayout()
{
    int iter = 200;
    float W = (float)w();
    float H = (float)h();
    float k = 180.0f;
    float temp = 100.0f;

    float centerX = 400.0f;
    float centerY = 300.0f;

    for (auto& v : vertices)
    {
        v.x = centerX + (rand() % 400 - 200);
        v.y = centerY + (rand() % 400 - 200);
    }

    for (int step = 0; step < iter; ++step)
    {
        for (size_t i = 0; i < vertices.size(); ++i)
        {
            vertices[i].dx = 0; vertices[i].dy = 0;
            vertices[i].dx += (centerX - vertices[i].x) * 0.01f;
            vertices[i].dy += (centerY - vertices[i].y) * 0.01f;

            for (size_t j = 0; j < vertices.size(); ++j)
            {
                if (i == j)
                    continue;

                float dx = vertices[i].x - vertices[j].x;
                float dy = vertices[i].y - vertices[j].y;
                float dist = sqrt(dx * dx + dy * dy) + 0.01f;
                if (dist < k * 4)
                {
                    float force = (k * k) / dist;
                    vertices[i].dx += (dx / dist) * force;
                    vertices[i].dy += (dy / dist) * force;
                }
            }
        }

        for (const auto& e : edges)
        {
            float dx = vertices[e.fromIdx].x - vertices[e.toIdx].x;
            float dy = vertices[e.fromIdx].y - vertices[e.toIdx].y;
            float dist = sqrt(dx * dx + dy * dy) + 0.01f;
            float force = (dist * dist) / k;
            float fx = (dx / dist) * force;
            float fy = (dy / dist) * force;
            vertices[e.fromIdx].dx -= fx;
            vertices[e.fromIdx].dy -= fy;
            vertices[e.toIdx].dx += fx;
            vertices[e.toIdx].dy += fy;
        }

        for (auto& v : vertices)
        {
            float dist = sqrt(v.dx * v.dx + v.dy * v.dy) + 0.01f;
            float limited_dist = (min)(dist, temp);
            v.x += (v.dx / dist) * limited_dist;
            v.y += (v.dy / dist) * limited_dist;

            v.x = (max)(10.0f, (min)(v.x, W - v.w - 10));
            v.y = (max)(10.0f, (min)(v.y, H - v.h - 10));
        }
        temp *= 0.95f;
    }

    redraw();
}

bool GraphCanvas::HasAnySelection()
{
    for (const auto& v : vertices)
    {
        if (v.visible && v.selected)
            return true;
    }

    for (const auto& g : groups)
    {
        if (g.selected)
            return true;
    }

    return false;
}

void GraphCanvas::GetReferencePoint(int nodeIdx, bool isStart, int cellIdx, int& outX, int& outY)
{
    const auto& v = vertices[nodeIdx];
    if (v.parentGroupIdx != -1)
    {
        const auto& g = groups[v.parentGroupIdx];
        outX = g.x + g.currentW / 2;
        outY = g.y + g.h / 2;
    }
    else
    {
        outX = isStart ? (int)(v.x + v.w) : (int)v.x;
        outY = isStart ? (int)(v.y + (cellIdx * v.cellH) + v.cellH / 2) : (int)(v.y + 10);
    }
}

void GraphCanvas::draw()
{
    fl_color(FL_WHITE);
    fl_rectf(x(), y(), w(), h());

    fl_push_clip(x(), y(), w(), h());

    // отрисовка ребер
    fl_color(FL_DARK_CYAN);
    for (const auto& e : edges)
    {
        if (vertices[e.fromIdx].parentGroupIdx != -1 &&
            vertices[e.fromIdx].parentGroupIdx == vertices[e.toIdx].parentGroupIdx)
            continue;

        int sxRaw, syRaw, exRaw, eyRaw;
        GetReferencePoint(e.fromIdx, true, e.cellIdx, sxRaw, syRaw);
        GetReferencePoint(e.toIdx, false, 0, exRaw, eyRaw);

        int sx = sxRaw + x(), sy = syRaw + y(), ex = exRaw + x(), ey = eyRaw + y();

        fl_line(sx, sy, ex, ey);
        double a = atan2(ey - sy, ex - sx);
        fl_begin_polygon();
        fl_vertex(ex, ey);
        fl_vertex(ex - 10 * cos(a - 0.4), ey - 10 * sin(a - 0.4));
        fl_vertex(ex - 10 * cos(a + 0.4), ey - 10 * sin(a + 0.4));
        fl_end_polygon();
    }

    // отрисовка групп
    for (auto& g : groups)
    {
        string label = g.name.empty() ? to_string(g.nodeIndices.size()) : g.name;
        fl_font(FL_HELVETICA_BOLD, 12);

        // измерение ширины текста и добавление отступов
        float text_w = fl_width(label.c_str());
        g.currentW = (max)(80.0f, text_w + 40.0f);

        int gx = g.x + x(), gy = g.y + y();
        float W = g.currentW;
        float H = g.h;
        float side = H / 2.0f;

        // рисуем шестиугольник для группы
        fl_color(g.selected ? fl_lighter(FL_MAGENTA) : FL_DARK_MAGENTA);
        fl_begin_complex_polygon();
        fl_vertex(gx + side, gy);
        fl_vertex(gx + W - side, gy);
        fl_vertex(gx + W, gy + H / 2);
        fl_vertex(gx + W - side, gy + H);
        fl_vertex(gx + side, gy + H);
        fl_vertex(gx, gy + H / 2);
        fl_end_complex_polygon();

        // отрисовка контура для выделенной группы
        if (g.selected)
        {
            fl_color(FL_RED);
            fl_line_style(FL_SOLID, 2);
            fl_begin_loop();
            fl_vertex(gx + side, gy);
            fl_vertex(gx + W - side, gy);
            fl_vertex(gx + W, gy + H / 2);
            fl_vertex(gx + W - side, gy + H);
            fl_vertex(gx + side, gy + H);
            fl_vertex(gx, gy + H / 2);
            fl_end_loop();
            fl_line_style(0);
        }

        fl_color(FL_WHITE);
        fl_draw(label.c_str(), gx, gy, (int)W, (int)H, FL_ALIGN_CENTER);
    }

    // отрисовка вершин
    for (auto& v : vertices)
    {
        if (!v.visible)
            continue;

        int vx = (int)v.x + x(), vy = (int)v.y + y();

        // пропускаем отрисовку, если узел вне видимой области
        if (vx + v.w < x() || vx > x() + w() || vy + v.h < y() || vy > y() + h())
            continue;

        fl_color(v.selected ? fl_lighter(FL_YELLOW) : FL_LIGHT3);
        fl_rectf(vx, vy, v.w, v.h);
        fl_color(v.selected ? FL_RED : FL_BLACK);
        fl_rect(vx, vy, v.w, v.h);
        for (size_t i = 0; i < v.data.content.size(); ++i)
        {
            int row_y = vy + (int)i * v.cellH;
            fl_rect(vx, row_y, v.w, v.cellH);
            fl_draw(v.data.content[i].c_str(), vx + 5, row_y, v.w - 10, v.cellH, FL_ALIGN_LEFT);
        }
    }

    // рамка выделения
    if (isSelecting)
    {
        fl_color(FL_BLUE); fl_line_style(FL_DOT);
        fl_rect((min)(selX1, selX2) + x(), (min)(selY1, selY2) + y(), abs(selX2 - selX1), abs(selY2 - selY1));
        fl_line_style(0);
    }
    fl_pop_clip();
}

void GraphCanvas::CreateGroupFromSelected()
{
    Group newGroup;
    vector<int> combinedNodes;
    float sumX = 0, sumY = 0;

    for (size_t i = 0; i < vertices.size(); ++i)
    {
        if (vertices[i].selected && vertices[i].visible)
        {
            combinedNodes.push_back((int)i);
            sumX += vertices[i].x; sumY += vertices[i].y;
        }
    }

    auto it = groups.begin();
    while (it != groups.end())
    {
        if (it->selected)
        {
            for (int idx : it->nodeIndices)
            {
                combinedNodes.push_back(idx);
            }

            sumX += it->x; sumY += it->y;
            it = groups.erase(it);
        }
        else
        {
            ++it;
        }
    }

    if (!combinedNodes.empty())
    {
        newGroup.nodeIndices = combinedNodes;
        newGroup.x = sumX / (combinedNodes.size() * 0.2f + 1);
        float real_x = 0, real_y = 0;
        for (int idx : combinedNodes)
        {
            real_x += vertices[idx].x;
            real_y += vertices[idx].y;
        }

        newGroup.x = real_x / combinedNodes.size();
        newGroup.y = real_y / combinedNodes.size();

        int new_idx = (int)groups.size();
        for (int idx : combinedNodes)
        {
            vertices[idx].visible = false; vertices[idx].selected = false;
            vertices[idx].parentGroupIdx = new_idx;
        }

        groups.push_back(newGroup);
    }

    ReindexGroups();
    redraw();
}

void GraphCanvas::ReindexGroups()
{
    for (size_t i = 0; i < groups.size(); ++i)
    {
        for (int ni : groups[i].nodeIndices)
            vertices[ni].parentGroupIdx = (int)i;
    }
}

void GraphCanvas::Ungroup(int group_idx)
{
    for (int nodeIdx : groups[group_idx].nodeIndices)
    {
        vertices[nodeIdx].visible = true;
        vertices[nodeIdx].parentGroupIdx = -1;
        vertices[nodeIdx].x = groups[group_idx].x + (rand() % 60 - 30);
        vertices[nodeIdx].y = groups[group_idx].y + (rand() % 60 - 30);
    }

    groups.erase(groups.begin() + group_idx);

    ReindexGroups();
    redraw();
}

int GraphCanvas::handle(int event)
{
    // Перевод координат мыши в локальные координаты холста
    int mx = Fl::event_x() - x();
    int my = Fl::event_y() - y();

    switch (event)
    {
    case FL_PUSH:
        if (Fl::event_button() == FL_RIGHT_MOUSE)
        {
            for (size_t i = 0; i < groups.size(); ++i)
            {
                if (groups[i].isInside(mx, my))
                {
                    Ungroup((int)i);
                    return 1;
                }
            }

            if (HasAnySelection())
            {
                ShowContextMenu(Fl::event_x(), Fl::event_y());
                return 1;
            }

            return 1;
        }

        if (Fl::event_button() == FL_LEFT_MOUSE)
        {
            if (Fl::event_clicks() > 0)
            {
                for (auto& g : groups)
                {
                    if (g.isInside(mx, my))
                    {
                        const char* newName = fl_input("Введите имя группы:", g.name.c_str());
                        if (newName)
                        {
                            g.name = newName;
                        }

                        Fl::event_clicks(0);
                        redraw();

                        return 1;
                    }
                }
            }

            for (auto& g : groups)
            {
                if (g.isInside(mx, my))
                {
                    if (!g.selected)
                    {
                        for (auto& v : vertices)
                            v.selected = false;

                        for (auto& og : groups)
                            og.selected = false;

                        g.selected = true;
                    }

                    isMovingSelection = true;
                    lastMx = mx;
                    lastMy = my;
                    redraw();

                    return 1;
                }
            }

            for (auto& v : vertices)
            {
                if (v.isInside(mx, my))
                {
                    if (!v.selected)
                    {
                        for (auto& other : vertices)
                            other.selected = false;

                        for (auto& og : groups)
                            og.selected = false;

                        v.selected = true;
                    }

                    isMovingSelection = true;
                    lastMx = mx;
                    lastMy = my;
                    redraw();

                    return 1;
                }
            }

            isSelecting = true;
            selX1 = selX2 = mx;
            selY1 = selY2 = my;

            for (auto& v : vertices)
                v.selected = false;

            for (auto& g : groups)
                g.selected = false;

            redraw();

            return 1;
        }

        return 1;

    case FL_DRAG:
        if (isSelecting)
        {
            selX2 = mx;
            selY2 = my;
            int rx = (min)(selX1, selX2), ry = (min)(selY1, selY2);
            int rw = abs(selX2 - selX1), rh = abs(selY2 - selY1);

            for (auto& v : vertices)
            {
                if (v.visible)
                    v.selected = (v.x >= rx && v.x + v.w <= rx + rw && v.y >= ry && v.y + v.h <= ry + rh);
            }

            for (auto& g : groups)
            {
                g.selected = (g.x >= rx && g.x + g.size <= rx + rw && g.y >= ry && g.y + g.size <= ry + rh);
            }

            redraw();
        }
        else if (isMovingSelection)
        {
            int dx = mx - lastMx;
            int dy = my - lastMy;

            for (auto& v : vertices)
            {
                if (v.selected && v.visible)
                {
                    v.x += dx;
                    v.y += dy;
                }
            }

            for (auto& g : groups)
            {
                if (g.selected)
                {
                    g.x += dx;
                    g.y += dy;
                }
            }

            lastMx = mx;
            lastMy = my;

            redraw();
        }

        return 1;

    case FL_RELEASE:

        isSelecting = false;
        isMovingSelection = false;
        redraw();

        return 1;
    }

    return Fl_Widget::handle(event);
}

void GraphCanvas::ShowContextMenu(int mx, int my)
{
    static Fl_Menu_Item menu[] = {
        {"Сгруппировать", 0, [](Fl_Widget* w, void* d) { ((GraphCanvas*)w)->CreateGroupFromSelected(); }},
        {0}
    };
    const Fl_Menu_Item* picked = menu->popup(mx, my);
    if (picked)
        picked->do_callback(this, nullptr);
}