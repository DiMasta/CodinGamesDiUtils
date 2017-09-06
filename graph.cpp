class Node {
public:
	Node();
	Node(int id, int nodeDepth);
	~Node();

	int getId() const {
		return id;
	}

	int getNodeDepth() const {
		return nodeDepth;
	}

	void setId(int id) { this->id = id; }
	void setNodeDepth(int nodeDepth) { this->nodeDepth = nodeDepth; }

private:
	int id;
	int nodeDepth;
};

//*************************************************************************************************************
//*************************************************************************************************************

Node::Node() :
	id(INVALID_ID),
	nodeDepth(INVALID_NODE_DEPTH)
{}

//*************************************************************************************************************
//*************************************************************************************************************

Node::Node(int id, int nodeDepth) :
	id(id),
	nodeDepth(nodeDepth)
{}

//*************************************************************************************************************
//*************************************************************************************************************

Node::~Node() {
}

//*************************************************************************************************************
//*************************************************************************************************************

typedef list<int> ChildrenList;
typedef map<int, ChildrenList> GraphMap;
typedef map<int, Node*> IdNodeMap;
typedef vector<int> NodeStack;
typedef queue<int> NodeQueue;
typedef set<int> NodeSet;

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

	Node* getNode(int nodeId) const {
		return idNodeMap.at(nodeId);
	}

	void setNodesCount(int nodesCount) { this->nodesCount = nodesCount; }
	void setGraph(GraphMap graph) { this->graph = graph; }
	void setIdNodeMap(IdNodeMap idNodeMap) { this->idNodeMap = idNodeMap; }

	void addEdge(int parentId, int childId);
	void addNodeInMap(Node* node);
	void createNode(int nodeId, int nodeDepth);
	bool nodeCreated(int nodeId) const;
	void deleteAllNodes();
	vector<int> treeRootsIds() const;
	void dfs(int treeRootNodeId);
	int getMaxNodeDepth() const;

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

vector<int> Graph::treeRootsIds() const {
	vector<int> res;

	for (IdNodeMap::const_iterator nodeIt = idNodeMap.begin(); nodeIt != idNodeMap.end(); ++nodeIt) {
		int nodeId = nodeIt->first;

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

void Graph::dfs(int treeRootNodeId) {
	getNode(treeRootNodeId)->setNodeDepth(TREE_ROOT_NODE_DEPTH);

	NodeStack frontier;
	NodeSet explored;

	frontier.push_back(treeRootNodeId);

	while (!frontier.empty()) {
		int state = frontier.back();
		frontier.pop_back();

		explored.insert(state);

		for (ChildrenList::iterator nodeIt = graph[state].begin(); nodeIt != graph[state].end(); ++nodeIt) {
			int childId = *nodeIt;

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

void Graph::addEdge(int parentId, int childId) {
	graph[parentId].push_back(childId);
}

//*************************************************************************************************************
//*************************************************************************************************************

void Graph::addNodeInMap(Node* node) {
	idNodeMap[node->getId()] = node;
}

//*************************************************************************************************************
//*************************************************************************************************************

void Graph::createNode(int nodeId, int nodeDepth) {
	if (!nodeCreated(nodeId)) {
		Node* node = new Node(nodeId, nodeDepth);
		idNodeMap[node->getId()] = node;
	}
}

//*************************************************************************************************************
//*************************************************************************************************************

bool Graph::nodeCreated(int nodeId) const {
	return idNodeMap.end() != idNodeMap.find(nodeId);
}