#include <iostream>
#include <vector>

// Структура для представления графа с использованием массива заголовков
class Graph {
public:
    int numVertices;            // Количество вершин
    std::vector<int> adjList;   // Линейный массив для хранения смежности
    std::vector<int> header;    // Массив заголовков для указания начала списка

    // Конструктор
    Graph(int vertices) : numVertices(vertices) {
        // Инициализация заголовков с количеством вершин + 1 для указателя конца
        header.resize(vertices + 1, -1);
    }

    // Метод для добавления ребра
    void addEdge(int src, int dest) {
        if (header[src] == -1) {
            header[src] = adjList.size(); // Установка указателя на начало списка
        }
        adjList.push_back(dest); // Добавление смежной вершины в линейный массив
    }

    // Метод для завершения добавления ребер и заполнения заголовков
    void finalizeHeader() {
        for (int i = 0; i < numVertices; ++i) {
            if (header[i] == -1) {
                header[i] = adjList.size(); // Установка указателя на конец списка
            }
        }
        // Указываем на конец списка для удобства обхода
        header[numVertices] = adjList.size();
    }

    // Метод для вывода графа
    void printGraph() {
        std::cout << "Header Array: ";
        for (int i = 0; i <= numVertices; ++i) {
            std::cout << header[i] << " ";
        }
        std::cout << "\nAdjacency List Array: ";
        for (const auto& val : adjList) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    // Метод для вывода смежных вершин для каждой вершины
    void printAdjacency() {
        for (int i = 0; i < numVertices; ++i) {
            std::cout << "Vertex " << i + 1 << ": ";
            for (int j = header[i]; j < header[i + 1]; ++j) {
                std::cout << adjList[j] << " ";
            }
            std::cout << std::endl;
        }
    }
};

int main() {
    // Пример создания графа
    int numVertices = 5;
    Graph graph(numVertices);

    // Добавление ребер (пример на основе загруженного изображения)
    graph.addEdge(0, 2);
    graph.addEdge(0, 3);
    graph.addEdge(1, 4);
    graph.addEdge(2, 4);
    graph.addEdge(3, 2);
    graph.addEdge(4, 5);

    // Завершаем заполнение заголовков
    graph.finalizeHeader();

    // Печать графа
    graph.printGraph();
    graph.printAdjacency();

    return 0;
}