#include <iostream>
#include <stdexcept>

class Account {     // the clients
	float balance;
	std::string name, password;
	public:
		Account (std::string n="empty", std::string pass="empty") {
			name=n;
			password=pass;
			balance=0;
		}
		std::string getName () const {
			return name;
		}
		std::string getPassword () const {
			return password;
		}
		float getBalance () const {
			return balance;
		}
		bool isEmpty () {
			if (name=="empty" or password=="empty") return true;
			return false;
		}
		void setName (std::string new_name) {
			name=new_name;
		}
		void setPassword (std::string new_pass) {
			password=new_pass;
		}
		void add (float sum) {
			balance+=sum;
		}
		void withdraw (float sum)  throw(std::logic_error) {
			if (sum >balance) throw std::logic_error("Not enough money left!");
			balance-=sum;
		}		
};

class Bank {      // the main system, able to add new clients, make transactions, etc.
	    Account *clients;
	    int size, cur_num;
	    bool end;
	public:
		Bank (int n) {    // initializing the bank class, giving the size of it
			size=n;
			clients = new Account [size];
			cur_num=0;
			end=false;
		}
		~Bank () {
			delete [] clients;
			std::cout << "Shutting down bank system...\n";
		}
		void addClient (std::string name, std::string password) throw(std::logic_error) {
			if (cur_num>=size) throw std::logic_error("Not enough space!");
			clients[cur_num].setName(name);
			clients[cur_num].setPassword(password);
			cur_num++;
		}
		int found (std::string name) {
			for (int i=0; i<cur_num; i++) {
				if (clients[i].getName()==name) return i;
			}
			return -1;
		}
		void show (std::string name) {
			if (found(name) > -1) {
			 std::cout << "Name: " << clients[found(name)].getName() << "\n";
			 std::cout << "Balance: " << clients[found(name)].getBalance() << "$\n";
	        }
	        else std::cout << "Account was not found!\n";
		}
		void show_all () {
			for (int i=0; i<cur_num; i++) {
				show(clients[i].getName());
				std::cout <<"--------------------------------------------------\n";
			}
		}
		void exit () {
			end=true;
		}
		bool login (std::string name, std::string pass) {     // checks if given name and password belong to a registered client
			if (found(name) > -1 and clients[found(name)].getPassword() == pass) return true;
			return false; 
		}
		void New_account () {    // adding a new client
			std::string name, password;
			std::cout << "Username: ";
			std::cin >> name;
			std::cout << "Password: ";
			std::cin >> password;
			try {
				addClient(name, password);
				std::cout << "You have been successfully registered!\n";
			} catch (std::logic_error) {
				std::cout << "Something went wrong! We are sorry for the inconvenience.\n";
			}
		}
		void View_account () {    // showing info about a specific client
			std::string name;
			std::cout << "Username: ";
			std::cin >> name;
			show(name);
		}
		void Transaction () {
			std::string name, password, receiver;
			float sum;
			std::cout << "Username: ";
			std::cin >> name;
			std::cout << "Password: ";
			std::cin >> password;
			if (!login(name, password)) std::cout << "Wrong combination of password and username!\n";
			else {
				std::cout <<"Give the name of the account you want to send money: ";
				std::cin >> receiver;
				if (found(receiver) == -1) {
					std::cout << "Account was not found!\n";
					return;
				}
				std::cout << "Give the sum you want to transfer: ";
				std::cin >> sum;
		      try {
	               clients[found(name)].withdraw(sum);
	               clients[found(receiver)].add(sum);
	               std::cout << "Transaction was completed successfully!\n";
	           } catch (std::logic_error) {
	               std::cout << "You don't have enough money for this action!\n";
			   }
		   }
		}
		void Addmoney () {    // adding funds to a account
			std::string name;
			float sum;
			std::cout << "Name: ";
			std::cin >> name;
			std::cout << "Give the sum you want to add to your balance: ";
			std::cin >> sum;
			if (found(name) == -1) std::cout << "This account was not found!\n";
			else {
				clients[found(name)].add(sum);
				std::cout << "The process was completed successfully! Your new balance is: " << clients[found(name)].getBalance() << "$\n";
			}
		}
		void Withdraw () {
			std::string name, password;
			float sum;
			std::cout << "Name: ";
			std::cin >> name;
			std::cout << "Password: ";
			std::cin >> password;
			if (found(name) != 1 and clients[found(name)].getPassword() == password) {
				std::cout << "Give the sum you wish to withdraw: ";
				std::cin >> sum;
				try {
					clients[found(name)].withdraw(sum);
					std::cout << "The sum was successfuly withdrawn! Your new balance is: " << clients[found(name)].getBalance() << "$\n";
				} catch (std::logic_error) {
					std::cout << "You don't have enough balance for this action!\n";
				}
			}
			else std::cout << "This combination of name and password doesn't exist!\n"; 
		}
		void start () {     // the main method, that starts the whole system
			std::cout << "WELCOME TO OUR BANK SYSTEM!\n";
			std::cout << "-------------------------------------------------------------------------------------\n";
			while (!end) {    // starting menu
				int answer;
				std::cout <<"Choose one of the options below (1, 2, 3, 4, 5, 6):\n";
				std::cout << "1.Create new account\n";
				std::cout << "2.View account\n";
				std::cout << "3.Start a transaction\n";
				std::cout << "4.Store cash\n";
				std::cout << "5.Withdraw cash\n";
				std::cout << "6.Exit\n";
				std::cin >> answer;
				switch (answer) {
					case 1: New_account(); break;   
					case 2: View_account(); break;
					case 3: Transaction(); break; 
					case 4: Addmoney(); break;   
					case 5: Withdraw(); break;   // not done yet
					case 6: exit(); break;           
					default: std::cout << "That is not an option! Please try again.\n"; break;
				}
				std::cout << "\n";
			}
			std::cout << "Thank you for using our bank!\n";
		}
};

int main () {
	Bank b(100);  // max size=100
	b.start();
	
	return 0;
}
