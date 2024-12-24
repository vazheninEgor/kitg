#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <stdexcept>

// Класс Heap для реализации min-кучи
class Heap {
private:
    std::vector<int> heap;

    void heapifyDown(int i) {
        int n = heap.size();
        int smallest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < n && heap[left] < heap[smallest]) {
            smallest = left;
        }
        if (right < n && heap[right] < heap[smallest]) {
            smallest = right;
        }
        if (smallest != i) {
            std::swap(heap[i], heap[smallest]);
            heapifyDown(smallest);
        }
    }

    void heapifyUp(int i) {
        while (i > 0 && heap[(i - 1) / 2] > heap[i]) {
            std::swap(heap[i], heap[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }

public:
    // Добавление элемента
    void insert(int key) {
        heap.push_back(key);
        heapifyUp(heap.size() - 1);
    }

    // Извлечение минимального элемента
    int extractMin() {
        if (heap.empty()) {
            throw std::out_of_range("Heap is empty");
        }
        int root = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) {
            heapifyDown(0);
        }
        return root;
    }

    // Уменьшение значения элемента
    void decreaseKey(int i, int newVal) {
        if (i < 0 || i >= heap.size()) {
            throw std::out_of_range("Index out of range");
        }
        if (newVal > heap[i]) {
            throw std::invalid_argument("New value must be smaller");
        }
        heap[i] = newVal;
        heapifyUp(i);
    }

    // Увеличение значения элемента
    void increaseKey(int i, int newVal) {
        if (i < 0 || i >= heap.size()) {
            throw std::out_of_range("Index out of range");
        }
        if (newVal < heap[i]) {
            throw std::invalid_argument("New value must be larger");
        }
        heap[i] = newVal;
        heapifyDown(i);
    }

    // Удаление произвольного элемента
    void remove(int i) {
        if (i < 0 || i >= heap.size()) {
            throw std::out_of_range("Index out of range");
        }
        decreaseKey(i, INT_MIN); // Снизить значение до минимального
        extractMin();           // Удалить минимальный элемент
    }

    // Печать элементов кучи
    void printHeap() {
        for (int val : heap) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
};

// Алгоритм Heapsort
void heapify(std::vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }
    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }
    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(std::vector<int>& arr) {
    int n = arr.size();

    // Построение max-кучи
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }

    // Сортировка
    for (int i = n - 1; i > 0; i--) {
        std::swap(arr[0], arr[i]);  // Перемещение текущего максимума в конец
        heapify(arr, i, 0);        // Восстановление свойств кучи
    }
}

// Алгоритм Дейкстры
void dijkstra(const std::vector<std::vector<std::pair<int, int>>>& graph, int start) {
    int n = graph.size();
    std::vector<int> dist(n, INT_MAX);
    dist[start] = 0;

    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;
    pq.push({0, start});

    while (!pq.empty()) {
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d > dist[u]) continue;

        for (auto [v, weight] : graph[u]) {
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }

    for (int i = 0; i < n; i++) {
        std::cout << "Vertex " << i << ", Distance: " << dist[i] << std::endl;
    }
}

// Пример использования приоритетной очереди
void priorityQueueExample() {
    std::priority_queue<int, std::vector<int>, std::greater<int>> pq; // Min-очередь

    pq.push(10);
    pq.push(5);
    pq.push(20);

    std::cout << "Элементы приоритетной очереди (по порядку удаления): ";
    while (!pq.empty()) {
        std::cout << pq.top() << " ";
        pq.pop();
    }
    std::cout << std::endl;
}

// Основная функция для тестирования
int main() {
    // Тестирование класса Heap
    Heap heap;
    heap.insert(10);
    heap.insert(5);
    heap.insert(20);
    heap.insert(2);
    heap.printHeap();

    std::cout << "Extracted Min: " << heap.extractMin() << std::endl;
    heap.printHeap();

    heap.decreaseKey(1, 1);
    heap.printHeap();

    heap.increaseKey(0, 15);
    heap.printHeap();

    heap.remove(0);
    heap.printHeap();

    // Тестирование Heapsort
    std::vector<int> arr = {10, 3, 15, 7, 8, 23, 98, 29};
    heapSort(arr);
    for (int val : arr) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // Тестирование алгоритма Дейкстры
    std::vector<std::vector<std::pair<int, int>>> graph = {
            {{1, 4}, {2, 1}},
            {{3, 1}},
            {{1, 2}, {3, 5}},
            {}
    };
    dijkstra(graph, 0);

    // Тестирование приоритетной очереди
    priorityQueueExample();

    return 0;
}
