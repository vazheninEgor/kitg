#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <chrono>
#include <random>
#include <tuple>
#include <cstdlib>

using namespace std;
using namespace chrono;

// Список дуг
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

    // Вставка дуги
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

    // Удаление дуги по индексу
    void removeEdge(int index) {
        if (index >= 0 && index < I.size()) {
            I.erase(I.begin() + index);
            J.erase(J.begin() + index);
            weights.erase(weights.begin() + index);
        }
    }

    // Удаление дуги по вершинам (удаляет все дуги с данными вершинами)
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

    // Поиск дуги (возвращает индекс или -1)
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
            H.resize(vertex + 1, -1); // Расширение массива голов при необходимости
        }
    }

    // Вставка дуги
    void insertEdge(int from, int to, double weight) {
        int edgeIndex = I.size();
        I.push_back(from);
        J.push_back(to);
        weights.push_back(weight);

        // Добавляем ссылку на предыдущую дугу
        L.push_back(H[from]);
        H[from] = edgeIndex;

        // Добавляем вершины в набор, если их ещё нет
        insertVertex(from);
        insertVertex(to);
    }

    // Удаление дуги по индексу
    void removeEdge(int index) {
        if (index >= 0 && index < I.size()) {
            int from = I[index];

            // Удаление дуги из списка пучков
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

            // Удаление элементов из массивов
            I.erase(I.begin() + index);
            J.erase(J.begin() + index);
            weights.erase(weights.begin() + index);
            L.erase(L.begin() + index);

            // Обновление ссылок после удаления элемента
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

    // Удаление дуги по инцидентным вершинам
    void removeEdge(int from, int to) {
        for (int i = H[from]; i != -1;) {
            if (J[i] == to) {
                int next = L[i];
                removeEdge(i);
                i = next;
            } else {
                i = L[i];
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
        H[vertex] = -1; // Обнуление головы списка для удалённой вершины
    }

    // Поиск вершины
    bool findVertex(int vertex) {
        return vertices.find(vertex) != vertices.end();
    }

    // Поиск дуги (возвращает индекс или -1)
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
    std::unordered_map<int, std::list<std::pair<int, double>>> adjList; // Словарь списков смежности
    std::unordered_set<int> vertices; // Набор уникальных вершин

    // Вставка вершины
    void insertVertex(int vertex) {
        vertices.insert(vertex);
        if (adjList.find(vertex) == adjList.end()) {
            adjList[vertex] = std::list<std::pair<int, double>>(); // Инициализация списка для вершины
        }
    }

    // Вставка дуги
    void insertEdge(int from, int to, double weight) {
        adjList[from].emplace_back(to, weight);
        insertVertex(from);
        insertVertex(to);
    }

    // Удаление дуги
    void removeEdge(int from, int to) {
        if (adjList.find(from) != adjList.end()) {
            adjList[from].remove_if([to](const std::pair<int, double>& edge) {
                return edge.first == to;
            });
        }
    }

    // Удаление вершины
    void removeVertex(int vertex) {
        adjList.erase(vertex);
        for (auto& [key, neighbors] : adjList) {
            neighbors.remove_if([vertex](const std::pair<int, double>& edge) {
                return edge.first == vertex;
            });
        }
        vertices.erase(vertex);
    }

    // Поиск вершины
    bool findVertex(int vertex) {
        return vertices.find(vertex) != vertices.end();
    }

    // Поиск дуги
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

    // Печать всех дуг
    void printEdges() {
        for (const auto& [vertex, neighbors] : adjList) {
            for (const auto& [to, weight] : neighbors) {
                std::cout << "Edge: " << vertex << " -> " << to
                          << ", Weight: " << weight << "\n";
            }
        }
    }
};

// Тестирование реализаций
void testRealization() {
    // Тестирование списка дуг (EdgeListGraph)
    std::cout << "Testing EdgeListGraph...\n";
    EdgeListGraph edgeListGraph;

    // Добавление вершин
    edgeListGraph.insertVertex(0);
    edgeListGraph.insertVertex(1);
    edgeListGraph.insertVertex(2);

    // Добавление рёбер
    edgeListGraph.insertEdge(0, 1, 5.0);
    edgeListGraph.insertEdge(1, 2, 3.0);
    edgeListGraph.insertEdge(2, 0, 2.5);

    // Печать всех вершин и рёбер
    std::cout << "Initial graph:\n";
    edgeListGraph.printVertices();
    edgeListGraph.printEdges();

    // Поиск вершин и рёбер
    std::cout << "Vertex 1 exists: " << edgeListGraph.findVertex(1) << "\n";
    int index = edgeListGraph.findEdge(1, 2);
    if (index != -1) {
        std::cout << "Edge (1 -> 2) found at index " << index << "\n";
    } else {
        std::cout << "Edge (1 -> 2) not found\n";
    }

    // Удаление вершины
    edgeListGraph.removeVertex(1);
    std::cout << "Graph after removing vertex 1:\n";
    edgeListGraph.printVertices();
    edgeListGraph.printEdges();

    // Удаление рёбер
    edgeListGraph.removeEdge(0, 1); // Удаление рёбра 0 -> 1, если оно существует
    std::cout << "Graph after removing edge 0 -> 1:\n";
    edgeListGraph.printEdges();

    // Тестирование списка пучков дуг (BundledEdgeListGraph)
    std::cout << "\nTesting BundledEdgeListGraph...\n";
    BundledEdgeListGraph bundledGraph(4); // Инициализация с 4 вершинами

    // Вставка рёбер
    bundledGraph.insertEdge(0, 1, 5.0);
    bundledGraph.insertEdge(1, 3, 2.0);
    bundledGraph.insertEdge(1, 2, 4.0);
    bundledGraph.insertEdge(2, 2, 3.0);
    bundledGraph.insertEdge(0, 0, 1.0);

    // Печать вершин и дуг
    std::cout << "Initial graph:\n";
    bundledGraph.printVertices();
    bundledGraph.printEdges();

    // Поиск и удаление рёбер
    index = bundledGraph.findEdge(1, 3);
    if (index != -1) {
        std::cout << "Edge (1 -> 3) found at index " << index << "\n";
    } else {
        std::cout << "Edge (1 -> 3) not found\n";
    }
    bundledGraph.removeEdge(index); // Удаление найденного рёбра
    std::cout << "Graph after removing edge 1 -> 3:\n";
    bundledGraph.printEdges();

    // Удаление вершины
    bundledGraph.removeVertex(1);
    std::cout << "Graph after removing vertex 1:\n";
    bundledGraph.printVertices();
    bundledGraph.printEdges();

    // Тестирование списка смежности (AdjacencyListGraph)
    std::cout << "\nTesting AdjacencyListGraph...\n";
    AdjacencyListGraph adjListGraph;

    // Вставка рёбер
    adjListGraph.insertEdge(0, 1, 5.0);
    adjListGraph.insertEdge(1, 2, 3.0);
    adjListGraph.insertEdge(2, 3, 1.5);
    adjListGraph.insertEdge(3, 0, 2.0);

    // Печать графа
    std::cout << "Initial graph:\n";
    adjListGraph.printEdges();

    // Поиск рёбер
    bool edgeExists = adjListGraph.findEdge(1, 2);
    std::cout << "Edge (1 -> 2) exists: " << edgeExists << "\n";

    // Удаление рёбра
    adjListGraph.removeEdge(1, 2);
    std::cout << "Graph after removing edge 1 -> 2:\n";
    adjListGraph.printEdges();

    // Удаление вершины
    adjListGraph.removeVertex(2);
    std::cout << "Graph after removing vertex 2:\n";
    adjListGraph.printEdges();

    // Проверка поиска вершины
    std::cout << "Vertex 0 exists: " << adjListGraph.findVertex(0) << "\n";
}

int main() {
    testRealization();
}