#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <string>

using namespace std;

// 哈夫曼编码树节点
struct HuffmanNode {
    char ch;
    int freq;
    HuffmanNode* left;
    HuffmanNode* right;
    HuffmanNode(char c, int f) : ch(c), freq(f), left(NULL), right(NULL) {}
    ~HuffmanNode() { delete left; delete right; }
};

// 哈夫曼编码结果
struct HuffmanCode {
    string code;
    int freq;
    HuffmanCode(string c, int f) : code(c), freq(f) {}
    HuffmanCode() : code(""), freq(0) {} // 添加默认构造函数
};


// 定义哈夫曼编码树节点比较函数
struct NodeCompare {
    bool operator()(const HuffmanNode* a, const HuffmanNode* b) const {
        return a->freq > b->freq;
    }
};

// 构造哈夫曼树
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

// 计算哈夫曼编码
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

// 获取文件内容
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

// 将字符串写入文件
void writeToFile(string fileName, string content) {
    ofstream fout(fileName.c_str());
    fout << content;
    fout.close();
}

// 从键盘输入获取字符集和权值
void getInput(map<char, int>& freqMap) {
    // 先清空之前的频率映射
    freqMap.clear();

    int n;
    cout << "请输入字符集大小：";
    cin >> n;

    int w;
    for (int i = 0; i < n; i++) {
        char ch;
        cout << "请输入第" << i + 1 << "个字符：";
        cin.ignore(); // 忽略上一次输入的结束符
        cin.get(ch); // 使用 get() 函数来获取输入的字符
        cout << "请输入字符 " << ch << " 对应的权值：";
        cin >> w;
        freqMap[ch] = w;
    }
}

// 打印哈夫曼编码结果
void printHuffmanCode(map<char, HuffmanCode>& codeMap) {
    cout << "字符\t哈夫曼编码\t出现频次" << endl;
    for (auto it = codeMap.begin(); it != codeMap.end(); ++it) {
        cout << it->first << "\t" << it->second.code << "\t" << "\t" << it->second.freq << endl;
    }
}

// 对字符串进行哈夫曼编码
string encode(string content, map<char, HuffmanCode>& codeMap) {
    string result;
    for (char c : content) {
        result += codeMap[c].code;
    }
    return result;
}

// 对哈夫曼编码进行译码
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
    cout << "欢迎使用哈夫曼编码器！" << endl;

    // 初始化
    map<char, int> freqMap;
    map<char, HuffmanCode> codeMap;
    HuffmanNode* root = NULL;

    do {
        cout << "----------哈夫曼编码器----------" << endl;
        cout << "**********请选择操作：**********" << endl;
        cout << "*****1. 建立哈夫曼树及编码******" << endl;
        cout << "*****2. 更新字符集和权值********" << endl;
        cout << "*****3.将字符串编码并保存到文件*" << endl;
        cout << "*****4.文件中读取编码进行译码***" << endl;
        cout << "*****5.打印哈夫曼编码***********" << endl;
        cout << "*************0. 退出************" << endl;
        cout << "--------------------------------" << endl;
        cout << "请输入操作：";
        cin >> choice;
        switch (choice) {
        case 1:
            // 建立哈夫曼树
            getInput(freqMap);
            delete root;  // 清空之前的哈夫曼树
            root = buildHuffmanTree(freqMap);

            // 计算哈夫曼编码
            codeMap.clear();
            calculateHuffmanCode(root, "", codeMap);

            // 打印哈夫曼编码结果
            printHuffmanCode(codeMap);
            break;

        case 2:
            // 更新字符集和权值
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
                cout << "请先建立哈夫曼树及编码!" << endl;
                break;
            }
            string content;
            cout << "请输入待编码内容：";
            cin.ignore();
            getline(cin, content);
            string encodedContent = encode(content, codeMap);
            writeToFile("encoded.txt", encodedContent);
            cout << "编码结果为:" << encodedContent << endl;
            cout << "已将编码结果保存到文件encoded.txt中。" << endl;
        }
        break;

        case 4:
        {
            if (root == NULL) {
                cout << "请先建立哈夫曼树及编码！" << endl;
                break;
            }
            string encodedText = getFileContent("encoded.txt");
            string decodedText = decode(encodedText, root);
            writeToFile("decoded.txt", decodedText);
            cout << "译码结果为：" << encodedText << "--" << decodedText << endl;
            cout << "已将译码结果保存到文件decoded.txt中。" << endl;
        }
        break;
        case 5:        printHuffmanCode(codeMap);
            break;
        case 0:

            delete root;
            cout << "欢迎下次使用哈夫曼编码器";
            break;
        default:
            cout << "无效操作，请重新输入";
            break;
        }
    } while (choice != 0);
    return 0;
}
