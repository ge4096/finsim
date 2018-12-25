#pragma once

#include "account/account.h"

class SavingsAccount: public Account {

public:
	SavingsAccount(std::string const& name,
	               Dollars const& balance): Account(name) {
		if(balance < 0) {
			throw Account::Overdraw();
		}
		this->balance = balance;
		this->interestBasis = Dollars();
		this->firstDayWasProcessed = false;
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
		while(!(this->firstDayWasProcessed) ||
		      (this->lastProcessedDay < update.currentDate)) {
			this->processDay(update);
		}
	}

private:
	void processDay(AccountUpdate const& update) {
		if(!(this->firstDayWasProcessed)) {
			this->lastProcessedDay = update.currentDate;
			this->firstDayWasProcessed = true;
		}
		else {
			++(this->lastProcessedDay);
		}
		Date const& dayToProcess = this->lastProcessedDay;
		
		double basisFraction = 1.0 / dayToProcess.getDaysInMonth();
		this->interestBasis += (this->balance * basisFraction);
		if(dayToProcess.isLastDayOfMonth()) {
			double interestRate = this->getMonthlyInterestRate(update);
			this->balance += (interestBasis * interestRate);
			this->interestBasis = Dollars();
		}
	}
	
	double getMonthlyInterestRate(AccountUpdate const& update) const {
		return ((update.annualInflation - 1.0) / 12);
	}
	
	Dollars balance;
	Dollars interestBasis;
	Date lastProcessedDay;
	bool firstDayWasProcessed;

};
