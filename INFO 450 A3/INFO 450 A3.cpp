// INFO 450 A3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cstdlib>
#include <iostream>
#include <math.h>

// define const
#define success 0
#define failure -1

using namespace std;

// class BankAccount
class BankAccount {
protected:// Define class member data,
	int id; // Account Number
	double interestRate; // Interest rate
	double balance; // Balance

public:
	// Define constructors
	BankAccount(int id) : id(id) {
		interestRate = 0.0;
	}

	/* Define member functions */

	// Method withdraw funds
	virtual int deposit(double deposit, int years = 0) = 0;

	// Method withdraw funds
	virtual int withdraw(double funds) = 0;

	// Method compute monthlyInterest
	virtual void monthlyInterest(int month) = 0;

	// method display Account
	void display() {
		cout << "Account Number: " << id << endl;
		cout << "Interest rate: " << interestRate << endl;
		cout << "Balance: " << balance << endl;
	}

};

// class SavingsAccount inheritance from  BankAccount
class SavingsAccount : public BankAccount {
private:
	// Method update interest rate
	void updateInterestRate() {
		interestRate = (balance < 10000) ? 0.01 : 0.02;
	}

public:
	// Define constructors
	SavingsAccount(int id) : BankAccount(id) {}

	int deposit(double deposit, int years = 0) {
		if (deposit < 0) return failure;

		balance = deposit;

		interestRate = (balance < 10000) ? 0.01 : 0.02;

		return success;
	}

	void monthlyInterest(int month) {
		if (month <= 0) return;

		// compute monthly rate
		double monthlyRate = pow(1 + interestRate, 1 / 12.0) - 1;


		balance += balance * monthlyRate;

		// update Interest Rate
		updateInterestRate();

		// compute next monthly interest rate 
		monthlyInterest(month - 1);
	}

	int withdraw(double funds) {
		// check funds go negative => failure returned
		if (funds <= 0) return failure;

		// update balance
		balance -= (funds + 2);

		updateInterestRate();

		// withdraw success
		return success;
	}
};

// class CheckingAccount inheritance from BankAccount
class CheckingAccount : public BankAccount {
public:
	// Define constructors
	CheckingAccount(int id) :BankAccount(id) {}

	int deposit(double deposit, int years = 0) {
		if (deposit < 0) return failure;

		balance = deposit;
		return success;
	}

	void monthlyInterest(int month) {}

	// method orderChecks
	void orderChecks() {
		if (balance < 15) {
			cout << endl << "You cannot afford to order checks with this balance" << endl;
			return;
		}

		// update balance
		balance -= 15;
	}

	int withdraw(double funds) {
		// check funds go negative => failure returned
		if (funds <= 0) return failure;

		// check account go negative => failure returned
		if (balance - funds < 0 || balance - funds - 5.0 < 0) {
			cout << endl << "You cannot afford to withdraw with this funds" << endl;
			return failure;
		}

		// update balance
		balance -= funds;
		if (balance < 500) balance -= 5.0;

		// withdraw success
		return success;
	}
};

// class CertificateDepositAccount inheritance from BankAccount
class CertificateDepositAccount : public BankAccount {
private:
	int month;
public:
	CertificateDepositAccount(int id) :BankAccount(id) {
		month = 0;
	}

	int deposit(double deposit, int years = 0) {
		if (deposit < 0) return failure;

		balance = deposit;

		interestRate = (years >= 5) ? 0.1 : 0.05;

		return success;
	}

	void monthlyInterest(int month) {
		if (month <= 0) return;

		// assign month
		if (this->month == 0) this->month = month;

		// compute monthly rate
		double monthlyRate = pow(1 + interestRate, 1 / 12.0) - 1;

		// update balance
		balance += balance * monthlyRate;

		// compute next monthly interest rate 
		monthlyInterest(month - 1);
	}

	int withdraw(double funds) {
		// check funds go negative => failure returned
		if (funds <= 0) return failure;

		// check account go negative => failure returned
		if ((month < 12 * 5 && balance - funds - 0.1*funds < 0) || balance - funds < 0) {
			cout << endl << "You cannot afford to withdraw with this funds" << endl;
			return failure;
		}

		// If funds are withdrawn before the term is over, a 10% fee on the entire pre-withdrawal balance is levied
		if (month < 12 * 5) balance -= (funds + 0.1*funds);
		else balance -= funds;

		// withdraw success
		return success;
	}
};

int main(int argc, char** argv) {
	// Create a new savings account and deposit $10k
	BankAccount * savingsAccount = new SavingsAccount(1);
	savingsAccount->deposit(10000);

	// Create a new checking account and deposit $600
	CheckingAccount * checkingAccount = new CheckingAccount(2);
	checkingAccount->deposit(600);

	// Create a new CD account with a 3 year term and deposit $10K
	BankAccount * CDAccount = new CertificateDepositAccount(3);
	CDAccount->deposit(10000, 3);

	// Get and display the attributes for each account
	cout << "==============================" << endl;
	cout << "======= Savings Account ======" << endl;
	savingsAccount->display();
	cout << endl;
	cout << "====== Checking Account ======" << endl;
	checkingAccount->display();
	cout << endl;
	cout << "= Certificate Deposit Account =" << endl;
	CDAccount->display();
	cout << "==============================" << endl;
	cout << endl;

	// Calculate monthly interest for the savings account
	savingsAccount->monthlyInterest(10);
	// Calculate the monthly interest for the CD account
	CDAccount->monthlyInterest(10);

	// Get and display the attributes for each account
	cout << "==============================" << endl;
	cout << "======= Savings Account ======" << endl;
	savingsAccount->display();
	cout << endl;
	cout << "====== Checking Account ======" << endl;
	checkingAccount->display();
	cout << endl;
	cout << "= Certificate Deposit Account =" << endl;
	CDAccount->display();
	cout << "==============================" << endl;
	cout << endl;

	// Order checks checkingAccount
	checkingAccount->orderChecks();
	// Withdraw $200 from checking
	checkingAccount->withdraw(200);
	// Withdraw $1000 from Savings
	savingsAccount->withdraw(1000);
	// Perform an early withdrawal of $2000 from the CD account
	CDAccount->withdraw(2000);

	// Get and display the attributes for each account
	cout << "==============================" << endl;
	cout << "======= Savings Account ======" << endl;
	savingsAccount->display();
	cout << endl;
	cout << "====== Checking Account ======" << endl;
	checkingAccount->display();
	cout << endl;
	cout << "= Certificate Deposit Account =" << endl;
	CDAccount->display();
	cout << "==============================" << endl;
	cout << endl;

	return 0;
}



