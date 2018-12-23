#pragma once

#include "account/account.h"

class CheckingAccount: public Account {

public:
	CheckingAccount(std::string const& name,
	                Dollars const& balance): Account(name) {
		if(balance < 0) {
			throw Account::Overdraw();
		}
		this->balance = balance;
	}
	
	void deposit(Dollars const& credit) override {
		this->balance += credit;
	}
	
	void withdraw(Dollars const& debit) override {
		if(debit > this->balance) {
			throw Account::Overdraw();
		}
		this->balance -= debit;
	}
	
	Dollars getValue() const override {
		return this->balance;
	}
	
	void dailyUpdate(AccountUpdate const& update) override {
		static_cast<void>(update);
	}

private:
	Dollars balance;

};
