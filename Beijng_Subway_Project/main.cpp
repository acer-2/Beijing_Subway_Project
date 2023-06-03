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
    string name; // վ������
    int line; // ���ڵ�����·
    bool transfer; // �Ƿ񻻳�
    vector<int> adj; // ����վ�������

    Station(string name, int line, bool transfer)
    {
        this->name = name;
        this->line = line;
        this->transfer = transfer;
    }
};

vector<Station> stations; // ����վ��
vector<vector<int>> lines; // ���е�����·

// ��ȡ����վ����Ϣ
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

                // �洢վ����Ϣ
                stations.emplace_back(name, line, transfer);
                lineStations.push_back(stations.size() - 1);

                // ����ǻ���վ����������·������վ������
                if (transfer) {
                    for (auto it = stations.begin(); it != stations.end(); ++it) {
                        if (it->name == name && it->line != line) {
                            it->adj.push_back(stations.size() - 1);
                            stations.back().adj.push_back(distance(stations.begin(), it));
                            break;
                        }
                    }
                }

                // ��ǰһ��վ������
                if (j > 0) {
                    stations.back().adj.push_back(lineStations[j - 1]);
                    stations[lineStations[j - 1]].adj.push_back(stations.size() - 1);
                }
            }

            // �洢������·��Ϣ
            lines.push_back(lineStations);
        }
    }
    input.close();
}

// Ѱ����վ��֮������·����ʹ��BFS�㷨
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

        // ��������վ��
        for (int i = 0; i < stations[cur].adj.size(); ++i) {
            int next = stations[cur].adj[i];
            if (visited[next] == -1) { // ���δ�����ʹ�����������
                visited[next] = cur;
                q.push(next);
            }
        }
    }

    // ���յ�վ����Ѱ��·��
    vector<int> path;
    for (int i = end; i != start; i = visited[i])
    {
        path.push_back(i);
    }
    path.push_back(start);

    reverse(path.begin(), path.end()); // ��ת·��

    return path;
}

// ����վ�����ƻ�ȡվ�������
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
    cout << "���������վ���յ�վ:\n";
    cin >> startName >> endName;

    int startIndex = getIndexByName(startName); // ��ȡ���վ����
    int endIndex = getIndexByName(endName); // ��ȡ�յ�վ����

    if (startIndex == -1 || endIndex == -1) 
    {
        cout << "վ��������Ч" << endl;
        LineSearch();
    }

    vector<int> path = findPath(startIndex, endIndex); // ��ȡ���·��

    if (!path.empty()) 
    {
        cout << "���·��: ";
        for (int i = 0; i < path.size(); ++i) {
            cout << stations[path[i]].name << "(" << stations[path[i]].line << ")";
            if (i != path.size() - 1) cout << "->";
        }
        cout << endl;
    }
    else {
        cout << "û���ҵ�·��" << endl;
        LineSearch();
    }
}

void LineNumSearch()
{
    string stationName;
    cout << "������վ������:\n";
    cin >> stationName;

    int index = getIndexByName(stationName);
    if (index != -1)
    {
        cout << "վ�� " << stationName << " �� " << stations[index].line << " ������" << endl;
    }
    else
    {
        cout << "վ��������Ч" << endl;
        LineNumSearch();
    }
}

// ��ѯĳ������·
void LineTraver()
{
    int lineNumber;
    cout << "�����������·��:\n";
    cin >> lineNumber;

    

    bool foundLine = false;
    for (int i = 0; i < lines.size(); ++i) 
    {
        if (stations[lines[i][0]].line == lineNumber)
        {
            foundLine = true;
            cout << lineNumber << "����·: ";
            for (int j = 0; j < lines[i].size(); ++j) 
            {
                cout << stations[lines[i][j]].name;
                if (j != lines[i].size() - 1) cout << "->";
            }
            cout << endl << "�� " << lines[i].size() << " ��վ��" << endl;
            break;
        }
    }

    if (!foundLine) 
    {
        cout << "��·����Ч" << endl;
        LineTraver();
    }
}

void func()
{

    while (true)// ѭ��չʾ�û�ѡ��˵�
    {
        cout << "��ѡ��\n";
        cout << "1.��ѯ����·�ߣ�\n";
        cout << "2.��ѯվ��������·��\n";
        cout << "3.��ѯĳ������·��\n";
        cout << "4.�˳�����\n";

        int choice;
        cin >> choice;

        if (choice == 1)
        { // ��ѯ·��
            LineSearch();
        }
        else if (choice == 2)
        { // ��ѯվ��������·
            LineNumSearch();
        }
        else if (choice == 3)
        { // ��ѯ������·
            LineTraver();
        }
        else if (choice == 4)
        { // �˳�����
            cout << "�������˳�" << endl;
            return ;
        }
        else
        {
            cout << "��ѡ��һ����Чѡ��" << endl;
        }
    }
}

int main()
{
    readStations("bgstations.txt"); // ��ȡ����վ����Ϣ

    func();

    return 0;
}