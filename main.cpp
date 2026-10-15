#include <bits/stdc++.h>
using namespace std;
struct Contact{
    int id;
    string name;
    string phone;
    string email;
    string address;
};
struct Action{
    string type;
    Contact contact;
};
// check valid phone numbers
bool isValidPhone(const string& phone){
    if(phone.length()!=10){
        return false;
    }
    for(char c: phone ){
        if(!isdigit(c)){
            return false;
        }
    }
    return true;
}
//id validation
bool idExists(const vector<Contact>& contacts,int id){
    for(const auto& contact : contacts){
        if(contact.id==id){
            return true;
        }
    }
    return false;
}
//Function to store phone  numbers in index
void rebuildPhoneIndex( const vector<Contact>& contacts,unordered_map<string,int>& phoneIndex){
     phoneIndex.clear();
     for(int i=0;i<contacts.size();i++){
        phoneIndex[contacts[i].phone]=i;
     }
    }
//Function to save contacts in separate storage
void saveContacts(const vector<Contact>& contacts){
    ofstream file("contact.txt");
    for(const auto& contact:contacts){
        file<<contact.id<<"|"
            <<contact.name<<"|"
            <<contact.phone<<"|"
            <<contact.email<<"|"
            <<contact.address<<"\n";
    }
    file.close();
}
void loadContacts(vector<Contact>& contacts,unordered_map<string,int>& phoneIndex){
    ifstream file("contact.txt");
    if(!file){
        return;
    }
    string line;
    while(getline(file,line)){
        if(line.empty()){
            continue;
        }
        stringstream ss(line);
        string id,name,phone,email,address;
        getline(ss,id,'|');
        getline(ss,name,'|');
        getline(ss,phone,'|');
        getline(ss,email,'|');
        getline(ss,address,'|');

        Contact contact;

        contact.id=stoi(id);
        contact.name=name;
        contact.phone=phone;
        contact.email=email;
        contact.address=address;

        contacts.push_back(contact);
    }
    file.close();
    rebuildPhoneIndex(contacts,phoneIndex);
}
//Function to Add contacts
void addContact(vector<Contact>& contacts,unordered_map<string,int>& phoneIndex,stack<Action>& undoStack){
    Contact contact;
    cout<<"Enter ID:";
    cin>>contact.id;
    
    if(cin.fail()){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>:: max(),'\n');

        cout<<"Invalid ID ! Please Enter a Number.\n";
        return;
    }
    if(idExists(contacts,contact.id)){
        cout<<"ID already Exists !\n";
        return;
    }
    cin.ignore(numeric_limits<streamsize>:: max(),'\n');

    cout<<"Enter Name:";
    getline(cin,contact.name);
    if(contact.name.empty()){
        cout<<"Name Cannot be empty !\n";
        return;
    }

    cout<<"Enter Phone:";
    getline(cin,contact.phone);
    if(!isValidPhone(contact.phone)){
        cout<<"Invalid Phone Number ! Enter 10 digits Exactly \n";
        return ;
    }
    
    if(phoneIndex.find(contact.phone)!=phoneIndex.end()){
        cout<<"Phone Number Already Exists ! \n";
        return;
    }


    cout<<"Enter Email:";
    getline(cin,contact.email);
    if(contact.email.empty()){
        cout<<"Email cannot be empty! \n";
        return ;
    }


    cout<<"Enter Address:";
    getline(cin,contact.address);
    
    if(contact.address.empty()){
        cout<<"Address cannot be empty !\n";
        return ;
    }

    contacts.push_back(contact);
    phoneIndex[contact.phone]=contacts.size()-1;

    //action for undo
    undoStack.push({"ADD",contact});
    saveContacts(contacts);

    cout<<"Contact added successfully! \n";
}
  //Function to Display Contact
  void displayContacts(const vector<Contact>& contacts){
  for(auto contact:contacts){
        cout << "\nID: "<<contact.id<<endl;
        cout << "Name: "<<contact.name<<endl;
        cout << "Phone: "<<contact.phone<<endl;
        cout << "Email: "<<contact.email<<endl;
        cout << "Address: "<<contact.address<<endl;
    }
  }   
  //print contacts
  void printContact(const Contact& contact){
    cout<<"\nID:"<<contact.id<<endl;
    cout<<"Name:"<<contact.name<<endl;
    cout<<"Phone:"<<contact.phone<<endl;
    cout<<"Email:"<<contact.email<<endl;
    cout<<"Address:"<<contact.address<<endl;
  }
  //Function to search Contact by id
  void searchById(const vector<Contact>& contacts, int id){
    bool found=false;
   for(const auto& contact: contacts){
    if(contact.id==id){
        printContact(contact);
        found=true;
        break;
     }    
    }
    if(!found){
        cout<<"Contact you are searching is not available !"<<endl;
    }

   }
   //function to search by Name
   void searchByName(const vector<Contact>& contacts,string name){
    bool found=false;
    for( const auto& contact: contacts){
        if(contact.name==name){
            printContact(contact);
            found=true;
            
        }
    }
    if(!found){
        cout<<"Name Not Found in Contacts"<<endl;
    }

   }
//Function to search by phone number
void searchByPhone(const vector<Contact>& contacts,
    unordered_map<string,int>& phoneIndex,const string& phone){
    auto it=phoneIndex.find(phone);
    
    if(it!=phoneIndex.end()){
       printContact(contacts[it->second]);
       cout<<" Contact Found !"<<endl;
    }else{
       cout<<"Contact Not Found !"<<endl;
    }  
}
//Function to update contact
void updateContact(vector<Contact>& contacts,
    unordered_map<string,int>& phoneIndex,int id,
stack<Action>& undoStack){
bool found=false;
for(int i=0;i<contacts.size();i++){
    if(contacts[i].id==id ){   
        Contact oldContact=contacts[i];

    cout<<"Enter Name:";
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
    getline(cin,contacts[i].name);

    cout<<"Enter Phone:";
    string oldphone=contacts[i].phone;
    getline(cin,contacts[i].phone);
    if(!isValidPhone(contacts[i].phone)){
        cout<<"Invalid Phone Number ! Enter Exactly 10 Digits.\n";
        contacts[i].phone = oldphone;
        return;
    }
    if(phoneIndex.find(contacts[i].phone)!=phoneIndex.end() && contacts[i].phone!=oldphone){
        cout<<"Phone Number Already Exists ! \n";
        contacts[i].phone=oldphone;
        return;
    }

    cout<<"Enter Email:";
    getline(cin,contacts[i].email);


    cout<<"Enter Address:";
    getline(cin,contacts[i].address);

    undoStack.push({"UPDATE",oldContact});
    saveContacts(contacts);
    cout<<"Contact updated successfully! \n";
    found=true;
    break;
    }
}
if(!found){
    cout<<"Contact Not Found !"<<endl;
}
}


//function to delete the contacts
void deleteContact(vector<Contact>& contacts,unordered_map<string,int>& phoneIndex,int id,
stack<Action>& undoStack){
    bool found=false;
    for(int i=0;i<contacts.size();i++){
        if(contacts[i].id==id){
            //save before deleting the contact
            Contact deletedContact=contacts[i];
            //record the operation
            undoStack.push({"DELETE",deletedContact});
            

            phoneIndex.erase(contacts[i].phone);
            contacts.erase(contacts.begin()+i);
            saveContacts(contacts);

            rebuildPhoneIndex(contacts,phoneIndex);
            cout<<"Contact deleted successfully !"<<endl;
            found=true;
            break;
        }
    }
    if(!found){ 
        cout<<"Contact Not Found !"<<endl;
    }
}
//sorting
void sortByName(vector<Contact>& contacts){
    sort(contacts.begin(),contacts.end(),[](const Contact& a,const Contact& b){
     return a.name<b.name;
    });
}
void sortById(vector<Contact>& contacts){
    sort(contacts.begin(),contacts.end(),[](const Contact& a,const Contact& b){
     return a.id<b.id;
    });
}
void sortByPhone(vector<Contact>& contacts){
    sort(contacts.begin(),contacts.end(),[](const Contact& a,const Contact& b){
     return a.phone<b.phone;
    });
}

//Undo Last Action
void undoLastAction(
    vector<Contact>& contacts,
    unordered_map<string,int>& phoneIndex,
    stack<Action>& undoStack
){
    if(undoStack.empty()){
        cout<<"Noting to undo ! \n";
        return;
    }
    Action lastAction = undoStack.top();
    undoStack.pop();
    saveContacts(contacts);
    
    if(lastAction.type=="ADD"){
        int id=lastAction.contact.id;
        for(int i=0;i<contacts.size();i++){
            if(contacts[i].id==id){
                phoneIndex.erase(contacts[i].phone);
                contacts.erase(contacts.begin()+i);

                rebuildPhoneIndex(contacts,phoneIndex);
                cout<<"Last ADD operation undone ! \n";
                return ;
            }
        }
    }else if(lastAction.type=="DELETE"){
        Contact deletedContact=lastAction.contact;
        contacts.push_back(deletedContact);
        rebuildPhoneIndex(contacts,phoneIndex);
        cout<<"Last DELETE Operation Undone ! \n";
    }else if(lastAction.type=="UPDATE"){
        Contact oldContact=lastAction.contact;
        for(int i=0;i<contacts.size();i++){
            if(contacts[i].id==oldContact.id){
                contacts[i]=oldContact;
                rebuildPhoneIndex(contacts,phoneIndex);
                cout<<"Last UPDATE operation undone !\n";
                return;
            }
        }
        cout<<"Contacts not Found.Cannot undo Update! \n";

    }
}


  
  

int main(){
    vector<Contact> contacts;
    stack<Action> undoStack;
    unordered_map<string,int> phoneIndex;
    loadContacts(contacts,phoneIndex);
    int id;
    string name;
    string phone;

     while(true){
        cout<<"\n===========================\n";
        cout<<"       CONTACT MANAGER \n";
        cout<<"\n===========================\n";
        cout<<"1. ADD CONTACT\n";
        cout<<"2. Display Contact\n";
        cout<<"3. Search By ID\n";
        cout<<"4. Search By Name\n";
        cout<<"5. Search By Phone\n";
        cout<<"6. Update Contact\n";
        cout<<"7. Delete Contact\n";
        cout<<"8. Sort Contacts\n";
        cout<<"9.Undo Last Operation\n";
        cout<<"10.Exit\n";
        cout<<"Enter Your Choice: ";
        int choice;
        cin>>choice;
        if(cin.fail()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');

            cout<<"Invalid Choice ! Please Enter A Valid Number.\n";
            continue;
        }
        switch(choice){
            case 1:
            addContact(contacts,phoneIndex,undoStack);
            break;
            case 2:
            displayContacts(contacts);
            break;
            case 3:
            cout<<"\n Enter Id to Search:";
            cin>>id;
            searchById(contacts,id);
            break;
            case 4:
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cout<<"\n Enter Name to Search:";
            getline(cin,name);
            searchByName(contacts,name);
            break;
            case 5:
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cout<<"\n Enter Phone To Search: ";
            getline(cin,phone);
            searchByPhone(contacts,phoneIndex,phone);
            break;
            case 6:
            cout<<"\nEnter ID to update: ";
            cin>>id;
            updateContact(contacts,phoneIndex,id,undoStack);
            break;
            case 7:
            cout<<"\n Enter ID to Delete: ";
            cin>>id; 
            deleteContact(contacts,phoneIndex,id,undoStack);
            break;
            case 8:
            {
                int sortChoice;

                cout<<"\n===========SORT CONTACTS ===========\n";
                cout<<"1.Sort By Name\n";
                cout<<"2.Sort By ID\n";
                cout<<"3.Sort By Phone\n";
                cout<<"4.Back\n";
                cout<<"Enter your Choice: ";

                cin>>sortChoice;

                switch(sortChoice){
                case 1:
                     sortByName(contacts);
                     cout<<"\n Contacts sorted by name successfully! \n";
                     break;
                case 2:
                     sortById(contacts);
                     cout<<"\n Contacts sorted by ID successfully! \n";
                     break;
                case 3:
                     sortByPhone(contacts);
                     cout<<"\n Contacts sorted by Phone successfully! \n";
                     break;
                case 4:
                     break;

                default:
                cout<<"\n Invalid sorting Choice !\n";
                }
                break;
            }
            case 9:
            undoLastAction(contacts,phoneIndex,undoStack);
            break;
            case 10:
            saveContacts(contacts);
            cout<<"Contacts saved Successfully ! \n";
            cout<<"Exiting Contact Manager...\n";
            return 0;

        }
     }
   /* //1.ADD
    addContact(contacts,phoneIndex);
    addContact(contacts,phoneIndex);
    addContact(contacts,phoneIndex);
    //2.Display
    displayContacts(contacts);

   //3.Search by Id
    cout<<"\n Enter the id you want to search:";
    cin>>id;
    searchById(contacts,id);
    //4.Search by Name
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
    cout<<"\nEnter name to search:";
    getline(cin,name);
    searchByName(contacts,name);

     //5.Search By Phone
    
    cout<<"\n Enter the Number you want to search:";
    getline(cin,phone);
    searchByPhone(contacts,phoneIndex,phone);

    //6.Update
    cout<<"\n Enter ID to Update:";
    cin>>id;
    updateContact(contacts,phoneIndex,id);

    //7.Display after updating
     displayContacts(contacts);


  
     //8.DELETE
    cout<<"\n Enter the id you want to delete:";
    cin>>id;
    deleteContact(contacts,phoneIndex,id);


    displayContacts(contacts);*/
    
    return 0;
}