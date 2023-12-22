#include <cfloat>
#include "GameCodeApp.h"
#include "PathingGraph.h"

GCC_MEMORYPOOL_DEFINITION(PathingNode);
GCC_MEMORYPOOL_AUTOINIT(PathingNode, 128); // currently 81 generated; if more desired, increase this


void PathingNode::AddArc(PathingArc* pArc)
{
    assert(pArc);
    m_arcs.push_back(pArc);
}

void PathingNode::GetNeighbors(PathingNodeList& outNeighbors)
{
    for (auto it = m_arcs.begin(); it != m_arcs.end(); ++it) {
        PathingArc* pArc = *it;
        outNeighbors.push_back(pArc->GetNeighbor(this));
    }
}

float PathingNode::GetCostFromNode(PathingNode* pFromNode)
{
    assert(pFromNode);
    PathingArc* pArc = FindArc(pFromNode);
    assert(pArc);
    Vec3 diff = pFromNode->GetPos() - m_pos;
    //return pArc->GetWeight() * diff.Length();
    return pArc->GetWeight() * Mag(diff);
}

PathingArc* PathingNode::FindArc(PathingNode* pLinkedNode)
{
    assert(pLinkedNode);
    for (auto it = m_arcs.begin(); it != m_arcs.end(); ++it) {
        PathingArc* pArc = *it;
        if (pArc->GetNeighbor(this) == pLinkedNode) {
            return pArc;
        }
    }
    return nullptr;
}




void PathingArc::LinkNodes(PathingNode* pNodeA, PathingNode* pNodeB)
{
    assert(pNodeA);
    assert(pNodeB);

    m_pNodes[0] = pNodeA;
    m_pNodes[1] = pNodeB;
}

PathingNode* PathingArc::GetNeighbor(PathingNode* pMe)
{
    assert(pMe);
    if (m_pNodes[0] == pMe) { return m_pNodes[1]; }
    /*else { */return m_pNodes[0];/* }*/
}



bool PathPlan::CheckForNextNode(const Vec3& pos)
{
    if (m_index == m_path.end()) {
        return false;
    }

    Vec3 diff = pos - (*m_index)->GetPos();
    if (Mag(diff) <= (*m_index)->GetTolerance()) {
        ++m_index;
        return true;
    }
    return false;
}

bool PathPlan::CheckForEnd()
{
    if (m_index == m_path.end()) {
        return true;
    }
    return false;
}

void PathPlan::AddNode(PathingNode* pNode)
{
    assert(pNode);
    m_path.push_front(pNode);
}


PathPlanNode::PathPlanNode(
    PathingNode* pNode,
    PathPlanNode* pPrevNode,
    PathingNode* pGoalNode)
{
    assert(pNode);
    m_pPathingNode = pNode;
    m_pPrev = pPrevNode;
    m_pGoalNode = pGoalNode;
    m_closed = false;
    UpdateHeuristics();
}

void PathPlanNode::UpdatePrevNode(PathPlanNode* pPrev)
{
    assert(pPrev);
    m_pPrev = pPrev;
    UpdateHeuristics();
}

void PathPlanNode::UpdateHeuristics()
{
    // total cost (g)
    if (m_pPrev) {
        m_goal =
            m_pPrev->GetGoal() +
            m_pPathingNode->GetCostFromNode(
                m_pPrev->GetPathingNode());
    } else {
        m_goal = 0;
    }

    // heuristic (h)
    Vec3 diff = m_pPathingNode->GetPos() - m_pGoalNode->GetPos();
    m_heuristic = Mag(diff);

    // cost to goal (f)
    m_fitness = m_goal + m_heuristic;
}



AStar::AStar()
{
    m_pStartNode = nullptr;
    m_pGoalNode = nullptr;
}
AStar::~AStar()
{
    Destroy();
}

void AStar::Destroy()
{
    // destroy all the PathPlanNode objects and clear the map
    for (auto it = m_nodes.begin(); it != m_nodes.end(); ++it) {  
        delete it->second;
    }
    m_nodes.clear();

    // clear the open set
    m_openSet.clear();

    // clear the start and goal nodes
    m_pStartNode = nullptr;
    m_pGoalNode = nullptr;
}

PathPlan* AStar::operator()(PathingNode* pStartNode, PathingNode* pGoalNode)
{
    assert(pStartNode);
    assert(pGoalNode);

    // if the start and end nodes are the same, we're close enough to b-line to the goal
    if (pStartNode == pGoalNode) {
        return nullptr;
    }

    // set our members
    m_pStartNode = pStartNode;
    m_pGoalNode = pGoalNode;

    // the open set is a priority queue of nodes to be evaluated.
    // If its' ever empty, it means we couldn't find a path to the goal.
    // The start node is the only node that is initially in the open set.
    AddToOpenSet(m_pStartNode, nullptr);

    while (!m_openSet.empty())
    {
        // grab the most likely candidate
        PathPlanNode* pNode = m_openSet.front();

        // If this node is our goal node, we've successfully found a path
        if (pNode->GetPathingNode() == m_pGoalNode) {
            return RebuildPath(pNode);
        }

        // We're processing this node so remove from open set and add to closed
        m_openSet.pop_front();
        AddToClosedSet(pNode); 

        // get the neighboring nodes
        PathingNodeList neighbors;
        pNode->GetPathingNode()->GetNeighbors(neighbors);

        // loop through all neighboring nodes and evaluate each
        for (auto it = neighbors.begin(); it != neighbors.end(); ++it) {
            PathingNode* pNodeToEvaluate = *it;

            // Try and find a PathPlanNode object for this node
            auto findIt = m_nodes.find(pNodeToEvaluate);

            // If exists and in closed list, we've already evaluated.
            // We can safely skip it
            if (findIt != m_nodes.end() && findIt->second->IsClosed()) {
                continue;
            }

            // figure out the cost for this route through the node
            float costForThisPath =
                pNode->GetGoal() +
                pNodeToEvaluate->GetCostFromNode(pNode->GetPathingNode());

            bool isPathBetter = false;

            // Grab the PathPlanNode if there is one
            PathPlanNode* pPathPlanNodeToEvaluate = nullptr;
            if (findIt != m_nodes.end()) {
                pPathPlanNodeToEvaluate = findIt->second;
            }

            // No PathPlanNode means we've never evaluated this pathing node,
            // so we need to add it to the open set, which has the side effect
            // of setting all the heuristic data. It also means that this is the
            // best path through this node that we've found, so the nodes are
            // linked together, which is why we don't bother setting isPathBetter
            // to true - it's done for us in AddToOpenSet()
            if (!pPathPlanNodeToEvaluate) {
                pPathPlanNodeToEvaluate = AddToOpenSet(pNodeToEvaluate, pNode);
            }

            // If this node is already in the open set, check to see if this route
            // to it is better than the last
            else if (costForThisPath < pPathPlanNodeToEvaluate->GetGoal()) {
                isPathBetter = true;
            }

            // If this path is better, relink the nodes appropriately,
            // update the heuristics data, and reinster the node into the open
            // list priority queue
            if (isPathBetter) {
                pPathPlanNodeToEvaluate->UpdatePrevNode(pNode);
                ReinsertNode(pPathPlanNodeToEvaluate);
            }
        }
    }

    // No path to the goal
    return nullptr;
}

PathPlanNode* AStar::AddToOpenSet(PathingNode* pNode, PathPlanNode* pPrevNode)
{
    assert(pNode);

    // Create new PathPlanNode if necessary
    auto it = m_nodes.find(pNode);
    PathPlanNode* pThisNode = nullptr;
    if (it == m_nodes.end()) {
        pThisNode = new PathPlanNode(pNode, pPrevNode, m_pGoalNode);
        m_nodes.insert(std::make_pair(pNode,pThisNode));
    } else {
        GCC_WARNING("Adding existing PathPlanNode to open set");
        pThisNode = it->second;
        pThisNode->SetClosed(false);
    }

    // now insert it into the priority queue
    InsertNode(pThisNode);

    return pThisNode;
}

void AStar::AddToClosedSet(PathPlanNode* pNode)
{
    assert(pNode);
    pNode->SetClosed();
}

void AStar::InsertNode(PathPlanNode* pNode)
{
    assert(pNode);

    // just add the node if the open set is empty
    if (m_openSet.empty()) {
        m_openSet.push_back(pNode);
        return;
    }

    // otherwise, perform insertion sort
    auto it = m_openSet.begin();
    PathPlanNode* pCompare = *it;
    while (pCompare->IsBetterChoiceThan(pNode)) {
        ++it;
        if (it != m_openSet.end()) {
            pCompare = *it;
        } else {
            break;
        }
    }
    m_openSet.insert(it, pNode);
}

void AStar::ReinsertNode(PathPlanNode* pNode)
{
    assert(pNode);
    for (auto it = m_openSet.begin(); it != m_openSet.end(); ++it) {
        if (pNode == (*it)) {
            m_openSet.erase(it);
            InsertNode(pNode);
            return;
        }
    }

    // if reach here, the node was never in the open set to begin with
    GCC_WARNING("Attempting to reinsert node that was never in the open list");
    InsertNode(pNode);
}

PathPlan* AStar::RebuildPath(PathPlanNode* pGoalNode)
{
    assert(pGoalNode);
    PathPlan* pPlan = new PathPlan;
    PathPlanNode* pNode = pGoalNode;
    while (pNode)
    {
        pPlan->AddNode(pNode->GetPathingNode());
        pNode = pNode->GetPrev();
    }
    return pPlan;
}




void PathingGraph::DestroyGraph()
{
    // destroy all the nodes
    for (auto it = m_nodes.begin(); it != m_nodes.end(); ++it) {
        delete *it;
    }
    m_nodes.clear();

    // destroy all the arcs
    for (auto it = m_arcs.begin(); it != m_arcs.end(); ++it) {
        delete *it;
    }
    m_arcs.clear();
}

PathingNode* PathingGraph::FindClosestNode(const Vec3& pos)
{
    // brute force o(n) that could be improved with octree, quadtree, or similar
    PathingNode* pClosestNode = m_nodes.front();
    float length = FLT_MAX;
    for (auto it = m_nodes.begin(); it != m_nodes.end(); ++it) {
        PathingNode* pNode = *it;
        Vec3 diff = pos - pNode->GetPos();
        if (Mag(diff) < length) {
            pClosestNode = pNode;
            length = Mag(diff);
        }
    }
    return pClosestNode;
}

PathingNode* PathingGraph::FindFurthestNode(const Vec3& pos)
{
    // brute force o(n) that could be improved with octree, quadtree, or similar
    PathingNode* pFurthestNode = m_nodes.front();
    float length = 0.0f;
    for (auto it = m_nodes.begin(); it != m_nodes.end(); ++it) {
        PathingNode* pNode = *it;
        Vec3 diff = pos - pNode->GetPos();
        if (Mag(diff) > length) {
            pFurthestNode = pNode;
            length = Mag(diff);
        }
    }
    return pFurthestNode;
}

PathingNode* PathingGraph::FindRandomNode()
{
    // cache this since it's not guranteed to be constant time
    unsigned int numNodes = (unsigned int)m_nodes.size();

    // choose random node
    unsigned int node = g_pApp->m_pGame->GetRNG().Random(numNodes);

    // if in lower half of node list, start from the bottom
    if (node <= numNodes / 2) {
        auto it = m_nodes.begin();
        for (unsigned int i = 0; i < node; ++i) {
            ++it;
        }
        return *it;
    }

    // otherwise start from top
    auto it = m_nodes.end();
    for (unsigned int i = numNodes; i >= node; --i) {
        --it;
    }
    return *it;
}

PathPlan* PathingGraph::FindPath(const Vec3& startPoint, const Vec3& endPoint)
{
    // There should be some ray-casting to ensure we can actually make it to
    // closest node, but this is good enough for now
    PathingNode* pStart = FindClosestNode(startPoint);
    PathingNode* pGoal = FindClosestNode(endPoint);
    return FindPath(pStart, pGoal);
}

PathPlan* PathingGraph::FindPath(const Vec3& startPoint, PathingNode* pGoalNode)
{
    PathingNode* pStart = FindClosestNode(startPoint);
    return FindPath(pStart, pGoalNode);
}

PathPlan* PathingGraph::FindPath(PathingNode* pStartNode, const Vec3& endPoint)
{
    PathingNode* pGoal = FindClosestNode(endPoint);
    return FindPath(pStartNode, pGoal);
}

PathPlan* PathingGraph::FindPath(PathingNode* pStartNode, PathingNode* pGoalNode)
{
    // find the best path using A* search
    AStar aStar;
    return aStar(pStartNode, pGoalNode);
}

// debug functions
void PathingGraph::BuildTestGraph()
{
    // this should never occur, but better safe than sorry
    if (!m_nodes.empty()) {
        DestroyGraph();
    }

    // keep from reallocating and copying the array
    m_nodes.reserve(81);

    // simple grid of noes
    int index = 0;
    for (float x = -45.0f; x < 45.0f; x += 10.0f) {
        for (float z = -45.0f; z < 45.0f; z += 10.0f) {
            PathingNode* pNode = new PathingNode(Vec3(x,0,z));
            m_nodes.push_back(pNode);

            int tempNode = index - 1;
            if (tempNode >= 0) {
                LinkNodes(m_nodes[tempNode], pNode);
            }

            tempNode = index - 9;
            if (tempNode >= 0) {
                LinkNodes(m_nodes[tempNode], pNode);
            }

            ++index;
        }
    }
}

void PathingGraph::LinkNodes(PathingNode* pNodeA, PathingNode* pNodeB)
{
    assert(pNodeA);
    assert(pNodeB);

    PathingArc* pArc = new PathingArc;
    pArc->LinkNodes(pNodeA, pNodeB);
    pNodeA->AddArc(pArc);
    pNodeB->AddArc(pArc);
    m_arcs.push_back(pArc);
}



PathingGraph* CreatePathingGraph()
{
    PathingGraph* pPathingGraph = new PathingGraph;
    pPathingGraph->BuildTestGraph();
    return pPathingGraph;
}

