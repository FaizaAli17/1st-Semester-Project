#include<iostream>
#include<fstream>
#include<iomanip>
#include<stdio.h>
#include<string.h> //allows us to store textual data of characters 
#include<conio.h> // we are using conio.h for console input/output
#include <windows.h>  // For Windows Console Color
using namespace std;

// declaring class 
class Items;
class Bill;
class billDetails;

// functions
void addNewItem(); // this function is to add items to the item data file
void showMenu(); // this function is to read data from file and display as items on menu
float getPriceFromFile(int); // this function is for getting the price from a data file
char *getNameFromFile(int); // to get name of product from data file
int generateBillNo(); // is for generating bill
void headingOrderDetail(); // is for providing heading to order items
void getOrderDetails(int); // is for displaying order items on screen from item data file 
void placeOrder(); // this fucntion for placing order and display the order details

//Menu fuctions
void adminMenu(); //this is for displaying the menu options for admin
void guestMenu(); //this is for display menu optuons for guest
void security(); //this is for securing login for admin
void project(); //for displaying admin and guest menu options

//decor functions
void drawLine();
void drawStars();


// Function to set the console color to baby pink (RGB: 255, 182, 193) for Unix-like systems
void setBabyPinkColor()
{
    
    // Using the ANSI escape sequence for RGB colors
    cout << "\033[38;2;255;182;193m"; // RGB value for baby pink
}

// Function to reset the console color
void resetColor()
{
    cout << "\033[0m"; // Reset to default text color
}


void setWindowsBabyPinkColor()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}
//class & fucntion declarations
class Items
{
	protected: // this specifies that it cannot be accessed from ouside the class
	    int id;
    	char name[100];
	    float price;
	
	public: // this specfier means that members are accessible from outside the class
	
		int getId(){ return id;	}
		float getPrice(){ return price; }
		char *getName(){ return name;}
    	void addItem();
	    void showItem();
	    void listItems();
	    void deleteItem(int id);
	} obj_i;

class Bill	
{
	protected:
		int billNo;
		
	public:
		Bill(){billNo=0;}
		int getBillNo(){return billNo;}
		
		void setBillNo(int b)
		{
			billNo=b;
		}
}obj_b;

class billDetails:public Bill, public Items
{
	protected:
		int qty; //quantity 
		float amt; // amount
	public:
		void setOrder(int, int, char[], float, int);
        void listBillDetails();
        float getAmount () {return amt;	}
		
};

//DEFINING THE FUNCTIONS

//fucntion of items base class
void Items::addItem()
{ 
	cout<<"Add Item Details\n";
	cout<<"Items Id: ";
	cin>>id;
	cout<<"Item Name: ";
	cin>>name;
	cout<<"Item Price: ";
	cin>>price;
}

void Items::showItem()
{
	cout<<"Items Details\n";
	cout<<"Item Id     :"<<id<<endl;
	cout<<"Item Name   : "<<name<<endl;
	cout<<"Item Price  :"<<price<<endl;
	cout<<endl;
}

void Items::listItems()
{
	cout.setf(ios::left);
	cout<<setw(8)<<id<<setw(25)<<name<<setw(12)<<price<<endl;	
}
void Items::deleteItem(int itemId) {
    Items temp;
    ifstream fin("items.dat", ios::in | ios::binary);
    if (!fin) {
        cerr << "Error opening the file!" << endl;
        return;
    }

    ofstream fout("temp.dat", ios::out | ios::binary);
    bool found = false;

    while (fin.read((char*)&temp, sizeof(temp))) {
        if (temp.getId() == itemId) {
            found = true; // Item found, do not write to temp file
            cout << "Item with ID " << itemId << " deleted successfully." << endl;
        } else {
            fout.write((char*)&temp, sizeof(temp)); // Write to temp file
        }
    }

    fin.close();
    fout.close();

    // Remove the original file and rename temp file
    remove("items.dat");
    rename("temp.dat", "items.dat");

    if (!found) {
        cout << "Item with ID " << itemId << " not found." << endl;
    }
}


//Functions of billDetails Derived Class
void billDetails::setOrder(int b, int i, char nm[], float p, int q )
{
	billNo=b;
	id=i;
	strcpy(name,nm);
	price=p;
	qty=q;
	amt=price*qty;
	
}

//Displays the bill items in tabular form
void billDetails::listBillDetails()
{
	cout.setf(ios::left);
	cout<<setw(5)<<id
	<<setw(20)<<name
	<<setw(10)<<price
	<<setw(8)<<qty
	<<setw(12)<<amt
	<<endl;
	
}

//thsi is for adding items to the data file
void addNewItem()
{
	ofstream fout;
	fout.open("Items.dat",ios::app);
	if(!fout){
		cerr<<"Error openeing the file for writing" << endl;
		return ;
	}
	obj_i.addItem();
	fout.write((char*)&obj_i,sizeof(obj_i));
	fout.close();
	
}

// reads the data from file and displays as menu on screen
void showMenu()
{
	int count=0;
	ifstream fin;
	fin.open("items.dat",ios::in);
	if(!fin)
	{cerr<<"error opening the items file!"<<endl;
	return ;
	}
	
	while(fin.read((char*)&obj_i,sizeof(obj_i)))
	{
		if(count<1)
		{
		
		cout.setf(ios::left);
		cout<<"\n"<<setw(8)<<"Id"<<setw(25)
		<<"Product Name"<<setw(12)<<"Rate"<<"\n";
		}
		obj_i.listItems();
    	count++;
	}
	fin.close();
}

//function is for the price from data file 
float getPriceFromFile(int n)
{
	int id=n;
	float r=0;
	ifstream fin;
	fin.open("items.dat",ios::in);
	if(!fin){
	
		cerr<<"Error opening the ites file!"<<endl;
		return r;}
		
	while (fin.read((char*)&obj_i,sizeof(obj_i)))
	{
		if(id==obj_i.getId())
		{
			r=obj_i.getPrice();
		}
	}
	fin.close();
	return r;
}

//this function is for the item name from data filr.
char *getNameFromFile(int n)
{
    int id = n;
    static char nm[20] = "none";
    ifstream fin("items.dat", ios::in);
    if (!fin) {
        return nm; 
    }
    while (fin.read((char*)&obj_i, sizeof(obj_i))) {
        if (n == obj_i.getId()) {
            strcpy(nm, obj_i.getName());
        }
    }
    fin.close();
    return nm;
}


//this function is for generating unique bill number
int generateBillNo()
{
	
	Bill b;
	int n=0;
	ifstream fin("bill.dat",ios::in|ios::binary);
	while (fin.read((char*)&b,sizeof(b)))
	{
		n=b.getBillNo();
	}
	fin.close();
	
	//for generating automatic account number
	if(n==0) n=1;
	else n+=1;
	
	// for writing bill number in the data file as unique id...
	b.setBillNo(n);
	ofstream fout("bill.dat",ios::out|ios::app|ios::binary);
if(!fout){
	cerr<< "Error opening the bill file for writing !"<<endl;
	return n;
}
	fout.write ((char*)&b,sizeof(b));
	fout.close();
	return n;
}

void headingOrderDetails()
{
	cout.setf(ios::left);
	cout<<setw(5)<<"No."
	<<setw(5)<<"ID"
	<<setw(20)<<"Product Name"
	<<setw(10)<<"Price"
	<<setw(8)<<"Qty."
	<<setw(12)<<"Amount"<<endl;
}

///place order
void placeOrder()
{
	    billDetails ord;
		
		int bn, n, qt;
		char itemName[20];
		float orderTotal=0;
		//automatic generated unique bill number
		bn=generateBillNo();
		//order data file
		ofstream fout("order.dat",ios::app);
		if(!fout){
			cerr<<"Error opening the order file for writing!"<<endl;
			return ;
		}
		
		do
		{
		 system ("cls");
		 cout<<"0. End Order and Exit\n";
		 showMenu();
		 cout<<"Enter item id: ";
		 cin>>n;
		 if(n==0)
		      break;
		 
		 cout<<"Order for ID: "<<n<<" : "<<getNameFromFile(n)<<" Rs."<<getPriceFromFile(n)<<endl;
		 cout<<"Enter Qty: ";
		 cin>>qt;
		 
		 
		 ord.setOrder(bn,n,getNameFromFile(n),getPriceFromFile(n),qt);
		 
		 //write the order to the data file
		 fout.write((char*)&ord,sizeof(ord));
	}
	while (n!=0);
	
	
		fout.close();
		cout<<"End Of Order file.....\n";
		
		//Read order details from order file and dispaly on screen
		headingOrderDetails();
		getOrderDetails(bn); 	
	 
}
 void getOrderDetails(int bn)
   {
       billDetails ord;
       float total = 0;
       int no = 1;
       ifstream fin;
       fin.open("order.dat"); 
       if (!fin) {
           cerr << "Error opening the order file!" << endl; 
           return;
       }
       while (fin.read((char*)&ord, sizeof(ord))) {
           if (bn == ord.getBillNo()) {
               cout.setf(ios::left);
               cout << setw(5) << no++;
               ord.listBillDetails();
               total += ord.getAmount();
           }
       }
       cout.setf(ios::left);
       cout << setw(40) << " "
            << setw(9) << "Total: "
            << setw(12) << total << endl;
}


void drawLine()
{
	for(int i=1;i<=50;i++)
	cout<<"=";
	cout<<endl;
}

void drawStars()
{
	for(int i=1;i<=50;i++)
	cout<<"*";
	cout<<endl;
}

void adminMenu()
{
	int ch;
	do{
		system("cls");
		drawLine();
		cout<<"\tADMIN MENU\n";
		drawLine();
		cout<<" 0. Logout\n";
		cout<<" 1. Add New Products\n";
		cout<<" 2. Show All Products\n ";
		cout<<"3.Remove Product From Menu\n";
		cin>>ch;
		switch(ch)
		{
			case 1: addNewItem(); 
			break;
			case 2: showMenu();
			break;
			case 3: {
				int id;
				cout<<"Enter the Item ID to delete: ";
				cin>>id;
				obj_i.deleteItem(id);
				break;
			}
			
		}
		system("pause");
		
	}while (ch!=0);
	
}

void guestMenu()
{
	int ch;
	do{
		system("cls");
		drawLine();
		cout<<"\tGUEST MENU\n";
		drawLine();
		cout<<"0. Back to main menu\n";
		cout<<"1. View Product Menu\n";
		cout<<"2. Place Order\n";
		cout<<"Enter your Choice: ";
		cin>>ch;
		switch(ch)
		{
			case 1: showMenu();
			break;
			case 2: placeOrder();
			break;
		}
		system("pause");
	}while(ch!=0);
}

void security()
{
	char pwd[]="password", un[]="admin";
	char cpwd[20], uname[20];
	char ch;
	int i=0, chance=1;
	
	retry:
		system("cls");
		drawLine();
		cout<<"\n\nEnter username: ";
		cin>>uname;
		
		//if the password matches
		if(strcmpi(un,uname)==0)
		{
			cout<<"\n\nEnter Password:";
			do{
				ch=_getch();
				if(ch!=13)
				{
					cpwd[i]=ch;
					i++;
					cout<<"*";
				}
				else
				{
					cpwd[i]='\0';
					i++;
				}
			}while(ch!=13);
			
			
			
			//if password matches...
			if(strcmp(pwd,cpwd)==0);
			{
				cout<<"\n\nWelcome"<<uname<<endl;
				system("pause");
				system("cls");
				
				adminMenu();
	     }
			{
				cout<<"\n\n";
				cout<<"Password does not match.....\n\n";
				cout<<chance<<"used of 3 chances.....\n";
				chance++;
			    	if (chance>3)
				{
					cout<<"\n\n";
					cout<<"Unauthorized access attempt\n\n";
					cout<<"Not an registered username...\n\n";
					system("pause");
					exit(0);
				}
				else
				{
					goto retry;
				}
			}
			system("pause");
			
		}

//if username does not match
else
{
	cout<<"\n\n";
	cout<<"Unauthorized access attempt\n\n";
	cout<<"not an registered username.....\n\n";
}
}


void project()
{
	int ch;
	do{
		system("cls");
		setBabyPinkColor();
		drawStars();
		cout<<"\t H O M E  P A G E\n";
		drawLine();
		cout<<"\n\n0.Exit\n\n";
		cout<<"1.Admin Login\n\n";
		cout<<"2.Guest Check in\n\n";
		cout<<"Enter Your Choice: ";
		cin>>ch;
		switch(ch)
		{
			case 1: security();
			break;
			case 2: guestMenu();
			break;
		}
	}while(ch!=0);
}

int main()
{
	project();
	setBabyPinkColor();
	resetColor();
	return 0;
	}
