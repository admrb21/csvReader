#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>
#include <cctype>
#include <algorithm>

using namespace std;
void* toLower(basic_string<char>& s) {
   for (basic_string<char>::iterator p = s.begin();
        p != s.end(); ++p) {
      *p = tolower(*p);
   }
}
string processString(const std::string& input) {
    std::string result;

    // Remove spaces
    result = input;
    result.erase(std::remove(result.begin(), result.end(), ' '), result.end());

    // Remove brackets and content inside brackets
    size_t startBracket = result.find('(');
    size_t endBracket = result.find(')');
    while (startBracket != std::string::npos && endBracket != std::string::npos) {
        result.erase(startBracket, endBracket - startBracket + 1);
        startBracket = result.find('(');
        endBracket = result.find(')');
    }

    // Remove content after the first dot
    size_t dotPos = result.find('.');
    if (dotPos != std::string::npos) {
        result.erase(dotPos);
    }
    return result;
}

int main() {
    ifstream inputFile("file_list.csv");
    if (!inputFile.is_open()) {
        cerr << "Error opening file_list.csv" << endl;
        return 1;
    }
    ofstream outputFile("output.csv");
    if (!outputFile.is_open()) {
        cerr << "Error: Unable to open output.csv" << endl;
        return 1;
    }

    // Error log file
    ofstream errorLogFile("error_log.txt");
    if (!errorLogFile.is_open()) {
        cerr << "Error: Unable to open error_log.txt" << endl;
        return 1;
    }

    unordered_map<string, pair<string, vector<string>>> studentMap;

    string line;

    while (getline(inputFile, line)) {
        // Use a stringstream to split the line into fields
        istringstream ss(line);
        string field;
        string studentID;
        string groupName;
        vector<string> questions;

        // Process each field in the line
        while (getline(ss, field, ',')) {
            
            string newstr = processString(field);
            vector<string> tokens;
            stringstream ss(newstr);
            string token;
            
            while (getline(ss, token, '_')) {
                tokens.push_back(token);
            }
            if (tokens.size()>=3)
            {
                studentID = tokens[2];
                toLower(tokens[0]);
                toLower(tokens[1]);
                if (tokens[0]=="a"||tokens[0]=="groupa"||tokens[0]=="grupa"||tokens[0]=="qroupa")
                {
                    groupName = "a";
                }
                if (tokens[0]=="b"||tokens[0]=="groupb"||tokens[0]=="grupb"||tokens[0]=="qroupb")
                {
                    groupName = "b";
                }
                if (tokens[0]=="c"||tokens[0]=="groupc"||tokens[0]=="grupc"||tokens[0]=="qroupc")
                {
                    groupName = "c";
                }
                if (tokens[0]=="d"||tokens[0]=="groupd"||tokens[0]=="grupd"||tokens[0]=="qroupd")
                {
                    groupName = "d";
                }
                if (tokens[1]=="1"||tokens[1]=="q1"||tokens[1]=="question1"||tokens[1]=="quesiton1")
                {
                   questions.push_back("1");
                }
                if (tokens[1]=="2"||tokens[1]=="q2"||tokens[1]=="question2"||tokens[1]=="quesiton2")
                {
                    questions.push_back("2");
                }
                if (tokens[1]=="3"||tokens[1]=="q3"||tokens[1]=="question3"||tokens[1]=="quesiton3")
                {
                    questions.push_back("3");
                }
                
            }
            if (!studentID.empty()) {
                if (studentMap.find(studentID) == studentMap.end()) {
                    // New student ID
                    studentMap[studentID] = make_pair(groupName, questions);
                } else {
                    // Existing student ID
                    pair<string, vector<string>>& existingEntry = studentMap[studentID];
                    const string& existingGroup = existingEntry.first;
                    vector<string>& existingQuestions = existingEntry.second;

                    if (existingGroup == groupName) {
                        // Log error for duplicate student IDs in the same group
                        existingQuestions.insert(existingQuestions.end(), questions.begin(), questions.end());
                    } else {
                        // Add questions to the existing entry
                        errorLogFile << "Error: Duplicate student ID in the same group - " << studentID << " for file " << field << endl;
                    }
                }
            } else {
                // Log error for misnamed files
                errorLogFile << "Error: Misnamed file - " << field << endl;
            }
        }
    }
    outputFile << "Student list,GroupName,QuestionsAnswered" << endl;
for (const auto& entry : studentMap) {
    const string& studentID = entry.first;
    string groupName = entry.second.first;
    const vector<string>& questions = entry.second.second;

    // Sort questions in ascending order
    vector<string> sortedQuestions = questions;
    sort(sortedQuestions.begin(), sortedQuestions.end());

    // Concatenate questions into a single string enclosed in quotes
    string answeredQuestions = "\"";
    for (size_t i = 0; i < sortedQuestions.size(); ++i) {
        answeredQuestions += sortedQuestions[i];
        if (i < sortedQuestions.size() - 1) {
            answeredQuestions += ",";
        }
    }
    answeredQuestions += "\"";

    // Write data to CSV file
    outputFile << studentID << ",";
    outputFile << groupName << ",";
    outputFile << answeredQuestions << endl;
}

    inputFile.close();
    inputFile.close();
    outputFile.close();
    errorLogFile.close();

    cout << "Output generated successfully. Check error_log.txt for any errors." << endl;
    return 0;
}
