#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream>

using namespace std;
using namespace std::chrono;

struct Records {

    string date;
    double price = 0;
    string source;
    string name;
};

void printVector(vector<Records>, string);
void printHistorySeq(vector<string>&);
void printMainMenu(vector<Records>);
void outputTable(vector<Records>);

void SortByName(vector<Records>&, vector<string>&, string, bool&, bool&);
void SortByPrice(vector<Records>&, vector<string>&, string, string);
void SortByDate(vector<Records>&, vector<string>&, string, string);

void InsertElement(vector<Records>&, vector<string>&, string);
void ModifyElement(vector<Records>&, vector<string>&);
void RemoveElement(vector<Records>&, vector<string>&);

void ImportFile(vector<Records>&, vector<string>&, string);
void MergeVectors(vector<Records>&, vector<Records>, string);

bool isdigitCheck(string);
bool isalphaCheck(string);
string CurrentTimeStr();
string FormatTime(system_clock::time_point);

int main()
{
    //split vector
    //print seperately based on name
    //in the case of multiple readings of the same name. (scraper)
    //sort already implemented

    //read from csv
    //print to csv
    //import csv
    //allow user to decide import file
    //split receipt and output it seperately
    //if char length during print > than #, add ... to string at the end
    //dummy element needs clearing after func completes

    //on import. filename dependent on trancating needs to be based on both names now

    //vector used for the base data
    vector<Records> record;
    vector<string> historySeq;
    Records tempV;

    bool isSorted = false;
    bool sortedName_A = false;
    bool sortedName_D = false;
    string Sort_Order;

    string filename = "3070.csv";

    //cout << endl << "Input Base File (filename.format): ";
    //cin >> filename;
    
    ifstream inFile;
    inFile.open(filename);

    //throw error if file can't open
    if (!inFile)
    {
        cerr << "File cannot be opened. " << endl;
        system("PAUSE");
        exit(1);
    }

    //file -> struct input -> vector
    string tempHeaderline;
    inFile.ignore(500,'\n');
    while (getline(inFile, tempV.date, ','))//!inFile.eof())
    {      
        string tempPrice;

        getline(inFile, tempPrice, ',');
        tempV.price = stof(tempPrice);
        getline(inFile, tempV.source, ',');
        getline(inFile, tempV.name);

        record.push_back(tempV);
    }

    printHistorySeq(historySeq);
    cout << "---Base data from file-------------------" << endl;
    printVector(record, filename);

    
    ImportFile(record, historySeq, filename);
    //outputTable(record);
    //SortByName(record, historySeq, filename, sortedName_A, sortedName_D);
    inFile.close();
    return 0;
}

void printMainMenu(vector<Records> record)
{

    cout << endl;
    cout << "    Beginning of the Menu Controller" << endl;
    cout << "-----------------------------------------" << endl;
    cout << " 1. Print Records" << endl;
    cout << " 2. Sort By: Name/Price/Quantity" << endl;
    cout << " 3. Element: Insert/Modify/Remove" << endl;
    cout << " 4. Search Element" << endl;
    cout << " 5. Import and Merge File" << endl;
    cout << " 6. Check for Duplicates" << endl;
    cout << " 7. Store Records on File" << endl;
    cout << " 8. Restart" << endl;
    cout << " 9. Quit" << endl;
    cout << "-----------------------------------------" << endl;

    cout << "Input choice (1-9): ";

}

void printVector(vector<Records> vector, string filename)
{
    cout << "---Vector.size(): " << vector.size() << endl;
    cout << "-----------------------------------------" << endl;
    for (int i = 0; i < (int)vector.size(); i++) {

        cout << " ";
        cout << i << ". " << vector[i].date << " $";
        cout << vector[i].price << " ";

        if (vector[i].name.size() > 60)
        {
            //filename = 3070.csv
            //filename_withoutformat = 3070
            //(index_found + filename_withoutformat.size())
            string filename_withoutformat = filename.substr(0, filename.size() - 4);
            int index_found = vector[i].name.find(filename_withoutformat);
            int k = 0;

            //while (k != (index_found + filename_withoutformat.size() + 15))
            while (k != (index_found + filename_withoutformat.size() + 15))
            {
                cout << vector[i].name[k];
                k++;
            }

            cout << "..";
        }
        else
        {
           cout << vector[i].name;
        }
        cout << endl;
    }
    cout << "-----------------------------------------" << endl;
}

void SortByName(vector<Records>& record, vector<string>& historySeq, string filename, bool& sortedName_A, bool& sortedName_D)
{
    /*
    Prompts user to input sort preference (ascending/descending).
    If input matches preference input string, sort based on preference

    If ascending preference is chosen:
    Returns bool sortedName_A = true and bool sortedName_D = false to be used as saved preference.
    If descending preference is chosen:
    Returns bool sortedName_A = false and bool sortedName_D = true to be used as saved preference.

    If preference input is invalid:
    Repeat.
    */

    string Repeat;
    string OrderPreference;

    cout << "Sort By (Ascending/Descending): ";
    cin >> OrderPreference;

    if (OrderPreference == "Ascending" || OrderPreference == "ascending") {

        system("cls");

        historySeq.push_back("SortName-A");
        printHistorySeq(historySeq);

        sort(record.begin(), record.end(), [](Records a, Records b)
            {return a.name < b.name; });

        cout << "---Sort By Name: Ascending Order" << endl;
        printVector(record, filename);

        cout << endl;

        sortedName_A = true;
        sortedName_D = false;
        return;

    }
    else if (OrderPreference == "Descending" || OrderPreference == "descending") {

        system("cls");

        historySeq.push_back("SortName-D");
        printHistorySeq(historySeq);

        sort(record.begin(), record.end(), [](Records a, Records b)
            {return a.name > b.name; });

        cout << "---Sort By Name: Descending Order" << endl;
        printVector(record, filename);

        cout << endl;

        sortedName_A = false;
        sortedName_D = true;
        return;

    }
    else
    {
        cout << "Invalid input. " << endl;
        SortByName(record, historySeq, filename, sortedName_A, sortedName_D);
    }

}

void SortByPrice(vector<Records>& record, vector<string>& historySeq, string filename, string OrderPreference)
{
    /*
    Prompts user to input sort preference (ascending/descending).
    If input matches preference input string, sort based on preference
    If preference input is invalid:
    Repeat.
    */

    string Repeat;

    cout << "Sort By (Ascending/Descending): ";
    cin >> OrderPreference;

    if (OrderPreference == "Ascending" || OrderPreference == "ascending") {

        system("cls");

        historySeq.push_back("SortPrice-A");
        printHistorySeq(historySeq);

        sort(record.begin(), record.end(), [](Records a, Records b)
            {return a.price < b.price; });

        cout << "---Sort By Price: Ascending Order" << endl;
        printVector(record, filename);

        cout << endl;
        return;


    }
    else if (OrderPreference == "Descending" || OrderPreference == "descending") {

        system("cls");

        historySeq.push_back("SortPrice-D");
        printHistorySeq(historySeq);

        sort(record.begin(), record.end(), [](Records a, Records b)
            {return a.price > b.price; });

        cout << "---Sort By Price: Descending Order" << endl;
        printVector(record, filename);

        cout << endl;
        return;

    }
    else
    {
        cout << "Invalid input. " << endl;
        SortByPrice(record, historySeq, filename, OrderPreference);
    }

}

void SortByDate(vector<Records>& record, vector<string>& historySeq, string filename, string OrderPreference)
{
    /*
    Prompts user to input sort preference (ascending/descending).
    If input matches preference input string, sort based on preference
    If preference input is invalid:
    Repeat.
    */

    string Repeat;

    cout << "Sort By (Ascending/Descending): ";
    cin >> OrderPreference;


    if (OrderPreference == "Ascending" || OrderPreference == "ascending") {

        system("cls");

        historySeq.push_back("SortQty-A");
        printHistorySeq(historySeq);

        sort(record.begin(), record.end(), [](Records a, Records b)
            {return a.date < b.date; });

        cout << "---Sort By Quantity: Ascending Order" << endl;

        printVector(record, filename);

        cout << endl;
        cout << "---Time complexity : O(nlogn)" << endl;
        return;

    }
    else if (OrderPreference == "Descending" || OrderPreference == "descending") {

        system("cls");

        historySeq.push_back("SortQty-D");
        printHistorySeq(historySeq);

        sort(record.begin(), record.end(), [](Records a, Records b)
            {return a.date > b.date; });


        cout << "---Sort By Quantity: Descending Order" << endl;
        printVector(record, filename);

        cout << endl;
        cout << "---Time complexity : O(nlogn)" << endl;
        return;

    }
    else
    {
        cout << "Invalid input. " << endl;
        SortByDate(record, historySeq, filename, OrderPreference);
    }

}

void printHistorySeq(vector<string>& historySeq)
{
    /*
    Vector of size 5 to store strings of actions user runs.
    Functions run push a string of function name onto historySeq vector.
    Prints historySeq vector contents to be used as history of actions used by the user.
    */

    //remove historySeq[0] when vector.size() reaches 5. cap size() at 5.
    if (historySeq.size() > 4)
        historySeq.erase(historySeq.begin());

    //only print history: when size() not empty
    if (historySeq.size() != 0)
    {
        cout << "History: ";

        //historySeq print
        for (int i = 0; i < (int)historySeq.size(); i++)
        {
            cout << historySeq[i] << " >> ";
        }

        cout << endl;
        cout << "-----------------------------------------" << endl;
    }
    //deallocate excess memory down to vector size()
    historySeq.shrink_to_fit();
    cout << endl;
}

bool isdigitCheck(string InputToCheck)
{
    /*
    Checks string input if it contains digits.
    Returns true if digits are found.
    Return false if no digits are found.
    */

    for (int i = 0; i < (int)InputToCheck.size(); i++)
    {
        if (isdigit(InputToCheck[i]))
        {
            cin.clear();
            cin.ignore();

            cout << endl << "---Invalid input. Try again!" << endl;
            cout << "---Characters valid only." << endl << endl;
            return true;
        }
    }
    return false;
}

bool isalphaCheck(string InputToCheck)
{
    /*
    Checks string input if it contains characters.
    Returns true if characters are found.
    Return false if no characters are found.
    */

    for (int i = 0; i < (int)InputToCheck.size(); i++)
    {
        if (isalpha(InputToCheck[i]))
        {
            cin.clear();
            cin.ignore();

            cout << endl << "---Invalid input. Try again!" << endl;
            cout << "---Digits valid only." << endl << endl;
            return true;
        }
    }
    return false;
}

void InsertElement(vector<Records>& record, vector<string>& historySeq, string filename)
{
    /*
    Prompts user to input values of a new element to be inserted into the vector [name:price:quantity].
    Checks to only accept characters for name.
    Checks to only accept digits for price.
    Checks to only accept digits for quantity.

    Prompts user confirmation to insert [name:price:quantity].
    Yes, inserts element in the vector
    No, breaks out of the function.
    */

    Records DummyElement;
    
    string InsertDate;
    string InsertPrice;
    string InsertSource;
    string InsertName;

    string Confirmation_One;
    string Confirmation_Two;

    bool Repeat = false;

    cout << endl << "Inserting Element into database.." << endl;
    cout << "Input [date:name:price:source]" << endl << endl;
    //assigning date of InsertElement action

    InsertDate = CurrentTimeStr();

    //input name of product
    cout << "Input Name: ";
    getline(cin, InsertName);

    //confirm string InsertPrice only contains digits
    //error into loop if character detected
    do {

        Repeat = false;

        cout << "Input Price: ";
        cin >> InsertPrice;

        Repeat = isalphaCheck(InsertPrice);

    } while (Repeat);
    //convert string InsertPrice to float InsertPrice_Float
    float InsertPrice_Float = stof(InsertPrice);

    //input product source
    cout << "Input Source: ";
    cin >> InsertSource;

    cout << endl;
    cout << "Confirm ["
        << InsertDate << ":"
        << InsertName << ":"
        << InsertPrice_Float << ":"
        << InsertSource << "]? (Y/N): ";

    cin >> Confirmation_One;

    if (Confirmation_One == "Y" || Confirmation_One == "y"
        || Confirmation_One == "Yes" || Confirmation_One == "yes")
    {
        DummyElement.date = InsertDate;
        DummyElement.name = InsertName;
        DummyElement.price = InsertPrice_Float;
        DummyElement.source = InsertSource;

        record.push_back(DummyElement);

        system("cls");
        cout << record[record.size() - 1].name << endl;

        historySeq.push_back("InsertElement");
        printHistorySeq(historySeq);

        cout << "---Inserted ["
            << DummyElement.date << ":"
            << DummyElement.name << ":"
            << DummyElement.price << ":"
            << DummyElement.source << "]" << endl;

        printVector(record, filename);
        return;
    }

    system("cls");
    printVector(record, filename);
    return;
}

string FormatTime(system_clock::time_point tp) {
    
    stringstream ss;
    auto t = system_clock::to_time_t(tp);
    auto tp2 = system_clock::from_time_t(t);
    if (tp2 > tp)
        t = system_clock::to_time_t(tp - seconds(1));
    ss << put_time(localtime(&t), "%d-%m-%Y")
        << setfill('0') << setw(2);
    return ss.str();
}

string CurrentTimeStr() {
    return FormatTime(system_clock::now());
}

void ImportFile(vector<Records>& record, vector<string>& historySeq, string filename)
{
    /*
    Prompts user to input filename.txt of the file to merge with the base data table.
    Checks if filename.txt exists, prompts error if not.
    Reads the data from filename.txt into a tempV and pushes to local tempVector.
    Calls MergeVectors(record, tempVector) that reserves memory of record.size() tempVector.size()
    Releases memory from tempVector.
    */

    vector<Records> tempVector;
    string filename1;

    cout << endl << "Import (filename.format): ";
    cin >> filename1;

    //system("cls");
    historySeq.push_back("Import&Merge");
    printHistorySeq(historySeq);

    ifstream inFile;
    inFile.open(filename1);

    if (!inFile.is_open())
    {
        cerr << "---File cannot be opened. " << endl;
        printVector(record, filename1);
        return;
    }

    string tempHeaderline;
    while (getline(inFile, tempHeaderline))
    {   
        Records tempV;
        string tempPrice;

        getline(inFile, tempV.date, ',');
        getline(inFile, tempPrice, ',');
        tempV.price = stof(tempPrice);
        getline(inFile, tempV.source, ',');
        getline(inFile, tempV.name);

        record.push_back(tempV);
    }

    cout << "---Base Data Table" << endl;
    printVector(record, filename1);

    cout << endl << "---Imported Data Table" << endl;
    printVector(tempVector, filename);

    MergeVectors(record, tempVector, filename);

    inFile.close();
    tempVector.clear();
    tempVector.shrink_to_fit();

    return;
}

void MergeVectors(vector<Records>& vector1, vector<Records> vector2, string filename)
{   
    //reserve memory for vector merge and insert vector2 at the back of vector1
    vector1.reserve(vector1.size() + vector2.size());
    vector1.insert(vector1.end(), vector2.begin(), vector2.end());

    cout << endl << "---Merged Data Table" << endl;
    printVector(vector1, filename);
}

void outputTable(vector<Records> vector)
{
    //output current iteration of the base data table into a filename.txt of the user's choice. 

    string filename;
    cout << endl << "Input file name to store records (filename.csv): ";
    cin >> filename;

    //system("cls");
    ofstream outputFile;
    outputFile.open(filename);
    outputFile << "[" << filename << "]" << endl;

    outputFile << "---Vector.size(): " << vector.size() << endl;
    outputFile << "---vector.capacity(): " << vector.capacity() << endl;
    outputFile << "-----------------------------------------" << endl;
    outputFile << endl << "  -Name-       -Price-    -Qty-" << endl;
    for (int i = 0; i < (int)vector.size(); i++) 
    {
        outputFile << vector[i].date << ",";
        outputFile << vector[i].price << ",";
        outputFile << vector[i].source << ",";
        outputFile << vector[i].name << ",";
        outputFile << endl;
    }

    cout << "---Records have been stored under " << filename << ". " << endl;

    outputFile.close();
}

void quicksort(vector<Records>& record, bool sortedName_A, bool sortedName_D, bool& isSorted)
{
    /*
    Mirror of SortbyName to auto sort based on saved SortbyName(Ascending/Descending) preference.
    */

    if (isSorted == 0)
    {
        if (sortedName_A == 1)
        {
            cout << "was sorted A: true, issorted = false" << endl;
            sort(record.begin(), record.end(), [](Records a, Records b)
                {return a.name < b.name; });

            isSorted = true;
            return;
        }
        else if (sortedName_D == 1)
        {
            cout << "was sorted D: true, issorted = false" << endl;
            sort(record.begin(), record.end(), [](Records a, Records b)
                {return a.name > b.name; });

            isSorted = true;
            return;
        }
    }

    return;
}