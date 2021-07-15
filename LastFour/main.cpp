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
#include<queue>
#include<stack>

using namespace std;
// Global varaibles
queue<string> correct_tag_same;// to overwrite the wrong data by the correct one in the same line
queue <int> line_num_same;//to know the location of the error
queue<int> modif_index_same;// to specify the modification index in the same line
//--------------------------------------------------------------------------------------
queue<string> correct_tag_outline;
queue <int> line_num_outline;
//--------------------------------------------------------------------------------------
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
// Functions
vector <string> read_xml_file(string FileName) {
	fstream  file;
	vector <string> v;
	string s;
	file.open(FileName, ios::in);
	if (file.is_open()) {
		cout << "the file is opened"<<endl;
		while (getline(file, s)) {
			v.push_back(s);
		}
		file.close();
		return v;
	}
	else {
		cout << "error when opening the file";
		exit(1);
	}
	
}
vector<string> fix_outline(vector<string> xml_data) {
	while (!line_num_outline.empty()) {
		string old;
		old = xml_data[line_num_outline.front()-1];
		xml_data[line_num_outline.front()-1] = old+"</" + correct_tag_outline.front() + ">";
		correct_tag_outline.pop(); line_num_outline.pop();
	}
	return xml_data;
}
vector<string> fix_inline(vector <string> xml_data) {
	
	while (!line_num_same.empty()) {
		int length = 0;// to know the length of the wrong tag to erase it
		int lineNum = line_num_same.front();
		int wrong_index = modif_index_same.front();
		modif_index_same.pop();
		line_num_same.pop();
		string wrong_line = xml_data[lineNum];
		// finding the length of the wrong tag
		for (int i = wrong_index; i < wrong_line.size(); i++) {
			if (wrong_line[i] != ' ' && wrong_line[i] != '>')
				length++;
		}
		// erase the wrong tag
		xml_data[lineNum].erase(wrong_index,length+1);
		// write the correct one 
		xml_data[lineNum] += correct_tag_same.front() + '>';
		correct_tag_same.pop();
	}
	return xml_data;
}
vector<string> fix_expersion_error(vector<string> xml_data) {
	stack<char> exp;
	int start_index;
	int length = 0;
	for (int i = 0; i < xml_data.size(); i++) {
		string xml_line = xml_data[i];
		int line_size = xml_line.size();
		if (xml_line[line_size - 1] != '>'&& xml_line[line_size - 1] != '\t') {
			cout << "there is experssion error in line " << i<<endl;
			int count = line_size - 1;
			while (xml_line[count] != '>') {
				length++;
				exp.push(xml_line[count]);
				count--;
			}
			start_index = count+1;
			// erase the expersion
			xml_data[i].erase(start_index, length);
		}
	}
	return xml_data;
}
vector<string> FixAll(vector<string> xml_data){
	xml_data = fix_expersion_error(xml_data);
	xml_data = fix_outline(xml_data);
	xml_data = fix_inline(xml_data);
	return xml_data;
}
void detect_syn_error(vector<string> xml_data) {
	stack <string>  tag_stack;
	stack <int> line_index;
	//vector<string> xml_data = data;//make a copy to be able to fix the error in the source code
	int index = 0;// to know the number of the line where you are
	string xml_line;// get line by line
	
	for(int k=0;k < xml_data.size();k++) {
		xml_line = xml_data[k]; 
		for (int i = 0; i < xml_line.size(); i++) {
			// pushing in the stack
			if (xml_line[i] == '<' && xml_line[i + 1] != '!' && xml_line[i + 1] != '?' && xml_line[i + 1] != '/') {
				string tag;
				int j = i+1;// to cut the chars of the tag;
				while (xml_line[j] != ' '&& xml_line[j] != '>') {
					tag += xml_line[j];
					j++;
				}
				tag_stack.push(tag);
				line_index.push(index);
			}
			//poping from the stack
			else if (xml_line[i] == '<' && xml_line[i + 1] == '/') {
				string tag;
				int j = i + 2;
				while (xml_line[j] != ' ' && xml_line[j] != '>') {
					tag += xml_line[j];
					j++;
				}
				if (tag_stack.top() == tag) {
					tag_stack.pop();
					line_index.pop();
				}
				//if you are in the same line
				else if(line_index.top()==index) {
					cout << "there is wrong closing tag in the line : " << index << endl;
					cout << "the correction is </" << tag_stack.top()<<">"<< endl;
					correct_tag_same.push(tag_stack.top());
					line_num_same.push(index);
					tag_stack.pop();
					line_index.pop();
					modif_index_same.push(i + 2);//save the start address of the wrong tag
				}
			}

		}
		//check if the string is empty
		// missing closing tag case
		int line_size = xml_line.size();
		if (line_size != 0) {
			if ( xml_line[line_size - 1] == '\t'&& !tag_stack.empty()) {
				correct_tag_outline.push(tag_stack.top());
				tag_stack.pop();
				line_num_outline.push(index);
				cout << "there is missing closed tag at line :" << index ;
				cout <<" the correction is :</" <<correct_tag_outline.front() <<">"<< endl;
				
			}
		}
		else if (index == xml_data.size() - 1) {
			correct_tag_outline.push(tag_stack.top());
			tag_stack.pop();
			line_num_outline.push(index);
			cout << correct_tag_outline.front();
			cout << index;
		}
		index++;
		
	}
}
void Create_correct_xml_file(vector<string> xml_data,string FileName) {
	fstream file;
	//file.open("corrected_structure.xml", ios::out);
	file.open(FileName, ios::out);
	// If no file is created, then
	// show the error message.
	if (file)
	{
		cout << "File created successfully.";
		for (int i = 0; i < xml_data.size(); ++i) {

			file<< xml_data[i] << endl; 
		}
	}
	else {
		cout << "Error in creating file!!!";
	}
	file.close();
}

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
