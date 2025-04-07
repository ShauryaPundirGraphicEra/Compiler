// #include <iostream>
// #include <sstream>
// #include <vector>
// #include <map>
// #include <stack>
// #include <fstream>
// #include <stdexcept>
// #include <regex>

// using namespace std;

// struct SymbolInfo {
//     string type;
//     int value;
//     string scope;
// };

// // ✅ Global symbol table
// map<string, SymbolInfo> symbolTable;

// struct TreeNode {
//     string value;
//     TreeNode* left;
//     TreeNode* right;
// };

// bool isOperator(const string& token) {
//     return token == "+" || token == "-" || token == "*" || token == "/";
// }

// int precedence(const string& op) {
//     if (op == "+" || op == "-") return 1;
//     if (op == "*" || op == "/") return 2;
//     return 0;
// }

// vector<string> tokenizeExpression(const string& expr) {
//     vector<string> tokens;
//     string token;
//     for (size_t i = 0; i < expr.length(); ++i) {
//         char ch = expr[i];
//         if (isspace(ch)) continue;
//         if (isdigit(ch)) {
//             token.clear();
//             while (i < expr.length() && isdigit(expr[i])) token += expr[i++];
//             --i;
//             tokens.push_back(token);
//         } else if (isalpha(ch)) {
//             token.clear();
//             while (i < expr.length() && (isalnum(expr[i]) || expr[i] == '_')) token += expr[i++];
//             --i;
//             tokens.push_back(token);
//         } else {
//             tokens.push_back(string(1, ch));
//         }
//     }
//     return tokens;
// }

// vector<string> infixToPostfix(const vector<string>& infix) {
//     stack<string> opStack;
//     vector<string> postfix;

//     for (const string& token : infix) {
//         if (isalpha(token[0]) || isdigit(token[0])) {
//             postfix.push_back(token);
//         } else if (isOperator(token)) {
//             while (!opStack.empty() && precedence(opStack.top()) >= precedence(token)) {
//                 postfix.push_back(opStack.top());
//                 opStack.pop();
//             }
//             opStack.push(token);
//         } else if (token == "(") {
//             opStack.push(token);
//         } else if (token == ")") {
//             while (!opStack.empty() && opStack.top() != "(") {
//                 postfix.push_back(opStack.top());
//                 opStack.pop();
//             }
//             if (!opStack.empty()) opStack.pop(); // remove '('
//         }
//     }

//     while (!opStack.empty()) {
//         postfix.push_back(opStack.top());
//         opStack.pop();
//     }

//     return postfix;
// }

// int evaluateExpression(const vector<string>& postfix) {
//     stack<int> st;
//     for (const string& token : postfix) {
//         if (isdigit(token[0])) {
//             st.push(stoi(token));
//         } else if (isalpha(token[0])) {
//             if (symbolTable.find(token) != symbolTable.end()) {
//                 st.push(symbolTable[token].value);
//             } else {
//                 throw runtime_error("Variable '" + token + "' not declared.");
//             }
//         } else {
//             int b = st.top(); st.pop();
//             int a = st.top(); st.pop();
//             if (token == "+") st.push(a + b);
//             else if (token == "-") st.push(a - b);
//             else if (token == "*") st.push(a * b);
//             else if (token == "/") st.push(a / b);
//         }
//     }
//     return st.top();
// }

// TreeNode* buildParseTreeWithAssignment(const string& varName, const vector<string>& postfix) {
//     stack<TreeNode*> st;
//     for (const string& token : postfix) {
//         if (isOperator(token)) {
//             TreeNode* right = st.top(); st.pop();
//             TreeNode* left = st.top(); st.pop();
//             st.push(new TreeNode{token, left, right});
//         } else {
//             st.push(new TreeNode{token, nullptr, nullptr});
//         }
//     }
//     TreeNode* exprTree = st.top(); st.pop();
//     TreeNode* assignNode = new TreeNode{"=", new TreeNode{varName, nullptr, nullptr}, exprTree};
//     return assignNode;
// }

// void visualizeParseTree(TreeNode* root, const string& filename) {
//     ofstream fout(filename);
//     fout << "digraph ParseTree {\n";
//     int id = 0;
//     map<TreeNode*, int> ids;

//     function<void(TreeNode*)> dfs = [&](TreeNode* node) {
//         if (!node) return;
//         ids[node] = id++;
//         fout << "  " << ids[node] << " [label=\"" << node->value << "\"];\n";
//         if (node->left) {
//             dfs(node->left);
//             fout << "  " << ids[node] << " -> " << ids[node->left] << ";\n";
//         }
//         if (node->right) {
//             dfs(node->right);
//             fout << "  " << ids[node] << " -> " << ids[node->right] << ";\n";
//         }
//     };

//     dfs(root);
//     fout << "}\n";
//     fout.close();
//     cout << "Parse Tree saved as " << filename << endl;
// }

// void declareVariables(const string& declaration) {
//     regex pattern(R"(int\s+([\w\s=,]+);)");
//     smatch match;
    
//     if (!regex_search(declaration, match, pattern)) {
//         cout << "Only 'int' type declarations supported.\n";
//         return;
//     }

//     string vars = match[1];
//     stringstream ss(vars);
//     string token;

//     while (getline(ss, token, ',')) {
//         string name;
//         int value = 0;

//         size_t eq = token.find('=');
//         if (eq != string::npos) {
//             name = token.substr(0, eq);
//             value = stoi(token.substr(eq + 1));
//         } else {
//             name = token;
//         }

//         name = regex_replace(name, regex("^\\s+|\\s+$"), ""); // Trim
//         cout << "Declaring variable: " << name << " = " << value << endl;
//         symbolTable[name] = {"int", value, "Global"};
//     }

//     cout << "Variables declared.\n";
// }

// void displaySymbolTable() {
//     cout << "\nSymbol Table:\n";
//     cout << "-------------------------------------------\n";
//     cout << "Name\tType\tValue\tScope\n";
//     cout << "-------------------------------------------\n";
//     for (const auto& entry : symbolTable) {
//         cout << entry.first << "\t" << entry.second.type << "\t" << entry.second.value << "\t" << entry.second.scope << "\n";
//     }
//     cout << "-------------------------------------------\n";
// }

// int main() {
//     int choice;
//     string input;

//     while (true) {
//         cout << "\nMenu:\n";
//         cout << "1. Enter Expression\n";
//         cout << "2. Declare Variables\n";
//         cout << "3. Display Symbol Table\n";
//         cout << "4. Visualize Parse Tree\n";
//         cout << "5. Exit\n";
//         cout << "Enter your choice: ";
//         cin >> choice;
//         cin.ignore();

//         if (choice == 1) {
//             cout << "Enter an expression ending with ';': ";
//             getline(cin, input);
//             stringstream ss(input);
//             string statement;

//             while (getline(ss, statement, ';')) {
//                 if (statement.empty()) continue;

//                 vector<string> tokens = tokenizeExpression(statement);
//                 if (tokens.size() > 2 && tokens[1] == "=") {
//                     string varName = tokens[0];
//                     if (symbolTable.find(varName) == symbolTable.end()) {
//                         cerr << "Error: Variable '" << varName << "' not declared.\n";
//                         continue;
//                     }

//                     tokens.erase(tokens.begin(), tokens.begin() + 2);
//                     vector<string> postfix = infixToPostfix(tokens);

//                     try {
//                         int result = evaluateExpression(postfix);
//                         symbolTable[varName].value = result;
//                         cout << "Assigned " << varName << " = " << result << endl;

//                         TreeNode* root = buildParseTreeWithAssignment(varName, postfix);
//                         visualizeParseTree(root, "parse_tree.dot");
//                     } catch (exception& e) {
//                         cerr << "Error evaluating expression: " << e.what() << endl;
//                     }
//                 } else {
//                     cout << "Invalid expression: " << statement << endl;
//                 }
//             }

//         } else if (choice == 2) {
//             cout << "Enter declaration (e.g., int a, b = 2;): ";
//             getline(cin, input);
//             declareVariables(input);
//         } else if (choice == 3) {
//             displaySymbolTable();
//         } else if (choice == 4) {
//             system("dot -Tpng parse_tree.dot -o parse_tree.png");
//             system("start parse_tree.png"); // For Windows
//         } else if (choice == 5) {
//             break;
//         } else {
//             cout << "Invalid choice.\n";
//         }
//     }

//     return 0;
// }

