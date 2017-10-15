typedef int NodeId;

class Node {
public:
	Node();
	Node(NodeId id, int nodeDepth);
	~Node();

	NodeId getId() const {
		return id;
	}

	int getNodeDepth() const {
		return nodeDepth;
	}

	void setId(NodeId id) { this->id = id; }
	void setNodeDepth(int nodeDepth) { this->nodeDepth = nodeDepth; }

private:
	NodeId id;
	int nodeDepth;
};

//*************************************************************************************************************
//*************************************************************************************************************

Node::Node() :
	id(),
	nodeDepth(INVALID_NODE_DEPTH)
{}

//*************************************************************************************************************
//*************************************************************************************************************

Node::Node(NodeId id, int nodeDepth) :
	id(id),
	nodeDepth(nodeDepth)
{}

//*************************************************************************************************************
//*************************************************************************************************************

Node::~Node() {
}

//*************************************************************************************************************
//*************************************************************************************************************

typedef list<NodeId> ChildrenList;
typedef map<NodeId, ChildrenList> GraphMap;
typedef map<NodeId, Node*> IdNodeMap;
typedef vector<NodeId> NodeStack;
typedef deque<NodeId> NodeQueue;
typedef set<NodeId> NodeSet;

class Graph {
public:
	Graph();
	Graph(int nodesCount, GraphMap graph);
	~Graph();

	int getNodesCount() const {
		return nodesCount;
	}

	GraphMap getGraph() const {
		return graph;
	}

	IdNodeMap getIdNodeMap() const {
		return idNodeMap;
	}

	Node* getNode(NodeId nodeId) const {
		return idNodeMap.at(nodeId);
	}

	void setNodesCount(int nodesCount) { this->nodesCount = nodesCount; }
	void setGraph(GraphMap graph) { this->graph = graph; }
	void setIdNodeMap(IdNodeMap idNodeMap) { this->idNodeMap = idNodeMap; }

	void createEdge(NodeId x, NodeId y);
	void addEdge(NodeId parentId, NodeId childId);
	void createNode(NodeId nodeId, int nodeDepth);
	bool nodeCreated(NodeId nodeId) const;
	void deleteAllNodes();
	vector<NodeId> treeRootsIds() const;
	void dfs(NodeId treeRootNodeId);
	void bfs(NodeId treeRootNodeId);
	int getMaxNodeDepth() const;
	bool edgeExists(NodeId parent, NodeId child) const;

private:
	int nodesCount;
	GraphMap graph;

	// Map used to store all nodes, used to check if node is already created
	// and for easy accesss when deleteing memory pointed by each node
	IdNodeMap idNodeMap;
};

//*************************************************************************************************************
//*************************************************************************************************************

Graph::Graph() :
	nodesCount(0),
	graph()
{}

//*************************************************************************************************************
//*************************************************************************************************************

Graph::Graph(int nodesCount, GraphMap graph) :
	nodesCount(nodesCount),
	graph(graph)
{}

//*************************************************************************************************************
//*************************************************************************************************************

Graph::~Graph() {
	deleteAllNodes();
	graph.clear();
}

//*************************************************************************************************************
//*************************************************************************************************************

void Graph::deleteAllNodes() {
	for (IdNodeMap::iterator it = idNodeMap.begin(); it != idNodeMap.end(); ++it) {
		Node* node = it->second;

		if (node) {
			delete node;
			node = NULL;
		}
	}
}

//*************************************************************************************************************
//*************************************************************************************************************

vector<NodeId> Graph::treeRootsIds() const {
	vector<NodeId> res;

	for (IdNodeMap::const_iterator nodeIt = idNodeMap.begin(); nodeIt != idNodeMap.end(); ++nodeIt) {
		NodeId nodeId = nodeIt->first;

		bool isChildOfANode = false;

		for (GraphMap::const_iterator graphIt = graph.begin(); graphIt != graph.end(); ++graphIt) {
			ChildrenList childrenList = graphIt->second;
			if (find(childrenList.begin(), childrenList.end(), nodeId) != childrenList.end()) {
				isChildOfANode = true;
				break;
			}
		}

		if (!isChildOfANode) {
			res.push_back(nodeId);
		}
	}

	return res;
}

//*************************************************************************************************************
//*************************************************************************************************************

void Graph::dfs(NodeId treeRootNodeId) {
	getNode(treeRootNodeId)->setNodeDepth(TREE_ROOT_NODE_DEPTH);

	NodeStack frontier;
	NodeSet explored;

	frontier.push_back(treeRootNodeId);

	while (!frontier.empty()) {
		NodeId state = frontier.back();
		frontier.pop_back();

		explored.insert(state);

		for (ChildrenList::iterator nodeIt = graph[state].begin(); nodeIt != graph[state].end(); ++nodeIt) {
			NodeId childId = *nodeIt;

			bool nodeExplored = explored.find(childId) != explored.end();
			bool nodeInFrontier = find(frontier.begin(), frontier.end(), childId) != frontier.end();
			if (!nodeExplored && !nodeInFrontier) {
				frontier.push_back(childId);

				int parentDepth = idNodeMap[state]->getNodeDepth();
				idNodeMap[childId]->setNodeDepth(parentDepth + 1);
			}
		}
	}
}

//*************************************************************************************************************
//*************************************************************************************************************

void Graph::bfs(NodeId treeRootNodeId) {
	getNode(treeRootNodeId)->setNodeDepth(TREE_ROOT_NODE_DEPTH);

	NodeQueue frontier;
	NodeSet explored;

	frontier.push_back(treeRootNodeId);

	while (!frontier.empty()) {
		NodeId state = frontier.front();
		frontier.pop_front();

		explored.insert(state);

		for (ChildrenList::iterator nodeIt = graph[state].begin(); nodeIt != graph[state].end(); ++nodeIt) {
			NodeId childId = *nodeIt;

			bool nodeExplored = explored.find(childId) != explored.end();
			bool nodeInFrontier = find(frontier.begin(), frontier.end(), childId) != frontier.end();
			if (!nodeExplored && !nodeInFrontier) {
				frontier.push_back(childId);

				int parentDepth = idNodeMap[state]->getNodeDepth();
				idNodeMap[childId]->setNodeDepth(parentDepth + 1);
			}
		}
	}
}

//*************************************************************************************************************
//*************************************************************************************************************

int Graph::getMaxNodeDepth() const {
	int maxNodeDepth = INVALID_NODE_DEPTH;

	for (IdNodeMap::const_iterator nodeIt = idNodeMap.begin(); nodeIt != idNodeMap.end(); ++nodeIt) {
		int nodeDepth = nodeIt->second->getNodeDepth();
		if (nodeDepth > maxNodeDepth) {
			maxNodeDepth = nodeDepth;
		}
	}

	return maxNodeDepth;
}

//*************************************************************************************************************
//*************************************************************************************************************

bool Graph::edgeExists(NodeId parent, NodeId child) const {
	bool res = false;

	if (nodeCreated(parent) && nodeCreated(child)) {
		ChildrenList children = graph.at(parent); // TODO: copying do not copy use * for children
		res = find(children.begin(), children.end(), child) != children.end();
	}

	return res;
}

//*************************************************************************************************************
//*************************************************************************************************************

void Graph::addEdge(NodeId parentId, NodeId childId) {
	createNode(parentId, INVALID_NODE_DEPTH);
	createNode(childId, INVALID_NODE_DEPTH);
	graph[parentId].push_back(childId);
}

//*************************************************************************************************************
//*************************************************************************************************************

void Graph::createNode(NodeId nodeId, int nodeDepth) {
	if (!nodeCreated(nodeId)) {
		Node* node = new Node(nodeId, nodeDepth);
		idNodeMap[node->getId()] = node;
		++nodesCount;
	}
}

//*************************************************************************************************************
//*************************************************************************************************************

bool Graph::nodeCreated(NodeId nodeId) const {
	return idNodeMap.end() != idNodeMap.find(nodeId);
}

//*************************************************************************************************************
//*************************************************************************************************************

void Graph::createEdge(NodeId x, NodeId y) {
	createNode(x, INVALID_NODE_DEPTH);
	createNode(y, INVALID_NODE_DEPTH);

	addEdge(x, y);
}

//*************************************************************************************************************
//*************************************************************************************************************
