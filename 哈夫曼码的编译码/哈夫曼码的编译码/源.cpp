#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <string>

using namespace std;

// �������������ڵ�
struct HuffmanNode {
    char ch;
    int freq;
    HuffmanNode* left;
    HuffmanNode* right;
    HuffmanNode(char c, int f) : ch(c), freq(f), left(NULL), right(NULL) {}
    ~HuffmanNode() { delete left; delete right; }
};

// ������������
struct HuffmanCode {
    string code;
    int freq;
    HuffmanCode(string c, int f) : code(c), freq(f) {}
    HuffmanCode() : code(""), freq(0) {} // ���Ĭ�Ϲ��캯��
};


// ����������������ڵ�ȽϺ���
struct NodeCompare {
    bool operator()(const HuffmanNode* a, const HuffmanNode* b) const {
        return a->freq > b->freq;
    }
};

// �����������
HuffmanNode* buildHuffmanTree(map<char, int>& freqMap) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, NodeCompare> pq;

    for (auto it = freqMap.begin(); it != freqMap.end(); ++it) {
        pq.push(new HuffmanNode(it->first, it->second));
    }

    while (pq.size() > 1) {
        HuffmanNode* left = pq.top();
        pq.pop();
        HuffmanNode* right = pq.top();
        pq.pop();
        HuffmanNode* parent = new HuffmanNode('#', left->freq + right->freq);
        parent->left = left;
        parent->right = right;
        pq.push(parent);
    }

    return pq.top();
}

// �������������
void calculateHuffmanCode(HuffmanNode* node, string code,
    map<char, HuffmanCode>& codeMap) {
    if (node) {
        if (node->left == NULL && node->right == NULL) {
            codeMap[node->ch] = HuffmanCode(code, node->freq);
        }
        else {
            calculateHuffmanCode(node->left, code + "0", codeMap);
            calculateHuffmanCode(node->right, code + "1", codeMap);
        }
    }
}

// ��ȡ�ļ�����
string getFileContent(string fileName) {
    string content;
    char c;
    ifstream fin(fileName.c_str());
    while (fin.get(c)) {
        content += c;
    }
    fin.close();
    return content;
}

// ���ַ���д���ļ�
void writeToFile(string fileName, string content) {
    ofstream fout(fileName.c_str());
    fout << content;
    fout.close();
}

// �Ӽ��������ȡ�ַ�����Ȩֵ
void getInput(map<char, int>& freqMap) {
    // �����֮ǰ��Ƶ��ӳ��
    freqMap.clear();

    int n;
    cout << "�������ַ�����С��";
    cin >> n;

    int w;
    for (int i = 0; i < n; i++) {
        char ch;
        cout << "�������" << i + 1 << "���ַ���";
        cin.ignore(); // ������һ������Ľ�����
        cin.get(ch); // ʹ�� get() ��������ȡ������ַ�
        cout << "�������ַ� " << ch << " ��Ӧ��Ȩֵ��";
        cin >> w;
        freqMap[ch] = w;
    }
}

// ��ӡ������������
void printHuffmanCode(map<char, HuffmanCode>& codeMap) {
    cout << "�ַ�\t����������\t����Ƶ��" << endl;
    for (auto it = codeMap.begin(); it != codeMap.end(); ++it) {
        cout << it->first << "\t" << it->second.code << "\t" << "\t" << it->second.freq << endl;
    }
}

// ���ַ������й���������
string encode(string content, map<char, HuffmanCode>& codeMap) {
    string result;
    for (char c : content) {
        result += codeMap[c].code;
    }
    return result;
}

// �Թ����������������
string decode(string code, HuffmanNode* root) {
    string result;
    HuffmanNode* p = root;
    for (char c : code) {
        if (c == '0') {
            p = p->left;
        }
        else {
            p = p->right;
        }

        if (p->left == NULL && p->right == NULL) {
            result += p->ch;
            p = root;
        }
    }
    return result;
}



int main() {
    int choice;
    cout << "��ӭʹ�ù�������������" << endl;

    // ��ʼ��
    map<char, int> freqMap;
    map<char, HuffmanCode> codeMap;
    HuffmanNode* root = NULL;

    do {
        cout << "----------������������----------" << endl;
        cout << "**********��ѡ�������**********" << endl;
        cout << "*****1. ������������������******" << endl;
        cout << "*****2. �����ַ�����Ȩֵ********" << endl;
        cout << "*****3.���ַ������벢���浽�ļ�*" << endl;
        cout << "*****4.�ļ��ж�ȡ�����������***" << endl;
        cout << "*****5.��ӡ����������***********" << endl;
        cout << "*************0. �˳�************" << endl;
        cout << "--------------------------------" << endl;
        cout << "�����������";
        cin >> choice;
        switch (choice) {
        case 1:
            // ������������
            getInput(freqMap);
            delete root;  // ���֮ǰ�Ĺ�������
            root = buildHuffmanTree(freqMap);

            // �������������
            codeMap.clear();
            calculateHuffmanCode(root, "", codeMap);

            // ��ӡ������������
            printHuffmanCode(codeMap);
            break;

        case 2:
            // �����ַ�����Ȩֵ
            getInput(freqMap);
            delete root;
            root = buildHuffmanTree(freqMap);
            codeMap.clear();
            calculateHuffmanCode(root, "", codeMap);
            printHuffmanCode(codeMap);
            break;

        case 3:
        {
            if (root == NULL) {
                cout << "���Ƚ�����������������!" << endl;
                break;
            }
            string content;
            cout << "��������������ݣ�";
            cin.ignore();
            getline(cin, content);
            string encodedContent = encode(content, codeMap);
            writeToFile("encoded.txt", encodedContent);
            cout << "������Ϊ:" << encodedContent << endl;
            cout << "�ѽ����������浽�ļ�encoded.txt�С�" << endl;
        }
        break;

        case 4:
        {
            if (root == NULL) {
                cout << "���Ƚ����������������룡" << endl;
                break;
            }
            string encodedText = getFileContent("encoded.txt");
            string decodedText = decode(encodedText, root);
            writeToFile("decoded.txt", decodedText);
            cout << "������Ϊ��" << encodedText << "--" << decodedText << endl;
            cout << "�ѽ����������浽�ļ�decoded.txt�С�" << endl;
        }
        break;
        case 5:        printHuffmanCode(codeMap);
            break;
        case 0:

            delete root;
            cout << "��ӭ�´�ʹ�ù�����������";
            break;
        default:
            cout << "��Ч����������������";
            break;
        }
    } while (choice != 0);
    return 0;
}
