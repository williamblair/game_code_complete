#include "AITeapotView.h"


AITeapotView::AITeapotView(std::shared_ptr<PathingGraph> pPathingGraph) :
    IGameView(),
    m_pPathingGraph(pPathingGraph)
{
}

AITeapotView::~AITeapotView()
{
    GCC_LOG("AI", "Destroying AITeapotView");
}


