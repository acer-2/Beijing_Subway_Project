#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include"head.h"

using namespace std;

int numOfLines;

struct Station
{
    string name; // 站点名称
    int line; // 所在地铁线路
    bool transfer; // 是否换乘
    vector<int> adj; // 相邻站点的索引

    Station(string name, int line, bool transfer)
    {
        this->name = name;
        this->line = line;
        this->transfer = transfer;
    }
};

vector<Station> stations; // 所有站点
vector<vector<int>> lines; // 所有地铁线路

// 读取地铁站点信息
void readStations(string inputFile) 
{
    ifstream input(inputFile);

    if (input.is_open()) {
        int numOfStations;
        input >> numOfStations;

        for (int i = 0; i < numOfStations; ++i) {
            int line, numStations;
            input >> line >> numStations;

            vector<int> lineStations;
            for (int j = 0; j < numStations; ++j) {
                string name;
                int transfer;
                input >> name >> transfer;

                // 存储站点信息
                stations.emplace_back(name, line, transfer);
                lineStations.push_back(stations.size() - 1);

                // 如果是换乘站，与其他线路的相邻站点相连
                if (transfer) {
                    for (auto it = stations.begin(); it != stations.end(); ++it) {
                        if (it->name == name && it->line != line) {
                            it->adj.push_back(stations.size() - 1);
                            stations.back().adj.push_back(distance(stations.begin(), it));
                            break;
                        }
                    }
                }

                // 与前一个站点相连
                if (j > 0) {
                    stations.back().adj.push_back(lineStations[j - 1]);
                    stations[lineStations[j - 1]].adj.push_back(stations.size() - 1);
                }
            }

            // 存储地铁线路信息
            lines.push_back(lineStations);
        }
    }
    input.close();
}

// 寻找两站点之间的最短路径，使用BFS算法
vector<int> findPath(int start, int end)
{
    vector<int> visited(stations.size(), -1);
    queue<int> q;
    visited[start] = start;
    q.push(start);

    while (!q.empty()) {
        int cur = q.front();
        q.pop();

        if (cur == end) break;

        // 遍历相邻站点
        for (int i = 0; i < stations[cur].adj.size(); ++i) {
            int next = stations[cur].adj[i];
            if (visited[next] == -1) { // 如果未被访问过，则加入队列
                visited[next] = cur;
                q.push(next);
            }
        }
    }

    // 从终点站往回寻找路径
    vector<int> path;
    for (int i = end; i != start; i = visited[i])
    {
        path.push_back(i);
    }
    path.push_back(start);

    reverse(path.begin(), path.end()); // 反转路径

    return path;
}

// 根据站点名称获取站点的索引
int getIndexByName(string name)
{
    for (int i = 0; i < stations.size(); ++i)
    {
        if (stations[i].name == name)
        {
            return i;
        }
    }
    return -1;
}

void LineSearch()
{
    string startName, endName;
    cout << "请输入起点站和终点站:\n";
    cin >> startName >> endName;

    int startIndex = getIndexByName(startName); // 获取起点站索引
    int endIndex = getIndexByName(endName); // 获取终点站索引

    if (startIndex == -1 || endIndex == -1) 
    {
        cout << "站点名称无效" << endl;
        LineSearch();
    }

    vector<int> path = findPath(startIndex, endIndex); // 获取最短路径

    if (!path.empty()) 
    {
        cout << "最短路线: ";
        for (int i = 0; i < path.size(); ++i) {
            cout << stations[path[i]].name << "(" << stations[path[i]].line << ")";
            if (i != path.size() - 1) cout << "->";
        }
        cout << endl;
    }
    else {
        cout << "没有找到路径" << endl;
        LineSearch();
    }
}

void LineNumSearch()
{
    string stationName;
    cout << "请输入站点名称:\n";
    cin >> stationName;

    int index = getIndexByName(stationName);
    if (index != -1)
    {
        cout << "站点 " << stationName << " 在 " << stations[index].line << " 号线上" << endl;
    }
    else
    {
        cout << "站点名称无效" << endl;
        LineNumSearch();
    }
}

// 查询某地铁线路
void LineTraver()
{
    int lineNumber;
    cout << "请输入地铁线路号:\n";
    cin >> lineNumber;

    

    bool foundLine = false;
    for (int i = 0; i < lines.size(); ++i) 
    {
        if (stations[lines[i][0]].line == lineNumber)
        {
            foundLine = true;
            cout << lineNumber << "号线路: ";
            for (int j = 0; j < lines[i].size(); ++j) 
            {
                cout << stations[lines[i][j]].name;
                if (j != lines[i].size() - 1) cout << "->";
            }
            cout << endl << "共 " << lines[i].size() << " 个站点" << endl;
            break;
        }
    }

    if (!foundLine) 
    {
        cout << "线路号无效" << endl;
        LineTraver();
    }
}

void func()
{

    while (true)// 循环展示用户选项菜单
    {
        cout << "请选择：\n";
        cout << "1.查询地铁路线；\n";
        cout << "2.查询站点所在线路；\n";
        cout << "3.查询某地铁线路；\n";
        cout << "4.退出程序；\n";

        int choice;
        cin >> choice;

        if (choice == 1)
        { // 查询路线
            LineSearch();
        }
        else if (choice == 2)
        { // 查询站点所在线路
            LineNumSearch();
        }
        else if (choice == 3)
        { // 查询地铁线路
            LineTraver();
        }
        else if (choice == 4)
        { // 退出程序
            cout << "程序已退出" << endl;
            return ;
        }
        else
        {
            cout << "请选择一个有效选项" << endl;
        }
    }
}

int main()
{
    readStations("bgstations.txt"); // 读取地铁站点信息

    func();

    return 0;
}