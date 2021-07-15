#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <map>
#include <bitset>
#include <regex>

using namespace std;

class Tag{
public:
    string name;
    map<string, string> attributes;
    vector<Tag *> childeren;
    string text;
    Tag * parent;

    Tag(string n, map<string, string> attr, string t)
    {
        name = n;
        attributes = attr;
        text = t;
    }
};

map<char, int> frequency;
class Node{
public:
    Node * parent;
    Node * left;
    Node * right;
    char key;
    int freq;
    string name;
    Node(Node * p, Node * l, Node * r, int k, int f)
    {
        parent = p;
        left = l;
        right = r;
        key = k;
        freq = f;
    }
};

void postOrder(Node * root)
{
    if(root->left != nullptr) postOrder(root->left);
    //cout << root->key << " ";
    if(root->right != nullptr) postOrder(root->right);
    if(root->left == nullptr && root->right == nullptr) cout << root->key << " ";
}

bool cmp(pair<char, int>& a,
         pair<char, int>& b)
{
    return a.second < b.second;
}

bool compare(Node * a, Node * b)
{
    return a->freq < b->freq;
}

// Function to sort the map according
// to value in a (key-value) pairs
vector<pair<char, int> > sortChars(map<char, int>& M)
{
    vector<pair<char, int> > result;
    for (auto& it : M) {
        result.push_back(it);
    }
    sort(result.begin(), result.end(), cmp);
    return result;
}

void updateMap(string textLine)
{
    for(char c : textLine)
    {
        frequency[c] +=1;
    }
}

vector<pair<char, int> > getAlphabet(string alphabetSource)
{
    string text;
    ifstream MyReadFile(alphabetSource);
    while (getline (MyReadFile, text)) {
        //frequency['\n'] +=1;
        updateMap(text);
    }
    MyReadFile.close();
    vector<pair<char, int> > results = sortChars(frequency);
    return results;
}

Node * getBinary(Node * root, char key, string & binaryCode)
{
    if(root->key==key) return root;
    else
    {
        if(root->left->name.find(key) != string::npos)
        {
            //cout << "0";
            binaryCode += "0";
            return getBinary(root->left, key, binaryCode);
        }
        else
        {
            //cout << "1";
            binaryCode += "1";
            return getBinary(root->right, key, binaryCode);
        }
    }
}

string extract(Node * root, string text)
{
    Node * head = root;
    string result = "";
    for(int i=0; i<text.size(); i++)
    {
        if(text[i]=='0') head = head->left;
        else head = head->right;

        if(head->left== nullptr && head->right== nullptr)
        {
            result.push_back(head->key);
            head = root;
        }
    }
    return result;
}

string code(Node * root, string text)
{
    string result = "";

    for(int i=0; i<text.size(); i++)
    {
        string binaryCode = "";
        getBinary(root, text[i], binaryCode);
        result += binaryCode;
    }
    return result;
}

int codedBinay(string binaryText, string binaryResultFilePath)
{
    int intNumber = 0;
    ofstream binaryResultFile(binaryResultFilePath, ios::out | ios::binary);
    ofstream integersFile("../integers.txt"); // for debugging
    int base = 0;
    int offset = 31; // should be 31
    string target = "";

    while(base+offset<binaryText.size())
    {
        target = binaryText.substr(base, offset);
        integersFile << target << endl;
        int outInteger = stoi(target, nullptr, 2);
        integersFile << outInteger << endl;
        intNumber++;
        binaryResultFile.write( (char*)&outInteger, sizeof(int));
        base += offset;
    }

    if(base != binaryText.size()-1)
    {
        target = binaryText.substr(base, binaryText.size()-base);
        integersFile << target << endl;
        int outInteger = stoi(target, 0, 2);
        integersFile << outInteger << endl;
        intNumber++;
        binaryResultFile.write( (char*)&outInteger, sizeof(int));
    }
    binaryResultFile.close();
    return intNumber;
}

void minifyFile(string filePath, string modifiedSourceFilePath)
{
    string text = "";
    string textLine = "";
    ifstream MyReadFile(filePath);
    regex r("\\s{2,}");
    while (getline(MyReadFile, textLine)) {
        text += regex_replace(textLine, r, " ");
    }
    MyReadFile.close();

    ofstream outFile(modifiedSourceFilePath);
    outFile << text;
    outFile.close();
}

Node * buildCodeTree(string alphabetSourceFilePath)
{
    vector<pair<char, int>> results = getAlphabet(alphabetSourceFilePath);
    vector<Node *> nodes;
    for(auto result : results)
    {
        Node * node = new Node(nullptr, nullptr, nullptr, result.first, result.second);
        node->name = node->key;
        nodes.push_back(node);
    }

    while(nodes.size()>1)
    {
        Node * right = nodes[0];
        Node * left = nodes[1];
        nodes.erase(nodes.begin());
        nodes.erase(nodes.begin());
        Node * parent = new Node(nullptr, left, right, '\0', left->freq+right->freq);
        parent->name += left->name + right->name;
        nodes.push_back(parent);
        sort(nodes.begin(), nodes.end(), compare);
    }
    return nodes[0];
}

void writeCodedText(string modifiedSourceFilePath, string textCodeFilePath, Node * root)
{
    string sourceFileLine;
    ifstream sourceFile(modifiedSourceFilePath);
    ofstream distenationFile(textCodeFilePath);

    while (getline (sourceFile, sourceFileLine)) {
        // Output the text from the file
        string textLine = sourceFileLine ; // not adding "\n" leaves only white spaces and results in one string
        string codedText = code(root, textLine);
        distenationFile << codedText;
    }
    sourceFile.close();
    distenationFile.close();
}

void extractCodedText(string textCodeFilePath, string decodedFilePath, Node * root)
{
    string textCodeFileLine;
    ifstream textCodeFile(textCodeFilePath);
    ofstream decodedFile(decodedFilePath);

    while (getline (textCodeFile, textCodeFileLine)) {
        string decodedText = extract(root, textCodeFileLine);
        decodedFile << decodedText; // this i think mostly appends no new lines
    }
    textCodeFile.close();
    decodedFile.close();
}

int codeBinaryFromText(string textCodeFilePath, string binaryResultFilePath)
{
    string sourceTextCodedFileLine;
    ifstream sourceTextCodedFile(textCodeFilePath);
    int linesNum = 0;
    while (getline(sourceTextCodedFile, sourceTextCodedFileLine)) {
        linesNum = codedBinay(sourceTextCodedFileLine, binaryResultFilePath);
    }
    sourceTextCodedFile.close();
    return linesNum;
}

void decompressBinaryFile(string binarySourceFilePath, string textDistnationFilePath,int linesNum, Node * alphabetRoot)
{
    ifstream binaryResultFile(binarySourceFilePath, ios::out | ios::binary);
    ofstream decodedbinaryFile(textDistnationFilePath);
    int codedNumber;
    string allText = "";
    for(int i=0; i<linesNum; i++)
    {
        binaryResultFile.read((char *) &codedNumber, sizeof(int));
        string str = bitset<31>(codedNumber).to_string();
        allText += str;
    }
    string decodedText = extract(alphabetRoot, allText);
    decodedbinaryFile << decodedText; // this i think mostly appends no new lines
    binaryResultFile.close();
    decodedbinaryFile.close();
}

int compresstoBinaryFile(string modifiedSourceFilePath, Node * alphabetRoot)
{
    string textCodeFilePath = "../textCoded.txt";
    string decodedTextFilePath = "../decodedText.txt";
    string binaryResultFilePath = "../BinaryCoded.dat";

    writeCodedText(modifiedSourceFilePath, textCodeFilePath, alphabetRoot);
    extractCodedText(textCodeFilePath, decodedTextFilePath, alphabetRoot);
    int linesNum = codeBinaryFromText(textCodeFilePath, binaryResultFilePath);
    return linesNum;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////


vector<string> splitString(string str, char delimiter)
{
    vector<string> words{};

    istringstream ss(str);
    string word;
    while (getline(ss, word, delimiter)){
        //word.erase(remove_if(word.begin(), word.end(), ispunct), word.end());
        words.push_back(word);
    }
    return words;
}

string getTagName(string text)
{
    string result;
    smatch sm;
    regex r("[\-_A-Za-z]+");
    if(regex_search(text, sm, r)) {
        result = sm[0];
    }
    return result;
}

vector<string> getattributes(string text)
{
    vector<string> result;
    smatch sm;
    regex r("[_A-Za-z]+=\"(.*?)\"");
    while(regex_search(text, sm, r)) {
        result.push_back(sm[0]);
        regex q((string)sm[0]);
        text = regex_replace(text, q, "*");
    }
    return result;
}

bool isSelfClosingTag(string text, int pos)
{
    int index = text.find('>', pos);
    return text[pos+1] == '?' || text[index-1] == '/';
}

Tag * addTag(string text, int pos)
{
    // tag attributes
    int index = text.find('>', pos);
    string openTagText = text.substr(pos+1, index - pos - 1);

    // tag text
    int otherIndex = text.find('<', index);
    string tagText = text.substr(index+1, otherIndex - index - 1);

    vector<string> attr = getattributes(openTagText);
    string tagName = getTagName(openTagText);

    map<string, string> attrVals;
    for(int i=0; i<attr.size(); i++)
    {
        //cout << attr[i] << endl;
        vector<string> p = splitString(attr[i], '=');
        attrVals[p[0]] = p[1];
    }
    Tag * tag = new Tag(tagName, attrVals, tagText);
    return tag;
}

Tag * addSelfClosingTag(string text, int pos)
{
    if(text[pos+1] == '?') return addTag(text, pos+1);
    else return addTag(text, pos);
}

int identDepth = 0;
void printIdent(string & text)
{
    for(int i =0; i< identDepth; i++)
    {
        text += "\t";
    }
}

void printJSON(Tag * tag, string &text)
{
    // print name
    printIdent(text);
    //cout << "\"" + tag->name + "\":{" << endl;
    text += "\"" + tag->name + "\":{\n";

    // print text
    identDepth++;
    printIdent(text);
    //cout << "\"#text\": \"" + tag->text + "\"" << "," << endl;
    text += "\"#text\": \"" + tag->text + "\"" + ",\n";

    // print attributes
    for(auto attrPair : tag->attributes)
    {
        printIdent(text);
        //cout
        text+= "\"@" + attrPair.first + "\": " + attrPair.second + "," + "\n";
    }

    //print children
    for(auto child : tag->childeren)
    {
        printJSON(child, text);
    }
    //close
    identDepth--;
    printIdent(text);
    text += "},\n";
}

void printXML(Tag * tag, string &text)
{
    // print name
    printIdent(text);
    text += "<" + tag->name + " ";

    // print attributes
    for(auto attrPair : tag->attributes)
    {
        text += attrPair.first + "=" + attrPair.second +" ";
    }
    text = text.substr(0, text.size()-1);
    text += ">\n";

    // print text
    if(tag->text.size()>0 && tag->text.compare(" "))
    {
        identDepth++;
        printIdent(text);
        text += tag->text + "\n";
        identDepth--;
    }

    //print children
    for(auto child : tag->childeren)
    {
        identDepth++;
        printXML(child, text);
        identDepth--;
    }
    //close
    printIdent(text);
    text += "</" + tag->name + ">" + "\n";
}

void formatXML(Tag * rootTag, string xmlFilePath)
{
    ofstream xmlFile(xmlFilePath);
    string XMLtext = "";
    printXML(rootTag, XMLtext);
    xmlFile << XMLtext; // this i think mostly appends no new lines
    xmlFile.close();
}

void formatJOSN(Tag * rootTag, string JSONfilePath)
{
    ofstream jsonFile(JSONfilePath);
    string JSONtext = "";
    printJSON(rootTag, JSONtext);
    jsonFile << JSONtext; // this i think mostly appends no new lines
    jsonFile.close();
}

Tag * buildParseTree(string modifiedSourceFilePath)
{
    Tag * parseTreeHead = nullptr;
    string mainString;
    ifstream textCodeFile(modifiedSourceFilePath);
    while (getline (textCodeFile, mainString));
    textCodeFile.close();

    Tag * ParseTreeRoot;
    for(int i=0; i<mainString.size(); i++)
    {
        if(mainString[i] == '<')
        {
            if(mainString[i+1] == '/') // closing tag
            {
                //cout << "parent : ";
                ParseTreeRoot = parseTreeHead; // to catch the root before being null

                parseTreeHead = parseTreeHead->parent;
                if(parseTreeHead != nullptr)
                {
                    //cout << parseTreeRoot->name << endl;
                }
            }
            else if(mainString[i+1] == '!') continue;

            else if(isSelfClosingTag(mainString, i))
            {
                //cout << "found self closing tag" << endl;

                Tag * tag = addSelfClosingTag(mainString, i);
                Tag * parent = parseTreeHead;
                tag->parent = parent; // new tag know its parent

                parseTreeHead->childeren.push_back(tag); // stay at parent level

            }

            else // opening tag
            {
                Tag * tag = addTag(mainString, i);

                Tag * parent = parseTreeHead;
                tag->parent = parent; // new tag know its parent

                if(parseTreeHead == nullptr) // if root
                {
                    parseTreeHead = tag;
                    //cout << "root :" << parseTreeRoot-> name << endl;
                }
                else // if not root make it child
                {
                    parseTreeHead->childeren.push_back(tag);
                    parseTreeHead = tag;
                    //cout << "child : " << tag-> name << " " + tag->text << endl;
                }
                //if self-close move to parent
            }
        }


    }
    return ParseTreeRoot;
}

int main() {

    // data files - uncomment any one to be used instead
    string sourceFilePath = "../data-sample.xml";
    //string sourceFilePath = "../data-sample2.xml";
    //string sourceFilePath = "../data.adj.xml";
    //string sourceFilePath = "../xml.txt";

    string modifiedSourceFilePath = "../modifiedSource.txt";
    //string textCodeFilePath = "../textCoded.txt";
    //string decodedTextFilePath = "../decodedText.txt";
    string binaryResultFilePath = "../BinaryCoded.dat"; // the compressed file
    string binaryDecodedFilePath = "../decodedBinary.txt"; // after decompression
    string formtattedXMLfilePath = "../formattedXML.xml"; // after formatting
    string JSONfilePath = "../json.json"; // JSON results file

    // the user should always do minify before any
    // 1- formatting
    // 2- xml --> json
    // 3- compression

    // reduces file size
    // TODO : a button for this function called Minify
    minifyFile(sourceFilePath, modifiedSourceFilePath);

    // tree of ascii chars, used as the codes references
    Node * alphabetRoot = buildCodeTree(modifiedSourceFilePath);

     // the following commented functions is an intermediate code augmented in the not commented functions
     // left only for clarification

    //writeCodedText(modifiedSourceFilePath, textCodeFilePath, alphabetRoot);
    //extractCodedText(textCodeFilePath, decodedTextFilePath, alphabetRoot);
    //int linesNum = codeBinaryFromText(textCodeFilePath, binaryResultFilePath);

    // linesNum tells how many binary files to be converted
    // TODO : a button for this function called Compress
    int linesNum = compresstoBinaryFile(modifiedSourceFilePath, alphabetRoot);

    // decompresses the binary file into text file
    // TODO : a button for this function called Decompress
    decompressBinaryFile(binaryResultFilePath, binaryDecodedFilePath, linesNum, alphabetRoot);


    //xml parse tree must be calculated
    Tag * rootTag = buildParseTree(modifiedSourceFilePath);

    // formats the file but the file's extra white space must removed first before using it
    // using minify function - see first fucntion in main -
    // TODO : a button for this function called Format
    formatXML(rootTag, formtattedXMLfilePath);

    // TODO : a button for this function called JSON
    formatJOSN(rootTag, JSONfilePath);

    return 0;
}
