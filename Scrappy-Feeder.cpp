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
    float price = 0;
    string source;
    string name;
    string key;

};

void printVector(vector<Records>);
void printHistorySeq(vector<string>&);
void printMainMenu(vector<Records>);
void MenuController(vector<Records>&);
void outputTable(vector<Records>);

void SortByName(vector<Records>&, vector<string>&, bool&, bool&);
void SortByPrice(vector<Records>&, vector<string>&, string, bool&, bool&);
void SortByDate(vector<Records>&, vector<string>&, string);
void forceSortName(vector<Records>&);

void InsertElement(vector<Records>&, vector<string>&);
void ModifyElement(vector<Records>&, vector<string>&);
void RemoveElement(vector<Records>&, vector<string>&);

void SearchIndex(vector<Records>, vector<string>&);
void ImportFile(vector<Records>&, vector<string>);
void MergeVectors(vector<Records>&, vector<Records>, vector<string>&);
void removeDuplicates(vector<Records>&, vector<string>&);

bool isdigitCheck(string);
bool isalphaCheck(string);
string CurrentTimeStr();
string FormatTime(system_clock::time_point);

int main()
{
    //print to csv
    //split receipt and output it seperately
    //remove everything higher than x price after split receipt
    
    //spacing issues with print. setw >> left >> right 

    //vector used for the base data
    vector<Records> record;
    vector<string> historySeq;
    Records tempV;

    string Sort_Order;
    string filename = "70.csv";

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
    //inFile.ignore(500, '\n');
    while (getline(inFile, tempV.date, ','))//!inFile.eof())
    {
        string tempPrice;

        getline(inFile, tempPrice, ',');
        getline(inFile, tempV.source, ',');
        getline(inFile, tempV.name);

        tempV.price = stof(tempPrice);
        tempV.key = filename.substr(0, filename.size() - 4);

        record.push_back(tempV);
    }

    printVector(record);
    cout << "---Base data from file-------------------" << endl;
    MenuController(record);

    inFile.close();
    return 0;
}

void MenuController(vector<Records>& record)
{
    /*
    Menu controller used to for the user to interface through the various functions to process and output data.

    1. Print Records
    2. Sort By: Name/Price/Quantity
    3. Element: Insert/Modify/Remove
    4. Search Element
    5. Import and Merge File
    6. Check for Duplicates
    7. Store Records on File
    8. Restart
    9. Quit
    */

    bool Repeat = true;

    bool isSorted = false;
    bool sortedName_A = false;
    bool sortedName_D = false;
    bool sortedPrice_A = false;
    bool sortedPrice_D = false;

    bool invalidChoice;

    char choice[6];
    string Sort_Order;
    vector<string> historySeq;

    string Preference;
    //bool bool_diag;

    do {

        printMainMenu(record);
        cin >> choice;



        //checks if choice[] input is alphabetical. forces a try again if so
        invalidChoice = isalphaCheck(choice);
        if (invalidChoice == false && strlen(choice) < 2)
        {
            switch (choice[0])
            {
            case '1':
                //print table:
                system("cls");
                printVector(record);
                historySeq.push_back("PrintRecords");
                printHistorySeq(historySeq);
                cout << "---Printing Records." << endl;

                break;

            case '2':

                cout << endl;
                cout << " 1. Sort by Name" << endl;
                cout << " 2. Sort by Price" << endl;
                cout << " 3. Sort by Date" << endl << endl;

                cout << "Input choice (1-3): ";
                cin >> choice;

                //checks if choice[] input is alphabetical. forces a try again if it is
                invalidChoice = isalphaCheck(choice);
                if (invalidChoice == false)
                {

                    switch (choice[0])
                    {
                    case '1':
                        //sortbyname:
                        SortByName(record, historySeq, sortedName_A, sortedName_D);
                        isSorted = true;
                        break;

                    case '2':
                        //sortbyprice:
                        SortByPrice(record, historySeq, Sort_Order, sortedPrice_A, sortedPrice_D);
                        isSorted = true;
                        break;

                    case '3':
                        //sortbydate:
                        SortByDate(record, historySeq, Sort_Order);
                        isSorted = false;
                        break;
                    }
                }
                else
                {
                    system("cls");
                    printVector(record);
                    printHistorySeq(historySeq);
                    cout << "---Invalid input. Digits valid only." << endl;
                }
                break;

            case '3':

                cout << endl;
                cout << " 1. Insert Element" << endl;
                cout << " 2. Modify Element" << endl;
                cout << " 3. Remove Element" << endl << endl;

                cout << "Input choice (1-3): ";
                cin >> choice;


                //checks if choice[] input is alphabetical.forces a try again if so
                invalidChoice = isalphaCheck(choice);
                if (invalidChoice == false)
                {
                    switch (choice[0])
                    {
                    case '1':
                        //insertelement:
                        InsertElement(record, historySeq);
                        isSorted = false;
                        break;

                    case '2':
                        //modifyelement:
                        ModifyElement(record, historySeq);
                        isSorted = false;
                        break;

                    case '3':
                        //removelement:
                        RemoveElement(record, historySeq);
                        break;
                    }
                }
                else
                {
                    system("cls");
                    printVector(record);
                    printHistorySeq(historySeq);
                    cout << "---Invalid input. Digits valid only." << endl;
                }
                break;

            case '4':
                //search:
                SearchIndex(record, historySeq);
                break;

            case '5':
                //import&merge file:
                ImportFile(record, historySeq);
                isSorted = false;
                break;

            case '6':
                //duplicate check:
                forceSortName(record);
                removeDuplicates(record, historySeq);
                break;

            case '7':
                //store records on file
                outputTable(record);
                historySeq.push_back("StoreRecords");
                printVector(record);
                break;

            case '8':
                //restart:
                system("cls");
                printVector(record);
                historySeq.push_back("Restart");
                printHistorySeq(historySeq);
                break;

            case '9':
                //quit:
                cout << endl;
                return;

            default:
                system("cls");
                printVector(record);
                printHistorySeq(historySeq);
                cout << "---Invalid input." << endl;
            }
        }
        else
        {
            if (strlen(choice) > 2)
            {
                //if input above length 1
                system("cls");
                printVector(record);
                printHistorySeq(historySeq);
                cout << "---Invalid input. Input of length 1 only." << endl;
            }
            else
            {
                //if characters exist
                system("cls");
                printVector(record);
                printHistorySeq(historySeq);
                cout << "---Invalid input. Digits valid only." << endl;
            }

        }
    } while (Repeat);
}

void printMainMenu(vector<Records> record)
{

    cout << endl;
    cout << "    Beginning of the Menu Controller" << endl;
    cout << "-----------------------------------------" << endl;
    cout << " 1. Print Records" << endl;
    cout << " 2. Sort By: Name/Price/Date" << endl;
    cout << " 3. Element: Insert/Modify/Remove" << endl;
    cout << " 4. Search Element" << endl;
    cout << " 5. Import and Merge File" << endl;
    cout << " 6. Check for Duplicates" << endl;
    cout << " 7. ***Store Records on File" << endl;
    cout << " 8. Restart" << endl;
    cout << " 9. Quit" << endl;
    cout << "-----------------------------------------" << endl;

    cout << "Input choice (1-9): ";

}

void printVector(vector<Records> vector)
{
    for (int i = 0; i < (int)vector.size(); i++) 
    {
        cout << " ";
        cout << setw(3) << i << ". " << vector[i].date << " $";
        cout << vector[i].price << " ";

        if (vector[i].name.size() > 60)
        {   
            int indexOfKey = vector[i].name.find(vector[i].key);
            int final_index = vector[i].name.find(" ", indexOfKey + vector[i].key.size() + 15);

            cout << vector[i].name.substr(0, final_index) << "..";
        }
        else
        {
            cout << vector[i].name;
        }
        cout << endl;
    }
    cout << endl << "---Vector.size(): " << vector.size() - 1<< endl;
    //cout << "-----------------------------------------" << endl;
    //cout << "-----------------------------------------" << endl;
}

void SortByName(vector<Records>& record, vector<string>& historySeq, bool& sortedName_A, bool& sortedName_D)
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

        sort(record.begin(), record.end(), [](Records a, Records b)
            {return a.name < b.name; });

        printVector(record);
        historySeq.push_back("SortName-A");
        printHistorySeq(historySeq);
        cout << "---Sort By Name: Ascending Order" << endl;

        cout << endl;

        sortedName_A = true;
        sortedName_D = false;
        return;

    }
    else if (OrderPreference == "Descending" || OrderPreference == "descending") {

        system("cls");

        sort(record.begin(), record.end(), [](Records a, Records b)
            {return a.name > b.name; });

        printVector(record);
        historySeq.push_back("SortName-D");
        printHistorySeq(historySeq);
        cout << "---Sort By Name: Descending Order" << endl;

        cout << endl;

        sortedName_A = false;
        sortedName_D = true;
        return;

    }
    else
    {
        cout << "Invalid input. " << endl;
        SortByName(record, historySeq, sortedName_A, sortedName_D);
    }

}

void SortByPrice(vector<Records>& record, vector<string>& historySeq, string OrderPreference, bool& sortedPrice_A, bool& sortedPrice_D)
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

        sort(record.begin(), record.end(), [](Records a, Records b)
            {return a.price < b.price; });
        
        printVector(record);
        historySeq.push_back("SortPrice-A");
        printHistorySeq(historySeq);
        cout << "---Sort By Price: Ascending Order" << endl;

        cout << endl;
        sortedPrice_A = true;
        sortedPrice_D = false;
        return;


    }
    else if (OrderPreference == "Descending" || OrderPreference == "descending") {

        system("cls");

        sort(record.begin(), record.end(), [](Records a, Records b)
            {return a.price > b.price; });

        printVector(record);
        historySeq.push_back("SortPrice-D");
        printHistorySeq(historySeq);
        cout << "---Sort By Price: Descending Order" << endl;

        cout << endl;
        sortedPrice_A = false;
        sortedPrice_D = true;
        return;

    }
    else
    {
        cout << "Invalid input. " << endl;
        SortByPrice(record, historySeq, OrderPreference, sortedPrice_A, sortedPrice_D);
    }

}

void SortByDate(vector<Records>& record, vector<string>& historySeq, string OrderPreference)
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



        sort(record.begin(), record.end(), [](Records a, Records b)
            {return a.date < b.date; });

        printVector(record);
        historySeq.push_back("SortDate-A");
        printHistorySeq(historySeq);
        cout << "---Sort By Date: Ascending Order" << endl;

        cout << endl;
        return;

    }
    else if (OrderPreference == "Descending" || OrderPreference == "descending") {

        system("cls");

        sort(record.begin(), record.end(), [](Records a, Records b)
            {return a.date > b.date; });

        printVector(record);
        historySeq.push_back("SortDate-D");
        printHistorySeq(historySeq);
        cout << "---Sort By Date: Descending Order" << endl;

        cout << endl;
        return;

    }
    else
    {
        cout << "Invalid input. " << endl;
        SortByDate(record, historySeq, OrderPreference);
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
        cout << "---History: ";

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
    //cout << endl;
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

void InsertElement(vector<Records>& record, vector<string>& historySeq)
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
    cout << "Input Date: " << InsertDate << endl;
    
    //input name of product
    cout << "Input Name: ";
    getline(cin >> ws, InsertName);

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

        printVector(record);
        historySeq.push_back("InsertElement");
        printHistorySeq(historySeq);

        cout << "---Inserted ["
            << DummyElement.date << ":"
            << DummyElement.name << ":"
            << DummyElement.price << ":"
            << DummyElement.source << "]" << endl;

        return;
    }

    system("cls");
    printVector(record);
    printHistorySeq(historySeq);
    cout << "---Insert Element cancelled." << endl;
    return;
}

void ModifyElement(vector<Records>& record, vector<string>& historySeq)
{
    /*
    Prompts user to input ElementToModify.
    Binarysearch to look up index of ElementToModify.
    If value exists, allows user to overrite [name:price:quantity]
    Option for the user to input "nochange" if they choose to leave value unchanged.

    Checks to only accept characters for name.
    Checks to only accept digits for price, with the exception of "nochange".
    Checks to only accept digits for quantity, with the exception of "nochange".

    Prompts user confirmation to remove [name:price:quantity].
    Yes, replaces non-"nochange" overrite values into index# of the ElementToModify.
    No, breaks out of the function.
    */

    string ModifyInput;
    string ElementToModify;
    string ValueLookup;
    int IntValueLookup;

    string InsertName;
    string InsertPrice;
    string InsertSource;

    int InsertQuantity_Int = 0;
    float InsertPrice_Float = 0;

    bool Repeat;
    string Confirmation;

    do {
        cout << "Input Index of Search: ";
        cin >> ValueLookup;

        Repeat = false;
        Repeat = isalphaCheck(ValueLookup);

        if (Repeat == false)
            IntValueLookup = stoi(ValueLookup);

    } while (Repeat);

    if (IntValueLookup > record.size() - 1 || IntValueLookup < 0)
    {
        system("cls");
        printVector(record);
        printHistorySeq(historySeq);
        cout << "---Index input does not exist in the records." << endl;
        return;
    }

    cout << endl << "Type 'nochange' for element variables you wish to keep unchanged." << endl;
    cout << "---Modifying Index " << ValueLookup << " of Price: $"
        << record[IntValueLookup].price << ", registered on Date: " << record[IntValueLookup].date << "." << endl;
    cout << "---Name: " << record[IntValueLookup].name << endl;
    cout << "---Source: " << record[IntValueLookup].source << endl << endl;

    //modify name
    cout << "Input Name: ";
    cin >> InsertName;

    //modify price
    do {
        Repeat = false;

        cout << "Input Price: ";
        cin >> InsertPrice;

        if (InsertPrice != "nochange")
            Repeat = isalphaCheck(InsertPrice);

    } while (Repeat);

    if (InsertPrice != "nochange")
        InsertPrice_Float = stof(InsertPrice);

    //modify source
    cout << "Input Source: ";
    cin >> InsertSource;

    cout << endl;
    cout << "Index " << IntValueLookup << " of Price: $";
    
    if (InsertPrice != "nochange")
        cout << InsertPrice;
    else
        cout << record[IntValueLookup].price;

    cout << ", registered on Date: " << record[IntValueLookup].date << "." << endl;

    if (InsertName != "nochange")
        cout << "---Name: " << InsertName << "." << endl;
    else
        cout << "---" << record[IntValueLookup].name << "." << endl;

    if (InsertSource != "nochange")
        cout << "---Source: " << InsertSource << "." << endl;
    else
        cout << "---" << record[IntValueLookup].source << "." << endl;

    cout << "---Confirm Element Modify (Y/N): ";
    cin >> Confirmation;

    if ((Confirmation == "Y" || Confirmation == "y" || Confirmation == "Yes" || Confirmation == "yes"))
    {
        if (InsertPrice != "nochange")
            record[IntValueLookup].price = InsertPrice_Float;
        if (InsertName != "nochange")
            record[IntValueLookup].name = InsertName;
        if (InsertSource != "nochange")
            record[IntValueLookup].source = InsertSource;

        system("cls");
        historySeq.push_back("ModifyElement");
        printVector(record);
        printHistorySeq(historySeq);

        cout << "---Modified Index " << IntValueLookup << " of Price: $";

        if (InsertPrice != "nochange"){
            cout << InsertPrice;
        }
        else
            cout << record[IntValueLookup].price;

        cout << ", registered on Date: " << record[IntValueLookup].date << "." << endl;

        if (InsertName != "nochange"){
            cout << "---Name: " << InsertName << "." << endl;
        }
        else
            cout << "---" << record[IntValueLookup].name << "." << endl;

        if (InsertSource != "nochange"){
            cout << "---Source: " << InsertSource << "." << endl;
        }
        else
            cout << "---Source: " << record[IntValueLookup].source << "." << endl;

        return;
    }
    
    system("cls");
    printVector(record);
    return;
}

void RemoveElement(vector<Records>& record, vector<string>& historySeq)
{
    /*
    Prompts user to input element name to remove.
    Binarysearch name to extract index.
    Prompts user confirmation to remove [name:price:quantity].
    Yes, removes index from the vector.
    No, breaks out of the function.
    */

    string Confirmation;
    string ValueLookup;
    int IntValueLookup;
    bool Repeat;
    int index;

    do {
        cout << "Input Index of Search: ";
        cin >> ValueLookup;

        Repeat = false;
        Repeat = isalphaCheck(ValueLookup);

        if (Repeat == false)
            IntValueLookup = stoi(ValueLookup);

    } while (Repeat);

    if (IntValueLookup > record.size() - 1 || IntValueLookup < 0)
    {
        system("cls");
        printVector(record);
        printHistorySeq(historySeq);
        cout << "---Index input does not exist in the records." << endl;
        return;
    }

    cout << endl;
    cout << "---Index " << IntValueLookup << " of Price: $"
        << record[IntValueLookup].price << ", registered on Date: " << record[IntValueLookup].date << "." << endl;
    cout << "---Name: " << record[IntValueLookup].name << endl;
    cout << "---Source: " << record[IntValueLookup].source << endl;
    cout << "---Confirm Element Remove (Y/N): ";

    cin >> Confirmation;

    if ((Confirmation == "Y" || Confirmation == "y"
        || Confirmation == "Yes" || Confirmation == "yes"))
    {
        system("cls");
        record.erase(record.begin() + (IntValueLookup));
        printVector(record);
        historySeq.push_back("RemoveElement");
        printHistorySeq(historySeq);

        cout << "---Element of Index " << IntValueLookup << " has been removed." << endl;

        return;
    }

    system("cls");
    printVector(record);
    return;
}

string FormatTime(system_clock::time_point tp) {

    stringstream ss;
    auto t = system_clock::to_time_t(tp);
    auto tp2 = system_clock::from_time_t(t);
    if (tp2 > tp)
        t = system_clock::to_time_t(tp - seconds(1));
    ss << put_time(localtime(&t), "%m/%d/22")
        << setfill('0') << setw(2);
    return ss.str();
}

string CurrentTimeStr() {
    return FormatTime(system_clock::now());
}

void ImportFile(vector<Records>& record, vector<string> historySeq)
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

    system("cls");

    ifstream inFile;
    inFile.open(filename1);

    if (!inFile.is_open())
    {
        cerr << "---File cannot be opened. " << endl;
        printVector(record);
        return;
    }

    cout << "---Base Data Table" << endl;

    printVector(record);

    Records tempStruct;
    string tempHeaderline;
    //inFile.ignore(500, '\n');
    while (getline(inFile, tempStruct.date, ','))
    {
        string tempPrice;

        getline(inFile, tempPrice, ',');
        getline(inFile, tempStruct.source, ',');
        getline(inFile, tempStruct.name);

        tempStruct.price = stof(tempPrice);
        tempStruct.key = filename1.substr(0, filename1.size() - 4);

        tempVector.push_back(tempStruct);
    }

    cout << endl << "---Imported Data Table" << endl;
    printVector(tempVector);

    MergeVectors(record, tempVector, historySeq);

    inFile.close();
    tempVector.clear();
    tempVector.shrink_to_fit();

    return;
}

void removeDuplicates(vector<Records>& record, vector<string>& historySeq)
{
    /*
    Breaks function if size() = 0.
    Runs for loop over the vector with an if statement checking for (record[current].name == record[previous].name)
    For every success if statement check, detected_dupes++.

    For dupes detected:
    Allows user to input which of the two duplicates to remove based on index#.
    Releases excess memory from elements that have been removed.

    If no dupes detected:
    Breaks function informing user that no dupes have been detected.
    */

    if (record.size() == 0)
    {
        return;
    }

    int previous = 0;
    int detected_dupes = 0;
    int dupetoRemove;
    bool Repeat = false;

    system("cls");
    printVector(record);
    historySeq.push_back("removeDupes");
    printHistorySeq(historySeq);
    cout << "---Checking for duplicates." << endl << endl;

    //string date;
    //double price = 0;
    //string source;
    //string name;
    //string key;

    //cout << "---Detected duplicates: " << endl;

    for (int current = 1; current < (int)record.size(); current++)
    {

        if (record[current].name == record[previous].name)
        {

            cout << "---Index " << previous << " of Price: $"
                << record[previous].price << ", registered on Date: " << record[previous].date << "." << endl;
            cout << "---Name: " << record[previous].name << endl;
            cout << "---Source: " << record[previous].source << endl << endl;
            
            cout << "---Index " << current << " of Price: $"
                << record[current].price << ", registered on Date: " << record[current].date << "." << endl;
            cout << "---Name: " << record[current].name << endl;
            cout << "---Source: " << record[current].source << endl << endl;
            
            cout << "---Input index of duplicate to remove (" << previous << "/" << current << "): ";
            cin >> dupetoRemove;
            cout << endl;

            if (dupetoRemove == previous)
            {
                record.erase(record.begin() + previous);
            }
            else if (dupetoRemove == current)
            {
                record.erase(record.begin() + current);
            }
            else
            {
                cin.clear();
                cin.ignore();
                removeDuplicates(record, historySeq);
            }
            detected_dupes++;
        }
        previous++;
    }


    if (detected_dupes == 0)
    {
        system("cls");
        printVector(record);
        historySeq.push_back("removeDupes");
        printHistorySeq(historySeq);
        cout << "---No duplicates detected." << endl;
        return;
    }

    system("cls");
    printVector(record);
    printHistorySeq(historySeq);

    record.shrink_to_fit();
    return;
}

void MergeVectors(vector<Records>& vector1, vector<Records> vector2, vector<string>& historySeq)
{
    //reserve memory for vector merge and insert vector2 at the back of vector1
    vector1.reserve(vector1.size() + vector2.size());
    vector1.insert(vector1.end(), vector2.begin(), vector2.end());

    cout << endl << "---Merged Data Table" << endl;
    printVector(vector1);
    historySeq.push_back("Import&Merge");
    printHistorySeq(historySeq);
    cout << "---File has been imported and merged." << endl;
}

void outputTable(vector<Records> vector)
{
    //output current iteration of the base data table into a filename.txt of the user's choice. 

    string filename;
    cout << endl << "Input file name to store records (filename.csv): ";
    cin >> filename;

    system("cls");
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

void SearchIndex(vector<Records> record, vector<string>& historySeq)
{
    bool Repeat;
    string ValueLookup;
    int IntValueLookup;

    do {
        cout << "Input Index of Search: ";
        cin >> ValueLookup;

        Repeat = false;
        Repeat = isalphaCheck(ValueLookup);

        if (Repeat == false)
            IntValueLookup = stoi(ValueLookup);

    } while (Repeat);

    system("cls");
    printVector(record);
    historySeq.push_back("Search");
    printHistorySeq(historySeq);

    if (IntValueLookup > record.size() - 1 || IntValueLookup < 0)
    {
        system("cls");
        printVector(record);
        printHistorySeq(historySeq);
        cout << "---Index input does not exist in the records." << endl;
        return;
    }

    cout << "---Index " << ValueLookup << " of Price: $"
        << record[IntValueLookup].price << ", registered on " << record[IntValueLookup].date << "." << endl;

    cout << "---" << record[IntValueLookup].name << endl;
    cout << "---" << record[IntValueLookup].source << endl;

}

void forceSortName(vector<Records>& record)
{
    /*
    Force SortbyName-Ascending.
    */

    sort(record.begin(), record.end(), [](Records a, Records b)
        {return a.name < b.name; });

    return;
}