// #include "stdafx.h"

#include <FL/Fl.H>
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

#include "GraphDisplay.h"
#include "GraphCanvas.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <future>
#include <stack>
#include <unordered_set>
#include <queue>

#include <windows.h>
#include <commdlg.h>


class HierarchicalList : public Fl_Tree
{
public:
    HierarchicalList(int X, int Y, int W, int H) : Fl_Tree(X, Y, W, H), indexMap(nullptr) {}

    int handle(int event) override
    {
        if (event == FL_PUSH)
        {
            if (Fl::event_clicks())
            {
                Fl_Tree_Item* item = callback_item();
                if (!item)
                    return Fl_Tree::handle(event);

                string label = item->label();

                auto pos = label.find("IC_");
                if (pos != string::npos && indexMap)
                {
                    int index = stoi(label.substr(pos + 3));

                    if (indexMap->count(index))
                    {
                        Fl_Tree_Item* target = (*indexMap)[index];

                        select(target);
                        show_item(target);
                        target->open();
                    }
                }

                return 1;
            }
        }

        return Fl_Tree::handle(event);
    }

    unordered_map<int, Fl_Tree_Item*>* indexMap;
};


class IconButton : public Fl_Button
{
    Fl_Image* icon = nullptr;

public:
    IconButton(int X, int Y, int W, int H)
        : Fl_Button(X, Y, W, H)
    {
        box(FL_FLAT_BOX);
    }

    void setIcon(Fl_Image* img)
    {
        icon = img;
        redraw();
    }

    void draw() override
    {
        Fl_Button::draw();

        int dx = 0;
        int dy = 0;

        if (Fl::pushed() == this)
        {
            dx = 1;
            dy = 1;
        }

        if (icon)
        {
            int ix = x() + (w() - icon->w()) / 2 + dx;
            int iy = y() + (h() - icon->h()) / 2 + dy;

            icon->draw(ix, iy);
        }
    }
};

struct GraphvizContext
{
    GraphDisplay* self;
    Fl_Choice* engineChoice;
    Fl_Scroll* scrollArea;
    Fl_Box* imageBox;
    Fl_PNG_Image* originalImage = nullptr;
    float zoomValue = 1.0f;

    string dotFile = "tmp_graph.dot";
    string svgFile = "tmp_graph.svg";
    string pngFile = "tmp_graph.png";
};

void GenerateGraphvizOnce(GraphvizContext* ctx)
{
    string dotContent = ctx->self->GenerateDot();
    if (dotContent.empty())
        return;

    ofstream out(ctx->dotFile);
    out << dotContent;
    out.close();

    const char* engine = ctx->engineChoice->text();

    string svgCommand =
        string(engine) +
        " -Gstart=1"
        " -Goverlap=false"
        " -Tsvg " + ctx->dotFile +
        " -o " + ctx->svgFile;

    system(svgCommand.c_str());
}

void RenderPng(GraphvizContext* ctx)
{
    string resvgCommand =
        "resvg "
        "-z " + to_string(ctx->zoomValue) + " "
        + ctx->svgFile + " "
        + ctx->pngFile;

    system(resvgCommand.c_str());

    if (ctx->imageBox->image())
        delete ctx->imageBox->image();

    Fl_PNG_Image* img = new Fl_PNG_Image(ctx->pngFile.c_str());
    ctx->imageBox->size(img->w(), img->h());
    ctx->imageBox->image(img);

    ctx->scrollArea->redraw();
}

void RefreshGraphvizCallback(Fl_Widget*, void* data)
{
    auto* ctx = static_cast<GraphvizContext*>(data);

    GenerateGraphvizOnce(ctx);

    RenderPng(ctx);
}

void ZoomInCallback(Fl_Widget* w, void* data)
{
    auto* ctx = static_cast<GraphvizContext*>(data);

    if (ctx->zoomValue < 5.0f)
        ctx->zoomValue *= 1.2f;

    RenderPng(ctx);
}

void ZoomOutCallback(Fl_Widget* w, void* data)
{
    auto* ctx = static_cast<GraphvizContext*>(data);

    if (ctx->zoomValue > 0.1f)
        ctx->zoomValue /= 1.2f;

    RenderPng(ctx);
}

void HighlightItem(Fl_Tree* tree, Fl_Tree_Item* item)
{
    if (!item) return;

    for (auto it = tree->first(); it; it = tree->next(it))
    {
        it->deselect();
        it->labelcolor(FL_BLACK);
        it->labelfont(FL_HELVETICA);
    }

    Fl_Tree_Item* parent = item->parent();
    while (parent)
    {
        parent->open();
        parent = parent->parent();
    }

    item->select();
    item->labelcolor(FL_RED);
    item->labelfont(FL_HELVETICA_BOLD);

    tree->show_item(item);

    tree->redraw();
    Fl::flush();
}


void FindInTree(Fl_Tree* tree, const string& text, SearchState& state)
{
    state.results.clear();
    state.currentIndex = -1;

    if (text.empty()) return;

    Fl_Tree_Item* item = tree->first();
    while (item)
    {
        if (item->label() && string(item->label()).find(text) != string::npos)
        {
            state.results.push_back(item);
        }
        item = tree->next(item);
    }

    if (!state.results.empty())
        state.currentIndex = 0;
}

void UpdateNavigationButtons(Fl_Button* nextBtn, Fl_Button* prevBtn, const SearchState& state)
{
    if (state.results.size() <= 1)
    {
        nextBtn->deactivate();
        prevBtn->deactivate();
    }
    else
    {
        nextBtn->activate();
        prevBtn->activate();
    }
}

void UpdateSearchFieldStyle(Fl_Input* input, bool hasResults)
{
    if (hasResults)
    {
        input->color(FL_WHITE);
        input->textcolor(FL_BLACK);
    }
    else
    {
        input->box(FL_BORDER_BOX);
        input->color(hasResults ? FL_WHITE : fl_rgb_color(255, 200, 200));
    }

    input->redraw();
}

void FindButtonCallback(Fl_Widget* widget, void* data)
{
    auto* ctx = static_cast<tuple<
        Fl_Tree*, Fl_Input*, Fl_Button*, Fl_Button*, SearchState*
    >*>(data);

    Fl_Tree* tree = get<0>(*ctx);
    Fl_Input* input = get<1>(*ctx);
    Fl_Button* nextBtn = get<2>(*ctx);
    Fl_Button* prevBtn = get<3>(*ctx);
    SearchState* state = get<4>(*ctx);

    FindInTree(tree, input->value(), *state);

    bool hasResults = !state->results.empty();

    UpdateSearchFieldStyle(input, hasResults);
    UpdateNavigationButtons(nextBtn, prevBtn, *state);

    if (hasResults)
    {
        HighlightItem(tree, state->results[state->currentIndex]);
    }
    else
    {
        tree->redraw();
        Fl::flush();
    }

}

void NextButtonCallback(Fl_Widget* widget, void* data)
{
    auto* ctx = static_cast<tuple<Fl_Tree*, SearchState*>*>(data);

    Fl_Tree* tree = get<0>(*ctx);
    SearchState* state = get<1>(*ctx);

    if (state->results.empty()) return;

    state->currentIndex = (state->currentIndex + 1) % state->results.size();

    HighlightItem(tree, state->results[state->currentIndex]);
}

void PrevButtonCallback(Fl_Widget* widget, void* data)
{
    auto* ctx = static_cast<tuple<Fl_Tree*, SearchState*>*>(data);

    Fl_Tree* tree = get<0>(*ctx);
    SearchState* state = get<1>(*ctx);

    if (state->results.empty()) return;

    state->currentIndex--;

    if (state->currentIndex < 0)
        state->currentIndex = state->results.size() - 1;

    HighlightItem(tree, state->results[state->currentIndex]);
}

void SearchInputCallback(Fl_Widget* w, void* data)
{
    auto* input = static_cast<Fl_Input*>(w);
    input->color(FL_WHITE);
    input->textcolor(FL_BLACK);
    input->redraw();
};

void SaveGraphImageCallback(Fl_Widget*, void* data)
{
    auto* ctx = static_cast<GraphvizContext*>(data);

    char filename[MAX_PATH] = "graph.png";

    OPENFILENAMEA ofn{};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;

    ofn.lpstrFilter =
        "PNG Image (*.png)\0*.png\0"
        "All Files (*.*)\0*.*\0";

    ofn.nFilterIndex = 1;

    ofn.Flags =
        OFN_PATHMUSTEXIST |
        OFN_OVERWRITEPROMPT |
        OFN_NOCHANGEDIR;

    ofn.lpstrDefExt = "png";

    if (GetSaveFileNameA(&ofn))
    {
        try
        {
            filesystem::copy_file(
                ctx->pngFile,
                filename,
                filesystem::copy_options::overwrite_existing
            );
        }
        catch (const exception& ex)
        {
            fl_alert("Не удалось сохранить изображение.");
        }
    }
}


void GraphDisplay::ProgFU(long int MK, LoadPoint Load, FU* Sender)
{
    MK %= FUMkRange; // Оставить только свои МК
    switch (MK)
    {
    case 0: // Reset
        GraphUk = nullptr;
        break;
    case 1: // Set Установить указатель на ОА-граф
        if (!Load.isIC())
            return;

        GraphUk = (IC_type)Load.Point;
        DisplayGraphWindow();
        break;
    case 5: // ExtendSet Установить флаг разворачивания всех ветвей иерархического списка
        Extend = Load.toBool();
        break;
    case 10: // MnemoTableSet Установить ссылку на ФУ мнемоник или список мнемоник
        if (Load.isFU()) {
            mnemoToStr.MnemoList = (FU*)Load.Point;
            mnemoToStr.MnemoListExt = true; // Установить флаг внешнего ФУ списка лексем
        }
        else {
            mnemoToStr.MnemoList = new List(Bus, nullptr);
            mnemoToStr.MnemoList->ProgFU(1, Load, Sender); // Установка списка лексем в ФУ списка
            mnemoToStr.MnemoListExt = false; // Установить флаг внешнего ФУ списка лексем
        }
        break;
    default:
        CommonMk(MK, Load);
        break;
    }
}

void GraphDisplay::DisplayGraphWindow()
{
    if (GraphUk == nullptr)
        return;

    auto p = make_shared<promise<void>>();
    auto f = p->get_future();

    auto* ctx = new pair<GraphDisplay*, shared_ptr<promise<void>>>(
        this, p
    );

    Fl::awake([](void* data) {
        auto* ctx = static_cast<pair<GraphDisplay*, shared_ptr<promise<void>>>*>(data);

        GraphDisplay* self = ctx->first;
        auto promisePtr = ctx->second;

        delete ctx;

        Fl_Double_Window* window = new Fl_Double_Window(800, 600, "GraphDisplay");
        window->size_range(600, 450);

        Fl_Tabs* tabs = new Fl_Tabs(0, 0, 800, 600);
        window->resizable(tabs);

        // --- Вкладка 1: Иерархический список ---
        Fl_Group* listTab = new Fl_Group(0, 30, 800, 570, "Иерархический список");

        Fl_Group* statsGroup = new Fl_Group(10, 75, 780, 60);
        statsGroup->box(FL_FLAT_BOX);
        statsGroup->color(fl_rgb_color(240, 240, 240));
        Fl_Box* statsBox = new Fl_Box(15, 80, 770, 50);
        statsBox->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
        statsGroup->end();

        Fl_Input* searchInput = new Fl_Input(60, 40, 180, 25, "Поиск:");

        IconButton* findBtn = new IconButton(250, 40, 40, 25);
        IconButton* nextBtn = new IconButton(295, 40, 40, 25);
        IconButton* prevBtn = new IconButton(340, 40, 40, 25);

        findBtn->setIcon(new Fl_PNG_Image("search_icon.png"));
        nextBtn->setIcon(new Fl_PNG_Image("next_icon.png"));
        prevBtn->setIcon(new Fl_PNG_Image("prev_icon.png"));

        Fl_Tree* tree = self->CreateFlTree();
        tree->resize(10, 135, 780, 425); // Увеличено под 800x600

        listTab->resizable(tree);

        string statsText =
            "Всего ИК: " + to_string(self->graphStats.vertexCount) +
            " | Всего ИП: " + to_string(self->graphStats.pairCount) +
            " | Всего массивов: " + to_string(self->graphStats.arrayCount) +
            " | Макс. глубина: " + to_string(self->graphStats.maxDepth);
        statsBox->copy_label(statsText.c_str());

        findBtn->callback(FindButtonCallback, new tuple<Fl_Tree*, Fl_Input*, Fl_Button*, Fl_Button*, SearchState*>(tree, searchInput, nextBtn, prevBtn, &self->searchState));
        nextBtn->callback(NextButtonCallback, new tuple<Fl_Tree*, SearchState*>(tree, &self->searchState));
        prevBtn->callback(PrevButtonCallback, new tuple<Fl_Tree*, SearchState*>(tree, &self->searchState));

        for (auto btn : { findBtn, nextBtn, prevBtn })
        {
            btn->label("");
            btn->box(FL_PLASTIC_UP_BOX);
            btn->color(fl_rgb_color(245, 245, 245));
            btn->selection_color(fl_rgb_color(220, 220, 220));
        }

        nextBtn->deactivate();
        prevBtn->deactivate();

        tree->root()->open();
        listTab->end();

        // --- Вкладка 2: Graphviz ---
        Fl_Group* graphvizTab = new Fl_Group(0, 30, 800, 570, "Graphviz");

        Fl_Group* gvToolbar = new Fl_Group(10, 40, 780, 35);
        gvToolbar->box(FL_ENGRAVED_FRAME);

        Fl_Choice* engineChoice = new Fl_Choice(75, 45, 80, 25, "Движок:");
        engineChoice->add("dot");
        engineChoice->add("fdp");
        engineChoice->add("sfdp");
        engineChoice->value(0);

        Fl_Button* refreshBtn = new Fl_Button(165, 45, 110, 25, "@reload Обновить");

        Fl_Button* zoomInBtn = new Fl_Button(285, 45, 35, 25, "+");
        zoomInBtn->tooltip("Приблизить");

        Fl_Button* zoomOutBtn = new Fl_Button(325, 45, 35, 25, "-");
        zoomOutBtn->tooltip("Отдалить");

        Fl_Button* saveBtn = new Fl_Button(370, 45, 110, 25, "@filesave Сохранить");

        gvToolbar->end();

        Fl_Scroll* scroll = new Fl_Scroll(10, 80, 780, 480); // Увеличено под 800x600
        scroll->box(FL_DOWN_BOX);
        scroll->color(FL_WHITE);
        scroll->type(Fl_Scroll::BOTH);

        Fl_Box* imageBox = new Fl_Box(10, 80, 0, 0);
        imageBox->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE);

        scroll->end();
        graphvizTab->resizable(scroll);

        GraphvizContext* gvCtx = new GraphvizContext;
        gvCtx->self = self;
        gvCtx->engineChoice = engineChoice;
        gvCtx->scrollArea = scroll;
        gvCtx->imageBox = imageBox;
        gvCtx->zoomValue = 1.0f;

        refreshBtn->callback(RefreshGraphvizCallback, gvCtx);
        engineChoice->callback(RefreshGraphvizCallback, gvCtx);
        zoomInBtn->callback(ZoomInCallback, gvCtx);
        zoomOutBtn->callback(ZoomOutCallback, gvCtx);
        saveBtn->callback(SaveGraphImageCallback, gvCtx);

        graphvizTab->end();

        Fl_Group* customTab = new Fl_Group(0, 30, 800, 570, "Интерактивная визуализация");

        LayoutBridge bridge;
        GraphCanvas::PrepareCanvasData(self->GraphUk, bridge, self->mnemoToStr);

        Fl_Button* layoutBtn = new Fl_Button(10, 35, 140, 25, "@refresh Авто-раскладка");

        Fl_Scroll* scroll2 = new Fl_Scroll(0, 65, 800, 505);
        scroll2->type(Fl_Scroll::BOTH);
        scroll2->box(FL_DOWN_BOX);

        GraphCanvas* canvas = new GraphCanvas(0, 65, 2500, 2500, bridge.nodes, bridge.edges);

        scroll2->end();
        customTab->resizable(scroll2); // Исправлено: ресайз на правильный скролл

        layoutBtn->callback([](Fl_Widget* w, void* d) {
            ((GraphCanvas*)d)->ApplyLayout();
            }, canvas);

        customTab->end();

        tabs->end();

        window->callback([](Fl_Widget* w, void* pdata) {
            auto p = static_cast<shared_ptr<promise<void>>*>(pdata);
            (*p)->set_value();
            delete p;
            delete w;
            }, new shared_ptr<promise<void>>(promisePtr));

        window->end();
        window->show();

        RefreshGraphvizCallback(nullptr, gvCtx);

        }, ctx);

    f.wait();
}


FU* GraphDisplay::Copy()
{
    GraphDisplay* copy = new GraphDisplay(Bus, this);
    copy->GraphUk = GraphUk;
    copy->Extend = Extend;
    return copy;
}

FU* GraphDisplay::TypeCopy()
{
    return new GraphDisplay(Bus, nullptr);
}

void AddLoadPointToTree(
    Fl_Tree* tree,
    const string& basePath,
    LoadPoint& lp,
    unordered_map<IC_type, string>& definedICs,
    unordered_map<int, Fl_Tree_Item*>& indexMap,
    int& icCounter,
    LoadMnemoToStr& mnemoToStr,
    bool isLastArrayElement = false)
{
    string path = basePath;

    if (lp.isConst())
        path += " # ";
    else
        path += " = ";

    if (lp.isIC())
    {
        IC_type ic = lp.IC();

        string icName;
        int icIndex;

        if (definedICs.count(ic))
        {
            icName = definedICs[ic];
            icIndex = stoi(icName.substr(3));

            auto item = tree->add((path + icName).c_str());
            item->labelcolor(FL_BLUE);

            return;
        }

        icIndex = icCounter;
        icName = "IC_" + to_string(icCounter++);
        definedICs[ic] = icName;

        string icPath = path + icName;

        auto item = tree->add(icPath.c_str());

        indexMap[icIndex] = item;

        for (auto& ip : *ic)
        {
            string childPath = icPath + "/" + mnemoToStr.AtrConv(ip.atr);

            AddLoadPointToTree(
                tree,
                childPath,
                ip.Load,
                definedICs,
                indexMap,
                icCounter,
                mnemoToStr
            );
        }

        return;
    }

    if (lp.getType() >> 1 == DLoadArray)
    {
        path += "Array [";
        tree->add(path.c_str());

        LoadVect_type arr = lp.LoadVect();

        for (int i = 0; i < arr->size(); ++i)
        {
            bool isLast = i == arr->size() - 1;

            string pathToNode = path + "/i " + to_string(i);

            AddLoadPointToTree(
                tree,
                pathToNode,
                (*arr)[i],
                definedICs,
                indexMap,
                icCounter,
                mnemoToStr,
                isLast
            );
        }

        return;
    }

    path += mnemoToStr.LoadConv(lp);

    if (isLastArrayElement)
        path += " ]";

    tree->add(path.c_str());
}



void TraverseLoadPoint(
    LoadPoint& lp,
    unordered_map<IC_type, size_t>& depthMap,
    queue<pair<IC_type, size_t>>& q,
    GraphStats& stats,
    size_t depth)
{
    if (lp.isIC())
    {
        IC_type ic = lp.IC();

        if (!depthMap.count(ic) || depthMap[ic] > depth + 1)
        {
            depthMap[ic] = depth + 1;
            q.push({ ic, depth + 1 });
        }

        return;
    }

    if (lp.getType() >> 1 == DLoadArray)
    {
        stats.arrayCount++;

        LoadVect_type arr = lp.LoadVect();

        for (auto& child : *arr)
        {
            TraverseLoadPoint(child, depthMap, q, stats, depth);
        }

        return;
    }

    stats.pairCount++;
}

ICVect GraphDisplay::TraverseGraph(IC_type graphUk, GraphStats& stats)
{
    ICVect graph;

    queue<pair<IC_type, size_t>> q;
    unordered_map<IC_type, size_t> depthMap;

    q.push({ graphUk, 0 });
    depthMap[graphUk] = 0;

    while (!q.empty())
    {
        auto [currentNode, depth] = q.front();
        q.pop();

        graph.push_back(currentNode);

        stats.vertexCount++;
        stats.maxDepth = (max)(stats.maxDepth, depth);

        for (auto& ip : *currentNode)
        {
            if (ip.Load.isIC())
            {
                stats.pairCount++;

                IC_type ic = ip.Load.IC();

                if (!depthMap.count(ic) || depthMap[ic] > depth + 1)
                {
                    depthMap[ic] = depth + 1;
                    q.push({ ic, depth + 1 });
                }
            }
            else if (ip.Load.getType() >> 1 == DLoadArray)
            {
                stats.arrayCount++;

                LoadVect_type arr = ip.Load.LoadVect();

                for (auto& lp : *arr)
                {
                    TraverseLoadPoint(lp, depthMap, q, stats, depth);
                }
            }
            else
            {
                stats.pairCount++;
            }
        }
    }

    return graph;
}

Fl_Tree* GraphDisplay::CreateFlTree()
{
    auto tree = new HierarchicalList(10, 10, 580, 380);
    tree->root_label("Семантическая сеть");

    TraverseGraph(GraphUk, graphStats);

    tree->indexMap = &treeIcMap;

    unordered_map<IC_type, string> definedICs;
    int icCounter = 0;

    string rootName = "IC_" + to_string(icCounter++);
    definedICs[GraphUk] = rootName;

    auto rootItem = tree->add(rootName.c_str());

    treeIcMap[0] = rootItem;

    for (auto& ip : *GraphUk)
    {
        string path = rootName + "/" + mnemoToStr.AtrConv(ip.atr);

        AddLoadPointToTree(
            tree,
            path,
            ip.Load,
            definedICs,
            treeIcMap,
            icCounter,
            mnemoToStr
        );
    }

    return tree;
}


// GRAPHVIZ

struct DotEdge
{
    string fromNode;
    string fromPort;
    string toNode;
};

string Escape(const string& s)
{
    string out;
    for (char c : s)
    {
        switch (c)
        {
        case '<': out += "&lt;"; break;
        case '>': out += "&gt;"; break;
        case '&': out += "&amp;"; break;
        default: out += c;
        }
    }
    return out;
}

string GenerateLoadPointDot(
    LoadPoint& lp,
    const vector<IC_type>& graph,
    vector<DotEdge>& edges,
    string& extraNodes,
    const string& currentNode,
    int& portCounter,
    const string& prefix,
    LoadMnemoToStr mnemoToStr)
{
    string port = "p" + to_string(portCounter++);
    string label = prefix;
    label += lp.isConst() ? " # " : " = ";

    if (lp.isIC())
    {
        IC_type ic = lp.IC();
        auto it = find(graph.begin(), graph.end(), ic);

        string target = (it != graph.end())
            ? "IC_" + to_string(distance(graph.begin(), it))
            : "unknown";

        label += target;

        edges.emplace_back(currentNode, port, target);

        return "<tr><td port=\"" + port + "\">" + Escape(label) + "</td></tr>\n";
    }

    if (lp.getType() >> 1 == DLoadArray)
    {
        string arrayNode = currentNode + "_arr_" + to_string(portCounter++);

        extraNodes += arrayNode + " [shape=none label=<\n";
        extraNodes += "<table border=\"1\" cellborder=\"1\" cellspacing=\"0\">\n";

        LoadVect_type arr = lp.LoadVect();

        int innerPort = 0;

        for (int i = 0; i < arr->size(); ++i)
        {
            string childPrefix = "i " + to_string(i);

            extraNodes += GenerateLoadPointDot(
                (*arr)[i],
                graph,
                edges,
                extraNodes,
                arrayNode,
                innerPort,
                childPrefix,
                mnemoToStr
            );
        }

        extraNodes += "</table>>];\n";

        edges.emplace_back(currentNode, port, arrayNode);

        return "<tr><td port=\"" + port + "\">" + Escape(label + "Array") + "</td></tr>\n";
    }

    label += mnemoToStr.LoadConv(lp);

    return "<tr><td port=\"" + port + "\">" + Escape(label) + "</td></tr>\n";
}

string GenerateICNodeDot(
    IC_type ic,
    int index,
    const vector<IC_type>& graph,
    vector<DotEdge>& edges,
    string& extraNodes,
    LoadMnemoToStr& mnemoToStr)
{
    string nodeName = "IC_" + to_string(index);

    string dot = nodeName + " [shape=none label=<\n";
    dot += "<table border=\"1\" cellborder=\"1\" cellspacing=\"0\">\n";

    dot += "<tr><td bgcolor=\"lightgray\"><b>" + nodeName + "</b></td></tr>\n";

    int portCounter = 0;

    for (auto& ip : *ic)
    {
        string prefix = mnemoToStr.AtrConv(ip.atr);

        dot += GenerateLoadPointDot(
            ip.Load,
            graph,
            edges,
            extraNodes,
            nodeName,
            portCounter,
            prefix,
            mnemoToStr
        );
    }

    dot += "</table>>];\n";

    return dot;
}

string GraphDisplay::GenerateDot()
{
    ICVect graph = TraverseGraph(GraphUk, graphStats);

    vector<DotEdge> edges;
    string extraNodes;
    string dot = "digraph G {\n";
    dot += "node [shape=plaintext];\n";

    // IC узлы
    for (int i = 0; i < graph.size(); ++i)
    {
        dot += GenerateICNodeDot(graph[i], i, graph, edges, extraNodes, mnemoToStr);
    }

    // массивы (отдельные узлы)
    dot += extraNodes;

    // ребра
    for (auto& e : edges)
    {
        dot += e.fromNode + ":" + e.fromPort + " -> " + e.toNode + ";\n";
    }

    dot += "}\n";

    return dot;
}