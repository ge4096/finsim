#pragma once

#include "account/account.h"
#include "account/savings_account.h"
#include "test/unit_test.h"

class SavingsAccountTest: public UnitTest {

public:
	bool /* UnitTest:: */ run() override {
		START_TEST(SavingsAccountTest);
		RUN_TEST_CASE(initializeZero);
		RUN_TEST_CASE(initializePositive);
		RUN_TEST_CASE(initializeNegative);
		RUN_TEST_CASE(deposit);
		RUN_TEST_CASE(withdraw);
		RUN_TEST_CASE(accumulatesInterest);
		RUN_TEST_CASE(intraMonthDepositInterest);
		RUN_TEST_CASE(intraMonthOpenInterest);
		END_TEST(SavingsAccountTest);
	}

private:
	void initializeZero() {
		Dollars const zeroBalance = Dollars();
		std::string const accountName = "My Account";
		
		SavingsAccount account(accountName, zeroBalance);
		ASSERT_EQUAL(account.getValue(), zeroBalance);
		ASSERT_EQUAL(account.getName(), accountName);
	}
	
	void initializePositive() {
		Dollars const positiveBalance = Dollars(12, 34);
		std::string const accountName = "My Positive Account";
		
		SavingsAccount account(accountName, positiveBalance);
		ASSERT_EQUAL(account.getValue(), positiveBalance);
		ASSERT_EQUAL(account.getName(), accountName);
	}
	
	void initializeNegative() {
		Dollars const negativeBalance = Dollars(-12, -34);
		std::string const accountName = "My Negative Account";
		
		ASSERT_THROW(SavingsAccount account(accountName, negativeBalance),
		             Account::Overdraw);
	}
	
	void deposit() {
		Dollars const startingBalance = Dollars(12, 34);
		Dollars const depositAmount = Dollars(1, 23);
		
		SavingsAccount account("My Account", startingBalance);
		account.deposit(Dollars());
		ASSERT_EQUAL(account.getValue(), startingBalance);
		account.deposit(depositAmount);
		ASSERT_EQUAL(account.getValue(), startingBalance + depositAmount);
		account.deposit(Dollars());
		ASSERT_EQUAL(account.getValue(), startingBalance + depositAmount);
	}
	
	void withdraw() {
		Dollars const startingBalance = Dollars(12, 34);
		Dollars const withdrawAmount = Dollars(1, 23);
		
		SavingsAccount account("My Account", startingBalance);
		account.withdraw(Dollars());
		ASSERT_EQUAL(account.getValue(), startingBalance);
		account.withdraw(withdrawAmount);
		ASSERT_EQUAL(account.getValue(), startingBalance - withdrawAmount);
		account.withdraw(Dollars());
		ASSERT_EQUAL(account.getValue(), startingBalance - withdrawAmount);
		ASSERT_THROW(account.withdraw(startingBalance), Account::Overdraw);
	}
	
	void accumulatesInterest() {
		Dollars const startingBalance = Dollars(1234);
		AccountUpdate update(Date(1970, 1, 1));
		update.annualInflation = 1.02;
		
		SavingsAccount account("My Account", startingBalance);
		for(uint8_t day = 0; day < 31; ++day) {
			account.dailyUpdate(update);
			++update.currentDate;
		}
		ASSERT_GREATER(account.getValue(), startingBalance);
		ASSERT_LESS(account.getValue(), startingBalance * 1.01);
	}
	
	void intraMonthDepositInterest() {
		Dollars const startingBalance = Dollars(1234);
		Dollars const depositAmount = Dollars(1234);
		AccountUpdate update(Date(1970, 1, 1));
		update.annualInflation = 1.02;
		Date const midMonthDepositDate(1970, 1, 15);
		
		SavingsAccount account1("Deposit at start of month", startingBalance);
		SavingsAccount account2("Deposit during month", startingBalance);
		account1.deposit(depositAmount);
		for(uint8_t day = 0; day < 31; ++day) {
			account1.dailyUpdate(update);
			account2.dailyUpdate(update);
			++update.currentDate;
			if(update.currentDate == midMonthDepositDate) {
				account2.deposit(depositAmount);
			}
		}
		ASSERT_GREATER(account1.getValue(), account2.getValue());
	}
	
	void intraMonthOpenInterest() {
		Dollars const startingBalance = Dollars(1234);
		AccountUpdate update(Date(1970, 1, 1));
		update.annualInflation = 1.02;
		Date const midMonthOpenDate(1970, 1, 15);
		
		SavingsAccount account1("Opened at start of month", startingBalance);
		SavingsAccount account2("Opened during month", startingBalance);
		for(uint8_t day = 0; day < 31; ++day) {
			account1.dailyUpdate(update);
			if(update.currentDate >= midMonthOpenDate) {
				account2.dailyUpdate(update);
			}
			++update.currentDate;
		}
		ASSERT_GREATER(account1.getValue(), account2.getValue());
	}

};
