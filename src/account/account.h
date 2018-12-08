#pragma once

#include "util/dollars.h"
#include <stdexcept>

class Account {

public:
	class Overdraft: public std::exception {
	
	public:
		Overdraft() {}
		
		~Overdraft() {}
		
		char const* what() const noexcept override {
			return "Account overdraft";
		}
	
	};
	
	virtual ~Account() {}
	
	virtual void deposit(Dollars amount) = 0;
	
	virtual void withdraw(Dollars amount) = 0;
	
	void transferTo(Account* account, Dollars amount) {
		if(amount < Dollars()) {
			Dollars positiveAmount = amount * -1;
			this->transferFrom(account, positiveAmount);
		}
		this->withdraw(amount);
		account->deposit(amount);
	}
	
	void transferFrom(Account* account, Dollars amount) {
		if(amount < Dollars()) {
			Dollars positiveAmount = amount * -1;
			this->transferTo(account, positiveAmount);
		}
		account->withdraw(amount);
		this->deposit(amount);
	}

};
