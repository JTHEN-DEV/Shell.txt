#include <bits/stdc++.h>

using namespace std;

vector<string> data;
// vector<string> data = {"Home", "-Documents", "--Document1.txt", "--Document2.docx", "--SubFolder", "---SubSubFolder", "---SubFile.txt", "-Music", "--File1.mp3", "--File2.mp3", "-Pictures", "--Img1.png", "--Img2.png", "--Img3.jpeg"};
vector<string> path = {"Home"};
int idx = 0;

void save(){
    ofstream output_file("data.txt");
    ostream_iterator<string> output_iterator(output_file, "\n");
    copy(data.begin(), data.end(), output_iterator);
}

void read(){
    ifstream in("data.txt");
    string str;
    data.clear();
    path.clear();
    while (getline(in, str)){
        if(str.size() > 0)
        data.push_back(str);
    }
    path.push_back(data[0]);
}

vector<string> getdir(int idx, int level){
    vector<string> out;
    for (int i = idx+1; i < data.size(); i++){
        if (data[i].substr(0, level+1) == string(level+1, '-') && data[i][level+1] != '-'){
            out.push_back(data[i].substr(level+1, data[i].length()-level-1));
        } else if (data[i][level] != '-'){
            break;
        }
    }
    return out;
}

map<string, int> getdirmap(int idx, int level){
    map<string, int> out;
    for (int i = idx+1; i < data.size(); i++){
        if (data[i].substr(0, level+1) == string(level+1, '-') && data[i][level+1] != '-'){
            out.insert({(data[i].substr(level+1, data[i].length()-level-1)), i});
        } else if (data[i][level] != '-'){
            break;
        }
    }
    return out;
}

string getpath(){
    string out = "";
    for (string s : path){
        out += "/" + s;
    }
    if (out == ""){
        out = "/";
    }
    return out;
}

void cd(string args);

void ls(int ix, vector<string> args){
    if (!args.empty()){
        string pwdbak = getpath();
        cd(args[0]);
        ls(idx, {});
        cd(pwdbak);
        return;
    }
    int level = path.size() - 1;
    vector<string> out = getdir(ix, level);
    for (string s : out){
        cout << s << endl;
    }
}

void abscd(string arg){
    path = {};
    idx = -1;
    cd(arg.substr(1, arg.length()-1));
}

void cd(string args){
    if (args == "" || args.empty()){
        return;
    }
    if (args[0] == '/'){
        abscd(args);
        return;
    }
    string segment;
    stringstream test(args);
    vector<string> seglist;
    while(getline(test, segment, '/'))
    {
        seglist.push_back(segment);
    }
    string arg = seglist[0];
    if (args.find('.') != string::npos){
        cout << "ERROR: DIRECTORY \'" << arg << "\' NOT FOUND" << endl;
        return;
    }
    seglist.erase(seglist.begin());
    string newarg = "";
    for (string abc : seglist){
        newarg += "/" + abc;
    }
    if (newarg != ""){
        newarg = newarg.substr(1, newarg.length()-1);
    }
    bool flag = true;
    for(pair<string, int> s : getdirmap(idx, path.size()-1)){
        if (s.first == arg){
            flag = false;
            path.push_back(arg);
            idx = s.second;
        }
    }
    if (flag){
        cout << "ERROR: DIRECTORY \'" << arg << "\' NOT FOUND" << endl;
        return;
    }
    cd(newarg);
}

void pwd(){
    cout << getpath() << endl;
}

vector<string> parse_keywords(vector<string> args){
    vector<string> out;
    vector<string> path2(path);
    if (!path.empty()){
        path2.erase(path2.end());
    }
    for (string s : args){
        if (s == ".."){
            string o = "";
            for (string a : path2){
                o += "/" + a;
            }
            if (o == "") o = "/";
            out.push_back(o);
        } else if (s == "."){
            out.push_back(getpath());
        } else if (s.substr(0, 2) == "..") {
            int n = 0;
            int end = 0;
            for (int i = 0; i < s.length(); i+= 3){
                if (s[i] == '.' && s[i+1] == '.' && s[i+2] == '/'){
                    n++;
                } else {
                    end = i;
                    break;
                }
            }
            string o = "";
            if (end == 0){
                for (int i = 0; i < n-1; i++){
                    path2.erase(path2.end());
                }
                for (string a : path2){
                    o += "/" + a;
                }
                if (o == "") o = "/";
                out.push_back(o);
            } else {
                for (int i = 0; i < n-1; i++){
                    path2.erase(path2.end());
                }
                for (string a : path2){
                    o += "/" + a;
                }
                if (o == "") o = "/";
                o += "/" + s.substr(end, s.length()-end);
                out.push_back(o);
            }
        } else {
            out.push_back(s);
        }
    }
    return out;
}

void mkdir(){
    cout << "Making a directory" << endl;
}

void cmd(string ins){
    // CHECK IF STRING IS EMPTY
    bool flag = true;
    for (char a : ins){
        if (!isspace(a)){
            flag = false;
            break;
        }
    }
    if (flag){
        return;
    }
    // PARSE STRING FOR COMMAND AND ARGS
    istringstream sin(ins);
    vector<string> args{istream_iterator<string>{sin}, istream_iterator<string>{}};
    string in = args[0];
    args.erase(args.begin());
    args = parse_keywords(args);
    // EXECUTE COMMAND
    if (in == "ls"){
        ls(idx, args);
    } else if (in == "cd"){
        cd(args[0]);
    } else if (in == "pwd"){
        pwd();
    } else if (in == "mkdir"){
        mkdir();
    } else {
        cout << "ERROR: COMMAND NOT FOUND" << endl;
    }
}

int main(){
    read();
    string in;
    while(true){
        cout << "[" <<  getpath() << "]" << " >>> ";
        getline(cin, in);
        cmd(in);
    }
}