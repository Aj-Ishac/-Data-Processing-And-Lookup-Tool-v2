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

void InsertElement(vector<Records>&, vector<string>&);
void ModifyElement(vector<Records>&, vector<string>&);
void RemoveElement(vector<Records>&, vector<string>&);

void Search(vector<Records>, vector<string>&);
int BinarySearch(vector<Records>, int, int, float);
int linearSearch(vector<Records>, float);
int linearSearch(vector<Records>, string);

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
    //look into changing historyseq from vector to array
    
    //binarysearch funcs currently disabled till solution presented. search, modify, remove disabled.
    //algo to run a check on incomplete string to complete string
    
    //spacing issues with print. setw >> left >> right 
    //cleanup input of length 1 only logic
    //search currently implemented to search through price or name, change to index. holy shit ook ook, won't depend on shit like this

    //vector used for the base data
    vector<Records> record;
    vector<string> historySeq;
    Records tempV;

    //bool isSorted = false;
    //bool sortedName_A = false;
    //bool sortedName_D = false;


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

                historySeq.push_back("PrintRecords");
                printHistorySeq(historySeq);

                cout << "---Printing Records." << endl;
                printVector(record);
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
                    printHistorySeq(historySeq);
                    cout << "---Invalid input. Digits valid only." << endl;
                    printVector(record);
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
                        //quicksort(record, sortedName_A, sortedName_D, isSorted);

                        if (isSorted == 0)
                        {
                            system("cls");
                            printHistorySeq(historySeq);
                            cout << "---Function requires records to be sorted by name!" << endl;
                            printVector(record);
                            break;
                        }

                        //ModifyElement(record, historySeq);
                        isSorted = false;
                        break;

                    case '3':
                        //removelement:
                        //quicksort(record, sortedName_A, sortedName_D, isSorted);

                        if (isSorted == 0)
                        {
                            system("cls");
                            printHistorySeq(historySeq);
                            cout << "---Function requires records to be sorted by name!" << endl;
                            printVector(record);
                            break;
                        }

                        //RemoveElement(record, historySeq);
                        break;
                    }

                }
                else
                {
                    system("cls");
                    printHistorySeq(historySeq);
                    cout << "---Invalid input. Digits valid only." << endl;
                    printVector(record);
                }
                break;

            case '4':
                //search:
                cout << "Input preference to search by (Name/Price): ";
                cin >> Preference;

                if (Preference == "Price" || Preference == "price" || Preference == "PRICE")
                {
                    if (isSorted == 0)
                    {
                        int index;
                        float ValueLookup;

                        cout << "LSearch" << endl;
                        cout << endl << "Search Product by Price: ";
                        cin >> ValueLookup;

                        index = linearSearch(record, ValueLookup);

                        system("cls");
                        printVector(record);
                        printHistorySeq(historySeq);

                        if (!(index == -1))
                        {
                            cout << "---Index " << index << " of Price: $"
                                << record[index].price << ", registered on " << record[index].date << "." << endl;

                            cout << "---" << record[index].name << endl;
                            cout << "---" << record[index].source << endl;
                            break;
                        }

                        cout << "---Failed to find input." << endl;
                        break;

                    }
                    else
                    {
                        cout << "BSearch" << endl;
                        Search(record, historySeq);
                        break;
                    }
                }
                else if(Preference == "Name" || Preference == "name" || Preference == "NAME")
                {
                    int index;
                    string ValueLookup;

                    cout << "LSearch" << endl;
                    cout << endl << "Search Product by Name: ";
                    getline(cin >> ws, ValueLookup);

                    index = linearSearch(record, ValueLookup);

                    system("cls");
                    printVector(record);
                    printHistorySeq(historySeq);

                    if (!(index == -1))
                    {
                        cout << "---Index " << index << " of Price: $"
                            << record[index].price << ", registered on " << record[index].date << "." << endl;

                        cout << "---" << record[index].name << endl;
                        cout << "---" << record[index].source << endl;
                        break;
                    }

                    cout << "---Failed to find input." << endl;
                    break;
                   
                }

                system("cls");
                printVector(record);
                printHistorySeq(historySeq);
                break;

            case '5':
                //import&merge file:

                ImportFile(record, historySeq);
                isSorted = false;
                break;

            case '6':
                //duplicate check:
                //quicksort(record, sortedName_A, sortedName_D, isSorted);

                if (isSorted == 0)
                {
                    system("cls");
                    printHistorySeq(historySeq);
                    cout << "---Function requires records to be sorted by name!" << endl;
                    printVector(record);

                    break;
                }

                //removeDuplicates(record, historySeq);
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
                historySeq.push_back("Restart");
                printHistorySeq(historySeq);
                printVector(record);
                break;

            case '9':
                //quit:
                cout << endl;
                return;

            default:
                system("cls");
                printHistorySeq(historySeq);
                cout << "---Invalid input." << endl;
                printVector(record);
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
    cout << " 3. Element: Insert/**Modify/**Remove" << endl;
    cout << " 4. **Search Element" << endl;
    cout << " 5. Import and Merge File" << endl;
    cout << " 6. Check for Duplicates" << endl;
    cout << " 7. Store Records on File" << endl;
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

void Search(vector<Records> record, vector<string>& historySeq)
{
    /*
    Requires vector to be SortbyName at least once in the program's lifetime, else prompts user to do so.
    Prompts user to input name to lookup through the vector.
    If ValueLookup exists, prints index # and [name:price:quantity] of the search name.
    */

    float ValueLookup;
    cout << endl << "Search Product by Price: ";
    cin >> ValueLookup;

    int index = BinarySearch(record, 0, record.size(), ValueLookup);

    if (index > -1)
    {

        system("cls");

        historySeq.push_back("Search");

        printVector(record);
        printHistorySeq(historySeq);

        cout << "---Index " << index << " of Price: $"
            << record[index].price << ", registered on " << record[index].date << "." << endl;
        
        cout << "---" << record[index].name << endl;
        cout << "---" << record[index].source << endl;

        return;

    }

    return;
}

int BinarySearch(vector<Records> record, int low, int high, float ValueLookup)
{
    if (record[0].price < record[record.size() - 1].price)
    {
        //if sorted in ascending order
        if (low <= high)
        {
            int mid = (low + high) / 2;
            if (ValueLookup == record[mid].price)
                return mid;

            if (ValueLookup < record[mid].price)
                return BinarySearch(record, low, mid - 1, ValueLookup);
            if (ValueLookup > record[mid].price)
                return BinarySearch(record, mid + 1, high, ValueLookup);
        }
    }
    if(record[0].price > record[record.size() - 1].price)
    {
        //if sorted in descending order
        if (low <= high)
        {
            int mid = (low + high) / 2;
            if (ValueLookup == record[mid].price)
                return mid;

            if (ValueLookup > record[mid].price)
                return BinarySearch(record, low, mid - 1, ValueLookup);
            if (ValueLookup < record[mid].price)
                return BinarySearch(record, mid + 1, high, ValueLookup);
        }
    }

    system("cls");
    printVector(record);
    cout << "---Failed to find input." << endl;

    return -1;
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

    string date;
    double price = 0;
    string source;
    string name;
    string key;

    printVector(record);
    historySeq.push_back("removeDupes");
    printHistorySeq(historySeq);
    cout << "---Checking for duplicates." << endl << endl;


    //cout << "---Detected duplicates: " << endl;

    for (int current = 1; current < (int)record.size(); current++)
    {

        if (record[current].name == record[previous].name)
        {
            cout << "---index " << previous << ": "
                << record[previous].date << "  Price: " << record[previous].price << endl
                << record[previous].name << endl;            
            
            cout << "---index " << current << ": "
                << record[current].date << "  Price: " << record[current].price << endl
                << record[current].name << endl;

            cout << "Indicate index # of the duplicate to remove: ";
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
                cout << "Invalid input." << endl;
                cout << "Input the index number of the duplicate to remove." << endl;
                removeDuplicates(record, historySeq);
            }
            detected_dupes++;
        }
        previous++;
    }


    if (detected_dupes == 0)
    {
        system("cls");

        historySeq.push_back("removeDupes");
        printHistorySeq(historySeq);

        cout << "---No duplicates detected." << endl;
        printVector(record);

        return;
    }

    system("cls");
    printHistorySeq(historySeq);
    printVector(record);

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

void quicksort(vector<Records>& record, bool sortedName_A, bool sortedName_D, bool& isSorted)
{
    /*
    Mirror of SortbyName to auto sort based on saved SortbyName(Ascending/Descending) preference.
    */

    if (isSorted == 0)
    {
        if (sortedName_A == 1)
        {
            sort(record.begin(), record.end(), [](Records a, Records b)
                {return a.name < b.name; });

            isSorted = true;
            return;
        }
        else if (sortedName_D == 1)
        {
            sort(record.begin(), record.end(), [](Records a, Records b)
                {return a.name > b.name; });

            isSorted = true;
            return;
        }
    }

    return;
}

int linearSearch(vector<Records> record, float ValueLookup)
{
    int index;
    cout << "linear search..." << endl;
    for (int i = 0; i < (int)record.size(); i++)
    {
        if (record[i].price == ValueLookup)
        {
            index = i;
            return index;
        }       
    }
    return -1;
}

int linearSearch(vector<Records> record, string ValueLookup)
{
    int index;
    cout << "linear search..." << endl;
    for (int i = 0; i < (int)record.size(); i++)
    {
        if (record[i].name.find(ValueLookup) != string::npos)
        {
            index = i;
            return index;
        }
    }
    return -1;
}

void SearchIndex(vector<Records> record, string ValueLookup)
{
    bool Repeat;
    int IndexInput;

    do {
        cout << "Input Index of Search: " << endl;
        Repeat = false;
        cin >> IndexInput;
        //Repeat = isalphaCheck(IndexInput);

        if (Repeat == true)
            cout << "---Invalid Input." << endl;
    } while (Repeat);

    cout << "---Index " << IndexInput << " of Price: $"
        << record[IndexInput].price << ", registered on " << record[IndexInput].date << "." << endl;

    cout << "---" << record[IndexInput].name << endl;
    cout << "---" << record[IndexInput].source << endl;


}