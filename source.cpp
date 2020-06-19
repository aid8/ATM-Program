#include <iostream>
#include <vector>
#include <fstream>
#include <stdlib.h> 
//#define cls system("cls"); //for windows
#define cls system("clear"); //for linux
using namespace std;

struct BankAC
{
    string acNum;
    string fName, mName, lName;
    string conNum;
    string pin;
    string state;
    float balance;
    int retries;
};

/*====================GLOBAL VARIABLES=================*/
vector<BankAC> record;
const int BSIZE = 54; //size of menu
const int maxRetry = 5; //max retry to block account
char currentMenu = ' ';
string bankName = "ATM++";
int currentAC = -1;
/*=====================================================*/

/*================SAVE AND LOAD FUNCTIONS==============*/
void loadData()
{
	ifstream inFile("data.in");
    BankAC bc;
    string s;
    while(inFile >> bc.acNum)
    {
    	inFile.ignore();
    	getline(inFile, bc.fName);
    	getline(inFile, bc.mName);
    	getline(inFile, bc.lName);
    	inFile >> bc.conNum;
    	inFile >> bc.pin;
    	inFile >> bc.state;
    	inFile >> bc.retries;
    	getline(inFile, s);
    	record.push_back(bc);
    }
}

void saveData()
{
	ofstream outFile("data.in");
	for(int i = 0; i < record.size(); i++)
	{
		outFile << record[i].acNum << "\n";
		outFile << record[i].fName << "\n";
		outFile << record[i].mName << "\n";
		outFile << record[i].lName << "\n";
		outFile << record[i].conNum << "\n";
		outFile << record[i].pin << "\n";
		outFile << record[i].state << "\n";
		outFile << record[i].retries << "\n\n";
	}
}

/*====================================================*/

/*===================OTHER FUNCTIONS==================*/
void mcout(string s = " ", char c = ' ')
{
    int sp = BSIZE-s.length()-2;
    cout << "|" << string(sp/2 + (s.length()%2 == 1), c) << s << string(sp/2, c) << "|\n";
}

void border(char c = '=')
{
    cout << string(BSIZE, c) << "\n";
}

void displayChoices(vector<string> v)
{
    int i, hc = 0, l, l2;
    for(i = 1; i < v.size(); i+=2) if(i < v.size()) if(v[i].length() > hc) hc = v[i].length();
   
    for(i = 0; i < v.size(); i+=2)
    {
        cout << "|" << string(BSIZE/10 - 1, ' ') << "[" << i+1 << "] " << v[i];
        if(i+1 < v.size()){
        	l = (BSIZE-(BSIZE/10)-v[i].length()) - (BSIZE/10 + hc)-8;
        	l2 = BSIZE - (BSIZE/10 + 4 + v[i].length() + l + 4 + v[i+1].length()) - 1;
        	cout << string(l, ' ')  << "[" << i+2 << "] " << v[i+1] << string(l2, ' ') << "|";
		}
        else cout << string(BSIZE - (BSIZE/10 + 4 + v[i].length()) -1, ' ') << "|";
        cout << "\n";
        mcout();
    }
}

string twodec(float f) 
{ 
	//converts 2 float to two decimal point and returns stirng
	string s = to_string(f);
	return s.substr(0,s.find(".")+3);
} 

//Use this function if you have a menu to display with choices
char displayMenu(string title, vector<string> info, vector<string> choices)
{
	char c;
	int i;
	bool error = false;
    
    while(true)
	{
	    border();
	    mcout(title);
	    border();
	    mcout();
	    if(error)
		{
			mcout("Invalid Input, Please try again!",'+');
			mcout();
		}
		
		for(i = 0; i < info.size(); i++)
		{
		    mcout(info[i]);
		    mcout();
		}
	    displayChoices(choices);
	    //mcout();
	    border();
	    
	    string s;
	    cout << "Input: "; cin >> s;
	    c = s[0];
	    if(c < '1' || c > char(choices.size())+'0' || s.length() > 1 || !isdigit(c)) error = true;
	    else break;
	    cls;
	}
	cls;
	return c;
}

//Error messages
void showError(bool errorFlag[])
{
    //ErrorMessages
    if(errorFlag[0])
	{
		mcout("Name should be less than 20 characters",'+');
		mcout();
	}
	if(errorFlag[1])
	{
		mcout("Number should be less than 15 characters",'+');
		mcout();
	}
	if(errorFlag[2])
	{
		mcout("Only letters are allowed",'+');
		mcout();
	}
	if(errorFlag[3])
	{
		mcout("Only numbers are allowed",'+');
		mcout();
	}
	if(errorFlag[4])
	{
		mcout("Invalid choice",'+');
		mcout();
	}
}

/*====================================================*/

/*======================MAIN MENU=====================*/
char displayMainMenu()
{
    /*Info to Return
    1 = Bank
    2 = ATM
    3 = Exit
    */
    string title = "Welcome to " + bankName;
    vector<string> info = {
		"Where do you want to go?"
	};
	vector<string> choices = {
		"Bank",
		"ATM",
		"Exit"
	};
    return displayMenu(title, info, choices);
}
/*====================================================*/

/*====================BANK FUNCTIONS==================*/

void logout()
{
	currentMenu = ' ';
	currentAC = -1;
}

char displayBank()
{
    /*Info to Return
    1 = New Account
    2 = Update Account
    3 = Delete Account
    4 = Unblock Account
    5 = Exit
    */
    string title = bankName + " BANK";
    vector<string> info = {
		"Welcome to the Bank",
		"Please select your choice"
	};
	vector<string> choices = {
		"New Account",
		"Update Account",
		"Delete Account",
		"Unblock Account",
		"Exit"
	};
    return displayMenu(title, info, choices);
}

/*====================NEW ACCOUNT==================*/
void newAccount()
{
	BankAC newAcc = {"","","","","","","new",0.0,0};
    bool enterFlag[4] = {false,false,false,false};
    bool errorFlag[5] = {false,false,false,false,false};
   	bool duplicate = false;
    int confirm = 0;
    string idfill(8 - to_string(record.size()).length(),'0');
    
    newAcc.acNum = idfill + to_string(record.size());
    newAcc.pin = to_string(rand() % 9000 + 1000);
    //generate idnum
    //string 
    //generate pin
    cin.ignore();
	while(true)
	{
	    border();
	    mcout("New Account Form");
	    border();
	    mcout();
	    
	    showError(errorFlag);
	   
		if(confirm == 0) mcout("Please fill out the questions");
		else if(confirm == 1) mcout("Are you sure about this data?");
		else mcout("What do you want to change? ");
		mcout();
		
		//First Name
		if(!enterFlag[0])
		{
			errorFlag[0] = false;
			errorFlag[2] = false;
			
			mcout("First Name:");
			mcout();
			border();
			cout << "Input: ";
			getline(cin, newAcc.fName);
			
			if(newAcc.fName.length() > 20) errorFlag[0] = true;
			for(int i = 0; i < newAcc.fName.length(); i++) 
				if(!isalpha(newAcc.fName[i]) && newAcc.fName[i] != ' ') 
				{
					errorFlag[2] = true;
					break;
				} 
			
			if(!errorFlag[0] && !errorFlag[2]) enterFlag[0] = true;
			cls;
			continue;
		}
		mcout("First Name: " + newAcc.fName); 
		
		//Middle Name
		if(!enterFlag[1])
		{
			errorFlag[0] = false;
			errorFlag[2] = false;
			
			mcout("Middle Name:");
			mcout();
			border();
			cout << "Input: ";
			getline(cin, newAcc.mName);
			
			if(newAcc.mName.length() > 20) errorFlag[0] = true;
			for(int i = 0; i < newAcc.mName.length(); i++) 
				if(!isalpha(newAcc.mName[i]) && newAcc.mName[i] != ' ') 
				{
					errorFlag[2] = true;
					break;
				} 
			
			if(!errorFlag[0] && !errorFlag[2]) enterFlag[1] = true;
			cls;
			continue;
		}
		mcout("Middle Name: " + newAcc.mName); 
		
		
		//Last Name
		if(!enterFlag[2])
		{
			errorFlag[0] = false;
			errorFlag[2] = false;
			mcout("Last Name:");
			mcout();
			border();
			cout << "Input: ";
			getline(cin, newAcc.lName);
			
			if(newAcc.lName.length() > 20) errorFlag[0] = true;
			for(int i = 0; i < newAcc.lName.length(); i++) 
				if(!isalpha(newAcc.lName[i]) && newAcc.lName[i] != ' ') 
				{
					errorFlag[2] = true;
					break;
				} 
			
			if(!errorFlag[0] && !errorFlag[2]) enterFlag[2] = true;
			cls;
			continue;
		}
		mcout("Last Name: " + newAcc.lName); 
		
		//Contact Number
		if(!enterFlag[3])
		{
			errorFlag[1] = false;
			errorFlag[3] = false;
			mcout("Contact #:");
			mcout();
			border();
			cout << "Input: ";
			getline(cin, newAcc.conNum);
			
			if(newAcc.conNum.length() > 15) errorFlag[1] = true;
			for(int i = 0; i < newAcc.conNum.length(); i++) 
				if(!isdigit(newAcc.conNum[i]) && newAcc.conNum[i] != ' ') 
				{
					errorFlag[3] = true;
					break;
				} 
			
			if(!errorFlag[1] && !errorFlag[3]) enterFlag[3] = true;
			cls;
			continue;
		}
		mcout("Contact #: " + newAcc.conNum);
		mcout();
		
		if(enterFlag[0] && enterFlag[1] && enterFlag[2] && enterFlag[3] && confirm == 0){
			confirm = 1;
			cls;
			continue;
		}
		
		//Confirmation
		mcout();
		string s;
		char c;
		errorFlag[4] = false;
		
		if(confirm == 1)
		{
			displayChoices(vector<string>{"Yes","No"});
			mcout();
			border();
			
			cout << "Input: "; cin >> s;
			c = s[0];
			if(s.length() > 1)
			{
				errorFlag[4] = true;
				cls;
				continue;
			}
			
			if(c == '1')
			{
				//check for duplicates
				for(int i = 0; i < record.size(); ++i)
				{
					if(record[i].fName == newAcc.fName && record[i].lName == newAcc.lName && record[i].mName == newAcc.mName)
					{
						cls;
						vector<string> info = {
							"Duplicate Name Found!",
							"If you forgot your pin, please go",
							"to Update Account section. Thank you!"
						};
						displayMenu("Failed To Register Account", info, vector<string>{"Back to Menu"});
						duplicate = true;
						break;
					}	
				}
				break;
			}
			else
			{
				confirm = 2;
				cls;
				continue;
			}
		}
		if(confirm == 2)
		{
			displayChoices(vector<string>{"First Name","Middle Name","Last Name","Contact Number","Cancel","Back To Menu"});
			mcout();
			border();
			
			cout << "Input: "; cin >> s;
			c = s[0];
			if(s.length() > 1)
			{
				errorFlag[4] = true;
				cls;
				continue;
			}
			
			if(c == '1')
			{
				enterFlag[0] = false;
				confirm = 0;
				cls;
				cin.ignore();
				continue;
			}
			if(c == '2')
			{
				enterFlag[1] = false;
				confirm = 0;
				cls;
				cin.ignore();
				continue;
			}
			if(c == '3')
			{
				enterFlag[2] = false;
				confirm = 0;
				cls;
				cin.ignore();
				continue;
			}
			if(c == '4')
			{
				enterFlag[3] = false;
				confirm = 0;
				cls;
				cin.ignore();
				continue;
			}
			if(c == '5')
			{
				confirm = 1;
				cls;
				continue;
			}
			if(c == '6')
			{
				cls;
				currentMenu = '1';
				return;
			}
		}
		cls;	
	}
	cls;
	
	if(duplicate)
	{
		currentMenu = '1';
		return;
	}
	
	vector<string> info = {
		"Welcome to " + bankName,
		newAcc.fName + " " + newAcc.lName,
		"Account #: " + newAcc.acNum,
		"Temporary Pin: " + newAcc.pin,
		" ",
		"Note: You can change your temporary",
		"pin upon logging in your account."
	};
	displayMenu("Succesfully Registered", info, vector<string>{"Back to Menu"});
	
	record.push_back(newAcc);
	currentMenu = '1';
}
/*===============================================*/

/*==================UPDATE ACCOUNT================*/
void updateAccount()
{
    BankAC *bc, oldData;
    string accNumF = "";
    bool errorFlag[5] = {false,false,false,false,false};
    bool accAvail = false, cancelMenu = false, inputtedPin = false;;
    
	//find account
	border();
	mcout("Update Account");
	border();
	mcout();
	mcout("Account #: ");
	mcout();
	border();
	cout << "Input: "; cin >> accNumF;
	
	for(int i = 0; i < record.size(); i++)
		if(record[i].acNum == accNumF)
		{
			bc = &record[i];
			oldData = record[i];
			accAvail = true;
			break;
		}
    
	if(!accAvail)
	{
		cls;
		char c2 = displayMenu("Failed to Update Account", vector<string>{"Account # not found"}, vector<string>{"Retry","Back to Menu"});
		if(c2 == '1') updateAccount();
		//else
		currentMenu = '1';
		return;
	}
	cls;
	char c = displayMenu("Update Account" , vector<string>{"Please input your choice"}, vector<string>{"Update Personal Info", "Change Pin"});
    
    while(true)
	{
		//Update Personal Info
	    if(c == '1')
		{
			bool toMain = false;
			string pinCon;
			while(!inputtedPin)
			{
				string pin;
				cls;
				border();
				mcout("Update Account");
				border();
				mcout();
				mcout("Enter Pin #: "); 
				mcout();
				border();
				cout << "Input: "; cin >> pinCon;
				if(pinCon == bc->pin)
				{
					inputtedPin = true;
					break;
				}
				cls;
				char c2 = displayMenu("Failed to Update Account", vector<string>{"Pin is invalid"}, vector<string>{"Retry","Back to Menu"});
				if(c2 == '2')
				{
					toMain = true;
					break;
				}
			}
			if(toMain)
			{
				currentMenu = '1';
				return;
			}
			
			cls;
			char c2 = displayMenu("Update Account" , vector<string>{"What do you want to change?"},vector<string>{"First Name","Middle Name","Last Name","Contact Number","Cancel"});
			cin.ignore();
			
			while(true)
			{
			    border();
			    mcout("Update Account");
			    border();
			    mcout();
			 	
				showError(errorFlag);
				
				if(c2 == '1')
				{
					errorFlag[0] = false;
					errorFlag[2] = false;
					mcout("Old First Name: " + bc->fName);
					mcout("New First Name:");
					mcout();
					border();
					cout << "Input: ";
					getline(cin, bc->fName);
					
					if(bc->fName.length() > 20) errorFlag[0] = true;
					for(int i = 0; i < bc->fName.length(); i++) 
						if(!isalpha(bc->fName[i]) && bc->fName[i] != ' ') 
						{
							errorFlag[2] = true;
							break;
						} 
					if(!errorFlag[0] && !errorFlag[2]) break;
					cls;
					continue;
				}
				
				if(c2 == '2')
				{
					errorFlag[0] = false;
					errorFlag[2] = false;
					mcout("Old Middle Name: " + bc->mName);
					mcout("New Middle Name:");
					mcout();
					border();
					cout << "Input: ";
					getline(cin, bc->mName);
					
					if(bc->mName.length() > 20) errorFlag[0] = true;
					for(int i = 0; i < bc->mName.length(); i++) 
						if(!isalpha(bc->mName[i]) && bc->mName[i] != ' ') 
						{
							errorFlag[2] = true;
							break;
						} 
					if(!errorFlag[0] && !errorFlag[2]) break;
					cls;
					continue;
				}
				
				if(c2 == '3')
				{
					errorFlag[0] = false;
					errorFlag[2] = false;
					mcout("Old Last Name: " + bc->lName);
					mcout("New Last Name:");
					mcout();
					border();
					cout << "Input: ";
					getline(cin, bc->lName);
					
					if(bc->lName.length() > 20) errorFlag[0] = true;
					for(int i = 0; i < bc->lName.length(); i++) 
						if(!isalpha(bc->lName[i]) && bc->lName[i] != ' ') 
						{
							errorFlag[2] = true;
							break;
						} 
					if(!errorFlag[0] && !errorFlag[2]) break;
					cls;
					continue;
				}
				
				if(c2 == '4')
				{
					errorFlag[1] = false;
					errorFlag[3] = false;
					mcout("Old Contact #: " + bc->conNum);
					mcout("New Contact #:");
					mcout();
					border();
					cout << "Input: ";
					getline(cin, bc->conNum);
					
					if(bc->conNum.length() > 15) errorFlag[1] = true;
					for(int i = 0; i < bc->conNum.length(); i++) 
						if(!isdigit(bc->conNum[i]) && bc->conNum[i] != ' ') 
						{
							errorFlag[3] = true;
							break;
						} 
					
					if(!errorFlag[1] && !errorFlag[3]) break;
					cls;
					continue;
				}
				
				if(c2 == '5')
				{
					cancelMenu = true;
					break;
				}
				
			}
		}
		//change pin
		else
		{
			char c3 = displayMenu("Update Account", vector<string>{"What do you want to do?"}, vector<string>{"Reset Pin", "Change Pin","Cancel"});
			string fName = "", mName = "", lName = "", conNum = "", pinCon, newPin;
			cls;
			cin.ignore();
			
			//Verify Account
			if(c3 == '1')
			{
				while(conNum == "")
				{
					border();
					mcout("Account Verification");
					border();
					mcout();
					//get details
					if(fName == "")
					{
						mcout("Verify First Name: ");
						mcout();
						border();
						cout << "Input: ";
						getline(cin, fName); 
						cls;
						continue;
					}
					else mcout("First Name: " + fName);
					
					if(mName == "")
					{
						mcout("Verify Middle Name: ");
						mcout();
						border();
						cout << "Input: ";
						getline(cin, mName);
						cls;
						continue; 
					}
					else mcout("Middle Name: " + mName);
					
					if(lName == "")
					{
						mcout("Verify Last Name: ");
						mcout();
						border();
						cout << "Input: ";
						getline(cin, lName);
						cls;
						continue; 
					}
					else mcout("Last Name: " + lName);
					
					if(conNum == "")
					{
						mcout("Verify Contact #: ");
						mcout();
						border();
						cout << "Input: ";
						getline(cin, conNum);
						cls; 
					}
				}
				vector<string> info = {
					"Checking if these informations",
					"are valid",
					"First Name: " + fName,
					"Middle Name: " + mName,
					"Last Name: " + lName,
					"Contact #: " + conNum
				};
				char c4 = displayMenu("Account Verification", info, vector<string>{"Confirm","Back"});
				if(c4 == '2') continue;
				//check
				if(bc->fName == fName && bc->mName == mName && bc->lName == lName && bc->conNum == conNum)
				{
					bc->pin = to_string(rand() % 9000 + 1000);
					info = {"Your New Pin: " + bc->pin};
					displayMenu("Successfully Updated", info, vector<string>{"Back to Menu"});
					cancelMenu = true;
				}
				else
				{
					info = {"The information given is", "not identical to the","accounts information"};
					displayMenu("Failed to Update Account", info, vector<string>{"Back"});
					continue;
				}
			}
			else if(c3 == '2')
			{
				while(true)
				{
					border();
					mcout("Update Account");
					border();
					mcout();
					mcout("Enter Pin #: "); 
					mcout();
					border();
					cout << "Input: "; cin >> pinCon;
					if(pinCon != bc->pin)
					{
						cls;
						char c4 = displayMenu("Update Account", vector<string>{"Invalid Pin Entered!"}, vector<string>{"Retry","Back to Menu"});
						if(c4 == '2')
						{
							currentMenu = '1';
							return;
						}
						continue;
					}
					else
					{
						currentMenu = '1';
						cancelMenu = true;
						break;
					}
				}
				
				bool errorFlag = false;
				while(true)
				{
					cls;
					border();
					mcout("Update Account");
					border();
					mcout();
					
					if(errorFlag)
					{
						mcout("Pin should be exactly 4 numbers",'+');
						mcout();
					}
					
					mcout("Enter New Pin #: "); 
					mcout();
					border();
					cout << "Input: "; cin >> newPin;
					
					if(newPin.length() != 4) errorFlag = true;
					else
					{
						bool goodPin = true;;
						for(int i = 0; i < newPin.length(); ++i) 
							if(!isdigit(newPin[i]))
							{
								goodPin = false;
								break;
							}
						if(!goodPin) errorFlag = true;
						else errorFlag = false;
					}
					if(!errorFlag) break;
				}
				bc->pin = newPin;
				cls;
				displayMenu("Successfully Updated", vector<string>{"Your New Pin: " + newPin}, vector<string>{"Back to Menu"});
			}
			else cancelMenu = true;
		}
		cls;
		if(cancelMenu)
		{
			currentMenu = '1';
			break;
		}
		
		bool duplicate = false;
		char dupc;
		//check for duplicates
		for(int i = 0; i < record.size(); ++i)
		{
			if(bc->acNum != record[i].acNum && record[i].fName == bc->fName && record[i].lName == bc->lName)
			{
				cls;
				vector<string> info = {
					"Duplicate Name Found!",
					"Please input your correct name.",
					"Thank you!"
				};
				dupc = displayMenu("Failed To Update Account", info, vector<string>{"Update Other Info", "Back to Menu"});
				duplicate = true;
				break;
			}	
		}
		if(duplicate)
		{
			*bc = oldData;
			cout << dupc << "\n";
			if(dupc == '2')
			{
				currentMenu = '1';
				break;
			}
		}
		else
		{
			vector<string> info = {
			"First Name: " + bc->fName,
			"Middle Name: " + bc->mName,
			"Last Name: " + bc->lName,
			"Contact #: " + bc->conNum
			};
			char c2 = displayMenu("Succesfully Updated", info, vector<string>{"Update Other Info", "Back to Menu"});
			if(c2 == '2')
			{
				currentMenu = '1';
				break;
			} 
		}
	}
}
/*====================================================*/

/*====================DELETE ACCOUNT==================*/
void deleteAccount()
{
	int index = -1;
	string accNumF = "", pin = "";
	bool correctData = false, blocked = false;
	vector<string> info;
	
	while(true)
	{
		border();
		mcout("Delete Account");
		border();
		mcout();
		
		if(accNumF == "")
		{
			mcout("Account #: ");
			mcout();
			border();
			cout << "Input: "; cin >> accNumF;
			cls;
			continue;
		}
		else mcout("Account #: " + accNumF);
		
		if(pin == "")
		{
			mcout("Pin #: ");
			mcout();
			border();
			cout << "Input: "; cin >> pin;
			cls;
			continue;
		}
		break;
	}
	
	for(int i = 0; i < record.size(); i++)
		if(record[i].acNum == accNumF && record[i].pin == pin)
		{
			correctData = true;
			if(record[i].state == "blocked") blocked = true;
			index = i;
			break;
		}
    
	if(!correctData || blocked)
	{
		cls;
		if(!correctData) info = {"Invalid Account # or Pin"};
		if(blocked) info = {"Account is Blocked","Please unblock first before deleting"};
		
		char c2 = displayMenu("Failed to Delete Account", info, vector<string>{"Retry","Back to Menu"});
		if(c2 == '1') deleteAccount();
		//else
		currentMenu = '1';
		return;
	}
	cls;
	info = {
		"Are you sure you want to",
		"delete this account?",
		"First name: " +  record[index].fName,
		"Middle name: " + record[index].mName,
		"Last name " + record[index].lName,
		"Contact Number: " + record[index].conNum,
		"Balance: " + twodec(record[index].balance),
		"Note: Upon deleting account, you",
		"can get your remaining balance."
	};
	char c = displayMenu("Delete Account",info,vector<string>{"Yes","Cancel"});
	
	if(c == '1')
	{
		cls;
		record.erase(record.begin()+index);
		info = {"The Account is no longer in " + bankName};
		displayMenu("Successfully Deleted", info, vector<string>{"Back to Menu"});
	}
	cls;
}
/*====================================================*/

/*====================UNBLOCK ACCOUNT==================*/
void unblockAccount()
{
	string accNumF = "", pin = "";
	bool correctData = false, blocked = false;
	vector<string> info;
	BankAC *bc;
	   	
	border();
	mcout("Unblock Account");
	border();
	mcout();
	mcout("Account #: ");
	mcout();
	border();
	cout << "Input: "; cin >> accNumF;
	cls;
		
	for(int i = 0; i < record.size(); i++)
		if(record[i].acNum == accNumF)
		{
			correctData = true;
			if(record[i].state == "blocked") blocked = true;
			bc = &record[i];
			break;
		}
    
	if(!correctData || !blocked)
	{
		cls;
		if(!correctData) info = {"Cannot find Account Number"};
		else if(!blocked) info = {"Account is not blocked","It does not need to be unblocked"};
		char c2 = displayMenu("Failed to Unblock Account", info, vector<string>{"Retry","Back to Menu"});
		if(c2 == '1') unblockAccount();
		//else
		currentMenu = '1';
		return;
	}
	cls;
	info = {"Select method in unblocking account"};
	char c = displayMenu("Unblock Account",info,vector<string>{"Using Pin","Verifying Info","Cancel"});
	
	if(c == '1')
	{
		cls;
		while(true)
		{
			border();
			mcout("Delete Account");
			border();
			mcout();
			mcout("Pin #: ");
			mcout();
			border();
			cout << "Input: "; cin >> pin;
			if(pin != bc->pin)
			{
				cls;
				char c2 = displayMenu("Unblock Account", vector<string>{"Invalid Pin Entered!"}, vector<string>{"Retry","Back to Menu"});
				if(c2 == '1') continue;
				currentMenu = '1';
				return;
			}
			break;
		}
	}
	if(c == '2')
	{
		string fName = "", mName = "", lName = "", conNum = "", newPin;
		cls;
		cin.ignore();
			
		//Verify Account
		while(conNum == "")
		{
			border();
			mcout("Account Verification");
			border();
			mcout();
			//get details
			if(fName == "")
			{
				mcout("Verify First Name: ");
				mcout();
				border();
				cout << "Input: ";
				getline(cin, fName); 
				cls;
				continue;
			}
			else mcout("First Name: " + fName);
				
			if(mName == "")
			{
				mcout("Verify Middle Name: ");
				mcout();
				border();
				cout << "Input: ";
				getline(cin, mName);
				cls;
				continue; 
			}
			else mcout("Middle Name: " + mName);
			
			if(lName == "")
			{
				mcout("Verify Last Name: ");
				mcout();
				border();
				cout << "Input: ";
				getline(cin, lName);
				cls;
				continue; 
			}
			else mcout("Last Name: " + lName);
			
			if(conNum == "")
			{
				mcout("Verify Contact #: ");
				mcout();
				border();
				cout << "Input: ";
				getline(cin, conNum);
				cls; 
			}
		}
		vector<string> info = {
			"Checking if these informations",
			"are valid",
			"First Name: " + fName,
			"Middle Name: " + mName,
			"Last Name: " + lName,
			"Contact #: " + conNum
		};
		char c2 = displayMenu("Account Verification", info, vector<string>{"Confirm","Cancel"});
		if(c2 == '2')
		{
			currentMenu = '1';
			return;
		}
		if(bc->fName != fName || bc->mName != mName || bc->lName != lName || bc->conNum != conNum)
		{
			info = {"The information given is", "not identical to the","accounts information"};
			displayMenu("Failed to Update Account", info, vector<string>{"Back"});
			currentMenu = '1';
			return;
		}
		else
		{
			bc->pin = to_string(rand() % 9000 + 1000);
			info = {"Your account is unblocked","Your New Pin: " + bc->pin};
			bc->state = "new";
			displayMenu("Successfully Unblocked", info, vector<string>{"Back to Menu"});
			currentMenu = '1';
			return;
		}
	}
	if(c == '3')
	{
		currentMenu = '1';
		return;
	}
	cls;
	info = {"Your account is unblocked","You can now use your account"};
	bc->state = "old";
	displayMenu("Successfully Unblocked", info, vector<string>{"Back to Menu"});
	currentMenu = '1';
	cls;
}
/*====================================================*/


/*=====================ATM FUNCTIONS==================*/
void changePin(bool newUser)
{
	string pinCon, newPin;
	
	if(!newUser)
	{
		while(true)
		{
			border();
			mcout("Change Pin");
			border();
			mcout();
			mcout("Enter Old Pin #: "); 
			mcout();
			border();
			cout << "Input: "; cin >> pinCon;
			if(pinCon != record[currentAC].pin)
			{
				cls;
				char c4 = displayMenu("Change Pin", vector<string>{"Invalid Pin Entered!"}, vector<string>{"Retry","Back to Menu"});
				if(c4 == '2')
				{
					currentMenu = '2';
					return;
				}
				continue;
			}
			break;
		}
	}
		
	bool errorFlag = false;
	while(true)
	{
		cls;
		border();
		mcout("Change Pin");
		border();
		mcout();
		
		if(errorFlag)
		{
			mcout("Pin should be exactly 4 numbers",'+');
			mcout();
		}
		
		mcout("Enter New Pin #: "); 
		mcout();
		border();
		cout << "Input: "; cin >> newPin;
		
		if(newPin.length() != 4) errorFlag = true;
		else
		{
			bool goodPin = true;;
			for(int i = 0; i < newPin.length(); ++i) 
				if(!isdigit(newPin[i]))
				{
					goodPin = false;
					break;
				}
			if(!goodPin) errorFlag = true;
			else errorFlag = false;
		}
		if(!errorFlag) break;
	}
	record[currentAC].pin = newPin;
	cls;
	displayMenu("Successfully Changed Pin", vector<string>{"Your New Pin: " + newPin}, vector<string>{"Back to Menu"});
	currentMenu = '2';
}

bool loginATM()
{
	if(currentAC != -1) return true;
	 
	string accNumF = "", pin = "";
	bool correctData = false, blocked = false;
	vector<string> info;
	
	while(true)
	{
		border();
		mcout(bankName + " Login");
		border();
		mcout();
		
		if(accNumF == "")
		{
			mcout("Enter Account #: ");
			mcout();
			border();
			cout << "Input: "; cin >> accNumF;
			cls;
			continue;
		}
		else mcout("Account #: " + accNumF);
		
		if(pin == "")
		{
			mcout("Enter Pin #: ");
			mcout();
			border();
			cout << "Input: "; cin >> pin;
		}
		
		for(int i = 0; i < record.size(); i++)
		{
			if(record[i].acNum == accNumF && record[i].pin == pin)
			{
				correctData = true;
				record[i].retries = 0;
				if(record[i].state == "blocked") blocked = true;
				else currentAC = i;
				
				if(record[i].state == "new")
				{
					cls;
					char c2 = displayMenu(bankName + " ATM", vector<string>{"This Account is New", "Do you want to change your temporary pin?","We recommend to change your pin."},vector<string>{"Yes","Next time"});
					if(c2 == '1')
					{
						record[i].state = "old";
						changePin(true);		
					}
				}
				
				if(correctData && !blocked) return true;
			}
			else if(record[i].acNum == accNumF && record[i].pin != pin)
			{
				record[i].retries++;
				if(record[i].retries >= maxRetry) record[i].state = "blocked";
			}
		}
		
		if(!correctData || blocked)
		{
			cls;
			if(!correctData) info = {"Invalid Account # or Pin #"};
			else if(blocked) info = {"Account is blocked","Please unblock the account","before logging in"};
			char c2 = displayMenu("Failed to Login", info, vector<string>{"Retry","Back to Menu"});
			if(c2 == '1') loginATM();
			else
			{
				return false;
			}
		}
		cls;
		break;
	}
}

char displayATM()
{
	bool login = loginATM();
	if(!login){
		currentMenu = ' ';
		currentAC = -1;
		return '0';
	}
	
    /*Info to Return
    1 = Balance Inquiry
    2 = Withdraw
    3 = Deposit
    4 = Change Pin
    5 = Load Mobile
    6 = Fund Transfer
    7 = Exit
    */
    cls;
    string title = bankName + " ATM";
    vector<string> info = {
		"Welcome to the ATM",
		"Please select your choice"
	};
	vector<string> choices = {
		"Balance Inquiry",
		"Withdraw",
		"Deposit",
		"Change Pin",
		"Load Mobile",
		"Fund Transfer",
		"Exit"
	};
    return displayMenu(title, info, choices);
}

void balanceInquiry()
{
	vector<string> info = {"Your Balance is ", twodec(record[currentAC].balance)};
	char c = displayMenu("Balance Inquiry", info, vector<string>{"Another Transaction","Exit"});
	if(c == '1') currentMenu = '2';
	else logout();
}

void withdraw()
{
	float money, uMoney = record[currentAC].balance;
	char c;
	cls;
	border();
	mcout("Withdraw");
	border();
	mcout();
	mcout ("How much do you want to withdraw?");
	mcout("Input value: ");
	mcout();
	border();
	cout << "Input: "; cin >> money;
	vector<string> info = {};
	if(money > uMoney) info.push_back("Not enough money");
	if(money < 200) info.push_back("Cannot withdraw money less than 200");
	if(int(money) % 100 != 0) info.push_back("Cannot withdraw bills less than 100");
	
	if(info.size() > 0)
	{
		cls;
		c = displayMenu("Failed to Withdraw Money",info,vector<string>{"Retry","Another Transaction","Exit"});
		if(c == '1') withdraw();
		else if(c == '2') currentMenu = '2';
		else logout();
	}
	else
	{
		cls;
		info = {"Are you sure you want to withdraw",twodec(money)+"?"};
		c = displayMenu("Withdraw",info,vector<string>{"Yes","Cancel"});
		if(c == '2')
		{
			currentMenu = '2';
			return;
		}
		
		cls;
		record[currentAC].balance -= money;
		info = {"You have Withdrawn",twodec(money),"Your Current Balance is",twodec(record[currentAC].balance)};
		c = displayMenu("Successfully Withdrawn Money",info,vector<string>{"Withdraw again","Another Transaction","Exit"});
		if(c == '1') withdraw();
		else if(c == '2') currentMenu = '2';
		else logout();
	}
}

void deposit()
{
	float money;
	vector<string>info;
	char c;
	border();
	mcout("Deposit");
	border();
	mcout();
	mcout ("How much do you want to deposit?");
	mcout("Input value: ");
	mcout();
	border();
	cout << "Input: "; cin >> money;
	cls;
	info = {"Are you sure you want to deposit",twodec(money)+"?"};
	c = displayMenu("Deposit",info,vector<string>{"Yes","Cancel"});
	if(c == '2')
	{
		currentMenu = '2';
		return;
	}
	cls;
	record[currentAC].balance += money;
	info = {"You have Deposited",twodec(money),"Your Current Balance is",twodec(record[currentAC].balance),""};
	c = displayMenu("Successfully Deposited Money",info,vector<string>{"Deposit again","Another Transaction","Exit"});
	if(c == '1') deposit();
	else if(c == '2') currentMenu = '2';
	else logout();
}

void loadMobile()
{
	string mobileNum = "";
	float load = 0.0;
	vector<string> info;
	char c;
	bool errorFlag = false, validNum;
	
	while(true)
	{
		cls;
		border();
		mcout("Load Mobile");
		border();
		mcout();
		
		if(errorFlag)
		{
			mcout("Invalid Input, Please try again!",'+');
			mcout();
		}
		
		if(mobileNum == "")
		{
			mcout("Enter Mobile #: "); 
			mcout();
			border();
			cout << "Input: "; cin >> mobileNum;
			continue;
		}
		else mcout("Mobile #: " + mobileNum);
		
		if(load == 0.0)
		{
			mcout("Enter Amount to Load: ");
			mcout();
			border();
			cout << "Input: "; cin >> load;
			continue;
		}
		else mcout("Amount: " + twodec(load));
		mcout();
		mcout("Are you sure you want to load");
		mcout(twodec(load) + " to " + mobileNum + "?");
		mcout();
		displayChoices(vector<string>{"Yes","Change Details","Cancel"});
		border();
	    
	    string s;
	    cout << "Input: "; cin >> s;
	    c = s[0];
		
		if(c == '1') break;
		else if(c == '2')
		{
			mobileNum = "";
			load = 0.0;
			errorFlag = false;
			continue;
		}
		else if(c == '3')
		{
			currentMenu = '2';
			return;
		}
		else errorFlag = true;
	}
	cls;
	
	for(int i = 0; i < record.size(); i++)
	{
		if(record[i].conNum == mobileNum)
		{
			validNum = true;
			break;
		}
	}
	
	if(load > record[currentAC].balance || !validNum)
	{
		if(load > record[currentAC].balance) info = {"You have insufficient balance","to load "+ twodec(load)};
		else  info = {"Invalid Number Entered"};
		c = displayMenu("Failed to Load Mobile",info,vector<string>{"Load again","Another Transaction","Exit"});
		if(c == '1') loadMobile();
		else if(c == '2')currentMenu = '2';
		else logout();
	}
	else
	{
		record[currentAC].balance -= load;
		info = {"You have loaded",twodec(load) + " to " + mobileNum,"Your Current Balance is",twodec(record[currentAC].balance),""};
		c = displayMenu("Successfully Loaded Mobile",info,vector<string>{"Load again","Another Transaction","Exit"});
		if(c == '1') loadMobile();
		else if(c == '2') currentMenu = '2';
		else logout();
	}
}

void fundTransfer()
{
   string accNum = "";
   float toTrans = 0.0;
   BankAC *bc = NULL;
   
	vector<string> info;
	char c;
	bool errorFlag = false;
	
	while(true)
	{
		cls;
		border();
		mcout("Fund Transfer");
		border();
		mcout();
		
		if(errorFlag)
		{
			mcout("Invalid Input, Please try again!",'+');
			mcout();
		}
		
		if(accNum == "")
		{
			mcout("Enter Account # to Transfer: "); 
			mcout();
			border();
			cout << "Input: "; cin >> accNum;
			continue;
		}
		else mcout("Account # to Transfer: " + accNum);
		
		if(toTrans == 0.0)
		{
			mcout("Enter Amount to Transfer: ");
			mcout();
			border();
			cout << "Input: "; cin >> toTrans;
			continue;
		}
		else mcout("Amount: " + twodec(toTrans));
		mcout();
		mcout("Are you sure you want to transfer");
		mcout(twodec(toTrans) + " to " + accNum + "?");
		mcout();
		displayChoices(vector<string>{"Yes","Change Details","Cancel"});
		border();
	    
	    string s;
	    cout << "Input: "; cin >> s;
	    c = s[0];
		
		if(c == '1') break;
		else if(c == '2')
		{
			accNum = "";
			toTrans = 0.0;
			errorFlag = false;
			continue;
		}
		else if(c == '3')
		{
			currentMenu = '2';
			return;
		}
		else errorFlag = true;
	}
	cls;
	//check account number if available and not blocked
	//check details of transfer fund
	//Are you sure you want to transfer to name
	
	for(int i = 0; i < record.size(); i++)
	{
		if(record[i].acNum == accNum)
		{
			bc = &record[i];
			break;
		}
	}
	
	if(bc == NULL || toTrans > record[currentAC].balance || bc->state == "blocked")
	{
		if(bc == NULL) info = {"Invalid Account Number"};
		else if(toTrans > record[currentAC].balance) info = {"You have insufficient balance","to transfer "+ twodec(toTrans)};
		else info = {"Account Number is Blocked"};
		
		c = displayMenu("Failed to Transfer Fund",info,vector<string>{"Transfer again","Another Transaction","Exit"});
		if(c == '1') fundTransfer();
		else if(c == '2')currentMenu = '2';
		else logout();
	}
	else
	{
		record[currentAC].balance -= toTrans;
		bc->balance += toTrans;
		info = {"You have transfered",twodec(toTrans) + " to " + accNum,"Your Current Balance is",twodec(record[currentAC].balance),"The Current Balance of the","Account # is " + twodec(bc->balance),""};
		c = displayMenu("Successfully Transfered Fund",info,vector<string>{"Transfer again","Another Transaction","Exit"});
		if(c == '1') fundTransfer();
		else if(c == '2') currentMenu = '2';
		else logout();
	}
}

/*====================================================*/

int main() 
{
    char mainMenuCh, bankCh, atmCh;
    
    //load data filestream
	loadData();
	
    //Main Loop
    while(true)
    {
        if(currentMenu == ' ') 
			mainMenuCh = displayMainMenu();
		else
			mainMenuCh = currentMenu;
        
        //Bank Menu
        if(mainMenuCh == '1')
        {
            bankCh = displayBank();
            if(bankCh == '1') 
                newAccount();
            else if(bankCh == '2')
                updateAccount();
            else if(bankCh == '3')
                deleteAccount();
            else if(bankCh == '4')
                unblockAccount();
            else
			{
				currentMenu = ' ';
                continue;
            }
            saveData();
        }
        //ATM Menu
        else if(mainMenuCh == '2')
        {
            atmCh = displayATM();
            if(atmCh == '1') 
                balanceInquiry();
            else if(atmCh == '2')
                withdraw();
            else if(atmCh == '3')
                deposit();
            else if(atmCh == '4')
                changePin(false);
            else if(atmCh == '5')
                loadMobile();
            else if(atmCh == '6')
                fundTransfer();
 			else
			{
				logout();
                continue;
            }
            saveData();
        }
        //EXIT
        else
        {
            cout << "Thank you for using our program!\n";
            break;
        }
    }
    return 0;
}
