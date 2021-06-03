#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <chrono>
#include <set>
#include <algorithm>
#include <iterator>
#include <utility>

using namespace std;

typedef __int8 index_t;
index_t n_first, n_second; // вершины, между которыми необходимо найти все пути

vector<vector<index_t>> graph;
list<list<index_t>> routs; // все найденные пути между заданной парой вершин
vector<bool> grey(20000); // серые вершины

static size_t all = 0;

//Depth-first search (Поиск в глубину)
void dfs(index_t const& cur_node, index_t const& last_node, list<index_t>& visited)
{
    if (!grey[cur_node])
    {
        grey[cur_node] = true;
        visited.push_back(cur_node);
        
        if (cur_node == last_node)
        {
            all++;
            //
            routs.push_back(visited);

            if (0 == all % 100000)
              cout << all << endl;
        }
        else {

            auto& it = graph[cur_node - 1];

            for (auto const& val : it)
            {
                dfs(val, last_node, visited);
            }
        }

        visited.pop_back();
        grey[cur_node] = false;
    }
}

//ввод размера графа и определение соседей для вершин
void input_graph() {
    int n, m;
    cout << "Введите количество вершин по горизонтали(=\"столбцов\") : ";
    cin >> n;

    cout << "Введите количество вершин по вертикали (= \"строк\") : ";
    cin >> m; // вершины, между которыми необходимо найти все пути
    n_first = 1;
    n_second = n*m;

    index_t current = n_first;
    for (index_t k = 1; k <= m; k++) {
        for (index_t t = 1; t <= n; t++, current++) {

            if (current % n == 1 && current > n && current < (n * m - (n - 1))) {
                graph.push_back(vector<index_t>({ (index_t)(current - n), (index_t)(current + 1), (index_t)(current + n) }));
            }
            else if (current % n == 1 && (current + (n - 1) == n * m)) {
                graph.push_back(vector<index_t>({ (index_t)(current - n), (index_t)(current + 1) }));
            }
            else if (current % n == 1) {
                graph.push_back(vector<index_t>({ (index_t)(current + 1), (index_t)(current + n) }));
            }
            else if (current / n == 1 && current % n == 0) {
                graph.push_back(vector<index_t>({ (index_t)(current - 1), (index_t)(current * 2) }));
            }
            else if (current / n == m && current % n == 0) {
                graph.push_back(vector<index_t>({ (index_t)(current - n), (index_t)(current - 1) }));
            }
            else if (current % n == 0) {
                graph.push_back(vector<index_t>({ (index_t)(current - n), (index_t)(current - 1), (index_t)(current + n) }));
            }
            else {
                if (k == 1) {
                    graph.push_back(vector<index_t>({ (index_t)(current - 1), (index_t)(current + 1), (index_t)(current + n) }));
                }
                if (k == m) {
                    graph.push_back(vector<index_t>({ (index_t)(current - n), (index_t)(current - 1), (index_t)(current + 1) }));
                }
                if (k != 1 && k != m) {
                    graph.push_back(vector<index_t>({ (index_t)(current - n), (index_t)(current - 1), (index_t)(current + 1), (index_t)(current + n) }));
                }
            }
        }
    }
}


//Сортировка по длине путей
void sort_routs() {

}


//Вывод всех путей
void output_routs() {
    cout << "Пути: " << endl;
    for (auto const& lst : routs){
        for (auto const& node : lst)
        {
            cout << (int)node << " ";
        }
        cout << endl;
    }
}

void output_routs_2() {
    cout << endl;
    cout << "Кратчайшие пути: " << endl;
    for (auto const& lst : routs) {
        if (lst.size() < (sqrt(n_second) * 2)) {
            for (auto const& node : lst)
            {
                cout << (int)node << " ";
            }
            cout << endl;
        }
    }
}

//Вывод данных графа(соседних вершин для каждой вершины)
void output_graph(size_t N) {

    cout << "Список смежности графа: " << endl;
    for (auto n = 0; n < N; n++)
    {
        cout << n << ":\t";
        for (const auto& w : graph[n]) {
            cout << w << " ";
        }
        cout << endl;
    }
    cout << endl;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    ////Пример заполнения графа вручную (размер 3*3) (в паре вершина и ее "соседи")
    //graph.push_back(make_pair(1, vector<index_t>({ 2, 4 }));
    //graph.push_back(make_pair(2, vector<index_t>({ 1, 3, 5 }));
    //graph.push_back(make_pair(3, vector<index_t>({ 2,  6 }));
    //graph.push_back(make_pair(4, vector<index_t>({ 1, 5, 7 }));
    //graph.push_back(make_pair(5, vector<index_t>({ 2, 4, 6, 8 }));
    //graph.push_back(make_pair(6, vector<index_t>({ 3, 5, 9 }));
    //graph.push_back(make_pair(7, vector<index_t>({ 4, 8 }));
    //graph.push_back(make_pair(8, vector<index_t>({ 5, 7, 9 }));
    //graph.push_back(make_pair(9, vector<index_t>({ 6, 8 }));
    //n_first = 1; n_second = 9;

    ofstream out;          // поток для записи
    out.open(".\\results.txt");
    if (!out.is_open())
    {
        cout << "Невозможно записать данные в файл!" << endl;
    }
    //ввод графа
    input_graph();

    //output_graph();
    chrono::duration<double, milli> all_time{ 0 };
    const auto start_t = chrono::system_clock::now();

    list<index_t> visited;
    // поиск всех путей между парой вершин
    dfs(n_first, n_second, visited);
    const auto stop_t = chrono::system_clock::now();
    all_time = stop_t - start_t;
    //output_graph();
    cout << "Время выполнения поиска путей = " << all_time.count() << " мс" << endl << endl;
    cout << "Количество путей из " << (int)n_first << " вершины в " << (int)n_second << " вершину: "  << all << "." << endl << endl;
    //output_routs();
    /*out << "Время выполнения поиска путей = " << all_time.count() << " мс" << endl << endl;
    out << "Количество путей из " << n_first << " вершины в " <<(index_t) n_second << " вершину: " << all << "." << endl << endl;
   */
    output_routs();
    //out.close();

    output_routs_2();
    system("pause");
    return 0;
}


