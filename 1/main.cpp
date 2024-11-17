#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <ctime>
#include <cstdlib>


// Список ребер (дуг)
class EdgeListGraph {
public:
    std::vector<int> I; // Начальные вершины
    std::vector<int> J; // Конечные вершины
    std::vector<double> weights; // Вес дуг
    std::unordered_set<int> vertices; // Набор для хранения уникальных вершин

    // Вставка вершины
    void insertVertex(int vertex) {
        vertices.insert(vertex);
    }

    // Вставка ребра
    void insertEdge(int from, int to, double weight) {
        I.push_back(from);
        J.push_back(to);
        weights.push_back(weight);
        // Убедимся, что вершины также добавлены
        insertVertex(from);
        insertVertex(to);
    }

    // Удаление вершины и всех её инцидентных рёбер
    void removeVertex(int vertex) {
        // Удаляем рёбра, где вершина выступает как начальная или конечная
        for (int k = I.size() - 1; k >= 0; --k) {
            if (I[k] == vertex || J[k] == vertex) {
                removeEdge(k);
            }
        }
        // Удаляем вершину
        vertices.erase(vertex);
    }

    // Удаление ребра по индексу
    void removeEdge(int index) {
        if (index >= 0 && index < I.size()) {
            I.erase(I.begin() + index);
            J.erase(J.begin() + index);
            weights.erase(weights.begin() + index);
        }
    }

    // Удаление ребра по вершинам
    void removeEdge(int from, int to) {
        for (int k = I.size() - 1; k >= 0; --k) {
            if (I[k] == from && J[k] == to) {
                removeEdge(k);
            }
        }
    }

    // Поиск вершины
    bool findVertex(int vertex) {
        return vertices.find(vertex) != vertices.end();
    }

    // Поиск ребра (возвращает индекс или -1)
    int findEdge(int from, int to) {
        for (size_t k = 0; k < I.size(); ++k) {
            if (I[k] == from && J[k] == to) {
                return k;
            }
        }
        return -1;
    }

    // Печать всех рёбер
    void printEdges() {
        for (size_t k = 0; k < I.size(); ++k) {
            std::cout << "Edge " << k << ": " << I[k] << " -> " << J[k]
                      << ", Weight: " << weights[k] << "\n";
        }
    }

    // Печать всех вершин
    void printVertices() {
        std::cout << "Vertices: ";
        for (const int &v : vertices) {
            std::cout << v << " ";
        }
        std::cout << "\n";
    }
};


// Список пучков дуг
class BundledEdgeListGraph {
public:
    std::vector<int> I; // Начальные вершины дуг
    std::vector<int> J; // Конечные вершины дуг
    std::vector<double> weights; // Вес дуг
    std::vector<int> H; // Массив голов списков пучков дуг
    std::vector<int> L; // Массив ссылок на следующую дугу
    std::unordered_set<int> vertices; // Уникальные вершины

    // Конструктор для инициализации массивов
    BundledEdgeListGraph(int numVertices) {
        H.resize(numVertices, -1);
    }

    // Вставка вершины
    void insertVertex(int vertex) {
        vertices.insert(vertex);
        if (vertex >= H.size()) {
            H.resize(vertex + 1, -1);
        }
    }

    // Вставка дуги
    void insertEdge(int from, int to, double weight) {
        int edgeIndex = I.size();
        I.push_back(from);
        J.push_back(to);
        weights.push_back(weight);
        L.push_back(H[from]);
        H[from] = edgeIndex;

        insertVertex(from);
        insertVertex(to);
    }

    // Удаление дуги по индексу
    void removeEdge(int index) {
        if (index >= 0 && index < I.size()) {
            int from = I[index];

            // Удаление дуги из списка H и L
            if (H[from] == index) {
                H[from] = L[index];
            } else {
                for (int i = H[from]; i != -1; i = L[i]) {
                    if (L[i] == index) {
                        L[i] = L[index];
                        break;
                    }
                }
            }

            // Удаление значений из массивов I, J, weights и L
            I.erase(I.begin() + index);
            J.erase(J.begin() + index);
            weights.erase(weights.begin() + index);
            L.erase(L.begin() + index);

            // Обновление массивов после удаления
            for (int &link : L) {
                if (link > index) {
                    --link;
                }
            }
            for (int &head : H) {
                if (head > index) {
                    --head;
                }
            }
        }
    }

    // Удаление вершины и всех её инцидентных рёбер
    void removeVertex(int vertex) {
        if (!findVertex(vertex)) return;

        for (int i = H[vertex]; i != -1;) {
            int next = L[i];
            removeEdge(i);
            i = next;
        }

        vertices.erase(vertex);
        H[vertex] = -1;
    }

    // Поиск вершины
    bool findVertex(int vertex) {
        return vertices.find(vertex) != vertices.end();
    }

    // Поиск дуги
    int findEdge(int from, int to) {
        for (int i = H[from]; i != -1; i = L[i]) {
            if (J[i] == to) {
                return i;
            }
        }
        return -1;
    }

    // Печать всех дуг
    void printEdges() {
        for (size_t k = 0; k < I.size(); ++k) {
            std::cout << "Edge " << k << ": " << I[k] << " -> " << J[k]
                      << ", Weight: " << weights[k] << "\n";
        }
    }

    // Печать всех вершин
    void printVertices() {
        std::cout << "Vertices: ";
        for (const int &v : vertices) {
            std::cout << v << " ";
        }
        std::cout << "\n";
    }
};


// Список смежности
class AdjacencyListGraph {
public:
    std::unordered_map<int, std::list<std::pair<int, double>>> adjList;
    std::unordered_set<int> vertices;

    // Конструктор по умолчанию
    AdjacencyListGraph() = default;  // Теперь этот класс можно создавать без параметров

    // Конструктор с параметром для задания числа вершин
    AdjacencyListGraph(int numVertices) {
        // Инициализация для заранее определённого числа вершин, если нужно
        for (int i = 0; i < numVertices; ++i) {
            vertices.insert(i);
        }
    }

    void insertVertex(int vertex) {
        vertices.insert(vertex);
        if (adjList.find(vertex) == adjList.end()) {
            adjList[vertex] = std::list<std::pair<int, double>>();
        }
    }

    void insertEdge(int from, int to, double weight) {
        adjList[from].emplace_back(to, weight);
        insertVertex(from);
        insertVertex(to);
    }

    void removeEdge(int from, int to) {
        if (adjList.find(from) != adjList.end()) {
            adjList[from].remove_if([to](const std::pair<int, double>& edge) {
                return edge.first == to;
            });
        }
    }

    void removeVertex(int vertex) {
        adjList.erase(vertex);
        for (auto& [key, neighbors] : adjList) {
            neighbors.remove_if([vertex](const std::pair<int, double>& edge) {
                return edge.first == vertex;
            });
        }
        vertices.erase(vertex);
    }

    bool findVertex(int vertex) {
        return vertices.find(vertex) != vertices.end();
    }

    bool findEdge(int from, int to) {
        if (adjList.find(from) != adjList.end()) {
            for (const auto& edge : adjList[from]) {
                if (edge.first == to) {
                    return true;
                }
            }
        }
        return false;
    }

    void printEdges() {
        for (const auto& [vertex, neighbors] : adjList) {
            for (const auto& [to, weight] : neighbors) {
                std::cout << "Edge: " << vertex << " -> " << to
                          << ", Weight: " << weight << "\n";
            }
        }
    }
};



void testPerformance() {
    const int numVertices = 1000000;
    const int numEdges = 10000000;

    // 1. EdgeListGraph
    EdgeListGraph edgeListGraph;
    clock_t start = clock();

    // Вставка рёбер
    for (int i = 0; i < numEdges; ++i) {
        edgeListGraph.insertEdge(rand() % numVertices, rand() % numVertices, rand() % 100);
    }
    std::cout << "EdgeListGraph insertEdge time: " << (clock() - start) << "ms\n";

    // Вставка вершины
    start = clock();
    edgeListGraph.insertVertex(1001);
    std::cout << "EdgeListGraph insertVertex time: " << (clock() - start) << "ms\n";

    // Удаление рёбер
    start = clock();
    edgeListGraph.removeEdge(0, 1);
    std::cout << "EdgeListGraph removeEdge time: " << (clock() - start) << "ms\n";

    // Удаление вершины
    start = clock();
    edgeListGraph.removeVertex(0);
    std::cout << "EdgeListGraph removeVertex time: " << (clock() - start) << "ms\n";

    // Поиск рёбер
    start = clock();
    edgeListGraph.findEdge(0, 1);
    std::cout << "EdgeListGraph findEdge time: " << (clock() - start) << "ms\n";

    // Поиск вершин
    start = clock();
    edgeListGraph.findVertex(0);
    std::cout << "EdgeListGraph findVertex time: " << (clock() - start) << "ms\n";

    // 2. AdjacencyListGraph
    AdjacencyListGraph adjListGraph;
    start = clock();

    // Вставка рёбер
    for (int i = 0; i < numEdges; ++i) {
        adjListGraph.insertEdge(rand() % numVertices, rand() % numVertices, rand() % 100);
    }
    std::cout << "AdjacencyListGraph insertEdge time: " << (clock() - start) << "ms\n";

    // Вставка вершины
    start = clock();
    adjListGraph.insertVertex(1001);
    std::cout << "AdjacencyListGraph insertVertex time: " << (clock() - start) << "ms\n";

    // Удаление рёбер
    start = clock();
    adjListGraph.removeEdge(0, 1);
    std::cout << "AdjacencyListGraph removeEdge time: " << (clock() - start) << "ms\n";

    // Удаление вершины
    start = clock();
    adjListGraph.removeVertex(0);
    std::cout << "AdjacencyListGraph removeVertex time: " << (clock() - start) << "ms\n";

    // Поиск рёбер
    start = clock();
    adjListGraph.findEdge(0, 1);
    std::cout << "AdjacencyListGraph findEdge time: " << (clock() - start) << "ms\n";

    // Поиск вершин
    start = clock();
    adjListGraph.findVertex(0);
    std::cout << "AdjacencyListGraph findVertex time: " << (clock() - start) << "ms\n";

    // 3. BundledEdgeListGraph
    BundledEdgeListGraph bundledGraph(numVertices);
    start = clock();

    // Вставка рёбер
    for (int i = 0; i < numEdges; ++i) {
        bundledGraph.insertEdge(rand() % numVertices, rand() % numVertices, rand() % 100);
    }
    std::cout << "BundledEdgeListGraph insertEdge time: " << (clock() - start) << "ms\n";

    // Вставка вершины
    start = clock();
    bundledGraph.insertVertex(1001);
    std::cout << "BundledEdgeListGraph insertVertex time: " << (clock() - start) << "ms\n";

    // Удаление рёбер
    start = clock();
    bundledGraph.removeEdge(0);
    std::cout << "BundledEdgeListGraph removeEdge time: " << (clock() - start) << "ms\n";

    // Удаление вершины
    start = clock();
    bundledGraph.removeVertex(0);
    std::cout << "BundledEdgeListGraph removeVertex time: " << (clock() - start) << "ms\n";

    // Поиск рёбер
    start = clock();
    bundledGraph.findEdge(0, 1);
    std::cout << "BundledEdgeListGraph findEdge time: " << (clock() - start) << "ms\n";

    // Поиск вершин
    start = clock();
    bundledGraph.findVertex(0);
    std::cout << "BundledEdgeListGraph findVertex time: " << (clock() - start) << "ms\n";
}


int main() {
    /* Тестирование списка ребер (дуг)
    EdgeListGraph graph;

    // Добавление вершин
    graph.insertVertex(0);
    graph.insertVertex(1);
    graph.insertVertex(2);

    // Добавление рёбер
    graph.insertEdge(0, 1, 5.0);
    graph.insertEdge(1, 2, 3.0);
    graph.insertEdge(2, 0, 2.5);

    // Печать всех вершин и рёбер
    graph.printVertices();
    graph.printEdges();

    // Поиск вершин и рёбер
    std::cout << "Vertex 1 exists: " << graph.findVertex(1) << "\n";
    int index = graph.findEdge(1, 2);
    if (index != -1) {
        std::cout << "Edge (1 -> 2) found at index " << index << "\n";
    } else {
        std::cout << "Edge (1 -> 2) not found\n";
    }

    // Удаление вершины
    graph.removeVertex(1);
    graph.printEdges();
    graph.printVertices(); */


    /* Тестирование списка пучков дуг
    BundledEdgeListGraph graph(4); // Инициализация с 4 вершинами

    // Вставка рёбер
    graph.insertEdge(0, 1, 5.0);
    graph.insertEdge(1, 3, 2.0);
    graph.insertEdge(1, 2, 4.0);
    graph.insertEdge(2, 2, 3.0);
    graph.insertEdge(0, 0, 1.0);

    // Печать вершин и дуг
    graph.printVertices();
    graph.printEdges();

    // Поиск и удаление
    int index = graph.findEdge(1, 3);
    if (index != -1) {
        std::cout << "Edge (1 -> 3) found at index " << index << "\n";
    } else {
        std::cout << "Edge (1 -> 3) not found\n";
    }

    // Удаление вершины
    graph.removeVertex(1);
    graph.printEdges();
    graph.printVertices(); */


    /* Тестирование списка смежности
    AdjacencyListGraph graph(4); // Инициализируем граф с 4 вершинами
    graph.insertEdge(0, 1, 5.0);
    graph.insertEdge(1, 2, 3.0);
    graph.insertEdge(2, 3, 1.5);

    std::cout << "Initial graph:\n";
    graph.printEdges();

    graph.removeEdge(1, 2);  // Удаляем ребро 1 -> 2
    std::cout << "\nGraph after removing edge 1 -> 2:\n";
    graph.printEdges();

    graph.removeVertex(2);  // Удаляем вершину 2
    std::cout << "\nGraph after removing vertex 2:\n";
    graph.printEdges(); */

    testPerformance();

    return 0;
}


