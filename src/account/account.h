#pragma once

#include "util/dollars.h"
#include <stdexcept>
#include <string>

class Account {

public:
	class Overdraw: public std::exception {
	
	public:
		Overdraw() {}
		
		~Overdraw() {}
		
		char const* what() const noexcept override {
			return "Account overdraw";
		}
	
	};
	
	Account(std::string const& name) {
		this->name = name;
	}
	
	virtual ~Account() {}
	
	virtual void deposit(Dollars const& credit) = 0;
	
	virtual void withdraw(Dollars const& debit) = 0;
	
	virtual Dollars getValue() const = 0;
	
	void transferTo(Account* account, Dollars const& amount) {
		if(amount < Dollars()) {
			Dollars positiveAmount = amount * -1;
			this->transferFrom(account, positiveAmount);
		}
		this->withdraw(amount);
		account->deposit(amount);
	}
	
	void transferFrom(Account* account, Dollars const& amount) {
		if(amount < Dollars()) {
			Dollars positiveAmount = amount * -1;
			this->transferTo(account, positiveAmount);
		}
		account->withdraw(amount);
		this->deposit(amount);
	}
	
	std::string const& getName() {
		return this->name;
	}
	
	std::string name;

};
