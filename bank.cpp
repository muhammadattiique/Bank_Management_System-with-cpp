#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
using namespace std;

class Bank {
private:
    string c_first_name, c_last_name, c_cnic, c_phone, c_email;
public:
    void customer_info() {
        cout << "ENTER FIRST NAME --> ";
        cin >> c_first_name;
        cout << "ENTER LAST NAME --> ";
        cin >> c_last_name;
    cnic_again:
    
        cout << "ENTER CNIC(WITH DASHES) --> ";
        cin.ignore();
        getline(cin, c_cnic);
        if (c_cnic.length() != 15) {
            cout << "INVALID ! YOUR CNIC MUST CONTAINS DASHES " << endl;
            goto cnic_again;
        }
    phone_again:
        cout << "ENTER PHONE NO(03xx) --> ";
        cin >> c_phone;
        if (c_phone.length() != 11) {
            cout << "INVALID ! " << endl;
            goto phone_again;
        }
    }

    void display_info() {
        cout << "FIRST NAME  --> " << c_first_name << endl;
        cout << "LAST  NAME --> " << c_last_name << endl;
        cout << "CNIC --> " << c_cnic << endl;
        cout << "PHONE NO --> " << c_phone << endl;
    }

    string return_fname() {
        return c_first_name;
    }

    string return_lname() {
        return c_last_name;
    }

    string return_cnic() {
        return c_cnic;
    }
};

class Customer;

class Account : public Bank {
private:
    string acc_type;
    int acc_no;
    int amount;
    string password;
public:
    void cust_account() {
        Bank::customer_info();
        acc_no = generating_unique_acc_no();
        cout << "ENTER ACCOUNT TYPE --> ";
        cin >> acc_type;
        cout << "Enter Your Password --> ";
        cin >> password;
        cout << "Enter Initial Amount You Want To Deposit --> ";
        cin >> amount;
    }

    void cust_display() {
        Bank::display_info();
        cout << "ACCOUNT TYPE --> " << acc_type << endl;
        cout << "ACCOUNT NO --> " << acc_no << endl;
        cout << "PASSWORD IS --> " << password << endl;
        cout << "BALANCE IS --> " << amount << endl;
    }

    static int generating_unique_acc_no() {
        srand(time(0));
        int min_random_no = 10000;
        int max_random_no = 99999;
        int random_acc_no = rand() % (max_random_no - min_random_no + 1) + min_random_no;
        return random_acc_no;
    }

    int returnBalance() {
        return amount;
    }

    string return_password() {
        return password;
    }

    void setBalance(int new_balance) {
        amount = new_balance;
    }

    int return_account_no() {
        return acc_no;
    }

    void saveToFile(ofstream& outFile) {
        outFile.write(reinterpret_cast<char*>(this), sizeof(Account));
    }

    void loadFromFile(ifstream& inFile) {
        inFile.read(reinterpret_cast<char*>(this), sizeof(Account));
    }

    friend void deposit(Customer c1, Account& a1);
    friend void withdraw(Customer c1, Account& a1);
    friend void transfer(Account& sender, Account& receiver, int amount);
};

class Customer 
{
public:
    friend void deposit(Customer c1, Account& a1);
    friend void withdraw(Customer c1, Account& a1);
    friend void transfer(Account& sender, Account& receiver, int amount);
};

void deposit(Customer c1, Account& a1) {
    int deposit;
    cout << "ENTER AMOUNT YOU WANT TO DEPOSIT --> ";
    cin >> deposit;
    a1.amount += deposit;
    cout << deposit << " HAS BEEN DEPOSITED SUCCESSFULLY!" << endl;

    fstream file("BankingDetail.dat", ios::in | ios::out | ios::binary);
    Account temp;
    while (file.read(reinterpret_cast<char*>(&temp), sizeof(Account))) {
        if (temp.return_account_no() == a1.return_account_no()) {
            file.seekp(-static_cast<int>(sizeof(Account)), ios::cur);
            file.write(reinterpret_cast<char*>(&a1), sizeof(Account));
            break;
        }
    }
    file.close();
}

void withdraw(Customer c1, Account& a1) {
    int withdraw;
withdraw_again:
    cout << "ENTER AMOUNT YOU WANT TO WITHDRAW --> ";
    cin >> withdraw;
    if (a1.amount >= withdraw) {
        a1.amount -= withdraw;
        cout << withdraw << " HAS BEEN WITHDRAWN SUCCESSFULLY!" << endl;
    }
    else {
        cout << "INSUFFICIENT BALANCE!" << endl;
        goto withdraw_again;
    }

    fstream file("BankingDetail.dat", ios::in | ios::out | ios::binary);
    Account temp;
    while (file.read(reinterpret_cast<char*>(&temp), sizeof(Account))) {
        if (temp.return_account_no() == a1.return_account_no()) {
            file.seekp(-static_cast<int>(sizeof(Account)), ios::cur);
            file.write(reinterpret_cast<char*>(&a1), sizeof(Account));
            break;
        }
    }
    file.close();
}

void transfer(Account& sender, Account& receiver, int amount) {
    if (sender.amount >= amount) {
        sender.amount -= amount;
        receiver.amount += amount;
        cout << amount << " HAS BEEN TRANSFERRED SUCCESSFULLY!" << endl;

        fstream file("BankingDetail.dat", ios::in | ios::out | ios::binary);
        Account temp;
        bool sender_updated = false;
        bool receiver_updated = false;
        while (file.read(reinterpret_cast<char*>(&temp), sizeof(Account))) {
            if (temp.return_account_no() == sender.return_account_no()) {
                file.seekp(-static_cast<int>(sizeof(Account)), ios::cur);
                file.write(reinterpret_cast<char*>(&sender), sizeof(Account));
                sender_updated = true;
            }
            else if (temp.return_account_no() == receiver.return_account_no()) {
                file.seekp(-static_cast<int>(sizeof(Account)), ios::cur);
                file.write(reinterpret_cast<char*>(&receiver), sizeof(Account));
                receiver_updated = true;
            }
            if (sender_updated && receiver_updated) break;
        }
        file.close();
    }
    else {
        cout << "INSUFFICIENT BALANCE FOR TRANSFER!" << endl;
    }
}

void starline() {
    for (int i = 0; i < 120; i++) {
        cout << "*";
    }
    cout << endl;
}

int main() {
    string username, password;
    int count = 0;
    Account a1;
    Customer c1;
top:
    system("cls");
    starline();
    cout << "\t\t\t\t\t" << "WELCOME TO ARID NATIONAL BANK" << "\t\t\t\t" << endl;
    starline();
    cout << "PRESS 1 FOR ADMIN LOGIN" << endl;
    cout << "PRESS 2 FOR CUSTOMER" << endl;
    cout << "PRESS 0 TO EXIT" << endl;
    int choice, sub_choice, choice2;
    bool flag = 0;
    cout << "\nEnter Your Choice --> ";
    cin >> choice;
    switch (choice) {
    case 0:
    {
        exit(0);
        break;
    }
    case 1: {
    admin_again:
        system("cls");
        starline();
        cout << "\t\t\t\t\tADMIN LOGIN PAGE" << endl;
        starline();
        cout << "\n\nENTER USERNAME --> ";
        cin >> username;
        cout << "ENTER PASSWORD --> ";
        cin >> password;
        if (((username == "arid") && (password == "arid123")) || ((username == "ARID") && (password == "ARID123"))) {
        admin_panel_again:
            system("cls");
            starline();
            cout << "\t\t\t\t\t\tWELCOME TO ADMIN PANEL" << endl;
            starline();
            cout << "\n\nPRESS 1 TO DISPLAY DATA OF ACCOUNT HOLDERS" << endl;
            cout << "PRESS 2 TO DELETE DATA OF ACCOUNT HOLDERS" << endl;
            cout << "PRESS 3 TO UPDATE DATA OF ACCOUNT HOLDERS" << endl;
            cout << "PRESS 4 TO SEARCH DATA OF ACCOUNT HOLDERS" << endl;
            cout << "PRESS 0 TO LOG OUT" << endl;
            cout << "ENTER YOUR CHOICE --> ";
            cin >> choice2;
            switch (choice2) {
            case 0:
                goto top;
            case 1: {
                system("cls");
                starline();
                cout << "\t\t\t\t\tAll ACCOUNT HOLDERS DATA" << endl;
                starline();
                ifstream f1("BankingDetail.dat", ios::binary);
                if (!f1) {
                    cout << "Error opening file for reading." << endl;
                    goto top;
                }
                cout << "Reading Data........" << endl;
                starline();
                while (f1.read(reinterpret_cast<char*>(&a1), sizeof(Account))) {
                    a1.cust_display();
                    starline();
                }
                f1.close();
                system("pause");
                goto admin_panel_again;
                break;
            }
            case 2: {
                system("cls");
                starline();
                cout << "\t\t\t\t\tDELETE ACCOUNT HOLDERS DATA" << endl;
                starline();
                ofstream write("temp_file.dat", ios::app | ios::binary);
                ifstream f1("BankingDetail.dat", ios::binary);
                if (!f1) {
                    cout << "Error opening file for reading." << endl;
                    goto top;
                }
                string cnic;
                cout << "ENTER CNIC --> ";
                cin >> cnic;
                while (f1.read(reinterpret_cast<char*>(&a1), sizeof(Account))) {
                    if (cnic != a1.return_cnic()) {
                        write.write(reinterpret_cast<char*>(&a1), sizeof(Account));
                    }
                    else
                    {
                        flag = 1;
                    }
                }
                if (!flag) {
                    cout << "CNIC NOT FOUND!" << endl;
                }
                else {
                    cout << "Account with CNIC " << cnic << " has been deleted!" << endl;
                }
                f1.close();
                write.close();
                remove("BankingDetail.dat");
                rename("temp_file.dat", "BankingDetail.dat");
                system("pause");
                goto admin_panel_again;
                break;
            }
            case 3: {
                system("cls");
                starline();
                cout << "\t\t\t\t\t\tUPDATE DATA" << endl;
                starline();
                ofstream write("temp_file.dat", ios::app | ios::binary);
                ifstream read("BankingDetail.dat", ios::binary);
                if (!read) {
                    cout << "Error opening file for reading." << endl;
                    goto top;
                }
                string new_cnic;
                cout << "ENTER CNIC OF PERSON YOU WANT TO UPDATE --> ";
                cin >> new_cnic;
                while (read.read(reinterpret_cast<char*>(&a1), sizeof(Account))) {
                    if (new_cnic == a1.return_cnic()) {
                        cout << "ENTER NEW DETAILS OF PERSON" << endl;
                        a1.customer_info();
                        flag = 1;
                    }
                    write.write(reinterpret_cast<char*>(&a1), sizeof(Account));
                }
                if (flag == 1) {
                    cout << "Account with CNIC " << new_cnic << " has been updated!" << endl;
                }
                else {
                    cout << "CNIC NOT FOUND!" << endl;
                }
                read.close();
                write.close();
                remove("BankingDetail.dat");
                rename("temp_file.dat", "BankingDetail.dat");
                system("pause");
                goto admin_panel_again;
                break;
            }
            case 4: {
                system("cls");
                starline();
                cout << "\t\t\t\t\t\tSEARCH DATA" << endl;
                starline();
                ifstream read("BankingDetail.dat", ios::binary);
                if (!read) {
                    cout << "Error opening file for reading." << endl;
                    goto top;
                }
                string search_cnic;
                cout << "ENTER CNIC OF PERSON YOU WANT TO RETRIEVE --> ";
                cin >> search_cnic;
                while (read.read(reinterpret_cast<char*>(&a1), sizeof(Account))) {
                    if (search_cnic == a1.return_cnic()) {
                        a1.display_info();
                        flag = 1;
                    }

                }
                if (!flag)
                {
                    cout << "\t\tNo Result Found!" << endl;
                }
                read.close();
                system("pause");
                goto admin_panel_again;
                break;
            }
            default:
                cout << "INVALID CHOICE!" << endl;
                system("pause");
                goto admin_panel_again;
            }
            break;
           
        }
        else {
            cout << "INVALID CREDENTIALS!" << endl;
            count++;
            cout << "YOU HAVE TOTAL " << 3 - count << " CHANCES LEFT" << endl;
            system("pause");
            if (count == 3) {
                cout << "SORRY! YOU HAVE USED ALL YOUR CHANCES" << endl;
                system("pause");
                goto top;
            }
            goto admin_again;
        }
        break;
    }
    case 2: {
        system("cls");
        starline();
        cout << "\t\t\t\t\t\tWELCOME TO CUSTOMER PANEL" << endl;
        starline();
        cout << "\nPress 1 for Customer Login" << endl;
        cout << "Press 2 for Customer Signup" << endl;
        cout << "Press 3 to See Your Account No" << endl;
        cout << "Press 0 to go Back " << endl;
        cout << "\nEnter your CHOICE --> ";
        cin >> sub_choice;
        switch (sub_choice) {
        case 0: {
            goto top;
            break;
        }
        case 1:
        {
        acc_no_again:
            system("cls");
            starline();
            cout << "\t\t\t\t\t\tCUSTOMER LOGIN PAGE" << endl;
            starline();
            ifstream read("BankingDetail.dat", ios::binary);
            if (!read) {
                cout << "Error opening file for reading." << endl;
                return 1;
            }
            int id;
            cout << "\n\nEnter Your Account Number --> ";
            cin >> id;
            string pass;
            cout << "Enter Your Password --> ";
            cin >> pass;
            bool found = false;
            while (read.read(reinterpret_cast<char*>(&a1), sizeof(Account)))
            {
                if (id == a1.return_account_no() && pass == a1.return_password()) {
                customer_again:
                    system("cls");
                    starline();
                    cout << "\t\t\t\t\t\tWELCOME " << a1.return_fname() << " " << a1.return_lname() << endl;
                    starline();
                    found = true;
                    cout << "\nPress 1 to See Balance" << endl;
                    cout << "Press 2 to Deposit Amount" << endl;
                    cout << "Press 3 to Withdraw Amount" << endl;
                    cout << "Press 4 to Transfer Amount" << endl;
                    cout << "Press 0 to Log Out" << endl;
                    int choice3;
                    cout << "\nENTER YOUR CHOICE --> ";
                    cin >> choice3;
                    switch (choice3) {
                    case 0:
                        goto top;
                    case 1: {
                        system("cls");
                        starline();
                        cout << "\t\t\t\t\t\tBALANCE PAGE" << endl;
                        starline();
                        ifstream read("BankingDetail.dat", ios::binary);
                        if (!read) {
                            cout << "Error opening file for reading." << endl;
                            return 1;
                        }
                        Account temp;
                        bool found = false;
                        while (read.read(reinterpret_cast<char*>(&temp), sizeof(Account))) {
                            if (temp.return_account_no() == a1.return_account_no()) {
                                cout << "Your Balance is " << temp.returnBalance() << " Rs" << endl;
                                found = true;
                                break;
                            }
                        }
                        if (!found) {
                            cout << "Account not found!" << endl;
                        }

                        read.close();
                        system("pause");
                        goto customer_again;
                        break;
                    }
                    case 2:
                        system("cls");
                        starline();
                        cout << "\t\t\t\t\t\tDEPOSIT PAGE" << endl;
                        starline();
                        deposit(c1, a1);
                        system("pause");
                        goto customer_again;
                        break;
                    case 3:
                        system("cls");
                        starline();
                        cout << "\t\t\t\t\t\tWITHDRAW PAGE" << endl;
                        starline();
                        withdraw(c1, a1);
                        system("pause");
                        goto customer_again;
                        break;
                    case 4: {
                        system("cls");
                        starline();
                        cout << "\t\t\t\t\t\tTRANSFER PAGE" << endl;
                        starline();
                        int receiver_acc_no, transfer_amount;
                        cout << "Enter receiver's account number: ";
                        cin >> receiver_acc_no;
                        cout << "Enter amount to transfer: ";
                        cin >> transfer_amount;

                        ifstream read("BankingDetail.dat", ios::binary);
                        if (!read) {
                            cout << "Error opening file for reading." << endl;
                            return 1;
                        }
                        Account receiver;
                        bool receiver_found = false;
                        while (read.read(reinterpret_cast<char*>(&receiver), sizeof(Account))) {
                            if (receiver.return_account_no() == receiver_acc_no) {
                                receiver_found = true;
                                break;
                            }
                        }
                        read.close();

                        if (receiver_found) {
                            transfer(a1, receiver, transfer_amount);
                        }
                        else {
                            cout << "Receiver account not found!" << endl;
                        }
                        system("pause");
                        goto customer_again;
                        break;
                    }
                    default:
                        cout << "INVALID CHOICE!" << endl;
                        system("pause");
                        goto customer_again;
                    }
                    break;

                }
            }
            if (!found) {
                cout << "INVALID CREDENTIALS!" << endl;
                count++;
                cout << "YOU HAVE TOTAL " << 3 - count << " CHANCES LEFT" << endl;
                system("pause");
                if (count == 3) {
                    system("pause");
                    goto top;
                }
                goto acc_no_again;
            }
            read.close();
            break;
        }
        case 2: {
            system("cls");
            starline();
            cout << "\t\t\t\t\t\tCUSTOMER SIGNUP PAGE" << endl;
            starline();
            ofstream write("BankingDetail.dat", ios::app | ios::binary);
            if (!write) {
                cout << "Error opening file for writing." << endl;
                return 1;
            }
            a1.cust_account();
            cout << "\nYOUR ACCOUNT NUMBER IS --> " << a1.return_account_no() << endl;
            write.write(reinterpret_cast<char*>(&a1), sizeof(Account));
            write.close();
            system("pause");
            goto top;
        }
        case 3: {
            system("cls");
            starline();
            cout << "\t\t\t\t\t\tSEARCH ACCOUNT NUMBER" << endl;
            starline();
            string cnic;
            cout << "Enter Your CNIC --> ";
            cin >> cnic;
            ifstream read("BankingDetail.dat", ios::binary);
            if (!read) {
                cout << "Error opening file for reading." << endl;
                return 1;
            }
            bool found = false;
            while (read.read(reinterpret_cast<char*>(&a1), sizeof(Account))) {
                if (cnic == a1.return_cnic()) {
                    cout << "Your Account Number is " << a1.return_account_no() << endl;
                    found = true;
                }
            }
            if (!found) {
                cout << "CNIC NOT FOUND!" << endl;
            }
            read.close();
            system("pause");
            goto top;
            break;
        }
        default: {
            cout << "INVALID CHOICE!" << endl;
            cout << "WANT TO PERFORM ANYTHING NEW (Y/N)? --> ";
            char ch;
            cin >> ch;
            if (ch == 'y' || ch == 'Y') {
                goto top;
            }
            else {
                cout << "THANKS FOR VISITING!" << endl;
                return 1;
            }
        }
        }
        break;
    }
    default:
        cout << "INVALID CHOICE!" << endl;
        cout << "WANT TO PERFORM ANYTHING NEW (Y/N)? --> ";
        char ch;
        cin >> ch;
        if (ch == 'y' || ch == 'Y') {
            goto top;
        }
        else {
            cout << "THANKS FOR VISITING!" << endl;
            return 1;
        }
    }
    return 0;
}